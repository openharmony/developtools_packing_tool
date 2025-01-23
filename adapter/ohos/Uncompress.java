/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import java.nio.charset.StandardCharsets;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.stream.Collectors;
import java.util.Set;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

/**
 * bundle uncompress.
 *
 */
public class Uncompress {
    private static final String HAP_SUFFIX = ".hap";
    private static final String APK_SUFFIX = ".apk";
    private static final String JSON_SUFFIX = ".json";

    private static final String PACK_INFO = "pack.info";
    private static final String HARMONY_PROFILE = "config.json";
    private static final String MODULE_JSON = "module.json";
    private static final String RESOURCE_INDEX = "resources.index";
    private static final String RPCID_SC = "rpcid.sc";
    private static final String LIBS = "libs";
    private static final String PACKAGEFILE = "packagefile";
    private static final String LINUX_FILE_SEPARATOR = "/";
    private static final String TEMP_PATH = "temp";
    private static final String HAP_SUFFIXI = ".hap";
    private static final String ENTRY_TYPE = "entry";
    private static final String SYSTEM_ACTION = "action.system.home";
    private static final String SYSTEM_WANT_HOME = "ohos.want.action.home";
    private static final String SYSTEM_ENTITY = "entity.system.home";
    private static final int READ_BUFFER_SIZE = 1024;
    private static final int BUFFER_SIZE = 10 * 1024;
    private static final String LIBS_DIR_NAME = "libs";
    private static final String CUT_ENTRY_FILENAME = "cut_entry.apk";
    private static final String SO_SUFFIX = ".so";
    private static final String RESOURCE_PATH = "resources/base/profile/";
    private static final String TRUE = "true";
    private static final String HQF_SUFFIX = ".hqf";
    private static final String PATCH_JSON = "patch.json";
    private static final String HAP_PREFIX = "HAP";
    private static final String HSP_SUFFIX = ".hsp";
    private static final int MAX_CPU_ABI_TYPE_NUM = 128;

    private static final Log LOG = new Log(Uncompress.class.toString());

    /**
     * unpackage entrance.
     *
     * @param utility common data
     * @return unpackageProcess if unpackage succeed
     */
    static boolean unpackageProcess(Utility utility) {
        if (utility == null) {
            LOG.error("Uncompress::unpackageProcess utility is null.");
            return false;
        }
        boolean unpackageResult = true;
        File destFile = new File(utility.getOutPath());

        if (!destFile.exists()) {
            if (!destFile.mkdirs()) {
                LOG.error("Uncompress::unpackageProcess create out file directory failed!");
                return false;
            }
        }
        try {
            if (!Utility.MODE_HAP.equals(utility.getMode()) || !TRUE.equals(utility.getRpcid())) {
                if (!utility.getForceRewrite().isEmpty() && "true".equals(utility.getForceRewrite())) {
                    File outPath = new File(utility.getOutPath());
                    deleteFile(outPath);
                    outPath.mkdirs();
                }
            }
            switch (utility.getMode()) {
                case Utility.MODE_HAP:
                    unpackageHapMode(utility);
                    break;
                case Utility.MODE_HAR:
                    dataTransferAllFiles(utility.getHarPath(), utility.getOutPath());
                    break;
                case Utility.MODE_APP:
                    dataTransferFilesByApp(utility, utility.getAppPath(), utility.getOutPath());
                    break;
                case Utility.MODE_APPQF:
                    uncompressAPPQFFile(utility);
                    break;
                case Utility.MODE_HSP:
                    unpackageHspMode(utility);
                    break;
                default:
                    LOG.error("Uncompress::unpackageProcess input wrong type!");
                    throw new BundleException("Uncompress::unpackageProcess input wrong type!");
            }
        } catch (BundleException ignored) {
            unpackageResult = false;
            LOG.error("Uncompress::unpackageProcess Bundle exception");
        }
        // return uncompress information.
        if (!unpackageResult) {
            LOG.error("Uncompress::unpackageProcess unpackage failed!");
        }
        return unpackageResult;
    }

    /**
     * unpack hap.
     *
     * @param utility common data
     */
    static void unpackageHapMode(Utility utility) throws BundleException {
        if (!Utility.MODE_HAP.equals(utility.getMode())) {
            LOG.error("unpackageHapMode input wrong unpack mode: " + utility.getMode());
            throw new BundleException("Uncompress::unpackageHapMode input wrong unpack mode");
        }
        try {
            if (TRUE.equals(utility.getLibs())) {
                unpackageLibsMode(utility.getHapPath(), utility.getOutPath(), utility.getFormattedCpuAbiList());
                return;
            }
            if (TRUE.equals(utility.getRpcid())) {
                getRpcidFromHap(utility.getHapPath(), utility.getOutPath());
                return;
            }
            if (TRUE.equals(utility.getUnpackApk())) {
                unzip(utility, utility.getHapPath(), utility.getOutPath(), APK_SUFFIX);
                String[] temp = utility.getHapPath().replace("\\", "/").split("/");
                String hapName = temp[temp.length - 1];
                repackHap(utility.getHapPath(), utility.getOutPath(), hapName, utility.getUnpackApk());
            } else {
                dataTransferAllFiles(utility.getHapPath(), utility.getOutPath());
            }
        } catch (BundleException e) {
            LOG.error("Uncompress::unpackageHapMode failed");
            throw new BundleException("Uncompress::unpackageHapMode failed");
        }
    }

    /**
     * unpack hsp.
     *
     * @param utility common data
     */
    static void unpackageHspMode(Utility utility) throws BundleException {
        if (!Utility.MODE_HSP.equals(utility.getMode())) {
            LOG.error("unpackageHspMode input wrong unpack mode: " + utility.getMode());
            throw new BundleException("Uncompress::unpackageHspMode input wrong unpack mode");
        }
        try {
            if (TRUE.equals(utility.getLibs())) {
                List<String> cpuAbiList = utility.getFormattedCpuAbiList();
                unpackageLibsMode(utility.getHspPath(), utility.getOutPath(), cpuAbiList);
                return;
            }
            dataTransferAllFiles(utility.getHspPath(), utility.getOutPath());
        } catch (BundleException e) {
            LOG.error("Uncompress::unpackageHspMode failed");
            throw new BundleException("Uncompress::unpackageHspMode failed");
        }
    }

    /**
     * uncompress app.
     *
     * @param utility common data
     * @return the uncompress result
     */
    static UncompressResult uncompressAppByPath(Utility utility) {
        UncompressResult compressResult = new UncompressResult();
        InputStream input = null;
        String srcPath = utility.getAppPath();
        String parseMode = utility.getParseMode();

        try {
            if (UncompressEntrance.PARSE_MODE_HAPLIST.equals(parseMode)) {
                compressResult = uncompress(utility.getDeviceType(), srcPath, PACK_INFO);
            } else if (UncompressEntrance.PARSE_MODE_HAPINFO.equals(parseMode)) {
                compressResult = uncompressHapAndHspFromAppPath(srcPath, utility);
            } else if (UncompressEntrance.PARSE_MODE_ALL.equals(parseMode)) {
                compressResult = uncompressAllAppByPath(srcPath);
            } else {
                LOG.error("Uncompress::uncompressApp parseMode is invalid!");
                compressResult.setResult(false);
                compressResult.setMessage("ParseApp parseMode is invalid");
            }

            compressResult.setPackageSize(FileUtils.getFileSize(srcPath));
        } catch (BundleException e) {
            LOG.error("Uncompress::uncompressApp Bundle exception");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp Bundle exception");
        } finally {
            Utility.closeStream(input);
        }
        return compressResult;
    }

