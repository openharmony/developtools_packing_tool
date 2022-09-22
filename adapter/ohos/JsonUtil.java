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

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Locale;

import ohos.utils.fastjson.JSON;
import ohos.utils.fastjson.JSONObject;
import ohos.utils.fastjson.JSONArray;

/**
 * Json Util.
 *
 */
public class JsonUtil {
    private static final String DEVICE_TYPE = "device-type";
    private static final String NAME = "name";
    private static final String MODULE_TYPE = "module-type";
    private static final String INSTALL_FLAG = "delivery-with-install";
    private static final String DEVICE_TYPE_NEW = "deviceType";
    private static final String MODULE_TYPE_NEW = "moduleType";
    private static final String INSTALL_FLAG_NEW = "deliveryWithInstall";
    private static final String ASSETS_DIR_NAME = "assets/";
    private static final String PROFILE = "$profile:";
    private static final Log LOG = new Log(JsonUtil.class.toString());

    private static final String SUMMARY = "summary";
    private static final String APP = "app";
    private static final String VERSION = "version";
    private static final String LEGACY_VERSION_CODE = "legacyVersionCode";
    private static final String LEGACY_VERSION_NAME = "legacyVersionName";
    private static final String MULTI_FRAMEWORK_BUNDLE = "multiFrameworkBundle";
    private static final String ACTION_SYSTEM_HOME = "action.system.home";
    private static final String ENTITY_SYSTEM_HOME = "entity.system.home";
    private static final String MAIN_ABILITY = "mainAbility";
    private static final String MAIN_ELEMENT = "mainElement";
    private static final String PAGE = "page";
    private static final String SERVICE = "service";
    private static final String FORM = "form";
    private static final String PACKAGES = "packages";
    private static final String ABILITIES = "abilities";
    private static final String WHEN = "when";
    private static final String STRING_RESOURCE = "$string:";
    private static final String EMPTY = "";

