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

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONException;

/**
 * Json Util.
 *
 */
public class JsonUtil {
    private static final String DEVICE_TYPE = "device-type";
    private static final String NAME = "name";
    private static final String VALUE = "value";
    private static final String RESOURCE = "resource";
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
    private static final String BUNDLENAME = "bundleName";
    private static final String VERSIONCODE = "versionCode";
    private static final String VERSIONNAME = "versionName";
    private static final String PATCH_VERSION_CODE = "patchVersionCode";
    private static final String PATCH_VERSION_NAME = "patchVersionName";
    private static final String ORIGINAL_MODULE_HASH = "originalModuleHash";
    private static final String MODULE = "module";
    private static final String DEVICE_TYPES = "deviceTypes";
    private static final String TYPE = "type";
    private static final String LABEL = "label";
    private static final String LABEL_ID = "labelId";
    private static final String DESCRIPTION = "description";
    private static final String DESCRIPTION_ID = "descriptionId";
    private static final String VERSION_CODE = "versionCode";
    private static final String VENDOR = "vendor";
    private static final String VERSION_NAME = "versionName";
    private static final String MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
    private static final String DEBUG = "debug";
    private static final String MIN_API_VERSION = "minAPIVersion";
    private static final String TARGET_API_VERSION = "targetAPIVersion";
    private static final String API_RELEASE_TYPE = "apiReleaseType";
    private static final String DISTRIBUTED_NOTIFICATION_ENABLED = "distributedNotificationEnabled";
    private static final String ENTITY_TYPE = "entityType";
    private static final String UNSPECIFIED = "unspecified";
    private static final String SRC_ENTRANCE = "srcEntrance";
    private static final String PROCESS = "process";
    private static final String PHONE = "phone";
    private static final String TABLET = "tablet";
    private static final String TV = "tv";
    private static final String WEARABLE = "wearable";
    private static final String AR = "ar";
    private static final String VR = "vr";
    private static final String CAR = "car";
    private static final String EARPHONES = "earphones";
    private static final String SPEAKER = "speaker";
    private static final String LINK_IOT = "linkIOT";
    private static final String ROUTER = "router";
    private static final String DELIVERY_WITH_INSTALL = "deliveryWithInstall";
    private static final String INSTALLATION_FREE = "installationFree";
    private static final String VIRTUAL_MACHINE = "virtualMachine";
    private static final String DEFAULT = "default";
    private static final String HML = "hml";
    private static final String UI_SYNTAX = "uiSyntax";
    private static final String PAGES = "pages";
    private static final String JSON_SUFFIX = ".json";
    private static final String META_DATA = "metadata";
    private static final String SRC = "src";
    private static final String EXTENSION_ABILITIES = "extensionAbilities";
    private static final String REQUEST_PERMISSIONS = "requestPermissions";
    private static final String REASON = "reason";
    private static final String REASON_ID = "reasonId";
    private static final String USED_SCENE = "usedScene";
    private static final String DEFINE_PERMISSIONS = "definePermissions";
    private static final String DISTRO_FILTER = "distroFilter";
    private static final String LAUNCH_TYPE = "launchType";
    private static final String STANDARD = "standard";
    private static final String PERMISSIONS = "permissions";
    private static final String READ_PERMISSION = "readPermission";
    private static final String WRITE_PERMISSION = "writePermission";
    private static final String VISIBLE = "visible";
    private static final String CONTINUABLE = "continuable";
    private static final String SKILLS = "skills";
    private static final String BACKGROUND_MODES = "backgroundModes";
    private static final String URI = "uri";
    private static final String FORMS = "forms";
    private static final String WINDOW = "window";
    private static final String IS_DEFAULT = "isDefault";
    private static final String COLOR_MODE = "colorMode";
    private static final String SUPPORT_DIMENSIONS = "supportDimensions";
    private static final String DEFAULT_DIMENSION = "defaultDimension";
    private static final String UPDATE_ENABLED = "updateEnabled";
    private static final String SCHEDULE_UPDATE_TIME = "scheduledUpdateTime";
    private static final String UPDATE_DURATION = "updateDuration";
    private static final String FROM_CONFIG_ABILITY = "formConfigAbility";
    private static final String FORM_VISIBLE_NOTIFY = "formVisibleNotify";
    private static final String AUTO = "auto";
    private static final String DOT = ".";

