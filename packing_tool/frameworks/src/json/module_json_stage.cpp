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
const std::string BUILDVERSION = "buildVersion";
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
const std::string EXTRACT_NATIVE_LIBS = "extractNativeLibs";
const std::string EXECUTABLE_BINARY_PATHS = "executableBinaryPaths";
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
const std::string SKILL_PROFILES = "skillProfiles";
const std::string SKILL_NAME = "name";
const std::string SKILL_ABILITY_NAME = "abilityName";
const std::string TYPE_SKILL = "skill";

std::string BuildStageBundleTypeError(const std::string& errMsg, const std::string& solution)
{
    return PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs({errMsg, solution});
}
}

bool ModuleJson::SetStageVersionCode(const int32_t& versionCode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSIONCODE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSIONCODE + " node!").c_str());
        return false;
    }
    if (appObj->SetInt(VERSIONCODE.c_str(), versionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + VERSIONCODE + " failed!").c_str());
        return false;
    }
    return true;
}


bool ModuleJson::SetBuildVersion(const std::string& buildVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUILDVERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + BUILDVERSION + " node!").c_str());
        return false;
    }
    if (appObj->SetString(BUILDVERSION.c_str(), buildVersion) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUILDVERSION + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageVersionName(const std::string& versionName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSIONNAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSIONNAME + " node!").c_str());
        return false;
    }
    if (appObj->SetString(VERSIONNAME.c_str(), versionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + VERSIONNAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageBundleName(const std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        if (!appObj->Add(BUNDLE_NAME.c_str(), bundleName.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + BUNDLE_NAME + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(BUNDLE_NAME.c_str(), bundleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (!appObj->Add(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageMinAPIVersion(const int32_t& minAPIVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(MIN_API_VERSION.c_str())) {
        if (!appObj->Add(MIN_API_VERSION.c_str(), minAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + MIN_API_VERSION + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetInt(MIN_API_VERSION.c_str(), minAPIVersion) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + MIN_API_VERSION + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageTargetAPIVersion(const int32_t& targetAPIVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(TARGET_API_VERSION.c_str())) {
        if (!appObj->Add(TARGET_API_VERSION.c_str(), targetAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + TARGET_API_VERSION + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetInt(TARGET_API_VERSION.c_str(), targetAPIVersion) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + TARGET_API_VERSION + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageApiReleaseType(const std::string& apiReleaseType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(API_RELEASE_TYPE.c_str())) {
        if (!appObj->Add(API_RELEASE_TYPE.c_str(), apiReleaseType.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + API_RELEASE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(API_RELEASE_TYPE.c_str(), apiReleaseType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + API_RELEASE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageBundleType(const std::string& bundleType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (!appObj->Add(BUNDLE_TYPE.c_str(), bundleType.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + BUNDLE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(BUNDLE_TYPE.c_str(), bundleType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageInstallationFree(const bool& installationFree)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(INSTALLATION_FREE.c_str())) {
        if (!moduleObj->Add(INSTALLATION_FREE.c_str(), installationFree)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + INSTALLATION_FREE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (moduleObj->SetBool(INSTALLATION_FREE.c_str(), installationFree) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node set ") + INSTALLATION_FREE + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageDeliveryWithInstall(const bool& deliveryWithInstall)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
        if (!moduleObj->Add(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (moduleObj->SetBool(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node set ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageDeviceTypes(const std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPES.c_str())) {
        if (!moduleObj->Add(DEVICE_TYPES.c_str(), deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + DEVICE_TYPES + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (moduleObj->SetArray(DEVICE_TYPES.c_str(), deviceTypes) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node set ") + DEVICE_TYPES + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetStageVersion(Version& version)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetStageVersionByAppObj(appObj, version);
}

bool ModuleJson::GetStageVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSIONCODE.c_str()) || !appObj->Contains(VERSIONNAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSIONCODE + " node or " + VERSIONNAME + " node").c_str());
        return false;
    }
    if (appObj->GetInt(VERSIONCODE.c_str(), &version.versionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSIONCODE + " failed!").c_str());
        return false;
    }
    if (appObj->GetString(VERSIONNAME.c_str(), &version.versionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSIONNAME + " failed!").c_str());
        return false;
    }
    if (appObj->Contains(BUILDVERSION.c_str())) {
        if (appObj->GetString(BUILDVERSION.c_str(), &version.buildVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + BUILDVERSION + " failed!").c_str());
            return false;
        }
    }
    if (appObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (appObj->GetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(),
            &version.minCompatibleVersionCode) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
}

bool ModuleJson::GetStageInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    installationFree = false;
    if (moduleObj->Contains(INSTALLATION_FREE.c_str())) {
        if (moduleObj->GetBool(INSTALLATION_FREE.c_str(), &installationFree) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + INSTALLATION_FREE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageModuleApiVersion(ModuleApiVersion& moduleApiVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion);
}

bool ModuleJson::GetStageModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(MIN_API_VERSION.c_str())) {
        if (appObj->GetInt(MIN_API_VERSION.c_str(), &moduleApiVersion.compatibleApiVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + MIN_API_VERSION + " failed!").c_str());
            return false;
        }
    }
    if (appObj->Contains(TARGET_API_VERSION.c_str())) {
        if (appObj->GetInt(TARGET_API_VERSION.c_str(), &moduleApiVersion.targetApiVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + TARGET_API_VERSION + " failed!").c_str());
            return false;
        }
    }
    if (appObj->Contains(API_RELEASE_TYPE.c_str())) {
        if (appObj->GetString(API_RELEASE_TYPE.c_str(), &moduleApiVersion.releaseType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + API_RELEASE_TYPE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageModuleName(std::string& stageModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetStageModuleNameByModuleObj(moduleObj, stageModuleName);
}

bool ModuleJson::GetStageModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& stageModuleName)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + NAME + " node!").c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &stageModuleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetStageCompileSdkType(std::string& compileSdkType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetStageCompileSdkTypeByAppObj(appObj, compileSdkType);
}

bool ModuleJson::GetStageCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(COMPILE_SDK_TYPE.c_str())) {
        if (appObj->GetString(COMPILE_SDK_TYPE.c_str(), &compileSdkType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + COMPILE_SDK_TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetStageCompileSdkVersionByAppObj(appObj, compileSdkVersion);
}

bool ModuleJson::GetStageCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(COMPILE_SDK_VERSION.c_str())) {
        if (appObj->GetString(COMPILE_SDK_VERSION.c_str(), &compileSdkVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + COMPILE_SDK_VERSION + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetStageDebugByAppObj(appObj, debug);
}

bool ModuleJson::GetStageDebugByAppObj(std::unique_ptr<PtJson>& appObj, bool& debug)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(DEBUG.c_str())) {
        if (appObj->GetBool(DEBUG.c_str(), &debug) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + DEBUG + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    std::string moduleType;
    if (!moduleObj->Contains(TYPE.c_str())) {
        std::string moduleName;
        if (moduleObj->Contains(NAME.c_str())) {
            moduleObj->GetString(NAME.c_str(), &moduleName);
        }
        std::string errMsg = moduleName.empty()
            ? "Module does not contain 'type' in module.json."
            : "Module: '" + moduleName + "' does not contain 'type' in module.json.";
        std::string solution = moduleName.empty()
            ? "Ensure the module.json file includes a valid 'type' field."
            : "Ensure the module.json file includes a valid 'type' field for module '" + moduleName + "'.";
        LOGE("%s", BuildStageBundleTypeError(errMsg, solution).c_str());
        return false;
    }
    if (moduleObj->GetString(TYPE.c_str(), &moduleType) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + TYPE + " failed!").c_str());
        return false;
    }
    if (moduleType.compare(ENTRY) == 0) {
        if (!GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                "GetStageDeviceTypesByModuleObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetStageDeviceTypes(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes);
}

bool ModuleJson::GetStageDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<std::string>& deviceTypes)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPES.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + DEVICE_TYPES + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> deviceTypeObj;
    if (moduleObj->GetArray(DEVICE_TYPES.c_str(), &deviceTypeObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + DEVICE_TYPES + " array node failed!").c_str());
        return false;
    }
    for (int32_t i = 0; i < deviceTypeObj->GetSize(); i++) {
        deviceTypes.push_back(deviceTypeObj->Get(i)->GetString());
    }
    return true;
}

bool ModuleJson::GetStageDistroFilter(DistroFilter& distroFilter,
    const std::map<std::string, std::string>& resourceMap)
{
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    if (!GetModuleMetadatas(moduleMetadataInfos, resourceMap)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleMetadatas failed!").c_str());
        return false;
    }
    return ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter);
}

bool ModuleJson::GetStageDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    const std::map<std::string, std::string>& resourceMap, DistroFilter& distroFilter)
{
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    if (!GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetModuleMetadatasByModuleObj failed!").c_str());
        return false;
    }
    return ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter);
}

bool ModuleJson::GetStageModuleType(std::string& moduleType)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetStageModuleTypeByModuleObj(moduleObj, moduleType);
}

bool ModuleJson::GetStageModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(TYPE.c_str())) {
        if (moduleObj->GetString(TYPE.c_str(), &moduleType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get app or module object from module.json.").c_str());
        return false;
    }
    if (!moduleObj->Contains(TYPE.c_str())) {
        std::string moduleName;
        if (moduleObj->Contains(NAME.c_str())) {
            moduleObj->GetString(NAME.c_str(), &moduleName);
        }
        std::string errMsg = moduleName.empty()
            ? "Module does not contain 'type' in module.json."
            : "Module: '" + moduleName + "' does not contain 'type' in module.json.";
        std::string solution = moduleName.empty()
            ? "Ensure the module.json file includes a valid 'type' field."
            : "Ensure the module.json file includes a valid 'type' field for module '" + moduleName + "'.";
        LOGE("%s", BuildStageBundleTypeError(errMsg, solution).c_str());
        return false;
    }
    std::string moduleName;
    std::string moduleType;
    if (!GetStageModuleNameByModuleObj(moduleObj, moduleName) ||
        !GetStageModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get module name or moduleType from module.json.").c_str());
        return false;
    }
    bool installationFree = false;
    GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (installationFree) {
            std::string errMsg =
                "The app.json5 file configuration does not match the 'installationFree' setting of true.";
            std::string solution = "Add the 'bundleType' field to the app.json5 file and set it atomicService.";
            LOGE("%s", BuildStageBundleTypeError(errMsg, solution).c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get app object from module.json.").c_str());
        return false;
    }
    return GetStageAsanEnabledByAppObj(appObj, asanEnabled);
}

bool ModuleJson::GetStageAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "The app object in module.json is null.").c_str());
        return false;
    }
    if (appObj->Contains(ASAN_ENABLED.c_str())) {
        if (appObj->GetBool(ASAN_ENABLED.c_str(), &asanEnabled) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + ASAN_ENABLED + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get app object from module.json.").c_str());
        return false;
    }
    return GetStageTsanEnabledByAppObj(appObj, tsanEnabled);
}

bool ModuleJson::GetStageTsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& tsanEnabled)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "The app object in module.json is null.").c_str());
        return false;
    }
    if (appObj->Contains(TSAN_ENABLED.c_str())) {
        if (appObj->GetBool(TSAN_ENABLED.c_str(), &tsanEnabled) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + TSAN_ENABLED + " failed!").c_str());
            return false;
        }
    } else {
        tsanEnabled = false;
    }
    return true;
}

bool ModuleJson::GetStageCompressNativeLibs(bool& compressNativeLibs)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get module object from module.json.").c_str());
        return false;
    }
    return GetStageCompressNativeLibsByAppObj(moduleObj, compressNativeLibs);
}

bool ModuleJson::GetStageCompressNativeLibsByAppObj(std::unique_ptr<PtJson>& moduleObj, bool& compressNativeLibs)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "The module object in module.json is null.").c_str());
        return false;
    }
    if (moduleObj->Contains(COMPRESS_NATIVE_LIBS.c_str())) {
        if (moduleObj->GetBool(COMPRESS_NATIVE_LIBS.c_str(), &compressNativeLibs) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + COMPRESS_NATIVE_LIBS + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get app object from module.json.").c_str());
        return false;
    }
    return GetStageApiReleaseTypeByAppObj(appObj, apiReleaseType);
}

bool ModuleJson::GetStageApiReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& apiReleaseType)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "The app object in module.json is null.").c_str());
        return false;
    }
    if (appObj->Contains(API_RELEASE_TYPE.c_str())) {
        if (appObj->GetString(API_RELEASE_TYPE.c_str(), &apiReleaseType) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + API_RELEASE_TYPE + " failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get app object from module.json.").c_str());
        return false;
    }
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get module object from module.json.").c_str());
        return false;
    }
    std::string bundleName;
    std::string bundleType;
    std::list<DependencyItem> dependencyItems;
    if (!GetBundleNameByAppObj(appObj, bundleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get bundleName from app object in module.json.").c_str());
        return false;
    }
    if (!GetStageBundleType(bundleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get bundleType from module.json.").c_str());
        return false;
    }
    if (!GetDependencyItemsByModuleObj(moduleObj, dependencyItems, bundleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "Failed to get dependencies from module object in module.json.").c_str());
        return false;
    }
    hapVerifyInfo.SetBundleName(bundleName);
    hapVerifyInfo.SetBundleType(bundleType);
    hapVerifyInfo.SetDependencyItemList(dependencyItems);
    if (!SetStageHapVerifyInfoByAppObj(appObj, hapVerifyInfo)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "SetStageHapVerifyInfoByAppObj failed!").c_str());
        return false;
    }
    if (!SetStageHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "SetStageHapVerifyInfoByModuleObj failed!").c_str());
        return false;
    }
    if (!SetStageHapVerifyInfoExtByModuleObj(moduleObj, hapVerifyInfo)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "SetStageHapVerifyInfoExtByModuleObj failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetStageHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    std::string vendor;
    Version version;
    ModuleApiVersion moduleApiVersion;
    std::string targetBundleName;
    int32_t targetPriority = 0;
    bool debug = false;
    MultiAppMode multiAppMode;
    std::list<std::string> assetAccessGroups;
    if (!GetVendorByAppObj(appObj, vendor)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetVendorByAppObj failed!").c_str());
        return false;
    }
    if (!GetStageVersionByAppObj(appObj, version)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageVersionByAppObj failed!").c_str());
        return false;
    }
    if (!GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageModuleApiVersionByAppObj failed!").c_str());
        return false;
    }
    if (!GetTargetBundleNameByAppObj(appObj, targetBundleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetTargetBundleNameByAppObj failed!").c_str());
        return false;
    }
    if (!GetTargetPriorityByAppObj(appObj, targetPriority)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetTargetPriorityByAppObj failed!").c_str());
        return false;
    }
    if (!GetStageDebugByAppObj(appObj, debug)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageDebugByAppObj failed!").c_str());
        return false;
    }
    if (!GetMultiAppModeByAppObj(appObj, multiAppMode)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetMultiAppModeByAppObj failed!").c_str());
        return false;
    }
    if (!GetAssetAccessGroupsByModuleObj(appObj, assetAccessGroups)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetAssetAccessGroupsByModuleObj failed!").c_str());
        return false;
    }
    hapVerifyInfo.SetVendor(vendor);
    hapVerifyInfo.SetVersion(version);
    hapVerifyInfo.SetApiVersion(moduleApiVersion);
    hapVerifyInfo.SetTargetBundleName(targetBundleName);
    hapVerifyInfo.SetTargetPriority(targetPriority);
    hapVerifyInfo.SetDebug(debug);
    hapVerifyInfo.SetMultiAppMode(multiAppMode);
    hapVerifyInfo.SetAssetAccessGroups(assetAccessGroups);
    return true;
}

bool ModuleJson::SetStageHapVerifyInfoByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageModuleNameByModuleObj failed!").c_str());
        return false;
    }
    if (!GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageDeviceTypesByModuleObj failed!").c_str());
        return false;
    }
    if (!GetStageModuleTypeByModuleObj(moduleObj, moduleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageModuleTypeByModuleObj failed!").c_str());
        return false;
    }
    if (!GetStageInstallationFreeByModuleObj(moduleObj, installationFree)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageInstallationFreeByModuleObj failed!").c_str());
        return false;
    }
    if (!GetTargetModuleNameByModuleObj(moduleObj, targetModuleName) ||
        !GetTargetModulePriorityByModuleObj(moduleObj, targetModulePriority)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetTargetModuleNameByModuleObj or GetTargetModulePriorityByModuleObj failed!").c_str());
        return false;
    }
    if (!GetStageCompileSdkTypeByAppObj(moduleObj, compileSdkType) ||
        !GetStageCompileSdkVersionByAppObj(moduleObj, compileSdkVersion)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageCompileSdkTypeByAppObj or GetStageCompileSdkVersionByAppObj failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    DistroFilter distroFilter;
    std::list<std::string> abilityNames;
    std::list<std::string> extensionAbilityNames;
    std::list<PreloadItem> preloadItems;
    std::list<std::string> proxyDataUris;
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    if (!GetStageDistroFilterByModuleObj(moduleObj, hapVerifyInfo.GetResourceMap(), distroFilter)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetStageDistroFilterByModuleObj failed!").c_str());
        return false;
    }
    if (!GetAbilityNamesByModuleObj(moduleObj, abilityNames)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetAbilityNamesByModuleObj failed!").c_str());
        return false;
    }
    if (!GetExtensionAbilityNamesByModuleObj(moduleObj, extensionAbilityNames)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetExtensionAbilityNamesByModuleObj failed!").c_str());
        return false;
    }
    if (!GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetAtomicServicePreloadsByModuleObj failed!").c_str());
        return false;
    }
    if (!GetProxyDataUrisByModuleObj(moduleObj, proxyDataUris)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetProxyDataUrisByModuleObj failed!").c_str());
        return false;
    }
    if (!GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            "GetAbilityContinueTypeMapByModuleObj failed!").c_str());
        return false;
    }
    hapVerifyInfo.SetDistroFilter(distroFilter);
    hapVerifyInfo.SetAbilityNames(abilityNames);
    hapVerifyInfo.AddAbilityNames(extensionAbilityNames);
    hapVerifyInfo.SetPreloadItems(preloadItems);
    hapVerifyInfo.SetProxyDataUris(proxyDataUris);
    hapVerifyInfo.SetContinueTypeMap(abilityContinueTypeMap);
    return true;
}