    /**
     * parse hap list by device type
     *
     * @param deviceType target deviceType
     * @param jsonString uncompress json String
     * @return the parseHapList result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<PackInfo> parseHapList(String deviceType, String jsonString) throws BundleException {
        List<PackInfo> packInfos = new ArrayList<PackInfo>();
        JSONObject jsonObject = JSONObject.parseObject(jsonString);
        if (jsonObject == null || !jsonObject.containsKey("packages")) {
            LOG.error("Uncompress::parseHapList exception: packages is null");
            throw new BundleException("Parse hap list failed, packages is null");
        }
        JSONArray jsonList = JSONArray.parseArray(getJsonString(jsonObject, "packages"));
        if (jsonList == null) {
            LOG.error("Uncompress::parseHapList exception: packages is null");
            throw new BundleException("Parse hap list failed, packages is null");
        }
        int jsonListSize = jsonList.size();
        for (int i = 0; i < jsonListSize; i++) {
            JSONObject tmpObj = jsonList.getJSONObject(i);
            String deviceTypes = getJsonString(tmpObj, DEVICE_TYPE);
            if (deviceTypes == null || EMPTY.equals(deviceTypes)) {
                deviceTypes = getJsonString(tmpObj, DEVICE_TYPE_NEW);
            }
            if (deviceTypes != null && deviceTypes.toLowerCase(Locale.ENGLISH).contains(
                    deviceType.toLowerCase(Locale.ENGLISH))) {
                PackInfo packInfo = new PackInfo();
                packInfo.name = getJsonString(tmpObj, NAME);
                packInfo.moduleType = getJsonString(tmpObj, MODULE_TYPE);
                if (packInfo.moduleType == null || EMPTY.equals(packInfo.moduleType)) {
                    packInfo.moduleType = getJsonString(tmpObj, MODULE_TYPE_NEW);
                }
                packInfo.deviceType = JSONArray.parseArray(deviceTypes, String.class);
                String deliveryWithInstall = getJsonString(tmpObj, INSTALL_FLAG);
                if (deliveryWithInstall == null || EMPTY.equals(deliveryWithInstall)) {
                    deliveryWithInstall = getJsonString(tmpObj, INSTALL_FLAG_NEW);
                }
                packInfo.deliveryWithInstall = Boolean.parseBoolean(deliveryWithInstall);
                packInfos.add(packInfo);
            }
        }
        return packInfos;
    }

    static List<PackInfo> parsePackInfos(String jsonString) throws BundleException {
        JSONObject jsonObject = JSONObject.parseObject(jsonString);
        if (jsonObject == null || !jsonObject.containsKey("packages")) {
            LOG.error("JsonUtil::parsePackInfos exception: packages is null");
            throw new BundleException("Parse hap list failed, packages is null");
        }
        String packages = getJsonString(jsonObject, PACKAGES);
        return JSONArray.parseArray(packages, PackInfo.class);
    }

    private static boolean parseShellVersionInfoToAppInfo(String packInfoJsonStr, AppInfo appInfo) {
        LOG.info("Uncompress::parseShellVersionInfoToAppInfo: begin");
        if (!appInfo.isMultiFrameworkBundle()) {
            LOG.info("Uncompress::parseShellVersionInfoToAppInfo: is not a multi framewok bundle.");
            return false;
        }

        JSONObject jsonObject = JSONObject.parseObject(packInfoJsonStr);
        if (jsonObject == null) {
            LOG.error("Uncompress::parseShellVersionInfoToAppInfo error: summary is null");
            return false;
        }

        JSONObject summaryJson = jsonObject.getJSONObject(SUMMARY);
        if (summaryJson == null) {
            LOG.error("Uncompress::parseShellVersionInfoToAppInfo error: summary is null");
            return false;
        }
        JSONObject appJson = summaryJson.getJSONObject(APP);
        if (appJson == null) {
            LOG.error("Uncompress::parseShellVersionInfoToAppInfo error: app is null");
            return false;
        }
        JSONObject versionJson = appJson.getJSONObject(VERSION);
        if (versionJson == null) {
            LOG.error("Uncompress::parseShellVersionInfoToAppInfo error: version is null");
            return false;
        }

        if (!versionJson.containsKey(LEGACY_VERSION_CODE) || !versionJson.containsKey(LEGACY_VERSION_NAME)) {
            LOG.error("Uncompress::parseShellVersionInfoToAppInfo no legacy version info.");
            return false;
        }
        appInfo.setShellVersionCode(versionJson.getString(LEGACY_VERSION_CODE));
        appInfo.setShellVersionName(versionJson.getString(LEGACY_VERSION_NAME));
        return true;
    }

    private static void parseDeviceTypeToHapInfo(String packInfoJsonStr, HapInfo hapInfo, String hapName) {
        LOG.info("Uncompress::parseDeviceTypeToHapInfo: begin");
        JSONObject jsonObject = JSONObject.parseObject(packInfoJsonStr);
        if (jsonObject == null || !jsonObject.containsKey("packages")) {
            LOG.error("Uncompress::parseDeviceTypeToHapInfo error: no packages");
            return;
        }
        JSONArray jsonList = JSONArray.parseArray(getJsonString(jsonObject, "packages"));
        if (jsonList == null) {
            LOG.error("Uncompress::parseDeviceTypeToHapInfo error: packages is null");
            return;
        }

        for (int i = 0; i < jsonList.size(); i++) {
            JSONObject tmpObj = jsonList.getJSONObject(i);
            if (tmpObj == null) {
                LOG.error("Uncompress::parseDeviceTypeToHapInfo error: obj is null");
                continue;
            }
            String name = getJsonString(tmpObj, NAME);
            if (name != null && name.equals(hapName)) {
                String deviceTypes = getJsonString(tmpObj, DEVICE_TYPE_NEW);
                if (deviceTypes == null || deviceTypes.isEmpty()) {
                    deviceTypes = getJsonString(tmpObj, DEVICE_TYPE);
                }
                if (deviceTypes != null && !deviceTypes.isEmpty()) {
                    hapInfo.deviceType = JSONArray.parseArray(deviceTypes
                        .replace(UncompressEntrance.DEVICE_TYPE_DEFAULT, UncompressEntrance.DEVICE_TYPE_PHONE),
                        String.class);
                }
                break;
            }
        }
        return;
    }

    /**
     * parse hap profile info
     *
     * @param harmonyProfileJsonString uncompress json String
     * @param data resource index data
     * @param paclInfoJsonString pack.info json String
     * @param hapName hap file name
     * @return the parseProfileInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ProfileInfo parseProfileInfo(String harmonyProfileJsonString, byte[] data, String paclInfoJsonString,
        String hapName) throws BundleException {
        ProfileInfo profileInfo = new ProfileInfo();
        JSONObject jsonObject = JSONObject.parseObject(harmonyProfileJsonString);
        if (jsonObject == null || !jsonObject.containsKey(APP) || !jsonObject.containsKey("deviceConfig")
                || !jsonObject.containsKey("module")) {
            LOG.error("Uncompress::parseProfileInfo exception: app, deviceConfig or module is null");
            throw new BundleException("Parse profile info failed, app, deviceConfig or module is null");
        }
        if (jsonObject.containsKey(APP)) {
            JSONObject appJson = jsonObject.getJSONObject(APP);
            profileInfo.appInfo = parseAppInfo(appJson, data);
        }
        if (jsonObject.containsKey("module")) {
            JSONObject hapJson = jsonObject.getJSONObject("module");
            profileInfo.hapInfo = parseHapInfo(hapJson, data);
        }
        parseDeviceTypeToHapInfo(paclInfoJsonString, profileInfo.hapInfo, hapName);
        if (jsonObject.containsKey("deviceConfig")) {
            JSONObject deviceConfigJson = jsonObject.getJSONObject("deviceConfig");
            profileInfo.deviceConfig = parseDeviceConfigInfo(deviceConfigJson, profileInfo.hapInfo.deviceType);
        }
        if (!parseShellVersionInfoToAppInfo(paclInfoJsonString, profileInfo.appInfo)) {
            profileInfo.appInfo.setDefaultShellVersion();
        }
        if (!profileInfo.appInfo.appName.isEmpty()) {
            return profileInfo;
        }

        if (profileInfo.hapInfo.abilities.size() == 1) {
            profileInfo.appInfo.appName = profileInfo.hapInfo.abilities.get(0).label;
            profileInfo.appInfo.appNameEN = profileInfo.hapInfo.abilities.get(0).label;
        } else {
            for (AbilityInfo abilityInfo : profileInfo.hapInfo.abilities) {
                boolean isLabel = false;
                for (SkillInfo skill : abilityInfo.skills) {
                    if (skill.actions.contains("action.system.home") && skill.entities.contains("entity.system.home")) {
                        isLabel = true;
                        break;
                    }
                }
                if (isLabel) {
                    profileInfo.appInfo.appName = abilityInfo.label;
                    profileInfo.appInfo.appNameEN = abilityInfo.label;
                    break;
                }
            }
        }
        return profileInfo;
    }

    /**
     * parse app info
     *
     * @param appJson global json Object
     * @param data resource index data
     * @return the parseAppInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static AppInfo parseAppInfo(JSONObject appJson, byte[] data) throws BundleException {
        AppInfo appInfo = new AppInfo();
        if (appJson == null) {
            LOG.error("Uncompress::parseAppInfo exception: appJson is null");
            throw new BundleException("Parse app info failed, appJson is null");
        }
        appInfo.bundleName = getJsonString(appJson, "bundleName");
        appInfo.vendor = getJsonString(appJson, "vendor");
        appInfo.relatedBundleName = getJsonString(appJson, "relatedBundleName");
        if (appJson.containsKey(VERSION)) {
            JSONObject version = appJson.getJSONObject(VERSION);
            appInfo.versionName = getJsonString(version, "name");
            appInfo.versionCode = getJsonString(version, "code");
        }
        if (appJson.containsKey("apiVersion")) {
            JSONObject apiVersion = appJson.getJSONObject("apiVersion");
            appInfo.compatibleApiVersion = apiVersion.getIntValue("compatible");
            appInfo.targetApiVersion = apiVersion.getIntValue("target");
            appInfo.releaseType = getJsonString(apiVersion, "releaseType");
        }
        String labelRes = "";
        if (appJson.containsKey("labelId")) {
            int labelId = appJson.getIntValue("labelId");
            labelRes = ResourcesParser.getBaseResourceById(labelId, data);
        }
        if (labelRes != null && !labelRes.isEmpty()) {
            appInfo.appName = labelRes;
            appInfo.appNameEN = labelRes;
        } else if (appJson.containsKey("label")) {
            appInfo.appName = getJsonString(appJson, "label");
            appInfo.appNameEN = getJsonString(appJson, "label");
        }
        appInfo.setMultiFrameworkBundle(appJson.getBooleanValue(MULTI_FRAMEWORK_BUNDLE));
        return appInfo;
    }

    /**
     * parse module app info
     *
     * @param appJson global json Object
     * @param data resource index data
     * @return the moduleApp result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleAppInfo parseModuleAppInfo(JSONObject appJson, byte[] data) throws BundleException {
        ModuleAppInfo moduleAppInfo = new ModuleAppInfo();
        if (appJson == null) {
            LOG.error("Uncompress::parseModuleAppInfo exception: appJson is null");
            throw new BundleException("Parse module app info failed, appJson is null");
        }

        moduleAppInfo.bundleName = getJsonString(appJson, "bundleName");
        if (appJson.containsKey("debug")) {
            moduleAppInfo.debug = appJson.getBoolean("debug");
        }

        moduleAppInfo.icon = parseIconById(appJson, data);
        moduleAppInfo.label = parseResourceByKey(appJson, data, "label", "labelId");
        moduleAppInfo.description = parseResourceByKey(appJson, data, "description", "descriptionId");

        if (appJson.containsKey("vendor")) {
            moduleAppInfo.vendor = getJsonString(appJson, "vendor");
        }

        if (appJson.containsKey("versionCode")) {
            moduleAppInfo.versionCode = appJson.getIntValue("versionCode");
        }

        if (appJson.containsKey("versionName")) {
            moduleAppInfo.versionName = getJsonString(appJson, "versionName");
        }

        if (appJson.containsKey("minCompatibleVersionCode")) {
            moduleAppInfo.minCompatibleVersionCode = appJson.getIntValue("minCompatibleVersionCode");
        } else {
            moduleAppInfo.minCompatibleVersionCode = appJson.getIntValue("versionCode");
        }

        if (appJson.containsKey("minAPIVersion")) {
            moduleAppInfo.minAPIVersion = appJson.getIntValue("minAPIVersion");
        }

        if (appJson.containsKey("targetAPIVersion")) {
            moduleAppInfo.targetAPIVersion = appJson.getIntValue("targetAPIVersion");
        }

        if (appJson.containsKey("apiReleaseType")) {
            moduleAppInfo.apiReleaseType = getJsonString(appJson, "apiReleaseType");
        }

        if (appJson.containsKey("distributedNotificationEnabled")) {
            moduleAppInfo.distributedNotificationEnabled = appJson.getBoolean("distributedNotificationEnabled");
        }

        if (appJson.containsKey("entityType")) {
            moduleAppInfo.entityType = getJsonString(appJson, "entityType");
        }

        // parse device type
        parseDeviceType(appJson, moduleAppInfo, "phone");
        parseDeviceType(appJson, moduleAppInfo, "tablet");
        parseDeviceType(appJson, moduleAppInfo, "tv");
        parseDeviceType(appJson, moduleAppInfo, "wearable");
        parseDeviceType(appJson, moduleAppInfo, "ar");
        parseDeviceType(appJson, moduleAppInfo, "vr");
        parseDeviceType(appJson, moduleAppInfo, "car");
        parseDeviceType(appJson, moduleAppInfo, "earphones");
        parseDeviceType(appJson, moduleAppInfo, "speaker");
        parseDeviceType(appJson, moduleAppInfo, "linkIOT");
        parseDeviceType(appJson, moduleAppInfo, "router");

        return moduleAppInfo;
    }

    /**
     * parse deviceType
     *
     * @param appJson is the json Object of app
     * @param moduleAppInfo the app in module
     * @param deviceName the device name in deviceType array
     * @return the parseDeviceConfigInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleDeviceType parseDeviceType(JSONObject appJson, ModuleAppInfo moduleAppInfo,
                                            String deviceName) throws BundleException {
        if (appJson == null) {
            LOG.error("JsonUtil::parseDeviceType exception: appJson is null");
            throw new BundleException("Parse app info failed, appJson is null");
        }
        ModuleDeviceType moduleDeviceType = new ModuleDeviceType();
        if (appJson.containsKey(deviceName)) {
            JSONObject deviceObj = appJson.getJSONObject(deviceName);
            if (deviceObj.containsKey("minAPIVersion")) {
                moduleDeviceType.minAPIVersion = deviceObj.getIntValue("minAPIVersion");
            } else {
                moduleDeviceType.minAPIVersion = moduleAppInfo.minAPIVersion;
            }

            if (deviceObj.containsKey("distributedNotificationEnabled")) {
                moduleDeviceType.distributedNotificationEnabled = deviceObj.getBoolean("distributedNotificationEnabled");
            } else {
                moduleDeviceType.distributedNotificationEnabled = moduleAppInfo.distributedNotificationEnabled;
            }

            moduleAppInfo.deviceTypes.put(deviceName, moduleDeviceType);
        }
        return moduleDeviceType;
    }

    /**
     * parse deviceConfig infos
     *
     * @param deviceConfigInfoJson deviceConfig json Object
     * @param deviceTypes the deviceTypes of the hap
     * @return the parseDeviceConfigInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static Map<String, DeviceConfig> parseDeviceConfigInfo(JSONObject deviceConfigInfoJson,
            List<String> deviceTypes) throws BundleException {
        Map<String, DeviceConfig> deviceConfigs = new HashMap<>();
        if (deviceConfigInfoJson == null) {
            return deviceConfigs;
        }

        DeviceConfig defaultConfig = new DeviceConfig();
        if (deviceConfigInfoJson.containsKey("default")) {
            defaultConfig = parseDeviceConfig(deviceConfigInfoJson.getJSONObject("default"),
                    new DeviceConfig());
        }

        for (String deviceType : deviceTypes) {
            getTargetDeviceConfig(deviceConfigs, deviceConfigInfoJson, defaultConfig, deviceType);
        }

        if (deviceConfigs.isEmpty()) {
            deviceConfigs.put(UncompressEntrance.DEVICE_TYPE_PHONE, defaultConfig);
        }

        return deviceConfigs;
    }

    /**
     * parse deviceConfig infos
     *
     * @param deviceConfigs the parseDeviceConfigInfo result
     * @param deviceConfigInfoJson deviceConfig json Object
     * @param defaultConfig the default deviceConfig
     * @param targetDeviceType the target deviceType
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static void getTargetDeviceConfig(Map<String, DeviceConfig> deviceConfigs, JSONObject deviceConfigInfoJson,
            DeviceConfig defaultConfig, String targetDeviceType) throws BundleException {
        if (deviceConfigInfoJson.containsKey(targetDeviceType)) {
            DeviceConfig deviceConfig = parseDeviceConfig(
                    deviceConfigInfoJson.getJSONObject(targetDeviceType), defaultConfig);
            deviceConfigs.put(targetDeviceType, deviceConfig);
        } else {
            deviceConfigs.put(targetDeviceType, defaultConfig);
        }
    }

    /**
     * parse deviceConfig info
     *
     * @param deviceConfigJson deviceConfig json Object
     * @param defaultConfig default deviceConfig
     * @return the parseDeviceConfigInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static DeviceConfig parseDeviceConfig(JSONObject deviceConfigJson, DeviceConfig defaultConfig)
            throws BundleException {
        DeviceConfig deviceConfig = new DeviceConfig();
        if (deviceConfigJson == null || defaultConfig == null) {
            LOG.error("Uncompress::parseDeviceConfigInfo exception: deviceConfigJson or defaultConfig is null");
            throw new BundleException("Parse device config info failed, deviceConfigJson or defaultConfig is null");
        }

        deviceConfig.jointUserid = getJsonString(deviceConfigJson, "jointUserid", defaultConfig.jointUserid);
        deviceConfig.process = getJsonString(deviceConfigJson, "process", defaultConfig.process);

        if (deviceConfigJson.containsKey("reqSdk")) {
            JSONObject reqSdk = deviceConfigJson.getJSONObject("reqSdk");
            deviceConfig.compatibleReqSdk = getJsonString(reqSdk, "compatible", defaultConfig.compatibleReqSdk);
            deviceConfig.targetReqSdk = getJsonString(reqSdk, "target", defaultConfig.targetReqSdk);
        }

        if (deviceConfigJson.containsKey("ark")) {
            JSONObject ark = deviceConfigJson.getJSONObject("ark");
            deviceConfig.arkFlag = getJsonString(ark, "flag", defaultConfig.arkFlag);
            if (ark.containsKey("reqVersion")) {
                JSONObject arkVersion = ark.getJSONObject("reqVersion");
                deviceConfig.targetArkVersion = getJsonString(arkVersion, "target", defaultConfig.targetArkVersion);
                deviceConfig.compatibleArkVersion = getJsonString(arkVersion, "compatible",
                        defaultConfig.compatibleArkVersion);
            }
        }

        if (deviceConfigJson.containsKey("directLaunch")) {
            deviceConfig.directLaunch = deviceConfigJson.getBoolean("directLaunch");
        } else {
            deviceConfig.directLaunch = defaultConfig.directLaunch;
        }

        return deviceConfig;
    }

    /**
     * parse hap info
     *
     * @param hapJson hap json Object
     * @param data resource index data
     * @return the parseHapInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static HapInfo parseHapInfo(JSONObject hapJson, byte[] data) throws BundleException {
        HapInfo hapInfo = new HapInfo();
        if (hapJson == null) {
            LOG.error("Uncompress::parseHapInfo exception: hapJson is null");
            throw new BundleException("Parse hap info failed, hapJson is null");
        }
        hapInfo.packageStr = getJsonString(hapJson, "package");
        hapInfo.name = getJsonString(hapJson, "name");
        hapInfo.description = parseResourceByKey(hapJson, data, "description", "descriptionId");

        if (hapJson.containsKey(MAIN_ABILITY)) {
            hapInfo.mainElement = getJsonString(hapJson, MAIN_ABILITY);
        }

        if (hapJson.containsKey("supportedModes")) {
            hapInfo.supportedModes = JSONObject.parseArray(getJsonString(hapJson, "supportedModes"),
                    String.class);
        }

        if (hapJson.containsKey("defPermissions")) {
            hapInfo.defPermissions = parseDefPermissions(hapJson, data);
        }

        if (hapJson.containsKey("definePermissions")) {
            hapInfo.definePermissions = parseDefinePermissions(hapJson, data);
        }

        if (hapJson.containsKey("defPermissiongroups")) {
            hapInfo.defPermissionsGroups = JSONArray.parseArray(getJsonString(hapJson, "defPermissiongroups"),
                    DefPermissionGroup.class);
        }

        if (hapJson.containsKey("distro")) {
            getDistro(hapJson, hapInfo);
        }

        if (hapJson.containsKey("reqCapabilities")) {
            hapInfo.reqCapabilities = JSONArray.parseArray(getJsonString(hapJson, "reqCapabilities"),
                    String.class);
        }

        if (hapJson.containsKey("deviceType")) {
            hapInfo.deviceType = JSONArray.parseArray(getJsonString(hapJson, "deviceType")
                    .replace(UncompressEntrance.DEVICE_TYPE_DEFAULT, UncompressEntrance.DEVICE_TYPE_PHONE),
                    String.class);
        }

        if (hapJson.containsKey("metaData")) {
            JSONObject metaDataJson = hapJson.getJSONObject("metaData");
            hapInfo.metaData = parseMetaData(metaDataJson);
        }

        if (hapJson.containsKey("js")) {
            List<JsInfo> jsInfos = JSONObject.parseArray(getJsonString(hapJson, "js"), JsInfo.class);
            if (jsInfos != null && jsInfos.size() > 0) {
                hapInfo.isJs = true;
            }
        }

        if (hapJson.containsKey("reqPermissions")) {
            hapInfo.reqPermissions = JSONObject.parseArray(getJsonString(hapJson, "reqPermissions"),
                    ReqPermission.class);
        }

        if (hapJson.containsKey("commonEvents")) {
            hapInfo.commonEvents = JSONObject.parseArray(getJsonString(hapJson, "commonEvents"),
                    CommonEvent.class);
        }

        if (hapJson.containsKey("shortcuts")) {
            hapInfo.shortcuts = parseShoruCuts(hapJson, data);
        }

        if (hapJson.containsKey("abilities")) {
            JSONArray abilities = hapJson.getJSONArray("abilities");
            List<AbilityInfo> abilitieList = new ArrayList<AbilityInfo>();
            int abilitiesSize = abilities.size();
            for (int i = 0; i < abilitiesSize; i++) {
                JSONObject tmpObj = abilities.getJSONObject(i);
                abilitieList.add(parseAbility(tmpObj, data));
            }
            hapInfo.abilities = abilitieList;
            setFAProviderAbility(hapJson, hapInfo, hapInfo.abilities);
        }

        if (hapJson.containsKey("distroFilter")) {
            hapInfo.distroFilter = JSONObject.parseObject(getJsonString(hapJson, "distroFilter"), DistroFilter.class);
        }
        return hapInfo;
    }

    /**
     * get distro
     *
     * @param hapJson hapJson json
     * @param hapInfo hapInfo json object
     */
    private static void getDistro(JSONObject hapJson, HapInfo hapInfo) {
        JSONObject distroObj = hapJson.getJSONObject("distro");
        Distro distro = new Distro();
        distro.moduleName = getJsonString(distroObj, "moduleName");
        distro.moduleType = getJsonString(distroObj, "moduleType");
        distro.deliveryWithInstall = distroObj.getBoolean("deliveryWithInstall");
        if (distroObj.containsKey("installationFree")) {
            boolean isFreeInstall = distroObj.getBoolean("installationFree");
            if (isFreeInstall) {
                distro.installationFree = 1;
            }else {
                distro.installationFree = 0;
            }
        } else {
            distro.installationFree = 2;
        }
        if (distroObj.containsKey("virtualMachine")) {
            distro.virtualMachine = getJsonString(distroObj, "virtualMachine");
        } else {
            distro.virtualMachine = "default";
        }
        hapInfo.distro = distro;
    }

