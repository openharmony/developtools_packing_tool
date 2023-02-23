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
import java.util.List;
import java.util.HashMap;
import java.util.Optional;

class ModuleJsonUtil {
    private static final String  VERSION_CODE = "\"versionCode\"";
    private static final String VERSION_NAME = "\"versionName\"";
    private static final String APP = "app";
    private static final String ABILITIES = "abilities";
    private static final String VERSIONCODE = "versionCode";
    private static final String VERSIONNAME = "versionName";
    private static final String MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
    private static final String API_VERSION = "apiVersion";
    private static final String MIN_API_VERSION = "minAPIVersion";
    private static final String TARGET_API_VERSION = "targetAPIVersion";
    private static final String API_RELEASE_TYPE = "apiReleaseType";
    private static final String COMPATIBLE = "compatible";
    private static final String RELEASE_TYPE = "releaseType";
    private static final String TARGET = "target";
    private static final String VERSION = "version";
    private static final String CODE = "code";
    private static final String NAME = "name";
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
    private static final String ATOMIC_SERVICE = "atomicService";
    private static final String SPLIT = "split";
    private static final String MAIN = "main";
    private static final String PRELOADS = "preloads";
    private static final String SHARED = "shared";
    private static final String COMPATIBLE_POLICY = "compatiblePolicy";
    private static final Log LOG = new Log(ModuleJsonUtil.class.toString());

