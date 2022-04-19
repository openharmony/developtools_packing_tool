package ohos;

import ohos.utils.fastjson.JSONObject;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;

class ModuleAdaption {

    private static final Log LOG = new Log(ModuleAdaption.class.toString());
    /**
     * adapte moduleResult to uncompressResult function.
     *
     * @param moduleResult for module.json result
     */
    UncomperssResult convertToUncompressResult(ModuleResult moduleResult) {
        UncomperssResult uncomperssResult = new UncomperssResult();
        uncomperssResult.setResult(moduleResult.getResult());
        uncomperssResult.setMessage(moduleResult.getMessage());
        // adapt profileInfos
        for (int i = 0; i < moduleResult.moduleProfileInfos.size(); ++i) {
            ModuleProfileInfo moduleProfileInfo = moduleResult.moduleProfileInfos.get(i);
            ProfileInfo profileInfo = new ProfileInfo();
            // adapt appInfo
            profileInfo.appInfo = new AppInfo();
            if (!convertToAppInfo(profileInfo.appInfo, moduleProfileInfo.moduleAppInfo)) {
                LOG.error("convertToAppInfo failed");
                uncomperssResult.setResult(false);
                uncomperssResult.setMessage("convertToAppInfo failed");
                return uncomperssResult;
            }
            //adapt deviceconfig
            if (!convertToDeviceConfig(profileInfo.deviceConfig,
                    moduleProfileInfo.moduleAppInfo.deviceTypes, moduleProfileInfo.moduleAppInfo)) {
                LOG.error("convertToDeviceConfig failed");
                uncomperssResult.setResult(false);
                uncomperssResult.setMessage("convertToDeviceConfig failed");
                return uncomperssResult;
            }
            // adapt hapInfo
            profileInfo.hapInfo = new HapInfo();
            if(!convertToHapInfo(profileInfo.hapInfo, moduleProfileInfo.moduleInfo)) {
                LOG.error("convertToHapInfo failed");
                uncomperssResult.setResult(false);
                uncomperssResult.setMessage("convertToHapInfo failed");
                return uncomperssResult;
            }
            uncomperssResult.addProfileInfo(profileInfo);
        }
        return uncomperssResult;
    }

    boolean convertToAppInfo(AppInfo appInfo, ModuleAppInfo moduleAppInfo) {
        if (moduleAppInfo == null) {
            LOG.error("convertToAppInfo failed: moduleAppInfo is null");
            return false;
        }
        appInfo.bundleName = moduleAppInfo.bundleName;
        appInfo.debug = moduleAppInfo.debug;
        appInfo.icon = moduleAppInfo.icon;
        appInfo.label = moduleAppInfo.label;
        appInfo.description = moduleAppInfo.description;
        appInfo.vendor = moduleAppInfo.vendor;
        appInfo.versionCode = String.valueOf(moduleAppInfo.versionCode);
        appInfo.versionName = moduleAppInfo.versionName;
        appInfo.minCompatibleVersionCode = moduleAppInfo.minCompatibleVersionCode;
        appInfo.compatibleApiVersion = moduleAppInfo.minAPIVersion;
        appInfo.targetApiVersion = moduleAppInfo.targetAPIVersion;
        appInfo.releaseType = moduleAppInfo.apiReleaseType;
        appInfo.distributedNotificationEnabled = moduleAppInfo.distributedNotificationEnabled;
        appInfo.entityType = moduleAppInfo.entityType;
        return true;
    }

    boolean convertToDeviceConfig(Map<String, DeviceConfig> deviceConfigMap,
                           Map<String, ModuleDeviceType> moduleDeviceTypeMap, ModuleAppInfo moduleAppInfo) {
        if (moduleAppInfo == null) {
            LOG.error("convertToDeviceConfig failed: moduleAppInfo is null");
            return false;
        }
        for(Map.Entry<String, ModuleDeviceType> entrty : moduleDeviceTypeMap.entrySet()) {
            DeviceConfig deviceConfig = new DeviceConfig();
            deviceConfig.minAPIVersion = entrty.getValue().minAPIVersion;
            deviceConfig.distributedNotificationEnabled = entrty.getValue().distributedNotificationEnabled;
            deviceConfigMap.put(entrty.getKey(), deviceConfig);
        }
        return true;
    }

