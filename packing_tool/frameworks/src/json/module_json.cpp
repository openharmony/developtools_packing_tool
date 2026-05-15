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

#include "log.h"
#include "utils.h"
#include "error/packing_tool_err_msg.h"

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
const std::string QUERY_SCHEMES = "querySchemes";
const int32_t QUERY_SCHEMES_CHECK_COUNT = 50;
const int32_t QUERY_SCHEMES_CHECK_MIN_API_VERSION = 21;
const std::string DEDUPLICATE_HAR = "deduplicateHar";
const int32_t DEDUPLICATE_HAR_CHECK_MIN_API_VERSION = 21;
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
const std::string APP_PLUGIN = "appPlugin";
const std::string TYPE_SKILL = "skill";
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
const std::string ASSET_ACCESS_GROUPS = "assetAccessGroups";
}

bool ModuleJson::ParseFromString(const std::string& jsonString)
{
    Release();
    if (jsonString.length() == 0) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json length is zero!").c_str());
        return false;
    }
    root_ = PtJson::Parse(jsonString);
    return IsValid();
}

bool ModuleJson::ParseFromFile(const std::string& jsonFile)
{
    Release();
    std::string realJsonFile;
    if (!Utils::GetRealPath(jsonFile, realJsonFile)) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("get real json file failed! jsonFile=" + jsonFile).c_str()).c_str());
        return false;
    }
    std::ifstream inFile(realJsonFile, std::ios::in);
    if (!inFile.is_open()) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("Open json file failed! jsonFile=" + jsonFile + ", realJsonFile=" + realJsonFile).c_str()).c_str());
        return false;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    root_ = PtJson::Parse(fileContent);
    return IsValid();
}

std::string ModuleJson::ToString()
{
    return root_->Stringify();
}

void ModuleJson::Release()
{
    if (root_.get() != nullptr) {
        root_->ReleaseRoot();
        root_ = nullptr;
    }
}

bool ModuleJson::IsValid()
{
    return (root_.get() != nullptr);
}

bool ModuleJson::GetAppObject(std::unique_ptr<PtJson>& appObj)
{
    if (root_.get() == nullptr) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_OBJECT_EXCEPTION.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(APP.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_OBJECT_EXCEPTION.toStringWithArgs(
            ("Json root has no " + APP + " node!").c_str()).c_str());
        return false;
    }
    if (root_->GetObject(APP.c_str(), &appObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_OBJECT_EXCEPTION.toStringWithArgs(
            ("Json root get " + APP + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetDeviceConfigObject(std::unique_ptr<PtJson>& deviceConfigObj)
{
    if (root_.get() == nullptr) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(DEVICE_CONFIG.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root has no " + DEVICE_CONFIG + " node!").c_str()).c_str());
        return false;
    }
    if (root_->GetObject(DEVICE_CONFIG.c_str(), &deviceConfigObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + DEVICE_CONFIG + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetVersionObject(std::unique_ptr<PtJson>& versionObj)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSION.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("App node has no " + VERSION + " node!").c_str()).c_str());
        return false;
    }
    if (appObj->GetObject(VERSION.c_str(), &versionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("App node get " + VERSION + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetModuleObject(std::unique_ptr<PtJson>& moduleObj)
{
    if (root_.get() == nullptr) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(MODULE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root has no " + MODULE + " node!").c_str()).c_str());
        return false;
    }
    if (root_->GetObject(MODULE.c_str(), &moduleObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + MODULE + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}


bool ModuleJson::GetDistroObject(std::unique_ptr<PtJson>& distroObj)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetDistroObjectByModuleObj(moduleObj, distroObj);
}

bool ModuleJson::GetDistroObjectByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::unique_ptr<PtJson>& distroObj)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DISTRO.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node has no " + DISTRO + " node!").c_str()).c_str());
        return false;
    }
    if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + DISTRO + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetApiVersionObject(std::unique_ptr<PtJson>& apiVersionObj)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        cJSON *apiVersion = cJSON_CreateObject();
        apiVersionObj = std::make_unique<PtJson>(apiVersion);
        if (appObj->Add(API_VERSION.c_str(), apiVersionObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node add " + API_VERSION + " node failed!").c_str()).c_str());
            return false;
        }
        return true;
    }
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("App node get " + API_VERSION + " node failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetModuleName(std::string& moduleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetModuleNameByModuleObj(moduleObj, moduleName);
}

bool ModuleJson::GetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleName)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node has no " + NAME + " node!").c_str()).c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &moduleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + NAME + " failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetPatchModuleName(std::string& patchModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node has no " + NAME + " node!").c_str()).c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &patchModuleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + NAME + " failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetBundleName(std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetBundleNameByAppObj(appObj, bundleName);
}

bool ModuleJson::GetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& bundleName)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs(
            "The module.json or config.json file does not contain 'bundleName'.").c_str());
        return false;
    }
    if (appObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs(
            ("App node get " + BUNDLE_NAME + " failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetBundleName(const std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("App node has no " + BUNDLE_NAME + " node!").c_str()).c_str());
        return false;
    }
    if (appObj->SetString(BUNDLE_NAME.c_str(), bundleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("App node set " + BUNDLE_NAME + " failed!").c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetVendor(std::string& vendor)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetVendorByAppObj(appObj, vendor);
}

bool ModuleJson::GetVendorByAppObj(std::unique_ptr<PtJson>& appObj, std::string& vendor)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(VENDOR.c_str())) {
        if (appObj->GetString(VENDOR.c_str(), &vendor) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + VENDOR + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        vendor = "";
    }
    return true;
}

bool ModuleJson::GetTargetBundleName(std::string& targetBundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetTargetBundleNameByAppObj(appObj, targetBundleName);
}

bool ModuleJson::GetTargetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& targetBundleName)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(TARGET_BUNDLE_NAME.c_str())) {
        if (appObj->GetString(TARGET_BUNDLE_NAME.c_str(), &targetBundleName) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + TARGET_BUNDLE_NAME + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        targetBundleName = "";
    }
    return true;
}

bool ModuleJson::GetTargetPriority(int32_t& targetPriority)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetTargetPriorityByAppObj(appObj, targetPriority);
}

bool ModuleJson::GetTargetPriorityByAppObj(std::unique_ptr<PtJson>& appObj, int32_t& targetPriority)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(TARGET_PRIORITY.c_str())) {
        if (appObj->GetInt(TARGET_PRIORITY.c_str(), &targetPriority) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + TARGET_PRIORITY + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        targetPriority = 0;
    }
    return true;
}

bool ModuleJson::GetTargetModuleName(std::string& targetModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetTargetModuleNameByModuleObj(moduleObj, targetModuleName);
}

bool ModuleJson::GetTargetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& targetModuleName)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(TARGET_MODULE_NAME.c_str())) {
        if (moduleObj->GetString(TARGET_MODULE_NAME.c_str(), &targetModuleName) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + TARGET_MODULE_NAME + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        targetModuleName = "";
    }
    return true;
}

