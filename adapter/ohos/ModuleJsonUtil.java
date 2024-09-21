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

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONException;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

class ModuleJsonUtil {
    private static final String APP = "app";
    private static final String BUNDLE_TYPE = "bundleType";
    private static final String ABILITIES = "abilities";
    private static final String VERSIONCODE = "versionCode";
    private static final String VERSIONNAME = "versionName";
    private static final String MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
    private static final String API_VERSION = "apiVersion";
    private static final String MIN_API_VERSION = "minAPIVersion";
    private static final String TARGET_API_VERSION = "targetAPIVersion";
    private static final String API_RELEASE_TYPE = "apiReleaseType";
    private static final String DEBUG = "debug";
    private static final String COMPATIBLE = "compatible";
    private static final String RELEASE_TYPE = "releaseType";
    private static final String TARGET = "target";
    private static final String VERSION = "version";
    private static final String CODE = "code";
    private static final String NAME = "name";
    private static final String SKILLS = "skills";
    private static final String SKILLS_ENTITIES = "entities";
    private static final String SKILLS_ACTIONS = "actions";
    private static final String ACTION_SYSTEM_HOME = "action.system.home";
    private static final String ENTITY_SYSTEM_HOME = "entity.system.home";
    private static final String MODULE = "module";
    private static final String MODULES = "modules";
    private static final String MODULE_NAME = "moduleName";
    private static final String MODULE_TYPE = "moduleType";
    private static final String DISTRO = "distro";
    private static final String PACKAGE = "package";
    private static final String PACKAGES = "packages";
    private static final String SUMMARY = "summary";
    private static final String BUNDLE_NAME = "bundleName";
    private static final String ENTRY = "entry";
    private static final char DOT = '.';
    private static final String CONFIG_JSON = "config.json";
    private static final String MODULE_JSON = "module.json";
    private static final String DEVICE_TYPE = "deviceType";
    private static final String DEVICE_TYPES = "deviceTypes";
    private static final String TYPE= "type";
    private static final String VENDOR = "vendor";
    private static final String METADATA = "metadata";
    private static final String RESOURCE = "resource";
    private static final String PROFILE = "$profile:";
    private static final String VALUE = "value";
    private static final String JSON_PERFIX = ".json";
    private static final String DISTRO_FILTER = "distroFilter";
    private static final String DISTRIBUTION_FILTER = "distributionFilter";
    private static final String DEPENDENCIES = "dependencies";
    private static final String EXTENSION_ABILITIES = "extensionAbilities";
    private static final String INSTALLATION_FREE = "installationFree";
    private static final String PATCH_JSON = "patch.json";
    private static final String PATCH_VERSION_CODE = "patchVersionCode";
    private static final String PATCH_VERSION_NAME = "patchVersionName";
    private static final String ORIGINAL_MODULE_HASH = "originalModuleHash";
    private static final String EMPTY_STRING = "";
    private static final String COMPRESS_NATIVE_LIBS = "compressNativeLibs";
    private static final String ASAN_ENABLED = "asanEnabled";
    private static final String TSAN_ENABLED = "tsanEnabled";
    private static final String GWP_ASAN_ENABLED = "GWPAsanEnabled";
    private static final String HW_ASAN_ENABLED = "hwasanEnabled";
    private static final String ATOMIC_SERVICE = "atomicService";
    private static final String SPLIT = "split";
    private static final String MAIN = "main";
    private static final String PRELOADS = "preloads";
    private static final String SHARED = "shared";
    private static final String APP_SERVICE = "appService";
    private static final String REQUEST_PERMISSIONS = "requestPermissions";
    private static final String TARGET_MODULE_NAME = "targetModuleName";
    private static final String TARGET_PRIORITY = "targetPriority";
    private static final String TARGET_BUNDLE_NAME = "targetBundleName";
    private static final String DEVICE_CONFIG = "deviceConfig";
    private static final String DEFAULT = "default";
    private static final String COMPILE_SDK_VERSION = "compileSdkVersion";
    private static final String COMPILE_SDK_TYPE = "compileSdkType";
    private static final String PROXY_DATAS = "proxyDatas";
    private static final String PROXY_DATA = "proxyData";
    private static final String PROXY_URI = "uri";
    private static final String CONTINUE_TYPE = "continueType";
    private static final String CONTINUE_BUNDLE_NAME = "continueBundleName";
    private static final String MULTI_APP_MODE = "multiAppMode";
    private static final String MULTI_APP_MODE_TYPE = "multiAppModeType";
    private static final String MULTI_APP_MODE_NUMBER = "maxCount";
    private static final String FORMS = "forms";
    private static final String DEFAULTDIMENSION = "defaultDimension";
    private static final String SUPPORTDIMENSIONS = "supportDimensions";

    private static final Log LOG = new Log(ModuleJsonUtil.class.toString());

