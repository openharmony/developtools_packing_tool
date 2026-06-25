/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ohos;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONException;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.serializer.SerializerFeature;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TimeZone;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

/**
 * SO deduplicator for Java packing tool.
 */
final class SODeduplicator {
    private static final String DEDUPLICATE_SO = "deduplicateSo";
    private static final String SO_SUFFIX = ".so";
    private static final String LIBS_DIR = "libs";
    private static final String MODULE_JSON = "module.json";
    private static final String CONFIG_JSON = "config.json";
    private static final String MODULE = "module";
    private static final String DISTRO = "distro";
    private static final String NAME = "name";
    private static final String TYPE = "type";
    private static final String MODULE_NAME = "moduleName";
    private static final String MODULE_TYPE = "moduleType";
    private static final String DEVICE_TYPES = "deviceTypes";
    private static final String DEVICE_TYPE = "deviceType";
    private static final String DELIVERY_WITH_INSTALL = "deliveryWithInstall";
    private static final String DISTRIBUTION_FILTER = "distributionFilter";
    private static final String DISTRO_FILTER = "distroFilter";
    private static final String REQUIRED_DEVICE_FEATURES = "requiredDeviceFeatures";
    private static final String COMPRESS_NATIVE_LIBS = "compressNativeLibs";
    private static final String EXTRACT_NATIVE_LIBS = "extractNativeLibs";
    private static final String LIB_ISOLATION = "libIsolation";
    private static final String ENTRY = "entry";
    private static final String REPORT_NAME = "so_dedup_report.json";
    private static final int EXACT_THRESHOLD = 20;
    private static final int BUFFER_SIZE = 8192;

    private static final Log LOG = new Log(SODeduplicator.class.toString());

    private SODeduplicator() {
    }

    private static void logDedupError(String cause) {
        LOG.error("[SO_DEDUP] " + PackingToolErrMsg.SO_DEDUPLICATION_FAILED.toString(cause));
    }

    static boolean isDeduplicateSoEnabled(String packInfoPath) throws BundleException {
        if (packInfoPath == null || packInfoPath.isEmpty()) {
            return false;
        }
        try {
            String content = readUtf8(Paths.get(packInfoPath));
            return isDeduplicateSoEnabledInContent(content);
        } catch (IOException | JSONException exception) {
            String errMsg = "Parse deduplicateSo in pack.info failed: " + exception.getMessage();
            logDedupError(errMsg);
            throw new BundleException(errMsg);
        }
    }

    static boolean isDeduplicateSoEnabledInContent(String packInfoContent) throws BundleException {
        if (packInfoContent == null || packInfoContent.isEmpty()) {
            return false;
        }
        try {
            JSONObject jsonObject = JSON.parseObject(packInfoContent);
            if (jsonObject == null) {
                return false;
            }
            if (jsonObject.containsKey(DEDUPLICATE_SO)) {
                return jsonObject.getBooleanValue(DEDUPLICATE_SO);
            }
            JSONObject summary = jsonObject.getJSONObject("summary");
            JSONObject app = summary == null ? jsonObject.getJSONObject("app") : summary.getJSONObject("app");
            return app != null && app.getBooleanValue(DEDUPLICATE_SO);
        } catch (JSONException exception) {
            String errMsg = "Parse deduplicateSo in pack.info failed: " + exception.getMessage();
            logDedupError(errMsg);
            throw new BundleException(errMsg);
        }
    }