bool ModuleJson::GetTargetModulePriority(int32_t& targetModulePriority)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetTargetModulePriorityByModuleObj(moduleObj, targetModulePriority);
}

bool ModuleJson::GetTargetModulePriorityByModuleObj(std::unique_ptr<PtJson>& moduleObj, int32_t& targetModulePriority)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(TARGET_PRIORITY.c_str())) {
        if (moduleObj->GetInt(TARGET_PRIORITY.c_str(), &targetModulePriority) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + TARGET_PRIORITY + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        targetModulePriority = 0;
    }
    return true;
}

// parseModuleMetadata
bool ModuleJson::GetModuleMetadatas(std::list<ModuleMetadataInfo>& moduleMetadataInfos,
    const std::map<std::string, std::string>& resourceMap)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos);
}

bool ModuleJson::GetModuleMetadataInfoByModuleMetadataInfoObj(std::unique_ptr<PtJson>& moduleMetadataInfoObj,
    const std::map<std::string, std::string>& resourceMap, ModuleMetadataInfo& moduleMetadataInfo)
{
    if (!moduleMetadataInfoObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ModuleMetadataInfo node is null!").c_str());
        return false;
    }
    if (moduleMetadataInfoObj->Contains(NAME.c_str())) {
        if (moduleMetadataInfoObj->GetString(NAME.c_str(), &moduleMetadataInfo.name) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("ModuleMetadataInfo node get " + NAME + " failed!").c_str()).c_str());
            return false;
        }
    }
    if (moduleMetadataInfoObj->Contains(VALUE.c_str())) {
        if (moduleMetadataInfoObj->GetString(VALUE.c_str(), &moduleMetadataInfo.value) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("ModuleMetadataInfo node get " + VALUE + " failed!").c_str()).c_str());
            return false;
        }
    }
    if (moduleMetadataInfoObj->Contains(RESOURCE.c_str())) {
        std::string resource;
        if (moduleMetadataInfoObj->GetString(RESOURCE.c_str(), &resource) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("ModuleMetadataInfo node get " + RESOURCE + " failed!").c_str()).c_str());
            return false;
        }
        std::string fileName = Utils::ReplaceAll(resource, PROFILE, "") + JSON_PREFIX;
        auto iter = resourceMap.find(fileName);
        if (iter == resourceMap.end()) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("find filename in resourceMap failed!").c_str());
            return false;
        }
        moduleMetadataInfo.resource = iter->second;
    }
    return true;
}

bool ModuleJson::GetModuleMetadatasByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    const std::map<std::string, std::string>& resourceMap, std::list<ModuleMetadataInfo>& moduleMetadataInfos)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> moduleMetadataInfosObj;
    if (moduleObj->Contains(METADATA.c_str())) {
        if (moduleObj->GetArray(METADATA.c_str(), &moduleMetadataInfosObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + METADATA + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < moduleMetadataInfosObj->GetSize(); i++) {
            ModuleMetadataInfo moduleMetadataInfo;
            std::unique_ptr<PtJson> moduleMetadataInfoObj = moduleMetadataInfosObj->Get(i);
            if (!GetModuleMetadataInfoByModuleMetadataInfoObj(moduleMetadataInfoObj, resourceMap,
                moduleMetadataInfo)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "GetModuleMetadataInfoByModuleMetadataInfoObj failed!").c_str());
                return false;
            }
            moduleMetadataInfos.push_back(moduleMetadataInfo);
        }
    }
    return true;
}