    private static final int DEFAULT_VERSION_CODE = -1;


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

    private static boolean parseShellVersionInfoToAppInfo(String packInfoJsonStr, AppInfo appInfo)
            throws BundleException {
        LOG.info("Uncompress::parseShellVersionInfoToAppInfo: begin");
        if (!appInfo.isMultiFrameworkBundle()) {
            LOG.info("Uncompress::parseShellVersionInfoToAppInfo: is not a multi framewok bundle.");
            return false;
        }
        try {
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
        } catch (JSONException msg) {
            LOG.error("parseShellVersionInfoToAppInfo exception");
            throw new BundleException("parseShellVersionInfoToAppInfo exception");
        }
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

        moduleAppInfo.bundleName = getJsonString(appJson, BUNDLENAME);
        moduleAppInfo.debug = getJsonBooleanValue(appJson, DEBUG, false);
        moduleAppInfo.icon = parseIconById(appJson, data);
        moduleAppInfo.label = parseResourceByKey(appJson, data, LABEL, LABEL_ID);
        moduleAppInfo.setLabels(parseResourceMapByKey(appJson, data, LABEL_ID));
        moduleAppInfo.description = parseResourceByKey(appJson, data, DESCRIPTION, DESCRIPTION_ID);
        moduleAppInfo.setDescriptions(parseResourceMapByKey(appJson, data, DESCRIPTION_ID));

        moduleAppInfo.vendor = getJsonString(appJson, VENDOR);
        moduleAppInfo.versionCode = getJsonIntValue(appJson, VERSION_CODE, DEFAULT_VERSION_CODE);
        moduleAppInfo.versionName = getJsonString(appJson, VERSION_NAME);

        if (appJson.containsKey(MIN_COMPATIBLE_VERSION_CODE)) {
            moduleAppInfo.minCompatibleVersionCode =
                    getJsonIntValue(appJson, MIN_COMPATIBLE_VERSION_CODE, DEFAULT_VERSION_CODE);
        } else {
            moduleAppInfo.minCompatibleVersionCode = getJsonIntValue(appJson, VERSION_CODE, DEFAULT_VERSION_CODE);
        }
        moduleAppInfo.minAPIVersion = getJsonIntValue(appJson, MIN_API_VERSION, DEFAULT_VERSION_CODE);
        moduleAppInfo.targetAPIVersion = getJsonIntValue(appJson, TARGET_API_VERSION, DEFAULT_VERSION_CODE);
        moduleAppInfo.apiReleaseType = getJsonString(appJson, API_RELEASE_TYPE);
        moduleAppInfo.distributedNotificationEnabled =
                getJsonBooleanValue(appJson, DISTRIBUTED_NOTIFICATION_ENABLED, false);
        moduleAppInfo.entityType = getJsonString(appJson, ENTITY_TYPE, UNSPECIFIED);
        // parse device type
        parseSpecifiedDeviceType(appJson, moduleAppInfo);
        return moduleAppInfo;
    }

