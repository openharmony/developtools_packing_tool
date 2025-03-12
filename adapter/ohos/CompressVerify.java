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

import com.alibaba.fastjson.JSONValidator;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.List;
import java.util.Optional;

/**
 * compress comment,command parser.
 *
 */
public class CompressVerify {
    private static final String COMMA_SPLIT = ",";
    private static final String JSON_PROFILE = "config.json";
    private static final String MODULE_PROFILE = "module.json";
    private static final String PATCH_PROFILE = "patch.json";
    private static final String PKG_CONTEXT_INFO = "pkgContextInfo.json";
    private static final String PROFILE_NAME = "CAPABILITY.profile";
    private static final String INDEX_PROFILE = "resources.index";
    private static final String RPCID_PROFILE = "rpcid.sc";
    private static final String PACK_INFO = "pack.info";
    private static final String PACK_RES = "pack.res";
    private static final String HAP_SUFFIX = ".hap";
    private static final String HAR_SUFFIX = ".har";
    private static final String APP_SUFFIX = ".app";
    private static final String APK_SUFFIX = ".apk";
    private static final String DEX_SUFFIX = ".dex";
    private static final String ABC_SUFFIX = ".abc";
    private static final String SO_SUFFIX = ".so";
    private static final String JAR_SUFFIX = ".jar";
    private static final String TXT_SUFFIX = ".txt";
    private static final String PNG_SUFFIX = ".png";
    private static final String RES_SUFFIX = ".res";
    private static final String HQF_SUFFIX = ".hqf";
    private static final String APPQF_SUFFIX = ".appqf";
    private static final String HSP_SUFFIX = ".hsp";
    private static final String JSON_SUFFIX = ".json";
    private static final String FALSE = "false";
    private static final String ENTRY_CARD_DIRECTORY_NAME = "EntryCard";
    private static final String VERSION_NAME_PATTERN = "^[0-9.]+|(?=.*[{])(?=.*[}])[0-9a-zA-Z_.{}]+$";
    private static final String LINUX_FILE_SEPARATOR = "/";
    private static final String BUNDLE_TYPE_SHARE = "shared";
    private static final String BUNDLE_TYPE_APP = "app";
    private static final String BUNDLE_TYPE_APP_SERVICE = "appService";
    private static final String SKILLS_ENTITIES = "entities";
    private static final String SKILLS_ACTIONS = "actions";
    private static final String ACTION_SYSTEM_HOME = "action.system.home";
    private static final String ENTITY_SYSTEM_HOME = "entity.system.home";
    private static final String BUNDLE_NAME_PATTERN =
            "([a-zA-Z]|[a-zA-Z]+(_*[0-9a-zA-Z])+)(\\.[0-9a-zA-Z]|\\.[0-9a-zA-Z]+(_*[0-9a-zA-Z])+){2,}";
    private static final int BUNDLE_NAME_LEN_MIN = 7;
    private static final int BUNDLE_NAME_LEN_MAX = 128;

    private static final Log LOG = new Log(CompressVerify.class.toString());

    private static final boolean TYPE_FILE = true;
    private static final boolean TYPE_DIR = false;
    private static final Integer ONE = 1;

    /**
     * if args valid.
     *
     * @param utility common data
     * @return commandVerify if command valid.
     */
    public static boolean commandVerify(Utility utility) {
        if (utility == null) {
            LOG.error(PackingToolErrMsg.COMMAND_VERIFY_FAILED.toString("Command verify utility is null."));
            return false;
        }

        if (!utility.getForceRewrite().isEmpty() && !"true".equals(utility.getForceRewrite())
                && !"false".equals(utility.getForceRewrite())) {
            LOG.error(PackingToolErrMsg.COMMAND_VERIFY_FAILED.toString(
                "If the --force parameter is configured, the value must be either 'true' or 'false'."));
            return false;
        }
        return commandPathVerify(utility);
    }

    /**
     * verify path.
     *
     * @param utility common data
     * @return commandPathVerify if command valid.
     */
    private static boolean commandPathVerify(Utility utility) {
        switch (utility.getMode()) {
            case Utility.MODE_HAP:
                if (!utility.getBinPath().isEmpty() && utility.getJsonPath().isEmpty()) {
                    return isOutPathValid(utility, HAP_SUFFIX);
                } else {
                    return isVerifyValidInHapCommonMode(utility) && isVerifyValidInHapMode(utility);
                }
            case Utility.MODE_HAR:
                return isVerifyValidInHarMode(utility);
            case Utility.MODE_APP:
                return isVerifyValidInAppMode(utility);
            case Utility.MODE_FAST_APP:
                return PackageUtil.isVerifyValidInFastAppMode(utility);
            case Utility.MODE_RES:
                return isVerifyValidInResMode(utility);
            case Utility.MODE_MULTI_APP:
                return isVerifyValidInMultiAppMode(utility);
            case Utility.MODE_HQF:
                return isVerifyValidInHQFMode(utility);
            case Utility.MODE_APPQF:
                return isVerifyValidInAPPQFMode(utility);
            case Utility.MODE_HSP:
                return isVerifyValidInHspMode(utility);
            case Utility.MODE_HAPADDITION:
                return isVerifyValidInHapAdditionMode(utility);
            case Utility.VERSION_NORMALIZE:
                return validateVersionNormalizeMode(utility);
            case Utility.PACKAGE_NORMALIZE:
                return validatePackageNormalizeMode(utility);
            default:
                LOG.error(PackingToolErrMsg.COMMAND_MODE_INVALID.toString());
                return false;
        }
    }