bool ModuleJson::ParseModuleMetadatasToDistroFilter(const std::list<ModuleMetadataInfo>& moduleMetadataInfos,
    DistroFilter& distroFilter)
{
    for (auto& moduleMetadataInfo : moduleMetadataInfos) {
        if (moduleMetadataInfo.resource.empty()) {
            continue;
        }
        std::unique_ptr<PtJson> distroFilterJsonObj = PtJson::Parse(moduleMetadataInfo.resource);
        if (!distroFilterJsonObj) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("DistroFilter node is null!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroFilterObj;
        if (distroFilterJsonObj->Contains(DISTRIBUTION_FILTER.c_str())) {
            if (distroFilterJsonObj->GetObject(DISTRIBUTION_FILTER.c_str(), &distroFilterObj) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("DistroFilter node get " + DISTRIBUTION_FILTER + " failed!").c_str()).c_str());
                return false;
            }
        } else if (distroFilterJsonObj->Contains(DISTRO_FILTER.c_str())) {
            if (distroFilterJsonObj->GetObject(DISTRO_FILTER.c_str(), &distroFilterObj) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("DistroFilter node get " + DISTRO_FILTER + " failed!").c_str()).c_str());
                return false;
            }
        }
        if (!distroFilter.ParseFromJson(distroFilterObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse distro filter failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetAbilityNames(std::list<std::string>& abilityNames)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetAbilityNamesByModuleObj(moduleObj, abilityNames);
}

bool ModuleJson::GetAbilityNamesByAbilitiesObj(std::unique_ptr<PtJson>& abilitiesObj,
    std::list<std::string>& abilityNames)
{
    if (!abilitiesObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Abilities node is null!").c_str());
        return false;
    }
    for (int32_t i = 0; i < abilitiesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> abilityObj = abilitiesObj->Get(i);
        if (abilityObj->Contains(NAME.c_str())) {
            std::string name;
            if (abilityObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("Ability node get " + NAME + " failed!").c_str()).c_str());
                return false;
            }
            abilityNames.push_back(name);
        }
    }
    return true;
}

bool ModuleJson::GetAbilityNamesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& abilityNames)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(ABILITIES.c_str())) {
        std::unique_ptr<PtJson> abilitiesObj;
        if (moduleObj->GetArray(ABILITIES.c_str(), &abilitiesObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + ABILITIES + " array node failed!").c_str()).c_str());
            return false;
        }
        if (!GetAbilityNamesByAbilitiesObj(abilitiesObj, abilityNames)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "GetAbilityNamesByAbilitiesObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetProxyDataUris(std::list<std::string>& proxyDataUris)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetProxyDataUrisByModuleObj(moduleObj, proxyDataUris);
}

bool ModuleJson::GetProxyDataUrisByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& proxyDataUris)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(PROXY_DATAS.c_str())) {
        std::unique_ptr<PtJson> proxyDatasObj;
        if (moduleObj->GetArray(PROXY_DATAS.c_str(), &proxyDatasObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + PROXY_DATAS + " array node failed!").c_str()).c_str());
            return false;
        }
        if (!GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "GetProxyDataUrisByProxyDatasObj failed!").c_str());
            return false;
        }
    } else if (moduleObj->Contains(PROXY_DATA.c_str())) {
        std::unique_ptr<PtJson> proxyDatasObj;
        if (moduleObj->GetArray(PROXY_DATA.c_str(), &proxyDatasObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + PROXY_DATA + " array node failed!").c_str()).c_str());
            return false;
        }
        if (!GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "GetProxyDataUrisByProxyDatasObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetProxyDataUrisByProxyDatasObj(std::unique_ptr<PtJson>& proxyDatasObj,
    std::list<std::string>& proxyDataUris)
{
    if (!proxyDatasObj || !proxyDatasObj->IsArray()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "ProxyData node is null or is not array!").c_str());
        return false;
    }
    for (int32_t i = 0; i < proxyDatasObj->GetSize(); i++) {
        std::unique_ptr<PtJson> proxyDataObj = proxyDatasObj->Get(i);
        if (!proxyDataObj->Contains(PROXY_URI.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("proxyData node has no " + PROXY_URI + " node!").c_str()).c_str());
            return false;
        }
        std::string proxyUri;
        if (proxyDataObj->GetString(PROXY_URI.c_str(), &proxyUri) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("ProxyData node get " + PROXY_URI + " failed!").c_str()).c_str());
            return false;
        }
        proxyDataUris.push_back(proxyUri);
    }
    return true;
}

bool ModuleJson::GetAssetAccessGroups(std::list<std::string>& assetAccessGroups)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetAssetAccessGroupsByModuleObj(moduleObj, assetAccessGroups);
}

bool ModuleJson::GetAssetAccessGroupsByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<std::string>& assetAccessGroups)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(ASSET_ACCESS_GROUPS.c_str())) {
        std::unique_ptr<PtJson> assetAccessGroupObj;
        if (moduleObj->GetArray(ASSET_ACCESS_GROUPS.c_str(), &assetAccessGroupObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + ASSET_ACCESS_GROUPS + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < assetAccessGroupObj->GetSize(); i++) {
            assetAccessGroups.push_back(assetAccessGroupObj->Get(i)->GetString());
        }
    }
    return true;
}

bool ModuleJson::GetExtensionAbilityNames(std::list<std::string>& extensionAbilityNames)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetAbilityNamesByModuleObj(moduleObj, extensionAbilityNames);
}

bool ModuleJson::GetExtensionAbilityNamesByExtensionAbilityObj(std::unique_ptr<PtJson>& extensionAbilitiesObj,
    std::list<std::string>& extensionAbilityNames)
{
    if (!extensionAbilitiesObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ExtensionAbilities node is null!").c_str());
        return false;
    }
    for (int32_t i = 0; i < extensionAbilitiesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> extensionAbilityObj = extensionAbilitiesObj->Get(i);
        if (extensionAbilityObj->Contains(NAME.c_str())) {
            std::string extensionAbilityName;
            if (extensionAbilityObj->GetString(NAME.c_str(), &extensionAbilityName) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("ExtensionAbility node get " + NAME + " failed!").c_str()).c_str());
                return false;
            }
            extensionAbilityNames.push_back(extensionAbilityName);
        }
    }
    return true;
}