    /**
     * get the version from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static Version parseStageVersion(String jsonString) throws BundleException {
        Version version = new Version();
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(VERSIONCODE) && appObj.containsKey(VERSIONNAME)) {
            version.versionCode = appObj.getIntValue(VERSIONCODE);
            version.versionName = appObj.getString(VERSIONNAME);
        } else {
            String errMsg = "ModuleJsonUtil:parseStageVersion json file do not contain version.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        if (appObj.containsKey(MIN_COMPATIBLE_VERSION_CODE)) {
            version.minCompatibleVersionCode = appObj.getIntValue(MIN_COMPATIBLE_VERSION_CODE);
        } else {
            version.minCompatibleVersionCode = version.versionCode;
        }
        return version;
    }

    /**
     * get the version from json file for fa module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static Version parseFaVersion(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        JSONObject versionObj = appObj.getJSONObject(VERSION);
        if (versionObj == null) {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not version.");
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not version.");
        }
        Version version = new Version();
        if (versionObj.containsKey(CODE) && versionObj.containsKey(NAME)) {
            version.versionName = versionObj.getString(NAME);
            version.versionCode = versionObj.getIntValue(CODE);
        } else {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not version name or version code.");
            throw new BundleException(
                    "ModuleJsonUtil:parseFaVersion failed : json file do not version name or version code.");
        }
        if (versionObj.containsKey(MIN_COMPATIBLE_VERSION_CODE)) {
            version.minCompatibleVersionCode = versionObj.getIntValue(MIN_COMPATIBLE_VERSION_CODE);
        } else {
            version.minCompatibleVersionCode = version.versionCode;
        }
        return version;
    }

    /**
     * get the bundleType fa module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseFaBundleType(String jsonString) throws BundleException {
        boolean installationFree = parseFAInstallationFree(jsonString);
        if (installationFree) {
            return ATOMIC_SERVICE;
        }
        return APP;
    }

    /**
     * get the apiVersion from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static ModuleApiVersion parseStageModuleApiVersion(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        ModuleApiVersion moduleApiVersion = new ModuleApiVersion();
        if (appObj.containsKey(MIN_API_VERSION)) {
            moduleApiVersion.setCompatibleApiVersion(appObj.getIntValue(MIN_API_VERSION));
        }
        if (appObj.containsKey(TARGET_API_VERSION)) {
            moduleApiVersion.setTargetApiVersion(appObj.getIntValue(TARGET_API_VERSION));
        }
        if (appObj.containsKey(API_RELEASE_TYPE)) {
            moduleApiVersion.setReleaseType(appObj.getString(API_RELEASE_TYPE));
        }
        return moduleApiVersion;
    }

    /**
     * get the apiVersion from json file for fa module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static ModuleApiVersion parseFAModuleApiVersion(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (!appObj.containsKey(API_VERSION)) {
            LOG.error("ModuleJsonUtil::parseFAAPIVersion json file do not contain apiVersion.");
            throw new BundleException("ModuleJsonUtil::parseFAAPIVersion json file do not contain apiVersion.");
        }
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        ModuleApiVersion moduleApiVersion = new ModuleApiVersion();
        if (apiVersionObj.containsKey(COMPATIBLE)) {
            moduleApiVersion.setCompatibleApiVersion(apiVersionObj.getIntValue(COMPATIBLE));
        }
        if (apiVersionObj.containsKey(RELEASE_TYPE)) {
            moduleApiVersion.setReleaseType(apiVersionObj.getString(RELEASE_TYPE));
        }
        if (apiVersionObj.containsKey(TARGET)) {
            moduleApiVersion.setTargetApiVersion(apiVersionObj.getIntValue(TARGET));
        }
        return moduleApiVersion;
    }

    /**
     * get the module name from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseStageModuleName(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        String moduleName;
        if (moduleObj.containsKey(NAME)) {
            moduleName = moduleObj.getString(NAME);
        } else {
            LOG.error("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module name.");
            throw new BundleException("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module name.");
        }
        return moduleName;
    }

    /**
     * get the moduleName from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseFaModuleName(String jsonString) throws BundleException {
        String moduleName;
        JSONObject moduleObj = getModuleObj(jsonString);
        JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
        if (distroObj == null) {
            LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain distro.");
            throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain distro.");
        }
        if (!distroObj.containsKey(MODULE_NAME)) {
            LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
            throw new BundleException(
                    "ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
        }
        moduleName = distroObj.getString(MODULE_NAME);
        return moduleName;
    }

    /**
     * get the moduleName from json file for hqf.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parsePatchModuleName(String jsonString) throws BundleException {
        String moduleName;
        JSONObject moduleObj = getModuleObj(jsonString);
        if (!moduleObj.containsKey(NAME)) {
            LOG.error("ModuleJsonUtil:parsePatchModuleName failed: json file do not contain moduleName.");
            throw new BundleException(
                    "ModuleJsonUtil:parsePatchModuleName failed: json file do not contain moduleName.");
        }
        moduleName = moduleObj.getString(NAME);
        return moduleName;
    }

    /**
     * get the package from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseFaPackageStr(String jsonString) throws BundleException {
        String packageStr = "";
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(PACKAGE)) {
            packageStr = moduleObj.getString(PACKAGE);
        } else {
            LOG.error("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain package.");
            throw new BundleException("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain package.");
        }
        return packageStr;
    }

    /**
     * get the bundleName from json file.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseBundleName(String jsonString) throws BundleException {
        JSONObject appObject = getAppObj(jsonString);
        String bundleName = "";
        if (appObject.containsKey(BUNDLE_NAME)) {
            bundleName = appObject.getString(BUNDLE_NAME);
        } else {
            LOG.error("ModuleJsonUtil::parseStageBundleName json object do not contain bundleNames.");
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain bundleNames.");
        }
        return bundleName;
    }

    /**
     * get the vendor from json file.
     *
     * @param jsonString uncompress json object
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String parseVendor(String jsonString) throws BundleException {
        JSONObject appObject = getAppObj(jsonString);
        String vendor = "";
        if (appObject.containsKey(VENDOR)) {
            vendor = appObject.getString(VENDOR);
        }
        return vendor;
    }

    /**
     * merge two pack.info file into one pack.info file.
     *
     * @param finalPackInfo is the final packInfo
     * @param srcPackInfo is the packInfo to be merged
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String mergeTwoPackInfo(String finalPackInfo, String srcPackInfo) throws BundleException {
        String desPackInfo = "";
        JSONObject finalPackObj;
        try {
            finalPackObj = JSON.parseObject(finalPackInfo);
            JSONObject srcPackObj = JSON.parseObject(srcPackInfo);
            if (!verifyPackInfo(finalPackObj, srcPackObj)) {
                LOG.error("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed.");
                throw new BundleException("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed.");
            }
            desPackInfo = mergePackInfoObj(finalPackObj, srcPackObj);
        } catch (BundleException | JSONException e) {
            LOG.error("ModuleJsonUtil:mergeTwoPackInfo merge pack.info failed: " + e.getMessage());
            throw new BundleException("ModuleJsonUtil:mergeTwoPackInfo merge pack.info failed.");
        }
        return desPackInfo;
    }

    /**
     * verify pack.info file.
     *
     * @param finalPackObj is the final pack.info object
     * @param srcPackObj is the src pack.info object
     * @return the result
     */
    public static boolean verifyPackInfo(JSONObject finalPackObj, JSONObject srcPackObj) throws BundleException {
        if (finalPackObj == null || srcPackObj == null) {
            LOG.error("ModuleJsonUtil:verifyPackInfo fail to read pack.info.");
            return false;
        }
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            LOG.error("ModuleJsonUtil:verifyPackInfo pack.info do not contain summary.");
            return false;
        }
        // check app info
        JSONObject finalAppObj = finalSummaryObj.getJSONObject(APP);
        JSONObject srcAppObj = srcSummaryObj.getJSONObject(APP);
        if (finalAppObj == null || srcAppObj == null) {
            LOG.error("ModuleJsonUtil:verifyPackInfo pack.info do not contain app.");
            return false;
        }
        if (!verifyAppInPackInfo(finalAppObj, srcAppObj)) {
            LOG.error("ModuleJsonUtil:verifyPackInfo verify app failed.");
            return false;
        }

