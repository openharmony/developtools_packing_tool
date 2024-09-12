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

bool ModuleJson::GetFaVersion(Version& version)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaVersionByAppObj(appObj, version);
}

bool ModuleJson::GetFaVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(VERSION.c_str())) {
        LOGE("App node has no %s node!", VERSION.c_str());
        return false;
    }
    std::unique_ptr<PtJson> versionObj;
    if (appObj->GetObject(VERSION.c_str(), &versionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", VERSION.c_str());
        return false;
    }
    return GetFaVersionByVersionObj(versionObj, version);
}

bool ModuleJson::GetFaVersionByVersionObj(std::unique_ptr<PtJson>& versionObj, Version& version)
{
    if (!versionObj) {
        LOGE("Version node is null!");
        return false;
    }
    if (!versionObj->Contains(CODE.c_str()) || !versionObj->Contains(NAME.c_str())) {
        LOGE("Version node has no %s node or %s node", CODE.c_str(), NAME.c_str());
        return false;
    }
    if (versionObj->GetInt(CODE.c_str(), &version.versionCode) != Result::SUCCESS) {
        LOGE("Version node get %s failed!", CODE.c_str());
        return false;
    }
    if (versionObj->GetString(NAME.c_str(), &version.versionName) != Result::SUCCESS) {
        LOGE("Version node get %s failed!", NAME.c_str());
        return false;
    }
    if (versionObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (versionObj->GetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(),
            &version.minCompatibleVersionCode) != Result::SUCCESS) {
            LOGE("Version node get %s failed!", MIN_COMPATIBLE_VERSION_CODE.c_str());
            return false;
        }
    } else {
        version.minCompatibleVersionCode = version.versionCode;
    }
    return true;
}

bool ModuleJson::SetFaVersionCode(const int32_t& versionCode)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        LOGE("GetVersionObject failed!");
        return false;
    }
    if (!versionObj->Contains(CODE.c_str())) {
        LOGE("Version node has no %s node!", CODE.c_str());
        return false;
    }
    if (versionObj->SetInt(CODE.c_str(), versionCode) != Result::SUCCESS) {
        LOGE("Version node set %s failed!", CODE.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaVersionName(const std::string& versionName)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        LOGE("GetVersionObject failed!");
        return false;
    }
    if (!versionObj->Contains(NAME.c_str())) {
        LOGE("Version node has no %s node!", NAME.c_str());
        return false;
    }
    if (versionObj->SetString(NAME.c_str(), versionName) != Result::SUCCESS) {
        LOGE("Version node set %s failed!", NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaInstallationFree(bool& installationFree)
{
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObject(distroObj)) {
        LOGE("GetDistroObject failed!");
        return false;
    }
    return GetFaInstallationFreeByDistroObj(distroObj, installationFree);
}

bool ModuleJson::GetFaInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("GetDistroObjectByModuleObj failed!");
        return false;
    }
    return GetFaInstallationFreeByDistroObj(distroObj, installationFree);
}

