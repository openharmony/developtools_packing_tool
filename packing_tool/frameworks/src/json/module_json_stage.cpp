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

#include "module_json.h"

#include <fstream>

#include "log.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
    const std::string APP = "app";
    const std::string BUNDLE_TYPE = "bundleType";
    const std::string ABILITIES = "abilities";
    const std::string VERSIONCODE = "versionCode";
    const std::string VERSIONNAME = "versionName";
    const std::string MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
    const std::string API_VERSION = "apiVersion";
    const std::string MIN_API_VERSION = "minAPIVersion";
    const std::string TARGET_API_VERSION = "targetAPIVersion";
    const std::string API_RELEASE_TYPE = "apiReleaseType";
    const std::string DEBUG = "debug";
    const std::string COMPATIBLE = "compatible";
    const std::string RELEASE_TYPE = "releaseType";
    const std::string TARGET = "target";
    const std::string VERSION = "version";
    const std::string CODE = "code";
    const std::string NAME = "name";
    const std::string MODULE = "module";
    const std::string MODULE_NAME = "moduleName";
    const std::string MODULE_TYPE = "moduleType";
    const std::string DISTRO = "distro";
    const std::string PACKAGE = "package";
    const std::string BUNDLE_NAME = "bundleName";
    const std::string ENTRY = "entry";
    const std::string DEVICE_TYPE = "deviceType";
    const std::string DEVICE_TYPES = "deviceTypes";
    const std::string TYPE = "type";
    const std::string VENDOR = "vendor";
    const std::string METADATA = "metadata";
    const std::string RESOURCE = "resource";
    const std::string PROFILE = "$profile:";
    const std::string VALUE = "value";
    const std::string JSON_PREFIX = ".json";
    const std::string DISTRO_FILTER = "distroFilter";
    const std::string DISTRIBUTION_FILTER = "distributionFilter";
    const std::string DEPENDENCIES = "dependencies";
    const std::string EXTENSION_ABILITIES = "extensionAbilities";
    const std::string INSTALLATION_FREE = "installationFree";
    const std::string COMPRESS_NATIVE_LIBS = "compressNativeLibs";
    const std::string ASAN_ENABLED = "asanEnabled";
    const std::string TSAN_ENABLED = "tsanEnabled";
    const std::string ATOMIC_SERVICE = "atomicService";
    const std::string PRELOADS = "preloads";
    const std::string SHARED = "shared";
    const std::string APP_SERVICE = "appService";
    const std::string REQUEST_PERMISSIONS = "requestPermissions";
    const std::string TARGET_MODULE_NAME = "targetModuleName";
    const std::string TARGET_PRIORITY = "targetPriority";
    const std::string TARGET_BUNDLE_NAME = "targetBundleName";
    const std::string DEVICE_CONFIG = "deviceConfig";
    const std::string DEFAULT = "default";
    const std::string COMPILE_SDK_VERSION = "compileSdkVersion";
    const std::string COMPILE_SDK_TYPE = "compileSdkType";
    const std::string PROXY_DATAS = "proxyDatas";
    const std::string PROXY_DATA = "proxyData";
    const std::string PROXY_URI = "uri";
    const std::string CONTINUE_TYPE = "continueType";
    const std::string MULTI_APP_MODE = "multiAppMode";
    const std::string MULTI_APP_MODE_TYPE = "multiAppModeType";
    const std::string MULTI_APP_MODE_NUMBER = "maxCount";
    const std::string GENERATE_BUILD_HASH = "generateBuildHash";
    const std::string BUILD_HASH = "buildHash";
}