    static List<String> deduplicateModules(List<String> modulePaths, boolean deduplicateSo, Path workDir,
                                           Path reportDir, int compressLevel) throws BundleException {
        if (!deduplicateSo) {
            LOG.info("[SO_DEDUP] SO deduplication skipped: disabled.");
            return modulePaths;
        }
        if (modulePaths == null || modulePaths.isEmpty()) {
            return modulePaths;
        }
        try {
            LOG.info("[SO_DEDUP] SO deduplication started.");
            Path modulesRoot = Files.createTempDirectory(workDir, "so_dedup_modules_");
            Path repackedRoot = Files.createTempDirectory(workDir, "so_dedup_repacked_");
            List<ModuleRecord> modules = extractModules(modulePaths, modulesRoot, repackedRoot);
            DedupResult result = deduplicateExtractedModules(modules);
            writeReport(result, reportDir);
            List<String> repacked = repackModules(modules, compressLevel);
            String reportPath = reportDir == null ? "" : reportDir.resolve(REPORT_NAME).toString();
            LOG.info("[SO_DEDUP] SO deduplication completed: removed=" + result.removedRecords.size()
                    + ", savedBytes=" + result.totalSavedSize + ", report=" + reportPath);
            return repacked;
        } catch (IOException exception) {
            String errMsg = "SO deduplication error: " + exception.getMessage();
            logDedupError(errMsg);
            throw new BundleException(errMsg);
        }
    }

    static void deduplicateExtractedModuleFiles(List<String> modulePaths, String packInfoPath, Path reportDir)
            throws BundleException {
        if (!isDeduplicateSoEnabled(packInfoPath)) {
            LOG.info("[SO_DEDUP] SO deduplication skipped: disabled.");
            return;
        }
        try {
            LOG.info("[SO_DEDUP] SO deduplication started.");
            List<ModuleRecord> modules = new ArrayList<>();
            for (String modulePath : modulePaths) {
                Path path = Paths.get(modulePath);
                ModuleConfig config = readModuleConfig(path);
                modules.add(new ModuleRecord(modulePath, path, path, config));
            }
            DedupResult result = deduplicateExtractedModules(modules);
            writeReport(result, reportDir);
            String reportPath = reportDir == null ? "" : reportDir.resolve(REPORT_NAME).toString();
            LOG.info("[SO_DEDUP] SO deduplication completed: removed=" + result.removedRecords.size()
                    + ", savedBytes=" + result.totalSavedSize + ", report=" + reportPath);
        } catch (IOException exception) {
            String errMsg = "SO deduplication error: " + exception.getMessage();
            logDedupError(errMsg);
            throw new BundleException(errMsg);
        }
    }

    private static List<ModuleRecord> extractModules(List<String> modulePaths, Path modulesRoot, Path repackedRoot)
            throws IOException, BundleException {
        List<ModuleRecord> modules = new ArrayList<>();
        for (String modulePath : modulePaths) {
            Path source = Paths.get(modulePath);
            String fileName = source.getFileName().toString();
            Path extractDir = modulesRoot.resolve(stripSuffix(fileName));
            Files.createDirectories(extractDir);
            unzip(source, extractDir);
            ModuleConfig config = readModuleConfig(extractDir);
            modules.add(new ModuleRecord(modulePath, extractDir, repackedRoot.resolve(fileName), config));
        }
        return modules;
    }

    private static DedupResult deduplicateExtractedModules(List<ModuleRecord> modules) throws IOException {
        List<ModuleRecord> eligibleModules = new ArrayList<>();
        for (ModuleRecord module : modules) {
            if (module.config.isEligible()) {
                eligibleModules.add(module);
            }
        }
        String strategy = eligibleModules.size() <= EXACT_THRESHOLD ? "exact" : "greedy";
        LOG.info("[SO_DEDUP] SO deduplication strategy: " + strategy
                + ", eligibleModules=" + eligibleModules.size() + ".");
        DedupResult result = new DedupResult(strategy);
        if (eligibleModules.isEmpty()) {
            return result;
        }
        Set<DeviceInstance> devices = calculateDevices(modules);
        if (devices.isEmpty()) {
            throw new IOException("No devices found for deduplication");
        }
        Map<DeviceInstance, Set<String>> mandatoryModules = calculateMandatoryModules(eligibleModules, devices);
        Map<String, ModuleConfig> moduleConfigs = new HashMap<>();
        for (ModuleRecord module : eligibleModules) {
            moduleConfigs.put(module.config.moduleName, module.config);
        }
        Map<String, List<SoRecord>> groups = collectDuplicateGroups(eligibleModules);
        for (List<SoRecord> group : groups.values()) {
            if (group.size() <= 1) {
                continue;
            }
            applyDedupForGroup(group, mandatoryModules, moduleConfigs, devices, result);
        }
        for (SoRecord removed : result.removedRecords) {
            Files.deleteIfExists(removed.path);
        }
        return result;
    }