    boolean convertToHapInfo(HapInfo hapInfo, ModuleInfo moduleInfo) {
        if (moduleInfo == null) {
            LOG.error("convertToHapInfo failed: moduleInfo is null");
            return false;
        }
        hapInfo.name = moduleInfo.name;
        hapInfo.distro = new Distro();
        hapInfo.distro.moduleName = moduleInfo.name;
        hapInfo.distro.moduleType = moduleInfo.type;
        hapInfo.distro.installationFree = moduleInfo.installationFree;
        hapInfo.distro.virtualMachine = moduleInfo.virtualMachine;
        hapInfo.distro.deliveryWithInstall = moduleInfo.deliveryWithInstall;
        hapInfo.srcEntrance = moduleInfo.srcEntrance;
        hapInfo.description = moduleInfo.description;
        hapInfo.process = moduleInfo.process;
        hapInfo.mainElement = moduleInfo.mainElement;
        hapInfo.deviceType = moduleInfo.deviceTypes;
        hapInfo.uiSyntax = moduleInfo.uiSyntax;
        // adapt pages
        hapInfo.pages = moduleInfo.pages;
        // adapt metadata
        hapInfo.moduleMetadataInfos = moduleInfo.moduleMetadataInfos;
        // adapt abilities
        convertToAbilityInfo(hapInfo.abilities, moduleInfo.abilities);
        // adapt extension abilities to abilities
        hapInfo.extensionAbilityInfos = moduleInfo.extensionAbilityInfos;
        // adapt request permissions
        hapInfo.reqPermissions = moduleInfo.requestPermissions;
        // adapt distrofilter
        hapInfo.distroFilter = moduleInfo.distroFilter;
        // adapt definePermissions
        convertToDefinePermissions(hapInfo.defPermissions, moduleInfo.definePermissions);
        return true;
    }

    void convertToAbilityInfo(List<AbilityInfo> abilityInfos, List<ModuleAbilityInfo> moduleAbilityInfos) {
        if (moduleAbilityInfos.isEmpty()) {
            return;
        }
        for (int i = 0; i < moduleAbilityInfos.size(); ++i) {
            AbilityInfo abilityInfo = new AbilityInfo();
            ModuleAbilityInfo moduleAbilityInfo = moduleAbilityInfos.get(i);
            abilityInfo.name = moduleAbilityInfo.name;
            abilityInfo.srcEntrance = moduleAbilityInfo.srcEntrance;
            abilityInfo.description = moduleAbilityInfo.description;
            abilityInfo.icon = moduleAbilityInfo.icon;
            abilityInfo.iconPath = moduleAbilityInfo.icon;
            abilityInfo.label = moduleAbilityInfo.label;
            abilityInfo.labelRes = moduleAbilityInfo.label;
            abilityInfo.permissions = moduleAbilityInfo.permissions;
            abilityInfo.visible = moduleAbilityInfo.visible;
            abilityInfo.continuable = moduleAbilityInfo.continuable;
            // adapt skillInfo
            abilityInfo.skills = moduleAbilityInfo.skills;
            // adapt configchanges
            abilityInfo.backgroundModes = moduleAbilityInfo.backgroundModes;
            abilityInfos.add(abilityInfo);
        }
    }

    void convertToDefinePermissions(List<DefPermission> defPermissions,
                                   List<ModuleDefinePermissions> moduleDefinePermissions) {
        if (moduleDefinePermissions.isEmpty()) {
            return;
        }
        for (int i = 0; i < moduleDefinePermissions.size(); ++i) {
            ModuleDefinePermissions moduleDefinePermission = moduleDefinePermissions.get(i);
            DefPermission defPermission = new DefPermission();
            defPermission.name = moduleDefinePermission.name;
            defPermission.grantMode = moduleDefinePermission.grantMode;
            defPermission.availableScope = moduleDefinePermission.availableLevel;
            defPermission.provisionEnabled = moduleDefinePermission.provisionEnabled;
            defPermission.distributedSceneEnable = moduleDefinePermission.distributedSceneEnable;
            defPermission.label = moduleDefinePermission.label;
            defPermission.description = moduleDefinePermission.description;
            defPermissions.add(defPermission);
        }
    }

}
