/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import org.apache.commons.compress.archivers.zip.DefaultBackingStoreSupplier;
import org.apache.commons.compress.archivers.zip.ParallelScatterZipCreator;
import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream;
import org.apache.commons.compress.archivers.zip.ZipFile;
import org.apache.commons.compress.parallel.InputStreamSupplier;
import org.apache.commons.compress.utils.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.stream.Stream;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;

/**
 * PackageUtil
 *
 * @since 2024-06-18
 */
public class PackageUtil {
    private static final Log LOG = new Log("");

    /**
     * get the package name list from pack.info
     *
     * @param path the path dir or hsp contains pack.info, or the path of pack.info
     * @return the list of package name
     */
    public static List<String> getPackageNameFromPath(Path path) {
        List<String> list = new ArrayList<>();
        if (!Files.exists(path)) {
            LOG.warning("getPackageNameFromPath path not exists: " + path);
            return list;
        }
        if (Files.isRegularFile(path)) {
            String filename = path.getFileName().toString();
            if (filename.endsWith(Constants.HSP_SUFFIX)) {
                // .hsp: return filename
                list.add(filename.substring(0, filename.lastIndexOf(Constants.HSP_SUFFIX)));
                return list;
            }
        }
        String content = getPackInfoContentFromPath(path);
        if (content == null) {
            return list;
        }
        return getPackageNameFromPackInfo(content);
    }

    /**
     * get the package name list from module.json
     *
     * @param path the path dir or hsp contains module.json, or the path of module.json
     * @return the bundleType
     */
    public static String getBundleTypeFromPath(Path path) {
        if (!Files.exists(path)) {
            LOG.warning("getBundleTypeFromPath path not exists: " + path);
            return "";
        }
        String content = getModuleJsonContentFromPath(path);
        if (content == null) {
            return "";
        }
        return getBundleTypeFromModuleJson(content);
    }

    /**
     * get the package name list from pack.info
     *
     * @param packInfoContent the content of pack.info
     * @return the list of package name
     */
    public static List<String> getPackageNameFromPackInfo(String packInfoContent) {
        List<String> packages = new ArrayList<>();
        try {
            JSONObject jsonObject = JSON.parseObject(packInfoContent, JSONObject.class);
            if (jsonObject == null) {
                LOG.warning("getPackagesFromPackInfo failed, json format invalid.");
                return packages;
            }
            JSONArray jsonArray = jsonObject.getJSONArray(Constants.PACKAGES);
            if (jsonArray == null) {
                LOG.warning("getPackagesFromPackInfo failed, json format invalid.");
                return packages;
            }
            for (int i = 0; i < jsonArray.size(); i++) {
                JSONObject object = jsonArray.getJSONObject(i);
                String packageName = object.getString(Constants.MODULE_NAME);
                if (packageName != null) {
                    packages.add(packageName);
                }
            }
            return packages;
        } catch (JSONException ex) {
            LOG.warning("getPackagesFromPackInfo err: " + ex.getMessage());
            return new ArrayList<>();
        }
    }

    private static String getBundleTypeFromModuleJson(String moduleJsonContent) {
        try {
            JSONObject jsonObject = JSON.parseObject(moduleJsonContent, JSONObject.class);
            if (jsonObject == null) {
                LOG.warning("getBundleTypeFromModuleJson failed, parse json is null.");
                return "";
            }
            JSONObject appObject = jsonObject.getJSONObject(Constants.APP);
            if (appObject == null) {
                LOG.warning("getBundleTypeFromModuleJson failed, [app] is null.");
                return "";
            }
            String bundleType = appObject.getString(Constants.BUNDLE_TYPE);
            return bundleType != null ? bundleType : Constants.APP;
        } catch (JSONException ex) {
            LOG.warning("getBundleTypeFromModuleJson failed: " + ex.getMessage());
        }
        return "";
    }

