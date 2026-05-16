/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include "error/packing_tool_err_msg.h"
#include "log.h"
#include "utils.h"

using packing_tool::error::PackingToolErrMsg;

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
const std::string DELIVERY_WITH_INSTALL = "deliveryWithInstall";
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaVersionByAppObj(appObj, version);
}

bool ModuleJson::GetFaVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSION + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> versionObj;
    if (appObj->GetObject(VERSION.c_str(), &versionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSION + " node failed!").c_str());
        return false;
    }
    return GetFaVersionByVersionObj(versionObj, version);
}

bool ModuleJson::GetFaVersionByVersionObj(std::unique_ptr<PtJson>& versionObj, Version& version)
{
    if (!versionObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Version node is null!").c_str());
        return false;
    }
    if (!versionObj->Contains(CODE.c_str()) || !versionObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + CODE + " node or " + NAME + " node").c_str());
        return false;
    }
    if (versionObj->GetInt(CODE.c_str(), &version.versionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node get ") + CODE + " failed!").c_str());
        return false;
    }
    if (versionObj->GetString(NAME.c_str(), &version.versionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node get ") + NAME + " failed!").c_str());
        return false;
    }
    if (versionObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (versionObj->GetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(),
            &version.minCompatibleVersionCode) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Version node get ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(CODE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + CODE + " node!").c_str());
        return false;
    }
    if (versionObj->SetInt(CODE.c_str(), versionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaVersionName(const std::string& versionName)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + NAME + " node!").c_str());
        return false;
    }
    if (versionObj->SetString(NAME.c_str(), versionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaInstallationFree(bool& installationFree)
{
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObject(distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
        return false;
    }
    return GetFaInstallationFreeByDistroObj(distroObj, installationFree);
}

bool ModuleJson::GetFaInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetDistroObjectByModuleObj failed!").c_str());
        return false;
    }
    return GetFaInstallationFreeByDistroObj(distroObj, installationFree);
}

bool ModuleJson::GetFaInstallationFreeByDistroObj(std::unique_ptr<PtJson>& distroObj, bool& installationFree)
{
    if (!distroObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Distro node is null!").c_str());
        return false;
    }
    installationFree = false;
    if (distroObj->Contains(INSTALLATION_FREE.c_str())) {
        if (distroObj->GetBool(INSTALLATION_FREE.c_str(), &installationFree) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Distro node get ") + INSTALLATION_FREE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaDeliveryWithInstall(bool& deliveryWithInstall)
{
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObject(distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
        return false;
    }
    if (!distroObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Distro node is null!").c_str());
        return false;
    }
    if (distroObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
        if (distroObj->GetBool(DELIVERY_WITH_INSTALL.c_str(), &deliveryWithInstall) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Distro node get ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
            return false;
        }
    } else {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node has no ") + DELIVERY_WITH_INSTALL + " node!").c_str());
        return false;
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaModuleApiVersionByAppObj(appObj, moduleApiVersion);
}

bool ModuleJson::GetFaModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + API_VERSION + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + API_VERSION + " node failed!").c_str());
        return false;
    }
    return GetFaModuleApiVersionByApiVersionObj(apiVersionObj, moduleApiVersion);
}