    static void parseSpecifiedDeviceType(JSONObject appJson, ModuleAppInfo moduleAppInfo) throws BundleException {
        if (appJson == null) {
            LOG.error("JsonUtil::parseSpecifiedDeviceType exception: appJson is null");
            throw new BundleException("Parse app info failed, appJson is null");
        }
        parseDeviceType(appJson, moduleAppInfo, PHONE);
        parseDeviceType(appJson, moduleAppInfo, TABLET);
        parseDeviceType(appJson, moduleAppInfo, TV);
        parseDeviceType(appJson, moduleAppInfo, WEARABLE);
        parseDeviceType(appJson, moduleAppInfo, AR);
        parseDeviceType(appJson, moduleAppInfo, VR);
        parseDeviceType(appJson, moduleAppInfo, CAR);
        parseDeviceType(appJson, moduleAppInfo, EARPHONES);
        parseDeviceType(appJson, moduleAppInfo, SPEAKER);
        parseDeviceType(appJson, moduleAppInfo, LINK_IOT);
        parseDeviceType(appJson, moduleAppInfo, ROUTER);
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
            if (deviceObj.containsKey(MIN_API_VERSION)) {
                moduleDeviceType.minAPIVersion = deviceObj.getIntValue(MIN_API_VERSION);
            } else {
                moduleDeviceType.minAPIVersion = moduleAppInfo.minAPIVersion;
            }

            if (deviceObj.containsKey(DISTRIBUTED_NOTIFICATION_ENABLED)) {
                moduleDeviceType.distributedNotificationEnabled =
                        deviceObj.getBoolean(DISTRIBUTED_NOTIFICATION_ENABLED);
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
            adaptAbilityName(hapInfo.abilities, hapInfo.packageStr);
            setFAProviderAbility(hapJson, hapInfo, hapInfo.abilities);
        }

        if (hapJson.containsKey("distroFilter")) {
            hapInfo.distroFilter = JSONObject.parseObject(
                    getJsonString(hapJson, "distroFilter"), DistroFilter.class);
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
            ability.configChanges = JSONObject.parseArray(
                    getJsonString(abilityJson, "configChanges"), String.class);
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

    private static void adaptAbilityName(List<AbilityInfo> abilityInfos, String packageName) {
        if (abilityInfos == null) {
            return;
        }
        for (AbilityInfo abilityInfo : abilityInfos) {
            if (abilityInfo.name.startsWith(DOT)) {
                abilityInfo.name = packageName + abilityInfo.name;
            }
        }
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
        if (jsonObject == null || !jsonObject.containsKey(APP) || !jsonObject.containsKey(MODULE)) {
            LOG.error("JsonUtil::parseModuleProfileInfo exception: app or module is null");
            throw new BundleException("Parse module profile info failed, app or module is null");
        }

        JSONObject appJson = jsonObject.getJSONObject(APP);
        moduleProfileInfo.moduleAppInfo = parseModuleAppInfo(appJson, data);
        JSONObject moduleJson = jsonObject.getJSONObject(MODULE);
        moduleProfileInfo.moduleInfo = parseModuleHapInfo(moduleJson, data,
                moduleProfileInfo.moduleAppInfo.bundleName, profileJsons);

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

        moduleInfo.name = getJsonString(moduleJson, NAME);
        moduleInfo.type = getJsonString(moduleJson, TYPE);
        moduleInfo.srcEntrance = getJsonString(moduleJson, SRC_ENTRANCE);
        moduleInfo.description = parseResourceByKey(moduleJson, data, DESCRIPTION, DESCRIPTION_ID);
        moduleInfo.setDescriptions(parseResourceMapByKey(moduleJson, data, DESCRIPTION_ID));
        if (moduleJson.containsKey(PROCESS)) {
            moduleInfo.process = getJsonString(moduleJson, PROCESS);
        } else {
            moduleInfo.process = hapName;
        }
        moduleInfo.mainElement = getJsonString(moduleJson, MAIN_ELEMENT);
        if (moduleJson.containsKey(DEVICE_TYPES)) {
            moduleInfo.deviceTypes = JSONObject.parseArray(getJsonString(moduleJson, DEVICE_TYPES), String.class);
        }
        moduleInfo.deliveryWithInstall = getJsonBooleanValue(moduleJson, DELIVERY_WITH_INSTALL, true);
        parseInstallationFree(moduleJson, moduleInfo);
        moduleInfo.virtualMachine = getJsonString(moduleJson, VIRTUAL_MACHINE, DEFAULT);
        moduleInfo.uiSyntax = getJsonString(moduleJson, UI_SYNTAX, HML);
        moduleInfo.pages = parseModulePages(moduleJson, profileJsons);
        moduleInfo.moduleMetadataInfos = parseModuleMetadataInfos(moduleJson, data, profileJsons);
        // parse shortcuts and distroFilter
        if (!moduleInfo.moduleMetadataInfos.isEmpty()) {
            moduleInfo.moduleShortcuts = parseModuleShortcut(moduleInfo.moduleMetadataInfos, data);
            moduleInfo.distroFilter = parseModuleDistrofilterFromMetadata(moduleInfo.moduleMetadataInfos);
        }
        // parse abilities
        parseModuleAbilityAndAddShortcut(moduleJson, moduleInfo, data, profileJsons);
        // parse extensionAbilities
        parseStageExtensionAbility(moduleJson, moduleInfo, data, profileJsons);
        // parse request permission
        moduleInfo.requestPermissions = parseReqPermission(moduleJson, data);
        // parse define permission
        moduleInfo.definePermissions = parseDefinePermissions(moduleJson, data);
        return moduleInfo;
    }

    private static void parseInstallationFree(JSONObject moduleJson, ModuleInfo moduleInfo) {
        if (moduleJson.containsKey(INSTALLATION_FREE)) {
            boolean isFreeInstall = moduleJson.getBoolean(INSTALLATION_FREE);
            if (isFreeInstall) {
                moduleInfo.installationFree = 1;
            } else {
                moduleInfo.installationFree = 0;
            }
        } else {
            moduleInfo.installationFree = 2;
        }
    }

    /**
     * parse module pages
     *
     * @param moduleJson is json object of modulejson
     * @param profileJsons is the profile map
     * @return the pages result
     */
    static List<String> parseModulePages(
            JSONObject moduleJson, HashMap<String, String> profileJsons) throws BundleException {
        List<String> pages = new ArrayList<>();
        String pageFile = getJsonString(moduleJson, PAGES);
        if (pageFile.isEmpty()) {
            return pages;
        }
        pageFile = pageFile.replace(PROFILE, "");
        pageFile += JSON_SUFFIX;
        String fileContent = profileJsons.get(pageFile);
        if (fileContent != null) {
            JSONObject pageObj = JSONObject.parseObject(fileContent);
            if (pageObj != null) {
                pages = JSONObject.parseArray(getJsonString(pageObj, SRC), String.class);
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
            if (distroFilter.containsKey(DISTRO_FILTER)) {
                return JSONObject.parseObject(getJsonString(distroFilter, DISTRO_FILTER), DistroFilter.class);
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
        JSONArray extensionAbilities = moduleJson.getJSONArray(EXTENSION_ABILITIES);
        int size = extensionAbilities.size();
        for (int i = 0; i < size; ++i) {
            JSONObject tmpObj = extensionAbilities.getJSONObject(i);
            extensionAbilityInfos.add(parseModuleExtensionAbility(tmpObj, data, profileJsons));
        }
        return extensionAbilityInfos;
    }

    private static void parseStageExtensionAbility(JSONObject moduleJson, ModuleInfo moduleInfo, byte[] data,
                                                   HashMap<String, String> profileJsons) throws BundleException {
        if (!moduleJson.containsKey(EXTENSION_ABILITIES)) {
            return;
        }
        // parse service providerAbility
        String serviceProviderAbility = parseStageServiceProvider(moduleJson, moduleInfo.abilities);
        moduleInfo.extensionAbilityInfos = parseModuleExtensionAbilities(moduleJson, data, profileJsons);
        // parse abilityform
        moduleInfo.abilityFormInfos = parseModuleAbilityforms(moduleInfo.extensionAbilityInfos,
                data, serviceProviderAbility);
        // parse commonEvent
        moduleInfo.commonEvents = parseModuleCommonEvents(moduleInfo.extensionAbilityInfos);
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
        moduleExtensionAbilityInfo.name = getJsonString(extensionAbilityJson, NAME);
        moduleExtensionAbilityInfo.srcEntrance = getJsonString(extensionAbilityJson, SRC_ENTRANCE);
        moduleExtensionAbilityInfo.icon = parseIconById(extensionAbilityJson, data);
        moduleExtensionAbilityInfo.label =
                parseResourceByKey(extensionAbilityJson, data, LABEL, LABEL_ID);
        moduleExtensionAbilityInfo.setLabels(parseResourceMapByKey(extensionAbilityJson, data, LABEL_ID));
        moduleExtensionAbilityInfo.description =
                parseResourceByKey(extensionAbilityJson, data, DESCRIPTION, DESCRIPTION_ID);
        moduleExtensionAbilityInfo.setDescriptions(parseResourceMapByKey(extensionAbilityJson, data, DESCRIPTION_ID));
        moduleExtensionAbilityInfo.type = getJsonString(extensionAbilityJson, TYPE);

        if (extensionAbilityJson.containsKey(PERMISSIONS)) {
            moduleExtensionAbilityInfo.permissions =
                    JSONObject.parseArray(getJsonString(extensionAbilityJson, PERMISSIONS), String.class);
        }
        moduleExtensionAbilityInfo.uri = getJsonString(extensionAbilityJson, URI);
        moduleExtensionAbilityInfo.readPermission = getJsonString(extensionAbilityJson, READ_PERMISSION);
        moduleExtensionAbilityInfo.writePermission = getJsonString(extensionAbilityJson, WRITE_PERMISSION);
        if (extensionAbilityJson.containsKey(SKILLS)) {
            moduleExtensionAbilityInfo.skills =
                    JSONObject.parseArray(getJsonString(extensionAbilityJson, SKILLS), SkillInfo.class);
        }

        if (extensionAbilityJson.containsKey(META_DATA)) {
            moduleExtensionAbilityInfo.metadataInfos =
                    parseModuleMetadataInfos(extensionAbilityJson, data, profileJsons);
            // convert to metadata
            ModuleAdaption adaption = new ModuleAdaption();
            moduleExtensionAbilityInfo.metadata = adaption.convertToMetadata(moduleExtensionAbilityInfo.metadataInfos);
        }
        moduleExtensionAbilityInfo.visible = getJsonBooleanValue(extensionAbilityJson, VISIBLE, false);
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
        if (!moduleJson.containsKey(ABILITIES)) {
            return abilities;
        }
        JSONArray abilityObjs = moduleJson.getJSONArray(ABILITIES);
        int size = abilityObjs.size();
        for (int i = 0; i < size; ++i) {
            JSONObject tmpObj =abilityObjs.getJSONObject(i);
            abilities.add(parseModuleAbility(tmpObj, data, profileJsons));
        }
        return abilities;
    }

    private static void parseModuleAbilityAndAddShortcut(JSONObject moduleJson, ModuleInfo moduleInfo, byte[] data,
                                                         HashMap<String, String> profileJsons) throws BundleException {
        moduleInfo.abilities = parseModuleAbilities(moduleJson, data, profileJsons);
        for (ModuleAbilityInfo abilityInfo : moduleInfo.abilities) {
            moduleInfo.moduleShortcuts.addAll(parseModuleShortcut(abilityInfo.metadata, data));
        }
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
        moduleAbilityInfo.name = getJsonString(abilityJson, NAME);
        moduleAbilityInfo.srcEntrance = getJsonString(abilityJson, SRC_ENTRANCE);
        moduleAbilityInfo.launchType = getJsonString(abilityJson, LAUNCH_TYPE, STANDARD);
        moduleAbilityInfo.description = parseResourceByKey(abilityJson, data, DESCRIPTION, DESCRIPTION_ID);
        moduleAbilityInfo.setDescriptions(parseResourceMapByKey(abilityJson, data, DESCRIPTION_ID));

        moduleAbilityInfo.icon = parseIconById(abilityJson, data);
        moduleAbilityInfo.label = parseResourceByKey(abilityJson, data, LABEL, LABEL_ID);
        moduleAbilityInfo.setLabels(parseResourceMapByKey(abilityJson, data, LABEL_ID));
        if (abilityJson.containsKey(PERMISSIONS)) {
            moduleAbilityInfo.permissions = JSONObject.parseArray(
                getJsonString(abilityJson, PERMISSIONS), String.class);
        }
        moduleAbilityInfo.metadata = parseModuleMetadataInfos(abilityJson, data, profileJsons);
        moduleAbilityInfo.visible = getJsonBooleanValue(abilityJson, VISIBLE, false);
        moduleAbilityInfo.continuable = getJsonBooleanValue(abilityJson, CONTINUABLE, false);

        if (abilityJson.containsKey(SKILLS)) {
            moduleAbilityInfo.skills = JSON.parseArray(getJsonString(abilityJson, SKILLS), SkillInfo.class);
        }

        if (abilityJson.containsKey(BACKGROUND_MODES)) {
            moduleAbilityInfo.backgroundModes =
                    JSONArray.parseArray(getJsonString(abilityJson, BACKGROUND_MODES), String.class);
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
        if (!jsonObject.containsKey(META_DATA)) {
            return moduleMetadataInfos;
        }
        JSONArray metadatas = jsonObject.getJSONArray(META_DATA);
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
            LOG.error("JsonUtil::parseModuleMetadata exception: jsonObject is null");
            throw new BundleException("Parse ModuleMetadataInfo failed, jsonObject is null");
        }
        ModuleMetadataInfo moduleMetadataInfo = new ModuleMetadataInfo();
        moduleMetadataInfo.name = getJsonString(jsonObject, NAME);
        moduleMetadataInfo.value = getJsonString(jsonObject, VALUE);

        if (jsonObject.containsKey(RESOURCE)) {
            moduleMetadataInfo.resource = getJsonString(jsonObject, RESOURCE);
            String fileName = moduleMetadataInfo.resource;
            fileName = fileName.replace(PROFILE, "");
            fileName = fileName + JSON_SUFFIX;
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
            if (jsonObj != null && jsonObj.containsKey(FORMS)) {
                JSONArray jsonForms = JSONObject.parseArray(getJsonString(jsonObj, FORMS));
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
        moduleFormInfo.name = getJsonString(formObj, NAME);
        moduleFormInfo.description = parseFormDescription(formObj, data);
        moduleFormInfo.setDescriptions(parseFormDescriptions(formObj, data));
        moduleFormInfo.src = getJsonString(formObj, SRC);

        if (formObj.containsKey(WINDOW)) {
            moduleFormInfo.windowInfo =
                    JSON.parseObject(getJsonString(formObj, WINDOW), AbilityFormInfo.ModuleWindowInfo.class);
        }
        moduleFormInfo.isDefault = getJsonBooleanValue(formObj, IS_DEFAULT, false);
        moduleFormInfo.colorMode = getJsonString(formObj, COLOR_MODE, AUTO);
        if (formObj.containsKey(SUPPORT_DIMENSIONS)) {
            moduleFormInfo.supportDimensions =
                    JSONObject.parseArray(getJsonString(formObj, SUPPORT_DIMENSIONS), String.class);
        }
        moduleFormInfo.defaultDimension = getJsonString(formObj, DEFAULT_DIMENSION);
        moduleFormInfo.updateEnabled = getJsonBooleanValue(formObj, UPDATE_ENABLED, false);
        moduleFormInfo.scheduledUpdateTime = getJsonString(formObj, SCHEDULE_UPDATE_TIME);
        moduleFormInfo.updateDuration = getJsonIntValue(formObj, UPDATE_DURATION, 1);
        moduleFormInfo.formConfigAbility = getJsonString(formObj, FROM_CONFIG_ABILITY);
        moduleFormInfo.formVisibleNotify = getJsonBooleanValue(formObj, FORM_VISIBLE_NOTIFY, false);
        return moduleFormInfo;
    }

    private static String parseFormDescription(JSONObject formObj, byte[] data) throws BundleException {
        if (formObj.containsKey(DESCRIPTION)) {
            String descriptionStr = getJsonString(formObj, DESCRIPTION);
            if (descriptionStr.contains(STRING_RESOURCE)) {
                return parseResourceByStringID(data, descriptionStr);
            } else {
                return descriptionStr;
            }
        }
        return EMPTY;
    }

    private static HashMap<String, String> parseFormDescriptions(JSONObject formObj, byte[] data) {
        HashMap<String, String> descriptions = new HashMap<>();
        if (!formObj.containsKey(DESCRIPTION)) {
            return descriptions;
        }
        String descriptionStr = getJsonString(formObj, DESCRIPTION);
        if (descriptionStr.contains(STRING_RESOURCE)) {
            int len = STRING_RESOURCE.length();
            String descriptionId = descriptionStr.substring(len);
            try {
                int id = Integer.parseInt(descriptionId);
                descriptions = ResourcesParser.getResourceMapById(id, data);
            } catch (NumberFormatException e) {
                LOG.error("parseFormDescriptions failed: invalid descriptionId!");
            }
            return descriptions;
        }
        return descriptions;
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
        if (!moduleJson.containsKey(DEFINE_PERMISSIONS)) {
            return definePermissions;
        }
        JSONArray definePermissionObjs = moduleJson.getJSONArray(DEFINE_PERMISSIONS);
        for (int i = 0; i < definePermissionObjs.size(); ++i) {
            definePermissions.add(parseDefinePermission(definePermissionObjs.getJSONObject(i), data));
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
        try {
            int finalId = Integer.parseInt(id.substring(index));
            res = ResourcesParser.getResourceStringById(finalId, data);
        } catch (NumberFormatException e) {
            LOG.error("parseResourceByStringID failed: input invalid of " + id);
        }
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
     */
    static String parseResourceByKey(JSONObject jsonObject, byte[] data, String key, String keyId) {
        String res = "";
        if (jsonObject.containsKey(keyId)) {
            int resId = jsonObject.getIntValue(keyId);
            res = ResourcesParser.getResourceStringById(resId, data);
        }
        if (res != null && !res.isEmpty()) {
            return res;
        } else if (jsonObject.containsKey(key)) {
           return getJsonString(jsonObject, key);
        } else {
            return "";
        }
    }

    static HashMap<String, String> parseResourceMapByKey(JSONObject jsonObject, byte[] data, String keyId) {
        HashMap<String, String> map = new HashMap<>();
        if (jsonObject.containsKey(keyId)) {
            int resId = jsonObject.getIntValue(keyId);
            map = ResourcesParser.getResourceMapById(resId, data);
        }
        return map;
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
        if (!moduleJson.containsKey(REQUEST_PERMISSIONS)) {
            return reqPermissions;
        }
        JSONArray requestPermissionObjs = moduleJson.getJSONArray(REQUEST_PERMISSIONS);
        for (int i = 0; i < requestPermissionObjs.size(); ++i) {
            ReqPermission reqPermission = new ReqPermission();
            JSONObject requestPermission = requestPermissionObjs.getJSONObject(i);
            reqPermission.name = getJsonString(requestPermission, NAME);
            if (requestPermission.containsKey(REASON_ID)) {
                reqPermission.reason = parseResourceByKey(requestPermission, data, REASON, REASON_ID);
                reqPermission.setReasons(parseResourceMapByKey(requestPermission, data, REASON_ID));
            }
            if (requestPermission.containsKey(USED_SCENE)) {
                reqPermission.usedScene = parseModuleUsedScene(requestPermission.getJSONObject(USED_SCENE));
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

    private static int getJsonIntValue(JSONObject jsonObject, String key, int defaultValue) {
        int value = defaultValue;
        if (jsonObject != null && jsonObject.containsKey(key)) {
            value = jsonObject.getIntValue(key);
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
            if (abilityInfo.type.equals(PAGE)) {
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

    /**
     * parse patch.json form json string.
     *
     * @param jsonString is the file path of hqf file
     * @return HQFVerifyInfo
     */
    static HQFInfo parsePatch(String jsonString) throws BundleException {
        HQFInfo hqfVerifyInfo = new HQFInfo();
        JSONObject jsonObject = JSON.parseObject(jsonString);
        JSONObject appObj = jsonObject.getJSONObject(APP);
        if (appObj == null) {
            LOG.error("Error: parsePatch failed, input patch.json is invalid, patch.json has no app!");
            throw new BundleException("Error: parsePatch failed, input patch.json is invalid!");
        }

        if (appObj.containsKey(BUNDLENAME)) {
            hqfVerifyInfo.setBundleName(appObj.getString(BUNDLENAME));
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
}