    /**
     * parse meta data
     *
     * @param metaDataJson meta data json
     * @return the parseMetaData result
     */
    private static MetaData parseMetaData(JSONObject metaDataJson) {
        MetaData metaData = new MetaData();
        if (metaDataJson == null) {
            LOG.error("Uncompress::parseMetaData : metaDataJson is null");
            return metaData;
        }

        if (metaDataJson.containsKey("parameters")) {
            metaData.parameters = JSONObject.parseArray(getJsonString(metaDataJson, "parameters"),
                    MetaDataInfo.class);
        }
        if (metaDataJson.containsKey("results")) {
            metaData.results = JSONObject.parseArray(getJsonString(metaDataJson, "results"),
                    MetaDataInfo.class);
        }
        if (metaDataJson.containsKey("customizeData")) {
            metaData.customizeDatas = JSONObject.parseArray(getJsonString(metaDataJson, "customizeData"),
                    CustomizeData.class);
        }
        return metaData;
    }

    /**
     * parse ability object.
     *
     * @param abilityJson ability json object
     * @param data resource index data
     * @return the parseAbility result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static AbilityInfo parseAbility(JSONObject abilityJson, byte[] data) throws BundleException {
        if (abilityJson == null) {
            LOG.error("Uncompress::parseAbility exception: abilityJson is null");
            throw new BundleException("Parse ability failed, abilityJson is null");
        }
        AbilityInfo ability = new AbilityInfo();
        ability.name = getJsonString(abilityJson, "name");
        if (abilityJson.containsKey("iconId")) {
            int iconId = abilityJson.getIntValue("iconId");
            String iconPath = ResourcesParser.getResourceById(iconId, data);
            if (iconPath != null && !iconPath.isEmpty()) {
                ability.iconPath = ASSETS_DIR_NAME + iconPath;
            }
        }
        ability.icon = ability.iconPath != null && !ability.iconPath.isEmpty() ? ability.iconPath :
                getJsonString(abilityJson, "icon");

        if (abilityJson.containsKey("descriptionId")) {
            int descriptionId = abilityJson.getIntValue("descriptionId");
            ability.descriptionRes = ResourcesParser.getBaseResourceById(descriptionId, data);
        }
        ability.description = ability.descriptionRes != null && !ability.descriptionRes.isEmpty() ?
                ability.descriptionRes : getJsonString(abilityJson, "description");

        if (abilityJson.containsKey("labelId")) {
            int labelId = abilityJson.getIntValue("labelId");
            ability.labelRes = ResourcesParser.getBaseResourceById(labelId, data);
        }
        if (ability.labelRes != null && !ability.labelRes.isEmpty()) {
            ability.label = ability.labelRes;
        } else if (!getJsonString(abilityJson, "label").isEmpty()) {
            ability.label = getJsonString(abilityJson, "label");
        } else {
            ability.label = ability.name;
        }

        ability.type = getJsonString(abilityJson, "type");
        ability.launchType = getJsonString(abilityJson, "launchType");
        ability.orientation = getJsonString(abilityJson, "orientation");
        ability.uri = getJsonString(abilityJson, "uri");
        if (abilityJson.containsKey("formsEnabled")) {
            ability.formsEnabled = abilityJson.getBoolean("formsEnabled");
        }

        if (abilityJson.containsKey("metaData")) {
            JSONObject metaDataJson = abilityJson.getJSONObject("metaData");
            ability.metaData = parseMetaData(metaDataJson);
        }

        if (abilityJson.containsKey("skills")) {
            ability.skills = JSONObject.parseArray(getJsonString(abilityJson, "skills"), SkillInfo.class);
        }

        if (abilityJson.containsKey("backgroundModes")) {
            ability.backgroundModes = JSONObject.parseArray(getJsonString(abilityJson, "backgroundModes"),
                    String.class);
        }

        if (abilityJson.containsKey("visible")) {
            ability.visible = abilityJson.getBoolean("visible");
        }

        if (abilityJson.containsKey("configChanges")) {
            ability.configChanges = JSONObject.parseArray(getJsonString(abilityJson, "configChanges"), String.class);
        }

        if (abilityJson.containsKey("srcLanguage")) {
            ability.srcLanguage = getJsonString(abilityJson, "srcLanguage");
        }

        if (abilityJson.containsKey("srcPath")) {
            ability.srcPath = getJsonString(abilityJson, "srcPath");
        }

        parseAbilityPermissions(abilityJson, ability);

        if (abilityJson.containsKey("forms")) {
            JSONArray forms = abilityJson.getJSONArray("forms");
            List<AbilityFormInfo> formList = new ArrayList<AbilityFormInfo>();
            int formSize = forms.size();
            for (int i = 0; i < formSize; i++) {
                JSONObject tmpObj = forms.getJSONObject(i);
                formList.add(parseAbilityFormInfo(tmpObj, data));
            }
            ability.formInfos = formList;
        }

        return ability;
    }

    /**
     * parse ability object.
     *
     * @param abilityJson ability json object
     * @param ability the parseAbility result
     * @return the parseAbility Permissions result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static void parseAbilityPermissions(JSONObject abilityJson, AbilityInfo ability) throws BundleException {
        if (abilityJson == null || ability == null) {
            LOG.error("Uncompress::parseAbilityPermissions exception: abilityJson or ability is null");
            throw new BundleException("Parse ability permissions failed, abilityJson or ability is null");
        }
        if (abilityJson.containsKey("permissions")) {
            ability.permissions = JSONObject.parseArray(getJsonString(abilityJson, "permissions"), String.class);
        }

        if (abilityJson.containsKey("grantPermission")) {
            ability.grantPermission = abilityJson.getBoolean("grantPermission");
        }
        ability.readPermission = getJsonString(abilityJson, "readPermission");
        ability.writePermission = getJsonString(abilityJson, "writePermission");

        if (abilityJson.containsKey("uriPermission")) {
            JSONObject uriPermission = abilityJson.getJSONObject("uriPermission");
            ability.uriPermissionMode = getJsonString(uriPermission, "mode");
            ability.uriPermissionPath = getJsonString(uriPermission, "path");
        }

        ability.mission = getJsonString(abilityJson, "mission");
        ability.targetAbility = getJsonString(abilityJson, "targetAbility");

        if (abilityJson.containsKey("directLaunch")) {
            ability.directLaunch = abilityJson.getBoolean("directLaunch");
        }

        if (abilityJson.containsKey("multiUserShared")) {
            ability.multiUserShared = abilityJson.getBoolean("multiUserShared");
        }

        if (abilityJson.containsKey("supportPipMode")) {
            ability.supportPipMode = abilityJson.getBoolean("supportPipMode");
        }

        if (abilityJson.containsKey("form")) {
            JSONObject form = abilityJson.getJSONObject("form");
            FormInfo formInfo = new FormInfo();
            if (form.containsKey("formEntity")) {
                formInfo.formEntity = JSONObject.parseArray(getJsonString(form, "formEntity"), String.class);
            }
            formInfo.minWidth = getJsonString(form, "minWidth");
            formInfo.defaultWidth = getJsonString(form, "defaultWidth");
            formInfo.minHeight = getJsonString(form, "minHeight");
            formInfo.defaultHeight = getJsonString(form, "defaultHeight");
            ability.formInfo = formInfo;
        }
    }
    /**
     * parse ability forms object
     *
     * @param abilityFormJson ability form json object
     * @return the ability form info result
     */
    static AbilityFormInfo parseAbilityFormInfo(JSONObject abilityFormJson, byte[] data) throws BundleException {
        AbilityFormInfo abilityForm = new AbilityFormInfo();
        if (abilityFormJson == null) {
            LOG.error("Uncompress::parseAbilityFormInfo : abilityFormJson is null");
            return abilityForm;
        }
        abilityForm.name = getJsonString(abilityFormJson, "name");
        abilityForm.type = getJsonString(abilityFormJson, "type");
        abilityForm.description = parseResourceByKey(abilityFormJson, data, "description", "descriptionId");
        if (abilityFormJson.containsKey("isDefault")) {
            abilityForm.isDefault = abilityFormJson.getBoolean("isDefault");
        }
        if (abilityFormJson.containsKey("colorMode")) {
            abilityForm.colorMode = getJsonString(abilityFormJson, "colorMode");
        }
        if (abilityFormJson.containsKey("formConfigAbility")) {
            abilityForm.formConfigAbility = getJsonString(abilityFormJson, "formConfigAbility");
        }
        if (abilityFormJson.containsKey("updateEnabled")) {
            abilityForm.updateEnabled = abilityFormJson.getBoolean("updateEnabled");
        }
        abilityForm.scheduledUpdateTime = getJsonString(abilityFormJson, "scheduledUpdateTime");
        if (abilityFormJson.containsKey("updateDuration")) {
            abilityForm.updateDuration = abilityFormJson.getIntValue("updateDuration");
        }
        if (abilityFormJson.containsKey("supportDimensions")) {
            abilityForm.supportDimensions = JSONObject.parseArray(getJsonString(abilityFormJson, "supportDimensions"),
                    String.class);
        }
        abilityForm.defaultDimension = getJsonString(abilityFormJson, "defaultDimension");
        if (abilityFormJson.containsKey("metaData")) {
            JSONObject metaDataJson = abilityFormJson.getJSONObject("metaData");
            abilityForm.metaData = parseMetaData(metaDataJson);
        }
        return abilityForm;
    }

