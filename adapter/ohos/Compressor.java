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
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import java.nio.charset.StandardCharsets;
import java.nio.file.attribute.FileTime;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.zip.CRC32;
import java.util.zip.CheckedOutputStream;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;


/**
 * bundle compressor class, compress file and directory.
 *
 */
public class Compressor {
    private static final String HAP_SUFFIX = ".hap";
    private static final String HSP_SUFFIX = ".hsp";
    private static final String PNG_SUFFIX = ".png";
    private static final String UPPERCASE_PNG_SUFFIX = ".PNG";
    private static final String CONFIG_JSON = "config.json";
    private static final String MODULE_JSON = "module.json";
    private static final String NAME = "name";
    private static final String NULL_DIR_NAME = "";
    private static final String RES_DIR_NAME = "res/";
    private static final String RESOURCES_DIR_NAME = "resources/";
    private static final String LIBS_DIR_NAME = "libs/";
    private static final String AN_DIR_NAME = "an/";
    private static final String AP_PATH_NAME = "ap/";
    private static final String ASSETS_DIR_NAME = "assets/";
    private static final String SO_DIR_NAME = "maple/";
    private static final String SO_ARM64_DIR_NAME = "maple/arm64/";
    private static final String LINUX_FILE_SEPARATOR = "/";
    private static final String DISTRO = "distro";
    private static final String FORMS = "forms";
    private static final String MODULE_NAME = "module-name";
    private static final String MODULE_NAME_NEW = "moduleName";
    private static final String JSON_END = "}";
    private static final String SEMICOLON = "\"";
    private static final String COMPRESS_NATIVE_LIBS = "compressNativeLibs";
    private static final String SHARED_LIBS_DIR_NAME = "shared_libs/";
    private static final String DEVICE_TYPE = "deviceType";
    private static final String DEVICE_TYPE_FITNESSWATCH = "fitnessWatch";
    private static final String DEVICE_TYPE_FITNESSWATCH_NEW = "liteWearable";
    private static final String ENTRYCARD_NAME = "EntryCard/";
    private static final String PACKINFO_NAME = "pack.info";
    private static final String ENTRYCARD_BASE_NAME = "base";
    private static final String ENTRYCARD_SNAPSHOT_NAME = "snapshot";
    private static final String PIC_1X2 = "1x2";
    private static final String PIC_2X2 = "2x2";
    private static final String PIC_2X4 = "2x4";
    private static final String PIC_4X4 = "4x4";
    private static final String REGEX_LANGUAGE = "^[a-z]{2}$";
    private static final String REGEX_SCRIPT = "^[A-Z][a-z]{3}$";
    private static final String REGEX_COUNTRY = "^[A-Z]{2,3}|[0-9]{3}$";
    private static final String REGEX_ORIENTATION = "^vertical|horizontal$";
    private static final String REGEX_DEVICE_TYPE = "^phone|tablet|car|tv|wearable|liteWearable$";
    private static final String REGEX_SCREEN_DENSITY = "^sdpi|mdpi|ldpi|xldpi|xxldpi$";
    private static final String REGEX_COLOR_MODE = "^light|dark$";
    private static final String REGEX_SHAPE = "^circle$";
    private static final String JS_PATH = "js/";
    private static final String ETS_PATH = "ets/";
    private static final String TEMP_HAP_DIR = "tempHapDir";
    private static final String TEMP_HSP_DIR = "tempHspDir";
    private static final String TEMP_SELECTED_HAP_DIR = "tempSelectedHapDir";
    private static final String EMPTY_STRING = "";
    private static final String RELEASE = "Release";
    private static final String TYPE_SHARED = "shared";
    private static final Integer ONE = 1;
    private static final String ATOMIC_SERVICE = "atomicService";

    // set timestamp to get fixed MD5
    private static final long FILE_TIME = 1546272000000L;
    private static final int ENTRY_FILE_LIMIT_DEFAULT = 2;
    private static final int NOT_ENTRY_FILE_LIMIT_DEFAULT = 2;
    private static final int TOTAL_FILE_LIMIT_DEFAULT = 10;
    private static final int FILE_LIMIT = 10;

    // set buffer size of each read
    private static final int BUFFER_SIZE = 10 * 1024;
    private static final Log LOG = new Log(Compressor.class.toString());
    private static int entryModuleSizeLimit = 2;
    private static int notEntryModuleSizeLimit = 2;
    private static int sumModuleSizeLimit = 10;
    private static boolean isOverlay = false;
    private ZipOutputStream zipOut = null;
    private boolean mIsContain2x2EntryCard = true;
    private List<String> list = new ArrayList<String>();
    private List<String> formNamesList = new ArrayList<String>();
    private List<String> fileNameList = new ArrayList<String>();
    private List<String> supportDimensionsList = Arrays.asList(PIC_1X2, PIC_2X2, PIC_2X4, PIC_4X4);

    public static int getEntryModuleSizeLimit() {
        return entryModuleSizeLimit;
    }

    public static void setEntryModuleSizeLimit(int entry) {
        entryModuleSizeLimit = entry;
    }

    public static int getNotEntryModuleSizeLimit() {
        return notEntryModuleSizeLimit;
    }

    public static void setNotEntryModuleSizeLimit(int notEntry) {
        notEntryModuleSizeLimit = notEntry;
    }

    public static int getSumModuleSizeLimit() {
        return sumModuleSizeLimit;
    }

    public static void setSumModuleSizeLimit(int sumModule) {
        sumModuleSizeLimit = sumModule;
    }

    /**
     * parse file size limit from utility.
     *
     * @param utility Indicates the utility.
     */
    public void parseFileSizeLimit(Utility utility) throws BundleException {
        int sumLimit = TOTAL_FILE_LIMIT_DEFAULT;
        String totalLimit = utility.getTotalLimit();
        if (!totalLimit.isEmpty()) {
            sumLimit = Integer.parseInt(totalLimit);
            if (sumLimit <= 0 || sumLimit > FILE_LIMIT) {
                LOG.error("parseFileSizeLimit failed, input total-limit invalid.");
                throw new BundleException("parseFileSizeLimit failed, input total-limit invalid.");
            }
        }

        String normalLimit = utility.getNormalModuleLimit();
        int notEntry = NOT_ENTRY_FILE_LIMIT_DEFAULT;
        if (!normalLimit.isEmpty()) {
            notEntry = Integer.parseInt(normalLimit);
            if (notEntry <= 0 || notEntry > sumLimit || notEntry > FILE_LIMIT) {
                LOG.error("parseFileSizeLimit failed, input normal-module-limit invalid.");
                throw new BundleException("parseFileSizeLimit failed, input normal-module-limit invalid.");
            }
        }

        String mainLimit = utility.getMainModuleLimit();
        int entryLimit = ENTRY_FILE_LIMIT_DEFAULT;
        if (!mainLimit.isEmpty()) {
            entryLimit = Integer.parseInt(mainLimit);
            if (entryLimit <= 0 || entryLimit > sumLimit || entryLimit > FILE_LIMIT) {
                LOG.error("parseFileSizeLimit failed, input main-module-limit invalid.");
                throw new BundleException("parseFileSizeLimit failed, input main-module-limit invalid.");
            }
        }

        setEntryModuleSizeLimit(entryLimit);
        setNotEntryModuleSizeLimit(notEntry);
        setSumModuleSizeLimit(sumLimit);
    }

    /**
     * check path if is a module.json file
     *
     * @param path   path input
     * @return true if path is a module file
     */
    private static boolean isModuleJSON(String path)
    {
        File file = new File(path);
        if ((file.isFile()) && MODULE_JSON.equals(file.getName())) {
            return true;
        }
        return false;
    }

    /**
     * start compress.
     * file orders as follows:
     * for hap: 1.config.json 2.lib 3.res 4.assets 5.*.so 6.*.dex 7.*.apk 8.resources.index
     * for app: 1.certificate 2.signature 3.pack.info 4.hap (1 and 2 may not be used)
     *
     * @param utility common data
     * @return compressProcess if compress succeed
     */
    public boolean compressProcess(Utility utility) {
        boolean compressResult = true;
        File destFile = new File(utility.getOutPath());

        // if out file directory not exist, mkdirs.
        File outParentFile = destFile.getParentFile();
        if ((outParentFile != null) && (!outParentFile.exists())) {
            if (!outParentFile.mkdirs()) {
                LOG.error("Compressor::compressProcess create out file parent directory failed.");
                return false;
            }
        }

        FileOutputStream fileOut = null;
        CheckedOutputStream checkedOut = null;
        try {
            fileOut = new FileOutputStream(destFile);
            checkedOut = new CheckedOutputStream(fileOut, new CRC32());
            zipOut = new ZipOutputStream(checkedOut);
            compressExcute(utility);
        } catch (FileNotFoundException exception) {
            compressResult = false;
            LOG.error("Compressor::compressProcess file not found exception" + exception.getMessage());
        } catch (BundleException ignored) {
            compressResult = false;
            LOG.error("Compressor::compressProcess Bundle exception.");
        } finally {
            closeZipOutputStream();
            Utility.closeStream(zipOut);
            Utility.closeStream(checkedOut);
            Utility.closeStream(fileOut);
        }
        // if compress failed, delete out file.
        if (!compressResult) {
            LOG.error("Compressor::compressProcess compress failed.");
            if (!destFile.delete()) {
                LOG.error("Compressor::compressProcess delete dest file failed.");
            }
        }
        return compressResult;
    }

    private void compressExcute(Utility utility) throws BundleException {
        switch (utility.getMode()) {
            case Utility.MODE_HAP:
                compressHap(utility);
                break;
            case Utility.MODE_HAR:
                compressHarMode(utility);
                break;
            case Utility.MODE_APP:
                compressAppMode(utility);
                break;
            case Utility.MODE_MULTI_APP:
                compressAppModeForMultiProject(utility);
                break;
            case Utility.MODE_HQF:
                compressHQFMode(utility);
                break;
            case Utility.MODE_APPQF:
                compressAPPQFMode(utility);
                break;
            case Utility.MODE_HSP:
                compressHsp(utility);
                break;
            default:
                compressPackResMode(utility);
        }
    }