    private static boolean isBundleNameValid(String bundleName) {
        if (bundleName != null &&
                bundleName.length() >= BUNDLE_NAME_LEN_MIN &&
                bundleName.length() <= BUNDLE_NAME_LEN_MAX) {
            Pattern pattern = Pattern.compile(BUNDLE_NAME_PATTERN);
            return pattern.matcher(bundleName).matches();
        }
        return false;
    }

    private static boolean validatePackageNormalizeMode(Utility utility) {
        if (utility.getHspList().isEmpty()) {
            LOG.error("CompressVerify::validatePackageNormalizeMode hsp-list is empty.");
            return false;
        } else {
            if (!compatibleProcess(utility, utility.getHspList(), utility.getFormattedHspPathList(), HSP_SUFFIX)) {
                LOG.error("CompressVerify::validatePackageNormalizeMode hsp-list is invalid.");
                return false;
            }
        }
        if (!isBundleNameValid(utility.getBundleName())) {
            LOG.error("CompressVerify::validatePackageNormalizeMode bundle-name is invalid.");
            return false;
        }
        if (utility.getVersionCode() <= 0) {
            LOG.error("CompressVerify::validatePackageNormalizeMode version-code is invalid.");
            return false;
        }
        if (utility.getOutPath().isEmpty()) {
            LOG.error("CompressVerify::validatePackageNormalizeMode out-path is empty.");
            return false;
        }
        File outDir = new File(utility.getOutPath());
        if (!outDir.isDirectory()) {
            LOG.error("CompressVerify::validatePackageNormalizeMode out-path is not a directory.");
            return false;
        }
        return true;
    }

    private static boolean validateVersionNormalizeMode(Utility utility) {
        if (utility.getInputList().isEmpty()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode input-list is empty.");
            return false;
        }

        if (!handleHapAndHspInput(utility, utility.getInputList(), utility.getFormattedHapList())) {
            LOG.error("CompressVerify::validateVersionNormalizeMode input-list is invalid.");
            return false;
        }

        if (utility.getFormattedHapList().isEmpty()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode input-list is empty.");
            return false;
        }

        if (utility.getVersionCode() <= 0) {
            LOG.error("CompressVerify::validateVersionNormalizeMode version-code is invalid.");
            return false;
        }

        if (utility.getVersionName().isEmpty()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode version-name is empty.");
            return false;
        }

        Pattern versionNamePattern = Pattern.compile(VERSION_NAME_PATTERN);
        Matcher versionNameMatcher = versionNamePattern.matcher(utility.getVersionName());
        if (!versionNameMatcher.matches()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode version-name is not valid.");
            return false;
        }

        if (utility.getOutPath().isEmpty()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode out-path is empty.");
            return false;
        }

        File outDir = new File(utility.getOutPath());
        if (!outDir.isDirectory()) {
            LOG.error("CompressVerify::validateVersionNormalizeMode out-path is not a directory.");
            return false;
        }
        return true;
    }