    /**
     * parse module hap info
     *
     * @param harmonyProfileJsonString uncompress json String
     * @param data resource index data
     * @param packInfoJsonString pack.info json String
     * @param hapName hap file name
     * @return the parseProfileInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleProfileInfo parseModuleProfileInfo(String harmonyProfileJsonString, byte[] data, String packInfoJsonString,
                                                    String hapName, HashMap<String, String> profileJsons) throws BundleException {
        ModuleProfileInfo moduleProfileInfo = new ModuleProfileInfo();
        JSONObject jsonObject = JSONObject.parseObject(harmonyProfileJsonString);
        if (jsonObject == null || !jsonObject.containsKey("app") || !jsonObject.containsKey("module")) {
            LOG.error("JsonUtil::parseModuleProfileInfo exception: app or module is null");
            throw new BundleException("Parse module profile info failed, app or module is null");
        }

        if (jsonObject.containsKey("app")) {
            JSONObject appJson = jsonObject.getJSONObject("app");
            moduleProfileInfo.moduleAppInfo = parseModuleAppInfo(appJson, data);
        }
        if (jsonObject.containsKey("module")) {
            JSONObject moduleJson = jsonObject.getJSONObject("module");
            moduleProfileInfo.moduleInfo = parseModuleHapInfo(moduleJson, data,
                    moduleProfileInfo.moduleAppInfo.bundleName, profileJsons);
        }

        // parse appName
        for (ModuleAbilityInfo abilityInfo : moduleProfileInfo.moduleInfo.abilities) {
            for (SkillInfo skill : abilityInfo.skills) {
                if (skill.actions.contains(ACTION_SYSTEM_HOME) && skill.entities.contains(ENTITY_SYSTEM_HOME)) {
                    moduleProfileInfo.moduleAppInfo.appName = abilityInfo.label;
                    moduleProfileInfo.moduleAppInfo.appNameEN = abilityInfo.label;
                    break;
                }
            }
        }

        return moduleProfileInfo;
    }

    /**
     * parse module hap info
     *
     * @param moduleJson Json hap json Object
     * @param data resource index data
     * @param hapName is the name of the hap
     * @param profileJsons is the map of profile
     * @return the ModuleProfileInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleInfo parseModuleHapInfo(JSONObject moduleJson, byte[] data, String hapName,
                                         HashMap<String, String> profileJsons) throws BundleException {
        ModuleInfo moduleInfo = new ModuleInfo();
        if (moduleJson == null) {
            LOG.error("JsonUtil::parseModuleHapInfo exception: moduleJson is null");
            throw new BundleException("Parse module hap info failed, moduleJson is null");
        }
        if (moduleJson.containsKey("name")) {
            moduleInfo.name = getJsonString(moduleJson, "name");
        }
        if (moduleJson.containsKey("type")) {
            moduleInfo.type = getJsonString(moduleJson, "type");
        }
        if (moduleJson.containsKey("srcEntrance")) {
            moduleInfo.srcEntrance = getJsonString(moduleJson, "srcEntrance");
        }
        moduleInfo.description = parseResourceByKey(moduleJson, data, "description", "descriptionId");

        if (moduleJson.containsKey("process")) {
            moduleInfo.process = getJsonString(moduleJson, "process");
        } else {
            moduleInfo.process = hapName;
        }
        if (moduleJson.containsKey("mainElement")) {
            moduleInfo.mainElement = getJsonString(moduleJson, "mainElement");
        }
        if (moduleJson.containsKey("deviceTypes")) {
            moduleInfo.deviceTypes = JSONObject.parseArray(getJsonString(moduleJson, "deviceTypes"), String.class);
        }
        if (moduleJson.containsKey("deliveryWithInstall")) {
            moduleInfo.deliveryWithInstall = moduleJson.getBoolean("deliveryWithInstall");
        }
        if (moduleJson.containsKey("installationFree")) {
            boolean isFreeInstall = moduleJson.getBoolean("installationFree");
            if (isFreeInstall) {
                moduleInfo.installationFree = 1;
            } else {
                moduleInfo.installationFree = 0;
            }
        } else {
            moduleInfo.installationFree = 2;
        }

        if (moduleJson.containsKey("virtualMachine")) {
            moduleInfo.virtualMachine = getJsonString(moduleJson, "virtualMachine");
        }

        if (moduleJson.containsKey("uiSyntax")) {
            moduleInfo.uiSyntax = getJsonString(moduleJson, "uiSyntax");
        }
        // parse pages
        if (moduleJson.containsKey("pages")) {
            moduleInfo.pages = parseModulePages(moduleJson, profileJsons);
        }
        // parse metadata
        if (moduleJson.containsKey("metadata")) {
            moduleInfo.moduleMetadataInfos = parseModuleMetadataInfos(moduleJson, data, profileJsons);
        }
        // parse shortcuts
        if (!moduleInfo.moduleMetadataInfos.isEmpty()) {
            moduleInfo.moduleShortcuts = parseModuleShortcut(moduleInfo.moduleMetadataInfos, data);
        }
        // parse distrofilter
        if (!moduleInfo.moduleMetadataInfos.isEmpty()) {
            moduleInfo.distroFilter = parseModuleDistrofilterFromMetadata(moduleInfo.moduleMetadataInfos);
        }
        // parse abilities
        if (moduleJson.containsKey("abilities")) {
            moduleInfo.abilities = parseModuleAbilities(moduleJson, data, profileJsons);
            for (ModuleAbilityInfo abilityInfo : moduleInfo.abilities) {
                moduleInfo.moduleShortcuts.addAll(parseModuleShortcut(abilityInfo.metadata, data));
            }
        }
        // parse extensionabilities
        if (moduleJson.containsKey("extensionAbilities")) {
            // parse service providerAbility
            String serviceProviderAbility = parseStageServiceProvider(moduleJson, moduleInfo.abilities);
            moduleInfo.extensionAbilityInfos = parseModuleExtensionAbilities(moduleJson, data, profileJsons);
            // parse abilityform
            moduleInfo.abilityFormInfos = parseModuleAbilityforms(moduleInfo.extensionAbilityInfos,
                    data, serviceProviderAbility);
            // parse commonEvent
            moduleInfo.commonEvents = parseModuleCommonEvents(moduleInfo.extensionAbilityInfos);
        }

        // parse request permission
        if (moduleJson.containsKey("requestPermissions")) {
            moduleInfo.requestPermissions = parseReqPermission(moduleJson, data);
        }
        // parse define permission
        if (moduleJson.containsKey("definePermissions")) {
            moduleInfo.definePermissions = parseDefinePermissions(moduleJson, data);
        }
        return moduleInfo;
    }

    /**
     * parse module pages
     *
     * @param moduleJson is json object of modulejson
     * @param profileJsons is the profile map
     * @return the pages result
     */
    static List<String> parseModulePages(JSONObject moduleJson, HashMap<String, String> profileJsons)  {
        List<String> pages = new ArrayList<>();
        String pageFile = getJsonString(moduleJson, "pages");
        pageFile = pageFile.replace(PROFILE, "");
        pageFile += ".json";
        String fileContent = profileJsons.get(pageFile);
        if (fileContent != null) {
            JSONObject pageObj = JSONObject.parseObject(fileContent);
            if (pageObj != null) {
                pages = JSONObject.parseArray(getJsonString(pageObj, "src"), String.class);
            }
        }
        return pages;
    }