    private void compressHsp(Utility utility) throws BundleException {
        if (isModuleJSON(utility.getJsonPath())) {
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            String jsonString = optional.get();
            if (!checkStageAtomicService(jsonString)) {
                LOG.error("checkStageAtomicService failed.");
                throw new BundleException("checkStageAtomicService failed.");
            }
            // check whether is an overlay hsp or not
            if (!checkStageOverlayCfg(jsonString)) {
                LOG.error("checkStageOverlayCfg failed.");
                throw new BundleException("checkStageOverlayCfg failed.");
            }
            String moduleType = ModuleJsonUtil.parseModuleType(jsonString);
            if (!TYPE_SHARED.equals(moduleType)) {
                LOG.error("module type must be shared.");
                throw new BundleException("compressHsp failed.");
            }
        }
        compressHSPMode(utility);
    }

    private void compressHap(Utility utility) throws BundleException {
        if (isModuleJSON(utility.getJsonPath())) {
            if (!checkStageHap(utility)) {
                LOG.error("checkStageHap failed.");
                throw new BundleException("checkStageHap failed.");
            }
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            String jsonString = optional.get();
            String moduleType = ModuleJsonUtil.parseModuleType(jsonString);
            if (TYPE_SHARED.equals(moduleType)) {
                LOG.warning("Compress mode is hap, but module type is shared.");
            }
            String bundleType = ModuleJsonUtil.parseStageBundleType(jsonString);
            if (TYPE_SHARED.equals(bundleType)) {
                LOG.warning("Compress mode is hap, but app type is shared.");
            }
            compressHapModeForModule(utility);
        } else {
            if (!checkFAHap(utility)) {
                LOG.error("checkFAHap failed.");
                throw new BundleException("checkStageHap failed.");
            }
            compressHapMode(utility);
        }
    }

    private static boolean checkStageHap(Utility utility) throws BundleException {
        Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
        String jsonString = optional.get();
        if (!checkStageAsanEnabledValid(jsonString)) {
            LOG.error("checkStageAsanEnabledValid failed.");
            return false;
        }
        // check atomicService in module.json
        if (!checkStageAtomicService(jsonString)) {
            LOG.error("checkStageAtomicService failed.");
            return false;
        }
        return true;
    }

    private static boolean checkStageAsanEnabledValid(String jsonString) throws BundleException {
        boolean asanEnabled = ModuleJsonUtil.getStageAsanEnabled(jsonString);
        boolean debug = ModuleJsonUtil.getDebug(jsonString);
        if (asanEnabled && !debug) {
            LOG.error("asanEnabled is not supported for Release.");
            return false;
        }
        return true;
    }

    private static boolean checkStageAtomicService(String jsonString) throws BundleException {
        // check consistency of atomicService
        if (!ModuleJsonUtil.isModuleAtomicServiceValid(jsonString)) {
            LOG.error("check module atomicService failed.");
            return false;
        }
        // check entry module must have ability
        if (!ModuleJsonUtil.checkEntryInAtomicService(jsonString)) {
            LOG.error("checkEntryInAtomicService failed.");
            return false;
        }
        // check installationFree
        if (!ModuleJsonUtil.checkAtomicServiceInstallationFree(jsonString)) {
            LOG.error("check atomic service installationFree failed.");
            return false;
        }

        return true;
    }

    private static boolean checkStageOverlayCfg(String jsonString) throws BundleException {
        // check module
        String targetModuleName = ModuleJsonUtil.getStageTargetModuleName(jsonString);
        if (!targetModuleName.isEmpty()) {
            // check targetModuleName and requestPermission
            if (ModuleJsonUtil.isExistedStageRequestPermissions(jsonString)) {
                LOG.error("targetModuleName cannot be existed with requestPermissions.");
                return false;
            }
            // check targetModuleName and name
            if (targetModuleName.equals(ModuleJsonUtil.parseStageModuleName(jsonString))) {
                LOG.error("targetModuleName cannot be same with name in the overlay module.");
                return false;
            }
        } else {
            if (ModuleJsonUtil.isExistedStageModuleTargetPriority(jsonString)) {
                LOG.error("targetPriority cannot be existed without the targetModuleName in module.json.");
                return false;
            }
        }
        // check app
        String targetBundleName = ModuleJsonUtil.getStageTargetBundleName(jsonString);
        if (!targetBundleName.isEmpty()) {
            if (targetModuleName.isEmpty()) {
                LOG.error("targetModuleName is necessary in the overlay bundle.");
                return false;
            }
            if (targetBundleName.equals(ModuleJsonUtil.parseBundleName(jsonString))) {
                LOG.error("targetBundleName cannot be same with the bundleName.");
                return false;
            }
        } else {
            if (ModuleJsonUtil.isExistedStageAppTargetPriority(jsonString)) {
                LOG.error("targetPriority cannot be existed without the targetBundleName in app.json.");
                return false;
            }
        }
        return true;
    }

    private static boolean checkFAHap(Utility utility) throws BundleException {
        Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
        String jsonString = optional.get();
        return checkFAAsanEnabledValid(jsonString);
    }

    private static boolean checkFAAsanEnabledValid(String jsonString) throws BundleException {
        boolean asanEnabled = ModuleJsonUtil.getFAAsanEnabled(jsonString);
        boolean debug = ModuleJsonUtil.getFADebug(jsonString);
        if (asanEnabled && !debug) {
            LOG.error("asanEnabled is not supported for Release.");
            return false;
        }
        return true;
    }