    private static boolean isValidRpcid(Utility utility) {
        if (!utility.getRpcidPath().isEmpty()) {
            File file = new File(utility.getRpcidPath());
            if (!file.isFile()) {
                String errMsg = "--rpcid-path is not a file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
            if (!RPCID_PROFILE.equals(file.getName())) {
                String errMsg = "--rpcid-path must be the rpcid.sc file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
        }
        return true;
    }

    private static boolean isValidPackInfo(Utility utility) {
        if (!utility.getPackInfoPath().isEmpty()) {
            File file = new File(utility.getPackInfoPath());
            if (!file.isFile()) {
                String errMsg = "--pack-info-path is not a file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
            if (!PACK_INFO.equals(file.getName())) {
                String errMsg = "--pack-info-path must be the pack.info file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
        }
        return true;
    }

    private static boolean isVerifyValidInHapCommonMode(Utility utility) {
        if (utility.getJsonPath().isEmpty()) {
            String errMsg = "--json-path is empty";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }
        if (!isPathValid(utility.getJsonPath(), TYPE_FILE, JSON_PROFILE)
                && !isPathValid(utility.getJsonPath(), TYPE_FILE, MODULE_PROFILE)) {
            String errMsg = "--json-path must be the config.json file or module.json file.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!isValidRpcid(utility) || !isValidPackInfo(utility)) {
            return false;
        }

        if (!utility.getApkPath().isEmpty() && !compatibleProcess(utility, utility.getApkPath(),
                utility.getFormattedApkPathList(), APK_SUFFIX)) {
            String errMsg = "--shell-apk-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getProfilePath().isEmpty()) {
            File file = new File(utility.getProfilePath());
            if (!file.isFile() || !PROFILE_NAME.equals(file.getName())) {
                String errMsg = "--profile-path must be the CAPABILITY.profile file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
        }

        if (!utility.getDexPath().isEmpty() && !compatibleProcess(utility, utility.getDexPath(),
                utility.getFormattedDexPathList(), DEX_SUFFIX)) {
            String errMsg = "--dex-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getAbcPath().isEmpty() && !compatibleProcess(utility, utility.getAbcPath(),
                utility.getFormattedAbcPathList(), ABC_SUFFIX)) {
            String errMsg = "--abc-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getDirList().isEmpty() && !splitDirList(utility, utility.getDirList(),
                utility.getFormatedDirList())) {
            String errMsg = "--dir-list is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }
        if (!utility.getPkgContextPath().isEmpty()) {
            File file = new File(utility.getPkgContextPath());
            if (!file.isFile() || !PKG_CONTEXT_INFO.equals(file.getName())) {
                String errMsg = "--pkg-context-path file must be the pkgContextInfo.json file.";
                LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
        }
        return true;
    }

    /**
     * parse and check args if valid in hap mode.
     *
     * @param utility common data
     * @return isVerifyValidInHapMode if verify valid in hap mode.
     */
    private static boolean isVerifyValidInHapMode(Utility utility) {
        File file = new File(utility.getIndexPath());
        if (!utility.getIndexPath().isEmpty() && !file.isFile() && INDEX_PROFILE.equals(file.getName())) {
            String errMsg = "--index-path must be the resources.index file.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getSoPath().isEmpty() &&
                !compatibleProcess(utility, utility.getSoPath(), utility.getFormattedSoPathList(), SO_SUFFIX)) {
            String errMsg = "--maple-so-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getAbilitySoPath().isEmpty() && !compatibleProcess(utility, utility.getAbilitySoPath(),
                utility.getFormattedAbilitySoPathList(), SO_SUFFIX)) {
            String errMsg = "--ability-so-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getSoDir())) {
            String errMsg = "--maple-so-dir is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getLibPath())) {
            String errMsg = "--lib-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getHnpPath())) {
            String errMsg = "--hnp-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getResPath())) {
            String errMsg = "--res-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getResourcesPath())) {
            String errMsg = "--resources-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getAssetsPath())) {
            String errMsg = "--assets-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getSharedLibsPath())) {
            String errMsg = "--shared-libs-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getJarPath().isEmpty()
                && !compatibleProcess(utility, utility.getJarPath(), utility.getFormattedJarPathList(), JAR_SUFFIX)) {
            String errMsg = "--jar-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getTxtPath().isEmpty()
                && !compatibleProcess(utility, utility.getTxtPath(), utility.getFormattedTxtPathList(), TXT_SUFFIX)) {
            String errMsg = "--txt-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getANPath())) {
            String errMsg = "--an-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getEtsPath().isEmpty() && !isPathExists(utility.getEtsPath())) {
            String errMsg = "--ets-path is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        return isOutPathValid(utility, HAP_SUFFIX);
    }

    /**
     * check hap path if valid
     *
     * @param path path input
     * @return isPathValid if path verify
     */
    private static boolean isHapPathValid(String path) {
        return (!path.isEmpty() && !isPathValid(path, TYPE_DIR, null));
    }

    /**
     * parse and check args if valid in har mode.
     *
     * @param utility common data
     * @return isVerifyValidInHarMode if verify valid in har mode.
     */
    private static boolean isVerifyValidInHarMode(Utility utility) {
        if (utility.getJsonPath().isEmpty()) {
            LOG.error("CompressVerify::isArgsValidInHarMode json-path is empty.");
            return false;
        }

        if (!isPathValid(utility.getJsonPath(), TYPE_FILE, JSON_PROFILE)
                && !isPathValid(utility.getJsonPath(), TYPE_FILE, MODULE_PROFILE)) {
            LOG.error("CompressVerify::isArgsValidInHarMode json-path must be config.json file.");
            return false;
        }

        if (!utility.getJarPath().isEmpty()
                && !compatibleProcess(utility, utility.getJarPath(), utility.getFormattedJarPathList(), JAR_SUFFIX)) {
            LOG.error("CompressVerify::isArgsValidInHarMode jar-path is invalid.");
            return false;
        }

        if (!utility.getTxtPath().isEmpty()
                && !compatibleProcess(utility, utility.getTxtPath(), utility.getFormattedTxtPathList(), TXT_SUFFIX)) {
            LOG.error("CompressVerify::isArgsValidInHarMode txt-path is invalid.");
            return false;
        }

        if (!utility.getLibPath().isEmpty() && !isPathValid(utility.getLibPath(), TYPE_DIR, null)) {
            LOG.error("CompressVerify::isArgsValidInHarMode lib-path is invalid.");
            return false;
        }

        if (!utility.getResPath().isEmpty() && !isPathValid(utility.getResPath(), TYPE_DIR, null)) {
            LOG.error("CompressVerify::isArgsValidInHarMode res-path is invalid.");
            return false;
        }

        if (utility.getResourcesPath().isEmpty() || !isPathValid(utility.getResourcesPath(), TYPE_DIR, null)) {
            LOG.error("CompressVerify::isArgsValidInHarMode resources-path is invalid.");
            return false;
        }

        if (!utility.getAssetsPath().isEmpty() && !isPathValid(utility.getAssetsPath(), TYPE_DIR, null)) {
            LOG.error("CompressVerify::isArgsValidInHarMode assets-path is invalid.");
            return false;
        }

        return isOutPathValid(utility, HAR_SUFFIX);
    }

    /**
     * parse and check args if valid in app mode.
     *
     * @param utility common data
     * @return isVerifyValidInAppMode if verify valid in app mode.
     */
    private static boolean isVerifyValidInAppMode(Utility utility) {
        if (!checkBundleTypeConsistency(utility)) {
            String errMsg = "The bundleType is inconsistent for different HAP and HSP modules.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!checkInputModulePath(utility)) {
            LOG.warning("CompressVerify::isArgsValidInAppMode input hap-path or hspPath is invalid.");
        }

        if (!utility.getHapPath().isEmpty()
                && !compatibleProcess(utility, utility.getHapPath(), utility.getFormattedHapPathList(), HAP_SUFFIX)) {
            String errMsg = "--hap-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getHspPath().isEmpty()
                && !compatibleProcess(utility, utility.getHspPath(), utility.getFormattedHspPathList(), HSP_SUFFIX)) {
            String errMsg = "--hsp-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (utility.getPackInfoPath().isEmpty()) {
            String errMsg = "--pack-info-path is empty.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        File file = new File(utility.getPackInfoPath());
        if (!file.isFile() || !PACK_INFO.equals(file.getName())) {
            String errMsg = "--pack-info-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!isValidEncryptJsonFile(utility)) {
            String errMsg = "--encrypt-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getSignaturePath().isEmpty() && !(new File(utility.getSignaturePath())).isFile()) {
            String errMsg = "--signature-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getCertificatePath().isEmpty() && !(new File(utility.getCertificatePath())).isFile()) {
            String errMsg = "--certificate-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getEntryCardPath().isEmpty() &&
                !compatibleProcess(utility, utility.getEntryCardPath(),
                        utility.getformattedEntryCardPathList(), PNG_SUFFIX)) {
            String errMsg = "--entrycard-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }
        if (!utility.getPackResPath().isEmpty() && !isPathValid(utility.getPackResPath(), TYPE_FILE, PACK_RES)) {
            String errMsg = "--pack-res-path is invalid.";
            LOG.error(PackingToolErrMsg.APP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        return isOutPathValid(utility, APP_SUFFIX);
    }

    private static boolean checkBundleTypeConsistency(Utility utility) {
        String bundleType = new String();
        List<String> tmpHapPathList = new ArrayList<>();
        List<String> tmpHspPathList = new ArrayList<>();
        compatibleProcess(utility, utility.getHapPath(), tmpHapPathList, HAP_SUFFIX);
        compatibleProcess(utility, utility.getHspPath(), tmpHspPathList, HSP_SUFFIX);
        try {
            if (!tmpHapPathList.isEmpty()) {
                HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(tmpHapPathList.get(0));
                bundleType = hapVerifyInfo.getBundleType();
            } else if (!tmpHspPathList.isEmpty()) {
                HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(tmpHspPathList.get(0));
                bundleType = hapVerifyInfo.getBundleType();
            }
            for (String hapPath : tmpHapPathList) {
                HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(hapPath);
                if (!bundleType.equals(hapVerifyInfo.getBundleType())) {
                    LOG.error(PackingToolErrMsg.CHECK_BUNDLETYPE_CONSISTENCY_FAILED.toString(
                        "The bundleType is not same for different HAP modules."));
                    return false;
                }
            }
            for (String hspPath : tmpHspPathList) {
                HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(hspPath);
                if (!bundleType.equals(hapVerifyInfo.getBundleType())) {
                    LOG.error(PackingToolErrMsg.CHECK_BUNDLETYPE_CONSISTENCY_FAILED.toString(
                        "The bundleType is not same for different HSP modules."));
                    return false;
                }
            }
        } catch (BundleException e) {
            return true;
        }
        return true;
    }

    private static boolean checkInputModulePath(Utility utility) {
        boolean isSharedApp = isSharedApp(utility);
        boolean isAppService = isAppService(utility);
        if (utility.getHapPath().isEmpty() && !isSharedApp && !isAppService) {
            LOG.warning("CompressVerify::CheckInputModulePath hap-path is empty.");
            return false;
        }

        if (utility.getHspPath().isEmpty() && isAppService) {
            LOG.warning("CompressVerify::CheckInputModulePath hsp-path is empty.");
            return false;
        }
        return true;
    }

    /**
     * parse and check args if valid in multiApp mode.
     *
     * @param utility common data
     * @return isVerifyValidInMultiAppMode if verify valid in multiApp mode.
     */
    private static boolean isVerifyValidInMultiAppMode(Utility utility) {
        if (utility.getAppList().isEmpty() && utility.getHapList().isEmpty() && utility.getHspList().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInMultiAppMode input app-list, hap-list and hsp-list are null.");
            return false;
        }
        if (!utility.getAppList().isEmpty()) {
            if (!compatibleProcess(utility, utility.getAppList(), utility.getFormattedAppList(), APP_SUFFIX)) {
                LOG.error("CompressVerify::isVerifyValidInMultiAppMode app-list is invalid.");
                return false;
            }
        }
        if (!utility.getHapList().isEmpty()) {
            if (!compatibleProcess(utility, utility.getHapList(), utility.getFormattedHapList(), HAP_SUFFIX)) {
                LOG.error("CompressVerify::isVerifyValidInMultiAppMode hap-list is invalid.");
                return false;
            }
        }

        if (!utility.getHspList().isEmpty()) {
            if (!compatibleProcess(utility, utility.getHspList(), utility.getFormattedHapList(), HSP_SUFFIX)) {
                LOG.error("CompressVerify::isVerifyValidInMultiAppMode hsp-list is invalid.");
                return false;
            }
        }

        if (!isValidEncryptJsonFile(utility)) {
            LOG.error("CompressVerify::isVerifyValidInMultiAppMode encrypt-path is invalid.");
            return false;
        }

        File outFile = new File(utility.getOutPath());
        if (("false".equals(utility.getForceRewrite())) && outFile.exists()) {
            LOG.error("CompressVerify::isVerifyValidInMultiAppMode out file already existed.");
            return false;
        }
        if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(APP_SUFFIX)) {
            LOG.error("CompressVerify::isVerifyValidInMultiAppMode out-path must end with .app.");
            return false;
        }
        return true;
    }


    /**
     * parse and check args if valid in res mode.
     *
     * @param utility common data
     * @return isVerifyValidInAppMode if verify valid in app mode.
     */
    private static boolean isVerifyValidInResMode(Utility utility) {
        if (!isPathValid(utility.getPackInfoPath(), TYPE_FILE, PACK_INFO)) {
            LOG.error("CompressVerify::isArgsValidInResMode pack-info-path is invalid.");
            return false;
        }

        if (!isDirectoryValidStrictCase(utility.getEntryCardPath(), ENTRY_CARD_DIRECTORY_NAME)) {
            LOG.error("CompressVerify::isArgsValidInResMode the level-1 directory name must is EntryCard" +
                ", current is " + utility.getEntryCardPath());
            return false;
        }
        if (!compatibleProcess(utility, utility.getEntryCardPath(),
                utility.getformattedEntryCardPathList(), PNG_SUFFIX)) {
            LOG.error("CompressVerify::isArgsValidInResMode entrycard-path is invalid.");
            return false;
        }
        return isOutPathValid(utility, RES_SUFFIX);
    }

    private static boolean isVerifyValidInHQFMode(Utility utility) {
        if (utility.getJsonPath().isEmpty()) {
            LOG.error("must input patch.json file when pack hqf file.");
            return false;
        }
        if (!utility.getEtsPath().isEmpty()) {
            if (!isPathValid(utility.getEtsPath(), TYPE_DIR, null)) {
                LOG.error("must input valid ets path when pack hqf file.");
                return false;
            }
        }
        if (!isPathValid(utility.getJsonPath(), TYPE_FILE, PATCH_PROFILE)) {
            LOG.error("input patch.json is invalid when pack hqf file.");
            return false;
        }
        if (!utility.getLibPath().isEmpty()) {
            if (!isPathValid(utility.getLibPath(), TYPE_DIR, null)) {
                LOG.error("input lib path is invalid when pack hqf file.");
                return false;
            }
        }
        if (!utility.getResourcesPath().isEmpty()) {
            if (!isPathValid(utility.getResourcesPath(), TYPE_DIR, null)) {
                LOG.error("input resources path is invalid when pack hqf file.");
                return false;
            }
        }
        File outFile = new File(utility.getOutPath());
        if ((FALSE.equals(utility.getForceRewrite())) && (outFile.exists())) {
            LOG.error(outFile.getName() + " already exist.");
            return false;
        }
        if (!utility.getOutPath().endsWith(HQF_SUFFIX)) {
            LOG.error("input out file must end with .hqf.");
            return false;
        }
        return true;
    }

    private static boolean isVerifyValidInAPPQFMode(Utility utility) {
        if (utility.getHqfList().isEmpty()) {
            LOG.error("input hqf list is empty.");
            return false;
        }
        if (!compatibleProcess(utility, utility.getHqfList(), utility.getFormatedHQFList(), HQF_SUFFIX)) {
            LOG.error("input hqf list is invalid.");
            return false;
        }
        File outFile = new File(utility.getOutPath());
        if ((FALSE.equals(utility.getForceRewrite())) && outFile.exists()) {
            LOG.error("Error out file already existed.");
            return false;
        }
        if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(APPQF_SUFFIX)) {
            LOG.error("Error out-path must end with .app.");
            return false;
        }
        return true;
    }

    /**
     * Compatible file input and directory input
     *
     * @param utility   common data
     * @param inputPath input path
     * @param fileList  save files' path with list
     * @param suffix    process type
     * @return Returns {@code true} if the compatible is successful; returns {@code false} otherwise.
     */
    public static boolean compatibleProcess(Utility utility, String inputPath,
        List<String> fileList, String suffix) {
        if (isPathValid(inputPath, TYPE_DIR, null)) {
            File inputFile = new File(inputPath);
            File[] files = inputFile.listFiles();
            if (files == null) {
                return true;
            }
            for (File fileItem : files) {
                if (fileItem.getName().toLowerCase(Locale.ENGLISH).endsWith(suffix)) {
                    fileList.add(fileItem.toString());
                }
            }
            return true;
        } else {
            String formattedPathItem = "";
            List<String> pathList = removeDuplicatePath(inputPath);
            for (String pathItem : pathList) {
                formattedPathItem = utility.getFormattedPath(pathItem);
                if (!isPathValid(formattedPathItem, TYPE_FILE, suffix)) {
                    return false;
                }
                fileList.add(formattedPathItem);
            }
            return true;
        }
    }

    private static boolean handleHapAndHspInput(Utility utility, String inputPath, List<String> fileList) {
        if (isPathValid(inputPath, TYPE_DIR, null)) {
            File inputFile = new File(inputPath);
            File[] files = inputFile.listFiles();
            if (files == null) {
                return true;
            }
            for (File fileItem : files) {
                if (fileItem.getName().toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX) ||
                    fileItem.getName().toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)) {
                    fileList.add(fileItem.toString());
                }
            }
            return true;
        } else {
            String formattedPathItem = "";
            List<String> pathList = removeDuplicatePath(inputPath);
            for (String pathItem : pathList) {
                formattedPathItem = utility.getFormattedPath(pathItem);
                if (!isPathValid(formattedPathItem, TYPE_FILE, HSP_SUFFIX) &&
                    !isPathValid(formattedPathItem, TYPE_FILE, HAP_SUFFIX)) {
                    LOG.error("input file " + formattedPathItem + " not valid");
                    return false;
                }
                fileList.add(formattedPathItem);
            }
            return true;
        }
    }

    private static boolean splitDirList(Utility utility, String dirList, List<String> fileList) {
        List<String> pathList = removeDuplicatePath(dirList);
        for (String pathItem : pathList) {
            String formattedPathItem = utility.getFormattedPath(pathItem);
            if (!isPathValid(formattedPathItem, TYPE_DIR, null)) {
                return false;
            }
            fileList.add(formattedPathItem);
        }
        return true;
    }

    /**
     * turn input path block to formatted path list
     *
     * @param utility common data
     * @param suffix  used to determine type
     * @return isVerifyValidInAppMode if verify valid in app mode.
     */
    private static boolean isOutPathValid(Utility utility, String suffix) {
        File outFile = new File(utility.getOutPath());

        if (("false".equals(utility.getForceRewrite())) && (outFile.exists())) {
            LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path file already existed."));
            return false;
        }

        if (HAP_SUFFIX.equals(suffix)) {
            if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(HAP_SUFFIX)) {
                LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path must end with .hap."));
                return false;
            } else {
                return true;
            }
        }

        if (HAR_SUFFIX.equals(suffix)) {
            if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(HAR_SUFFIX)) {
                LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path must end with .har."));
                return false;
            } else {
                return true;
            }
        }

        if (APP_SUFFIX.equals(suffix)) {
            if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(APP_SUFFIX)) {
                LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path must end with .app."));
                return false;
            } else {
                return true;
            }
        }

        if (RES_SUFFIX.equals(suffix)) {
            if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(RES_SUFFIX)) {
                LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path must end with .res."));
                return false;
            } else {
                return true;
            }
        }

        if (HSP_SUFFIX.equals(suffix)) {
            if (!outFile.getName().toLowerCase(Locale.ENGLISH).endsWith(HSP_SUFFIX)) {
                LOG.error(PackingToolErrMsg.OUT_PATH_INVALID.toString("--out-path must end with .hsp."));
                return false;
            } else {
                return true;
            }
        }
        return false;
    }

    /**
     * check path if valid
     *
     * @param path   path input
     * @param isFile type input
     * @param flag   flag input
     * @return isPathValid if path verify
     */
    private static boolean isPathValid(String path, boolean isFile, String flag) {
        File file = new File(path);
        if (isFile && (file.isFile()) && file.getName().toLowerCase(Locale.ENGLISH).endsWith(flag)) {
            return true;
        }
        return (!isFile) && file.isDirectory();
    }

    private static boolean isPathExists(String path) {
        if (path != null && !path.isEmpty()) {
            File filePath = new File(path);
            return filePath.exists();
        }
        return false;
    }

    private static boolean isDirectoryValidStrictCase(String path, String directoryName) {
        File file = new File(path);
        if (!file.exists()) {
            LOG.error("CompressVerify::isDirectoryValidStrictCase directory is not exist, directoryPath: "
                + path + ".");
            return false;
        }
        if (file.isDirectory()) {
            return directoryName.equals(file.getName());
        }
        return false;
    }

    /**
     * remove duplicate in path.
     *
     * @param path input path, use comma separate.
     * @return result list
     */
    private static List<String> removeDuplicatePath(String path) {
        String[] array = path.split(COMMA_SPLIT);
        List<String> list = new ArrayList<>();

        for (String item : array) {
            if (!list.contains(item)) {
                list.add(item);
            }
        }
        return list;
    }

    private static boolean isVerifyValidInHspMode(Utility utility) {
        if (utility.getJsonPath().isEmpty()) {
            String errMsg = "--json-path is empty.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!isPathValid(utility.getJsonPath(), TYPE_FILE, MODULE_PROFILE)) {
            String errMsg = "--json-path must be the module.json file.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if ((isBundleTypeShared(utility) || isBundleTypeAppService(utility))) {
            boolean hspHasAbilities = hspHasAbilities(utility);
            boolean hspHasExtensionAbilities = hspHasExtensionAbilities(utility);
            if (hspHasAbilities && hspHasExtensionAbilities) {
                String errMsg = "shared/appService hsp has abilities and extensionAbilities.";
                LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
            if (hspHasAbilities) {
                LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString("shared/appService hsp has abilities."));
                return false;
            }
            if (hspHasExtensionAbilities) {
                LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(
                        "shared/appService hsp has extensionAbilities."));
                return false;
            }
        }

        if(hasHomeAbility(utility)) {
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString("hsp has entry ability."));
            return false;
        }