bool ModuleJson::GetFaInstallationFreeByDistroObj(std::unique_ptr<PtJson>& distroObj, bool& installationFree)
{
    if (!distroObj) {
        LOGE("Distro node is null!");
        return false;
    }
    installationFree = false;
    if (distroObj->Contains(INSTALLATION_FREE.c_str())) {
        if (distroObj->GetBool(INSTALLATION_FREE.c_str(), &installationFree) != Result::SUCCESS) {
            LOGE("Distro node get %s failed!", INSTALLATION_FREE.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaBundleType(std::string& bundleType)
{
    bool installationFree = false;
    if (!GetFaInstallationFree(installationFree)) {
        return false;
    }
    if (installationFree) {
        bundleType = ATOMIC_SERVICE;
    } else {
        bundleType = APP;
    }
    return true;
}

bool ModuleJson::GetFaModuleApiVersion(ModuleApiVersion& moduleApiVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaModuleApiVersionByAppObj(appObj, moduleApiVersion);
}

bool ModuleJson::GetFaModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("App node has no %s node!", API_VERSION.c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", API_VERSION.c_str());
        return false;
    }
    return GetFaModuleApiVersionByApiVersionObj(apiVersionObj, moduleApiVersion);
}

bool ModuleJson::GetFaModuleApiVersionByApiVersionObj(
    std::unique_ptr<PtJson>& apiVersionObj, ModuleApiVersion& moduleApiVersion)
{
    if (!apiVersionObj) {
        LOGE("ApiVersion node is null!");
        return false;
    }
    if (apiVersionObj->Contains(COMPATIBLE.c_str())) {
        if (apiVersionObj->GetInt(COMPATIBLE.c_str(), &moduleApiVersion.compatibleApiVersion) != Result::SUCCESS) {
            LOGE("ApiVersion node get %s node failed!", COMPATIBLE.c_str());
            return false;
        }
    }
    if (apiVersionObj->Contains(TARGET.c_str())) {
        if (apiVersionObj->GetInt(TARGET.c_str(), &moduleApiVersion.targetApiVersion) != Result::SUCCESS) {
            LOGE("ApiVersion node get %s node failed!", TARGET.c_str());
            return false;
        }
    }
    if (apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
        if (apiVersionObj->GetString(RELEASE_TYPE.c_str(), &moduleApiVersion.releaseType) != Result::SUCCESS) {
            LOGE("ApiVersion node get %s node failed!", RELEASE_TYPE.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaModuleName(std::string& faModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("GetDistroObjectByModuleObj failed!");
        return false;
    }
    if (!distroObj->Contains(MODULE_NAME.c_str())) {
        LOGE("Distro node has no %s node!", MODULE_NAME.c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_NAME.c_str(), &faModuleName) != Result::SUCCESS) {
        LOGE("Distro node get %s node failed!", MODULE_NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& faModuleName)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("GetDistroObjectByModuleObj failed!");
        return false;
    }
    return GetFaModuleNameByDistroObj(distroObj, faModuleName);
}

bool ModuleJson::GetFaModuleNameByDistroObj(std::unique_ptr<PtJson>& distroObj, std::string& faModuleName)
{
    if (!distroObj) {
        LOGE("Distro node is null!");
        return false;
    }
    if (!distroObj->Contains(MODULE_NAME.c_str())) {
        LOGE("Distro node has no %s node!", MODULE_NAME.c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_NAME.c_str(), &faModuleName) != Result::SUCCESS) {
        LOGE("Distro node get %s node failed!", MODULE_NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaPackageStr(std::string& packageStr)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetFaPackageStrByModuleObj(moduleObj, packageStr);
}

bool ModuleJson::GetFaPackageStrByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& packageStr)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(PACKAGE.c_str())) {
        LOGE("Module node has no %s node!", PACKAGE.c_str());
        return false;
    }
    if (moduleObj->GetString(PACKAGE.c_str(), &packageStr) != Result::SUCCESS) {
        LOGE("Module node get %s failed!", PACKAGE.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaCompileSdkType(std::string& compileSdkType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaCompileSdkTypeByAppObj(appObj, compileSdkType);
}

bool ModuleJson::GetFaCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("App node has no %s node!", API_VERSION.c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", API_VERSION.c_str());
        return false;
    }
    if (apiVersionObj->Contains(COMPILE_SDK_TYPE.c_str())) {
        if (apiVersionObj->GetString(COMPILE_SDK_TYPE.c_str(), &compileSdkType) != Result::SUCCESS) {
            LOGE("ApiVersion node get %s failed!", COMPILE_SDK_TYPE.c_str());
            return false;
        }
    } else {
        compileSdkType = "";
    }
    return true;
}

bool ModuleJson::GetFaCompileSdkVersion(std::string& compileSdkVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaCompileSdkVersionByAppObj(appObj, compileSdkVersion);
}

bool ModuleJson::GetFaCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("App node has no %s node!", API_VERSION.c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", API_VERSION.c_str());
        return false;
    }
    if (apiVersionObj->Contains(COMPILE_SDK_VERSION.c_str())) {
        if (apiVersionObj->GetString(COMPILE_SDK_VERSION.c_str(), &compileSdkVersion) != Result::SUCCESS) {
            LOGE("ApiVersion node get %s failed!", COMPILE_SDK_VERSION.c_str());
            return false;
        }
    } else {
        compileSdkVersion = "";
    }
    return true;
}

bool ModuleJson::GetFaDebug(bool& debug)
{
    std::unique_ptr<PtJson> deviceConfigObj;
    if (!GetDeviceConfigObject(deviceConfigObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaDebugByDeviceConfigObj(deviceConfigObj, debug);
}

bool ModuleJson::GetFaDebugByDeviceConfigObj(std::unique_ptr<PtJson>& deviceConfigObj, bool& debug)
{
    if (!deviceConfigObj) {
        LOGE("DeviceConfig node is null!");
        return false;
    }
    if (!deviceConfigObj->Contains(DEFAULT.c_str())) {
        LOGE("DeviceConfig node has no %s node!", DEFAULT.c_str());
        return false;
    }
    std::unique_ptr<PtJson> defaultObj;
    if (deviceConfigObj->GetObject(DEFAULT.c_str(), &defaultObj) != Result::SUCCESS) {
        LOGE("DeviceConfig node get %s failed!", DEFAULT.c_str());
        return false;
    }
    if (defaultObj->Contains(DEBUG.c_str())) {
        if (defaultObj->GetBool(DEBUG.c_str(), &debug) != Result::SUCCESS) {
            LOGE("debug node get %s failed!", DEBUG.c_str());
            return false;
        }
    }
    return true;
}

// java: parseFaEntry / getDeviceTypeFromFAModule
// parseFaEntry not called anywhere
bool ModuleJson::GetFaEntry(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!moduleObj->Contains(DISTRO.c_str())) {
        LOGE("Module node has no %s node!", DISTRO.c_str());
        return false;
    }
    if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
        LOGE("Module node get %s node failed!", DISTRO.c_str());
        return false;
    }
    std::string moduleType;
    if (!distroObj->Contains(MODULE_TYPE.c_str())) {
        LOGE("Distro node has no %s node!", MODULE_TYPE.c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_TYPE.c_str(), &moduleType) != Result::SUCCESS) {
        LOGE("Distro node get %s failed!", MODULE_TYPE.c_str());
        return false;
    }
    if (moduleType.compare(ENTRY) == 0) {
        if (!GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
            LOGE("GetFaDeviceTypesByModuleObj failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaDeviceTypes(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes);
}

bool ModuleJson::GetFaDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPE.c_str())) {
        LOGE("Module node has no %s node!", DEVICE_TYPE.c_str());
        return false;
    }
    std::unique_ptr<PtJson> deviceTypeObj;
    if (moduleObj->GetArray(DEVICE_TYPE.c_str(), &deviceTypeObj) != Result::SUCCESS) {
        LOGE("Module node get %s array node failed!", DEVICE_TYPE.c_str());
        return false;
    }
    for (int32_t i = 0; i < deviceTypeObj->GetSize(); i++) {
        deviceTypes.push_back(deviceTypeObj->Get(i)->GetString());
    }
    return true;
}

bool ModuleJson::GetFaModuleType(std::string& moduleType)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetFaModuleTypeByModuleObj(moduleObj, moduleType);
}

// java : parseFAIsEntry
bool ModuleJson::GetFaModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    moduleType = "";
    if (moduleObj->Contains(DISTRO.c_str())) {
        std::unique_ptr<PtJson> distroObj;
        if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
            LOGE("Module node get %s node failed!", DISTRO.c_str());
            return false;
        }
        if (distroObj->Contains(MODULE_TYPE.c_str())) {
            if (distroObj->GetString(MODULE_TYPE.c_str(), &moduleType) != Result::SUCCESS) {
                LOGE("Module node get %s failed!", MODULE_TYPE.c_str());
                return false;
            }
        }
    }
    return true;
}

bool ModuleJson::GetFaReleaseType(std::string& releaseType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaReleaseTypeByAppObj(appObj, releaseType);
}

bool ModuleJson::GetFaReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& releaseType)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    releaseType = "";
    if (appObj->Contains(API_VERSION.c_str())) {
        if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
            LOGE("App node get %s node failed!", API_VERSION.c_str());
            return false;
        }
        if (apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
            if (apiVersionObj->GetString(RELEASE_TYPE.c_str(), &releaseType) != Result::SUCCESS) {
                LOGE("App node get %s failed!", RELEASE_TYPE.c_str());
                return false;
            }
        }
    }
    return true;
}