    /**
     * compress in hap mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressHapMode(Utility utility) throws BundleException {
        pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

        pathToFile(utility, utility.getProfilePath(), NULL_DIR_NAME, false);

        if (!utility.getIndexPath().isEmpty() && !utility.getModuleName().isEmpty()) {
            String assetsPath = ASSETS_DIR_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR;
            pathToFile(utility, utility.getIndexPath(), assetsPath, false);
        }

        if (!utility.getLibPath().isEmpty()) {
            pathToFile(utility, utility.getLibPath(), LIBS_DIR_NAME, utility.isCompressNativeLibs());
        }

        if (!utility.getFilePath().isEmpty()) {
            pathToFile(utility, utility.getFilePath(), NULL_DIR_NAME, false);
        }

        if (!utility.getResPath().isEmpty() && !utility.getModuleName().isEmpty()) {
            String resPath = ASSETS_DIR_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR
                    + RESOURCES_DIR_NAME;
            String deviceTypes = utility.getDeviceType().replace("\"", "").trim();
            if (DEVICE_TYPE_FITNESSWATCH.equals(deviceTypes) ||
                    DEVICE_TYPE_FITNESSWATCH_NEW.equals(deviceTypes)) {
                resPath = RES_DIR_NAME;
            }
            pathToFile(utility, utility.getResPath(), resPath, false);
        }

        if (!utility.getResourcesPath().isEmpty() && !utility.getModuleName().isEmpty()) {
            String resourcesPath = ASSETS_DIR_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR
                    + RESOURCES_DIR_NAME;
            pathToFile(utility, utility.getResourcesPath(), resourcesPath, false);
        }

        if (!utility.getRpcidPath().isEmpty()) {
            String rpcidPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getRpcidPath(), rpcidPath, false);
        }

        if (!utility.getPackInfoPath().isEmpty()) {
            String packInfoPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getPackInfoPath(), packInfoPath, false);
        }

        if (!utility.getAssetsPath().isEmpty()) {
            pathToFile(utility, utility.getAssetsPath(), ASSETS_DIR_NAME, false);
        }

        if (!utility.getBinPath().isEmpty()) {
            pathToFile(utility, utility.getBinPath(), NULL_DIR_NAME, false);
        }
        // pack --dir-list
        if (!utility.getFormatedDirList().isEmpty()) {
            for (int i = 0; i < utility.getFormatedDirList().size(); ++i) {
                String baseDir = new File(utility.getFormatedDirList().get(i)).getName() + File.separator;
                pathToFile(utility, utility.getFormatedDirList().get(i), baseDir, false);
            }
        }

        compressHapModeMultiple(utility);
    }

    /**
     * compress in hap mode for module.json.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressHapModeForModule(Utility utility) throws BundleException {
        pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

        pathToFile(utility, utility.getProfilePath(), NULL_DIR_NAME, false);

        if (!utility.getIndexPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String assetsPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getIndexPath(), assetsPath, false);
        }

        if (!utility.getLibPath().isEmpty()) {
            pathToFile(utility, utility.getLibPath(), LIBS_DIR_NAME, utility.isCompressNativeLibs());
        }

        if (!utility.getANPath().isEmpty()) {
            pathToFile(utility, utility.getANPath(), AN_DIR_NAME, false);
        }

        if (!utility.getAPPath().isEmpty()) {
            pathToFile(utility, utility.getAPPath(), AP_PATH_NAME, false);
        }

        if (!utility.getFilePath().isEmpty()) {
            pathToFile(utility, utility.getFilePath(), NULL_DIR_NAME, false);
        }

        if (!utility.getResPath().isEmpty() && !utility.getModuleName().isEmpty()) {
            String resPath = ASSETS_DIR_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR
                    + RESOURCES_DIR_NAME;
            String deviceTypes = utility.getDeviceType().replace("\"", "").trim();
            if (DEVICE_TYPE_FITNESSWATCH.equals(deviceTypes) ||
                    DEVICE_TYPE_FITNESSWATCH_NEW.equals(deviceTypes)) {
                resPath = RES_DIR_NAME;
            }
            pathToFile(utility, utility.getResPath(), resPath, false);
        }

        if (!utility.getResourcesPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String resourcesPath = RESOURCES_DIR_NAME;
            pathToFile(utility, utility.getResourcesPath(), resourcesPath, false);
        }
        if (!utility.getJsPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String jsPath = JS_PATH;
            pathToFile(utility, utility.getJsPath(), jsPath, false);
        }

        if (!utility.getEtsPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String etsPath = ETS_PATH;
            pathToFile(utility, utility.getEtsPath(), etsPath, false);
        }

        if (!utility.getRpcidPath().isEmpty()) {
            String rpcidPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getRpcidPath(), rpcidPath, false);
        }

        if (!utility.getAssetsPath().isEmpty()) {
            pathToFile(utility, utility.getAssetsPath(), ASSETS_DIR_NAME, false);
        }

        if (!utility.getBinPath().isEmpty()) {
            pathToFile(utility, utility.getBinPath(), NULL_DIR_NAME, false);
        }

        if (!utility.getPackInfoPath().isEmpty()) {
            pathToFile(utility, utility.getPackInfoPath(), NULL_DIR_NAME, false);
        }

        // pack --dir-list
        if (!utility.getFormatedDirList().isEmpty()) {
            for (int i = 0; i < utility.getFormatedDirList().size(); ++i) {
                String baseDir = new File(utility.getFormatedDirList().get(i)).getName() + File.separator;
                pathToFile(utility, utility.getFormatedDirList().get(i), baseDir, false);
            }
        }

        compressHapModeMultiple(utility);
    }

    /**
     * compress in hap mode multiple path.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressHapModeMultiple(Utility utility) throws BundleException {
        for (String soPathItem : utility.getFormattedSoPathList()) {
            pathToFile(utility, soPathItem, SO_ARM64_DIR_NAME, false);
        }

        if (utility.getFormattedSoPathList().size() == 0 && !utility.getSoDir().isEmpty()) {
            pathToFile(utility, utility.getSoDir(), SO_DIR_NAME, false);
        }

        for (String soPathItem : utility.getFormattedAbilitySoPathList()) {
            pathToFile(utility, soPathItem, NULL_DIR_NAME, false);
        }

        for (String dexPathItem : utility.getFormattedDexPathList()) {
            pathToFile(utility, dexPathItem, NULL_DIR_NAME, false);
        }

        for (String abcPathItem : utility.getFormattedAbcPathList()) {
            pathToFile(utility, abcPathItem, NULL_DIR_NAME, false);
        }

        for (String apkPathItem : utility.getFormattedApkPathList()) {
            pathToFile(utility, apkPathItem, NULL_DIR_NAME, false);
        }

        for (String jarPathItem : utility.getFormattedJarPathList()) {
            pathToFile(utility, jarPathItem, NULL_DIR_NAME, false);
        }

        for (String txtPathItem : utility.getFormattedTxtPathList()) {
            pathToFile(utility, txtPathItem, NULL_DIR_NAME, false);
        }

        if (!utility.getSharedLibsPath().isEmpty()) {
            pathToFile(utility, utility.getSharedLibsPath(), SHARED_LIBS_DIR_NAME, utility.isCompressNativeLibs());
        }
    }

    /**
     * compress in har mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressHarMode(Utility utility) throws BundleException {
        pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

        if (!utility.getLibPath().isEmpty()) {
            pathToFile(utility, utility.getLibPath(), LIBS_DIR_NAME, utility.isCompressNativeLibs());
        }

        if (!utility.getResPath().isEmpty()) {
            pathToFile(utility, utility.getResPath(), RESOURCES_DIR_NAME, false);
        }

        if (!utility.getResourcesPath().isEmpty()) {
            pathToFile(utility, utility.getResourcesPath(), RESOURCES_DIR_NAME, false);
        }

        if (!utility.getAssetsPath().isEmpty()) {
            pathToFile(utility, utility.getAssetsPath(), ASSETS_DIR_NAME, false);
        }

        for (String jarPathItem : utility.getFormattedJarPathList()) {
            pathToFile(utility, jarPathItem, NULL_DIR_NAME, false);
        }

        for (String txtPathItem : utility.getFormattedTxtPathList()) {
            pathToFile(utility, txtPathItem, NULL_DIR_NAME, false);
        }
    }

    /**
     * compress in app mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressAppMode(Utility utility) throws BundleException {
        List<String> fileList = new ArrayList<>();
        File appOutputFile = new File(utility.getOutPath().trim());
        String tempPath = appOutputFile.getParentFile().getParent() + File.separator + TEMP_HAP_DIR;
        String hspTempDirPath = appOutputFile.getParentFile().getParent() + File.separator + TEMP_HSP_DIR;
        try {
            pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

            if (!utility.getCertificatePath().isEmpty()) {
                pathToFile(utility, utility.getCertificatePath(), NULL_DIR_NAME, false);
            }

            if (!utility.getSignaturePath().isEmpty()) {
                pathToFile(utility, utility.getSignaturePath(), NULL_DIR_NAME, false);
            }

            File tempDir = new File(tempPath);
            if (!tempDir.exists()) {
                tempDir.mkdirs();
            }

            for (String hapPathItem : utility.getFormattedHapPathList()) {
                File hapFile = new File(hapPathItem.trim());
                String hapTempPath = tempDir + File.separator + hapFile.getName();
                fileList.add(hapTempPath);
                try {
                    compressPackinfoIntoHap(hapPathItem, hapTempPath, utility.getPackInfoPath());
                } catch (IOException e) {
                    LOG.error("Compressor::compressAppMode compress pack.info into hap failed.");
                    throw new BundleException("Compressor::compressAppMode compress pack.info into hap failed.");
                }
            }

            File hspTempDir = new File(hspTempDirPath);
            if (!hspTempDir.exists()) {
                hspTempDir.mkdirs();
            }
            for (String hspPathItem : utility.getFormattedHspPathList()) {
                File hspFile = new File(hspPathItem.trim());
                String hspTempPath = hspTempDir + File.separator + hspFile.getName();
                fileList.add(hspTempPath);
                try {
                    compressPackinfoIntoHap(hspPathItem, hspTempPath, utility.getPackInfoPath());
                } catch (IOException e) {
                    LOG.error("Compressor::compressAppMode compress pack.info into hsp failed.");
                    throw new BundleException("Compressor::compressAppMode compress pack.info into hsp failed.");
                }
            }
            parseFileSizeLimit(utility);
            // check hap is valid
            if (!checkHapIsValid(fileList, utility.getSharedApp())) {
                throw new BundleException("Compressor::compressFile verify failed, check version, " +
                        "apiVersion,moduleName,packageName.");
            }
            for (String hapPath : fileList) {
                pathToFile(utility, hapPath, NULL_DIR_NAME, false);
            }

            if (!utility.getEntryCardPath().isEmpty()) {
                String entryCardPath = ENTRYCARD_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR
                        + ENTRYCARD_BASE_NAME + ENTRYCARD_SNAPSHOT_NAME;
                for (String entryCardPathItem : utility.getformattedEntryCardPathList()) {
                    pathToFile(utility, entryCardPathItem, entryCardPath, true);
                }
            }

            if (!utility.getPackResPath().isEmpty()) {
                pathToFile(utility, utility.getPackResPath(), NULL_DIR_NAME, false);
            }
            File file = new File(utility.getPackInfoPath());
            compressFile(utility, file, NULL_DIR_NAME, false);
        } catch (BundleException e) {
            LOG.error("Compressor::compressAppMode compress failed.");
            throw new BundleException("Compressor::compressAppMode compress failed.");
        } finally {
            // delete temp file
            for (String path : fileList) {
                deleteFile(path);
            }
            deleteFile(tempPath);
            deleteFile(hspTempDirPath);
        }
    }

    /**
     * compress in app mode for multi project.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressAppModeForMultiProject(Utility utility) throws BundleException {
        List<String> fileList = new ArrayList<>();
        File appOutputFile = new File(utility.getOutPath().trim());
        String tempPath = appOutputFile.getParentFile().getParent() + File.separator + TEMP_HAP_DIR;
        String tempSelectedHapPath = appOutputFile.getParentFile().getParent() +File.separator + TEMP_SELECTED_HAP_DIR;
        try {
            File tempSelectedHapDir = new File(tempSelectedHapPath);
            FileUtils.makeDir(tempSelectedHapDir);
            File tempHapDir = new File(tempPath);
            FileUtils.makeDir(tempHapDir);
            // pack app and dispose conflict
            // save hap name into list
            List<String> seletedHaps = new ArrayList<>();
            String finalPackInfoStr = disposeApp(utility, seletedHaps, tempSelectedHapPath);
            // pack hap and dispose conflict
            finalPackInfoStr = disposeHap(utility, seletedHaps, tempSelectedHapPath, finalPackInfoStr);

            // save final pack.info file
            String finalPackInfoPath = tempSelectedHapDir.getPath() + File.separator + PACKINFO_NAME;
            writePackInfo(finalPackInfoPath, finalPackInfoStr);
            // pack haps
            for (String selectedHapName : seletedHaps) {
                String hapPathItem = tempSelectedHapDir.getPath() + File.separator + selectedHapName;
                File hapFile = new File(hapPathItem.trim());
                String hapTempPath = tempHapDir.getPath() + File.separator + hapFile.getName();
                fileList.add(hapTempPath);
                compressPackinfoIntoHap(hapPathItem, hapTempPath, finalPackInfoPath);
            }
            // check hap is valid
            if (!checkHapIsValid(fileList, false)) {
                String errMsg = "Compressor::compressAppModeForMultiProject There are some " +
                        "haps with different version code or duplicated moduleName or packageName.";
                throw new BundleException(errMsg);
            }
            for (String hapPath : fileList) {
                pathToFile(utility, hapPath, NULL_DIR_NAME, false);
            }
            File file = new File(finalPackInfoPath);
            compressFile(utility, file, NULL_DIR_NAME, false);
        } catch (BundleException | IOException exception) {
            String errMsg = "Compressor::compressAppModeForMultiProject file failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        } finally {
            deleteFile(tempPath);
            deleteFile(tempSelectedHapPath);
        }
    }

    /**
     * pack hap in app to selectedHaps
     *
     * @param utility is common data
     * @param seletedHaps is seleted haps should be pack into app
     * @return the final pack.info string after dispose app
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static String disposeApp(Utility utility, List<String> seletedHaps,
        String tempDir) throws BundleException {
        // dispose app conflict
        if (utility.getFormattedAppList().isEmpty()) {
            return "";
        }
        String finalAppPackInfo = "";
        try {
            for (String appPath : utility.getFormattedAppList()) {
                // select hap in app
                finalAppPackInfo = selectHapInApp(appPath, seletedHaps, tempDir, finalAppPackInfo);
            }
        } catch (BundleException | IOException e) {
            String errMsg = "Compressor:disposeApp disposeApp failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        return finalAppPackInfo;
    }

    /**
     * select hap from app file list
     *
     * @param appPath is common data
     * @param selectedHaps is list of packInfos
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static String selectHapInApp(String appPath, List<String> selectedHaps, String tempDir,
                                         String finalAppPackInfo) throws BundleException, IOException {
        List<String> selectedHapsInApp = new ArrayList<>();
        // classify hap in app
        copyHapAndHspFromApp(appPath, selectedHapsInApp, selectedHaps, tempDir);
        // rebuild pack.info
        String packInfoStr = FileUtils.getJsonInZips(new File(appPath), PACKINFO_NAME);
        if (packInfoStr.isEmpty()) {
            String errorMsg = "Compressor:selectHapInApp failed, app has no pack.info.";
            LOG.error(errorMsg);
            throw new BundleException(errorMsg);
        }
        if (finalAppPackInfo.isEmpty()) {
            finalAppPackInfo = packInfoStr;
            return finalAppPackInfo;
        }
        // read selected module in temp hap
        HashMap<String, String> packagePair = new HashMap<>();
        for (String hapName : selectedHapsInApp) {
            packagePair.put(hapName, readModlueNameFromHap(tempDir + File.separator + hapName));
        }
        return ModuleJsonUtil.mergeTwoPackInfoByPackagePair(finalAppPackInfo, packInfoStr, packagePair);
    }

    /**
     * copy hap from app file
     *
     * @param appPath is common data
     * @param selectedHapsInApp is list of haps and hsps selected in app file
     * @param selectedHaps is the list of haps and hsps selected in input
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static void copyHapAndHspFromApp(String appPath, List<String> selectedHapsInApp, List<String> selectedHaps,
        String tempDir) throws BundleException {
        ZipInputStream zipInput = null;
        ZipFile zipFile = null;
        OutputStream outputStream = null;
        InputStream inputStream = null;
        ZipEntry zipEntry = null;
        try {
            zipInput = new ZipInputStream(new FileInputStream(appPath));
            zipFile = new ZipFile(appPath);
            while ((zipEntry = zipInput.getNextEntry()) != null) {
                File file = null;
                if (!zipEntry.getName().endsWith(HAP_SUFFIX) && !zipEntry.getName().endsWith(HSP_SUFFIX)) {
                    continue;
                }
                // copy duplicated hap to duplicated dir and get moduleName of duplicated hap
                if (selectedHaps.contains(zipEntry.getName())) {
                    LOG.error("Compressor::copyHapFromApp file duplicated, file is " + zipEntry.getName() + ".");
                    throw new BundleException("Compressor::copyHapFromApp file duplicated, file is "
                            + zipEntry.getName() + ".");
                } else {
                    // copy selectedHap to tempDir
                    file = new File(tempDir + File.separator + zipEntry.getName());
                    selectedHaps.add(file.getName());
                    selectedHapsInApp.add(file.getName());
                }
                outputStream = new FileOutputStream(file);
                inputStream = zipFile.getInputStream(zipEntry);
                int len;
                while ((len = inputStream.read()) != -1) {
                    outputStream.write(len);
                }
                outputStream.close();
                inputStream.close();
            }
        } catch (IOException e) {
            String errMsg = "Compressor:copyHapFromApp app path not found.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        } finally {
            Utility.closeStream(zipInput);
            Utility.closeStream(zipFile);
            Utility.closeStream(outputStream);
            Utility.closeStream(inputStream);
        }
    }

    /**
     * read moduleName in hap
     *
     * @param hapPath is path of hap file
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static String readModlueNameFromHap(String hapPath) throws BundleException {
        String moduleName = "";
        File hapFile = new File(hapPath);
        if (isModuleHap(hapPath)) {
            String jsonString = FileUtils.getJsonInZips(hapFile, MODULE_JSON);
            moduleName = ModuleJsonUtil.parseStageModuleName(jsonString);
        } else {
            String jsonString = FileUtils.getJsonInZips(hapFile, CONFIG_JSON);
            moduleName = ModuleJsonUtil.parseFaModuleName(jsonString);
        }
        return moduleName;
    }

    /**
     * dispose input of hap
     *
     * @param utility is common data
     * @param seletedHaps is the selected  haps of all input
     * @param tempDir is the path of temp directory
     * @param finalPackInfoStr is the pack.info of the final app
     * @throws BundleException FileNotFoundException|IOException.
     */
    private static String disposeHap(Utility utility, List<String> seletedHaps, String tempDir,
                                   String finalPackInfoStr) throws BundleException, IOException {
        // dispose hap conflict
        if (utility.getFormattedHapList().isEmpty()) {
            return finalPackInfoStr;
        }
        for (String hapPath : utility.getFormattedHapList()) {
            if (seletedHaps.contains(new File(hapPath).getName())) {
                LOG.error("Compressor::disposeHap file duplicated, file is " + new File(hapPath).getName() + ".");
                throw new BundleException("Compressor::disposeHap file duplicated, file is "
                        + new File(hapPath).getName() + ".");
            }
            File hapFile = new File(hapPath);
            seletedHaps.add(hapFile.getName());
            // copy hap to tempDir
            FileUtils.copyFile(hapFile, new File((tempDir +File.separator + hapFile.getName())));
            String packInfo = FileUtils.getJsonInZips(hapFile, PACKINFO_NAME);

            if (packInfo.isEmpty()) {
                String errMsg = "Compressor::disposeHap failed, hap has no pack.info.";
                LOG.error(errMsg);
                throw new BundleException(errMsg);
            }
            if (finalPackInfoStr.isEmpty()) {
                finalPackInfoStr = packInfo;
            } else {
                finalPackInfoStr = ModuleJsonUtil.mergeTwoPackInfo(finalPackInfoStr, packInfo);
            }
        }
        return finalPackInfoStr;
    }