bool ModuleJson::GetFaModuleApiVersionByApiVersionObj(
    std::unique_ptr<PtJson>& apiVersionObj, ModuleApiVersion& moduleApiVersion)
{
    if (!apiVersionObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("ApiVersion node is null!").c_str());
        return false;
    }
    if (apiVersionObj->Contains(COMPATIBLE.c_str())) {
        if (apiVersionObj->GetInt(COMPATIBLE.c_str(), &moduleApiVersion.compatibleApiVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node get ") + COMPATIBLE + " node failed!").c_str());
            return false;
        }
    }
    if (apiVersionObj->Contains(TARGET.c_str())) {
        if (apiVersionObj->GetInt(TARGET.c_str(), &moduleApiVersion.targetApiVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node get ") + TARGET + " node failed!").c_str());
            return false;
        }
    }
    if (apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
        if (apiVersionObj->GetString(RELEASE_TYPE.c_str(), &moduleApiVersion.releaseType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node get ") + RELEASE_TYPE + " node failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaModuleName(std::string& faModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetDistroObjectByModuleObj failed!").c_str());
        return false;
    }
    if (!distroObj->Contains(MODULE_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node has no ") + MODULE_NAME + " node!").c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_NAME.c_str(), &faModuleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node get ") + MODULE_NAME + " node failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& faModuleName)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObjectByModuleObj(moduleObj, distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetDistroObjectByModuleObj failed!").c_str());
        return false;
    }
    return GetFaModuleNameByDistroObj(distroObj, faModuleName);
}

bool ModuleJson::GetFaModuleNameByDistroObj(std::unique_ptr<PtJson>& distroObj, std::string& faModuleName)
{
    if (!distroObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Distro node is null!").c_str());
        return false;
    }
    if (!distroObj->Contains(MODULE_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node has no ") + MODULE_NAME + " node!").c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_NAME.c_str(), &faModuleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node get ") + MODULE_NAME + " node failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaPackageStr(std::string& packageStr)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetFaPackageStrByModuleObj(moduleObj, packageStr);
}

bool ModuleJson::GetFaPackageStrByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& packageStr)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(PACKAGE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + PACKAGE + " node!").c_str());
        return false;
    }
    if (moduleObj->GetString(PACKAGE.c_str(), &packageStr) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + PACKAGE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetFaCompileSdkType(std::string& compileSdkType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaCompileSdkTypeByAppObj(appObj, compileSdkType);
}

bool ModuleJson::GetFaCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + API_VERSION + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + API_VERSION + " node failed!").c_str());
        return false;
    }
    if (apiVersionObj->Contains(COMPILE_SDK_TYPE.c_str())) {
        if (apiVersionObj->GetString(COMPILE_SDK_TYPE.c_str(), &compileSdkType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node get ") + COMPILE_SDK_TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaCompileSdkVersionByAppObj(appObj, compileSdkVersion);
}

bool ModuleJson::GetFaCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + API_VERSION + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + API_VERSION + " node failed!").c_str());
        return false;
    }
    if (apiVersionObj->Contains(COMPILE_SDK_VERSION.c_str())) {
        if (apiVersionObj->GetString(COMPILE_SDK_VERSION.c_str(), &compileSdkVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node get ") + COMPILE_SDK_VERSION + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaDebugByDeviceConfigObj(deviceConfigObj, debug);
}

bool ModuleJson::GetFaDebugByDeviceConfigObj(std::unique_ptr<PtJson>& deviceConfigObj, bool& debug)
{
    if (!deviceConfigObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("DeviceConfig node is null!").c_str());
        return false;
    }
    if (!deviceConfigObj->Contains(DEFAULT.c_str())) {
        LOGI("DeviceConfig node has no %s node!", DEFAULT.c_str());
        return false;
    }
    std::unique_ptr<PtJson> defaultObj;
    if (deviceConfigObj->GetObject(DEFAULT.c_str(), &defaultObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("DeviceConfig node get ") + DEFAULT + " failed!").c_str());
        return false;
    }
    if (defaultObj->Contains(DEBUG.c_str())) {
        if (defaultObj->GetBool(DEBUG.c_str(), &debug) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("debug node get ") + DEBUG + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> distroObj;
    if (!moduleObj->Contains(DISTRO.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + DISTRO + " node!").c_str());
        return false;
    }
    if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + DISTRO + " node failed!").c_str());
        return false;
    }
    std::string moduleType;
    if (!distroObj->Contains(MODULE_TYPE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node has no ") + MODULE_TYPE + " node!").c_str());
        return false;
    }
    if (distroObj->GetString(MODULE_TYPE.c_str(), &moduleType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node get ") + MODULE_TYPE + " failed!").c_str());
        return false;
    }
    if (moduleType.compare(ENTRY) == 0) {
        if (!GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                "GetFaDeviceTypesByModuleObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetFaDeviceTypes(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes);
}

bool ModuleJson::GetFaDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + DEVICE_TYPE + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> deviceTypeObj;
    if (moduleObj->GetArray(DEVICE_TYPE.c_str(), &deviceTypeObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + DEVICE_TYPE + " array node failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetFaModuleTypeByModuleObj(moduleObj, moduleType);
}

// java : parseFAIsEntry
bool ModuleJson::GetFaModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    moduleType = "";
    if (moduleObj->Contains(DISTRO.c_str())) {
        std::unique_ptr<PtJson> distroObj;
        if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + DISTRO + " node failed!").c_str());
            return false;
        }
        if (distroObj->Contains(MODULE_TYPE.c_str())) {
            if (distroObj->GetString(MODULE_TYPE.c_str(), &moduleType) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                    std::string("Module node get ") + MODULE_TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaReleaseTypeByAppObj(appObj, releaseType);
}

bool ModuleJson::GetFaReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& releaseType)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    releaseType = "";
    if (appObj->Contains(API_VERSION.c_str())) {
        if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + API_VERSION + " node failed!").c_str());
            return false;
        }
        if (apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
            if (apiVersionObj->GetString(RELEASE_TYPE.c_str(), &releaseType) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                    std::string("App node get ") + RELEASE_TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetFaAsanEnabledByAppObj(appObj, asanEnabled);
}

bool ModuleJson::GetFaAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(ASAN_ENABLED.c_str())) {
        if (appObj->GetBool(ASAN_ENABLED.c_str(), &asanEnabled) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + ASAN_ENABLED + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetFaDistroFilterByModuleObj(moduleObj, distroFilter);
}

bool ModuleJson::GetFaDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj, DistroFilter& distroFilter)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(DISTRO_FILTER.c_str())) {
        std::string distroFilterStr;
        if (moduleObj->GetString(DISTRO_FILTER.c_str(), &distroFilterStr) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + DISTRO_FILTER + " failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroFilterJsonObj = PtJson::Parse(distroFilterStr);
        if (!distroFilterJsonObj) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                "Parse distro filter string failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroFilterObj;
        if (distroFilterJsonObj->Contains(DISTRO_FILTER.c_str())) {
            if (distroFilterJsonObj->GetObject(DISTRO_FILTER.c_str(), &distroFilterObj) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                    std::string("DistroFilter node get ") + DISTRO_FILTER + " failed!").c_str());
                return false;
            }
        }
        if (!distroFilter.ParseFromJson(distroFilterObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                "Parse distro filter failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "Failed to get app object from config.json.").c_str());
        return false;
    }
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "Failed to get module object from config.json.").c_str());
        return false;
    }
    std::string bundleName;
    std::string bundleType;
    std::list<DependencyItem> dependencyItems;
    bool debug = false;
    if (!GetBundleNameByAppObj(appObj, bundleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "Failed to get bundleName from app object in config.json.").c_str());
        return false;
    }
    if (!GetFaBundleType(bundleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "Failed to get bundleType from config.json.").c_str());
        return false;
    }
    if (!GetDependencyItemsByModuleObj(moduleObj, dependencyItems, bundleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "Failed to get dependencies from module object in config.json.").c_str());
        return false;
    }
    GetFaDebug(debug);
    hapVerifyInfo.SetBundleName(bundleName);
    hapVerifyInfo.SetBundleType(bundleType);
    hapVerifyInfo.SetDependencyItemList(dependencyItems);
    hapVerifyInfo.SetDebug(debug);
    if (!SetFaHapVerifyInfoByAppObj(appObj, hapVerifyInfo)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "SetFaHapVerifyInfoByAppObj failed!").c_str());
        return false;
    }
    if (!SetFaHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "SetFaHapVerifyInfoByModuleObj failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "The app object in config.json is null.").c_str());
        return false;
    }
    std::string vendor;
    Version version;
    ModuleApiVersion moduleApiVersion;
    std::string compileSdkType;
    std::string compileSdkVersion;
    if (!GetVendorByAppObj(appObj, vendor)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetVendorByAppObj failed!").c_str());
        return false;
    }
    if (!GetFaVersionByAppObj(appObj, version)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetFaVersionByAppObj failed!").c_str());
        return false;
    }
    if (!GetFaModuleApiVersionByAppObj(appObj, moduleApiVersion)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaModuleApiVersionByAppObj failed!").c_str());
        return false;
    }
    if (!GetFaCompileSdkTypeByAppObj(appObj, compileSdkType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaCompileSdkTypeByAppObj failed!").c_str());
        return false;
    }
    if (!GetFaCompileSdkVersionByAppObj(appObj, compileSdkVersion)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaCompileSdkVersionByAppObj failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "The module object in config.json is null.").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaModuleNameByModuleObj failed!").c_str());
        return false;
    }
    if (!GetFaDistroFilterByModuleObj(moduleObj, distroFilter)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaDistroFilterByModuleObj failed!").c_str());
        return false;
    }
    if (!GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaDeviceTypesByModuleObj failed!").c_str());
        return false;
    }
    if (!GetAbilityNamesByModuleObj(moduleObj, abilityNames)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetAbilityNamesByModuleObj failed!").c_str());
        return false;
    }
    if (!GetFaModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaModuleTypeByModuleObj failed!").c_str());
        return false;
    }
    if (!GetFaPackageStrByModuleObj(moduleObj, packageStr)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaPackageStrByModuleObj failed!").c_str());
        return false;
    }
    if (!GetFaInstallationFreeByModuleObj(moduleObj, installationFree)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            "GetFaInstallationFreeByModuleObj failed!").c_str());
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