bool ModuleJson::SetStageVersionCode(const int32_t& versionCode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!appObj->Contains(VERSIONCODE.c_str())) {
        LOGE("App node has no %s node!", VERSIONCODE.c_str());
        return false;
    }
    if (appObj->SetInt(VERSIONCODE.c_str(), versionCode) != Result::SUCCESS) {
        LOGE("App node set %s failed!", VERSIONCODE.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageVersionName(const std::string& versionName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!appObj->Contains(VERSIONNAME.c_str())) {
        LOGE("App node has no %s node!", VERSIONNAME.c_str());
        return false;
    }
    if (appObj->SetString(VERSIONNAME.c_str(), versionName) != Result::SUCCESS) {
        LOGE("App node set %s failed!", VERSIONNAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetStageVersion(Version& version)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageVersionByAppObj(appObj, version);
}

bool ModuleJson::GetStageVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(VERSIONCODE.c_str()) || !appObj->Contains(VERSIONNAME.c_str())) {
        LOGE("App node has no %s node or %s node", VERSIONCODE.c_str(), VERSIONNAME.c_str());
        return false;
    }
    if (appObj->GetInt(VERSIONCODE.c_str(), &version.versionCode) != Result::SUCCESS) {
        LOGE("App node get %s failed!", VERSIONCODE.c_str());
        return false;
    }
    if (appObj->GetString(VERSIONNAME.c_str(), &version.versionName) != Result::SUCCESS) {
        LOGE("App node get %s failed!", VERSIONNAME.c_str());
        return false;
    }
    if (appObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (appObj->GetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(),
            &version.minCompatibleVersionCode) != Result::SUCCESS) {
            LOGE("App node get %s failed!", MIN_COMPATIBLE_VERSION_CODE.c_str());
            return false;
        }
    } else {
        version.minCompatibleVersionCode = version.versionCode;
    }
    return true;
}

bool ModuleJson::GetStageInstallationFree(bool& installationFree)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
}

bool ModuleJson::GetStageInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    installationFree = false;
    if (moduleObj->Contains(INSTALLATION_FREE.c_str())) {
        if (moduleObj->GetBool(INSTALLATION_FREE.c_str(), &installationFree) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", INSTALLATION_FREE.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageModuleApiVersion(ModuleApiVersion& moduleApiVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion);
}

bool ModuleJson::GetStageModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(MIN_API_VERSION.c_str())) {
        if (appObj->GetInt(MIN_API_VERSION.c_str(), &moduleApiVersion.compatibleApiVersion) != Result::SUCCESS) {
            LOGE("App node get %s failed!", MIN_API_VERSION.c_str());
            return false;
        }
    }
    if (appObj->Contains(TARGET_API_VERSION.c_str())) {
        if (appObj->GetInt(TARGET_API_VERSION.c_str(), &moduleApiVersion.targetApiVersion) != Result::SUCCESS) {
            LOGE("App node get %s failed!", TARGET_API_VERSION.c_str());
            return false;
        }
    }
    if (appObj->Contains(API_RELEASE_TYPE.c_str())) {
        if (appObj->GetString(API_RELEASE_TYPE.c_str(), &moduleApiVersion.releaseType) != Result::SUCCESS) {
            LOGE("App node get %s failed!", API_RELEASE_TYPE.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageModuleName(std::string& stageModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetStageModuleNameByModuleObj(moduleObj, stageModuleName);
}

bool ModuleJson::GetStageModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& stageModuleName)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("Module node has no %s node!", NAME.c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &stageModuleName) != Result::SUCCESS) {
        LOGE("Module node get %s failed!", NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetStageCompileSdkType(std::string& compileSdkType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageCompileSdkTypeByAppObj(appObj, compileSdkType);
}

bool ModuleJson::GetStageCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(COMPILE_SDK_TYPE.c_str())) {
        if (appObj->GetString(COMPILE_SDK_TYPE.c_str(), &compileSdkType) != Result::SUCCESS) {
            LOGE("App node get %s failed!", COMPILE_SDK_TYPE.c_str());
            return false;
        }
    } else {
        compileSdkType = "";
    }
    return true;
}

bool ModuleJson::GetStageCompileSdkVersion(std::string& compileSdkVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageCompileSdkVersionByAppObj(appObj, compileSdkVersion);
}

bool ModuleJson::GetStageCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(COMPILE_SDK_VERSION.c_str())) {
        if (appObj->GetString(COMPILE_SDK_VERSION.c_str(), &compileSdkVersion) != Result::SUCCESS) {
            LOGE("App node get %s failed!", COMPILE_SDK_VERSION.c_str());
            return false;
        }
    } else {
        compileSdkVersion = "";
    }
    return true;
}

bool ModuleJson::GetStageDebug(bool& debug)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageDebugByAppObj(appObj, debug);
}

bool ModuleJson::GetStageDebugByAppObj(std::unique_ptr<PtJson>& appObj, bool& debug)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(DEBUG.c_str())) {
        if (appObj->GetBool(DEBUG.c_str(), &debug) != Result::SUCCESS) {
            LOGE("App node get %s failed!", DEBUG.c_str());
            return false;
        }
    } else {
        debug = false;
    }
    return true;
}