    /**
     * write string to pack.info
     *
     * @param filePath pack.info file path
     * @param packInfoStr is the string of pack.info
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void writePackInfo(String filePath, String packInfoStr) throws BundleException, IOException {
        FileWriter fwriter = null;
        try {
            fwriter = new FileWriter(filePath);
            fwriter.write(packInfoStr);
        } catch (IOException e) {
            String errMsg = "Compressor:writePackInfo failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        } finally {
            if (fwriter != null) {
                fwriter.flush();
                fwriter.close();
            }
        }
    }

    private void copy(InputStream input, OutputStream output) throws IOException {
        int bytesRead;
        byte[] data = new byte[BUFFER_SIZE];
        while ((bytesRead = input.read(data, 0, BUFFER_SIZE)) != -1) {
            output.write(data, 0, bytesRead);
        }
    }

    private void compressPackinfoIntoHap(String hapPathItem, String outPathString, String packInfo)
            throws IOException, BundleException {
        ZipFile sourceHapFile = new ZipFile(hapPathItem);
        ZipOutputStream append = new ZipOutputStream(new FileOutputStream(outPathString));
        try {
            Enumeration<? extends ZipEntry> entries = sourceHapFile.entries();
            while (entries.hasMoreElements()) {
                ZipEntry zipEntry = entries.nextElement();
                if (PACKINFO_NAME.equals(zipEntry.getName())) {
                    continue;
                }
                ZipEntry newEntry = new ZipEntry(zipEntry.getName());
                append.putNextEntry(newEntry);
                if (!zipEntry.isDirectory()) {
                    copy(sourceHapFile.getInputStream(zipEntry), append);
                }
                append.closeEntry();
            }
            File packInfoFile = new File(packInfo);
            ZipEntry zipEntry = getStoredZipEntry(packInfoFile, PACKINFO_NAME);
            append.putNextEntry(zipEntry);
            FileInputStream in = new FileInputStream(packInfoFile);
            try {
                byte[] buf = new byte[BUFFER_SIZE];
                int len;
                while ((len = in.read(buf)) != -1) {
                    append.write(buf, 0, len);
                }
            } finally {
                in.close();
            }
            append.closeEntry();
        } catch (IOException exception) {
            LOG.error("Compressor::compressPackinfoIntoHap io exception.");
            throw new BundleException("Compressor::compressPackinfoIntoHap io exception.");
        } finally {
            sourceHapFile.close();
            append.close();
        }
    }

    /**
     * delete file
     *
     * @param path file path which will be deleted
     */
    private static void deleteFile(final String path) {
        File file = new File(path);
        if (file.exists()) {
            if (file.isDirectory()) {
                File[] files = file.listFiles();
                for (int i = 0; i < files.length; i++) {
                    deleteFile(files[i].toString());
                }
            }
            file.delete();
        }
    }

    /**
     * compress in res mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressPackResMode(Utility utility) throws BundleException {
        if (!utility.getPackInfoPath().isEmpty()) {
            File file = new File(utility.getPackInfoPath());
            infoSpecialProcess(utility, file);
        }
        if (!utility.getEntryCardPath().isEmpty()) {
            getFileList(utility.getEntryCardPath());
            if (!mIsContain2x2EntryCard) {
                LOG.error("Compressor::compressPackResMode No 2x2 resource file exists.");
                throw new BundleException("No 2x2 resource file exists.");
            }
            for (String fileName : fileNameList) {
                if (fileName.endsWith(PNG_SUFFIX) || fileName.endsWith(UPPERCASE_PNG_SUFFIX)) {
                    String fName = fileName.trim();
                    String[] temp = fName.replace("\\", "/").split("/");
                    if (temp.length < 4) {
                        LOG.error("Compressor::compressPackResMode the hap file path is invalid, length: "
                            + temp.length + ".");
                        continue;
                    }
                    String moduleName = temp[temp.length - 4];
                    if (!isModelName(moduleName)) {
                        LOG.error("Compressor::compressProcess compress pack.res failed, moduleName "
                            + moduleName + " is error, please check it in config.json.");
                        throw new BundleException("Compress pack.res failed, moduleName Error.");
                    }
                    String fileLanguageCountryName = temp[temp.length - 3];
                    if (!isThirdLevelDirectoryNameValid(fileLanguageCountryName)) {
                        LOG.error("Compressor::compressProcess compress failed third level directory name: "
                            + fileLanguageCountryName + " is invalid, please check it with reference to this example: "
                            + "zh_Hani_CN-vertical-car-mdpi-dark or zh_Hani_CN-vertical-car-mdpi.");
                        throw new BundleException("Compress failed third level directory name Error.");
                    }
                    String filePicturingName = temp[temp.length - 1];
                    if (!isPicturing(filePicturingName, utility)) {
                        LOG.error("Compressor::compressProcess Compress pack.res failed, Invalid resource file" +
                            " name: " + filePicturingName + ", correct format example is formName-2x2.png.");
                        throw new BundleException("Compress pack.res failed, Invalid resource file name: "
                            + filePicturingName + ", correct format example is formName-2x2.png.");
                    }

                } else {
                    LOG.error("Compressor::compressProcess compress failed No image in PNG format is found.");
                    throw new BundleException("Compress pack.res failed, compress failed No image in"
                        + " PNG format is found.");
                }
            }
            pathToFile(utility, utility.getEntryCardPath(), ENTRYCARD_NAME, false);
        }
    }

    /**
     * Check whether modelname meets specifications.
     *
     * @param name modelName
     * @return false and true
     */
    private boolean isModelName(String name) {
        for (String listName : list) {
            if (name.equals(listName)) {
                return true;
            }
        }
        return false;
    }