bool ModuleJson::GetStageExtractNativeLibs(bool& extractNativeLibs)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetStageExtractNativeLibsByAppObj(moduleObj, extractNativeLibs);
}

bool ModuleJson::GetStageExtractNativeLibsByAppObj(std::unique_ptr<PtJson>& moduleObj, bool& extractNativeLibs)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(EXTRACT_NATIVE_LIBS.c_str())) {
        if (moduleObj->GetBool(EXTRACT_NATIVE_LIBS.c_str(), &extractNativeLibs) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
                std::string("Module node get ") + EXTRACT_NATIVE_LIBS + " failed!").c_str());
            return false;
        }
    } else {
        extractNativeLibs = true;  // default value is true
    }
    return true;
}

bool ModuleJson::HasExecutableBinaries()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return HasExecutableBinariesByModuleObj(moduleObj);
}

bool ModuleJson::HasExecutableBinariesByModuleObj(std::unique_ptr<PtJson>& moduleObj)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(EXECUTABLE_BINARY_PATHS.c_str())) {
        return false;
    }
    std::unique_ptr<PtJson> executablesObj;
    if (moduleObj->GetArray(EXECUTABLE_BINARY_PATHS.c_str(), &executablesObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + EXECUTABLE_BINARY_PATHS + " failed!").c_str());
        return false;
    }
    if (!executablesObj) {
        return false;
    }
    return executablesObj->GetSize() > 0;
}