// java: parseStageEntry / getDeviceTypesFromStageModule
bool ModuleJson::GetStageEntry(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    std::string moduleType;
    if (!moduleObj->Contains(TYPE.c_str())) {
        LOGE("Module node has no %s node!", TYPE.c_str());
        return false;
    }
    if (moduleObj->GetString(TYPE.c_str(), &moduleType) != Result::SUCCESS) {
        LOGE("Module node get %s failed!", TYPE.c_str());
        return false;
    }
    if (moduleType.compare(ENTRY) == 0) {
        if (!GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
            LOGE("GetStageDeviceTypesByModuleObj failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageDeviceTypes(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes);
}

bool ModuleJson::GetStageDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<std::string>& deviceTypes)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPES.c_str())) {
        LOGE("Module node has no %s node!", DEVICE_TYPES.c_str());
        return false;
    }
    std::unique_ptr<PtJson> deviceTypeObj;
    if (moduleObj->GetArray(DEVICE_TYPES.c_str(), &deviceTypeObj) != Result::SUCCESS) {
        LOGE("Module node get %s array node failed!", DEVICE_TYPES.c_str());
        return false;
    }
    for (int32_t i = 0; i < deviceTypeObj->GetSize(); i++) {
        deviceTypes.push_back(deviceTypeObj->Get(i)->GetString());
    }
    return true;
}

bool ModuleJson::GetStageDistroFilter(DistroFilter& distroFilter)
{
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    if (!GetModuleMetadatas(moduleMetadataInfos)) {
        LOGE("GetModuleMetadatas failed!");
        return false;
    }
    return ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter);
}

bool ModuleJson::GetStageDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj, DistroFilter& distroFilter)
{
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    if (!GetModuleMetadatasByModuleObj(moduleObj, moduleMetadataInfos)) {
        LOGE("GetModuleMetadatasByModuleObj failed!");
        return false;
    }
    return ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter);
}

bool ModuleJson::GetStageModuleType(std::string& moduleType)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetStageModuleTypeByModuleObj(moduleObj, moduleType);
}

bool ModuleJson::GetStageModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(TYPE.c_str())) {
        if (moduleObj->GetString(TYPE.c_str(), &moduleType) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", TYPE.c_str());
            return false;
        }
    } else {
        moduleType = "";
    }
    return true;
}

bool ModuleJson::GetStageBundleType(std::string& bundleType)
{
    std::unique_ptr<PtJson> appObj;
    std::unique_ptr<PtJson> moduleObj;
    if (!GetAppObject(appObj) || !GetModuleObject(moduleObj)) {
        LOGE("GetAppObject or module node failed!");
        return false;
    }
    if (!moduleObj->Contains(TYPE.c_str())) {
        LOGE("Module node has no %s node!", TYPE.c_str());
        return false;
    }
    std::string moduleName;
    std::string moduleType;
    if (!GetStageModuleNameByModuleObj(moduleObj, moduleName) ||
        !GetStageModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("GetStageModuleNameByModuleObj or GetStageModuleTypeByModuleObj failed!");
        return false;
    }
    bool installationFree = false;
    GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (installationFree) {
            LOGE("The app.json5 file configuration does not match the installationFree: "
                 "true settings. Add the bundleType field to the app.json5 file and set it atomicService.");
            return false;
        }
        bundleType = APP;
        return true;
    } else if (appObj->GetString(BUNDLE_TYPE.c_str(), &bundleType) == Result::SUCCESS) {
        return CheckStageBundleType(moduleName, moduleType, bundleType, installationFree);
    }
    return false;
}

bool ModuleJson::GetStageAsanEnabled(bool& asanEnabled)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageAsanEnabledByAppObj(appObj, asanEnabled);
}

bool ModuleJson::GetStageAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(ASAN_ENABLED.c_str())) {
        if (appObj->GetBool(ASAN_ENABLED.c_str(), &asanEnabled) != Result::SUCCESS) {
            LOGE("App node get %s failed!", ASAN_ENABLED.c_str());
            return false;
        }
    } else {
        asanEnabled = false;
    }
    return true;
}

bool ModuleJson::GetStageTsanEnabled(bool& tsanEnabled)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageTsanEnabledByAppObj(appObj, tsanEnabled);
}

bool ModuleJson::GetStageTsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& tsanEnabled)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(TSAN_ENABLED.c_str())) {
        if (appObj->GetBool(TSAN_ENABLED.c_str(), &tsanEnabled) != Result::SUCCESS) {
            LOGE("App node get %s failed!", TSAN_ENABLED.c_str());
            return false;
        }
    } else {
        tsanEnabled = false;
    }
    return true;
}