    private boolean isThirdLevelDirectoryNameValid(String thirdLevelDirectoryName) {
        if (thirdLevelDirectoryName == null || thirdLevelDirectoryName.isEmpty()) {
            return false;
        }
        if (ENTRYCARD_BASE_NAME.equals(thirdLevelDirectoryName)) {
            return true;
        }
        // example: zh_Hani_CN-vertical-car-mdpi-dark or zh_Hani_CN-vertical-car-mdpi
        int firstDelimiterIndex = thirdLevelDirectoryName.indexOf("_");
        if (firstDelimiterIndex < 0) {
            return false;
        }
        String language = thirdLevelDirectoryName.substring(0, firstDelimiterIndex);
        int secondDelimiterIndex = thirdLevelDirectoryName.indexOf("_", firstDelimiterIndex + 1);
        if (secondDelimiterIndex < 0) {
            return false;
        }
        String script = thirdLevelDirectoryName.substring(firstDelimiterIndex + 1, secondDelimiterIndex);
        int thirdDelimiterIndex = thirdLevelDirectoryName.indexOf("-", secondDelimiterIndex + 1);
        if (thirdDelimiterIndex < 0) {
            return false;
        }
        String country = thirdLevelDirectoryName.substring(secondDelimiterIndex + 1, thirdDelimiterIndex);
        if (!checkLanguage(language) || !checkScript(script) || !checkCountry(country)) {
            return false;
        }
        int forthDelimiterIndex = thirdLevelDirectoryName.indexOf("-", thirdDelimiterIndex + 1);
        if (forthDelimiterIndex < 0) {
            return false;
        }
        String orientation = thirdLevelDirectoryName.substring(thirdDelimiterIndex + 1, forthDelimiterIndex);
        int fifthDelimiterIndex = thirdLevelDirectoryName.indexOf("-", forthDelimiterIndex + 1);
        if (fifthDelimiterIndex < 0) {
            return false;
        }
        String deviceType = thirdLevelDirectoryName.substring(forthDelimiterIndex + 1, fifthDelimiterIndex);
        if (!checkOrientation(orientation) || !checkDeviceType(deviceType)) {
            return false;
        }
        int sixthDelimiterIndex = thirdLevelDirectoryName.indexOf("-", fifthDelimiterIndex + 1);
        if (sixthDelimiterIndex < 0) {
            String screenDensity = thirdLevelDirectoryName.substring(fifthDelimiterIndex + 1,
                    thirdLevelDirectoryName.length());
            return checkScreenDensity(screenDensity);
        } else {
            String screenDensity = thirdLevelDirectoryName.substring(fifthDelimiterIndex + 1, sixthDelimiterIndex);
            if (!checkScreenDensity(screenDensity)) {
                return false;
            }
        }
        int seventhDelimiterIndex = thirdLevelDirectoryName.indexOf("-", sixthDelimiterIndex + 1);
        if (seventhDelimiterIndex < 0) {
            String tmp = thirdLevelDirectoryName.substring(sixthDelimiterIndex + 1, thirdLevelDirectoryName.length());
            return checkColorModeOrShape(tmp);
        }
        if (!checkColorMode(thirdLevelDirectoryName.substring(sixthDelimiterIndex + 1, seventhDelimiterIndex))) {
            return false;
        }
        String shape = thirdLevelDirectoryName.substring(seventhDelimiterIndex + 1, thirdLevelDirectoryName.length());
        return checkShape(shape);
    }

    private boolean checkLanguage(String language) {
        if (!Pattern.compile(REGEX_LANGUAGE).matcher(language).matches()) {
            LOG.error("Compressor::compressProcess language " + language + " is not in ISO 639-1 list.");
            return false;
        }
        return true;
    }

    private boolean checkScript(String script) {
        if (!Pattern.compile(REGEX_SCRIPT).matcher(script).matches()) {
            LOG.error("Compressor::compressProcess script " + script + " is not in ISO 15924 list.");
            return false;
        }
        return true;
    }

    private boolean checkCountry(String country) {
        if (!Pattern.compile(REGEX_COUNTRY).matcher(country).matches()) {
            LOG.error("Compressor::compressProcess country " + country + " is not in ISO 3166-1 list.");
            return false;
        }
        return true;
    }

    private boolean checkOrientation(String orientation) {
        if (!Pattern.compile(REGEX_ORIENTATION).matcher(orientation).matches()) {
            LOG.error("Compressor::compressProcess orientation " + orientation +
                " is not in {vertical, horizontal} list.");
            return false;
        }
        return true;
    }

    private boolean checkDeviceType(String deviceType) {
        if (!Pattern.compile(REGEX_DEVICE_TYPE).matcher(deviceType).matches()) {
            LOG.error("Compressor::compressProcess deviceType " + deviceType +
                    " is not in {phone, tablet, car, tv, wearable, liteWearable} list.");
            return false;
        }
        return true;
    }

    private boolean checkScreenDensity(String screenDensity) {
        if (!Pattern.compile(REGEX_SCREEN_DENSITY).matcher(screenDensity).matches()) {
            LOG.error("Compressor::compressProcess screenDensity " + screenDensity +
                    " is not in {sdpi, mdpi, ldpi, xldpi, xxldpi} list.");
            return false;
        }
        return true;
    }

    private boolean checkColorMode(String colorMode) {
        if (!Pattern.compile(REGEX_COLOR_MODE).matcher(colorMode).matches()) {
            LOG.error("Compressor::compressProcess colorMode " + colorMode +
                    " is not in {light, dark} list.");
            return false;
        }
        return true;
    }

    private boolean checkColorModeOrShape(String tmp) {
        if (Pattern.compile(REGEX_COLOR_MODE).matcher(tmp).matches() ||
            Pattern.compile(REGEX_SHAPE).matcher(tmp).matches()) {
            return true;
        }
        LOG.error("Compressor::compressProcess " + tmp +
                " is neither in colorMode list {light, dark} nor in shape list {circle}.");
        return false;
    }

    private boolean checkShape(String shape) {
        if (Pattern.compile(REGEX_SHAPE).matcher(shape).matches()) {
            return true;
        }
        LOG.error("Compressor::compressProcess shape" + shape + " is not in {circle} list.");
        return false;
    }

    /**
     * Check whether picturingName meets specifications.
     *
     * @param name picturingName
     * @param utility common data
     * @return false and true
     */
    private boolean isPicturing(String name, Utility utility) {
        boolean isSpecifications = false;
        if (name == null || name.isEmpty()) {
            return isSpecifications;
        }
        if (!name.endsWith(PNG_SUFFIX) && !name.endsWith(UPPERCASE_PNG_SUFFIX)) {
            LOG.error("isPicturing: the suffix is not .png or .PNG.");
            return false;
        }
        int delimiterIndex = name.lastIndexOf("-");
        if (delimiterIndex < 0) {
            LOG.error("isPicturing: the entry card naming format is invalid and should be separated by '-'.");
            return false;
        }
        String formName = name.substring(0, delimiterIndex);
        if (!utility.getFormNameList().contains(formName)) {
            LOG.error("isPicturing: the name is not same as formName, name: " + formName + " is not in " +
                utility.getFormNameList().toString() + ".");
            return false;
        }
        String dimension = name.substring(delimiterIndex + 1, name.lastIndexOf("."));
        if (!supportDimensionsList.contains(dimension)) {
            LOG.error("isPicturing: the dimension: " + dimension + " is invalid, is not in the following list: "
                + "{1X2, 2X2, 2X4, 4X4}.");
            return false;
        }
        return true;
    }

    private void getFileList(final String filePath) throws BundleException {
        File file = new File(filePath);
        if (!file.exists()) {
            LOG.error("getFileList: file is not exists.");
            return;
        }
        File[] files = file.listFiles();
        if (files == null) {
            LOG.error("getFileList: no file in this file path.");
            return;
        }
        for (File f : files) {
            try {
                if (f.isFile()) {
                    if (f.getName().endsWith(".DS_Store")) {
                        deleteFile(f.getCanonicalPath());
                        continue;
                    }
                    String snapshotDirectoryName = f.getParentFile().getName();
                    if (!ENTRYCARD_SNAPSHOT_NAME.equals(snapshotDirectoryName)) {
                        LOG.error("The level-4 directory of EntryCard must be named as snapshot" +
                            ", but current is: " + snapshotDirectoryName + ".");
                        throw new BundleException("The level-4 directory of EntryCard must be named as snapshot" +
                            ", but current is: " + snapshotDirectoryName + ".");
                    }
                    checkContain2x2EntryCard(f.getParentFile());
                    fileNameList.add(f.getCanonicalPath());
                } else if (f.isDirectory()) {
                    getFileList(f.getCanonicalPath());
                } else {
                    LOG.error("It's not file or directory.");
                }
            } catch (IOException msg) {
                LOG.error("IOException error: " + msg.getMessage());
                return;
            }
        }
    }

    private void checkContain2x2EntryCard(final File snapshotDirectory) throws IOException, BundleException {
        if (!snapshotDirectory.exists()) {
            LOG.error("checkContain2x2EntryCard: file is not exist: " + snapshotDirectory.getName() + ".");
            throw new BundleException("checkContain2x2EntryCard: file is not exist.");
        }
        File[] files = snapshotDirectory.listFiles();
        if (files == null) {
            LOG.error("checkContain2x2EntryCard: no file in this file path.");
            throw new BundleException("checkContain2x2EntryCard: no file in this file path.");
        }

        for (File entryCardFile : files) {
            if (entryCardFile.isFile() && entryCardFile.getName().contains(PIC_2X2)) {
                return;
            }
        }
        mIsContain2x2EntryCard = false;
        LOG.error("checkContain2x2EntryCard: must contain 2x2 entryCard, please check it in "
            + snapshotDirectory.getCanonicalPath() + ".");
        throw new BundleException("checkContain2x2EntryCard: must contain 2x2 entryCard, please check it in "
            + snapshotDirectory.getCanonicalPath() + ".");
    }