bool ModuleJson::SetFaBundleName(const std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        if (!appObj->Add(BUNDLE_NAME.c_str(), bundleName.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + BUNDLE_NAME + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(BUNDLE_NAME.c_str(), bundleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (!versionObj->Add(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Version node add ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (versionObj->SetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaMinAPIVersion(const int32_t& minAPIVersion)
{
    std::unique_ptr<PtJson> apiVersionObj;
    if (!GetApiVersionObject(apiVersionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!apiVersionObj->Contains(COMPATIBLE.c_str())) {
        if (!apiVersionObj->Add(COMPATIBLE.c_str(), minAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node add ") + COMPATIBLE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (apiVersionObj->SetInt(COMPATIBLE.c_str(), minAPIVersion) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("ApiVersion node set ") + COMPATIBLE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaTargetAPIVersion(const int32_t& targetAPIVersion)
{
    std::unique_ptr<PtJson> apiVersionObj;
    if (!GetApiVersionObject(apiVersionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetApiVersionObject failed!").c_str());
        return false;
    }
    if (!apiVersionObj->Contains(TARGET.c_str())) {
        if (!apiVersionObj->Add(TARGET.c_str(), targetAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node add ") + TARGET + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (apiVersionObj->SetInt(TARGET.c_str(), targetAPIVersion) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("ApiVersion node set ") + TARGET + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaApiReleaseType(const std::string& apiReleaseType)
{
    std::unique_ptr<PtJson> apiVersionObj;
    if (!GetApiVersionObject(apiVersionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetApiVersionObject failed!").c_str());
        return false;
    }
    if (!apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
        if (!apiVersionObj->Add(RELEASE_TYPE.c_str(), apiReleaseType.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("ApiVersion node add ") + RELEASE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (apiVersionObj->SetString(RELEASE_TYPE.c_str(), apiReleaseType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("ApiVersion node set ") + RELEASE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaBundleType(const std::string& bundleType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (!appObj->Add(BUNDLE_TYPE.c_str(), bundleType.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + BUNDLE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(BUNDLE_TYPE.c_str(), bundleType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaInstallationFree(const bool& installationFree)
{
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObject(distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
        return false;
    }
    if (!distroObj->Contains(INSTALLATION_FREE.c_str())) {
        if (!distroObj->Add(INSTALLATION_FREE.c_str(), installationFree)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Distro node add ") + INSTALLATION_FREE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (distroObj->SetBool(INSTALLATION_FREE.c_str(), installationFree) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node set ") + INSTALLATION_FREE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaDeliveryWithInstall(const bool& deliveryWithInstall)
{
    std::unique_ptr<PtJson> distroObj;
    if (!GetDistroObject(distroObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
        return false;
    }
    if (!distroObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
        if (!distroObj->Add(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Distro node add ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (distroObj->SetBool(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Distro node set ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetFaDeviceTypes(const std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPE.c_str())) {
        if (!moduleObj->Add(DEVICE_TYPE.c_str(), deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
                std::string("Module node add ") + DEVICE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (moduleObj->SetArray(DEVICE_TYPE.c_str(), deviceTypes) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_FA_JSON_FAILED.toStringWithArgs(
            std::string("Module node set ") + DEVICE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