    private static void applyDedupForGroup(List<SoRecord> group,
                                           Map<DeviceInstance, Set<String>> mandatoryModules,
                                           Map<String, ModuleConfig> moduleConfigs,
                                           Set<DeviceInstance> devices, DedupResult result) {
        List<SoRecord> remaining;
        if ("exact".equals(result.strategy)) {
            remaining = findExactSolution(group, mandatoryModules, moduleConfigs, devices);
        } else {
            remaining = findGreedySolution(group, mandatoryModules, moduleConfigs, devices);
        }
        Set<SoRecord> kept = new HashSet<>(remaining);
        for (SoRecord so : group) {
            if (!kept.contains(so)) {
                result.addRemoved(so);
            }
        }
        for (SoRecord so : remaining) {
            result.addKept(so);
        }
    }

    private static List<SoRecord> findExactSolution(List<SoRecord> group,
                                                     Map<DeviceInstance, Set<String>> mandatoryModules,
                                                     Map<String, ModuleConfig> moduleConfigs,
                                                     Set<DeviceInstance> devices) {
        if (group.size() > EXACT_THRESHOLD) {
            return findGreedySolution(group, mandatoryModules, moduleConfigs, devices);
        }
        List<SoRecord> best = new ArrayList<>(group);
        long bestKeptSize = sumSize(best);
        long subsetCount = 1L << group.size();
        for (long mask = 1; mask < subsetCount; mask++) {
            int keptCount = Long.bitCount(mask);
            if (keptCount > best.size()) {
                continue;
            }
            List<SoRecord> candidate = new ArrayList<>();
            for (int i = 0; i < group.size(); i++) {
                if ((mask & (1L << i)) != 0) {
                    candidate.add(group.get(i));
                }
            }
            if (!satisfiesAllDevices(group, candidate, mandatoryModules, moduleConfigs, devices)) {
                continue;
            }
            long keptSize = sumSize(candidate);
            if (candidate.size() < best.size() || candidate.size() == best.size() && keptSize < bestKeptSize) {
                best = candidate;
                bestKeptSize = keptSize;
            }
        }
        return best;
    }

    private static List<SoRecord> findGreedySolution(List<SoRecord> group,
                                                      Map<DeviceInstance, Set<String>> mandatoryModules,
                                                      Map<String, ModuleConfig> moduleConfigs,
                                                      Set<DeviceInstance> devices) {
        List<SoRecord> remaining = new ArrayList<>(group);
        List<SoRecord> candidates = new ArrayList<>(group);
        candidates.sort(Comparator.comparingLong((SoRecord so) -> so.size).reversed());
        boolean changed = true;
        while (changed) {
            changed = false;
            for (SoRecord so : candidates) {
                if (!remaining.contains(so)) {
                    continue;
                }
                List<SoRecord> trial = new ArrayList<>(remaining);
                trial.remove(so);
                if (satisfiesAllDevices(group, trial, mandatoryModules, moduleConfigs, devices)) {
                    remaining = trial;
                    changed = true;
                    break;
                }
            }
        }
        return remaining;
    }