    /**
     * compress file or directory.
     *
     * @param utility       common data
     * @param path          create new file by path
     * @param baseDir       base path for file
     * @param isCompression if need compression
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void pathToFile(Utility utility, String path, String baseDir, boolean isCompression)
            throws BundleException {
        if (path.isEmpty()) {
            return;
        }
        File fileItem = new File(path);
        if (fileItem.isDirectory()) {
            File[] files = fileItem.listFiles();
            if (files == null) {
                return;
            }
            for (File file : files) {
                if (file.isDirectory()) {
                    compressDirectory(utility, file, baseDir, isCompression);
                } else if (isCompression) {
                    compressFile(utility, file, baseDir, isCompression);
                } else {
                    compressFile(utility, file, baseDir, isCompression);
                }
            }
        } else {
            compressFile(utility, fileItem, baseDir, isCompression);
        }
    }

    /**
     * compress file directory.
     *
     * @param utility       common data
     * @param dir           file directory
     * @param baseDir       current directory name
     * @param isCompression if need compression
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressDirectory(Utility utility, File dir, String baseDir, boolean isCompression)
            throws BundleException {
        File[] files = dir.listFiles();
        if (files == null) {
            return;
        }
        for (File file : files) {
            if (file.isDirectory()) {
                compressDirectory(utility, file, baseDir + dir.getName() + File.separator, isCompression);
            } else {
                compressFile(utility, file, baseDir + dir.getName() + File.separator, isCompression);
            }
        }
    }

    /**
     * compress pack.info
     *
     * @param sourceFile source
     * @param zipOutputStream ZipOutputStream
     * @param name filename
     * @param KeepDirStructure Empty File
     */
    private void compress(File sourceFile, ZipOutputStream zipOutputStream, String name,
                                boolean KeepDirStructure) {
        FileInputStream in = null;
        try {
            byte[] buf = new byte[BUFFER_SIZE];
            if (sourceFile.isFile()) {
                ZipEntry zipEntry = getStoredZipEntry(sourceFile, name);
                zipOutputStream.putNextEntry(zipEntry);
                in = new FileInputStream(sourceFile);
                int len;
                while ((len = in.read(buf)) != -1) {
                    zipOutputStream.write(buf, 0, len);
                }
                zipOutputStream.closeEntry();
            } else {
                File[] listFiles = sourceFile.listFiles();
                if (listFiles == null || listFiles.length == 0) {
                    if (KeepDirStructure) {
                        if (!name.isEmpty()) {
                            ZipEntry zipEntry = getStoredZipEntry(sourceFile, name + "/");
                            zipOutputStream.putNextEntry(zipEntry);
                        } else {
                            ZipEntry zipEntry = getStoredZipEntry(sourceFile, name);
                            zipOutputStream.putNextEntry(zipEntry);
                        }
                        zipOutputStream.closeEntry();
                    }
                } else {
                    for (File file : listFiles) {
                        if (KeepDirStructure) {
                            isNameEmpty(zipOutputStream, name, KeepDirStructure, file);
                        } else {
                            compress(file, zipOutputStream, file.getName(), KeepDirStructure);
                        }
                    }
                }
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("Compressor::compressFile file not found exception.");
        } catch (IOException exception) {
            LOG.error("Compressor::compressFile io exception: " + exception.getMessage());
        } catch (BundleException bundleException) {
            LOG.error("Compressor::compressFile bundle exception" + bundleException.getMessage());
        } finally {
            Utility.closeStream(in);
        }
    }

    private ZipEntry getStoredZipEntry(File sourceFile, String name) throws BundleException {
        ZipEntry zipEntry = new ZipEntry(name);
        zipEntry.setMethod(ZipEntry.STORED);
        zipEntry.setCompressedSize(sourceFile.length());
        zipEntry.setSize(sourceFile.length());
        CRC32 crc = getCrcFromFile(sourceFile);
        zipEntry.setCrc(crc.getValue());
        FileTime fileTime = FileTime.fromMillis(FILE_TIME);
        zipEntry.setLastAccessTime(fileTime);
        zipEntry.setLastModifiedTime(fileTime);
        return zipEntry;
    }

    private CRC32 getCrcFromFile(File file) throws BundleException {
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
            LOG.error("Uncompressor::getCrcFromFile file not found exception.");
            throw new BundleException("Get Crc from file failed.");
        } catch (IOException exception) {
            LOG.error("Uncompressor::getCrcFromFile io exception: " + exception.getMessage());
            throw new BundleException("Get Crc from file failed.");
        } finally {
            Utility.closeStream(fileInputStream);
        }
        return crc;
    }

    /**
     * isNameEmpty
     *
     * @param zipOutputStream ZipOutputStream
     * @param name filename
     * @param KeepDirStructure KeepDirStructure
     * @param file file
     */
    private void isNameEmpty(ZipOutputStream zipOutputStream, String name, boolean KeepDirStructure, File file) {
        if (!name.isEmpty()) {
            compress(file, zipOutputStream, name + "/" + file.getName(), KeepDirStructure);
        } else {
            compress(file, zipOutputStream, file.getName(), KeepDirStructure);
        }
    }