bool ModuleJson::GetExtensionAbilityNamesByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<std::string>& extensionAbilityNames)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(EXTENSION_ABILITIES.c_str())) {
        std::unique_ptr<PtJson> extensionAbilitiesObj;
        if (moduleObj->GetArray(EXTENSION_ABILITIES.c_str(), &extensionAbilitiesObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + EXTENSION_ABILITIES + " array node failed!").c_str()).c_str());
            return false;
        }
        if (!GetExtensionAbilityNamesByExtensionAbilityObj(extensionAbilitiesObj, extensionAbilityNames)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "GetExtensionAbilityNamesByExtensionAbilityObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetDependencyItems(std::list<DependencyItem>& dependencyItems, const std::string& defaultBundleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName);
}

bool ModuleJson::GetDependencyBundleNameByDependencyItemObj(std::unique_ptr<PtJson>& dependencyItemObj,
    std::string& bundleName, const std::string& defaultBundleName)
{
    if (!dependencyItemObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("dependencyItem node is null!").c_str());
        return false;
    }
    if (dependencyItemObj->Contains(BUNDLE_NAME.c_str())) {
        if (dependencyItemObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("DependencyItem node get " + BUNDLE_NAME + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        bundleName = defaultBundleName;
    }
    return true;
}

bool ModuleJson::GetDependencyModuleNameByDependencyItemObj(std::unique_ptr<PtJson>& dependencyItemObj,
    std::string& moduleName)
{
    if (!dependencyItemObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("dependencyItem node is null!").c_str());
        return false;
    }
    moduleName = "";
    if (dependencyItemObj->Contains(MODULE_NAME.c_str())) {
        if (dependencyItemObj->GetString(MODULE_NAME.c_str(), &moduleName) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("DependencyItem node get " + MODULE_NAME + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetDependencyItemsByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<DependencyItem>& dependencyItems, const std::string& defaultBundleName)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(DEPENDENCIES.c_str())) {
        std::unique_ptr<PtJson> dependencyItemsObj;
        if (moduleObj->GetArray(DEPENDENCIES.c_str(), &dependencyItemsObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + DEPENDENCIES + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < dependencyItemsObj->GetSize(); i++) {
            std::unique_ptr<PtJson> dependencyItemObj = dependencyItemsObj->Get(i);
            DependencyItem dependencyItem;
            if (!GetDependencyBundleNameByDependencyItemObj(dependencyItemObj, dependencyItem.bundleName,
                defaultBundleName)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "GetDependencyBundleNameByDependencyItemObj failed!").c_str());
                return false;
            }
            if (!GetDependencyModuleNameByDependencyItemObj(dependencyItemObj, dependencyItem.moduleName)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "GetDependencyModuleNameByDependencyItemObj failed!").c_str());
                return false;
            }
            dependencyItems.push_back(dependencyItem);
        }
    }

    return true;
}

bool ModuleJson::CheckStageBundleType(const std::string& moduleName, const std::string& moduleType,
    const std::string& bundleType, const bool& installationFree)
{
    const bool hasSkillType = moduleType.compare(TYPE_SKILL) == 0;
    const bool hasSkillBundleType = bundleType.compare(TYPE_SKILL) == 0;
    if (hasSkillType != hasSkillBundleType) {
        std::string errMsg = "Invalid skill configuration in module '" + moduleName + "': 'moduleType' and "
            "'bundleType' must both be 'skill' when using skill. If this is a skill module, package it in "
            "HSP mode instead of HAP mode.";
        std::string solution = "Set both 'module.moduleType' and 'app.bundleType' to 'skill', or remove "
            "'skill' from both. Use HSP mode for skill modules.";
        LOGE("%s", PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs(
            std::vector<std::string>{errMsg, solution}).c_str());
        return false;
    }
    if (bundleType.compare(APP) == 0) {
        if (installationFree) {
            std::string errMsg = "'installationFree' must be false in module '" + moduleName +
                "' when 'bundleType' is app.";
            std::string solution = "Set 'installationFree' to false in the module.json when 'bundleType' is app.";
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(ATOMIC_SERVICE) == 0) {
        if (!installationFree) {
            std::string errMsg = "'installationFree' must be true in module '" + moduleName +
                "' when 'bundleType' is atomicService.";
            std::string solution = "Set 'installationFree' to true in the module.json when 'bundleType' "
                "is atomicService.";
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(SHARED) == 0) {
        if (moduleType.compare(SHARED) != 0) {
            std::string errMsg = "'type' must be shared in module '" + moduleName +
                "' when 'bundleType' is shared.";
            std::string solution = "Set the 'type' to shared in the module.json when 'bundleType' is shared.";
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(APP_SERVICE) == 0) {
        return true;
    } else if (bundleType.compare(APP_PLUGIN) == 0) {
        if (moduleType.compare(SHARED) != 0) {
            std::string errMsg = "'type' must be shared in module '" + moduleName +
                "' when 'bundleType' is appPlugin.";
            std::string solution = "Set the 'type' to shared in the module.json when 'bundleType' is appPlugin.";
            LOGE("%s", PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(TYPE_SKILL) == 0) {
        return true;
    }
    return false;
}

bool ModuleJson::GetAtomicServicePreloads(std::list<PreloadItem>& preloadItems)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems);
}

bool ModuleJson::GetAtomicServicePreloadsByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<PreloadItem>& preloadItems)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }

    if (!moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        return true;
    }
    std::unique_ptr<PtJson> atomicServiceObj;
    if (moduleObj->GetObject(ATOMIC_SERVICE.c_str(), &atomicServiceObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + ATOMIC_SERVICE + " node failed!").c_str()).c_str());
        return false;
    }

    if (!atomicServiceObj->Contains(PRELOADS.c_str())) {
        return true;
    }
    std::unique_ptr<PtJson> preloadsObj;
    if (atomicServiceObj->GetArray(PRELOADS.c_str(), &preloadsObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + PRELOADS + " array node failed!").c_str()).c_str());
        return false;
    }
    for (int32_t i = 0; i < preloadsObj->GetSize(); i++) {
        std::unique_ptr<PtJson> preloadObj = preloadsObj->Get(i);
        PreloadItem preloadItem;
        std::string moduleName;
        if (preloadObj->Contains(MODULE_NAME.c_str())) {
            if (preloadObj->GetString(MODULE_NAME.c_str(), &moduleName) == Result::SUCCESS) {
                preloadItem.moduleName = moduleName;
            }
        }
        preloadItems.push_back(preloadItem);
    }

    return true;
}