        if (hasHomeExtensionAbility(utility)) {
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString("hsp has entry extensionAbility."));
            return false;
        }

        if (!utility.getJarPath().isEmpty()
                && !compatibleProcess(utility, utility.getJarPath(), utility.getFormattedJarPathList(), JAR_SUFFIX)) {
            String errMsg = "--jar-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getTxtPath().isEmpty()
                && !compatibleProcess(utility, utility.getTxtPath(), utility.getFormattedTxtPathList(), TXT_SUFFIX)) {
            String errMsg = "--txt-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getLibPath().isEmpty() && !isPathValid(utility.getLibPath(), TYPE_DIR, null)) {
            String errMsg = "--lib-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getResPath().isEmpty() && !isPathValid(utility.getResPath(), TYPE_DIR, null)) {
            String errMsg = "--res-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getResourcesPath().isEmpty() && !isPathValid(utility.getResourcesPath(), TYPE_DIR, null)) {
            String errMsg = "--resources-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getAssetsPath().isEmpty() && !isPathValid(utility.getAssetsPath(), TYPE_DIR, null)) {
            String errMsg = "--assets-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getDirList().isEmpty() && !splitDirList(utility, utility.getDirList(),
                utility.getFormatedDirList())) {
            String errMsg = "--dir-list is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getAPPath())) {
            String errMsg = "--ap-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (isHapPathValid(utility.getANPath())) {
            String errMsg = "--an-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        if (!utility.getPkgContextPath().isEmpty()) {
            File file = new File(utility.getPkgContextPath());
            if (!file.isFile() || !PKG_CONTEXT_INFO.equals(file.getName())) {
                String errMsg = "--pkg-context-path file must be the pkgContextInfo.json file.";
                LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
                return false;
            }
        }

        if (!utility.getEtsPath().isEmpty() && !isPathExists(utility.getEtsPath())) {
            String errMsg = "--ets-path is invalid.";
            LOG.error(PackingToolErrMsg.HSP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }

        return isOutPathValid(utility, HSP_SUFFIX);
    }

    private static boolean isVerifyValidInHapAdditionMode(Utility utility) {
        if (utility.getHapPath().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode hapPath is empty.");
            return false;
        }
        String hapPath = utility.getAbsoluteHapPath();
        File hapFile = new File(hapPath);
        if (hapFile.isDirectory()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode hapPath cannot be a folder.");
            return false;
        }
        if (!(hapPath.endsWith(HAP_SUFFIX) || hapPath.endsWith(HSP_SUFFIX))) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode hapPath is invalid.");
            return false;
        }
        if (!hapFile.exists()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode hap file does not exist.");
            return false;
        }
        if (utility.getJsonPath().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode jsonPath is empty.");
            return false;
        }
        if (!utility.getJsonPath().endsWith(JSON_SUFFIX)) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode jsonPath is invalid.");
            return false;
        }
        File jsonFile = new File(utility.getJsonPath());
        if (!jsonFile.exists()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode json file does not exist.");
            return false;
        }
        if (!checkJsonIsValid(jsonFile)) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode json format is incorrect.");
            return false;
        }

        if (utility.getOutPath().isEmpty()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode outPath is empty.");
            return false;
        }
        File dir = new File(utility.getOutPath());
        if (dir.exists() && dir.isFile()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode outPath is file.");
            return false;
        }
        File absoluteHapFile = new File(utility.getAbsoluteHapPath());
        String hapFileName = absoluteHapFile.getName();
        String destPath = utility.getOutPath() + LINUX_FILE_SEPARATOR + hapFileName;
        File destFile = new File(destPath);
        if ("false".equals(utility.getForceRewrite()) && destFile.exists()) {
            LOG.error("CompressVerify::isVerifyValidInHapAdditionMode target file already exists.");
            return false;
        }
        return true;
    }

    private static boolean checkJsonIsValid(File jsonFile) {
        StringBuffer jsonData = new StringBuffer();
        try (FileReader fileReader = new FileReader(jsonFile);
             Reader reader = new InputStreamReader(new FileInputStream(jsonFile), StandardCharsets.UTF_8)) {
            int ch = 0;
            while ((ch = reader.read()) != -1) {
                jsonData.append((char) ch);
            }
        } catch (IOException e) {
            LOG.error("CompressVerify::CheckJsonIsValid failed: " + e.getMessage());
            return false;
        }
        JSONValidator validator = JSONValidator.from(jsonData.toString());
        return validator.validate();
    }

    private static boolean isSharedApp(Utility utility) {
        if (!utility.getHapPath().isEmpty()) {
            return false;
        }
        if (utility.getHspPath().isEmpty()) {
            return false;
        }
        List<String> tmpHspPathList = new ArrayList<>();
        if (compatibleProcess(utility, utility.getHspPath(), tmpHspPathList, HSP_SUFFIX)
            && verifyIsSharedApp(tmpHspPathList)) {
            utility.setIsSharedApp(true);
            return true;
        }
        return false;
    }

    private static boolean isAppService(Utility utility) {
        if (!utility.getHapPath().isEmpty()) {
            List<String> tmpHapPathList = new ArrayList<>();
            if (compatibleProcess(utility, utility.getHapPath(), tmpHapPathList, HSP_SUFFIX)
                    && verifyIsAppService(tmpHapPathList)) {
                utility.setIsAppService(true);
                return true;
            }
        }
        if (utility.getHspPath().isEmpty()) {
            return false;
        }
        List<String> tmpHspPathList = new ArrayList<>();
        if (compatibleProcess(utility, utility.getHspPath(), tmpHspPathList, HSP_SUFFIX)
                && verifyIsAppService(tmpHspPathList)) {
            utility.setIsAppService(true);
            return true;
        }
        return false;
    }
    private static boolean verifyIsAppService(List<String> modulePathList) {
        if (modulePathList.isEmpty()) {
            return false;
        }
        try {
            for (String modulePath : modulePathList) {
                HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(modulePath);
                if (!hapVerifyInfo.getBundleType().equals(BUNDLE_TYPE_APP_SERVICE)) {
                    return false;
                }
            }
        } catch (BundleException e) {
            return false;
        }
        return true;
    }
    private static boolean verifyIsSharedApp(List<String> hspPath) {
        try {
            HapVerifyInfo hapVerifyInfo = Compressor.parseStageHapVerifyInfo(hspPath.get(0));
            return hapVerifyInfo.getBundleType().equals(BUNDLE_TYPE_SHARE);
        } catch (BundleException e) {
            return false;
        }
    }

    private static boolean isBundleTypeShared(Utility utility) {
        try {
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if(optional.isPresent()) {
                return ModuleJsonUtil.parseStageBundleType(optional.get()).equals(BUNDLE_TYPE_SHARE);
            } else {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.BUNDLE_TYPE_SHARED_INVALID.toString(errMsg));
                return false;
            }
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.BUNDLE_TYPE_SHARED_INVALID.toString("BundleException: " + e.getMessage()));
            return false;
        }
    }

    private static boolean isBundleTypeAppService(Utility utility) {
        try {
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if(optional.isPresent()) {
                return ModuleJsonUtil.parseStageBundleType(optional.get()).equals(BUNDLE_TYPE_APP_SERVICE);
            } else {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.BUNDLE_TYPE_APPSERVICE_INVALID.toString(errMsg));
                return false;
            }
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.BUNDLE_TYPE_APPSERVICE_INVALID.toString("BundleException: " + e.getMessage()));
            return false;
        }
    }

    private static boolean hspHasAbilities(Utility utility) {
        try {
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if(optional.isPresent()) {
                return ModuleJsonUtil.parseModuleType(optional.get()).equals(BUNDLE_TYPE_SHARE) && !ModuleJsonUtil.parseAbilityNames(optional.get()).isEmpty();
            } else {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.HSP_HAS_ABILITIES_FAILED.toString(errMsg));
                return false;
            }
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.HSP_HAS_ABILITIES_FAILED.toString("BundleException: " + e.getMessage()));
            return false;
        }
    }

    private static boolean hspHasExtensionAbilities(Utility utility) {
        try {
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if (optional.isPresent()) {
                return ModuleJsonUtil.parseModuleType(optional.get()).equals(BUNDLE_TYPE_SHARE) &&
                        !ModuleJsonUtil.parseExtensionAbilityName(optional.get()).isEmpty();
            } else {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.HSP_HAS_EXTENSION_ABILITIES_FAILED.toString(errMsg));
            }
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.HSP_HAS_EXTENSION_ABILITIES_FAILED.toString(
                    "BundleException: " + e.getMessage()));
        }
        return false;
    }

    private static boolean hasHomeAbility(Utility utility) {
        try {
            boolean result = false;
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if(!optional.isPresent()) {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.HAS_HOME_ABILITY_INVALID.toString(errMsg));
                return false;
            }
            Map<String, Boolean> abilitiesMap = ModuleJsonUtil.parseAbilitySkillsMap(optional.get());
            if (abilitiesMap.containsValue(true)) {
                result = true;
            }
            LOG.info("CompressVerify::hasHomeAbilities result = " + result);
            return result;
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.HAS_HOME_ABILITY_INVALID.toString("BundleException: " + e.getMessage()));
            return false;
        }
    }

    /**
     * Indicates whether the "--encrypt-path" parameter is valid.
     *
     * @param utility - compress parameters
     * @return true if "--encrypt-path" param exists and the file name is encrypt.json, or the "--encrypt-path"
     *         param is empty, or has no "--encrypt-path" param
     *         false other situations
     */
    private static boolean isValidEncryptJsonFile(Utility utility) {
        if (!utility.getEncryptPath().isEmpty()) {
            File fileEncryptJson = new File(utility.getEncryptPath());
            return fileEncryptJson.isFile() && Constants.FILE_ENCRYPT_JSON.equals(fileEncryptJson.getName());
        }
        return true;
    }

    private static boolean hasHomeExtensionAbility(Utility utility) {
        try {
            boolean result = false;
            Optional<String> optional = FileUtils.getFileContent(utility.getJsonPath());
            if (!optional.isPresent()) {
                String errMsg = "Parse --json-path content failed.";
                LOG.error(PackingToolErrMsg.HAS_HOME_EXTENSION_ABILITY_INVALID.toString(errMsg));
                return false;
            }
            Map<String, Boolean> extensionAbilitiesMap = ModuleJsonUtil.parseExtensionAbilitySkillsMap(optional.get());
            if (extensionAbilitiesMap.containsValue(true)) {
                result = true;
            }
            LOG.info("CompressVerify::hasHomeExtensionAbility result = " + result);
            return result;
        } catch (BundleException e) {
            LOG.error(PackingToolErrMsg.HAS_HOME_EXTENSION_ABILITY_INVALID.toString(
                    "BundleException: " + e.getMessage()));
            return false;
        }
    }
}