    private static boolean satisfiesAllDevices(List<SoRecord> original, List<SoRecord> remaining,
                                                Map<DeviceInstance, Set<String>> mandatoryModules,
                                                Map<String, ModuleConfig> moduleConfigs,
                                                Set<DeviceInstance> devices) {
        if (remaining.isEmpty()) {
            return false;
        }
        Set<String> mandatoryUnion = new HashSet<>();
        for (Map.Entry<DeviceInstance, Set<String>> entry : mandatoryModules.entrySet()) {
            Set<String> modules = entry.getValue();
            mandatoryUnion.addAll(modules);
            if (containsSoFromModules(original, modules) && !containsSoFromModules(remaining, modules)) {
                return false;
            }
        }
        for (SoRecord so : original) {
            if (remaining.contains(so) || mandatoryUnion.contains(so.moduleName)) {
                continue;
            }
            ModuleConfig config = moduleConfigs.get(so.moduleName);
            if (config == null) {
                return false;
            }
            for (DeviceInstance device : devices) {
                if (supportsDevice(config, device)
                        && !containsSoFromModules(remaining, mandatoryModules.get(device))) {
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean containsSoFromModules(List<SoRecord> records, Set<String> modules) {
        if (modules == null || modules.isEmpty()) {
            return false;
        }
        for (SoRecord so : records) {
            if (modules.contains(so.moduleName)) {
                return true;
            }
        }
        return false;
    }

    private static long sumSize(List<SoRecord> records) {
        long size = 0;
        for (SoRecord record : records) {
            size += record.size;
        }
        return size;
    }

    private static Set<DeviceInstance> calculateDevices(List<ModuleRecord> modules) {
        Set<DeviceInstance> devices = new HashSet<>();
        for (ModuleRecord module : modules) {
            if (ENTRY.equals(module.config.moduleType)) {
                for (String deviceType : module.config.deviceTypes) {
                    devices.add(new DeviceInstance(deviceType, module.config.distributionFilter));
                }
            }
        }
        return devices;
    }

    private static Map<DeviceInstance, Set<String>> calculateMandatoryModules(
            List<ModuleRecord> modules, Set<DeviceInstance> devices) {
        Map<DeviceInstance, Set<String>> mandatoryModules = new HashMap<>();
        for (DeviceInstance device : devices) {
            Set<String> moduleNames = new HashSet<>();
            for (ModuleRecord module : modules) {
                ModuleConfig config = module.config;
                if (supportsDevice(config, device) && config.deliveryWithInstall
                        && config.distributionFilter.isEmpty() && config.requiredDeviceFeatures.isEmpty()) {
                    moduleNames.add(config.moduleName);
                }
            }
            mandatoryModules.put(device, moduleNames);
        }
        return mandatoryModules;
    }

    private static boolean supportsDevice(ModuleConfig config, DeviceInstance device) {
        if (!config.deviceTypes.contains(device.type)) {
            return false;
        }
        return config.distributionFilter.isEmpty() || config.distributionFilter.equals(device.distributionFilter);
    }

    private static Map<String, List<SoRecord>> collectDuplicateGroups(List<ModuleRecord> modules) throws IOException {
        Map<String, List<SoRecord>> groups = new HashMap<>();
        for (ModuleRecord module : modules) {
            Path libsPath = module.extractDir.resolve(LIBS_DIR);
            if (!Files.exists(libsPath)) {
                continue;
            }
            collectSoFiles(groups, module, libsPath);
        }
        return groups;
    }

    private static void collectSoFiles(final Map<String, List<SoRecord>> groups, final ModuleRecord module,
                                       Path libsPath) throws IOException {
        Files.walkFileTree(libsPath, new SimpleFileVisitor<Path>() {
            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                if (file.getFileName().toString().endsWith(SO_SUFFIX)) {
                    addSoRecord(groups, module, file);
                }
                return FileVisitResult.CONTINUE;
            }
        });
    }

    private static void addSoRecord(Map<String, List<SoRecord>> groups, ModuleRecord module, Path path)
            throws IOException {
        String md5;
        try {
            md5 = calculateMd5(path);
        } catch (NoSuchAlgorithmException exception) {
            throw new IOException("Calculate SO MD5 failed: " + path + ", " + exception.getMessage(), exception);
        }
        Path relative = module.extractDir.relativize(path);
        SoRecord so = new SoRecord(module.config.moduleName, relative.toString().replace('\\', '/'), path,
                Files.size(path));
        groups.computeIfAbsent(so.relativePath + '\n' + md5, key -> new ArrayList<>()).add(so);
    }

    private static ModuleConfig readModuleConfig(Path moduleDir) throws IOException {
        Path moduleJson = moduleDir.resolve(MODULE_JSON);
        if (Files.exists(moduleJson)) {
            return readStageModuleConfig(moduleJson);
        }
        Path configJson = moduleDir.resolve(CONFIG_JSON);
        if (Files.exists(configJson)) {
            return readFaModuleConfig(configJson);
        }
        ModuleConfig config = new ModuleConfig();
        config.moduleName = moduleDir.getFileName().toString();
        return config;
    }

    private static ModuleConfig readStageModuleConfig(Path moduleJson) throws IOException {
        JSONObject root = JSON.parseObject(readUtf8(moduleJson));
        JSONObject module = root == null ? null : root.getJSONObject(MODULE);
        ModuleConfig config = new ModuleConfig();
        if (module == null) {
            return config;
        }
        config.moduleName = getString(module, NAME, moduleJson.getParent().getFileName().toString());
        config.moduleType = getString(module, TYPE, "");
        config.deviceTypes = getDeviceTypes(module.getJSONArray(DEVICE_TYPES));
        config.deliveryWithInstall = module.getBooleanValue(DELIVERY_WITH_INSTALL);
        config.distributionFilter = parseDistributionFilter(module);
        Object requiredFeatures = module.get(REQUIRED_DEVICE_FEATURES);
        if (requiredFeatures instanceof JSONObject && !((JSONObject) requiredFeatures).isEmpty()
                || requiredFeatures instanceof JSONArray && !((JSONArray) requiredFeatures).isEmpty()) {
            config.requiredDeviceFeatures.add(REQUIRED_DEVICE_FEATURES);
        }
        config.compressNativeLibs = module.getBooleanValue(COMPRESS_NATIVE_LIBS);
        config.extractNativeLibs = !module.containsKey(EXTRACT_NATIVE_LIBS)
                || module.getBooleanValue(EXTRACT_NATIVE_LIBS);
        config.libIsolation = module.getBooleanValue(LIB_ISOLATION);
        return config;
    }

    private static ModuleConfig readFaModuleConfig(Path configJson) throws IOException {
        JSONObject root = JSON.parseObject(readUtf8(configJson));
        JSONObject module = root == null ? null : root.getJSONObject(MODULE);
        JSONObject distro = module == null ? null : module.getJSONObject(DISTRO);
        ModuleConfig config = new ModuleConfig();
        if (module == null) {
            return config;
        }
        config.moduleName = distro == null ? getString(module, NAME, configJson.getParent().getFileName().toString())
                : getString(distro, MODULE_NAME, configJson.getParent().getFileName().toString());
        config.moduleType = distro == null ? getString(module, TYPE, "") : getString(distro, MODULE_TYPE, "");
        config.deviceTypes = getDeviceTypes(module.getJSONArray(DEVICE_TYPE));
        config.deliveryWithInstall = distro != null && distro.getBooleanValue(DELIVERY_WITH_INSTALL);
        config.compressNativeLibs = module.getBooleanValue(COMPRESS_NATIVE_LIBS);
        config.extractNativeLibs = !module.containsKey(EXTRACT_NATIVE_LIBS)
                || module.getBooleanValue(EXTRACT_NATIVE_LIBS);
        return config;
    }

    private static String parseDistributionFilter(JSONObject module) {
        JSONObject filter = module.getJSONObject(DISTRIBUTION_FILTER);
        if (filter == null) {
            filter = module.getJSONObject(DISTRO_FILTER);
        }
        return filter == null || filter.isEmpty() ? "" : filter.toJSONString();
    }

    private static List<String> repackModules(List<ModuleRecord> modules, int compressLevel) throws IOException {
        List<String> repacked = new ArrayList<>();
        for (ModuleRecord module : modules) {
            zipDirectory(module.extractDir, module.repackedPath, compressLevel);
            repacked.add(module.repackedPath.toString());
        }
        return repacked;
    }

    private static void writeReport(DedupResult result, Path reportDir) throws IOException {
        if (reportDir == null) {
            throw new IOException("SO deduplication report directory is empty");
        }
        Files.createDirectories(reportDir);
        JSONObject report = new JSONObject(true);
        report.put("version", "1.0");
        report.put("timestamp", timestamp());
        report.put("strategy", result.strategy);
        report.put("totalSavedSize", result.totalSavedSize);
        JSONObject modules = new JSONObject(true);
        for (Map.Entry<String, ModuleReport> entry : result.modules.entrySet()) {
            JSONObject module = new JSONObject(true);
            module.put("kept", entry.getValue().kept);
            module.put("removed", entry.getValue().removed);
            modules.put(entry.getKey(), module);
        }
        report.put("modules", modules);
        Files.write(reportDir.resolve(REPORT_NAME),
                JSON.toJSONString(report, SerializerFeature.PrettyFormat).getBytes(StandardCharsets.UTF_8));
    }

    private static void unzip(Path source, Path dest) throws IOException {
        try (ZipFile zipFile = new ZipFile(source.toFile())) {
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                Path out = dest.resolve(entry.getName()).normalize();
                if (!out.startsWith(dest)) {
                    throw new IOException("Invalid zip entry path: " + entry.getName());
                }
                if (entry.isDirectory()) {
                    Files.createDirectories(out);
                    continue;
                }
                Files.createDirectories(out.getParent());
                try (InputStream input = new BufferedInputStream(zipFile.getInputStream(entry));
                     OutputStream output = new BufferedOutputStream(Files.newOutputStream(out))) {
                    copy(input, output);
                }
            }
        }
    }

    private static void zipDirectory(Path sourceDir, Path targetZip, int compressLevel) throws IOException {
        Files.createDirectories(targetZip.getParent());
        try (ZipOutputStream zipOutput = new ZipOutputStream(
                new CheckedOutputStream(Files.newOutputStream(targetZip), new CRC32()))) {
            zipOutput.setLevel(compressLevel);
            Files.walkFileTree(sourceDir, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                    String entryName = sourceDir.relativize(file).toString().replace('\\', '/');
                    zipOutput.putNextEntry(new ZipEntry(entryName));
                    Files.copy(file, zipOutput);
                    zipOutput.closeEntry();
                    return FileVisitResult.CONTINUE;
                }
            });
        }
    }