bool ModuleJson::GetAbilityContinueTypeMap(std::map<std::string, std::list<std::string>>& abilityContinueTypeMap)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap);
}

bool ModuleJson::GetAbilityNameByAbilityObj(std::unique_ptr<PtJson>& abilityObj,
    std::string& abilityName)
{
    if (!abilityObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Ability node is null!").c_str());
        return false;
    }
    abilityName = "";
    if (abilityObj->Contains(NAME.c_str())) {
        if (abilityObj->GetString(NAME.c_str(), &abilityName) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Ability node get " + NAME + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetContinueTypesByAbilityObj(std::unique_ptr<PtJson>& abilityObj,
    std::list<std::string>& continueTypes)
{
    if (!abilityObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Ability node is null!").c_str());
        return false;
    }
    if (abilityObj->Contains(CONTINUE_TYPE.c_str())) {
        std::unique_ptr<PtJson> continueTypeObj;
        if (abilityObj->GetArray(CONTINUE_TYPE.c_str(), &continueTypeObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + CONTINUE_TYPE + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < continueTypeObj->GetSize(); i++) {
            continueTypes.push_back(continueTypeObj->Get(i)->GetString());
        }
    }
    return true;
}

bool ModuleJson::GetAbilityContinueTypeMapByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::map<std::string, std::list<std::string>>& abilityContinueTypeMap)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(ABILITIES.c_str())) {
        std::unique_ptr<PtJson> abilitysObj;
        if (moduleObj->GetArray(ABILITIES.c_str(), &abilitysObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + PRELOADS + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < abilitysObj->GetSize(); i++) {
            std::unique_ptr<PtJson> abilityObj = abilitysObj->Get(i);
            std::string abilityName = "";
            if (!GetAbilityNameByAbilityObj(abilityObj, abilityName)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "GetAbilityNameByAbilityObj failed!").c_str());
                return false;
            }
            std::list<std::string> continueTypes;
            if (!GetContinueTypesByAbilityObj(abilityObj, continueTypes)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "GetContinueTypesByAbilityObj failed!").c_str());
                return false;
            }
            abilityContinueTypeMap.emplace(abilityName, continueTypes);
        }
    }
    return true;
}

bool ModuleJson::GetMultiAppMode(MultiAppMode& multiAppMode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetMultiAppModeByAppObj(appObj, multiAppMode);
}
bool ModuleJson::GetMultiAppModeByAppObj(std::unique_ptr<PtJson>& appObj, MultiAppMode& multiAppMode)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(MULTI_APP_MODE.c_str())) {
        std::unique_ptr<PtJson> multiAppModeObj;
        if (appObj->GetObject(MULTI_APP_MODE.c_str(), &multiAppModeObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + MULTI_APP_MODE + " node failed!").c_str()).c_str());
            return false;
        }
        multiAppMode.multiAppModeType = "";
        multiAppMode.maxCount = 0;
        if (multiAppModeObj->Contains(MULTI_APP_MODE_TYPE.c_str())) {
            if (multiAppModeObj->GetString(MULTI_APP_MODE_TYPE.c_str(),
                &multiAppMode.multiAppModeType) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("App node get " + MULTI_APP_MODE_TYPE + " failed!").c_str()).c_str());
                return false;
            }
        }
        if (multiAppModeObj->Contains(MULTI_APP_MODE_NUMBER.c_str())) {
            if (multiAppModeObj->GetInt(MULTI_APP_MODE_NUMBER.c_str(), &multiAppMode.maxCount) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("App node get " + MULTI_APP_MODE_NUMBER + " failed!").c_str()).c_str());
                return false;
            }
        }
    }
    return true;
}

bool ModuleJson::IsExistedStageRequestPermissions()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return moduleObj->Contains(REQUEST_PERMISSIONS.c_str());
}

bool ModuleJson::IsExistedStageModuleTargetPriority()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return moduleObj->Contains(TARGET_PRIORITY.c_str());
}

bool ModuleJson::IsExistedStageAppTargetPriority()
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return appObj->Contains(TARGET_PRIORITY.c_str());
}

