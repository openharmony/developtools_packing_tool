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

import com.sun.istack.internal.FinalArrayList;
import ohos.utils.fastjson.JSON;
import ohos.utils.fastjson.JSONArray;
import ohos.utils.fastjson.JSONObject;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;

public class ModuleJsonUtil {
    private static final String  VERSION_CODE = "\"versionCode\"";
    private static final String VERSION_NAME = "\"versionName\"";
    private static final String APP = "app";
    private static final String VERSIONCODE = "versionCode";
    private static final String VERSIONNAME = "versionName";
    private static final String MIN_COMPATIBLE_VERSIONCODE = "minCompatibleVersionCode";
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
    private static final String MODULENAME = "moduleName";
    private static final String MODULETYPE= "moduleType";
    private static final String DISTRO = "distro";
    private static final String PACKAGE = "package";
    private static final String PACKAGES = "packages";
    private static final String SUMMARY = "summary";
    private static final String BUNDLENAME = "bundleName";
    private static final String ENTRY = "entry";
    private static final char DOT = '.';
    private static final String CONFIG_JSON = "config.json";
    private static final String MODULE_JSON = "module.json";
    private static final String DEVICE_TYPE = "deviceType";
    private static final String DEVICE_TYPES = "deviceTypes";
    private static final String TYPE= "type";
    private static final String VENDOR = "vendor";
    private static final Log LOG = new Log(ModuleJsonUtil.class.toString());

    /**
     * get the versionCode from json file.
     *
     * @param jsonString uncompress json object
     * @return the result
     */
     public static Version getVersion(String jsonString) throws BundleException {
         Version version = new Version();
         try {
             if (jsonString != null) {
                // find versionName
                int indexOfVersionName = jsonString.indexOf(VERSION_NAME);
                if (indexOfVersionName <= 0) {
                    LOG.error("ModuleJsonUtil::getVersion obtain index of VERSION_NAME failed");
                    return version;
                }
                indexOfVersionName += VERSION_NAME.length();
                int nameStart = jsonString.indexOf("\"", indexOfVersionName);
                int nameEnd = nameStart + 1;
                while(jsonString.charAt(nameEnd) != '\"') {
                    ++nameEnd;
                }
                version.versionName = jsonString.substring(nameStart + 1, nameEnd).trim();
                //find versionCode
                int indexOfVersionCode = jsonString.indexOf(VERSION_CODE);
                if (indexOfVersionCode <= 0) {
                    LOG.error("ModuleJsonUtil::getVersion obtain index of VERSION_CODE failed");
                    return version;
                }
                indexOfVersionCode += VERSION_CODE.length();
                while(!Character.isDigit(jsonString.charAt(indexOfVersionCode))) {
                    ++indexOfVersionCode;
                }
                String versionCode = "";
                while(Character.isDigit(jsonString.charAt(indexOfVersionCode))) {
                    versionCode += jsonString.charAt(indexOfVersionCode);
                    ++indexOfVersionCode;
                }
                version.versionCode = Integer.parseInt(versionCode.trim());
            }
         } catch (NumberFormatException exception) {
             LOG.error("ModuleJsonUtil::getVersion NumberFormatException exception: " + exception.getMessage());
             throw new BundleException("ModuleJsonUtil::getVersion failed");
         }
         return version;
     }