    /**
     * parse module hap info
     *
     * @param moduleMetadataInfos metedata in moduleInfo
     * @return the parse result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static DistroFilter parseModuleDistrofilterFromMetadata(
            List<ModuleMetadataInfo> moduleMetadataInfos) throws BundleException {
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            String resource = moduleMetadataInfo.resource;
            if (resource.isEmpty()) {
                continue;
            }
            JSONObject distroFilter = JSONObject.parseObject(resource);
            if (distroFilter.containsKey("distroFilter")) {
                return JSONObject.parseObject(getJsonString(distroFilter, "distroFilter"), DistroFilter.class);
            }
        }
        return null;
    }

    /**
     * parse module hap info
     *
     * @param moduleJson is the json object of module object.
     * @param data is the resource of hap
     * @param profileJsons is the map of profile file name and file content
     * @return the parsed extensionAbilityInfos
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<ExtensionAbilityInfo> parseModuleExtensionAbilities(
            JSONObject moduleJson, byte[] data, HashMap<String, String> profileJsons) throws BundleException {
        List<ExtensionAbilityInfo> extensionAbilityInfos = new ArrayList<>();
        JSONArray extensionAbilities = moduleJson.getJSONArray("extensionAbilities");
        int size = extensionAbilities.size();
        for (int i = 0; i < size; ++i) {
            JSONObject tmpObj = extensionAbilities.getJSONObject(i);
            extensionAbilityInfos.add(parseModuleExtensionAbility(tmpObj, data, profileJsons));
        }
        return extensionAbilityInfos;
    }

    /**
     * parse module hap info
     *
     * @param extensionAbilityJson is the json object of extension ability
     * @param data is the resource of hap
     * @param profileJsons is the map of profile file name and file content
     * @return the parsed extensionAbilityInfo
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ExtensionAbilityInfo parseModuleExtensionAbility(JSONObject extensionAbilityJson, byte[] data,
                                                            HashMap<String, String> profileJsons) throws BundleException {
        if (extensionAbilityJson == null) {
            LOG.error("JsonUtil::parseModuleExtensionAbility exception: extensionAbilityJson is null");
            throw new BundleException("Parse ability failed, abilityJson is null");
        }
        ExtensionAbilityInfo moduleExtensionAbilityInfo = new ExtensionAbilityInfo();
        if (extensionAbilityJson.containsKey("name")) {
            moduleExtensionAbilityInfo.name = getJsonString(extensionAbilityJson, "name");
        }
        if (extensionAbilityJson.containsKey("srcEntrance")) {
            moduleExtensionAbilityInfo.srcEntrance = getJsonString(extensionAbilityJson, "srcEntrance");
        }
        moduleExtensionAbilityInfo.icon = parseIconById(extensionAbilityJson, data);
        moduleExtensionAbilityInfo.label =
                parseResourceByKey(extensionAbilityJson, data, "label", "labelId");
        moduleExtensionAbilityInfo.description =
                parseResourceByKey(extensionAbilityJson, data, "description", "descriptionId");
        if (extensionAbilityJson.containsKey("type")) {
            moduleExtensionAbilityInfo.type = getJsonString(extensionAbilityJson, "type");
        }
        if (extensionAbilityJson.containsKey("permissions")) {
            moduleExtensionAbilityInfo.permissions =
                    JSONObject.parseArray(getJsonString(extensionAbilityJson, "permissions"), String.class);
        }
        if (extensionAbilityJson.containsKey("uri")) {
            moduleExtensionAbilityInfo.uri = getJsonString(extensionAbilityJson, "uri");
        }
        if (extensionAbilityJson.containsKey("readPermission")) {
            moduleExtensionAbilityInfo.readPermission = getJsonString(extensionAbilityJson, "readPermission");
        }
        if (extensionAbilityJson.containsKey("writePermission")) {
            moduleExtensionAbilityInfo.writePermission = getJsonString(extensionAbilityJson, "writePermission");
        }
        if (extensionAbilityJson.containsKey("skills")) {
            moduleExtensionAbilityInfo.skills =
                    JSONObject.parseArray(getJsonString(extensionAbilityJson, "skills"), SkillInfo.class);
        }
        if (extensionAbilityJson.containsKey("metadata")) {
            moduleExtensionAbilityInfo.metadataInfos =
                    parseModuleMetadataInfos(extensionAbilityJson, data, profileJsons);
            // convert to metadata
            ModuleAdaption adaption = new ModuleAdaption();
            moduleExtensionAbilityInfo.metadata = adaption.convertToMetadata(moduleExtensionAbilityInfo.metadataInfos);
        }

        if (extensionAbilityJson.containsKey("visible")) {
            moduleExtensionAbilityInfo.visible = extensionAbilityJson.getBoolean("visible");
        }

        return moduleExtensionAbilityInfo;
    }

    /**
     * parse abilities info
     *
     * @param moduleJson Json hap json Object
     * @param data resource index data
     * @return the List<ModuleAbilityInfo> result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<ModuleAbilityInfo> parseModuleAbilities(JSONObject moduleJson, byte[] data,
                                                        HashMap<String, String> profileJsons) throws BundleException {
        List<ModuleAbilityInfo> abilities = new ArrayList<>();
        JSONArray abilityObjs = moduleJson.getJSONArray("abilities");
        int size = abilityObjs.size();
        for (int i = 0; i < size; ++i) {
            JSONObject tmpObj =abilityObjs.getJSONObject(i);
            abilities.add(parseModuleAbility(tmpObj, data, profileJsons));
        }
        return abilities;
    }

    /**
     * parse ability info
     *
     * @param abilityJson Json hap json Object
     * @param data resource index data
     * @return the ModuleAbilityInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleAbilityInfo parseModuleAbility(JSONObject abilityJson, byte[] data,
                                                HashMap<String, String> profileJsons) throws BundleException {
        if (abilityJson == null) {
            LOG.error("Uncompress::parseModuleAbility exception: abilityJson is null");
            throw new BundleException("Parse ability failed, abilityJson is null");
        }
        ModuleAbilityInfo moduleAbilityInfo = new ModuleAbilityInfo();
        if (abilityJson.containsKey("name")) {
            moduleAbilityInfo.name = getJsonString(abilityJson, "name");
        }
        if (abilityJson.containsKey("srcEntrance")) {
            moduleAbilityInfo.srcEntrance = getJsonString(abilityJson, "srcEntrance");
        }
        if (abilityJson.containsKey("launchType")) {
            moduleAbilityInfo.launchType = getJsonString(abilityJson, "launchType");
        }
        moduleAbilityInfo.description = parseResourceByKey(abilityJson, data, "description", "descriptionId");
        moduleAbilityInfo.icon = parseIconById(abilityJson, data);
        moduleAbilityInfo.label = parseResourceByKey(abilityJson, data, "label", "labelId");

        if (abilityJson.containsKey("permissions")) {
            moduleAbilityInfo.permissions = JSONObject.parseArray(getJsonString(abilityJson, "permissions"), String.class);
        }
        if (abilityJson.containsKey("metadata")) {
            moduleAbilityInfo.metadata = parseModuleMetadataInfos(abilityJson, data, profileJsons);
        }
        if (abilityJson.containsKey("visible")) {
            moduleAbilityInfo.visible = abilityJson.getBoolean("visible");
        }
        if (abilityJson.containsKey("continuable")) {
            moduleAbilityInfo.continuable = abilityJson.getBoolean("continuable");
        }
        if (abilityJson.containsKey("skills")) {
            moduleAbilityInfo.skills = JSON.parseArray(getJsonString(abilityJson, "skills"), SkillInfo.class);
        }

        if (abilityJson.containsKey("backgroundModes")) {
            moduleAbilityInfo.backgroundModes =
                    JSONArray.parseArray(getJsonString(abilityJson, "backgroundModes"), String.class);
        }
        return moduleAbilityInfo;
    }

    /**
     * parse metadata array
     *
     * @param jsonObject indicates json Object.
     * @param data resource index data.
     * @return the List<ModuleMetadataInfo> result.
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<ModuleMetadataInfo> parseModuleMetadataInfos(
            JSONObject jsonObject, byte[] data, HashMap<String, String> profileJsons) throws BundleException {
        List<ModuleMetadataInfo> moduleMetadataInfos = new ArrayList<>();
        JSONArray metadatas = jsonObject.getJSONArray("metadata");
        for (int i = 0; i < metadatas.size(); ++i) {
            JSONObject metadata = metadatas.getJSONObject(i);
            moduleMetadataInfos.add(parseModuleMetadata(metadata, data, profileJsons));
        }
        return moduleMetadataInfos;
    }

    /**
     * parse metadata info
     *
     * @param jsonObject Json hap json Object
     * @param data resource index data
     * @return the ModuleMetadataInfo result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static ModuleMetadataInfo parseModuleMetadata(JSONObject jsonObject, byte[] data,
                                                          HashMap<String, String> profileJson) throws BundleException {
        if (jsonObject == null) {
            LOG.error("JsonUitl::parseModuleMetadata exception: jsonObject is null");
            throw new BundleException("Parse ModuleMetadataInfo failed, jsonObject is null");
        }
        ModuleMetadataInfo moduleMetadataInfo = new ModuleMetadataInfo();
        if (jsonObject.containsKey("name")) {
            moduleMetadataInfo.name = getJsonString(jsonObject, "name");
        }
        if (jsonObject.containsKey("value")) {
            moduleMetadataInfo.value = getJsonString(jsonObject, "value");
        }
        if (jsonObject.containsKey("resource")) {
            moduleMetadataInfo.resource = getJsonString(jsonObject, "resource");
            String fileName = moduleMetadataInfo.resource;
            fileName = fileName.replace(PROFILE, "");
            fileName = fileName + ".json";
            moduleMetadataInfo.resource = profileJson.get(fileName);
        }
        return moduleMetadataInfo;
    }

    /**
     * parse abilityform in stage module.
     *
     * @param extensionAbilityInfos is extent ability in module object.
     * @param data is resource byte in hap.
     * @return the List<AbilityFormInfo> result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<AbilityFormInfo> parseModuleAbilityforms(List<ExtensionAbilityInfo> extensionAbilityInfos,
                                                         byte[] data, String serviceProviderAbility)
            throws BundleException {
        List<AbilityFormInfo> abilityFormInfos = new ArrayList<>();
        if (extensionAbilityInfos.isEmpty()) {
            return abilityFormInfos;
        }
        for (ExtensionAbilityInfo extensionAbilityInfo : extensionAbilityInfos) {
            List<AbilityFormInfo> formInfos =
                    parseModuleFormInfoInMetadata(data, extensionAbilityInfo.metadataInfos);
            if (FORM.equals(extensionAbilityInfo.type)) {
                for (AbilityFormInfo formInfo : formInfos) {
                    formInfo.providerAbility = serviceProviderAbility;
                }
            }
            abilityFormInfos.addAll(formInfos);
        }
        return abilityFormInfos;
    }

    /**
     * parse commonevents info
     *
     * @param extensionAbilityInfos is the list of extensionAbility.
     * @return the List<CommonEvent> result
     */
    static List<CommonEvent> parseModuleCommonEvents(List<ExtensionAbilityInfo> extensionAbilityInfos) {
        List<CommonEvent> allCommonEvent = new ArrayList<>();
        if (extensionAbilityInfos.isEmpty()) {
            return allCommonEvent;
        }
        for (ExtensionAbilityInfo extensionAbilityInfo : extensionAbilityInfos) {
            List<CommonEvent> commonEvents =
                    parseCommoneventsInMetadata(extensionAbilityInfo.metadataInfos);
            allCommonEvent.addAll(commonEvents);
        }
        return allCommonEvent;
    }