bool ModuleJson::IsModuleAtomicServiceValid()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        LOGW("Module node has no %s node!", ATOMIC_SERVICE.c_str());
        return true;
    }
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
            LOGE("%s", PackingToolErrMsg::CHECK_MODULE_ATOMIC_SERVICE_FAILED.toStringWithArgs(
                "Module cannot config atomicService when 'bundleType' is missing.").c_str());
            return false;
        } else {
            std::string bundleType;
            if (appObj->GetString(BUNDLE_TYPE.c_str(), &bundleType) != Result::SUCCESS) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    ("App node get " + BUNDLE_TYPE + " failed!").c_str()).c_str());
                return false;
            }
            if (bundleType.compare(ATOMIC_SERVICE) != 0) {
                LOGE("%s", PackingToolErrMsg::CHECK_MODULE_ATOMIC_SERVICE_FAILED.toStringWithArgs(
                    "Module cannot config atomicService when 'bundleType' is not atomicService.").c_str());
                return false;
            }
        }
    }
    return true;
}

bool ModuleJson::CheckEntryInAtomicService()
{
    std::string bundleType;
    if (!GetStageBundleType(bundleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetStageBundleType failed!").c_str());
        return false;
    }
    if (bundleType.compare(ATOMIC_SERVICE) != 0) {
        return true;
    }
    std::string moduleType;
    std::list<std::string> abilityNames;
    if (!GetStageModuleType(moduleType) || !GetAbilityNames(abilityNames)) {
        LOGE("%s", PackingToolErrMsg::CHECK_LEASTONE_ABILITY.toStringWithArgs(
            "entry module must contain at least one ability.").c_str());
        return false;
    }
    if (moduleType.compare(ENTRY) == 0 && abilityNames.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_LEASTONE_ABILITY.toStringWithArgs(
            "entry module must contain at least one ability.").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckAtomicServiceInstallationFree()
{
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> appObj;
    if (!GetModuleObject(moduleObj) || !GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetAppObject or module node failed!").c_str());
        return false;
    }
    bool installationFree = false;
    GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (installationFree) {
            std::string errMsg =
                "The app.json5 file configuration does not match the 'installationFree' setting of true.";
            std::string solution = "Add the 'bundleType' field to the app.json5 file and set it atomicService.";
            LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
        return true;
    }
    std::string bundleType;
    if (!GetStageBundleType(bundleType)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetStageBundleType failed!").c_str());
        return false;
    }
    std::string moduleName;
    if (!GetStageModuleNameByModuleObj(moduleObj, moduleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetStageModuleNameByModuleObj failed!").c_str());
        return false;
    }
    if ((bundleType.compare(APP) == 0) ||
        (bundleType.compare(SHARED) == 0) ||
        (bundleType.compare(APP_SERVICE) == 0) ||
        (bundleType.compare(APP_PLUGIN) == 0)) {
        if (installationFree) {
            std::string errMsg = "'installationFree' must be false in module '" + moduleName +
                "' when 'bundleType' is " + bundleType + ".";
            std::string solution = "Set 'installationFree' to false in the module.json when 'bundleType' is " +
                bundleType + ".";
            LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
    } else if (bundleType.compare(ATOMIC_SERVICE) == 0) {
        if (!installationFree) {
            std::string errMsg = "'installationFree' must be true in module '" + moduleName +
                "' when 'bundleType' is atomicService.";
            std::string solution = "Set 'installationFree' to true in the module.json when 'bundleType' "
                "is atomicService.";
            LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED.toStringWithArgs(
                std::vector<std::string>{errMsg, solution}).c_str());
            return false;
        }
    } else if (bundleType.compare(TYPE_SKILL) == 0) {
        return true;
    } else {
        std::string errMsg = "'bundleType' is invalid in the app.json.";
        std::string solution = "Ensure that the 'bundleType' field in the app.json file is correctly set to one "
            "of the valid types: 'app', 'atomicService', 'shared', 'appService', 'appPlugin', or 'skill'.";
        LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED.toStringWithArgs(
            std::vector<std::string>{errMsg, solution}).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageAsanTsanEnabledValid()
{
    bool asanEnabled = false;
    bool tsanEnabled = false;
    if (!GetStageAsanEnabled(asanEnabled) || !GetStageTsanEnabled(tsanEnabled)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetStageAsanEnabled or GetStageTsanEnabled failed").c_str());
        return false;
    }

    if (asanEnabled && tsanEnabled) {
        LOGE("%s", PackingToolErrMsg::CHECK_AS_TSAN_ENABLED.toStringWithArgs(
            "asanEnabled and tsanEnabled cannot be true at the same time.").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageAtomicService()
{
    if (!IsModuleAtomicServiceValid()) {
        LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_FAILED.toStringWithArgs(
            "IsModuleAtomicServiceValid failed!").c_str());
        return false;
    }
    if (!CheckEntryInAtomicService()) {
        LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_FAILED.toStringWithArgs(
            "CheckEntryInAtomicService failed!").c_str());
        return false;
    }
    if (!CheckAtomicServiceInstallationFree()) {
        LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_FAILED.toStringWithArgs(
            "CheckAtomicServiceInstallationFree failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckQuerySchemes()
{
    int32_t rawMinAPIVersion = -1;
    GetMinApiVersion(rawMinAPIVersion);
    int32_t minAPIVersion = rawMinAPIVersion % 1000;
    if (minAPIVersion == -1 || minAPIVersion >= QUERY_SCHEMES_CHECK_MIN_API_VERSION) {
        return true;
    }
    std::list<std::string> querySchemes;
    GetQuerySchemes(querySchemes);
    int querySchemesCount = querySchemes.size();
    if (querySchemesCount > QUERY_SCHEMES_CHECK_COUNT) {
        //     QUERY_SCHEMES_CHECK_COUNT, QUERY_SCHEMES_CHECK_MIN_API_VERSION);
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            ("The number of querySchemes in the Hap(entry) exceeds " + std::to_string(QUERY_SCHEMES_CHECK_COUNT) +
            ", and the minAPIVersion is less than " + std::to_string(QUERY_SCHEMES_CHECK_MIN_API_VERSION)).c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckDeduplicateHar()
{
    int32_t rawMinAPIVersion = -1;
    GetMinApiVersion(rawMinAPIVersion);
    int32_t minAPIVersion = rawMinAPIVersion % 1000;
    if (minAPIVersion == -1 || minAPIVersion >= DEDUPLICATE_HAR_CHECK_MIN_API_VERSION) {
        return true;
    }
    bool deduplicateHar = false;
    GetDeduplicateHar(deduplicateHar);
    if (deduplicateHar) {
        std::string moduleName;
        GetStageModuleName(moduleName);
        //     moduleName.c_str(), minAPIVersion, DEDUPLICATE_HAR_CHECK_MIN_API_VERSION);
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            ("The deduplicateHar of module " + moduleName + " is true, and the minAPIVersion(" +
            std::to_string(minAPIVersion) + ") is less than " +
            std::to_string(DEDUPLICATE_HAR_CHECK_MIN_API_VERSION)).c_str()).c_str());
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageOverlayCfg()
{
    std::string targetModuleName;
    if (!GetTargetModuleName(targetModuleName)) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "GetTargetModuleName failed.").c_str());
        return false;
    }
    if (!targetModuleName.empty()) {
        if (IsExistedStageRequestPermissions()) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "targetModuleName cannot be existed with requestPermissions.").c_str());
            return false;
        }
        std::string moduleName;
        if (!GetStageModuleName(moduleName)) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "GetModuleName failed.").c_str());
            return false;
        }
        if (targetModuleName == moduleName) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "targetModuleName cannot be same with name in the overlay module.").c_str());
            return false;
        }
    } else if (IsExistedStageModuleTargetPriority()) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "targetPriority cannot be existed without the targetModuleName in module.json.").c_str());
        return false;
    }

    std::string targetBundleName;
    if (!GetTargetBundleName(targetBundleName)) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "GetTargetBundleName failed.").c_str());
        return false;
    }
    if (!targetBundleName.empty()) {
        if (targetModuleName.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "targetModuleName is necessary in the overlay bundle.").c_str());
            return false;
        }
        std::string bundleName;
        if (!GetBundleName(bundleName)) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "GetModuleName failed.").c_str());
            return false;
        }
        if (targetBundleName == bundleName) {
            LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
                "targetBundleName cannot be same with the bundleName.").c_str());
            return false;
        }
    } else if (IsExistedStageAppTargetPriority()) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "targetPriority cannot be existed without the targetBundleName in app.json.").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetGenerateBuildHash(bool& generateBuildHash)
{
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> appObj;
    if (!GetModuleObject(moduleObj) || !GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::HAS_GENERATE_BUILD_HASH.toStringWithArgs(
            "GetAppObject or module node failed!").c_str());
        return false;
    }
    if (appObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        if (appObj->GetBool(GENERATE_BUILD_HASH.c_str(), &generateBuildHash) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::HAS_GENERATE_BUILD_HASH.toStringWithArgs(
                ("App node get " + GENERATE_BUILD_HASH + " failed!").c_str()).c_str());
            return false;
        }
    } else if (moduleObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        if (moduleObj->GetBool(GENERATE_BUILD_HASH.c_str(), &generateBuildHash) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::HAS_GENERATE_BUILD_HASH.toStringWithArgs(
                ("Module node get " + GENERATE_BUILD_HASH + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        LOGW("App node and module node are both not contain %s node", GENERATE_BUILD_HASH.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::RemoveGenerateBuildHash()
{
    if (!RemoveGenerateBuildHashFromAppObj() || !RemoveGenerateBuildHashFromModuleObj()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "RemoveGenerateBuildHashFromAppObj or RemoveGenerateBuildHashFromModuleObj failed!").c_str());
        return false;
    }
    return true;
}

bool ModuleJson::RemoveGenerateBuildHashFromAppObj()
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (appObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        LOGD("Remove %s nodefrom app node", GENERATE_BUILD_HASH.c_str());
        appObj->Remove(GENERATE_BUILD_HASH.c_str());
    }
    return true;
}

bool ModuleJson::RemoveGenerateBuildHashFromModuleObj()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (moduleObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        LOGD("Remove %s node from module node", GENERATE_BUILD_HASH.c_str());
        moduleObj->Remove(GENERATE_BUILD_HASH.c_str());
    }
    return true;
}

bool ModuleJson::GetNormalizeVersion(NormalizeVersion& normalizeVersion, const bool& isStage)
{
    Version version;
    if (isStage) {
        if (!GetStageVersion(version)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetStageVersion failed!").c_str());
            return false;
        }
    } else {
        if (!GetFaVersion(version)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetFaVersion failed!").c_str());
            return false;
        }
    }
    std::string moduleName;
    if (!GetModuleName(moduleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleName failed!").c_str());
        return false;
    }
    normalizeVersion.originVersionCode = version.versionCode;
    normalizeVersion.originVersionName = version.versionName;
    normalizeVersion.moduleName = moduleName;
    return true;
}

bool ModuleJson::SetVersionCodeAndName(const int32_t& versionCode, const std::string& versionName, const bool& isStage)
{
    if (isStage) {
        if (!SetStageVersionCode(versionCode) || !SetStageVersionName(versionName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageVersionCode or SetStageVersionName failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaVersionCode(versionCode) || !SetFaVersionName(versionName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaVersionCode or SetFaVersionName failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetBuildHash(const std::string& buildHash)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (moduleObj->Contains(BUILD_HASH.c_str())) {
        if (moduleObj->SetString(BUILD_HASH.c_str(), buildHash) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node set " + BUILD_HASH + " failed!").c_str()).c_str());
            return false;
        }
    } else {
        if (!moduleObj->Add(BUILD_HASH.c_str(), buildHash.c_str())) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node add " + BUILD_HASH + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetVersionCode(const int32_t& versionCode, const bool& isStage)
{
    if (isStage) {
        if (!SetStageVersionCode(versionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageVersionCode failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaVersionCode(versionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaVersionCode failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetVersionName(const std::string& versionName, const bool& isStage)
{
    if (isStage) {
        if (!SetStageVersionName(versionName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageVersionName failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaVersionName(versionName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaVersionName failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetBundleName(const std::string& bundleName, const bool& isStage)
{
    if (isStage) {
        if (!SetStageBundleName(bundleName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageBundleName failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaBundleName(bundleName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaBundleName failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode, const bool& isStage)
{
    if (isStage) {
        if (!SetStageMinCompatibleVersionCode(minCompatibleVersionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageMinCompatibleVersionCode failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaMinCompatibleVersionCode(minCompatibleVersionCode)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaMinCompatibleVersionCode failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetMinAPIVersion(const int32_t& minAPIVersion, const bool& isStage)
{
    if (isStage) {
        if (!SetStageMinAPIVersion(minAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageMinAPIVersion failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaMinAPIVersion(minAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaMinAPIVersion failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetTargetAPIVersion(const int32_t& targetAPIVersion, const bool& isStage)
{
    if (isStage) {
        if (!SetStageTargetAPIVersion(targetAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageTargetAPIVersion failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaTargetAPIVersion(targetAPIVersion)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaTargetAPIVersion failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetApiReleaseType(const std::string& apiReleaseType, const bool& isStage)
{
    if (isStage) {
        if (!SetStageApiReleaseType(apiReleaseType)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageApiReleaseType failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaApiReleaseType(apiReleaseType)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaApiReleaseType failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetBundleType(const std::string& bundleType, const bool& isStage)
{
    if (isStage) {
        if (!SetStageBundleType(bundleType)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageBundleType failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaBundleType(bundleType)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaBundleType failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetInstallationFree(const bool& installationFree, const bool& isStage)
{
    if (isStage) {
        if (!SetStageInstallationFree(installationFree)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageInstallationFree failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaInstallationFree(installationFree)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaInstallationFree failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetDeliveryWithInstall(const bool& deliveryWithInstall, const bool& isStage)
{
    if (isStage) {
        if (!SetStageDeliveryWithInstall(deliveryWithInstall)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageDeliveryWithInstall failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaDeliveryWithInstall(deliveryWithInstall)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaDeliveryWithInstall failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetDeviceTypes(const std::list<std::string>& deviceTypes, const bool& isStage)
{
    if (isStage) {
        if (!SetStageDeviceTypes(deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetStageDeviceTypen failed!").c_str());
            return false;
        }
    } else {
        if (!SetFaDeviceTypes(deviceTypes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "SetFaDeviceTypen failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetMinApiVersion(int32_t& minAPIVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(MIN_API_VERSION.c_str())) {
        if (appObj->GetInt(MIN_API_VERSION.c_str(), &minAPIVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + MIN_API_VERSION + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetTargetApiVersion(int32_t& targetAPIVersion)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(TARGET_API_VERSION.c_str())) {
        if (appObj->GetInt(TARGET_API_VERSION.c_str(), &targetAPIVersion) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + TARGET_API_VERSION + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetDeliveryWithInstall(bool& deliveryWithInstall)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
        if (moduleObj->GetBool(DELIVERY_WITH_INSTALL.c_str(), &deliveryWithInstall) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + DELIVERY_WITH_INSTALL + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetQuerySchemes(std::list<std::string>& querySchemes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetQuerySchemesByModuleObj(moduleObj, querySchemes);
}

bool ModuleJson::GetQuerySchemesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& querySchemes)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (moduleObj->Contains(QUERY_SCHEMES.c_str())) {
        std::unique_ptr<PtJson> querySchemesObj;
        if (moduleObj->GetArray(QUERY_SCHEMES.c_str(), &querySchemesObj) != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Module node get " + QUERY_SCHEMES + " array node failed!").c_str()).c_str());
            return false;
        }
        if (!GetQuerySchemesByArray(querySchemesObj, querySchemes)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                "GetQuerySchemesByArray failed!").c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetQuerySchemesByArray(std::unique_ptr<PtJson>& querySchemesObj,
    std::list<std::string>& querySchemes)
{
    if (!querySchemesObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("querySchemes node is null!").c_str());
        return false;
    }
    for (int32_t i = 0; i < querySchemesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> obj = querySchemesObj->Get(i);
        if (obj == nullptr) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("querySchemesObj->Get(" + std::to_string(i) + ") returned null").c_str()).c_str());
            continue;
        }
        querySchemes.push_back(obj->GetString());
    }
    return true;
}

bool ModuleJson::GetDeduplicateHar(bool &deduplicateHar)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetDeduplicateHar() -> GetModuleObject failed!").c_str());
        return false;
    }
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            "GetDeduplicateHar() -> Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEDUPLICATE_HAR.c_str())) {
        return false;
    }
    if (moduleObj->GetBool(DEDUPLICATE_HAR.c_str(), &deduplicateHar) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Module node get " + DEDUPLICATE_HAR + " bool node failed!").c_str()).c_str());
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