    /**
     * get the version from json file for stage module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static Version parseStageVersion(String jsonString) throws BundleException {
        Version version = new Version();
        JSONObject jsonObject = JSON.parseObject(jsonString);
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
            if (appObj.containsKey(MIN_COMPATIBLE_VERSIONCODE)) {
                version.minCompatibleVersionCode = appObj.getIntValue(MIN_COMPATIBLE_VERSIONCODE);
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
        JSONObject jsonObject = JSON.parseObject(jsonString);
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
        if (versionObj.containsKey(MIN_COMPATIBLE_VERSIONCODE)) {
            version.minCompatibleVersionCode = versionObj.getIntValue(MIN_COMPATIBLE_VERSIONCODE);
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
    public static APIVersion parseStageAPIVersion(String jsonString) throws BundleException {
        APIVersion apiVersion = new APIVersion();
        JSONObject jsonObject = JSON.parseObject(jsonString);
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            throw new BundleException("ModuleJsonUtil:parseFaVersion failed : json file do not contain app.");
        }
        if (appObj.containsKey(MIN_API_VERSION)) {
            apiVersion.compatibleApiVersion = appObj.getIntValue(MIN_API_VERSION);
        }
        if (appObj.containsKey(TARGET_API_VERSION)) {
            apiVersion.targetApiVersion = appObj.getIntValue(TARGET_API_VERSION);
        }
        if (appObj.containsKey(API_RELEASE_TYPE)) {
            apiVersion.releaseType = appObj.getString(API_RELEASE_TYPE);
        }
        return apiVersion;
    }

    /**
     * get the apiVersion from json file for fa module.
     *
     * @param jsonString uncompress json object
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static APIVersion parseFAAPIVersion(String jsonString) throws BundleException {
        APIVersion apiVersion = new APIVersion();
        JSONObject jsonObject = JSON.parseObject(jsonString);
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            throw new BundleException("ModuleJsonUtil::parseFAAPIVersion json file do not contain app.");
        }
        if (!appObj.containsKey(API_VERSION)) {
            throw new BundleException("ModuleJsonUtil::parseFAAPIVersion json file do not contain apiVersion.");
        }
        JSONObject apiVersionObj = appObj.getJSONObject(API_VERSION);
        if (apiVersionObj.containsKey(COMPATIBLE)) {
            apiVersion.compatibleApiVersion = apiVersionObj.getIntValue(COMPATIBLE);
        }
        if (apiVersionObj.containsKey(RELEASE_TYPE)) {
            apiVersion.releaseType = apiVersionObj.getString(RELEASE_TYPE);
        }
        if (apiVersionObj.containsKey(TARGET)) {
            apiVersion.targetApiVersion = apiVersionObj.getIntValue(TARGET);
        }
        return apiVersion;
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
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
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
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
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
            if (!distroObj.containsKey(MODULENAME)) {
                LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULENAME);
        } catch (BundleException e) {
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
        try {
            JSONObject jsonObject = JSON.parseObject(jsonString);
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
        } catch (BundleException e) {
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
        String bundleName = "";
        JSONObject jsonObject = JSON.parseObject(jsonString);
        JSONObject appObject = jsonObject.getJSONObject(APP);
        if (appObject == null) {
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
        }
        if (appObject.containsKey(BUNDLENAME)) {
            bundleName = appObject.getString(BUNDLENAME);
        } else {
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
        String vendor = "";
        JSONObject jsonObject = JSON.parseObject(jsonString);
        JSONObject appObject = jsonObject.getJSONObject(APP);
        if (appObject == null) {
            throw new BundleException("ModuleJsonUtil::parseStageBundleName json object do not contain app!");
        }
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
        try {
            JSONObject finalPackObj = JSON.parseObject(finalPackInfo);
            JSONObject srcPackObj = JSON.parseObject(srcPackInfo);
            if (!verifyPackInfo(finalPackObj, srcPackObj)) {
                LOG.error("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed!");
                throw new BundleException("ModuleJsonUtil:mergeTwoPackInfo verify pack.info failed!");
            }
            desPackInfo = mergePackInfoObj(finalPackObj, srcPackObj);
        } catch (BundleException e) {
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
        // check modules info
        if (!verifyModuleInPackInfo(finalSummaryObj.getJSONArray(MODULES), srcSummaryObj.getJSONArray(MODULES))) {
            LOG.error("ModuleJsonUtil:verifyPackInfo verify module failed!");
            return false;
        }
        // check packageName info
        if (!verifyPackageName(finalPackObj.getJSONArray(PACKAGES), srcPackObj.getJSONArray(PACKAGES))) {
            LOG.error("ModuleJsonUtil:verifyPackInfo verify package failed!");
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
        String finalBundleName = finalAppObj.getString(BUNDLENAME);
        String srcBundleName = srcAppObj.getString(BUNDLENAME);
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
            if (!distroObj.containsKey(MODULENAME)) {
                LOG.error("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
                throw new BundleException("ModuleJsonUtil:parseFaModuleName failed: json file do not contain moduleName.");
            }
            moduleName = distroObj.getString(MODULENAME);
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
        JSONObject finalPackObj = JSON.parseObject(finalPackInfo);
        JSONObject srcPackObj = JSON.parseObject(srcPackInfo);
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
            if (distroObj.getString(MODULENAME).equals(moduleName)) {
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
        JSONObject faObj = JSONObject.parseObject(configJson);
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
        String moduleType = distroObj.getString(MODULETYPE);
        if (moduleType.equals(ENTRY)) {
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
        JSONObject stageObj = JSONObject.parseObject(moduleJson);
        JSONObject moduleObj = stageObj.getJSONObject(MODULE);
        if (moduleObj == null) {
            String errMSg = "ModuleJsonUtil::isFaEntry error, json do not contain module!";
            LOG.error(errMSg);
            throw new BundleException(errMSg);
        }
        List<String> deviceTypes = new ArrayList<>();
        String type = moduleObj.getString(TYPE);
        if (type.equals(ENTRY)) {
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
}