        return true;
    }

    /**
     * verify app in pack.info file.
     *
     * @param finalAppObj is the final pack.info app object
     * @param srcAppObj is the src pack.info app object
     * @return the result
     */
    public static boolean verifyAppInPackInfo(JSONObject finalAppObj, JSONObject srcAppObj) {
        if (finalAppObj == null || srcAppObj == null) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo input null json object.");
            return false;
        }
        // check bundleName
        String finalBundleName = finalAppObj.getString(BUNDLE_NAME);
        String srcBundleName = srcAppObj.getString(BUNDLE_NAME);
        if (!finalBundleName.equals(srcBundleName)) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo bundleName is different.");
            return false;
        }
        // check bundleType
        if (!checkBundleTypeInPackInfo(finalAppObj, srcAppObj)) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo bundleType is different.");
            return false;
        }
        // check version
        JSONObject finalVersionObj = finalAppObj.getJSONObject(VERSION);
        JSONObject srcVersionObj = srcAppObj.getJSONObject(VERSION);
        if (finalVersionObj == null || srcVersionObj == null) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo version object is empty.");
            return false;
        }
        int finalVersionCode = finalVersionObj.getIntValue(CODE);
        int srcVersionCode = srcVersionObj.getIntValue(CODE);
        if (finalVersionCode != srcVersionCode) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo versionCode is different.");
            return false;
        }
        return true;
    }

    /**
     * verify bundleType in pack.info file.
     *
     * @param finalAppObj is the final pack.info app objects
     * @param srcAppObj is the src pack.info app objects
     * @return the result
     */
    public static boolean checkBundleTypeInPackInfo(JSONObject finalAppObj, JSONObject srcAppObj) {
        if (finalAppObj.isEmpty() || srcAppObj.isEmpty()) {
            LOG.error("ModuleJsonUtil:checkBundleTypeInPackInfo pack.info has empty module.");
            return false;
        }
        String finalBundleType = "app";
        String srcBundleType = "app";
        if (finalAppObj.containsKey(BUNDLE_TYPE)) {
            finalBundleType = getJsonString(finalAppObj, BUNDLE_TYPE);
        }
        if (srcAppObj.containsKey(BUNDLE_TYPE)) {
            srcBundleType = getJsonString(srcAppObj, BUNDLE_TYPE);
        }
        if (!finalBundleType.equals(srcBundleType)) {
            LOG.error("bundleType in pack.info is not same.");
            return false;
        }
        return true;
    }

    /**
     * verify module in pack.info file.
     *
     * @param finalModuleObs is the final pack.info module objects
     * @param srcModuleObs is the src pack.info module objects
     * @return the result
     */
    public static boolean verifyModuleInPackInfo(JSONArray finalModuleObs, JSONArray srcModuleObs)
            throws BundleException {
        if (finalModuleObs.isEmpty() || srcModuleObs.isEmpty()) {
            LOG.error("ModuleJsonUtil:verifyModuleInPackInfo pack.info has empty module.");
            throw new BundleException("ModuleJsonUtil:verifyModuleInPackInfo pack.info has empty module.");
        }
        List<String> moduleNames = new ArrayList<>();
        for (int i = 0; i < finalModuleObs.size(); ++i) {
            JSONObject finalModuleObj = finalModuleObs.getJSONObject(i);
            String moduleName = parseDistroModuleName(finalModuleObj);
            if (moduleNames.contains(moduleName)) {
                LOG.error("ModuleJsonUtil:verifyModuleInPackInfo duplicated moduleName.");
                return false;
            } else {
                moduleNames.add(moduleName);
            }
        }
        for (int i = 0; i < srcModuleObs.size(); ++i) {
            JSONObject srcModuleObj = srcModuleObs.getJSONObject(i);
            String moduleName = parseDistroModuleName(srcModuleObj);
            if (moduleNames.contains(moduleName)) {
                LOG.error("ModuleJsonUtil:verifyModuleInPackInfo duplicated moduleName.");
                return false;
            } else {
                moduleNames.add(moduleName);
            }
        }
        return true;
    }



    /**
     * parse forms name in pack.info file.
     *
     * @param jsonString the pack.info json string
     * @param formNameList the forms name in pack.info
     * @param fullFormNameList the forms name and moduleName merged result in pack.info
     */
    public static void parsePackInfoFormsName(String jsonString, List<String> formNameList, List<String> fullFormNameList)
            throws BundleException {
        JSONObject jsonObject = JSONObject.parseObject(jsonString);
        if (jsonObject == null || !jsonObject.containsKey(SUMMARY)) {
            LOG.error("ModuleJsonUtil::parsePackInfoFormsName error: summary is null.");
            throw new BundleException("Parse pack info forms name failed, summary is null.");
        }

        JSONObject summaryJson = jsonObject.getJSONObject(SUMMARY);
        if (summaryJson == null || !summaryJson.containsKey("modules")) {
            LOG.error("ModuleJsonUtil::parsePackInfoFormsName error: summary.modules is null.");
            return;
        }

        JSONArray moduleJsonList = summaryJson.getJSONArray("modules");
        for (int i = 0; i < moduleJsonList.size(); i++) {
            JSONObject moduleJson = moduleJsonList.getJSONObject(i);
            if (moduleJson == null || !moduleJson.containsKey(DISTRO)) {
                LOG.error("ModuleJsonUtil::parsePackInfoFormsName error: summary.modules.distro is null.");
                continue;
            }

            JSONObject distroObj = moduleJson.getJSONObject(DISTRO);
            if (distroObj == null || !distroObj.containsKey(MODULE_NAME)) {
                LOG.error("ModuleJsonUtil::parsePackInfoFormsName error: summary.modules.distro.moduleName is null.");
                continue;
            }

            String moduleName = distroObj.getString(MODULE_NAME);
            parsePackInfoExtensionAbility(moduleName, moduleJson, formNameList, fullFormNameList);
        }
    }

    /**
     * verify package name in pack.info file.
     *
     * @param finalPackageObs is the final pack.info objects
     * @param srcPackageObs is the src pack.info objects
     * @return the result
     */
    public static boolean verifyPackageName(JSONArray finalPackageObs, JSONArray srcPackageObs) {
        if (finalPackageObs == null || finalPackageObs.isEmpty() || srcPackageObs == null || srcPackageObs.isEmpty()) {
            LOG.error("ModuleJsonUtil:verifyPackageName pack.info has empty packages.");
            return false;
        }
        List<String> packageNames = new ArrayList<>();
        for (int i = 0; i < finalPackageObs.size(); ++i) {
            JSONObject packageObj = finalPackageObs.getJSONObject(i);
            String packageName = packageObj.getString(NAME);
            if (packageNames.contains(packageName)) {
                LOG.error("ModuleJsonUtil:verifyPackageName duplicated package name.");
                return false;
            } else {
                packageNames.add(packageName);
            }
        }
        for (int i = 0; i < srcPackageObs.size(); ++i) {
            JSONObject packageObj = srcPackageObs.getJSONObject(i);
            String packageName = packageObj.getString(NAME);
            if (packageNames.contains(packageName)) {
                LOG.error("ModuleJsonUtil:verifyPackageName duplicated package name.");
                return false;
            } else {
                packageNames.add(packageName);
            }
        }
        return true;
    }

    /**
     * parse moduleName in pack.info file.
     *
     * @param moduleObj is the final pack.info objects
     * @return the result
     */
    public static String parseDistroModuleName(JSONObject moduleObj) throws BundleException {
        String moduleName = "";
        try {
            if (moduleObj == null) {
                LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain module.");
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain module.");
            }
            JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
            if (distroObj == null) {
                LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain distro.");
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain distro.");
            }
            if (!distroObj.containsKey(MODULE_NAME)) {
                LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed:" +
                        "json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULE_NAME);
        } catch (BundleException e) {
            LOG.error("ModuleJsonUtil:parseFaModuleName failed.");
            throw new BundleException("ModuleJsonUtil:parseFaModuleName failed.");
        }
        return moduleName;
    }

    /**
     * merge pack.info file.
     *
     * @param finalPackinfoObj is the final pack.info objects
     * @param srcPackinfoObj is the final pack.info objects
     * @return the result
     */
    public static String mergePackInfoObj(JSONObject finalPackinfoObj, JSONObject srcPackinfoObj) throws BundleException {
        if (finalPackinfoObj == null || srcPackinfoObj == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input an invalid json object.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject finalSummaryObj = finalPackinfoObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackinfoObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty summary.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge modules
        JSONArray finalModuleObs = finalSummaryObj.getJSONArray(MODULES);
        JSONArray srcModuleObs = srcSummaryObj.getJSONArray(MODULES);
        if (finalModuleObs == null || srcModuleObs == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty module.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        finalModuleObs.addAll(srcModuleObs);
        // merge packages
        JSONArray finalPackageObs = finalPackinfoObj.getJSONArray(PACKAGES);
        JSONArray srcPackageObs = srcPackinfoObj.getJSONArray(PACKAGES);
        if (finalPackageObs == null || srcPackageObs == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty packages.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        finalPackageObs.addAll(srcPackageObs);
        return finalPackinfoObj.toString();
    }

    /**
     * merge two pack.info file into one pack.info file by packagePair.
     *
     * @param finalPackInfo is the final packInfo
     * @param srcPackInfo is the packInfo to be merged
     * @param packagePair is the selected packageName-moduleName pair map
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static String mergeTwoPackInfoByPackagePair(String finalPackInfo, String srcPackInfo,
                                                       HashMap<String, String> packagePair) throws BundleException {
        JSONObject finalPackObj;
        JSONObject srcPackObj;
        try {
            finalPackObj = JSON.parseObject(finalPackInfo);
            srcPackObj = JSON.parseObject(srcPackInfo);
        } catch (JSONException exception) {
            String errMsg = "parse JSONObject failed: " + exception.getMessage();
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // verify app in pack.info
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject finalAppObj = finalSummaryObj.getJSONObject(APP);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        JSONObject srcAppObj = srcSummaryObj.getJSONObject(APP);
        if (!verifyAppInPackInfo(finalAppObj, srcAppObj)) {
            String errMsg = "verify pack.info failed, different version, bundleType or bundleName.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        for (HashMap.Entry<String, String> entry : packagePair.entrySet()) {
            String packageName = entry.getKey().substring(0, entry.getKey().lastIndexOf(DOT));
            mergeTwoPackInfoObjByPackagePair(finalPackObj, srcPackObj, packageName, entry.getValue());
        }
        return finalPackObj.toString();
    }

    /**
     * merge two pack.info file into one pack.info file by packagePair.
     *
     * @param finalPackObj is the final packInfo
     * @param srcPackObj is the packInfo to be merged
     * @param packageName is the selected packageName
     * @param moduleName is the selected moduleName
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static void mergeTwoPackInfoObjByPackagePair(JSONObject finalPackObj, JSONObject srcPackObj,
                                                        String packageName, String moduleName) throws BundleException {
        if (finalPackObj == null || srcPackObj == null) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info is not json object.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge module
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info do not contain summary.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONArray finalModules = finalSummaryObj.getJSONArray(MODULES);
        JSONArray srcModules = srcSummaryObj.getJSONArray(MODULES);
        if (finalModules == null || srcModules == null) {
            LOG.error("ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair input json file has empty module.");
            throw new
                BundleException("ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair input json file has empty module.");
        }
        boolean findModule = false;
        for (int index = 0; index < srcModules.size(); ++index) {
            JSONObject moduleObj = srcModules.getJSONObject(index);
            JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
            if (distroObj.getString(MODULE_NAME).equals(moduleName)) {
                finalModules.add(moduleObj);
                findModule = true;
                break;
            }
        }
        if (!findModule) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair" +
                    " input json do not contain " + moduleName + ".";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge package
        JSONArray finalPackages = finalPackObj.getJSONArray(PACKAGES);
        JSONArray srcPackages = srcPackObj.getJSONArray(PACKAGES);
        if (finalPackages == null || srcPackages == null) {
            String errMsg =
                    "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info do not contain packages.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        boolean findPackage = false;
        for (int index = 0; index < srcPackages.size(); ++index) {
            JSONObject srcPackageObj = srcPackages.getJSONObject(index);
            if (srcPackageObj.getString(NAME).equals(packageName)) {
                finalPackages.add(srcPackageObj);
                findPackage = true;
                break;
            }
        }
        if (!findPackage) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair input json do not contain "
                    + packageName + ".";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
    }

    /**
     * parse FA hap is entry hap, if it is, return device type.
     *
     * @param hapPath is the path of hap.
     *
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static List<String> parseFaEntry(String hapPath) throws BundleException {
        String configJson = FileUtils.getJsonInZips(new File(hapPath), CONFIG_JSON);
        JSONObject faObj;
        try {
            faObj = JSONObject.parseObject(configJson);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = faObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain module.";
            LOG.error(errMSg);
            throw new BundleException(errMSg);
        }
        List<String> deviceTypes = new ArrayList<>();
        JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
        if (distroObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain distro.";
            LOG.error(errMSg);
            throw new BundleException(errMSg);
        }
        String moduleType = distroObj.getString(MODULE_TYPE);
        if (ENTRY.equals(moduleType)) {
            deviceTypes = getDeviceTypeFromFAModule(moduleObj);
        }
        return deviceTypes;
    }

    /**
     * parse stage hap is entry hap, if it is, record device type.
     *
     * @param hapPath is the path of hap.
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static List<String> parseStageEntry(String hapPath) throws BundleException {
        String moduleJson = FileUtils.getJsonInZips(new File(hapPath), MODULE_JSON);
        JSONObject stageObj;
        try {
            stageObj = JSONObject.parseObject(moduleJson);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = stageObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain module.";
            LOG.error(errMSg);
            throw new BundleException(errMSg);
        }
        List<String> deviceTypes = new ArrayList<>();
        String type = moduleObj.getString(TYPE);
        if (type != null && type.equals(ENTRY)) {
            deviceTypes = getDeviceTypesFromStageModule(moduleObj);
        }

        return deviceTypes;
    }

    /**
     * get deviceType from fa module.
     *
     * @param moduleObj is the object of module.
     * @return true is for entry hap  and false is other kind of hap
     */
    public static List<String> getDeviceTypeFromFAModule(JSONObject moduleObj) {
        List<String> deviceTypes = new ArrayList<>();
        if (moduleObj == null) {
            return deviceTypes;
        }
        deviceTypes = JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPE), String.class);
        return deviceTypes;
    }

    /**
     * get deviceType from stage module.
     *
     * @param moduleObj is the object of module.
     * @return true is for entry hap  and false is other kind of hap
     */
    public static List<String> getDeviceTypesFromStageModule(JSONObject moduleObj) {
        List<String> deviceTypes = new ArrayList<>();
        if (moduleObj == null) {
            return deviceTypes;
        }
        deviceTypes = JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPES), String.class);
        return deviceTypes;
    }

    /**
     * parse stage hapVerifyInfo.
     *
     * @param hapVerifyInfo is the parse result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static void parseStageHapVerifyInfo(HapVerifyInfo hapVerifyInfo) throws BundleException {
        if (hapVerifyInfo.getProfileStr().isEmpty()) {
            throw new BundleException("ModuleJsonUtil::parseStageHapVerifyInfo failed, module.json is empty.");
        }
        String bundleName = parseBundleName(hapVerifyInfo.getProfileStr());
        hapVerifyInfo.setBundleName(bundleName);
        hapVerifyInfo.setVendor(parseVendor(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setVersion(parseStageVersion(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setApiVersion(parseStageModuleApiVersion(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setModuleName(parseStageModuleName(hapVerifyInfo.getProfileStr()));
        List<ModuleMetadataInfo> moduleMetadataInfos =
                parseModuleAllMetadata(hapVerifyInfo.getProfileStr(), hapVerifyInfo.getResourceMap());
        hapVerifyInfo.setDistroFilter(parseStageDistroFilter(moduleMetadataInfos));
        hapVerifyInfo.setDeviceType(parseDeviceType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setAbilityNames(parseAbilityNames(hapVerifyInfo.getProfileStr()));
        List<String> extensionAbilityNames = parseExtensionAbilityName(hapVerifyInfo.getProfileStr());
        hapVerifyInfo.addAbilityNames(extensionAbilityNames);
        hapVerifyInfo.setModuleType(parseModuleType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDependencyItemList(parseDependencies(hapVerifyInfo.getProfileStr(), bundleName));
        hapVerifyInfo.setInstallationFree(parseStageInstallation(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setBundleType(parseStageBundleType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setPreloadItems(parseAtomicServicePreloads(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setTargetBundleName(parseTargetBundleName(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setTargetPriority(parseTargetPriority(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setTargetModuleName(parseTargetModuleName(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setTargetModulePriority(parseTargetModulePriority(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDebug(getDebug(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setCompileSdkType(getCompileSdkType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setCompileSdkVersion(getCompileSdkVersion(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setProxyDataUris(parseProxyDataUri(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setContinueTypeMap(parseAbilityContinueTypeMap(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setMultiAppMode(parseMultiAppMode(hapVerifyInfo.getProfileStr()));
    }

    /**
     * parse FA hapVerifyInfo.
     *
     * @param hapVerifyInfo is the parse result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static void parseFAHapVerifyInfo(HapVerifyInfo hapVerifyInfo) throws BundleException {
        if (hapVerifyInfo.getProfileStr().isEmpty()) {
            LOG.error("ModuleJsonUtil::parseFAHapVerifyInfo failed, config.json is empty.");
            throw new BundleException("ModuleJsonUtil::parseFAHapVerifyInfo failed, config.json is empty.");
        }
        String bundleName = parseBundleName(hapVerifyInfo.getProfileStr());
        hapVerifyInfo.setBundleName(bundleName);
        hapVerifyInfo.setBundleType(parseFaBundleType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setVendor(parseVendor(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setVersion(parseFaVersion(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setApiVersion(parseFAModuleApiVersion(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setModuleName(parseFaModuleName(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDistroFilter(parseFADistroFilter(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDeviceType(parseDeviceType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setAbilityNames(parseAbilityNames(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setModuleType(parseFAIsEntry(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setPackageName(parseFaPackageStr(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDependencyItemList(parseDependencies(hapVerifyInfo.getProfileStr(), bundleName));
        hapVerifyInfo.setInstallationFree(parseFAInstallationFree(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDebug(getFADebug(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setCompileSdkType(getFACompileSdkType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setCompileSdkVersion(getFACompileSdkVersion(hapVerifyInfo.getProfileStr()));
    }

    private static MultiAppMode parseMultiAppMode(String jsonString) throws BundleException {
        MultiAppMode multiAppMode = new MultiAppMode();
        JSONObject appObj = getAppObj(jsonString);
        JSONObject modeObj = appObj.getJSONObject(MULTI_APP_MODE);
        if (modeObj != null) {
            String type = modeObj.getString(MULTI_APP_MODE_TYPE);
            Integer number = modeObj.getInteger(MULTI_APP_MODE_NUMBER);
            multiAppMode.setMultiAppModeType(type != null ? type : "");
            multiAppMode.setMaxCount(number != null ? number : 0);
        }
        return multiAppMode;
    }

    /**
     * parse stage distroFilter.
     *
     * @param moduleMetadataInfos all metadata of module
     * @return DistroFilter is the result of parsed distroFilter
     */
    public static DistroFilter parseStageDistroFilter(List<ModuleMetadataInfo> moduleMetadataInfos) {
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            String resource = moduleMetadataInfo.resource;
            if (resource.isEmpty()) {
                continue;
            }
            JSONObject distroFilter = JSONObject.parseObject(resource);
            if (distroFilter.containsKey(DISTRIBUTION_FILTER)) {
                return JSONObject.parseObject(getJsonString(distroFilter, DISTRIBUTION_FILTER), DistroFilter.class);
            }
            if (distroFilter.containsKey(DISTRO_FILTER)) {
                return JSONObject.parseObject(getJsonString(distroFilter, DISTRO_FILTER), DistroFilter.class);
            }
        }
        return new DistroFilter();
    }

    /**
     * parse stage parseModuleAllMetadata.
     *
     * @param jsonString is the string of module.json
     * @param profileJson is the <fileName,fileString> of profile
     * @return DistroFilter is the result of parsed distroFilter
     */
    public static List<ModuleMetadataInfo> parseModuleAllMetadata(
            String jsonString, HashMap<String, String> profileJson) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        List<ModuleMetadataInfo> moduleMetadataInfos = new ArrayList<>();
        if (moduleObj.containsKey(METADATA)) {
            JSONArray metadatas = moduleObj.getJSONArray(METADATA);
            for (int i = 0; i < metadatas.size(); ++i) {
                JSONObject metadata = metadatas.getJSONObject(i);
                moduleMetadataInfos.add(parseModuleMetadata(metadata, profileJson));
            }
        }
        return moduleMetadataInfos;
    }

    /**
     * parse metadata info
     *
     * @param jsonObject Json hap json Object
     * @param profileJson is the <fileName,fileString> of profile
     * @return the ModuleMetadataInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static ModuleMetadataInfo parseModuleMetadata(JSONObject jsonObject, HashMap<String, String> profileJson)
            throws BundleException {
        if (jsonObject == null) {
            throw new BundleException("ModuleJsonUtil::parseModuleMetadata failed, jsonObject is null.");
        }
        ModuleMetadataInfo moduleMetadataInfo = new ModuleMetadataInfo();
        if (jsonObject.containsKey(NAME)) {
            moduleMetadataInfo.name = getJsonString(jsonObject, NAME);
        }
        if (jsonObject.containsKey(VALUE)) {
            moduleMetadataInfo.value = getJsonString(jsonObject, VALUE);
        }
        if (jsonObject.containsKey(RESOURCE)) {
            moduleMetadataInfo.resource = getJsonString(jsonObject, RESOURCE);
            String fileName = moduleMetadataInfo.resource;
            fileName = fileName.replace(PROFILE, "");
            fileName = fileName + JSON_PERFIX;
            moduleMetadataInfo.resource = profileJson.get(fileName);
        }
        return moduleMetadataInfo;
    }

    /**
     * parse metadata info
     *
     * @param jsonString Json string of config.json
     * @return the ModuleMetadataInfo result
     */
    public static DistroFilter parseFADistroFilter(String jsonString) throws BundleException {
        DistroFilter distroFilter = new DistroFilter();
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
            if (jsonObj.containsKey(MODULE)) {
                JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
                if (moduleObj.containsKey(DISTRO_FILTER)) {
                    distroFilter = JSONObject.parseObject(getJsonString(moduleObj,
                            DISTRO_FILTER), DistroFilter.class);
                }
            }
        } catch (JSONException exception) {
            String errMsg = "parse JSONObject failed: " + exception.getMessage();
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        return distroFilter;
    }

    /**
     * get deviceType from json file.
     *
     * @param jsonString is the json String of module.json or config.json
     */
    public static List<String> parseDeviceType(String jsonString) throws BundleException {
        List<String> deviceType = new ArrayList<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(DEVICE_TYPE)) {
            return JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPE), String.class);
        } else if (moduleObj.containsKey(DEVICE_TYPES)) {
            return JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPES), String.class);
        } else {
            return deviceType;
        }
    }

    /**
     * get ability names from json file.
     *
     * @param jsonString is the json String of module.json or config.json
     * @return ability names
     */
    public static List<String> parseAbilityNames(String jsonString) throws BundleException {
        List<String> abilityNames = new ArrayList<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(ABILITIES)) {
            JSONArray abilityObjs = moduleObj.getJSONArray(ABILITIES);
            for (int i = 0; i < abilityObjs.size(); ++i) {
                JSONObject abilityObj = abilityObjs.getJSONObject(i);
                if (abilityObj.containsKey(NAME)) {
                    abilityNames.add(getJsonString(abilityObj, NAME));
                }
            }
        }

        return abilityNames;
    }

    /**
     * get ability continueType map from json file.
     *
     * @param jsonString is the json String of module.json
     * @return continueType map
     */
    public static Map<String, List<String>> parseAbilityContinueTypeMap(String jsonString)
            throws BundleException {
        Map<String, List<String>> continueTypeMap = new HashMap<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(ABILITIES)) {
            JSONArray abilityObjs = moduleObj.getJSONArray(ABILITIES);
            for (int i = 0; i < abilityObjs.size(); ++i) {
                JSONObject abilityObj = abilityObjs.getJSONObject(i);
                String abilityName = getJsonString(abilityObj, NAME);
                if (abilityObj.containsKey(CONTINUE_TYPE)) {
                    JSONArray typeArray = abilityObj.getJSONArray(CONTINUE_TYPE);
                    continueTypeMap.put(abilityName, typeArray.toJavaList(String.class));
                } else {
                    continueTypeMap.put(abilityName, new ArrayList<>());
                }
            }
        }
        return continueTypeMap;
    }

    /**
     * parse stage ExtensionAbility names
     *
     * @param jsonString is the json String of module.json
     * @return extensionAbilityNames
     */
    public static List<String> parseExtensionAbilityName(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        List<String> extensionAbilityNames = new ArrayList<>();
        if (moduleObj.containsKey(EXTENSION_ABILITIES)) {
            JSONArray extensionAbilityObjs = moduleObj.getJSONArray(EXTENSION_ABILITIES);
            for (int i = 0; i < extensionAbilityObjs.size(); ++i) {
                JSONObject extensionAbilityObj = extensionAbilityObjs.getJSONObject(i);
                if (extensionAbilityObj.containsKey(NAME)) {
                    extensionAbilityNames.add(getJsonString(extensionAbilityObj, NAME));
                }
            }
        }
        return extensionAbilityNames;
    }

    /**
     * get ability skills map from json file.
     *
     * @param jsonString is the json String of module.json
     * @return skillsMap key is ability,value indicates whether this door is a home ability
     */
    public static Map<String, Boolean> parseAbilitySkillsMap(String jsonString)
            throws BundleException {
        Map<String, Boolean> skillsMap = new HashMap<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        JSONArray abilityObs = moduleObj.getJSONArray(ABILITIES);
        if(abilityObs == null) {
            return skillsMap;
        }
        for (int i = 0; i < abilityObs.size(); ++i) {
            JSONObject abilityObj = abilityObs.getJSONObject(i);
            String abilityName = getJsonString(abilityObj, NAME);
            skillsMap.put(abilityName, false);
            if(!abilityObj.containsKey(SKILLS)) {
                break;
            }
            JSONArray skillArray = abilityObj.getJSONArray(SKILLS);
            for (int j = 0; j < skillArray.size(); ++j) {
                JSONObject skillObj = skillArray.getJSONObject(j);
                String entities = getJsonString(skillObj, SKILLS_ENTITIES);
                String actions = getJsonString(skillObj, SKILLS_ACTIONS);
                if (entities.contains(ENTITY_SYSTEM_HOME) && actions.contains(ACTION_SYSTEM_HOME)) {
                    skillsMap.put(abilityName, true);
                    break;
                }
            }
        }
        return skillsMap;
    }

    /**
     * parse stage module type.
     *
     * @param jsonString is the json String of module.json or config.json
     * @return is entry
     */
    public static String parseModuleType(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(TYPE)) {
            return getJsonString(moduleObj, TYPE);
        }
        return EMPTY_STRING;
    }

    /**
     * parse FA module type.
     *
     * @param jsonString is the json String of module.json or config.json
     * @return is entry
     */
    public static String parseFAIsEntry(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(DISTRO)) {
            JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
            if (distroObj.containsKey(MODULE_TYPE)) {
                return getJsonString(distroObj, MODULE_TYPE);
            }
        }
        return EMPTY_STRING;
    }

    static List<DependencyItem> parseDependencies(String jsonString, String bundleName) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);

        List<DependencyItem> dependencyItemList = new ArrayList<>();
        if (!moduleObj.containsKey(DEPENDENCIES)) {
            return dependencyItemList;
        }
        JSONArray dependencyObjList = moduleObj.getJSONArray(DEPENDENCIES);
        for (int i = 0; i < dependencyObjList.size(); ++i) {
            JSONObject object = dependencyObjList.getJSONObject(i);
            DependencyItem item = new DependencyItem();
            if (object.containsKey(BUNDLE_NAME)) {
                item.setBundleName(object.getString(BUNDLE_NAME));
            } else {
                item.setBundleName(bundleName);
            }
            if (object.containsKey(MODULE_NAME)) {
                item.setModuleName(object.getString(MODULE_NAME));
            }
            dependencyItemList.add(item);
        }
        return dependencyItemList;
    }

    static boolean parseStageInstallation(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);

        if (moduleObj.containsKey(INSTALLATION_FREE)) {
            return moduleObj.getBoolean(INSTALLATION_FREE);
        }
        return false;
    }

    static String parseStageBundleType(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        String moduleName = parseStageModuleName(jsonString);
        if (!moduleObj.containsKey(TYPE)) {
            String errMsg = "parse failed, input module.json is invalid, " +
                    "module.json has no type in module: " + moduleName;
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        String type = moduleObj.getString(TYPE);
        boolean installationFree = getJsonBooleanValue(moduleObj, INSTALLATION_FREE, false);
        JSONObject appObj = getAppObj(jsonString);
        if (!appObj.containsKey(BUNDLE_TYPE)) {
            if (installationFree) {
                String errMessage = "The app.json5 file configuration does not match the installationFree:" +
                        " true settings. Add the bundleType field to the app.json5 file and set it atomicService.";
                LOG.error(errMessage);
                throw new BundleException(errMessage);
            }
            return APP;
        } else {
            String bundleType = getJsonString(appObj, BUNDLE_TYPE);
            if (bundleType.equals(APP)) {
                if (installationFree) {
                    String errMsg = "installationFree must be false in module(" +
                            moduleName + ") when bundleType is app.";
                    LOG.error(errMsg);
                    throw new BundleException(errMsg);
                }
                return APP;
            } else if (bundleType.equals(ATOMIC_SERVICE)) {
                if (!installationFree) {
                    String errMsg = "installationFree must be true in module(" +
                            moduleName + ") when bundleType is atomicService.";
                    LOG.error(errMsg);
                    throw new BundleException(errMsg);
                }
                return ATOMIC_SERVICE;
            } else if (SHARED.equals(bundleType)) {
                if (!SHARED.equals(type)) {
                    String errMsg = "type must be shared in module(" + moduleName + ") when bundleType is shared.";
                    LOG.error(errMsg);
                    throw new BundleException(errMsg);
                }
                return SHARED;
            } else if (APP_SERVICE.equals(bundleType)) {
                return APP_SERVICE;
            } else {
                LOG.error("bundleType is invalid in app.json.");
                throw new BundleException("bundleType is invalid in app.json.");
            }
        }
    }

    static List<PreloadItem> parseAtomicServicePreloads(String jsonString) throws BundleException {
        List<PreloadItem> preloadItems = new ArrayList<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        JSONObject atomicServiceObj = null;
        if (!moduleObj.containsKey(ATOMIC_SERVICE)) {
            return preloadItems;
        }
        atomicServiceObj = moduleObj.getJSONObject(ATOMIC_SERVICE);
        if (!atomicServiceObj.containsKey(PRELOADS)) {
            return preloadItems;
        }
        JSONArray preloadObjs = atomicServiceObj.getJSONArray(PRELOADS);
        for (int i = 0; i < preloadObjs.size(); ++i) {
            PreloadItem preloadItem = new PreloadItem();
            JSONObject itemObj = preloadObjs.getJSONObject(i);
            if (itemObj.containsKey(MODULE_NAME)) {
                preloadItem.setModuleName(getJsonString(itemObj, MODULE_NAME));
            }
            preloadItems.add(preloadItem);
        }
        return preloadItems;
    }

    static List<String> parseProxyDataUri(String jsonString) throws BundleException {
        List<String> proxyDataUris = new ArrayList<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        if (!moduleObj.containsKey(PROXY_DATAS) && !moduleObj.containsKey(PROXY_DATA)) {
            return proxyDataUris;
        } else if (moduleObj.containsKey(PROXY_DATA)) {
            JSONArray proxyData = moduleObj.getJSONArray(PROXY_DATA);
            for (int i = 0; i < proxyData.size(); ++i) {
                JSONObject itemObj = proxyData.getJSONObject(i);
                if (!itemObj.containsKey(PROXY_URI)) {
                    LOG.error("parse JOSNObject failed in parseProxyDataUri.");
                    throw new BundleException("parse JOSNObject failed in parseProxyDataUri.");
                }
                String uri = itemObj.getString(PROXY_URI);
                proxyDataUris.add(uri);
            }
        } else {
            JSONArray proxyDatas = moduleObj.getJSONArray(PROXY_DATAS);
            for (int i = 0; i < proxyDatas.size(); ++i) {
                JSONObject itemObj = proxyDatas.getJSONObject(i);
                if (!itemObj.containsKey(PROXY_URI)) {
                    LOG.error("parse JOSNObject failed in parseProxyDataUri.");
                    throw new BundleException("parse JOSNObject failed in parseProxyDataUri.");
                }
                String uri = itemObj.getString(PROXY_URI);
                proxyDataUris.add(uri);
            }
        }
        return proxyDataUris;
    }

    static JSONObject getAppObj(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain app.");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain app.");
        }
        return appObj;
    }

    static JSONObject getModuleObj(String jsonString) throws BundleException {
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        if (jsonObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation jsonObj is null.");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation jsonObj is null.");
        }
        JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain module.");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain module.");
        }
        return moduleObj;
    }

    static String parseTargetBundleName(String jsonString) throws BundleException {
        JSONObject appObject = getAppObj(jsonString);
        String targetBundleName = "";
        if (appObject.containsKey(TARGET_BUNDLE_NAME)) {
            targetBundleName = appObject.getString(TARGET_BUNDLE_NAME);
        }
        return targetBundleName;
    }

    static int parseTargetPriority(String jsonString) throws BundleException {
        JSONObject appObject = getAppObj(jsonString);
        int targetPriority = 0;
        if (appObject.containsKey(TARGET_PRIORITY)) {
            targetPriority = appObject.getIntValue(TARGET_PRIORITY);
        }
        return targetPriority;
    }

    static String parseTargetModuleName(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        String targetModuleName = "";
        if (moduleObj.containsKey(TARGET_MODULE_NAME)) {
            targetModuleName = moduleObj.getString(TARGET_MODULE_NAME);
        }
        return targetModuleName;
    }

    static int parseTargetModulePriority(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        int targetModulePriority = 0;
        if (moduleObj.containsKey(TARGET_PRIORITY)) {
            targetModulePriority = moduleObj.getIntValue(TARGET_PRIORITY);
        }
        return targetModulePriority;
    }

    static boolean parseFAInstallationFree(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
        if (distroObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain distro.");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain distro.");
        }
        if (distroObj.containsKey(INSTALLATION_FREE)) {
            return distroObj.getBoolean(INSTALLATION_FREE);
        }
        return false;
    }

    /**
     * get hqfVerifyINfo from hqf file
     *
     * @param hqfPath is the file path of hqf file
     * @return HQFVerifyInfo
     */
    static HQFInfo parseHQFInfo(String hqfPath) throws BundleException {
        File hqfFile = new File(hqfPath);
        String patchJson = FileUtils.getJsonInZips(hqfFile, PATCH_JSON);
        return parsePatch(patchJson);
    }

    /**
     * parse patch.json form json string.
     *
     * @param jsonString is the file path of hqf file
     * @return HQFVerifyInfo
     */
    static HQFInfo parsePatch(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        HQFInfo hqfVerifyInfo = new HQFInfo();
        if (appObj.containsKey(BUNDLE_NAME)) {
            hqfVerifyInfo.setBundleName(appObj.getString(BUNDLE_NAME));
        }
        if (appObj.containsKey(VERSIONCODE)) {
            hqfVerifyInfo.setVersionCode(appObj.getIntValue(VERSIONCODE));
        }
        if (appObj.containsKey(VERSIONNAME)) {
            hqfVerifyInfo.setVersionName(appObj.getString(VERSIONNAME));
        }
        if (appObj.containsKey(PATCH_VERSION_CODE)) {
            hqfVerifyInfo.setPatchVersionCode(appObj.getIntValue(PATCH_VERSION_CODE));
        }
        if (appObj.containsKey(PATCH_VERSION_NAME)) {
            hqfVerifyInfo.setPatchVersionName(appObj.getString(PATCH_VERSION_NAME));
        }

        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(NAME)) {
            hqfVerifyInfo.setModuleName(moduleObj.getString(NAME));
        }
        if (moduleObj.containsKey(TYPE)) {
            hqfVerifyInfo.setType(moduleObj.getString(TYPE));
        }
        if (moduleObj.containsKey(DEVICE_TYPES)) {
            hqfVerifyInfo.setDeviceTypes(JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPES), String.class));
        }
        if (moduleObj.containsKey(ORIGINAL_MODULE_HASH)) {
            hqfVerifyInfo.setOriginalModuleHash(moduleObj.getString(ORIGINAL_MODULE_HASH));
        }
        return hqfVerifyInfo;
    }

    /**
     * determine whether it is a native compression libs.
     *
     * @param jsonString is the file path of hqf file
     * @return the result
     */
    public static boolean stageIsCompressNativeLibs(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(COMPRESS_NATIVE_LIBS)) {
            return moduleObj.getBoolean(COMPRESS_NATIVE_LIBS);
        }

        return false;
    }

    /**
     * get asanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of asanEnabled
     */
    public static boolean getStageAsanEnabled(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(ASAN_ENABLED)) {
            return appObj.getBoolean(ASAN_ENABLED);
        }
        return false;
    }

    /**
     * get tsanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of tsanEnabled
     */
    public static boolean getStageTsanEnabled(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(TSAN_ENABLED)) {
            return appObj.getBoolean(TSAN_ENABLED);
        }
        return false;
    }

    /**
     * get gwpAsanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of gwpAsanEnabled
     */
    public static boolean getStageGwpAsanEnabled(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(GWP_ASAN_ENABLED)) {
            return appObj.getBoolean(GWP_ASAN_ENABLED);
        }
        return false;
    }

    /**
     * get hwasanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of hwasanEnabled
     */
    public static boolean getStageHwasanEnabled(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(HW_ASAN_ENABLED)) {
            return appObj.getBoolean(HW_ASAN_ENABLED);
        }
        return false;
    }


    /**
     * get ability continueBundleName map from json file.
     *
     * @param jsonString is the json String of module.json
     * @return continueBundleName map
     */
    public static Map<String, List<String>> getAbilityContinueBundleNameMap(String jsonString)
            throws BundleException {
        Map<String, List<String>> continueBundleName = new HashMap<>();
        JSONObject moduleObj = getModuleObj(jsonString);
        if (moduleObj.containsKey(ABILITIES)) {
            JSONArray abilityObjs = moduleObj.getJSONArray(ABILITIES);
            for (int i = 0; i < abilityObjs.size(); ++i) {
                JSONObject abilityObj = abilityObjs.getJSONObject(i);
                String abilityName = getJsonString(abilityObj, NAME);
                if (abilityObj.containsKey(CONTINUE_BUNDLE_NAME)) {
                    JSONArray typeArray = abilityObj.getJSONArray(CONTINUE_BUNDLE_NAME);
                    continueBundleName.put(abilityName, typeArray.toJavaList(String.class));
                } else {
                    continueBundleName.put(abilityName, new ArrayList<>());
                }
            }
        }
        return continueBundleName;
    }

    /**
     * get apiReleaseType in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getStageApiReleaseType(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        return getJsonString(appObj, API_RELEASE_TYPE);
    }

    /**
     * get debug in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static boolean getDebug(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);

        return getJsonBooleanValue(appObj, DEBUG, false);
    }

    /**
     * get debug in config.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static boolean getFADebug(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("parse JOSNObject failed in getStageApiReleaseType.");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType.");
        }
        JSONObject deviceConfigObj = jsonObject.getJSONObject(DEVICE_CONFIG);
        if (deviceConfigObj == null) {
            return false;
        }
        JSONObject defaultObj = deviceConfigObj.getJSONObject(DEFAULT);
        if (defaultObj == null) {
            return false;
        }

        return getJsonBooleanValue(defaultObj, DEBUG, false);
    }

    /**
     * get compileSdkVersion in config.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getFACompileSdkVersion(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        String compileSdkVersion = "";
        if (apiVersionObj.containsKey(COMPILE_SDK_VERSION)) {
            compileSdkVersion = apiVersionObj.getString(COMPILE_SDK_VERSION);
        }
        return compileSdkVersion;
    }

    /**
     * get compileSdkType in config.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getFACompileSdkType(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (!appObj.containsKey(API_VERSION)) {
            throw new BundleException("ModuleJsonUtil::parseFAAPIVersion json file do not contain apiVersion.");
        }
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        String compileSdkType = "";
        if (apiVersionObj.containsKey(COMPILE_SDK_TYPE)) {
            compileSdkType = apiVersionObj.getString(COMPILE_SDK_TYPE);
        }
        return compileSdkType;
    }

    /**
     * get compileSdkVersion in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getCompileSdkVersion(String jsonString) throws BundleException {
        String compileSdkVersion = "";
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(COMPILE_SDK_VERSION)) {
            compileSdkVersion = appObj.getString(COMPILE_SDK_VERSION);
        } else {
            LOG.warning("getCompileSdkType failed: json file do not contain module compileSdkVersion.");
        }
        return compileSdkVersion;
    }

    /**
     * get compileSdkType in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getCompileSdkType(String jsonString) throws BundleException {
        String compileSdkType = "";
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(COMPILE_SDK_TYPE)) {
            compileSdkType = appObj.getString(COMPILE_SDK_TYPE);
        } else {
            LOG.warning("getCompileSdkType failed: json file do not contain module compileSdkType.");
        }
        return compileSdkType;
    }

    /**
     * get targetModuleName in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getStageTargetModuleName(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        return getJsonString(moduleObj, TARGET_MODULE_NAME);
    }

    /**
     * get targetBundleName in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getStageTargetBundleName(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        return getJsonString(appObj, TARGET_BUNDLE_NAME);
    }

    /**
     * is existed requestPermission in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static boolean isExistedStageRequestPermissions(String jsonString) throws BundleException {
        return isExistedProperty(jsonString, MODULE, REQUEST_PERMISSIONS);
    }

    /**
     * is existed targetPriority in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static boolean isExistedStageModuleTargetPriority(String jsonString) throws BundleException {
        return isExistedProperty(jsonString, MODULE, TARGET_PRIORITY);
    }

    /**
     * is existed targetPriority in app.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static boolean isExistedStageAppTargetPriority(String jsonString) throws BundleException {
        return isExistedProperty(jsonString, APP, TARGET_PRIORITY);
    }

    private static boolean isExistedProperty(String jsonString, String fatherProperty,
                                             String childProperty) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("parse JOSNObject failed in isExistedProperty.");
            throw new BundleException("parse JOSNObject failed in isExistedProperty.");
        }
        JSONObject appObj = jsonObject.getJSONObject(fatherProperty);
        if (appObj == null) {
            LOG.error("parse failed, input module.json is invalid, module.json has no " + fatherProperty + ".");
            throw new BundleException("parse failed, input module.json is invalid, module.json has no " +
                    fatherProperty + ".");
        }
        return appObj.containsKey(childProperty);
    }

    /**
     * get asanEnabled in config.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of asanEnabled
     */
    public static boolean getFAAsanEnabled(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        if (appObj.containsKey(ASAN_ENABLED)) {
            return appObj.getBoolean(ASAN_ENABLED);
        }
        return false;
    }

    /**
     * get releaseType in config.json
     *
     * @param jsonString is the file content of config.json
     * @return the result
     */
    public static String getFAReleaseType(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        if (apiVersionObj == null) {
            return "";
        }
        return getJsonString(apiVersionObj, RELEASE_TYPE);
    }

    /**
     * check module atomic service is valid
     *
     * @param jsonString is the file content of config.json
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static boolean isModuleAtomicServiceValid(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        if (!moduleObj.containsKey(ATOMIC_SERVICE)) {
            return true;
        }
        JSONObject appObj = getAppObj(jsonString);

        if (moduleObj.containsKey(ATOMIC_SERVICE) && (!appObj.containsKey(BUNDLE_TYPE) ||
                !getJsonString(appObj, BUNDLE_TYPE).equals(ATOMIC_SERVICE))) {
            LOG.error("module can not config atomicService when bundleType is not atomicService.");
            return false;
        }
        return true;
    }

    /**
     * check entry module must contain at least one ability.
     *
     * @param jsonString Indicates the jsonString.
     * @return Returns true if jsonString is valid.
     */
    public static boolean checkEntryInAtomicService(String jsonString) throws BundleException {
        if (!parseStageBundleType(jsonString).equals(ATOMIC_SERVICE)) {
            return true;
        }
        if (parseModuleType(jsonString).equals(ENTRY) && parseAbilityNames(jsonString).isEmpty()) {
            LOG.error("entry module must contain at least one ability.");
            return false;
        }
        return true;
    }

    /**
     * check module atomic installation free is valid
     *
     * @param jsonString is the file content of config.json
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static boolean checkAtomicServiceInstallationFree(String jsonString) throws BundleException {
        JSONObject moduleObj = getModuleObj(jsonString);
        JSONObject appObj = getAppObj(jsonString);

        boolean installationFree = getJsonBooleanValue(moduleObj, INSTALLATION_FREE, false);
        if (!appObj.containsKey(BUNDLE_TYPE)) {
            if (installationFree) {
                String errMessage = "The app.json5 file configuration does not match the installationFree:" +
                        " true settings. Add the bundleType field to the app.json5 file and set it atomicService.";
                LOG.error(errMessage);
                return false;
            }
            return true;
        }
        String bundleType = getJsonString(appObj, BUNDLE_TYPE);
        if (bundleType.equals(APP)) {
            if (installationFree) {
                LOG.error("installationFree must be false when bundleType is app.");
                return false;
            }
        } else if (bundleType.equals(ATOMIC_SERVICE)) {
            if (!installationFree) {
                LOG.error("installationFree must be true when bundleType is atomicService.");
                return false;
            }
        } else if (SHARED.equals(bundleType)) {
            if (installationFree) {
                LOG.error("installationFree must be false when bundleType is shared.");
                return false;
            }
        } else if (APP_SERVICE.equals(bundleType)) {
            if (installationFree) {
                LOG.error("installationFree must be false when bundleType is appService.");
                return false;
            }
        } else {
            LOG.error("bundleType is invalid in app.json.");
            return false;
        }
        return true;
    }

    /**
     * get the String from JSONObject by the key.
     *
     * @param jsonObject uncompress json object
     * @param key value key
     * @return the result
     */
    private static String getJsonString(JSONObject jsonObject, String key) {
        String value = "";
        if (jsonObject != null && jsonObject.containsKey(key) && jsonObject.get(key) != null) {
            value = jsonObject.get(key).toString();
        }
        return value;
    }

    private static boolean getJsonBooleanValue(JSONObject jsonObject, String key, boolean defaultValue) {
        boolean value = defaultValue;
        if (jsonObject != null && jsonObject.containsKey(key)) {
            value = jsonObject.getBooleanValue(key);
        }
        return value;
    }

    private static int getCount(String str, char targetChar) {
        int count = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == targetChar) {
                count++;
            }
        }
        return count;
    }

    private static void parsePackInfoExtensionAbility(String moduleName, JSONObject moduleJson, List<String> formNameList,
                                              List<String> fullFormNameList) throws BundleException {
        if (!moduleJson.containsKey(EXTENSION_ABILITIES)) {
            LOG.warning("ModuleJsonUtil::parsePackInfoExtensionAbility error: summary.modules.extensionAbilities is null.");
            return;
        }

        JSONArray extensionAbilityJsonList = moduleJson.getJSONArray(EXTENSION_ABILITIES);
        for (int j = 0; j < extensionAbilityJsonList.size(); j++) {
            JSONObject extensionAbilityJson = extensionAbilityJsonList.getJSONObject(j);
            if (extensionAbilityJson == null || !extensionAbilityJson.containsKey(FORMS)) {
                LOG.warning("ModuleJsonUtil::parsePackInfoExtensionAbility error: " +
                        "summary.modules.extensionAbilities.forms is null.");
                continue;
            }

            parsePackInfoForms(moduleName, extensionAbilityJson, formNameList, fullFormNameList);
        }
    }

    private static void parsePackInfoForms(String moduleName, JSONObject extensionAbilityJson, List<String> formNameList,
                                                 List<String> fullFormNameList) throws BundleException {
        JSONArray formJsonList = extensionAbilityJson.getJSONArray(FORMS);
        for (int i = 0; i < formJsonList.size(); i++) {
            JSONObject formObj = formJsonList.getJSONObject(i);
            if (formObj == null || !formObj.containsKey(NAME)) {
                LOG.warning("ModuleJsonUtil::parsePackInfoForms error: " +
                        "summary.modules.extensionAbilities.forms.name is null.");
                continue;
            }

            String name = formObj.getString(NAME);
            formNameList.add(name);
            if (!formObj.containsKey(DEFAULTDIMENSION)) {
                LOG.error("ModuleJsonUtil::parsePackInfoForms exception: " +
                        "summary.modules.extensionAbilities.forms.defaultDimension is null.");
                throw new BundleException("Parse pack info defaultDimension failed, defaultDimension is null.");
            }

            String defaultDimension = formObj.getString(DEFAULTDIMENSION);
            if (getCount(defaultDimension, '*') != 1) {
                LOG.error("ModuleJsonUtil::parsePackInfoForms exception: " +
                        "summary.modules.extensionAbilities.forms.defaultDimension is not only 1.");
                throw new BundleException("Parse pack info defaultDimension failed, defaultDimension is not only 1.");
            }
            if (!formObj.containsKey(SUPPORTDIMENSIONS)) {
                LOG.error("ModuleJsonUtil::parsePackInfoForms exception: " +
                        "summary.modules.extensionAbilities.forms.supportDimensions is null.");
                throw new BundleException("Parse pack info supportDimensions failed, supportDimensions is null.");
            }

            List<String> dimensionList = JSONObject.parseArray(getJsonString(formObj, SUPPORTDIMENSIONS), String.class);
            for(String dimension : dimensionList) {
                String nameWithDimension = moduleName + "/" + name + "-" + dimension.replace("*", "x");
                fullFormNameList.add(nameWithDimension);
            }
        }
    }
}