    private static String calculateMd5(Path path) throws IOException, NoSuchAlgorithmException {
        MessageDigest digest = MessageDigest.getInstance("MD5");
        try (InputStream input = Files.newInputStream(path)) {
            byte[] buffer = new byte[BUFFER_SIZE];
            int len;
            while ((len = input.read(buffer)) != -1) {
                digest.update(buffer, 0, len);
            }
        }
        StringBuilder builder = new StringBuilder();
        for (byte value : digest.digest()) {
            builder.append(String.format("%02x", value & 0xff));
        }
        return builder.toString();
    }

    private static void copy(InputStream input, OutputStream output) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        int len;
        while ((len = input.read(buffer)) != -1) {
            output.write(buffer, 0, len);
        }
    }

    private static String readUtf8(Path path) throws IOException {
        return new String(Files.readAllBytes(path), StandardCharsets.UTF_8);
    }

    private static String timestamp() {
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'", Locale.ENGLISH);
        format.setTimeZone(TimeZone.getTimeZone("UTC"));
        return format.format(new Date());
    }

    private static String stripSuffix(String fileName) {
        int index = fileName.lastIndexOf('.');
        return index > 0 ? fileName.substring(0, index) : fileName;
    }

    private static String getString(JSONObject object, String key, String defaultValue) {
        String value = object.getString(key);
        return value == null ? defaultValue : value;
    }

    private static List<String> getStringList(JSONArray array) {
        List<String> values = new ArrayList<>();
        if (array == null) {
            return values;
        }
        for (Object item : array) {
            if (item != null) {
                values.add(String.valueOf(item));
            }
        }
        return values;
    }

    private static List<String> getDeviceTypes(JSONArray array) {
        List<String> rawValues = getStringList(array);
        List<String> values = new ArrayList<>();
        for (String value : rawValues) {
            String normalized = normalizeDeviceType(value);
            if (!values.contains(normalized)) {
                values.add(normalized);
            }
        }
        if (values.isEmpty()) {
            values.add("phone");
            values.add("tablet");
            values.add("2in1");
            values.add("wearable");
            values.add("tv");
            values.add("car");
        }
        return values;
    }

    private static String normalizeDeviceType(String deviceType) {
        return "default".equals(deviceType) ? "phone" : deviceType;
    }

    private static final class ModuleConfig {
        private String moduleName = "";
        private String moduleType = "";
        private List<String> deviceTypes = new ArrayList<>();
        private boolean deliveryWithInstall = false;
        private String distributionFilter = "";
        private List<String> requiredDeviceFeatures = new ArrayList<>();
        private boolean compressNativeLibs = false;
        private boolean extractNativeLibs = false;
        private boolean libIsolation = false;

        private boolean isEligible() {
            return (compressNativeLibs || extractNativeLibs) && !libIsolation;
        }
    }

    private static final class ModuleRecord {
        private final String sourcePath;
        private final Path extractDir;
        private final Path repackedPath;
        private final ModuleConfig config;

        private ModuleRecord(String sourcePath, Path extractDir, Path repackedPath, ModuleConfig config) {
            this.sourcePath = sourcePath;
            this.extractDir = extractDir;
            this.repackedPath = repackedPath;
            this.config = config;
        }
    }

    private static final class DeviceInstance {
        private final String type;
        private final String distributionFilter;

        private DeviceInstance(String type, String distributionFilter) {
            this.type = type;
            this.distributionFilter = distributionFilter;
        }

        @Override
        public boolean equals(Object other) {
            if (this == other) {
                return true;
            }
            if (!(other instanceof DeviceInstance)) {
                return false;
            }
            DeviceInstance device = (DeviceInstance) other;
            return type.equals(device.type) && distributionFilter.equals(device.distributionFilter);
        }

        @Override
        public int hashCode() {
            return 31 * type.hashCode() + distributionFilter.hashCode();
        }
    }

    private static final class SoRecord {
        private final String moduleName;
        private final String relativePath;
        private final Path path;
        private final long size;

        private SoRecord(String moduleName, String relativePath, Path path, long size) {
            this.moduleName = moduleName;
            this.relativePath = relativePath;
            this.path = path;
            this.size = size;
        }
    }

    private static final class ModuleReport {
        private final List<String> kept = new ArrayList<>();
        private final List<String> removed = new ArrayList<>();
    }

    private static final class DedupResult {
        private final String strategy;
        private final Map<String, ModuleReport> modules = new LinkedHashMap<>();
        private final List<SoRecord> removedRecords = new ArrayList<>();
        private long totalSavedSize = 0;

        private DedupResult(String strategy) {
            this.strategy = strategy;
        }

        private void addKept(SoRecord so) {
            modules.computeIfAbsent(so.moduleName, key -> new ModuleReport()).kept.add(so.relativePath);
        }

        private void addRemoved(SoRecord so) {
            modules.computeIfAbsent(so.moduleName, key -> new ModuleReport()).removed.add(so.relativePath);
            removedRecords.add(so);
            totalSavedSize += so.size;
        }
    }
}