bool ModuleJson::GetSkillProfiles(std::list<std::map<std::string, std::string>>& skillProfiles)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(SKILL_PROFILES.c_str())) {
        return true;
    }
    std::unique_ptr<PtJson> skillProfilesObj;
    if (moduleObj->GetArray(SKILL_PROFILES.c_str(), &skillProfilesObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "Get skillProfiles array failed!").c_str());
        return false;
    }
    if (!skillProfilesObj) {
        return true;
    }
    for (int32_t i = 0; i < skillProfilesObj->GetSize(); ++i) {
        std::unique_ptr<PtJson> profileObj = skillProfilesObj->Get(i);
        if (!profileObj) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "Get skillProfile failed!").c_str());
            return false;
        }
        std::map<std::string, std::string> profileMap;
        if (profileObj->Contains(SKILL_NAME.c_str())) {
            std::string name;
            if (profileObj->GetString(SKILL_NAME.c_str(), &name) == Result::SUCCESS) {
                profileMap[SKILL_NAME] = name;
            }
        }
        if (profileObj->Contains(SKILL_ABILITY_NAME.c_str())) {
            std::string abilityName;
            if (profileObj->GetString(SKILL_ABILITY_NAME.c_str(), &abilityName) == Result::SUCCESS) {
                profileMap[SKILL_ABILITY_NAME] = abilityName;
            }
        }
        skillProfiles.push_back(profileMap);
    }
    return true;
}


} // namespace AppPackingTool
} // namespace OHOS