    private static ModuleJsonInfo parseModuleJsonInfo(Path moduleJson) {
        ModuleJsonInfo moduleJsonInfo = new ModuleJsonInfo();
        try (FileInputStream input = new FileInputStream(moduleJson.toFile())) {
            JSONObject jsonObject = JSON.parseObject(input, JSONObject.class);
            if (jsonObject == null) {
                LOG.warning("parseModuleJsonInfo failed, json format invalid.");
                return moduleJsonInfo;
            }
            JSONObject appObject = jsonObject.getJSONObject(Constants.APP);
            if (appObject == null) {
                LOG.warning("parseModuleJsonInfo failed, not found [app]");
                return moduleJsonInfo;
            }
            JSONObject moduleObject = jsonObject.getJSONObject(Constants.MODULE);
            if (moduleObject == null) {
                LOG.warning("parseModuleJsonInfo failed, not found [module]");
                return moduleJsonInfo;
            }
            moduleJsonInfo.setGenerateBuildHash(
                    appObject.getBooleanValue(Constants.GENERATE_BUILD_HASH) ||
                    moduleObject.getBooleanValue(Constants.GENERATE_BUILD_HASH));
            moduleJsonInfo.setCompressNativeLibs(
                    moduleObject.getBooleanValue(Constants.COMPRESS_NATIVE_LIBS));
            String moduleType = moduleObject.getString(Constants.MODULE_TYPE);
            moduleJsonInfo.setModuleType(moduleType != null ? moduleType : "");
            String moduleName = moduleObject.getString(Constants.MODULE_NAME);
            moduleJsonInfo.setModuleName(moduleName != null ? moduleName : "");
            return moduleJsonInfo;
        } catch (IOException ex) {
            LOG.warning("parseModuleJsonInfo err: " + ex.getMessage());
        }
        return moduleJsonInfo;
    }

    private static String getPackInfoContentFromPath(Path path) {
        try {
            if (Files.isRegularFile(path)) {
                String filename = path.getFileName().toString();
                if (filename.equals(Constants.FILE_PACK_INFO)) {
                    return new String(Files.readAllBytes(path));
                } else if (filename.endsWith(Constants.HSP_SUFFIX)) {
                    return getZipEntryContent(path, Constants.FILE_PACK_INFO);
                }
            } else {
                return new String(Files.readAllBytes(path.resolve(Constants.FILE_PACK_INFO)));
            }
        } catch (IOException ex) {
            LOG.warning("getPackInfoContentFromPath err: " + ex.getMessage());
        }
        return null;
    }

    private static String getModuleJsonContentFromPath(Path path) {
        try {
            if (Files.isRegularFile(path)) {
                String filename = path.getFileName().toString();
                if (filename.equals(Constants.FILE_MODULE_JSON)) {
                    return new String(Files.readAllBytes(path));
                } else if (filename.endsWith(Constants.HSP_SUFFIX)) {
                    return getZipEntryContent(path, Constants.FILE_MODULE_JSON);
                }
            } else {
                return new String(Files.readAllBytes(path.resolve(Constants.FILE_MODULE_JSON)));
            }
        } catch (IOException ex) {
            LOG.warning("getPackInfoContentFromPath err: " + ex.getMessage());
        }
        return null;
    }

    private static String getZipEntryContent(Path zipPath, String entryName) {
        if (!Files.isRegularFile(zipPath)) {
            return null;
        }
        try (ZipFile zipFile = new ZipFile(zipPath.toFile())) {
            ZipArchiveEntry zipEntry = zipFile.getEntry(entryName);
            if (zipEntry != null) {
                ByteArrayOutputStream output = new ByteArrayOutputStream();
                IOUtils.copy(zipFile.getInputStream(zipEntry), output);
                return output.toString();
            }
        } catch (IOException ex) {
            LOG.warning("getZipEntryContent err: " + ex.getMessage());
        }
        return null;
    }