    /**
     * parse commonevents info
     *
     * @param moduleMetadataInfos is the list of ModuleMetadataInfo
     * @return the List<CommonEvent> result
     */
    static List<CommonEvent> parseCommoneventsInMetadata(List<ModuleMetadataInfo> moduleMetadataInfos) {
        List<CommonEvent> commonEvents = new ArrayList<>();
        // find common events and parse in metadata
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            String jsonStr = moduleMetadataInfo.resource;
            JSONObject jsonObj = JSONObject.parseObject(jsonStr);
            if (jsonObj != null && jsonObj.containsKey("commonEvents")) {
                commonEvents.addAll(parseModuleCommonEvents(jsonObj));
            }
        }
        return commonEvents;
    }

    /**
     * parse commonevents info
     *
     * @param jsonObject is the json  objects of commonevent.
     * @return the List<CommonEvent> result
     */
    static List<CommonEvent> parseModuleCommonEvents(JSONObject jsonObject) {
        List<CommonEvent> commonEvents = new ArrayList<>();
        JSONArray commonEventObjs = jsonObject.getJSONArray("commonEvents");
        for (int i = 0; i < commonEventObjs.size(); ++i) {
            JSONObject commonEventObj = commonEventObjs.getJSONObject(i);
            CommonEvent commonEvent = new CommonEvent();
            if (commonEventObj.containsKey("name")) {
                commonEvent.name = getJsonString(commonEventObj, "name");
            }
            if (commonEventObj.containsKey("permission")) {
                commonEvent.permission = getJsonString(commonEventObj, "permission");
            }
            if (commonEventObj.containsKey("types")) {
                commonEvent.type = JSON.parseArray(getJsonString(commonEventObj, "types"), String.class);
            }
            if (commonEventObj.containsKey("events")) {
                commonEvent.events = JSON.parseArray(getJsonString(commonEventObj, "events"), String.class);
            }
            commonEvents.add(commonEvent);
        }
        return commonEvents;
    }

    /**
     * parse stage shortcuts info
     *
     * @param moduleMetadataInfos is the list of ModuleMetadataInfo
     * @param data is resource byte in hap
     * @return the List<moduleShortcut> result
     * @throws BundleException Throws this exception when parse failed.
     */
    static List<ModuleShortcut> parseModuleShortcut(List<ModuleMetadataInfo> moduleMetadataInfos, byte[] data)
            throws BundleException {
        List<ModuleShortcut> shortcuts = new ArrayList<>();
        // find shortcut and parse in metadata
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            String jsonStr = moduleMetadataInfo.resource;
            if (jsonStr.isEmpty()) {
                continue;
            }
            JSONObject jsonObj = JSON.parseObject(jsonStr);
            if (jsonObj.containsKey("shortcuts")) {
                ModuleShortcut shortcut = new ModuleShortcut();
                JSONArray shortcutObjs = jsonObj.getJSONArray("shortcuts");
                for (int j = 0; j < shortcutObjs.size(); ++j) {
                    shortcuts.add(parseModuleShortcutObj(shortcutObjs.getJSONObject(j), data));
                }
            }
        }
        return shortcuts;
    }

    /**
     * parse stage shortcuts json object array
     *
     * @param shortcutObj is the objects of shortcut
     * @param data is resource byte in hap
     * @return the List<ModuleShortcuts> result
     * @throws BundleException Throws this exception when parse failed.
     */
    static ModuleShortcut parseModuleShortcutObj(JSONObject shortcutObj, byte[] data) throws BundleException {
        if (shortcutObj == null) {
            LOG.error("JsonUtil::parseModuleShortcutObj failed");
            throw new BundleException("JsonUtil::parseModuleShortcutObj failed");
        }
        ModuleShortcut moduleShortcut = new ModuleShortcut();
        if (shortcutObj.containsKey("shortcutId")) {
            moduleShortcut.setShortcutId(shortcutObj.getString("shortcutId"));
        }
        if (shortcutObj.containsKey("label")) {
            moduleShortcut.setLabel(parseResourceByStringID(data, getJsonString(shortcutObj, "label")));
        }
        if (shortcutObj.containsKey("icon")) {
            String iconPath = parseResourceByStringID(data, getJsonString(shortcutObj, "icon"));
            moduleShortcut.setIcon(iconPath.substring(iconPath.indexOf("resources")));
        }
        if (shortcutObj.containsKey("wants")) {
            moduleShortcut.setWants(JSON.parseArray(getJsonString(shortcutObj, "wants"), Want.class));
        }
        return moduleShortcut;
    }

    /**
     * parse fa shortcuts json object array
     *
     * @param moduleJson is the object of module
     * @param data is resource byte in hap
     * @return the List<ModuleShortcuts> result
     * @throws BundleException Throws this exception when parse failed.
     */
    static List<Shortcut> parseShoruCuts(JSONObject moduleJson, byte[] data) throws BundleException {
        List<Shortcut> shortcuts = new ArrayList<>();
        if (moduleJson.containsKey("shortcuts")) {
            JSONArray shortcutObjs = moduleJson.getJSONArray("shortcuts");
            for (int i =  0; i < shortcutObjs.size(); ++i) {
                shortcuts.add(parseShortObj(shortcutObjs.getJSONObject(i), data));
            }
        }
        return shortcuts;
    }

    /**
     * parse fa shortcuts json object array
     *
     * @param shortcutObj is the object of shortcut
     * @param data is resource byte in hap
     * @return the List<ModuleShortcuts> result
     * @throws BundleException Throws this exception when parse failed.
     */
    static Shortcut parseShortObj(JSONObject shortcutObj, byte[] data) throws BundleException {
        if (shortcutObj == null) {
            LOG.error("JsonUtil::parseModuleShortcutObj failed");
            throw new BundleException("JsonUtil::parseModuleShortcutObj failed");
        }
        Shortcut shortcut = new Shortcut();
        if (shortcutObj.containsKey("shortcutId")) {
            shortcut.shortcutId = shortcutObj.getString("shortcutId");
        }
        if (shortcutObj.containsKey("label")) {
            shortcut.label = parseResourceByKey(shortcutObj, data, "label", "labelId");
        }
        if (shortcutObj.containsKey("icon")) {
            shortcut.icon = parseIconById(shortcutObj, data);
        }
        if (shortcutObj.containsKey("intents")) {
            shortcut.intents = JSON.parseArray(getJsonString(shortcutObj, "intents"), IntentInfo.class);
        }
        return shortcut;
    }

    /**
     * parse form info
     *
     * @param data is resource byte in hap
     * @param moduleMetadataInfos is the list of ModuleMetadataInfo
     * @return the List<AbilityFormInfo> result
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<AbilityFormInfo> parseModuleFormInfoInMetadata(
            byte[] data, List<ModuleMetadataInfo> moduleMetadataInfos) throws BundleException {
        List<AbilityFormInfo> abilityFormInfos = new ArrayList<>();
        if (moduleMetadataInfos.isEmpty()) {
            return abilityFormInfos;
        }
        // find form json and parse in metadata
        for (ModuleMetadataInfo moduleMetadataInfo : moduleMetadataInfos) {
            String jsonStr = moduleMetadataInfo.resource;
            JSONObject jsonObj = JSONObject.parseObject(jsonStr);
            if (jsonObj!= null && jsonObj.containsKey("forms")) {
                JSONArray jsonForms = JSONObject.parseArray(getJsonString(jsonObj, "forms"));
                int size = jsonForms.size();
                for (int j = 0; j < size; ++j) {
                    JSONObject tmpObj = jsonForms.getJSONObject(j);
                    abilityFormInfos.add(parseModuleForm(tmpObj, data));
                }
            }
        }
        return abilityFormInfos;
    }

    /**
     * parse form object
     *
     * @param formObj is form json object
     * @param data is resource byte in hap
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static AbilityFormInfo parseModuleForm(JSONObject formObj, byte[] data) throws BundleException {
        if (formObj == null) {
            LOG.error("JsonUtil::parseModuleForm exception: formObj is null");
            throw new BundleException("Parse parseModuleForm failed, formObj is null");
        }
        AbilityFormInfo moduleFormInfo = new AbilityFormInfo();
        if (formObj.containsKey("name")) {
            moduleFormInfo.name = getJsonString(formObj, "name");
        }
        moduleFormInfo.description = parseFormDescription(formObj, data);
        if (formObj.containsKey("src")) {
            moduleFormInfo.src = getJsonString(formObj, "src");
        }
        if (formObj.containsKey("window")) {
            moduleFormInfo.windowInfo =
                    JSON.parseObject(getJsonString(formObj, "window"), AbilityFormInfo.ModuleWindowInfo.class);
        }
        if (formObj.containsKey("isDefault")) {
            moduleFormInfo.isDefault = formObj.getBoolean("isDefault");
        }
        if (formObj.containsKey("colorMode")) {
            moduleFormInfo.colorMode = getJsonString(formObj, "colorMode");
        }
        if (formObj.containsKey("supportDimensions")) {
            moduleFormInfo.supportDimensions =
                    JSONObject.parseArray(getJsonString(formObj, "supportDimensions"), String.class);
        }
        if (formObj.containsKey("defaultDimension")) {
            moduleFormInfo.defaultDimension = getJsonString(formObj, "defaultDimension");
        }
        if (formObj.containsKey("updateEnabled")) {
            moduleFormInfo.updateEnabled = formObj.getBoolean("updateEnabled");
        }
        if (formObj.containsKey("scheduledUpdateTime")) {
            moduleFormInfo.scheduledUpdateTime = getJsonString(formObj, "scheduledUpdateTime");
        }
        if (formObj.containsKey("updateDuration")) {
            moduleFormInfo.updateDuration = formObj.getIntValue("updateDuration");
        }
        if (formObj.containsKey("formConfigAbility")) {
            moduleFormInfo.formConfigAbility = getJsonString(formObj, "formConfigAbility");
        }
        if (formObj.containsKey("formVisibleNotify")) {
            moduleFormInfo.formVisibleNotify = formObj.getBoolean("formVisibleNotify");
        }
        return moduleFormInfo;
    }

    private static String parseFormDescription(JSONObject formObj, byte[] data) throws BundleException {
        if (formObj.containsKey("description")) {
            String descriptionStr = getJsonString(formObj, "description");
            if (descriptionStr.contains(STRING_RESOURCE)) {
                return parseResourceByStringID(data, descriptionStr);
            } else {
                return descriptionStr;
            }
        }
        return EMPTY;
    }

    /**
     * parse definepermission objects
     *
     * @param moduleJson is module json object
     * @param data is resource byte in hap
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<DefinePermission> parseDefinePermissions(JSONObject moduleJson, byte[] data) throws BundleException {
        List<DefinePermission> definePermissions = new ArrayList<>();
        if (moduleJson.containsKey("definePermissions")) {
            JSONArray definePermissionObjs = moduleJson.getJSONArray("definePermissions");
            for (int i = 0; i < definePermissionObjs.size(); ++i) {
                definePermissions.add(parseDefinePermission(definePermissionObjs.getJSONObject(i), data));
            }
        }
        return definePermissions;
    }

    /**
     * parse define permission objects
     *
     * @param definePermissionObj is def permission json object
     * @param data is resource byte in hap
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static DefinePermission parseDefinePermission(JSONObject definePermissionObj, byte[] data) throws BundleException {
        DefinePermission definePermission = new DefinePermission();
        definePermission.name = getJsonString(definePermissionObj, "name", definePermission.name);
        definePermission.grantMode = getJsonString(definePermissionObj, "grantMode", definePermission.grantMode);
        definePermission.availableLevel =
                getJsonString(definePermissionObj, "availableLevel", definePermission.availableLevel);
        if (definePermissionObj.containsKey("provisionEnable")) {
            definePermission.provisionEnable = definePermissionObj.getBoolean("provisionEnable");
        }
        if (definePermissionObj.containsKey("distributedSceneEnable")) {
            definePermission.distributedSceneEnable = definePermissionObj.getBoolean("distributedSceneEnable");
        }
        definePermission.label =
                parseResourceByKey(definePermissionObj, data, "label", "labelId");
        definePermission.description =
                parseResourceByKey(definePermissionObj, data, "description", "descriptionId");

        return definePermission;
    }

    /**
     * parse defpermission objects
     *
     * @param moduleJson is module json object
     * @param data is resource byte in hap
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static List<DefPermission> parseDefPermissions(JSONObject moduleJson, byte[] data) throws BundleException {
        List<DefPermission> defPermissions = new ArrayList<>();
        if (moduleJson.containsKey("defPermissions")) {
            JSONArray defPermissionObjs = moduleJson.getJSONArray("defPermissions");
            for (int i = 0; i < defPermissionObjs.size(); ++i) {
                defPermissions.add(parseDefPermission(defPermissionObjs.getJSONObject(i), data));
            }
        }
        return defPermissions;
    }

    /**
     * parse defpermission objects
     *
     * @param defPermissionObj is def permission json object
     * @param data is resource byte in hap
     * @throws BundleException Throws this exception if the json is not standard.
     */
    static DefPermission parseDefPermission(JSONObject defPermissionObj, byte[] data) throws BundleException {
        DefPermission defPermission = new DefPermission();
        if (defPermissionObj.containsKey("name")) {
            defPermission.name = getJsonString(defPermissionObj, "name");
        }
        if (defPermissionObj.containsKey("grantMode")) {
            defPermission.grantMode = getJsonString(defPermissionObj, "grantMode");
        }
        if (defPermissionObj.containsKey("availableScope")) {
            defPermission.availableScope =
                    JSONObject.parseArray(getJsonString(defPermissionObj, "availableScope"), String.class);
        }
        defPermission.label = parseResourceByKey(defPermissionObj, data, "label", "labelId");
        defPermission.description = parseResourceByKey(defPermissionObj, data, "description", "descriptionId");
        if (defPermissionObj.containsKey("group")) {
            defPermission.group = getJsonString(defPermissionObj, "group");
        }
        return defPermission;
    }

    /**
     * get resource from JSONObject by the id.
     *
     * @param data is resource byte in hap.
     * @param id is the resource id.
     * @return the result
     * @throws BundleException Throws this exception when parse failed.
     */
    static String parseResourceByStringID(byte[] data, String id) throws BundleException {
        String res = "";
        int index = 0;
        while(id.charAt(index) < '0' || id.charAt(index) > '9') {
            index++;
        }
        res = ResourcesParser.getBaseResourceById(Integer.parseInt(id.substring(index, id.length())), data);
        return res;
    }

    /**
     * get resource from JSONObject by the key.
     *
     * @param jsonObject uncompress json object.
     * @param data is resource byte in hap.
     * @param key is the index of json object.
     * @param keyId is the index id of resource.
     * @return the result
     * @throws BundleException Throws this exception when parse failed.
     */
    static String parseResourceByKey(JSONObject jsonObject, byte[] data, String key, String keyId)
            throws BundleException {
        String res = "";
        if (jsonObject.containsKey(keyId)) {
            int resId = jsonObject.getIntValue(keyId);
            res = ResourcesParser.getBaseResourceById(resId, data);
        }
        if (res != null && !res.isEmpty()) {
            return res;
        } else if (jsonObject.containsKey(key)) {
           return getJsonString(jsonObject, key);
        } else {
            return "";
        }
    }

    /**
     * get icon path from resource by the key.
     *
     * @param jsonObject uncompress json object.
     * @param data is resource byte in hap
     * @return the result
     * @throws BundleException Throws this exception when parse failed.
     */
    static String parseIconById(JSONObject jsonObject, byte[] data) throws BundleException {
        String iconPath = "";
        if (jsonObject.containsKey("iconId")) {
            int resId = jsonObject.getIntValue("iconId");
            iconPath = ResourcesParser.getBaseResourceById(resId, data);
            iconPath = iconPath.substring(iconPath.indexOf("resources"));
        }
        if (!iconPath.isEmpty()) {
            return iconPath;
        } else if (jsonObject.containsKey("icon")) {
           return getJsonString(jsonObject, "icon");
        } else {
            return "";
        }
    }

    /**
     * parse stage module reqPermissions.
     *
     * @param moduleJson module json object.
     * @param data is resource byte in hap.
     * @return the result
     */
    static List<ReqPermission> parseReqPermission(JSONObject moduleJson, byte[] data) throws BundleException {
        List<ReqPermission> reqPermissions = new ArrayList<>();
        JSONArray requestPermissionObjs = moduleJson.getJSONArray("requestPermissions");
        for (int i = 0; i < requestPermissionObjs.size(); ++i) {
            ReqPermission reqPermission = new ReqPermission();
            JSONObject requestPermission = requestPermissionObjs.getJSONObject(i);
            if (requestPermission.containsKey("name")) {
                reqPermission.name = getJsonString(requestPermission, "name");
            }
            if (requestPermission.containsKey("reasonId")) {
                reqPermission.reason = parseResourceByKey(requestPermission, data, "reason", "reasonId");
            }
            if (requestPermission.containsKey("usedScene"))  {
                reqPermission.usedScene = parseModuleUsedScene(requestPermission.getJSONObject("usedScene"));
            }
            reqPermissions.add(reqPermission);
        }
        return reqPermissions;
    }

    private static UsedScene parseModuleUsedScene(JSONObject usedSceneObj) {
        UsedScene usedScene = new UsedScene();
        if (usedSceneObj.containsKey(ABILITIES)) {
            usedScene.ability = JSON.parseArray(getJsonString(usedSceneObj, ABILITIES), String.class);
        }
        if (usedSceneObj.containsKey(WHEN)) {
            usedScene.when = getJsonString(usedSceneObj, WHEN);
        }
        return usedScene;
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

    /**
     * get the String from JSONObject by the key.
     *
     * @param jsonObject uncompress json object
     * @param key value key
     * @param defaultValue the default value
     * @return the result
     */
    private static String getJsonString(JSONObject jsonObject, String key, String defaultValue) {
        String value = defaultValue;
        if (jsonObject != null && jsonObject.containsKey(key)) {
            value = jsonObject.get(key).toString();
        }
        return value;
    }

    private static void setFAProviderAbility(JSONObject moduleJson, HapInfo hapInfo,
                                             List<AbilityInfo> abilityInfos) throws BundleException {
        if (abilityInfos.isEmpty()) {
            throw new BundleException("JsonUtil::setProviderAbility abilityInfo is empty!");
        }
        String serviceProviderAbility = parseFAServiceProviderAbility(moduleJson, abilityInfos);
        for (AbilityInfo abilityInfo : abilityInfos) {
            if (!abilityInfo.formInfos.isEmpty()) {
                if (SERVICE.equals(abilityInfo.type)) {
                    setProviderAbilityForForm(abilityInfo.formInfos, serviceProviderAbility);
                }
                if (PAGE.equals(abilityInfo.type)) {
                    setProviderAbilityForForm(abilityInfo.formInfos, abilityInfo.name);
                }
                hapInfo.formInfos.addAll(abilityInfo.formInfos);
            }
        }
    }

    private static void setProviderAbilityForForm(List<AbilityFormInfo> abilityFormInfos, String providerAbility) {
        for (AbilityFormInfo abilityFormInfo : abilityFormInfos) {
            abilityFormInfo.providerAbility = providerAbility;
        }
    }

    private static String parseStageServiceProvider(JSONObject moduleJson,
                                                    List<ModuleAbilityInfo> moduleAbilityInfos) throws BundleException {
        if (moduleJson.containsKey(MAIN_ELEMENT)) {
            return getJsonString(moduleJson, MAIN_ELEMENT);
        }
        if (!moduleAbilityInfos.isEmpty()) {
            for (ModuleAbilityInfo moduleAbilityInfo : moduleAbilityInfos) {
                if (isSystemHomeAbility(moduleAbilityInfo.skills)) {
                    return moduleAbilityInfo.name;
                }
            }
            return moduleAbilityInfos.get(0).name;
        }
        return "";
    }

    private static String parseFAServiceProviderAbility(JSONObject moduleJson,
                                                        List<AbilityInfo> abilityInfos) throws BundleException {
        if (abilityInfos.isEmpty()) {
            throw new BundleException("JsonUtil::parseServiceProviderAbility abilityInfos is empty!");
        }

        if (moduleJson.containsKey(MAIN_ABILITY)) {
            return getJsonString(moduleJson, MAIN_ABILITY);
        }
        for (AbilityInfo abilityInfo : abilityInfos) {
            if (isSystemHomeAbility(abilityInfo.skills)) {
                return abilityInfo.name;
            }
        }
        for (AbilityInfo abilityInfo : abilityInfos) {
            if (PAGE.equals(abilityInfo.type)) {
                return abilityInfo.name;
            }
        }
        return "";
    }

    private static boolean isSystemHomeAbility(List<SkillInfo> skills) {
        for (SkillInfo skillInfo : skills) {
            if (skillInfo.entities.contains(ENTITY_SYSTEM_HOME) && skillInfo.actions.contains(ACTION_SYSTEM_HOME)) {
                return true;
            }
        }
        return false;
    }
}