    /**
     * compress process.
     *
     * @param utility       common data
     * @param srcFile       source file to zip
     * @param baseDir       current directory name of file
     * @param isCompression if need compression
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressFile(Utility utility, File srcFile, String baseDir, boolean isCompression)
            throws BundleException {
        BufferedInputStream bufferedInputStream = null;
        FileInputStream fileInputStream = null;
        try {
            String entryName = (baseDir + srcFile.getName()).replace(File.separator, LINUX_FILE_SEPARATOR);
            ZipEntry zipEntry = new ZipEntry(entryName);
            String srcName = srcFile.getName().toLowerCase(Locale.ENGLISH);
            if (CONFIG_JSON.equals(srcName) || MODULE_JSON.equals(srcName)) {
                zipEntry.setMethod(ZipEntry.STORED);
                jsonSpecialProcess(utility, srcFile, zipEntry);
                return;
            }

            if (isCompression) {
                zipEntry.setMethod(ZipEntry.DEFLATED);
            } else {
                zipEntry.setMethod(ZipEntry.STORED);

                // update size
                zipEntry.setCompressedSize(srcFile.length());
                zipEntry.setSize(srcFile.length());

                // update crc
                CRC32 crc = getCrcFromFile(utility, srcFile);
                zipEntry.setCrc(crc.getValue());
            }

            // update fileTime
            FileTime fileTime = FileTime.fromMillis(FILE_TIME);
            zipEntry.setLastAccessTime(fileTime);
            zipEntry.setLastModifiedTime(fileTime);

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
            throw new BundleException("CoompressFile failed.");
        } catch (IOException exception) {
            LOG.error("Compressor::compressFile io exception: " + exception.getMessage());
            throw new BundleException("CoompressFile failed.");
        } finally {
            Utility.closeStream(bufferedInputStream);
            Utility.closeStream(fileInputStream);
        }
    }

    /**
     * check hap type for pack app.
     *
     * @param hapPath source file to zip
     * @return true is for is stage type and false is for FA type
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static boolean isModuleHap(String hapPath) throws BundleException {
        if (!hapPath.toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)) {
            return true;
        }

        FileInputStream zipInput = null;
        ZipInputStream zin = null;
        ZipEntry entry = null;
        try {
            zipInput = new FileInputStream(hapPath);
            zin = new ZipInputStream(zipInput);
            while ((entry = zin.getNextEntry()) != null) {
                if (MODULE_JSON.equals(entry.getName().toLowerCase())) {
                    return true;
                }
            }
        } catch (IOException exception) {
            LOG.error("Compressor::isModuleHap io exception: " + exception.getMessage());
            throw new BundleException("Compressor::isModuleHap failed.");
        } finally {
            Utility.closeStream(zipInput);
            Utility.closeStream(zin);
        }
        return false;
    }

    /**
     * get CRC32 from file.
     *
     * @param utility common data
     * @param file    source file
     * @return CRC32
     * @throws BundleException FileNotFoundException|IOException.
     */
    private CRC32 getCrcFromFile(Utility utility, File file) throws BundleException {
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
            throw new BundleException("Get Crc from file failed.");
        } catch (IOException exception) {
            LOG.error("Compressor::getCrcFromFile io exception: " + exception.getMessage());
            throw new BundleException("Get Crc from file failed.");
        } finally {
            Utility.closeStream(fileInputStream);
        }
        return crc;
    }

    private void infoSpecialProcess(Utility utility, File srcFile)
            throws BundleException {
        FileInputStream fileInputStream = null;
        BufferedReader bufferedReader = null;
        InputStreamReader inputStreamReader = null;

        try {
            fileInputStream = new FileInputStream(srcFile);
            inputStreamReader = new InputStreamReader(fileInputStream, StandardCharsets.UTF_8);
            bufferedReader = new BufferedReader(inputStreamReader);
            bufferedReader.mark((int) srcFile.length() + 1);
            // parse moduleName from pack.info
            parsePackModuleName(bufferedReader, utility);
            bufferedReader.reset();
            parsePackFormName(bufferedReader, utility);
            bufferedReader.reset();
            parseDeviceType(bufferedReader, utility);
            bufferedReader.reset();

            Pattern pattern = Pattern.compile(System.lineSeparator());
            String str = bufferedReader.readLine();
            StringBuilder builder = new StringBuilder();
            while (str != null) {
                Matcher matcher = pattern.matcher(str.trim());
                String dest = matcher.replaceAll("");
                builder.append(dest);
                str = bufferedReader.readLine();
            }
        } catch (IOException exception) {
            LOG.error("Compressor::jsonSpecialProcess io exception: " + exception.getMessage());
            throw new BundleException("Json special process failed.");
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(inputStreamReader);
            Utility.closeStream(fileInputStream);
        }
    }

    /**
     * trim and remove "\r\n" in *.json file.
     *
     * @param utility common data
     * @param srcFile file input
     * @param entry   zip file entry
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void jsonSpecialProcess(Utility utility, File srcFile, ZipEntry entry)
            throws BundleException {
        FileInputStream fileInputStream = null;
        BufferedReader bufferedReader = null;
        InputStreamReader inputStreamReader = null;

        try {
            fileInputStream = new FileInputStream(srcFile);
            inputStreamReader = new InputStreamReader(fileInputStream, StandardCharsets.UTF_8);
            bufferedReader = new BufferedReader(inputStreamReader);
            bufferedReader.mark((int) srcFile.length() + 1);
            // parse moduleName from config.json
            parseModuleName(bufferedReader, utility);
            bufferedReader.reset();
            String srcName = srcFile.getName().toLowerCase(Locale.ENGLISH);
            if (CONFIG_JSON.equals(srcName)) {
                parseCompressNativeLibs(bufferedReader, utility);
            } else if (MODULE_JSON.equals(srcName)) {
                parseStageCompressNativeLibs(bufferedReader, utility);
            }
            bufferedReader.reset();
            parseDeviceType(bufferedReader, utility);
            bufferedReader.reset();

            Pattern pattern = Pattern.compile(System.lineSeparator());
            String str = bufferedReader.readLine();
            StringBuilder builder = new StringBuilder();
            while (str != null) {
                Matcher matcher = pattern.matcher(str.trim());
                String dest = matcher.replaceAll("");
                builder.append(dest);
                str = bufferedReader.readLine();
            }
            byte[] trimJson = builder.toString().getBytes(StandardCharsets.UTF_8);

            // update crc
            CRC32 crc = new CRC32();
            crc.update(trimJson);
            entry.setCrc(crc.getValue());

            // update size
            entry.setSize(trimJson.length);
            entry.setCompressedSize(trimJson.length);

            // update fileTime
            FileTime fileTime = FileTime.fromMillis(FILE_TIME);
            entry.setLastAccessTime(fileTime);
            entry.setLastModifiedTime(fileTime);

            // compress data
            zipOut.putNextEntry(entry);
            zipOut.write(trimJson);
        } catch (IOException exception) {
            LOG.error("Compressor::jsonSpecialProcess io exception: " + exception.getMessage());
            throw new BundleException("Json special process failed.");
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(inputStreamReader);
            Utility.closeStream(fileInputStream);
        }
    }

    /**
     * Parse module name from config.json
     *
     * @param bufferedReader config.json buffered Reader
     * @param utility        common data
     * @throws BundleException IOException
     */
    private void parseModuleName(BufferedReader bufferedReader, Utility utility) throws BundleException {
        String lineStr = null;
        boolean isDistroStart = false;
        try {
            while ((lineStr = bufferedReader.readLine()) != null) {
                if (!isDistroStart) {
                    if (lineStr.contains(DISTRO)) {
                        isDistroStart = true;
                    }
                    continue;
                }
                if (lineStr.contains(JSON_END)) {
                    continue;
                }
                if (lineStr.contains(MODULE_NAME_NEW) || lineStr.contains(MODULE_NAME)) {
                    getModuleNameFromString(lineStr, utility);
                    break;
                }
            }
        } catch (IOException exception) {
            LOG.error("Compressor::parseModuleName io exception: " + exception.getMessage());
            throw new BundleException("Parse module name failed.");
        }
    }

    /**
     * Parse module name from pack.info
     *
     * @param bufferedReader pack.info buffered Reader
     * @param utility        common data
     * @throws BundleException IOException
     */
    private void parsePackModuleName(BufferedReader bufferedReader, Utility utility) throws BundleException {
        String lineStr = null;
        try {
            while ((lineStr = bufferedReader.readLine()) != null) {
                if (lineStr.contains(DISTRO)) {
                    continue;
                }
                if (lineStr.contains(JSON_END)) {
                    continue;
                }
                if (lineStr.contains(MODULE_NAME_NEW) || lineStr.contains(MODULE_NAME)) {
                    getModuleNameFromString(lineStr, utility);
                }
            }
        } catch (IOException exception) {
            LOG.error("Compressor::parseModuleName io exception: " + exception.getMessage());
            throw new BundleException("Parse module name failed.");
        }
    }

    /**
     * Parse Forms name from pack.info
     *
     * @param bufferedReader pack.info buffered Reader
     * @param utility        common data
     * @throws BundleException IOException
     */
    private void parsePackFormName(BufferedReader bufferedReader, Utility utility) throws BundleException {
        String lineStr = null;
        try {
            while ((lineStr = bufferedReader.readLine()) != null) {
                if (lineStr.contains("abilities")) {
                    continue;
                }
                if (lineStr.contains(FORMS)) {
                    continue;
                }
                if (lineStr.contains(JSON_END)) {
                    continue;
                }
                if (lineStr.contains(NAME)) {
                    getNameFromString(lineStr, utility);
                }
            }
        } catch (IOException exception) {
            LOG.error("Compressor::parseModuleName io exception: " + exception.getMessage());
            throw new BundleException("Parse module name failed.");
        }
    }


    /**
     * Get name from line string
     *
     * @param lineStr line string
     * @param utility common data
     * @throws BundleException StringIndexOutOfBoundsException
     */
    private void getNameFromString(String lineStr, Utility utility) throws BundleException {
        try {
            int endIndex = lineStr.lastIndexOf(SEMICOLON);
            if (endIndex <= 0) {
                LOG.error("Compressor::getModuleNameFromString field the json is not standard.");
                throw new BundleException("Parse module name failed, module-name is invalid.");
            }
            int startIndex = lineStr.lastIndexOf(SEMICOLON, endIndex - 1) + 1;
            String formName = lineStr.substring(startIndex, endIndex);
            if (formName == null || formName.isEmpty()) {
                LOG.error("Compressor::getModuleNameFromString field module-name is empty.");
                throw new BundleException("Parse module name failed, module-name is empty.");
            }
            String[] nameList = formName.split("\\.");
            if (nameList.length <= 1) {
                formNamesList.add(formName);
                utility.addFormNameList(formName);
            }
        } catch (StringIndexOutOfBoundsException exception) {
            LOG.error("Compressor::parseModuleName field module-name is fault: " + exception.getMessage());
            throw new BundleException("Parse module name failed, module-name is invalid.");
        }
    }

    /**
     * Get module name from line string
     *
     * @param lineStr line string
     * @param utility common data
     * @throws BundleException StringIndexOutOfBoundsException
     */
    private void getModuleNameFromString(String lineStr, Utility utility) throws BundleException {
        try {
            int endIndex = lineStr.lastIndexOf(SEMICOLON);
            if (endIndex <= 0) {
                LOG.error("Compressor::getModuleNameFromString field the json is not standard.");
                throw new BundleException("Parse module name failed, module-name is invalid.");
            }
            int startIndex = lineStr.lastIndexOf(SEMICOLON, endIndex - 1) + 1;
            String moduleName = lineStr.substring(startIndex, endIndex);
            list.add(moduleName);
            if (moduleName == null || moduleName.isEmpty()) {
                LOG.error("Compressor::getModuleNameFromString field module-name is empty.");
                throw new BundleException("Parse module name failed, module-name is empty.");
            }
            utility.setModuleName(moduleName);
        } catch (StringIndexOutOfBoundsException exception) {
            LOG.error("Compressor::parseModuleName field module-name is fault: " + exception.getMessage());
            throw new BundleException("Parse module name failed, module-name is invalid.");
        }
    }

    private void parseStageCompressNativeLibs(BufferedReader bufferedReader, Utility utility) throws BundleException {
        StringBuffer buffer = new StringBuffer();
        String lineString = null;
        String jsonString = null;
        try {
            while ((lineString = bufferedReader.readLine()) != null) {
                buffer.append(lineString.trim());
            }
            jsonString = buffer.toString();
        } catch (IOException exception) {
            LOG.error("Compressor::parseStageCompressNativeLibs io exception: " + exception.getMessage());
            throw new BundleException("Parse compress native libs failed.");
        }
        utility.setIsCompressNativeLibs(ModuleJsonUtil.stageIsCompressNativeLibs(jsonString));
    }

    private void parseCompressNativeLibs(BufferedReader bufferedReader, Utility utility) throws BundleException {
        String lineStr = null;
        try {
            while ((lineStr = bufferedReader.readLine()) != null) {
                if (lineStr.contains(COMPRESS_NATIVE_LIBS)) {
                    if (lineStr.contains(Utility.FALSE_STRING)) {
                        utility.setIsCompressNativeLibs(false);
                        break;
                    }
                }
            }
        } catch (IOException exception) {
            LOG.error("Compressor::parseCompressNativeLibs io exception: " + exception.getMessage());
            throw new BundleException("Parse compress native libs failed.");
        }
    }

    /**
     * ZipOutputStream flush, closeEntry and finish.
     */
    private void closeZipOutputStream() {
        try {
            if (zipOut != null) {
                zipOut.flush();
            }
        } catch (IOException exception) {
            LOG.error("Compressor::closeZipOutputStream flush exception " + exception.getMessage());
        }
        try {
            if (zipOut != null) {
                zipOut.closeEntry();
            }
        } catch (IOException exception) {
            LOG.error("Compressor::closeZipOutputStream close entry io exception " + exception.getMessage());
        }
        try {
            if (zipOut != null) {
                zipOut.finish();
            }
        } catch (IOException exception) {
            LOG.error("Compressor::closeZipOutputStream finish exception " + exception.getMessage());
        }
    }

    /**
     * Parse device type from config.json
     *
     * @param bufferedReader config.json buffered Reader
     * @param utility        common data
     * @throws BundleException IOException
     */
    private void parseDeviceType(BufferedReader bufferedReader, Utility utility) throws BundleException {
        String lineStr = null;
        boolean isDeviceTypeStart = false;
        try {
            while ((lineStr = bufferedReader.readLine()) != null) {
                if (!isDeviceTypeStart) {
                    if (lineStr.contains(DEVICE_TYPE)) {
                        isDeviceTypeStart = true;
                    }
                    continue;
                }
                if (lineStr.contains(JSON_END)) {
                    break;
                }
                utility.setDeviceType(lineStr);
                break;
            }
        } catch (IOException exception) {
            LOG.error("Compressor::parseDeviceType io exception: " + exception.getMessage());
            throw new BundleException("Parse device type failed.");
        }
    }

    /**
     * check hap is valid in haps when pack app, check type has bundleName,
     * vendor, version, apiVersion moduleName, packageName.
     *
     * @param fileLists is the list of hapPath.
     * @return true is for successful and false is for failed
     * @throws BundleException FileNotFoundException|IOException.
     */
    private boolean checkHapIsValid(List<String> fileLists, boolean isSharedApp) throws BundleException {
        List<HapVerifyInfo> hapVerifyInfos = new ArrayList<>();
        for (String hapPath : fileLists) {
            if (hapPath.isEmpty()) {
                LOG.error("Compressor::checkHapIsValid input wrong hap file.");
                throw new BundleException("Compressor::checkHapIsValid input wrong hap file.");
            }
            File srcFile = new File(hapPath);
            String fileStr = srcFile.getName();
            if (fileStr.isEmpty()) {
                LOG.error("Compressor::checkHapIsValid get file name failed.");
                throw new BundleException("Compressor::checkHapIsValid get file name failed.");
            }
            if (!fileStr.toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)
                    && !fileStr.toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
                LOG.error("Compressor::checkHapIsValid input wrong hap file.");
                throw new BundleException("Compressor::checkHapIsValid input wrong hap file.");
            }
            if (isModuleHap(hapPath)) {
                hapVerifyInfos.add(parseStageHapVerifyInfo(hapPath));
            } else {
                hapVerifyInfos.add(parseFAHapVerifyInfo(hapPath));
            }
        }
        if (isSharedApp) {
            boolean res = checkSharedAppIsValid(hapVerifyInfos);
            if (!res) {
                return false;
            }
            if (!isOverlay) {
                return true;
            }
        } else {
            for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
                String bundleType = hapVerifyInfo.getBundleType();
                if (TYPE_SHARED.equals(bundleType)) {
                    LOG.error("Compressor::checkHapIsValid shared app should not be included in --hsp-path.");
                    return false;
                }
            }
        }
        setAtomicServiceFileSizeLimit(hapVerifyInfos);
        if (!HapVerify.checkHapIsValid(hapVerifyInfos)) {
            return false;
        }
        return true;
    }

    /**
     * parse stage file to hap verify info from hap path.
     *
     * @param filePath is the hap path
     * @return hapVerifyInfo
     */
    public static HapVerifyInfo parseStageHapVerifyInfo(String filePath) throws BundleException {
        HapVerifyInfo hapVerifyInfo = readStageHapVerifyInfo(filePath);
        hapVerifyInfo.setStageModule(true);
        ModuleJsonUtil.parseStageHapVerifyInfo(hapVerifyInfo);
        hapVerifyInfo.setFileLength(FileUtils.getFileSize(filePath));
        return hapVerifyInfo;
    }

    /**
     * set file size limit for each HapVerifyInfo.
     *
     * @param hapVerifyInfos Indicates hapVerifyInfo list.
     */
    public static void setAtomicServiceFileSizeLimit(List<HapVerifyInfo>hapVerifyInfos) {
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (!hapVerifyInfo.getBundleType().equals(ATOMIC_SERVICE)) {
                continue;
            }
            hapVerifyInfo.setEntrySizeLimit(getEntryModuleSizeLimit());
            hapVerifyInfo.setNotEntrySizeLimit(getNotEntryModuleSizeLimit());
            hapVerifyInfo.setSumSizeLimit(getSumModuleSizeLimit());
        }
    }

    /**
     * parse fa file to hap verify info from hap path.
     *
     * @param filePath is the hap path
     * @return hapVerifyInfo
     */
    public static HapVerifyInfo parseFAHapVerifyInfo(String filePath) throws BundleException {
        HapVerifyInfo hapVerifyInfo = readFAHapVerifyInfo(filePath);
        hapVerifyInfo.setStageModule(false);
        hapVerifyInfo.setFileLength(FileUtils.getFileSize(filePath));
        ModuleJsonUtil.parseFAHapVerifyInfo(hapVerifyInfo);
        return hapVerifyInfo;
    }

    /**
     * read stage hap verify info from hap file.
     *
     * @param srcPath source file to zip
     * @return HapVerifyInfo of parse result
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static HapVerifyInfo readStageHapVerifyInfo(String srcPath) throws BundleException {
        HapVerifyInfo hapVerifyInfo = new HapVerifyInfo();
        ZipFile zipFile = null;
        try {
            File srcFile = new File(srcPath);
            zipFile = new ZipFile(srcFile);
            hapVerifyInfo.setResourceMap(FileUtils.getProfileJson(zipFile));
            hapVerifyInfo.setProfileStr(FileUtils.getFileStringFromZip(MODULE_JSON, zipFile));
        } catch (IOException e) {
            LOG.error("FileUtil::parseStageHapVerifyInfo file not available.");
            throw new BundleException("FileUtil::parseStageHapVerifyInfo file not available.");
        } finally {
            Utility.closeStream(zipFile);
        }
        return hapVerifyInfo;
    }

    /**
     * read fa hap verify info from hap file.
     *
     * @param srcPath source file to zip
     * @return HapVerifyInfo of parse result
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static HapVerifyInfo readFAHapVerifyInfo(String srcPath) throws BundleException {
        HapVerifyInfo hapVerifyInfo = new HapVerifyInfo();
        ZipFile zipFile = null;
        try {
            File srcFile = new File(srcPath);
            zipFile = new ZipFile(srcFile);
            hapVerifyInfo.setProfileStr(FileUtils.getFileStringFromZip(CONFIG_JSON, zipFile));
        } catch (IOException e) {
            LOG.error("FileUtil::parseStageHapVerifyInfo file not available.");
            throw new BundleException("FileUtil::parseStageHapVerifyInfo file not available.");
        } finally {
            Utility.closeStream(zipFile);
        }
        return hapVerifyInfo;
    }

    /**
     * compress in hqf mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressHQFMode(Utility utility) throws BundleException {
        pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

        if (!utility.getEtsPath().isEmpty()) {
            pathToFile(utility, utility.getEtsPath(), ETS_PATH, false);
        }
        if (!utility.getLibPath().isEmpty()) {
            pathToFile(utility, utility.getLibPath(), LIBS_DIR_NAME, false);
        }
    }

    /**
     * compress in appqf mode.
     *
     * @param utility common data
     * @throws BundleException FileNotFoundException|IOException.
     */
    private void compressAPPQFMode(Utility utility) throws BundleException {
        List<String> fileList = utility.getFormatedHQFList();
        if (!checkHQFIsValid(fileList)) {
            LOG.error("checkHQFIsValid failed when pack appqf file.");
            throw new BundleException("checkHQFIsValid failed when pack appqf file.");
        }
        for (String hapPath : fileList) {
            pathToFile(utility, hapPath, NULL_DIR_NAME, false);
        }
    }

    /**
     * check input hqf is valid.
     *
     * @param fileList is input path of hqf files
     * @throws BundleException FileNotFoundException|IOException.
     */
    private boolean checkHQFIsValid(List<String> fileList) throws BundleException {
        List<HQFInfo> hqfVerifyInfos = new ArrayList<>();
        for (String file : fileList) {
            hqfVerifyInfos.add(ModuleJsonUtil.parseHQFInfo(file));
        }
        if (!HQFVerify.checkHQFIsValid(hqfVerifyInfos)) {
            LOG.error("input hqf is invalid.");
            return false;
        }
        return true;
    }

    private void compressHSPMode(Utility utility) throws BundleException {
        pathToFile(utility, utility.getJsonPath(), NULL_DIR_NAME, false);

        pathToFile(utility, utility.getProfilePath(), NULL_DIR_NAME, false);

        if (!utility.getIndexPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String assetsPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getIndexPath(), assetsPath, false);
        }

        if (!utility.getLibPath().isEmpty()) {
            pathToFile(utility, utility.getLibPath(), LIBS_DIR_NAME, utility.isCompressNativeLibs());
        }

        if (!utility.getANPath().isEmpty()) {
            pathToFile(utility, utility.getANPath(), AN_DIR_NAME, false);
        }

        if (!utility.getFilePath().isEmpty()) {
            pathToFile(utility, utility.getFilePath(), NULL_DIR_NAME, false);
        }

        if (!utility.getResPath().isEmpty() && !utility.getModuleName().isEmpty()) {
            String resPath = ASSETS_DIR_NAME + utility.getModuleName() + LINUX_FILE_SEPARATOR
                    + RESOURCES_DIR_NAME;
            if (DEVICE_TYPE_FITNESSWATCH.equals(
                utility.getDeviceType().replace(SEMICOLON, EMPTY_STRING).trim()) ||
                    DEVICE_TYPE_FITNESSWATCH_NEW.equals(
                        utility.getDeviceType().replace(SEMICOLON, EMPTY_STRING).trim())) {
                resPath = RES_DIR_NAME;
            }
            pathToFile(utility, utility.getResPath(), resPath, false);
        }

        if (!utility.getResourcesPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String resourcesPath = RESOURCES_DIR_NAME;
            pathToFile(utility, utility.getResourcesPath(), resourcesPath, false);
        }
        if (!utility.getJsPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String jsPath = JS_PATH;
            pathToFile(utility, utility.getJsPath(), jsPath, false);
        }

        if (!utility.getEtsPath().isEmpty() && isModuleJSON(utility.getJsonPath())) {
            String etsPath = ETS_PATH;
            pathToFile(utility, utility.getEtsPath(), etsPath, false);
        }

        if (!utility.getRpcidPath().isEmpty()) {
            String rpcidPath = NULL_DIR_NAME;
            pathToFile(utility, utility.getRpcidPath(), rpcidPath, false);
        }

        if (!utility.getAssetsPath().isEmpty()) {
            pathToFile(utility, utility.getAssetsPath(), ASSETS_DIR_NAME, false);
        }

        if (!utility.getBinPath().isEmpty()) {
            pathToFile(utility, utility.getBinPath(), NULL_DIR_NAME, false);
        }

        if (!utility.getPackInfoPath().isEmpty()) {
            pathToFile(utility, utility.getPackInfoPath(), NULL_DIR_NAME, false);
        }

        compressHapModeMultiple(utility);
    }

    private static boolean checkSharedAppIsValid(List<HapVerifyInfo> hapVerifyInfos) {
        if (hapVerifyInfos.isEmpty()) {
            LOG.error("no module included");
            return false;
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (!hapVerifyInfo.getTargetBundleName().isEmpty()) {
                isOverlay = true;
                return true;
            }
        }

        if (!ONE.equals(hapVerifyInfos.size())) {
            LOG.error("Shared app only can contain one module.");
            return false;
        }
        HapVerifyInfo sharedHspInfo = hapVerifyInfos.get(0);
        if (!sharedHspInfo.getDependencyItemList().isEmpty()) {
            LOG.error("Shared hsp cannot depend on other modules.");
            return false;
        }
        String bundleType = sharedHspInfo.getBundleType();
        if (TYPE_SHARED.equals(bundleType)) {
            LOG.error("The input hsp is invalid for shared app.");
            return false;
        }
        return true;
    }
}