bool ModuleJson::GetStageCompressNativeLibs(bool& compressNativeLibs)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageCompressNativeLibsByAppObj(appObj, compressNativeLibs);
}

bool ModuleJson::GetStageCompressNativeLibsByAppObj(std::unique_ptr<PtJson>& appObj, bool& compressNativeLibs)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(COMPRESS_NATIVE_LIBS.c_str())) {
        if (appObj->GetBool(COMPRESS_NATIVE_LIBS.c_str(), &compressNativeLibs) != Result::SUCCESS) {
            LOGE("App node get %s failed!", COMPRESS_NATIVE_LIBS.c_str());
            return false;
        }
    } else {
        compressNativeLibs = false;
    }
    return true;
}

// apiReleaseType was included in ModuleApiVersion.
bool ModuleJson::GetStageApiReleaseType(std::string& apiReleaseType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetStageApiReleaseTypeByAppObj(appObj, apiReleaseType);
}

bool ModuleJson::GetStageApiReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& apiReleaseType)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(API_RELEASE_TYPE.c_str())) {
        if (appObj->GetString(API_RELEASE_TYPE.c_str(), &apiReleaseType) != Result::SUCCESS) {
            LOGE("App node get %s failed!", API_RELEASE_TYPE.c_str());
            return false;
        }
    } else {
        apiReleaseType = "";
    }
    return true;
}

// java Compressor.java : parseStageHapVerifyInfo / ModuleJsonUtil.java : parseStageHapVerifyInfo
// The following parameters require additional settings
// SetStageModule: stage is true, fa is false
// SetFileLength
// SetResourceMap
// SetProfileStr: this param can be delete, because it is also parsed by MODULE_JSON file
bool ModuleJson::GetStageHapVerifyInfo(HapVerifyInfo& hapVerifyInfo)
{
    std::unique_ptr<PtJson> appObj;
    std::unique_ptr<PtJson> moduleObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    std::string bundleName;
    std::string bundleType;
    std::list<DependencyItem> dependencyItems;
    if (!GetBundleNameByAppObj(appObj, bundleName)) {
        LOGE("GetBundleNameByAppObj failed!");
        return false;
    }
    if (!GetStageBundleType(bundleType)) {
        LOGE("GetStageBundleType failed!");
        return false;
    }
    if (!GetDependencyItemsByModuleObj(moduleObj, dependencyItems, bundleName)) {
        LOGE("GetDependencyItemsByModuleObj failed!");
        return false;
    }
    hapVerifyInfo.SetBundleName(bundleName);
    hapVerifyInfo.SetBundleType(bundleType);
    hapVerifyInfo.SetDependencyItemList(dependencyItems);
    if (!SetStageHapVerifyInfoByAppObj(appObj, hapVerifyInfo)) {
        LOGE("SetStageHapVerifyInfoByAppObj failed!");
        return false;
    }
    if (!SetStageHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo)) {
        LOGE("SetStageHapVerifyInfoByModuleObj failed!");
        return false;
    }
    return true;
}

bool ModuleJson::SetStageHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    std::string vendor;
    Version version;
    ModuleApiVersion moduleApiVersion;
    std::string targetBundleName;
    int32_t targetPriority = 0;
    bool debug = false;
    if (!GetVendorByAppObj(appObj, vendor)) {
        LOGE("GetVendorByAppObj failed!");
        return false;
    }
    if (!GetStageVersionByAppObj(appObj, version)) {
        LOGE("GetStageVersionByAppObj failed!");
        return false;
    }
    if (!GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion)) {
        LOGE("GetStageModuleApiVersionByAppObj failed!");
        return false;
    }
    if (!GetTargetBundleNameByAppObj(appObj, targetBundleName)) {
        LOGE("GetTargetBundleNameByAppObj failed!");
        return false;
    }
    if (!GetTargetPriorityByAppObj(appObj, targetPriority)) {
        LOGE("GetTargetPriorityByAppObj failed!");
        return false;
    }
    if (!GetStageDebugByAppObj(appObj, debug)) {
        LOGE("GetStageDebugByAppObj failed!");
        return false;
    }
    hapVerifyInfo.SetVendor(vendor);
    hapVerifyInfo.SetVersion(version);
    hapVerifyInfo.SetApiVersion(moduleApiVersion);
    hapVerifyInfo.SetTargetBundleName(targetBundleName);
    hapVerifyInfo.SetTargetPriority(targetPriority);
    hapVerifyInfo.SetDebug(debug);
    return true;
}