    private static UncompressResult uncompressHapAndHspFromAppPath(
            String srcPath, Utility utility) throws BundleException {
        UncompressResult result = new UncompressResult();
        String hapName = utility.getHapName();
        if (!hapName.toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)
                && !hapName.toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
            hapName += HAP_SUFFIX;
        }
        ZipFile appFile = null;
        ZipEntry entry = null;
        InputStream stream = null;
        try {
            appFile = new ZipFile(srcPath);
            Enumeration<? extends ZipEntry> entries = appFile.entries();
            while (entries.hasMoreElements()) {
                entry = entries.nextElement();
                stream = appFile.getInputStream(entry);
                if (!hapName.equals(entry.getName().toLowerCase(Locale.ENGLISH))) {
                    continue;
                }
                UncompressResult hapInfo = uncompressHapByStream("", stream, hapName);
                if (hapInfo.getProfileInfos() != null && hapInfo.getProfileInfos().size() > 0) {
                    hapInfo.getProfileInfos().get(0).hapInfo.originalSize = entry.getSize();
                    hapInfo.getProfileInfos().get(0).hapInfo.compressedSize = entry.getCompressedSize();
                    result.addProfileInfo(hapInfo.getProfileInfos().get(0));
                    result.addProfileInfoStr(hapInfo.getProfileInfosStr().get(0));
                }
                break;
            }
        } catch (IOException | BundleException e) {
            LOG.error("uncompressHapFromAppPath failed: " + e.getMessage());
            throw new BundleException("uncompressHapFromAppPath failed!");
        } finally {
            Utility.closeStream(appFile);
            Utility.closeStream(stream);
        }
        return result;
    }

    private static UncompressResult uncompressAllAppByPath(String srcPath) throws BundleException {
        UncompressResult result = new UncompressResult();
        ZipFile appFile = null;
        ZipEntry entry = null;
        InputStream stream = null;
        try {
            appFile = new ZipFile(srcPath);
            Enumeration<? extends ZipEntry> entries = appFile.entries();
            while (entries.hasMoreElements()) {
                entry = entries.nextElement();
                stream = appFile.getInputStream(entry);
                if (PACK_INFO.equals(entry.getName().toLowerCase(Locale.ENGLISH))) {
                    String packInfo = new BufferedReader(new InputStreamReader(stream, StandardCharsets.UTF_8))
                            .lines().parallel().collect(Collectors.joining(System.lineSeparator()));
                    List<PackInfo> packInfos = JsonUtil.parseHapList("", packInfo);
                    result.setPackInfoStr(packInfo);
                    result.setPackInfos(packInfos);
                }
                if (entry.getName().toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)
                        || entry.getName().toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
                    UncompressResult hapInfo = uncompressHapByStream("", stream, entry.getName());
                    if (hapInfo.getProfileInfos() != null && hapInfo.getProfileInfos().size() > 0) {
                        hapInfo.getProfileInfos().get(0).hapInfo.originalSize = entry.getSize();
                        hapInfo.getProfileInfos().get(0).hapInfo.compressedSize = entry.getCompressedSize();
                        result.addProfileInfo(hapInfo.getProfileInfos().get(0));
                        result.addProfileInfoStr(hapInfo.getProfileInfosStr().get(0));
                    }
                }
            }
            result = checkParseAllResult(result);
            result = obtainLabelAndIcon(result);
        } catch (IOException | BundleException e) {
            LOG.error("uncompressAllAppByPath failed: " + e.getMessage());
            throw new BundleException("uncompressAllAppByPath failed!");
        } finally {
            Utility.closeStream(appFile);
            Utility.closeStream(stream);
        }
        return result;
    }


    /**
     * uncompress app.
     *
     * @param utility common data
     * @param input the InputStream about the app package.
     * @return the uncompress result
     */
    static UncompressResult uncompressAppByInput(Utility utility, InputStream input) {
        UncompressResult compressResult = new UncompressResult();
        String parseMode = utility.getParseMode();
        try {
            if (!parseMode.isEmpty() && UncompressEntrance.PARSE_MODE_HAPLIST.equals(parseMode)) {
                compressResult = uncompressByInput(utility.getDeviceType(), input, PACK_INFO, "");
            } else if (!parseMode.isEmpty() && UncompressEntrance.PARSE_MODE_HAPINFO.equals(parseMode)) {
                compressResult = uncompressHapFromAppStream(utility.getDeviceType(), input, utility.getHapName());
            } else if (!parseMode.isEmpty() && UncompressEntrance.PARSE_MODE_ALL.equals(parseMode)) {
                compressResult = uncompressAllFromAppStream(input);
            } else {
                LOG.error("Uncompress::uncompressAppByInput parseMode is invalid!");
                compressResult.setResult(false);
                compressResult.setMessage("ParseApp parseMode is invalid");
            }
        } catch (BundleException exception) {
            LOG.error("Uncompress::uncompressAppByInput Bundle exception");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp Bundle exception");
        }
        return compressResult;
    }

    private static UncompressResult uncompressHapFromAppStream(String deviceType, InputStream stream, String fileName)
            throws BundleException {
        String hapFile = fileName;
        if (!fileName.toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)
                && !fileName.toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
            hapFile += HAP_SUFFIX;
        }
        UncompressResult result = new UncompressResult();
        ZipInputStream zipInputStream = null;
        try {
            zipInputStream = new ZipInputStream(stream);
            ZipEntry appEntry;
            while ((appEntry = zipInputStream.getNextEntry()) != null) {
                if (appEntry.getName().toLowerCase(Locale.ENGLISH).equals(hapFile)) {
                    result = uncompressHapByStream("", zipInputStream, appEntry.getName());
                }

                if (result.getProfileInfos().size() > 0) {
                    result.getProfileInfos().get(0).hapInfo.originalSize = appEntry.getSize();
                    result.getProfileInfos().get(0).hapInfo.compressedSize = appEntry.getCompressedSize();
                }
            }
        } catch (IOException e) {
            LOG.error("uncompressHapFromAppStream failed: " + e.getMessage());
            throw new BundleException("uncompressHapFromAppStream failed!");
        } finally {
            Utility.closeStream(zipInputStream);
        }
        return result;
    }

    private static UncompressResult uncompressAllFromAppStream(InputStream stream) throws BundleException {
        UncompressResult result = new UncompressResult();
        ZipInputStream zipInputStream = null;
        try {
            zipInputStream = new ZipInputStream(stream);
            ZipEntry entry;
            while ((entry = zipInputStream.getNextEntry()) != null) {
                if (PACK_INFO.equals(entry.getName().toLowerCase(Locale.ENGLISH))) {
                    String packInfo = new BufferedReader(new InputStreamReader(zipInputStream,
                            StandardCharsets.UTF_8)).lines().parallel()
                            .collect(Collectors.joining(System.lineSeparator()));
                    List<PackInfo> packInfos = JsonUtil.parseHapList("", packInfo);
                    result.setPackInfoStr(packInfo);
                    result.setPackInfos(packInfos);
                }
                if (entry.getName().toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)
                        || entry.getName().toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
                    UncompressResult hapResult = uncompressHapByStream("", zipInputStream, entry.getName());
                    if (hapResult.getProfileInfos() != null && hapResult.getProfileInfos().size() > 0) {
                        hapResult.getProfileInfos().get(0).hapInfo.originalSize = entry.getSize();
                        hapResult.getProfileInfos().get(0).hapInfo.compressedSize = entry.getCompressedSize();
                        result.addProfileInfo(hapResult.getProfileInfos().get(0));
                        result.addProfileInfoStr(hapResult.getProfileInfosStr().get(0));
                    }
                }
            }
            result = checkParseAllResult(result);
            result = obtainLabelAndIcon(result);
        } catch (IOException | BundleException e) {
            LOG.error("uncompressAllFromAppStream failed: " + e.getMessage());
            throw new BundleException("uncompressAllFromAppStream failed!");
        } finally {
            Utility.closeStream(zipInputStream);
        }
        return result;
    }

    /**
     * uncompress hap.
     *
     * @param utility common data
     * @return the uncompress result
     */
    static UncompressResult uncompressHap(Utility utility) {
        UncompressResult compressResult = new UncompressResult();
        try {
            compressResult = uncompressHapByPath(utility.getDeviceType(), utility.getHapPath());
        } catch (BundleException ignored) {
            LOG.error("Uncompress::uncompressHap Bundle exception");
            compressResult.setResult(false);
            compressResult.setMessage("uncompressHap Bundle exception");
        }
        return compressResult;
    }

    /**
     * uncompress hap by path, it can adapt stage module and fa module.
     *
     * @param deviceType indicates the device type of parse type.
     * @param hapPath indicates the hap path of hap.
     * @return the uncompress result
     */
    static UncompressResult uncompressHapByPath(String deviceType, String hapPath) throws BundleException {
        UncompressResult compressResult = new UncompressResult();
        try {
            if (isModuleHap(hapPath, compressResult)) {
                compressResult = unCompressModuleHap(deviceType, hapPath, MODULE_JSON);
            } else {
                compressResult = uncompress(deviceType, hapPath, HARMONY_PROFILE);
                compressResult = obtainLabelAndIcon(compressResult);
            }
        } catch (BundleException e) {
            LOG.error("Uncompress::uncompressHapByPath Bundle exception");
            throw new BundleException("Uncompress::uncompressHapByPath failed");
        }
        return compressResult;
    }

    /**
     * uncompress hap.
     *
     * @param utility common data
     * @param input the InputStream about the app package.
     * @return the uncompress result
     */
    static UncompressResult uncompressHapByInput(Utility utility, InputStream input) {
        UncompressResult compressResult = new UncompressResult();
        try {
            compressResult = uncompressHapByStream(utility.getDeviceType(), input, "");
        } catch (BundleException ignored) {
            LOG.error("Uncompress::uncompressHapByInput Bundle exception");
            compressResult.setResult(false);
            compressResult.setMessage("uncompressHapByInput Bundle exception");
        }
        return compressResult;
    }

    /**
     * uncompress hap by InputStream, it can adapt stage module and fa module.
     *
     * @param deviceType indicates the device type of parse type.
     * @param stream indicates the input stream of hap.
     * @return the uncompress result
     */
    static UncompressResult uncompressHapByStream(String deviceType, InputStream stream,
                                                  String hapName) throws BundleException {
        UncompressResult compressResult = new UncompressResult();
        compressResult = uncompressHapByBigStream(deviceType, stream, hapName);
        return compressResult;
    }

    static UncompressResult uncompressHapByBigStream(String deviceType, InputStream stream, String hapName)
            throws BundleException {
        UncompressResult compressResult = new UncompressResult();
        InputStream fileStream = null;
        InputStream parseStream = null;
        File file = null;
        try {
            String releativePath = System.getProperty("user.dir");
            File directory = new File(releativePath);
            file = File.createTempFile(HAP_PREFIX, HAP_SUFFIX, directory);
            writeToTempFile(stream, file);
            fileStream = new FileInputStream(file);
            boolean isModule = false;
            if (isModuleInput(fileStream)) {
                isModule = true;
            }
            parseStream = new FileInputStream(file);
            if (isModule) {
                compressResult = uncompressModuleHapByInput(deviceType, parseStream, MODULE_JSON, hapName);
            } else {
                compressResult = uncompressByInput(deviceType, parseStream, HARMONY_PROFILE, hapName);
                compressResult = obtainLabelAndIcon(compressResult);
            }
        } catch (IOException e) {
            LOG.error("uncompressHapByBigStream failed for IO exception: " + e.getMessage());
            throw new BundleException("uncompressHapByBigStream failed for IO exception!");
        } finally {
            Utility.closeStream(fileStream);
            Utility.closeStream(parseStream);
            if (file != null) {
                FileUtils.deleteFile(file.getPath());
            }
        }
        return compressResult;
    }

    /**
     * unzip process
     *
     * @param utility common data
     * @param srcPath source file path
     * @param destDirPath destination file path
     * @param suffix suffix for judgment
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void unzip(Utility utility, String srcPath, String destDirPath, String suffix)
            throws BundleException {
        if (utility == null) {
            LOG.error("Uncompress::unzip utility is null!");
            throw new BundleException("Unzip failed, utility is null");
        }

        if (srcPath.isEmpty() || !UncompressVerify.isPathValid(srcPath, true, "")) {
            LOG.error("Uncompress::unzip srcPath is invalid!");
            throw new BundleException("Unzip failed, srcPath is invalid");
        }

        if (destDirPath.isEmpty() || !UncompressVerify.isPathValid(destDirPath, false, null)) {
            LOG.error("Uncompress::unzip destDirPath is invalid!");
            throw new BundleException("Unzip failed, destDirPath is invalid");
        }
        unzipFromFile(utility, srcPath, destDirPath, suffix);
    }

    /**
     * unzip process from the file
     *
     * @param utility common data
     * @param srcPath source file path
     * @param destDirPath destination file path
     * @param suffix suffix for judgment
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void unzipFromFile(Utility utility, String srcPath, String destDirPath, String suffix)
            throws BundleException {
        ZipFile zipFile = null;
        String hapNames = "";
        try {
            zipFile = new ZipFile(new File(srcPath));
            if (utility != null && !utility.getDeviceType().isEmpty()
                    && (HAP_SUFFIX.equals(suffix) || HSP_SUFFIX.equals(suffix))) {
                List<PackInfo> packInfos = uncompress(utility.getDeviceType(), srcPath, PACK_INFO).getPackInfos();
                for (PackInfo packinfo : packInfos) {
                    hapNames += packinfo.name + ",";
                }
            }

            int entriesNum = 0;
            for (Enumeration<? extends ZipEntry> entries = zipFile.entries(); entries.hasMoreElements(); ) {
                entriesNum++;
                ZipEntry entry = entries.nextElement();
                String entryName = "";
                if (entry == null || entry.getName().isEmpty()) {
                    continue;
                }
                if (entry.getName().toLowerCase().endsWith(CUT_ENTRY_FILENAME) &&
                        "false".equals(utility.getUnpackCutEntryApk())) {
                    continue;
                }
                entryName = entry.getName();
                if (!entryName.toLowerCase(Locale.ENGLISH).endsWith(suffix) ||
                        (!hapNames.isEmpty() && !hapNames.contains(entryName.replace(suffix, "")))) {
                    continue;
                }
                String tempDir = destDirPath.replace(File.separator, LINUX_FILE_SEPARATOR);
                if (HAP_SUFFIX.equals(suffix) && "true".equals(utility.getUnpackApk())) {
                    tempDir = tempDir + LINUX_FILE_SEPARATOR + entryName.replace(suffix, "");
                    File destFileDir = new File(tempDir);
                    if (!destFileDir.exists()) {
                        destFileDir.mkdir();
                    }
                }
                if (APK_SUFFIX.equals(suffix) && "true".equals(utility.getUnpackApk())
                        && entryName.contains(LINUX_FILE_SEPARATOR)) {
                    // only unpack shell apk which in the root directory
                    continue;
                }
                String tempPath = tempDir + LINUX_FILE_SEPARATOR + entryName;
                if (!FileUtils.matchPattern(tempPath)) {
                    LOG.error("Input invalid file: " + tempPath);
                    throw new BundleException("Input invalid file " + tempPath);
                }
                File destFile = new File(tempPath);
                dataTransfer(zipFile, entry, destFile);
                if (JSON_SUFFIX.equals(suffix)) {
                    break;
                } else if (HAP_SUFFIX.equals(suffix) && "true".equals(utility.getUnpackApk())) {
                    unzip(utility, tempPath, tempDir, APK_SUFFIX);
                    repackHap(tempPath, tempDir, entryName, utility.getUnpackApk());
                }
            }
        } catch (IOException | BundleException exception) {
            LOG.error("Uncompress::unzipInHapMode failed: " + exception.getMessage());
            throw new BundleException("Unzip in hap mode failed");
        } finally {
            Utility.closeStream(zipFile);
        }
    }

    /**
     * uncompress dataTransfer
     *
     * @param zipFile input zip file
     * @param entry input file in zip
     * @param destFile output file path
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void dataTransfer(ZipFile zipFile, ZipEntry entry, File destFile) throws BundleException {
        InputStream fileInputStream = null;
        FileOutputStream fileOutStream = null;
        try {
            if (!FileUtils.matchPattern(destFile.getCanonicalPath())) {
                LOG.error("Input invalid file " + destFile.getCanonicalPath());
                throw new BundleException("Input invalid file" + destFile.getCanonicalPath());
            }
            fileInputStream = zipFile.getInputStream(entry);
            fileOutStream = new FileOutputStream(destFile);
            byte[] data = new byte[BUFFER_SIZE];
            int count = fileInputStream.read(data);
            int total = 0;
            while (count > 0) {
                fileOutStream.write(data, 0, count);
                total += count;
                count = fileInputStream.read(data);
            }
        } catch (IOException | BundleException exception) {
            LOG.error("Uncompress::dataTransfer file " + exception.getMessage());
            throw new BundleException("DataTransfer failed");
        } finally {
            Utility.closeStream(fileOutStream);
            Utility.closeStream(fileInputStream);
        }
    }

    /**
     * uncompress dataTransfer all files.
     *
     * @param srcPath source file path
     * @param destDirPath destination file path
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void dataTransferAllFiles(String srcPath, String destDirPath) throws BundleException {
        ZipFile zipFile = null;
        try {
            if (!FileUtils.matchPattern(srcPath)) {
                throw new BundleException("Input invalid file " + srcPath);
            }
            zipFile = new ZipFile(new File(srcPath));
            int entriesNum = 0;
            for (Enumeration<? extends ZipEntry> entries = zipFile.entries(); entries.hasMoreElements(); ) {
                entriesNum++;
                ZipEntry entry = entries.nextElement();
                if (entry == null) {
                    continue;
                }
                String tempPath = destDirPath + LINUX_FILE_SEPARATOR + entry.getName();
                File destFile = new File(tempPath);
                if (destFile != null && destFile.getParentFile() != null && !destFile.getParentFile().exists()) {
                    destFile.getParentFile().mkdirs();
                }
                dataTransfer(zipFile, entry, destFile);
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Uncompress::unzipApk file not found exception: " + ignored.getMessage());
            throw new BundleException("Unzip Apk failed");
        } catch (IOException exception) {
            LOG.error("Uncompress::unzipApk io exception: " + exception.getMessage());
            throw new BundleException("Unzip Apk failed");
        } finally {
            Utility.closeStream(zipFile);
        }
    }

    private static void dataTransferFilesByApp(Utility utility, String srcPath, String destDirPath)
            throws BundleException {
        ZipFile zipFile = null;
        try {
            zipFile = new ZipFile(new File(srcPath));
            int entriesNum = 0;
            for (Enumeration<? extends ZipEntry> entries = zipFile.entries(); entries.hasMoreElements(); ) {
                entriesNum++;
                ZipEntry entry = entries.nextElement();
                if (entry == null) {
                    continue;
                }
                String filePath = destDirPath + LINUX_FILE_SEPARATOR + entry.getName();
                if (!FileUtils.matchPattern(filePath)) {
                    throw new BundleException("Input invalid path " + filePath);
                }
                File destFile = new File(filePath);
                if (destFile != null && destFile.getParentFile() != null && !destFile.getParentFile().exists()) {
                    destFile.getParentFile().mkdirs();
                }
                boolean isUnpackApk = "true".equals(utility.getUnpackApk());
                if (isUnpackApk && filePath.toLowerCase().endsWith(HAP_SUFFIX)) {
                    dataTransfer(zipFile, entry, destFile);
                    unzip(utility, filePath, destDirPath, APK_SUFFIX);
                    String[] temp = filePath.replace("\\", "/").split("/");
                    String hapName = "";
                    if (temp.length > 0) {
                        hapName = temp[temp.length - 1];
                    }
                    repackHap(filePath, destDirPath, hapName, utility.getUnpackApk());
                } else {
                    dataTransfer(zipFile, entry, destFile);
                }
            }
        } catch (IOException | BundleException exception) {
            LOG.error("Uncompress::unzipApk file failed " + exception.getMessage());
            throw new BundleException("Unzip Apk failed");
        } finally {
            Utility.closeStream(zipFile);
        }
    }

    private static byte[] getResourceDataFromHap(ZipFile zipFile) throws BundleException, IOException {
        int entriesNum = 0;
        InputStream indexInputStream = null;
        try {
            for (Enumeration<? extends ZipEntry> entries = zipFile.entries(); entries.hasMoreElements(); ) {
                entriesNum++;
                ZipEntry indexEntry = entries.nextElement();
                if (indexEntry == null) {
                    continue;
                }
                if (indexEntry != null && !"".equals(indexEntry.getName()) &&
                        indexEntry.getName().toLowerCase().endsWith(RESOURCE_INDEX)) {
                    indexInputStream = zipFile.getInputStream(indexEntry);
                    return getByte(indexInputStream);
                }
            }
        } finally {
            Utility.closeStream(indexInputStream);
        }
        return null;
    }

    private static HapZipInfo unZipHapFileFromHapFile(String srcPath)
            throws BundleException, IOException {
        HapZipInfo hapZipInfo = new HapZipInfo();
        ZipFile zipFile = null;
        if (!FileUtils.matchPattern(srcPath)) {
            LOG.error("Input invalid path " + srcPath);
            throw new BundleException("Input invalid path " + srcPath);
        }
        try {
            File srcFile = new File(srcPath);
            zipFile = new ZipFile(srcFile);
            hapZipInfo.setHarmonyProfileJsonStr(FileUtils.getFileStringFromZip(HARMONY_PROFILE, zipFile));
            hapZipInfo.setResDataBytes(getResourceDataFromHap(zipFile));
            hapZipInfo.setPackInfoJsonStr(FileUtils.getFileStringFromZip(PACK_INFO, zipFile));
            hapZipInfo.setHapFileName(getHapNameWithoutSuffix(srcFile.getName()));
        }  finally {
            Utility.closeStream(zipFile);
        }
        return hapZipInfo;
    }

    /**
     * uncompress from specified file name
     *
     * @param deviceType device type
     * @param srcPath source file path
     * @param fileName uncompress file name
     * @return the uncompress result
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static UncompressResult uncompress(String deviceType, String srcPath, String fileName)
            throws BundleException {
        if (srcPath.isEmpty() || fileName.isEmpty()) {
            LOG.error("Uncompress::uncompress srcPath, fileName is empty!");
            throw new BundleException("Uncompress failed, srcPath or fileName is empty");
        }

        UncompressResult result = new UncompressResult();
        try {
            HapZipInfo hapZipInfo = unZipHapFileFromHapFile(srcPath);
            if (isPackInfo(fileName)) {
                uncompressPackInfo(deviceType, hapZipInfo, result);
            } else {
                uncompressProfileInfo(hapZipInfo, result);
            }
        } catch (IOException exception) {
            LOG.error("Uncompress::uncompress io exception: " + exception.getMessage());
            throw new BundleException("Uncompress failed");
        }
        return result;
    }

    private static void uncompressPackInfo(String deviceType, HapZipInfo hapZipInfo, UncompressResult uncomperssResult)
            throws BundleException {
        List<PackInfo> packInfos = JsonUtil.parseHapList(deviceType, hapZipInfo.getPackInfoJsonStr());
        uncomperssResult.setPackInfoStr(hapZipInfo.getPackInfoJsonStr());
        uncomperssResult.setPackInfos(packInfos);
    }

    private static void uncompressProfileInfo(HapZipInfo hapZipInfo, UncompressResult uncomperssResult)
            throws BundleException {
        ProfileInfo profileInfo = JsonUtil.parseProfileInfo(hapZipInfo.getHarmonyProfileJsonStr(),
                hapZipInfo.getResDataBytes(), hapZipInfo.getPackInfoJsonStr(), hapZipInfo.getHapFileName());
        profileInfo.hapName = hapZipInfo.getHapFileName();
        profileInfo.appInfo.setBundleType(getFABundleType(profileInfo));
        uncomperssResult.addProfileInfoStr(hapZipInfo.getHarmonyProfileJsonStr());
        uncomperssResult.addProfileInfo(profileInfo);
    }

    private static String getFABundleType(ProfileInfo profileInfo) {
        String bundleType = "app";
        if (profileInfo.hapInfo.distro.installationFree == 1) {
            bundleType = "atomicService";
        }
        return bundleType;
    }

    private static HapZipInfo unZipHapFileFromInputStream(InputStream input) throws BundleException, IOException {
        BufferedInputStream bufIn = null;
        ZipInputStream zipIn = null;
        BufferedReader bufferedReader = null;
        HapZipInfo hapZipInfo = new HapZipInfo();
        try {
            ZipEntry entry = null;
            bufIn = new BufferedInputStream(input);
            zipIn = new ZipInputStream(bufIn);
            int entriesNum = 0;
            while ((entry = zipIn.getNextEntry()) != null) {
                entriesNum++;
                if (entry.getName().toLowerCase().endsWith(RESOURCE_INDEX)) {
                    hapZipInfo.setResDataBytes(getByte(zipIn));
                    continue;
                }
                if (isPackInfo(entry.getName())) {
                    bufferedReader = new BufferedReader(new InputStreamReader(zipIn));
                    hapZipInfo.setPackInfoJsonStr(readStringFromInputStream(zipIn, bufferedReader));
                    continue;
                }
                if (isHarmonyProfile(entry.getName())) {
                    bufferedReader = new BufferedReader(new InputStreamReader(zipIn));
                    hapZipInfo.setHarmonyProfileJsonStr(readStringFromInputStream(zipIn, bufferedReader));
                }
            }
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(bufIn);
            Utility.closeStream(zipIn);
        }
        return hapZipInfo;
    }

    private static String readStringFromInputStream(ZipInputStream zipIn, BufferedReader bufferedReader)
            throws IOException {
        String line;
        StringBuilder sb = new StringBuilder();
        while ((line = bufferedReader.readLine()) != null) {
            sb.append(line);
        }
        return sb.toString();
    }

    /**
     * uncompress process by InputStream
     *
     * @param deviceType device type
     * @param input the InputStream about the package.
     * @param fileName uncompress file name
     * @return the uncompress result
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static UncompressResult uncompressByInput(String deviceType, InputStream input,
                                                      String fileName, String hapName) throws BundleException {
        UncompressResult result = new UncompressResult();
        try {
            HapZipInfo hapZipInfo = unZipHapFileFromInputStream(input);
            hapZipInfo.setHapFileName(hapName);
            if (isPackInfo(fileName)) {
                uncompressPackInfo(deviceType, hapZipInfo, result);
            } else {
                uncompressProfileInfo(hapZipInfo, result);
            }
        } catch (IOException exception) {
            LOG.error("Uncompress::uncompressByInput io exception: " + exception.getMessage());
            throw new BundleException("Uncompress by input failed");
        }
        return result;
    }

    /**
     * uncompress process by InputStream
     *
     * @param deviceType device type
     * @param input the InputStream about the package.
     * @param fileName uncompress file name
     * @return the module uncompress result
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static ModuleResult uncompressModuleByInput(String deviceType, InputStream input,
                                                        String fileName, String hapName) throws BundleException {
        ModuleResult result = new ModuleResult();
        try {
            HapZipInfo hapZipInfo = unZipModuleHapFileFromInputStream(input);
            hapZipInfo.setHapFileName(hapName);
            if (isPackInfo(fileName)) {
                // for parse app
            } else {
                uncompressModuleJsonInfo(hapZipInfo, result);
            }
        } catch (BundleException exception) {
            LOG.error("Uncompress::uncompressByInput io exception: " + exception.getMessage());
            throw new BundleException("Uncompress by input failed");
        }
        return result;
    }

    /**
     * Get entry byte array.
     *
     * @param zis the InputStream about the entry.
     * @return Return the entry byte array.
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static byte[] getByte(InputStream zis) throws BundleException {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        byte[] buf = null;
        try {
            byte[] temp = new byte[READ_BUFFER_SIZE];
            int length = 0;

            while ((length = zis.read(temp, 0, READ_BUFFER_SIZE)) != -1) {
                bos.write(temp, 0, length);
            }

            buf = bos.toByteArray();
        } catch (IOException e) {
            LOG.error("Uncompress::getByte io exception: " + e.getMessage());
            throw new BundleException("Get byte failed");
        } finally {
            Utility.closeStream(bos);
        }
        return buf;
    }

    /**
     * repack hap
     *
     * @param srcPath source file path
     * @param destDirPath destination file path
     * @param fileName target file name
     * @param unpackApk unpackApk flag
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void repackHap(String srcPath, String destDirPath, String fileName, String unpackApk)
            throws BundleException {
        if (srcPath.isEmpty() || destDirPath.isEmpty() || fileName.isEmpty()) {
            LOG.error("Uncompress::repackHap srcPath, destDirPath or fileName is empty!");
            throw new BundleException("Repack hap failed, srcPath, destDirPath or fileName is empty");
        }

        if (!UncompressVerify.isPathValid(srcPath, true, "")) {
            LOG.error("Uncompress::repackHap srcPath is invalid!");
            throw new BundleException("Repack hap failed, srcPath is invalid");
        }

        if (!UncompressVerify.isPathValid(destDirPath, false, null)) {
            LOG.error("Uncompress::repackHap destDirPath is invalid!");
            throw new BundleException("Repack hap failed, destDirPath is invalid");
        }

        String tempDir = destDirPath.replace(File.separator, LINUX_FILE_SEPARATOR) + LINUX_FILE_SEPARATOR +
                TEMP_PATH;
        dataTransferAllFiles(srcPath, tempDir);
        packFilesByPath(tempDir, destDirPath, fileName, unpackApk);
        File deleteFile = new File(tempDir);
        deleteFile(deleteFile);
    }

    /**
     * compress file directory.
     *
     * @param srcPath source file path
     * @param destDirPath destination file path
     * @param fileName target file name
     * @param unpackApk unpackApk flag
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void packFilesByPath(String srcPath, String destDirPath, String fileName, String unpackApk)
            throws BundleException {
        if (srcPath.isEmpty() || destDirPath.isEmpty() || fileName.isEmpty()) {
            LOG.error("Uncompress::packFilesByPath srcPath, destDirPath or fileName is empty!");
            throw new BundleException("Pack files by path failed, srcPath, destDirPath or fileName is empty");
        }

        if (!UncompressVerify.isPathValid(srcPath, false, null) ||
                !UncompressVerify.isPathValid(destDirPath, false, null)) {
            LOG.error("Uncompress::packFilesByPath srcPath or destDirPath is invalid!");
            throw new BundleException("Pack files by path failed, srcPath or destDirPath is invalid");
        }

        File srcDir = new File(srcPath);
        File[] srcFiles = srcDir.listFiles();
        if (srcFiles == null) {
            return;
        }
        FileOutputStream fileOut = null;
        CheckedOutputStream checkedOut = null;
        ZipOutputStream zipOut = null;

        try {
            String zipPath = destDirPath + LINUX_FILE_SEPARATOR + fileName;
            if (!FileUtils.matchPattern(zipPath)) {
                throw new BundleException("Input invalid file" + zipPath);
            }
            File zipFile = new File(zipPath);
            fileOut = new FileOutputStream(zipFile);
            checkedOut = new CheckedOutputStream(fileOut, new CRC32());
            zipOut = new ZipOutputStream(checkedOut);
            for (int i = 0; i < srcFiles.length; i++) {
                File srcFile = srcFiles[i];
                if (srcFile.isDirectory()) {
                    if (srcFile.getPath().toLowerCase(Locale.ENGLISH).endsWith(LIBS_DIR_NAME)) {
                        compressDirectory(srcFile, "", zipOut, true);
                    } else {
                        compressDirectory(srcFile, "", zipOut, false);
                    }
                } else {
                    if (srcFile.getPath().toLowerCase(Locale.ENGLISH).endsWith(APK_SUFFIX) &&
                            "true".equals(unpackApk)) {
                        continue;
                    }
                    compressFile(srcFile, "", zipOut, false);
                }
            }
        } catch (FileNotFoundException | BundleException exception) {
            LOG.error("Uncompress::packFilesByPath " + exception.getMessage());
            throw new BundleException("Pack files by path failed");
        } finally {
            Utility.closeStream(zipOut);
            Utility.closeStream(checkedOut);
            Utility.closeStream(fileOut);
        }
    }

    /**
     * compress file directory.
     *
     * @param dir file directory
     * @param baseDir base path for file
     * @param zipOut zip outPutStream
     * @param isCompression if need compression
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void compressDirectory(File dir, String baseDir, ZipOutputStream zipOut, boolean isCompression)
            throws BundleException {
        File[] files = dir.listFiles();
        if (files == null) {
            return;
        }
        for (File file : files) {
            if (file.isDirectory()) {
                compressDirectory(file, baseDir + dir.getName() + File.separator, zipOut, isCompression);
            } else {
                compressFile(file, baseDir + dir.getName() + File.separator, zipOut, isCompression);
            }
        }
    }

    /**
     * compress process.
     *
     * @param srcFile source file to zip
     * @param baseDir base path for file
     * @param zipOut zip outPutStream
     * @param isCompression if need compression
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void compressFile(File srcFile, String baseDir, ZipOutputStream zipOut, boolean isCompression)
            throws BundleException {
        FileInputStream fileInputStream = null;
        BufferedInputStream bufferedInputStream = null;

        try {
            String entryName = (baseDir + srcFile.getName()).replace(File.separator, LINUX_FILE_SEPARATOR);
            ZipEntry zipEntry = new ZipEntry(entryName);
            boolean isNeedCompress = isCompression;
            if (srcFile.isFile() && srcFile.getName().toLowerCase(Locale.ENGLISH).endsWith(SO_SUFFIX)) {
                isNeedCompress = false;
            }
            if (isNeedCompress) {
                zipEntry.setMethod(ZipEntry.DEFLATED);
            } else {
                zipEntry.setMethod(ZipEntry.STORED);
                zipEntry.setCompressedSize(srcFile.length());
                zipEntry.setSize(srcFile.length());
                CRC32 crc = getCrcFromFile(srcFile);
                zipEntry.setCrc(crc.getValue());
            }
            zipOut.putNextEntry(zipEntry);
            byte[] data = new byte[BUFFER_SIZE];
            fileInputStream = new FileInputStream(srcFile);
            bufferedInputStream = new BufferedInputStream(fileInputStream);
            int count = bufferedInputStream.read(data);
            while (count > 0) {
                zipOut.write(data, 0, count);
                count = bufferedInputStream.read(data);
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Uncompress::compressFile file not found exception: " + ignored.getMessage());
            throw new BundleException("Compress file failed");
        } catch (IOException exception) {
            LOG.error("Uncompress::compressFile io exception: " + exception.getMessage());
            throw new BundleException("Compress file failed");
        } finally {
            Utility.closeStream(fileInputStream);
            Utility.closeStream(bufferedInputStream);
        }
    }

    /**
     * get CRC32 from file.
     *
     * @param file source file
     * @return CRC32
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static CRC32 getCrcFromFile(File file) throws BundleException {
        FileInputStream fileInputStream = null;
        CRC32 crc = new CRC32();
        try {
            fileInputStream = new FileInputStream(file);
            byte[] buffer = new byte[BUFFER_SIZE];

            int count = fileInputStream.read(buffer);
            while (count > 0) {
                crc.update(buffer, 0, count);
                count = fileInputStream.read(buffer);
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Uncompressor::getCrcFromFile file not found exception");
            throw new BundleException("Get Crc from file failed");
        } catch (IOException exception) {
            LOG.error("Uncompressor::getCrcFromFile io exception: " + exception.getMessage());
            throw new BundleException("Get Crc from file failed");
        } finally {
            Utility.closeStream(fileInputStream);
        }
        return crc;
    }

    /**
     * delete file
     *
     * @param file the file to be deleted
     */
    private static void deleteFile(File file) {
        if (file.exists()) {
            if (file.isDirectory()) {
                File[] files = file.listFiles();
                for (int i = 0; i < files.length; i++) {
                    deleteFile(files[i]);
                }
            }
            file.delete();
        }
    }

    /**
     * check
     *
     * @param result the result of parse app all mode
     * @return return the result of checkParseAllResult.
     */
    private static UncompressResult checkParseAllResult(UncompressResult result) {
        UncompressResult errorResult = new UncompressResult();
        errorResult.setResult(false);
        errorResult.setMessage("App package is invalid.");
        if (result == null || result.getPackInfos() == null || result.getProfileInfos() == null) {
            return errorResult;
        }

        List<PackInfo> packInfos = result.getPackInfos();
        List<ProfileInfo> profileInfos = result.getProfileInfos();
        int packInfoSize = packInfos.size();
        int profileInfoSize = profileInfos.size();
        if (packInfoSize == 0 || profileInfoSize == 0 || packInfoSize != profileInfoSize) {
            LOG.error("Uncompress::checkParseAllResult error, hapNum is invalid in app");
            return errorResult;
        }

        for (int i = 0; i < packInfoSize; i++) {
            if (packInfos.get(i) == null || packInfos.get(i).name.isEmpty()) {
                return errorResult;
            }
            boolean isHave = false;
            for (int j = 0; j < profileInfoSize; j++) {
                if (profileInfos.get(j) == null || profileInfos.get(j).hapName.isEmpty()) {
                    return errorResult;
                }
                if (comparePackAndProfile(packInfos.get(i), profileInfos.get(j))) {
                    isHave = true;
                    break;
                }
            }
            if (!isHave) {
                return errorResult;
            }
        }

        return result;
    }

    /**
     * get label and icon for application
     *
     * @param result the result of parse app all mode
     * @return return the result which contains icon and label
     */
    private static UncompressResult obtainLabelAndIcon(UncompressResult result) {
        List<ProfileInfo> profileInfos = result.getProfileInfos();
        if (profileInfos.isEmpty()) {
            return result;
        }
        for (ProfileInfo profileInfo : profileInfos) {
            if (profileInfo == null) {
                continue;
            }
            HapInfo hapInfo = profileInfo.hapInfo;
            if (hapInfo == null) {
                continue;
            }
            Distro distro = hapInfo.distro;
            if (distro == null) {
                continue;
            }
            String moduleType = distro.moduleType;
            if (ENTRY_TYPE.equals(moduleType.toLowerCase(Locale.ENGLISH))) {
                return obtainInnerLabelAndIcon(result, hapInfo, distro);
            }
        }
        return result;
    }

    /**
     * get label and icon for application
     *
     * @param result the result of parse app all mode
     * @param hapInfo hap info of entry hap
     * @return return the result which contains icon and label
     */
    private static UncompressResult obtainInnerLabelAndIcon(UncompressResult result, HapInfo hapInfo, Distro distro) {
        List<AbilityInfo> abilities = hapInfo.abilities;
        if ((abilities == null) || (abilities.isEmpty())) {
            result.setLabel(distro.moduleName);
            return result;
        }
        int size = 0;
        for (AbilityInfo info : abilities) {
            if (info == null) {
                size++;
                continue;
            }
            if ((info.skills == null) || (info.skills.isEmpty())) {
                continue;
            }
            for (SkillInfo skill : info.skills) {
                if (skill == null) {
                    continue;
                }
                List<String> actions = skill.actions;
                List<String> entities = skill.entities;
                if ((!actions.isEmpty()) && (actions.contains(SYSTEM_ACTION) || actions.contains(SYSTEM_WANT_HOME))) {
                    if ((!entities.isEmpty()) && (entities.contains(SYSTEM_ENTITY))) {
                        result.setLabel(info.label);
                        result.setIcon(info.icon);
                        return result;
                    }
                }
            }
        }
        if (size == abilities.size()) {
            result.setLabel(distro.moduleName);
            return result;
        }
        for (AbilityInfo info : abilities) {
            if (info != null) {
                result.setLabel(info.label);
                result.setIcon(info.icon);
                break;
            }
        }
        return result;
    }


    private static boolean isHarmonyProfile(String fileName) {
        return HARMONY_PROFILE.equals(fileName);
    }

    private static boolean isPackInfo(String fileName) {
        return PACK_INFO.equals(fileName);
    }

    private static String getHapNameWithoutSuffix(String hapFileName) {
        if (hapFileName == null || hapFileName.isEmpty() || hapFileName.lastIndexOf(".") == -1) {
            return "";
        }
        return hapFileName.substring(0, hapFileName.lastIndexOf("."));
    }

    private static HapZipInfo unZipModuleHapFile(String srcPath)
            throws BundleException, IOException {
        HapZipInfo hapZipInfo = new HapZipInfo();
        ZipFile zipFile = null;
        try {
            File srcFile = new File(srcPath);
            zipFile = new ZipFile(srcFile);
            getProfileJson(zipFile, hapZipInfo.resourcemMap);
            hapZipInfo.setHarmonyProfileJsonStr(FileUtils.getFileStringFromZip(MODULE_JSON, zipFile));
            hapZipInfo.setPackInfoJsonStr(FileUtils.getFileStringFromZip(PACK_INFO, zipFile));
            hapZipInfo.setResDataBytes(getResourceDataFromHap(zipFile));
            hapZipInfo.setHapFileName(getHapNameWithoutSuffix(srcFile.getName()));
        } finally {
            Utility.closeStream(zipFile);
        }
        return hapZipInfo;
    }

    /**
     * uncompress from HapZipInfo
     *
     * @param hapZipInfo hap zip info
     * @return the parse moduleResult
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void uncompressModuleJsonInfo(HapZipInfo hapZipInfo, ModuleResult moduleResult)
            throws BundleException {
        ModuleProfileInfo moduleProfileInfo = JsonUtil.parseModuleProfileInfo(hapZipInfo.getHarmonyProfileJsonStr(),
                hapZipInfo.getResDataBytes(), hapZipInfo.getPackInfoJsonStr(), hapZipInfo.getHapFileName(),
                hapZipInfo.resourcemMap);
        moduleProfileInfo.hapName = hapZipInfo.getHapFileName();
        moduleResult.addModuleProfileInfo(moduleProfileInfo);
        moduleResult.moduleProfileStr.add(hapZipInfo.getHarmonyProfileJsonStr());
    }

    /**
     * uncompress from specified file name
     *
     * @param deviceType device type
     * @param srcPath source file path
     * @param fileName uncompress file name
     * @return the uncompress result
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static ModuleResult uncompressModule(String deviceType, String srcPath, String fileName)
            throws BundleException {
        if (srcPath.isEmpty() || fileName.isEmpty()) {
            LOG.error("Uncompress::uncompressModule srcPath, fileName is empty!");
            throw new BundleException("uncompressModule failed, srcPath or fileName is empty");
        }
        ModuleResult moduleResult = new ModuleResult();
        try {
            HapZipInfo hapZipInfo = unZipModuleHapFile(srcPath);
            uncompressModuleJsonInfo(hapZipInfo, moduleResult);
            if (moduleResult.packInfos.isEmpty() && !hapZipInfo.getPackInfoJsonStr().isEmpty()) {
                moduleResult.packInfos = JsonUtil.parsePackInfos(hapZipInfo.getPackInfoJsonStr());
            }
        } catch (IOException exception) {
            moduleResult.setResult(false);
            LOG.error("Uncompress::uncompressModule parseMode is invalid!");
        }
        return moduleResult;
    }

    /**
     * uncompress module hap.
     *
     * @param deviceType indicates the device type of uncompress mode.
     * @param srcPath indicates the path type of hap.
     * @param fileName indicates json file name.
     * @return the uncompress result
     */
    static UncompressResult unCompressModuleHap(String deviceType, String srcPath, String fileName)
            throws BundleException{
        if (srcPath.isEmpty() || fileName.isEmpty()) {
            LOG.error("Uncompress::uncompress srcPath, fileName is empty!");
            throw new BundleException("Uncompress failed, srcPath or fileName is empty");
        }
        UncompressResult uncomperssResult = new UncompressResult();
        ModuleResult moduleResult = new ModuleResult();
        try {
            moduleResult = uncompressModule(deviceType, srcPath, fileName);
            ModuleAdaption moduleAdaption = new ModuleAdaption();
            uncomperssResult = moduleAdaption.convertToUncompressResult(moduleResult);
        } catch (BundleException ignored) {
            LOG.error("Uncompress::uncompressHap Bundle exception");
            uncomperssResult.setResult(false);
            uncomperssResult.setMessage("uncompressHap Bundle exception");
        }
        return uncomperssResult;
    }
    /**
     * get all resource in profile.
     *
     * @param zipFile is the hap file
     * @return the parse resource result
     */
    static void getProfileJson(ZipFile zipFile, HashMap<String, String> resourceMap) throws BundleException {
        try {
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (entry.getName().contains(RESOURCE_PATH)) {
                    String filePath = entry.getName();
                    String fileName = filePath.replaceAll(RESOURCE_PATH, "");
                    String fileContent = FileUtils.getFileStringFromZip(filePath, zipFile);
                    resourceMap.put(fileName, fileContent);
                }
            }
        } catch (IOException e) {
            LOG.error("Uncompress::getProfileJson IOException");
            throw new BundleException("Uncompress::getProfileJson failed");
        }
    }

    /**
     * uncompress module hap.
     *
     * @param deviceType indicates the deviceType of parse hap.
     * @param input the InputStream about the app package.
     * @param fileName the file name of json file.
     * @return the uncompress result
     */
    static UncompressResult uncompressModuleHapByInput(String deviceType,
                                                       InputStream input, String fileName, String hapName) {
        UncompressResult uncompressResult = new UncompressResult();
        ModuleResult moduleResult = new ModuleResult();
        try {
            moduleResult = uncompressModuleByInput(deviceType, input, MODULE_JSON, hapName);
            ModuleAdaption moduleAdaption = new ModuleAdaption();
            uncompressResult = moduleAdaption.convertToUncompressResult(moduleResult);
        } catch (BundleException ignored) {
            LOG.error("Uncompress::uncompressHapByInput Bundle exception");
            uncompressResult.setResult(false);
            uncompressResult.setMessage("uncompressHapByInput Bundle exception");
        }
        return uncompressResult;
    }

    /**
     * unzip module hap from zip file.
     *
     * @param input Indicates the InputStream about the package.
     * @return Return the uncomperss result of parseHap
     */
    private static HapZipInfo unZipModuleHapFileFromInputStream(InputStream input) throws BundleException {
        BufferedInputStream bufIn = null;
        ZipInputStream zipIn = null;
        BufferedReader bufferedReader = null;
        HapZipInfo hapZipInfo = new HapZipInfo();
        try {
            ZipEntry entry = null;
            bufIn = new BufferedInputStream(input);
            zipIn = new ZipInputStream(bufIn);
            while ((entry = zipIn.getNextEntry()) != null) {
                if (entry.getName().toLowerCase().endsWith(RESOURCE_INDEX)) {
                    hapZipInfo.setResDataBytes(getByte(zipIn));
                    continue;
                }
                if (isPackInfo(entry.getName())) {
                    bufferedReader = new BufferedReader(new InputStreamReader(zipIn));
                    hapZipInfo.setPackInfoJsonStr(readStringFromInputStream(zipIn, bufferedReader));
                    continue;
                }
                if (MODULE_JSON.equals(entry.getName())) {
                    bufferedReader = new BufferedReader(new InputStreamReader(zipIn));
                    hapZipInfo.setHarmonyProfileJsonStr(readStringFromInputStream(zipIn, bufferedReader));
                }
                if (entry.getName().contains(RESOURCE_PATH)) {
                    bufferedReader = new BufferedReader(new InputStreamReader(zipIn));
                    String filePath = entry.getName();
                    String fileName = filePath.replaceAll(RESOURCE_PATH, "");
                    String fileContent = readStringFromInputStream(zipIn, bufferedReader);
                    hapZipInfo.pushResourceMap(fileName, fileContent);
                }
            }
        } catch (BundleException | IOException e) {
            LOG.error("unZipModuleHapFileFromInputStream failed!");
            throw new BundleException("unZipModuleHapFileFromInputStream failed!");
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(bufIn);
            Utility.closeStream(zipIn);
        }
        return hapZipInfo;
    }

    /**
     * Parse the hap type.
     *
     * @param hapPath Indicates the hap path.
     * @param compressResult Indicates the result of parse hap.
     * @return Return the type result of isModuleHap
     */
    public static boolean isModuleHap(String hapPath, UncompressResult compressResult) {
        ZipFile zipFile = null;
        try {
            zipFile = new ZipFile(new File(hapPath));
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (MODULE_JSON.equals(entry.getName())) {
                    return true;
                }
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Uncompress::isModuleHap file not found exception");
            compressResult.setResult(false);
            compressResult.setMessage("judge is module failed");
        } catch (IOException exception) {
            LOG.error("Uncompress::isModuleHap io exception: " + exception.getMessage());
            compressResult.setResult(false);
            compressResult.setMessage("judge is module failed");
        } finally {
            Utility.closeStream(zipFile);
        }
        return false;
    }

    /**
     * Parse the hap type.
     *
     * @param input Indicates the hap FileInputStream.
     * @return Return the type result of isModuleHap.
     */
    public static boolean isModuleInput(InputStream input) throws BundleException {
        BufferedInputStream bufIn = null;
        ZipInputStream zipIn = null;
        BufferedReader bufferedReader = null;
        try {
            ZipEntry entry = null;
            bufIn = new BufferedInputStream(input);
            zipIn = new ZipInputStream(bufIn);
            while((entry = zipIn.getNextEntry()) != null) {
                if (entry.getName().toLowerCase().endsWith(MODULE_JSON)) {
                    return true;
                }
            }
        } catch (IOException ignored) {
            LOG.error("Uncompress::isModuleHap judge failed!");
            throw new BundleException("Uncompress::isModuleHap judge failed!");
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(bufIn);
            Utility.closeStream(zipIn);
        }
        return false;
    }

    /**
     * create file output stream from InputStream .
     *
     * @param stream Indicates the input stream of hap.
     * @param file Indicates the temp file to save input stream.
     */
    static void writeToTempFile(InputStream stream, File file) throws BundleException {
        FileOutputStream fileOutputStream = null;
        try {
            if (file == null) {
                LOG.error("file not exist!");
            }
            fileOutputStream = new FileOutputStream(file);
            int bytesRead = 0;
            byte[] buffer = new byte[1024];
            while ((bytesRead = stream.read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, bytesRead);
            }
        } catch (IOException e) {
            LOG.error("writeToTempFile failed!");
            throw new BundleException("writeToTempFile failed!");
        } finally {
            Utility.closeStream(fileOutputStream);
        }
    }

    /**
     * copy rpcid.sc file.
     *
     * @param srcFile Indicates the path of hap.
     * @param rpcidPath Indicates the output path of rpcid.sc file.
     */
    static void getRpcidFromHap(String srcFile, String rpcidPath) throws BundleException {
        ZipFile zipFile = null;
        InputStream inputStream = null;
        FileOutputStream outputStream = null;
        try {
            zipFile = new ZipFile(srcFile);
            String filePath = null;
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (RPCID_SC.equals(entry.getName())) {
                    filePath = entry.getName();
                    break;
                }
            }
            if (filePath != null) {
                File rpcidFile = new File(rpcidPath, RPCID_SC);
                if (rpcidFile.getParentFile() != null && !rpcidFile.getParentFile().exists()) {
                    rpcidFile.getParentFile().mkdirs();
                }
                ZipEntry rpcidEntry = zipFile.getEntry(filePath);
                inputStream = zipFile.getInputStream(rpcidEntry);
                byte[] buffer = new byte[1024];
                int noBytes = 0;
                outputStream = new FileOutputStream(rpcidFile);
                while((noBytes = inputStream.read(buffer)) != -1) {
                    outputStream.write(buffer, 0, noBytes);
                }
            } else {
                LOG.error("Uncompress::getRpcidFromHap hap has no rpcid.sc file");
                throw new BundleException("Uncompress::getRpcidFromHap hap has no rpcid.sc file");
            }
        } catch (IOException e) {
            LOG.error("Uncompress::getRpcidFromHap IOException " + e.getMessage());
            throw new BundleException("Uncompress::getRpcidFromHap failed");
        } finally {
            Utility.closeStream(zipFile);
            Utility.closeStream(inputStream);
            Utility.closeStream(outputStream);
        }
    }

    /**
     * unpacking according to the architecture
     *
     * @param srcFile Indicates the path of hap or hsp.
     * @param outPath Indicates the output path of unpacking target path.
     * @param cpuAbiList Indicates the designated cpuAbi.
     */
    static void unpackageLibsMode(String srcFile, String outPath, List<String> cpuAbiList) throws BundleException {
        String normalizedSrcFilePath = srcFile.replace("\\", LINUX_FILE_SEPARATOR);
        String normalizedOutFilePath = outPath.replace("\\", LINUX_FILE_SEPARATOR);
        String tempDir = normalizedOutFilePath + LINUX_FILE_SEPARATOR + TEMP_PATH;
        try {
            List<String> cpuAbiListRes = getLibsFromPackageAndUnpackage(normalizedSrcFilePath, tempDir, cpuAbiList);
            repackPackage(normalizedSrcFilePath, normalizedOutFilePath, tempDir, cpuAbiListRes);
        } catch (BundleException  e) {
            LOG.error("Uncompress::unpackageLibsMode failed.");
            throw new BundleException("Uncompress::unpackageLibsMode failed");
        } finally {
            File deleteFile = new File(tempDir);
            deleteFile(deleteFile);
        }
    }

    /**
     * get libs directory and unpack other files.
     *
     * @param srcFile Indicates the path of hap or hsp.
     * @param tempDirPath Indicates the temporary directory path.
     * @param cpuAbiList Indicates the Designated cpuAbi.
     */
    static List<String> getLibsFromPackageAndUnpackage(String srcFile, String tempDirPath, List<String> cpuAbiList)
            throws BundleException {
        try (ZipFile zipFile = new ZipFile(srcFile)) {
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            Set<String> cpuAbiSetRes = new HashSet<>();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (entry.getName().startsWith(LIBS + LINUX_FILE_SEPARATOR)) {
                    String cpuAbi = entry.getName().substring(LIBS.length() + 1).split(LINUX_FILE_SEPARATOR)[0];
                    if (!cpuAbiList.isEmpty() && !cpuAbiList.contains(cpuAbi)) {
                        continue;
                    }
                    cpuAbiSetRes.add(cpuAbi);
                    File tempFile = new File(tempDirPath, entry.getName().substring(LIBS.length() + 1));
                    FileUtils.createParentDir(tempFile);
                    try (InputStream inputStream = zipFile.getInputStream(entry);
                         FileOutputStream outputStream = new FileOutputStream(tempFile)) {
                        FileUtils.copyStream(inputStream, outputStream);
                    } catch (IOException e) {
                        LOG.error("Uncompress::getLibsFromPackageAndUnpackage IOException " + e.getMessage());
                        throw new BundleException("Uncompress::getLibsFromPackageAndUnpackage failed");
                    }
                } else {
                    String tempPath = tempDirPath + LINUX_FILE_SEPARATOR + PACKAGEFILE + LINUX_FILE_SEPARATOR
                            + entry.getName();
                    File destFile = new File(tempPath);
                    FileUtils.createParentDir(destFile);
                    dataTransfer(zipFile, entry, destFile);
                }
            }
            if (cpuAbiSetRes.size() != cpuAbiList.size() && !cpuAbiList.isEmpty()) {
                List<String> notExistAbi = cpuAbiList.stream()
                        .filter(item -> !cpuAbiSetRes.contains(item))
                        .collect(Collectors.toList());
                LOG.error("The specified abi does not exist, " + notExistAbi);
                throw new BundleException("Uncompress::getLibsFromPackageAndUnpackage failed");
            }
            if (cpuAbiSetRes.size() > MAX_CPU_ABI_TYPE_NUM && cpuAbiList.isEmpty()) {
                LOG.error("Uncompress::getLibsFromPackageAndUnpackage failed: the architecture type exceeds the " +
                        "limit and must be less than or equal to 128");
                throw new BundleException("Uncompress::getLibsFromPackageAndUnpackage failed");
            }
            if (cpuAbiSetRes.isEmpty()) {
                LOG.error("Uncompress::getLibsFromPackageAndUnpackage failed: libs has no CPU ABI");
                throw new BundleException("Uncompress::getLibsFromPackageAndUnpackage failed");
            }
            return new ArrayList<>(cpuAbiSetRes);
        } catch (IOException e) {
            LOG.error("Uncompress::getLibsFromPackageAndUnpackage IOException " + e.getMessage());
            throw new BundleException("Uncompress::getLibsFromPackageAndUnpackage failed");
        }
    }

    /**
     * repack Package
     *
     * @param srcPath source file path
     * @param outPath out file path
     * @param tempDir temp file name
     * @param cpuAbiList cpuAbi list
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void repackPackage(String srcPath, String outPath, String tempDir, List<String> cpuAbiList)
            throws BundleException {
        File srcDir = new File(tempDir + LINUX_FILE_SEPARATOR + PACKAGEFILE);
        File[] srcFiles = srcDir.listFiles();
        if (srcFiles == null) {
            return;
        }
        String[] srcPathItem = srcPath.split(LINUX_FILE_SEPARATOR);
        if (srcPathItem.length == 0) {
            LOG.error("Uncompress::repackPackage failed: Wrong file path");
            throw new BundleException("Uncompress::repackPackage failed");
        }
        String hapName = srcPathItem[srcPathItem.length - 1];
        for (String cpuAbi : cpuAbiList) {
            processCpuAbi(outPath, hapName, cpuAbi, srcFiles, tempDir);
        }
    }

    private static void processCpuAbi(String outPath, String hapName, String cpuAbi, File[] srcFiles, String tempDir)
            throws BundleException {
        String targetZipPath = outPath + LINUX_FILE_SEPARATOR + cpuAbi + LINUX_FILE_SEPARATOR + hapName;
        File targetZipFile = new File(targetZipPath);
        if (targetZipFile.getParentFile() != null && !targetZipFile.getParentFile().exists()) {
            targetZipFile.getParentFile().mkdirs();
        }
        try (ZipOutputStream targetZipOutputStream = new ZipOutputStream(new FileOutputStream(targetZipPath))) {
            compressSourceFiles(srcFiles, targetZipOutputStream);
            File libsDir = new File(tempDir + LINUX_FILE_SEPARATOR + cpuAbi);
            compressDirectory(libsDir, LIBS + LINUX_FILE_SEPARATOR, targetZipOutputStream, true);
        } catch (IOException e) {
            LOG.error("Uncompress::processCpuAbi IOException for path: " + targetZipPath + " - " + e.getMessage());
            throw new BundleException("Uncompress::processCpuAbi failed for path: " + targetZipPath);
        }
    }

    private static void compressSourceFiles(File[] srcFiles, ZipOutputStream targetZipOutputStream)
            throws IOException, BundleException{
        for (File srcFile : srcFiles) {
            if (srcFile.isDirectory()) {
                compressDirectory(srcFile, "", targetZipOutputStream, false);
            } else {
                compressFile(srcFile, "", targetZipOutputStream, false);
            }
        }
    }

    /**
     * parse resource.index file.
     *
     * @param srcPath Indicates the path of hap.
     */
    static List<ResourceIndexResult> getResourceFromHap(String srcPath) throws BundleException, IOException {
        ZipFile zipFile = null;
        try {
            File srcFile = new File(srcPath);
            zipFile = new ZipFile(srcFile);
            byte[] data = getResourceDataFromHap(zipFile);
            return ResourcesParser.getAllDataItem(data);
        } finally {
            Utility.closeStream(zipFile);
        }
    }

    /**
     * uncompress appqf file.
     *
     * @param utility is the common args for input.
     * @throws BundleException if uncompress failed.
     */
    private static void uncompressAPPQFFile(Utility utility) throws BundleException {
        ZipFile zipFile = null;
        try {
            zipFile = new ZipFile(new File(utility.getAPPQFPath()));
            int entriesNum = 0;
            for (Enumeration<? extends ZipEntry> entries = zipFile.entries(); entries.hasMoreElements();) {
                entriesNum++;
                ZipEntry entry = entries.nextElement();
                if (entry == null) {
                    continue;
                }
                String filePath = utility.getOutPath() + File.separator + entry.getName();
                if (!FileUtils.matchPattern(filePath)) {
                    LOG.error("uncompressAPPQFFile: Input invalid file" + filePath);
                    throw new BundleException("uncompressAPPQFFile: Input invalid file" + filePath);
                }
                File destFile = new File(filePath);
                if (destFile != null && destFile.getParentFile() != null && !destFile.getParentFile().exists()) {
                    destFile.getParentFile().mkdirs();
                }
                dataTransfer(zipFile, entry, destFile);
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Uncompress::uncompressAPPQFFile file not found exception");
            throw new BundleException("Uncompress::uncompressAPPQFFile file not found");
        } catch (IOException exception) {
            LOG.error("Uncompress::uncompressAPPQFFile io exception");
            throw new BundleException("Uncompress::uncompressAPPQFFile io exception");
        } finally {
            Utility.closeStream(zipFile);
        }
    }

    /**
     * parse appqf file, .
     *
     * @param appqfPath is the path of appqf file.
     * @throws BundleException if uncompress failed.
     * @throws IOException if IOException happened.
     */
    public static List<HQFInfo> parseAPPQFFile(String appqfPath) throws BundleException {
        List<String> patchList = new ArrayList<>();
        ZipFile zipFile = null;
        InputStream stream = null;
        ZipInputStream zipInputStream = null;
        try {
            zipFile = new ZipFile(appqfPath);
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                ZipEntry appqfEntry = entries.nextElement();
                stream = zipFile.getInputStream(appqfEntry);
                if (!appqfEntry.getName().endsWith(HQF_SUFFIX)) {
                    continue;
                }
                zipInputStream = new ZipInputStream(stream);
                String patchJson = readPatchJson(zipInputStream);
                if (patchJson == null) {
                    continue;
                }
                patchList.add(patchJson);
            }
        } catch (IOException e) {
            LOG.error("parseAPPQFFile failed!");
            throw new BundleException("parseAPPQFFile failed!");
        } finally {
            Utility.closeStream(zipFile);
            Utility.closeStream(stream);
            Utility.closeStream(zipInputStream);
        }
        return parsePatchJson(patchList);
    }

    private static String readPatchJson(ZipInputStream zipInputStream) throws BundleException {
        String patchJson = null;
        ZipEntry hqfEntry;
        try {
            while ((hqfEntry = zipInputStream.getNextEntry()) != null) {
                if (!PATCH_JSON.equals(hqfEntry.getName())) {
                    continue;
                }
                patchJson = new BufferedReader(new InputStreamReader(zipInputStream,
                        StandardCharsets.UTF_8)).lines().parallel()
                        .collect(Collectors.joining(System.lineSeparator()));
            }
        } catch (IOException e) {
            LOG.error("readPatchJson failed!");
            throw new BundleException("readPatchJson failed!");
        }
        return patchJson;
    }

    private static List<HQFInfo> parsePatchJson(List<String> patchList) throws BundleException {
        List<HQFInfo> hqfInfoList = new ArrayList<>();
        for (String patchJson : patchList) {
            hqfInfoList.add(JsonUtil.parsePatch(patchJson));
        }
        return hqfInfoList;
    }

    private static boolean comparePackAndProfile(PackInfo packInfo, ProfileInfo profileInfo) {
        if (profileInfo.hapName.replace(HAP_SUFFIXI, "").equals(packInfo.name.replace(HAP_SUFFIXI, ""))) {
            return true;
        }
        if (profileInfo.hapName.replace(HSP_SUFFIX, "").equals(packInfo.name.replace(HSP_SUFFIX, ""))) {
            return true;
        }
        return false;
    }
}