bool ModuleJson::GetFaAsanEnabled(bool& asanEnabled)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetFaAsanEnabledByAppObj(appObj, asanEnabled);
}

bool ModuleJson::GetFaAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled)
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

bool ModuleJson::GetFaDistroFilter(DistroFilter& distroFilter)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetFaDistroFilterByModuleObj(moduleObj, distroFilter);
}

bool ModuleJson::GetFaDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj, DistroFilter& distroFilter)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(DISTRO_FILTER.c_str())) {
        std::string distroFilterStr;
        if (moduleObj->GetString(DISTRO_FILTER.c_str(), &distroFilterStr) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", DISTRO_FILTER.c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroFilterJsonObj = PtJson::Parse(distroFilterStr);
        if (!distroFilterJsonObj) {
            LOGE("Parse distro filter string failed!");
            return false;
        }
        std::unique_ptr<PtJson> distroFilterObj;
        if (distroFilterJsonObj->Contains(DISTRO_FILTER.c_str())) {
            if (distroFilterJsonObj->GetObject(DISTRO_FILTER.c_str(), &distroFilterObj) != Result::SUCCESS) {
                LOGE("DistroFilter node get %s failed!", DISTRO_FILTER.c_str());
                return false;
            }
        }
        if (!distroFilter.ParseFromJson(distroFilterObj)) {
            LOGE("Parse distro filter failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaHapVerifyInfo(HapVerifyInfo& hapVerifyInfo)
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
    bool debug = false;
    if (!GetBundleNameByAppObj(appObj, bundleName)) {
        LOGE("GetBundleNameByAppObj failed!");
        return false;
    }
    if (!GetFaBundleType(bundleType)) {
        LOGE("GetFaBundleType failed!");
        return false;
    }
    if (!GetDependencyItemsByModuleObj(moduleObj, dependencyItems, bundleName)) {
        LOGE("GetDependencyItemsByModuleObj failed!");
        return false;
    }
    GetFaDebug(debug);
    hapVerifyInfo.SetBundleName(bundleName);
    hapVerifyInfo.SetBundleType(bundleType);
    hapVerifyInfo.SetDependencyItemList(dependencyItems);
    hapVerifyInfo.SetDebug(debug);
    if (!SetFaHapVerifyInfoByAppObj(appObj, hapVerifyInfo)) {
        LOGE("SetFaHapVerifyInfoByAppObj failed!");
        return false;
    }
    if (!SetFaHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo)) {
        LOGE("SetFaHapVerifyInfoByModuleObj failed!");
        return false;
    }
    return true;
}

bool ModuleJson::SetFaHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    std::string vendor;
    Version version;
    ModuleApiVersion moduleApiVersion;
    std::string compileSdkType;
    std::string compileSdkVersion;
    if (!GetVendorByAppObj(appObj, vendor)) {
        LOGE("GetVendorByAppObj failed!");
        return false;
    }
    if (!GetFaVersionByAppObj(appObj, version)) {
        LOGE("GetFaVersionByAppObj failed!");
        return false;
    }
    if (!GetFaModuleApiVersionByAppObj(appObj, moduleApiVersion)) {
        LOGE("GetFaModuleApiVersionByAppObj failed!");
        return false;
    }
    if (!GetFaCompileSdkTypeByAppObj(appObj, compileSdkType)) {
        LOGE("GetFaCompileSdkTypeByAppObj failed!");
        return false;
    }
    if (!GetFaCompileSdkVersionByAppObj(appObj, compileSdkVersion)) {
        LOGE("GetFaCompileSdkVersionByAppObj failed!");
        return false;
    }
    hapVerifyInfo.SetVendor(vendor);
    hapVerifyInfo.SetVersion(version);
    hapVerifyInfo.SetApiVersion(moduleApiVersion);
    hapVerifyInfo.SetCompileSdkType(compileSdkType);
    hapVerifyInfo.SetCompileSdkVersion(compileSdkVersion);
    return true;
}

bool ModuleJson::SetFaHapVerifyInfoByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    std::string moduleName;
    DistroFilter distroFilter;
    std::list<std::string> deviceTypes;
    std::list<std::string> abilityNames;
    std::string moduleType;
    std::string packageStr;
    bool installationFree = false;
    if (!GetFaModuleNameByModuleObj(moduleObj, moduleName)) {
        LOGE("GetFaModuleNameByModuleObj failed!");
        return false;
    }
    if (!GetFaDistroFilterByModuleObj(moduleObj, distroFilter)) {
        LOGE("GetFaDistroFilterByModuleObj failed!");
        return false;
    }
    if (!GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
        LOGE("GetFaDeviceTypesByModuleObj failed!");
        return false;
    }
    if (!GetAbilityNamesByModuleObj(moduleObj, abilityNames)) {
        LOGE("GetAbilityNamesByModuleObj failed!");
        return false;
    }
    if (!GetFaModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("GetFaModuleTypeByModuleObj failed!");
        return false;
    }
    if (!GetFaPackageStrByModuleObj(moduleObj, packageStr)) {
        LOGE("GetFaPackageStrByModuleObj failed!");
        return false;
    }
    if (!GetFaInstallationFreeByModuleObj(moduleObj, installationFree)) {
        LOGE("GetFaInstallationFreeByModuleObj failed!");
        return false;
    }
    hapVerifyInfo.SetModuleName(moduleName);
    hapVerifyInfo.SetDistroFilter(distroFilter);
    hapVerifyInfo.SetDeviceTypes(deviceTypes);
    hapVerifyInfo.SetAbilityNames(abilityNames);
    hapVerifyInfo.SetModuleType(moduleType);
    hapVerifyInfo.SetPackageName(packageStr);
    hapVerifyInfo.SetInstallationFree(installationFree);
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