bool ModuleJson::SetStageHapVerifyInfoByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    std::string moduleName;
    std::list<std::string> deviceTypes;
    std::string moduleType;
    bool installationFree = false;
    std::string targetModuleName;
    int32_t targetModulePriority = 0;
    std::string compileSdkType;
    std::string compileSdkVersion;
    if (!GetStageModuleNameByModuleObj(moduleObj, moduleName)) {
        LOGE("GetStageModuleNameByModuleObj failed!");
        return false;
    }
    if (!GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
        LOGE("GetStageDeviceTypesByModuleObj failed!");
        return false;
    }
    if (!GetStageModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("GetStageModuleTypeByModuleObj failed!");
        return false;
    }
    if (!GetStageInstallationFreeByModuleObj(moduleObj, installationFree)) {
        LOGE("GetStageInstallationFreeByModuleObj failed!");
        return false;
    }
    if (!GetTargetModuleNameByModuleObj(moduleObj, targetModuleName) ||
        !GetTargetModulePriorityByModuleObj(moduleObj, targetModulePriority)) {
        LOGE("GetTargetModuleNameByModuleObj or GetTargetModulePriorityByModuleObj failed!");
        return false;
    }
    if (!GetStageCompileSdkTypeByAppObj(moduleObj, compileSdkType) ||
        !GetStageCompileSdkVersionByAppObj(moduleObj, compileSdkVersion)) {
        LOGE("GetStageCompileSdkTypeByAppObj or GetStageCompileSdkVersionByAppObj failed!");
        return false;
    }
    hapVerifyInfo.SetModuleName(moduleName);
    hapVerifyInfo.SetDeviceTypes(deviceTypes);
    hapVerifyInfo.SetModuleType(moduleType);
    hapVerifyInfo.SetInstallationFree(installationFree);
    hapVerifyInfo.SetTargetModuleName(targetModuleName);
    hapVerifyInfo.SetTargetModulePriority(targetModulePriority);
    hapVerifyInfo.SetCompileSdkType(compileSdkType);
    hapVerifyInfo.SetCompileSdkVersion(compileSdkVersion);
    return true;
}

bool ModuleJson::SetStageHapVerifyInfoExtByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    DistroFilter distroFilter;
    std::list<std::string> abilityNames;
    std::list<std::string> extensionAbilityNames;
    std::list<PreloadItem> preloadItems;
    std::list<std::string> proxyDataUris;
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    MultiAppMode multiAppMode;
    if (!GetStageDistroFilterByModuleObj(moduleObj, distroFilter)) {
        LOGE("GetStageDistroFilterByModuleObj failed!");
        return false;
    }
    if (!GetAbilityNamesByModuleObj(moduleObj, abilityNames)) {
        LOGE("GetAbilityNamesByModuleObj failed!");
        return false;
    }
    if (!GetExtensionAbilityNamesByModuleObj(moduleObj, extensionAbilityNames)) {
        LOGE("GetExtensionAbilityNamesByModuleObj failed!");
        return false;
    }
    if (!GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems)) {
        LOGE("GetAtomicServicePreloadsByModuleObj failed!");
        return false;
    }
    if (!GetProxyDataUrisByModuleObj(moduleObj, proxyDataUris)) {
        LOGE("GetProxyDataUrisByModuleObj failed!");
        return false;
    }
    if (!GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap)) {
        LOGE("GetAbilityContinueTypeMapByModuleObj failed!");
        return false;
    }
    if (!GetMultiAppModeByAppObj(moduleObj, multiAppMode)) {
        LOGE("GetMultiAppModeByAppObj failed!");
        return false;
    }
    hapVerifyInfo.SetDistroFilter(distroFilter);
    hapVerifyInfo.SetAbilityNames(abilityNames);
    hapVerifyInfo.AddAbilityNames(extensionAbilityNames);
    hapVerifyInfo.SetPreloadItems(preloadItems);
    hapVerifyInfo.SetProxyDataUris(proxyDataUris);
    hapVerifyInfo.SetContinueTypeMap(abilityContinueTypeMap);
    hapVerifyInfo.SetMultiAppMode(multiAppMode);
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