    /**
     * pack hap or hsp
     *
     * @param inputPath     input hap/hsp path
     * @param appPackInfo   app scope pack.info
     * @param outPath       output dir
     * @param compressLevel compress level
     * @return the hap/hsp path
     */
    public static Path pack(Path inputPath, Path appPackInfo, Path outPath, int compressLevel)
            throws BundleException, IOException {
        if (!Files.exists(inputPath)) {
            throw new BundleException("pack err, input path not exists.");
        }
        if (!Files.exists(appPackInfo)) {
            throw new BundleException("pack err, app pack.info not exists.");
        }
        if (Files.isDirectory(inputPath)) {
            return packDir(inputPath, appPackInfo, outPath, compressLevel);
        } else if (Files.isRegularFile(inputPath) &&
                inputPath.getFileName().toString().endsWith(Constants.HSP_SUFFIX)) {
            return repackHsp(inputPath, appPackInfo, outPath, compressLevel);
        }
        throw new BundleException("pack err, not support: " + inputPath);
    }

    /**
     * rm dir
     *
     * @param dir input path to rm
     * @return true if rm dir success
     */
    public static boolean rmdir(Path dir) {
        try {
            Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                    Files.delete(file);
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public FileVisitResult postVisitDirectory(Path dir, IOException ex) throws IOException {
                    Files.delete(dir);
                    return FileVisitResult.CONTINUE;
                }
            });
            return true;
        } catch (IOException ex) {
            LOG.warning("rmdir err: " + ex.getMessage());
        }
        return false;
    }

    private static String hash(Path path) {
        try (Stream<Path> pathStream = Files.walk(path)) {
            MessageDigest md = MessageDigest.getInstance(Constants.SHA_256);
            pathStream.filter(Files::isRegularFile)
                    .sorted()
                    .forEach(file -> {
                        try {
                            byte[] fileBytes = Files.readAllBytes(file);
                            md.update(fileBytes);
                        } catch (IOException e) {
                            LOG.warning("calc hash err: " + e.getMessage());
                        }
                    });
            byte[] hashBytes = md.digest();
            StringBuilder sb = new StringBuilder();
            for (byte b : hashBytes) {
                sb.append(String.format("%02x", b));
            }
            return sb.toString();
        } catch (NoSuchAlgorithmException | IOException e) {
            LOG.warning("calc hash err: " + e.getMessage());
        }
        return "";
    }

    private static Path repackHsp(Path inputPath, Path appPackInfo, Path outPath, int compressLevel)
            throws BundleException, IOException {
        Path outHsp = Files.createFile(outPath.resolve(inputPath.getFileName()));
        try (ZipFile hspFile = new ZipFile(inputPath.toFile());
             ZipArchiveOutputStream zipOut = new ZipArchiveOutputStream(
                     new CheckedOutputStream(Files.newOutputStream(outHsp), new CRC32()))) {
            int cores = Runtime.getRuntime().availableProcessors();
            ThreadPoolExecutor executorService = new ThreadPoolExecutor(cores, cores, 60L,
                    TimeUnit.SECONDS, new LinkedBlockingQueue<>());
            ParallelScatterZipCreator zipCreator = new ParallelScatterZipCreator(
                    executorService, new DefaultBackingStoreSupplier(null), compressLevel);
            // pack.info
            pathToZipEntry(appPackInfo, Constants.NULL_DIR, zipCreator, false);
            // others
            Enumeration<ZipArchiveEntry> entries = hspFile.getEntries();
            while (entries.hasMoreElements()) {
                ZipArchiveEntry zipEntry = entries.nextElement();
                if (Constants.FILE_PACK_INFO.equals(zipEntry.getName())) {
                    continue;
                }
                InputStreamSupplier supplier = () -> {
                    try {
                        return hspFile.getInputStream(zipEntry);
                    } catch (IOException e) {
                        LOG.error("addArchiveEntry err: " + e.getMessage());
                        return null;
                    }
                };
                zipCreator.addArchiveEntry(zipEntry, supplier);
            }
            zipCreator.writeTo(zipOut);
        } catch (InterruptedException | ExecutionException e) {
            String errMsg = "repackHsp err: " + e.getMessage();
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        return outHsp;
    }

    private static Path packDir(Path inputPath, Path appPackInfo, Path outPath, int compressLevel)
            throws BundleException, IOException {
        List<String> packageNames = getPackageNameFromPath(inputPath.resolve(Constants.FILE_PACK_INFO));
        if (packageNames.size() != 1) {
            throw new BundleException("pack err, pack.info format err");
        }
        ModuleJsonInfo moduleJsonInfo = parseModuleJsonInfo(inputPath.resolve(Constants.FILE_MODULE_JSON));
        String pkgName = packageNames.get(0);
        String suffix = moduleJsonInfo.isShared() ? Constants.HSP_SUFFIX : Constants.HAP_SUFFIX;
        Path outHap = Files.createFile(outPath.resolve(pkgName + suffix));
        File[] files = inputPath.toFile().listFiles();
        if (files == null || files.length == 0) {
            throw new BundleException("pack err, dir is empty");
        }
        try (ZipArchiveOutputStream zipOut = new ZipArchiveOutputStream(
                new CheckedOutputStream(Files.newOutputStream(outHap), new CRC32()))) {
            int cores = Runtime.getRuntime().availableProcessors();
            ThreadPoolExecutor executorService = new ThreadPoolExecutor(cores, cores, 60L,
                    TimeUnit.SECONDS, new LinkedBlockingQueue<>());
            ParallelScatterZipCreator zipCreator = new ParallelScatterZipCreator(
                    executorService, new DefaultBackingStoreSupplier(null), compressLevel);
            // pack.info
            pathToZipEntry(appPackInfo, Constants.NULL_DIR, zipCreator, false);
            // module.json generateBuildHash
            if (moduleJsonInfo.isGenerateBuildHash()) {
                genBuildHash(inputPath, zipCreator);
            }
            // others
            filesToZipEntry(files, zipCreator, moduleJsonInfo.isGenerateBuildHash(),
                    moduleJsonInfo.isCompressNativeLibs());
            // write to zip
            zipCreator.writeTo(zipOut);
        } catch (InterruptedException | ExecutionException e) {
            String errMsg = "packDir err: " + e.getMessage();
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        return outHap;
    }

    private static void filesToZipEntry(File[] files, ParallelScatterZipCreator zipCreator,
                                        boolean genHash, boolean compress) throws BundleException {
        for (File file : files) {
            if (file.isFile() && !file.getName().equals(Constants.FILE_PACK_INFO)) {
                if (genHash && file.getName().equals(Constants.FILE_MODULE_JSON)) {
                    continue;
                }
                pathToZipEntry(file.toPath(), Constants.NULL_DIR, zipCreator, false);
            } else if (file.isDirectory()) {
                if (file.getName().equals(Constants.LIBS_DIR)) {
                    pathToZipEntry(file.toPath(), Constants.LIBS_DIR + Constants.SLASH, zipCreator, compress);
                } else {
                    pathToZipEntry(file.toPath(), file.getName() + Constants.SLASH, zipCreator, false);
                }
            }
        }
    }

    private static void genBuildHash(Path path, ParallelScatterZipCreator zipCreator) {
        String hash = hash(path);
        if (hash.isEmpty()) {
            return;
        }
        Path moduleJson = path.resolve(Constants.FILE_MODULE_JSON);
        if (!Files.exists(moduleJson)) {
            LOG.warning("module.json not found: " + path);
            return;
        }
        try (FileInputStream input = new FileInputStream(moduleJson.toFile())) {
            JSONObject jsonObject = JSON.parseObject(input, JSONObject.class);
            if (jsonObject == null) {
                LOG.warning("generateBuildHash: parse json is null.");
                return;
            }
            JSONObject moduleObject = jsonObject.getJSONObject(Constants.MODULE);
            if (moduleObject == null) {
                LOG.warning("generateBuildHash: parse json[module] is null.");
                return;
            }
            moduleObject.put(Constants.BUILD_HASH, hash);
            byte[] data = JSON.toJSONBytes(jsonObject, SerializerFeature.WriteMapNullValue,
                    SerializerFeature.WriteDateUseDateFormat, SerializerFeature.SortField);
            ZipArchiveEntry zipEntry = new ZipArchiveEntry(Constants.FILE_MODULE_JSON);
            zipEntry.setMethod(ZipArchiveEntry.STORED);
            InputStreamSupplier supplier = () -> new ByteArrayInputStream(data);
            zipCreator.addArchiveEntry(zipEntry, supplier);
        } catch (IOException ex) {
            LOG.warning("genBuildHash err: " + ex.getMessage());
        }
    }

    private static void pathToZipEntry(Path path, String baseDir, ParallelScatterZipCreator zipCreator,
                                       boolean compress) throws BundleException {
        try {
            File file = path.toFile();
            if (file.isDirectory()) {
                File[] files = file.listFiles();
                if (files == null) {
                    return;
                }
                for (File f : files) {
                    addArchiveEntry(f, baseDir, zipCreator, compress);
                }
            } else {
                addArchiveEntry(file, baseDir, zipCreator, compress);
            }
        } catch (IOException e) {
            String errMsg = "pathToZip err: " + e.getMessage();
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
    }

    private static void addArchiveEntry(File file, String baseDir, ParallelScatterZipCreator zipCreator,
                                        boolean compress)
            throws IOException {
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files == null) {
                LOG.error("listFiles null: " + file.getName());
                return;
            }
            if (files.length == 0) {
                String entryName = (baseDir + file.getName() + File.separator)
                        .replace(File.separator, Constants.SLASH);
                ZipArchiveEntry zipEntry = new ZipArchiveEntry(entryName);
                zipEntry.setMethod(ZipArchiveEntry.STORED);
                zipEntry.setSize(0);
                zipEntry.setCrc(0);
                InputStreamSupplier supplier = () -> new ByteArrayInputStream(new byte[0]);
                zipCreator.addArchiveEntry(zipEntry, supplier);
            }
            for (File f : files) {
                addArchiveEntry(f, baseDir + file.getName() + File.separator, zipCreator, compress);
            }
        } else {
            String entryName = (baseDir + file.getName()).replace(File.separator, Constants.SLASH);
            ZipArchiveEntry zipEntry = new ZipArchiveEntry(entryName);
            if (compress) {
                zipEntry.setMethod(ZipArchiveEntry.DEFLATED);
            } else {
                zipEntry.setMethod(ZipArchiveEntry.STORED);
            }
            InputStreamSupplier supplier = () -> {
                try {
                    return getInputStream(entryName, file);
                } catch (IOException e) {
                    LOG.error("addArchiveEntry err: " + e.getMessage());
                    return null;
                }
            };
            zipCreator.addArchiveEntry(zipEntry, supplier);
        }
    }

    private static InputStream getInputStream(String entryName, File file) throws IOException {
        if (!entryName.contains(Constants.RAW_FILE_PATH) && !entryName.contains(Constants.RES_FILE_PATH) &&
                file.getName().toLowerCase(Locale.ENGLISH).endsWith(Constants.JSON_SUFFIX)) {
            try {
                Object jsonObject = JSON.parse(Files.readAllBytes(file.toPath()));
                byte[] data = JSON.toJSONBytes(jsonObject,
                        SerializerFeature.WriteMapNullValue, SerializerFeature.WriteDateUseDateFormat);
                return new ByteArrayInputStream(data);
            } catch (Exception ex) {
                LOG.warning("json format err: " + file.getAbsolutePath());
            }
        }
        return Files.newInputStream(file.toPath());
    }

    private static boolean checkBundleTypeConsistency(List<String> hapPathList, List<String> hspPathList,
                                                      Utility utility) {
        String bundleType = "";
        if (!hapPathList.isEmpty()) {
            bundleType = getBundleTypeFromPath(Paths.get(hapPathList.get(0)));
        } else if (!hspPathList.isEmpty()) {
            bundleType = getBundleTypeFromPath(Paths.get(hspPathList.get(0)));
        }
        if (bundleType == null || bundleType.isEmpty()) {
            return false;
        }
        for (String hapPath : hapPathList) {
            if (!bundleType.equals(getBundleTypeFromPath(Paths.get(hapPath)))) {
                LOG.error("bundleType is not same");
                return false;
            }
        }
        for (String hspPath : hspPathList) {
            if (!bundleType.equals(getBundleTypeFromPath(Paths.get(hspPath)))) {
                LOG.error("bundleType is not same");
                return false;
            }
        }
        if (bundleType.equals(Constants.BUNDLE_TYPE_SHARED) ||
                bundleType.equals(Constants.BUNDLE_TYPE_APP_SERVICE)) {
            utility.setIsSharedApp(true);
        }
        return true;
    }

    private static boolean moduleJsonAndPackInfoExists(List<String> hapPathList, List<String> hspPathList) {
        for (String hapPath : hapPathList) {
            Path path = Paths.get(hapPath);
            if (!Files.exists(path.resolve(Constants.FILE_MODULE_JSON))) {
                LOG.error("not found module.json in path: " + path);
                return false;
            }
            if (!Files.exists(path.resolve(Constants.FILE_PACK_INFO))) {
                LOG.error("not found pack.info in path: " + path);
                return false;
            }
        }
        for (String hspPath : hspPathList) {
            Path path = Paths.get(hspPath);
            if (Files.isDirectory(path)) {
                if (!Files.exists(path.resolve(Constants.FILE_MODULE_JSON))) {
                    LOG.error("not found module.json in path: " + path);
                    return false;
                }
                if (!Files.exists(path.resolve(Constants.FILE_PACK_INFO))) {
                    LOG.error("not found pack.info in path: " + path);
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean isFileValid(String filePath, String suffix) {
        Path path = Paths.get(filePath);
        return Files.isRegularFile(path) && path.getFileName().toString().endsWith(suffix);
    }

    private static boolean isDirValid(String filePath) {
        return Files.isDirectory(Paths.get(filePath));
    }

    /**
     * verify input param
     *
     * @param utility common data
     * @return true if verify ok
     */
    public static boolean isVerifyValidInFastAppMode(Utility utility) {
        if (utility.getPackInfoPath().isEmpty()) {
            LOG.error("CompressVerify::isArgsValidInAppMode pack-info-path is empty.");
            return false;
        }
        if (!isFileValid(utility.getPackInfoPath(), Constants.FILE_PACK_INFO)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode pack-info-path is invalid.");
            return false;
        }
        if (!utility.getSignaturePath().isEmpty() && !isFileValid(utility.getSignaturePath(), "")) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode signature-path is invalid.");
            return false;
        }
        if (!utility.getCertificatePath().isEmpty() &&
                !isFileValid(utility.getCertificatePath(), "")) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode certificate-path is invalid.");
            return false;
        }
        if (!utility.getPackResPath().isEmpty() && !isFileValid(utility.getPackResPath(), Constants.FILE_PACK_RES)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode pack-res-path is invalid.");
            return false;
        }
        if (!utility.getEntryCardPath().isEmpty() &&
                !CompressVerify.compatibleProcess(utility, utility.getEntryCardPath(),
                        utility.getformattedEntryCardPathList(), Constants.PNG_SUFFIX)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode entrycard-path is invalid.");
            return false;
        }
        if (!utility.getHapPath().isEmpty() &&
                (!isFormatPathValid(utility.getHapPath(), utility.getFormattedHapPathList()) ||
                        !isHapPathValid(utility.getFormattedHapPathList()))) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode hap-path is invalid.");
            return false;
        }
        if (!utility.getHspPath().isEmpty() &&
                (!isFormatPathValid(utility.getHspPath(), utility.getFormattedHspPathList()) ||
                        !isHspPathValid(utility.getFormattedHspPathList()))) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode hsp-path is invalid.");
            return false;
        }
        if (utility.getHapPath().isEmpty() && utility.getHspPath().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode hap-path and hsp-path is empty.");
            return false;
        }
        if (!moduleJsonAndPackInfoExists(utility.getFormattedHapPathList(), utility.getFormattedHspPathList())) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode hap-path or hsp-path is invalid.");
            return false;
        }
        if (!checkBundleTypeConsistency(
                utility.getFormattedHapPathList(), utility.getFormattedHspPathList(), utility)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode bundleType is inconsistent.");
            return false;
        }
        if (!isPackInfoValid(Paths.get(utility.getPackInfoPath()),
                utility.getFormattedHapPathList(), utility.getFormattedHspPathList())) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode pack.info is invalid.");
            return false;
        }
        if (utility.getOutPath().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode out-path is empty.");
            return false;
        }
        Path outPath = Paths.get(utility.getOutPath());
        if (utility.getForceRewrite().equals(Constants.FALSE) && Files.exists(outPath)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode out file already existed.");
            return false;
        }
        if (!outPath.getFileName().toString().endsWith(Constants.APP_SUFFIX)) {
            LOG.error("CompressVerify::isVerifyValidInFastAppMode out-path must end with .app.");
            return false;
        }
        return true;
    }

    private static boolean isHapPathValid(List<String> formatPathList) {
        for (String path : formatPathList) {
            if (!isDirValid(path)) {
                return false;
            }
        }
        return true;
    }

    private static boolean isHspPathValid(List<String> formatPathList) {
        for (String path : formatPathList) {
            if (!isDirValid(path) && !isFileValid(path, Constants.HSP_SUFFIX)) {
                return false;
            }
        }
        return true;
    }

    private static boolean isFormatPathValid(String inputPath, List<String> formatPathList) {
        Set<String> formatPathSet = new HashSet<>();
        for (String path : inputPath.split(Constants.COMMA)) {
            try {
                Path realpath = Paths.get(path).toRealPath();
                if (Files.exists(realpath)) {
                    formatPathSet.add(realpath.toString());
                } else {
                    LOG.error("PackageUtil::formatPath not exists: " + realpath);
                    return false;
                }
            } catch (IOException ex) {
                LOG.error("PackageUtil::formatPath err: " + ex.getMessage());
                return false;
            }
        }
        formatPathList.addAll(formatPathSet);
        return true;
    }

    private static boolean isPackInfoValid(Path packInfo, List<String> hapPathList, List<String> hspPathList) {
        List<String> allPackages = getPackageNameFromPath(packInfo);
        Set<String> allPackageSet = new HashSet<>(allPackages);
        if (allPackages.size() > allPackageSet.size()) {
            LOG.error("package name is redundant in app pack.info: " + packInfo);
            return false;
        }
        if (allPackages.isEmpty()) {
            LOG.error("app pack.info format err: " + packInfo);
            return false;
        }
        Set<String> packages = new HashSet<>();
        for (String hapPath : hapPathList) {
            List<String> list = getPackageNameFromPath(Paths.get(hapPath));
            if (list.size() != 1) {
                LOG.error("module pack.info format err: " + hapPath);
                return false;
            }
            String packageName = list.get(0);
            if (!allPackages.contains(packageName)) {
                LOG.error("module pack.info name not exist in app pack.info name list: " + hapPath);
                return false;
            }
            if (packages.contains(packageName)) {
                LOG.error("package name is redundant in " + hapPath);
                return false;
            }
            packages.add(packageName);
        }
        for (String hspPath : hspPathList) {
            List<String> list = getPackageNameFromPath(Paths.get(hspPath));
            if (list.size() != 1) {
                LOG.error("module pack.info format err: " + hspPath);
                return false;
            }
            String packageName = list.get(0);
            if (!allPackages.contains(packageName)) {
                LOG.error("module pack.info name not exist in app pack.info name list: " + hspPath);
                return false;
            }
            if (packages.contains(packageName)) {
                LOG.error("package name is redundant in " + hspPath);
                return false;
            }
            packages.add(packageName);
        }
        if (!allPackageSet.equals(packages)) {
            LOG.error("package name not same between module and app pack.info.");
            return false;
        }
        return true;
    }

}