    /**
     * get the version from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static Version parseStageVersion(String jsonString) throws BundleException {
        Version version = new Version();
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        if (jsonObject.containsKey(APP)) {
            JSONObject appObj = jsonObject.getJSONObject(APP);
            if (appObj.containsKey(VERSIONCODE) && appObj.containsKey(VERSIONNAME)) {
                version.versionCode = appObj.getIntValue(VERSIONCODE);
                version.versionName = appObj.getString(VERSIONNAME);
            } else {
                String errMsg = "ModuleJsonUtil:parseStageVersion json file do not contain version!";
                LOG.error(errMsg);
                throw new BundleException(errMsg);
            }
            if (appObj.containsKey(MIN_COMPATIBLE_VERSION_CODE)) {
                version.minCompatibleVersionCode = appObj.getIntValue(MIN_COMPATIBLE_VERSION_CODE);
            } else {
                version.minCompatibleVersionCode = version.versionCode;
            }
        } else {
            String errMsg = "ModuleJsonUtil:parseStageVersion json file do not contain app!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }

        return version;
    }

    /**
     * get the version from json file for fa module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static Version parseFaVersion(String jsonString) throws BundleException {
        Version version = new Version();
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not contain app.");
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not contain app.");
        }
        JSONObject versionObj = appObj.getJSONObject(VERSION);
        if (versionObj == null) {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not version.");
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not version.");
        }
        if (versionObj.containsKey(CODE) && versionObj.containsKey(NAME)) {
            version.versionName = versionObj.getString(NAME);
            version.versionCode = versionObj.getIntValue(CODE);
        } else {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not version name or version code.");
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not version name or version code.");
        }
        if (versionObj.containsKey(MIN_COMPATIBLE_VERSION_CODE)) {
            version.minCompatibleVersionCode = versionObj.getIntValue(MIN_COMPATIBLE_VERSION_CODE);
        } else {
            version.minCompatibleVersionCode = version.versionCode;
        }
        return version;
    }

    /**
     * get the apiVersion from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static ModuleApiVersion parseStageModuleApiVersion(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("ModuleJsonUtil:parseFaVersion failed : json file do not contain app.");
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not contain app.");
        }
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
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static ModuleApiVersion parseFAModuleApiVersion(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            throw new BundleException("ModuleJsonUtil::parseFAAPIVersion json file do not contain app.");
        }
        if (!appObj.containsKey(API_VERSION)) {
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
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseStageModuleName(String jsonString) throws BundleException {
        String moduleName = "";
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
            if (moduleObj == null) {
                LOG.error("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module.");
                throw new BundleException("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module.");
            }
            if (moduleObj.containsKey(NAME)) {
                moduleName = moduleObj.getString(NAME);
            } else {
                LOG.error("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module name.");
                throw new BundleException("ModuleJsonUtil:parseStageModuleName failed: json file do not contain module name.");
            }
        } catch (BundleException e) {
            LOG.error("ModuleJsonUtil:parseStageModuleName failed!");
            throw new BundleException("ModuleJsonUtil:parseStageModuleName failed!");
        }
        return moduleName;
    }

    /**
     * get the moduleName from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseFaModuleName(String jsonString) throws BundleException {
        String moduleName = "";
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
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
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULE_NAME);
        } catch (BundleException | JSONException e) {
            LOG.error("ModuleJsonUtil:parseFaModuleName failed!");
            throw new BundleException("ModuleJsonUtil:parseFaModuleName failed!");
        }
        return moduleName;
    }

    /**
     * get the package from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseFaPackageStr(String jsonString) throws BundleException {
        String packageStr = "";
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
            JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
            if (moduleObj == null) {
                LOG.error("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain module.");
                throw new BundleException("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain module.");
            }
            if (moduleObj.containsKey(PACKAGE)) {
                packageStr = moduleObj.getString(PACKAGE);
            } else {
                LOG.error("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain package.");
                throw new BundleException("ModuleJsonUtil:parseFaPackageStr failed: json file do not contain package.");
            }
        } catch (BundleException | JSONException e) {
            LOG.error("ModuleJsonUtil:parseFaPackageStr failed!");
            throw new BundleException("ModuleJsonUtil:parseFaPackageStr failed!");
        }
        return packageStr;
    }

    /**
     * get the bundleName from json file.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseBundleName(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObject = jsonObject.getJSONObject(APP);
        if (appObject == null) {
            LOG.error("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
        }
        String bundleName = "";
        if (appObject.containsKey(BUNDLE_NAME)) {
            bundleName = appObject.getString(BUNDLE_NAME);
        } else {
            LOG.error("ModuleJsonUtil::parseStageBundleName json object do not contain bundleNames!");
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain bundleNames!");
        }
        return bundleName;
    }

    /**
     * get the vendor from json file.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String parseVendor(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObject = jsonObject.getJSONObject(APP);
        if (appObject == null) {
            LOG.error("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
        }
        String vendor = "";
        if (appObject.containsKey(VENDOR)) {
            vendor = appObject.getString(VENDOR);
        } else {
            return vendor;
        }
        return vendor;
    }

    /**
     * merge two pack.info file into one pack.info file.
     *
     * @param finalPackInfo is the final packInfo
     * @param srcPackInfo is the packInfo to be merged
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String mergeTwoPackInfo(String finalPackInfo, String srcPackInfo) throws BundleException {
        String desPackInfo = "";
        JSONObject finalPackObj;
        try {
            finalPackObj = JSON.parseObject(finalPackInfo);
            JSONObject srcPackObj = JSON.parseObject(srcPackInfo);
            if (!verifyPackInfo(finalPackObj, srcPackObj)) {
                LOG.error("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed!");
                throw new BundleException("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed!");
            }
            desPackInfo = mergePackInfoObj(finalPackObj, srcPackObj);
        } catch (BundleException | JSONException e) {
            LOG.error("ModuleJsonUtil:mergeTwoPackInfo merge pack.info failed!");
            throw new BundleException("ModuleJsonUtil:mergeTwoPackInfo merge pack.info failed!");
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
            LOG.error("ModuleJsonUtil:verifyPackInfo fail to read pack.info!");
            return false;
        }
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            LOG.error("ModuleJsonUtil:verifyPackInfo pack.info do not contain summary!");
            return false;
        }
        // check app info
        JSONObject finalAppObj = finalSummaryObj.getJSONObject(APP);
        JSONObject srcAppObj = srcSummaryObj.getJSONObject(APP);
        if (finalAppObj == null || srcAppObj == null) {
            LOG.error("ModuleJsonUtil:verifyPackInfo pack.info do not contain app!");
            return false;
        }
        if (!verifyAppInPackInfo(finalAppObj, srcAppObj)) {
            LOG.error("ModuleJsonUtil:verifyPackInfo verify app failed!");
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
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo input null json object!");
            return false;
        }
        // check bundleName
        String finalBundleName = finalAppObj.getString(BUNDLE_NAME);
        String srcBundleName = srcAppObj.getString(BUNDLE_NAME);
        if (!finalBundleName.equals(srcBundleName)) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo bundleName is different!");
            return false;
        }
        // check version
        JSONObject finalVersionObj = finalAppObj.getJSONObject(VERSION);
        JSONObject srcVersionObj = srcAppObj.getJSONObject(VERSION);
        if (finalVersionObj == null || srcVersionObj == null) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo version object is empty!");
            return false;
        }
        String finalVersionName = finalVersionObj.getString(NAME);
        String srcVersionName = srcVersionObj.getString(NAME);
        if (!finalVersionName.equals(srcVersionName)) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo versionName is different!");
            return false;
        }
        int finalVersionCode = finalVersionObj.getIntValue(CODE);
        int srcVersionCode = srcVersionObj.getIntValue(CODE);
        if (finalVersionCode != srcVersionCode) {
            LOG.error("ModuleJsonUtil:verifyAppInPackInfo versionCode is different!");
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
            LOG.error("ModuleJsonUtil:verifyModuleInPackInfo pack.info has empty module!");
            throw new BundleException("ModuleJsonUtil:verifyModuleInPackInfo pack.info has empty module!");
        }
        List<String> moduleNames = new ArrayList<>();
        for (int i = 0; i < finalModuleObs.size(); ++i) {
            JSONObject finalModuleObj = finalModuleObs.getJSONObject(i);
            String moduleName = parseDistroModuleName(finalModuleObj);
            if (moduleNames.contains(moduleName)) {
                LOG.error("ModuleJsonUtil:verifyModuleInPackInfo duplicated moduleName!");
                return false;
            } else {
                moduleNames.add(moduleName);
            }
        }
        for (int i = 0; i < srcModuleObs.size(); ++i) {
            JSONObject srcModuleObj = srcModuleObs.getJSONObject(i);
            String moduleName = parseDistroModuleName(srcModuleObj);
            if (moduleNames.contains(moduleName)) {
                LOG.error("ModuleJsonUtil:verifyModuleInPackInfo duplicated moduleName!");
                return false;
            } else {
                moduleNames.add(moduleName);
            }
        }
        return true;
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
            LOG.error("ModuleJsonUtil:verifyPackageName pack.info has empty packages!");
            return false;
        }
        List<String> packageNames = new ArrayList<>();
        for (int i = 0; i < finalPackageObs.size(); ++i) {
            JSONObject packageObj = finalPackageObs.getJSONObject(i);
            String packageName = packageObj.getString(NAME);
            if (packageNames.contains(packageName)) {
                LOG.error("ModuleJsonUtil:verifyPackageName duplicated package name!");
                return false;
            } else {
                packageNames.add(packageName);
            }
        }
        for (int i = 0; i < srcPackageObs.size(); ++i) {
            JSONObject packageObj = srcPackageObs.getJSONObject(i);
            String packageName = packageObj.getString(NAME);
            if (packageNames.contains(packageName)) {
                LOG.error("ModuleJsonUtil:verifyPackageName duplicated package name!");
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
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULE_NAME);
        } catch (BundleException e) {
            LOG.error("ModuleJsonUtil:parseFaModuleName failed!");
            throw new BundleException("ModuleJsonUtil:parseFaModuleName failed!");
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
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input an invalid json object!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject finalSummaryObj = finalPackinfoObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackinfoObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty summary!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge modules
        JSONArray finalModuleObs = finalSummaryObj.getJSONArray(MODULES);
        JSONArray srcModuleObs = srcSummaryObj.getJSONArray(MODULES);
        if (finalModuleObs == null || srcModuleObs == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty module";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        finalModuleObs.addAll(srcModuleObs);
        // merge packages
        JSONArray finalPackageObs = finalPackinfoObj.getJSONArray(PACKAGES);
        JSONArray srcPackageObs = srcPackinfoObj.getJSONArray(PACKAGES);
        if (finalPackageObs == null || srcPackageObs == null) {
            String errMsg = "ModuleJsonUtil:mergePackInfoObj input json file has empty packages!";
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
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static String mergeTwoPackInfoByPackagePair(String finalPackInfo, String srcPackInfo,
                                                       HashMap<String, String> packagePair) throws BundleException {
        JSONObject finalPackObj;
        JSONObject srcPackObj;
        try {
            finalPackObj = JSON.parseObject(finalPackInfo);
            srcPackObj = JSON.parseObject(srcPackInfo);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // verify app in pack.info
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject finalAppObj = finalSummaryObj.getJSONObject(APP);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        JSONObject srcAppObj = srcSummaryObj.getJSONObject(APP);
        if (!verifyAppInPackInfo(finalAppObj, srcAppObj)) {
            String errMsg = "verify pack.info failed, different version or bundleName!";
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
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info is not json object!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge module
        JSONObject finalSummaryObj = finalPackObj.getJSONObject(SUMMARY);
        JSONObject srcSummaryObj = srcPackObj.getJSONObject(SUMMARY);
        if (finalSummaryObj == null || srcSummaryObj == null) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info do not contain summary!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONArray finalModules = finalSummaryObj.getJSONArray(MODULES);
        JSONArray srcModules = srcSummaryObj.getJSONArray(MODULES);
        if (finalModules == null || srcModules == null) {
            LOG.error("ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair input json file has empty module!");
            throw new BundleException("ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair input json file has empty module!");
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
                    " input json do not contain " + moduleName + "!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        // merge package
        JSONArray finalPackages = finalPackObj.getJSONArray(PACKAGES);
        JSONArray srcPackages = srcPackObj.getJSONArray(PACKAGES);
        if (finalPackages == null || srcPackages == null) {
            String errMsg = "ModuleJsonUtil:mergeTwoPackInfoObjByPackagePair failed: pack.info do not contain packages!";
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
                    + packageName + "!";
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
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = faObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain module!";
            LOG.error(errMSg);
            throw new BundleException(errMSg);
        }
        List<String> deviceTypes = new ArrayList<>();
        JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
        if (distroObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain distro!";
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
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = stageObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain module!";
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
            throw new BundleException("ModuleJsonUtil::parseStageHapVerifyInfo failed, module.json is empty!");
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
        hapVerifyInfo.setModuleType(parseStageIsEntry(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setDependencyItemList(parseDependencies(hapVerifyInfo.getProfileStr(), bundleName));
        hapVerifyInfo.setInstallationFree(parseStageInstallation(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setSplit(parseStageAtomicServiceSpilt(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setMain(parseStageAtomicServiceMain(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setAppType(parseStageAppType(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setPreloadItems(parseAtomicServicePreloads(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setSharedHsp(parseSharedApp(hapVerifyInfo.getProfileStr()));
        hapVerifyInfo.setCompatiblePolicy(parseCompatiblePolicy(hapVerifyInfo.getProfileStr()));
    }

    /**
     * parse FA hapVerifyInfo.
     *
     * @param hapVerifyInfo is the parse result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    public static void parseFAHapVerifyInfo(HapVerifyInfo hapVerifyInfo) throws BundleException {
        if (hapVerifyInfo.getProfileStr().isEmpty()) {
            LOG.error("ModuleJsonUtil::parseStageHapVerifyInfo failed, config.json is empty!");
            throw new BundleException("ModuleJsonUtil::parseStageHapVerifyInfo failed, config.json is empty!");
        }
        String bundleName = parseBundleName(hapVerifyInfo.getProfileStr());
        hapVerifyInfo.setBundleName(parseBundleName(hapVerifyInfo.getProfileStr()));
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
    }

    /**
     * parse stage distroFilter.
     *
     * @param moduleMetadataInfos all metadata of module
     * @return DistroFilter is the result of parsed distroFilter
     */
    public static DistroFilter parseStageDistroFilter(
            List<ModuleMetadataInfo> moduleMetadataInfos) throws BundleException {
        DistroFilter distroFilter = new DistroFilter();
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            if (moduleMetadataInfo.resource.isEmpty()) {
                continue;
            }
            try {
                JSONObject distroFilterObj = JSON.parseObject(moduleMetadataInfo.resource);
                if (distroFilterObj.containsKey(DISTRO_FILTER)) {
                    distroFilter = JSONObject.parseObject(getJsonString(distroFilterObj,
                            DISTRO_FILTER), DistroFilter.class);
                }
            } catch (JSONException exception) {
                String errMsg = "parse JSONobject failed";
                LOG.error(errMsg);
                throw new BundleException(errMsg);
            }
        }
        return distroFilter;
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
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("ModuleJsonUtil::parseModuleAllMetadata failed, module is null!");
            throw new BundleException("ModuleJsonUtil::parseModuleAllMetadata failed, module is null!");
        }
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
            throw new BundleException("ModuleJsonUtil::parseModuleMetadata failed, jsonObject is null");
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
            String errMsg = "parse JSONobject failed";
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
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        List<String> deviceType = new ArrayList<>();
        if (jsonObj.containsKey(MODULE)) {
            JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
            if (moduleObj.containsKey(DEVICE_TYPE)) {
                return JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPE), String.class);
            } else if (moduleObj.containsKey(DEVICE_TYPES)) {
                return JSONObject.parseArray(getJsonString(moduleObj, DEVICE_TYPES), String.class);
            } else {
                return deviceType;
            }
        }
        return deviceType;
    }

    /**
     * get ability names from json file.
     *
     * @param jsonString is the json String of module.json or config.json
     * @return ability names
     */
    public static List<String> parseAbilityNames(String jsonString) throws BundleException {
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        List<String> abilityNames = new ArrayList<>();
        JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
        if (moduleObj != null && moduleObj.containsKey(ABILITIES)) {
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
     * parse stage ExtensionAbility names
     *
     * @param jsonString is the json String of module.json
     * @return extensionAbilityNames
     */
    public static List<String> parseExtensionAbilityName(String jsonString) throws BundleException {
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("ModuleJsonUtil::parseExtensionAbilityName failed, module is null!");
            throw new BundleException("ModuleJsonUtil::parseExtensionAbilityName failed, module is null!");
        }
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
     * parse stage module type.
     *
     * @param jsonString is the json String of module.json or config.json
     * @return is entry
     */
    public static String parseStageIsEntry(String jsonString) throws BundleException {
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONObject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        if (jsonObj.containsKey(MODULE)) {
            JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
            if (moduleObj.containsKey(TYPE)) {
                return getJsonString(moduleObj, TYPE);
            }
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
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        if (jsonObj.containsKey(MODULE)) {
            JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
            if (moduleObj.containsKey(DISTRO)) {
                JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
                if (distroObj.containsKey(MODULE_TYPE)) {
                    return getJsonString(distroObj, MODULE_TYPE);
                }
            }
        }
        return EMPTY_STRING;
    }

    static List<DependencyItem> parseDependencies(String jsonString, String bundleName) throws BundleException {
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse json object failed!";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("parseDependencies failed: lack of module object!");
            throw new BundleException("parseDependencies failed: lack of module object!");
        }
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
        JSONObject jsonObj;
        try {
            jsonObj = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain module!");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain module!");
        }
        if (moduleObj.containsKey(INSTALLATION_FREE)) {
            return moduleObj.getBoolean(INSTALLATION_FREE);
        }
        return false;
    }

    static boolean parseStageAtomicServiceSpilt(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        if (!appObj.containsKey(ATOMIC_SERVICE)) {
            return true;
        }
        JSONObject atomicService = appObj.getJSONObject(ATOMIC_SERVICE);
        return getJsonBooleanValue(atomicService, SPLIT, true);
    }

    static String parseStageAtomicServiceMain(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        if (!appObj.containsKey(ATOMIC_SERVICE)) {
            return "";
        }
        JSONObject atomicServiceObj = appObj.getJSONObject(ATOMIC_SERVICE);
        return getJsonString(atomicServiceObj, MAIN);
    }

    static String parseStageAppType(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        if (appObj.containsKey(ATOMIC_SERVICE)) {
            return ATOMIC_SERVICE;
        }
        return APP;
    }

    static List<PreloadItem> parseAtomicServicePreloads(String jsonString) throws BundleException {
        List<PreloadItem> preloadItems = new ArrayList<>();
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
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

    static JSONObject getAppObj(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain app!");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain app!");
        }
        return appObj;
    }

    static boolean parseSharedApp(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        JSONObject sharedObj = appObj.getJSONObject(SHARED);
        return sharedObj != null;
    }

    static String parseCompatiblePolicy(String jsonString) throws BundleException {
        JSONObject appObj = getAppObj(jsonString);
        JSONObject sharedObj = appObj.getJSONObject(SHARED);
        if (sharedObj != null && sharedObj.containsKey(COMPATIBLE_POLICY)) {
            return sharedObj.getString(COMPATIBLE_POLICY);
        }
        return EMPTY_STRING;
    }

    static boolean parseFAInstallationFree(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain module!");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain module!");
        }
        JSONObject distroObj = moduleObj.getJSONObject(DISTRO);
        if (distroObj == null) {
            LOG.error("ModuleJsonUtil::parseStageInstallation json do not contain distro!");
            throw new BundleException("ModuleJsonUtil::parseStageInstallation json do not contain distro!");
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
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parsePatch failed, input patch.json is invalid, patch.json has no app!");
            throw new BundleException("Error: parsePatch failed, input patch.json is invalid!");
        }
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
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("Error: parse failed, input patch.json is invalid, patch.json has no module!");
            throw new BundleException("Error: parse failed, input patch.json is invalid, patch.json has no module!");
        }
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
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            String errMsg = "Error: parse JSONobject failed";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no module!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no module!");
        }
        if (moduleObj.containsKey(COMPRESS_NATIVE_LIBS)) {
            return moduleObj.getBoolean(COMPRESS_NATIVE_LIBS);
        }

        return true;
    }

    /**
     * get asanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of asanEnabled
     */
    public static boolean getStageAsanEnabled(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        if (appObj.containsKey(ASAN_ENABLED)) {
            return appObj.getBoolean(ASAN_ENABLED);
        }
        return false;
    }

    /**
     * get asanEnabled in module.json
     *
     * @param jsonString is the file content of module.json
     * @return the result
     */
    public static String getStageApiReleaseType(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageApiReleaseType!");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        return getJsonString(appObj, API_RELEASE_TYPE);
    }

    /**
     * get asanEnabled in config.json
     *
     * @param jsonString is the file content of module.json
     * @return the value of asanEnabled
     */
    public static boolean getFAAsanEnabled(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageAsanEnabled!");
            throw new BundleException("parse JOSNObject failed in getStageAsanEnabled!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input module.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
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
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageApiReleaseType!");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, config.json has no app!");
            throw new BundleException("Error: parse failed, input config.json is invalid, config.json has no app!");
        }
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        if (apiVersionObj == null) {
            return "";
        }
        return getJsonString(apiVersionObj, RELEASE_TYPE);
    }

    /**
     * check atomicService split is valid
     * if split is true , must config main
     * if split is false, can not config main
     * @param jsonString is the file content of config.json
     * @return the result
     */
    public static boolean isAtomicServiceSplitValid(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageApiReleaseType!");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, config.json has no app!");
            throw new BundleException("Error: parse failed, input module.json is invalid, module.json has no app!");
        }
        if (!appObj.containsKey(ATOMIC_SERVICE)) {
            return true;
        }
        JSONObject atomicServiceObj = appObj.getJSONObject(ATOMIC_SERVICE);
        if (!atomicServiceObj.containsKey(SPLIT)) {
            LOG.error("Error: atomicService must contain split!");
            return false;
        }
        boolean split = getJsonBooleanValue(atomicServiceObj, SPLIT, true);
        if (!split && atomicServiceObj.containsKey(MAIN)) {
            LOG.error("Error: atomicService can not contain main when split is false!");
            return false;
        }
        if (split && !atomicServiceObj.containsKey(MAIN)) {
            LOG.error("Error: atomicService must contain main when split is true!");
            return false;
        }

        return true;
    }

    /**
     * check module atomic service is valid
     * @param jsonString is the file content of config.json
     * @return the result
     */
    public static boolean isModuleAtomicServiceValid(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageApiReleaseType!");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType!");
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, config.json has no module!");
            throw new BundleException("Error: parse failed, input config.json is invalid, module.json has no app!");
        }
        if (!moduleObj.containsKey(ATOMIC_SERVICE)) {
            return true;
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input config.json is invalid, module.json has no app!");
        }
        if (moduleObj.containsKey(ATOMIC_SERVICE) && !appObj.containsKey(ATOMIC_SERVICE)) {
            LOG.error("Error: module can not config atomicService when app do not contain atomicService!");
            return false;
        }
        JSONObject appAtomicServiceObj = appObj.getJSONObject(ATOMIC_SERVICE);
        if (!appAtomicServiceObj.containsKey(SPLIT)) {
            LOG.error("Error: atomicService must contain split!");
            return false;
        }
        boolean split = getJsonBooleanValue(appAtomicServiceObj, SPLIT, true);
        JSONObject moduleAtomicService = moduleObj.getJSONObject(ATOMIC_SERVICE);
        if (!split && moduleAtomicService.containsKey(PRELOADS)) {
            LOG.error("Error: can not config preloads when split is false!");
            return false;
        }
        return true;
    }

    /**
     * check module atomic installation free is valid
     * @param jsonString is the file content of config.json
     * @return the result
     */
    public static boolean checkAtomicServiceInstallationFree(String jsonString) throws BundleException {
        JSONObject jsonObject;
        try {
            jsonObject = JSON.parseObject(jsonString);
        } catch (JSONException exception) {
            LOG.error("Error: parse JOSNObject failed in getStageApiReleaseType!");
            throw new BundleException("parse JOSNObject failed in getStageApiReleaseType!");
        }
        JSONObject moduleObj = jsonObject.getJSONObject(MODULE);
        if (moduleObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, config.json has no module!");
            throw new BundleException("Error: parse failed, input config.json is invalid, module.json has no app!");
        }
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parse failed, input config.json is invalid, module.json has no app!");
            throw new BundleException("Error: parse failed, input config.json is invalid, module.json has no app!");
        }
        boolean installationFree = getJsonBooleanValue(moduleObj, INSTALLATION_FREE, false);
        if (!installationFree && appObj.containsKey(ATOMIC_SERVICE)) {
            LOG.error("Error: installationFree must be true for atomic service");
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
        if (jsonObject != null && jsonObject.containsKey(key)) {
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
}
