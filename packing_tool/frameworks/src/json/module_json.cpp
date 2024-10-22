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

bool ModuleJson::ParseFromString(const std::string& jsonString)
{
    Release();
    if (jsonString.length() == 0) {
        LOGE("Json length is zero!");
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
        LOGE("get real json file failed! jsonFile=%s", jsonFile.c_str());
        return false;
    }
    std::ifstream inFile(realJsonFile, std::ios::in);
    if (!inFile.is_open()) {
        LOGE("Open json file failed! jsonFile=%s, realJsonFile=%s", jsonFile.c_str(), realJsonFile.c_str());
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
        LOGE("Json root is null!");
        return false;
    }
    if (!root_->Contains(APP.c_str())) {
        LOGE("Json root has no %s node!", APP.c_str());
        return false;
    }
    if (root_->GetObject(APP.c_str(), &appObj) != Result::SUCCESS) {
        LOGE("Json root get %s node failed!", APP.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetDeviceConfigObject(std::unique_ptr<PtJson>& deviceConfigObj)
{
    if (root_.get() == nullptr) {
        LOGE("Json root is null!");
        return false;
    }
    if (!root_->Contains(DEVICE_CONFIG.c_str())) {
        LOGE("Json root has no %s node!", DEVICE_CONFIG.c_str());
        return false;
    }
    if (root_->GetObject(DEVICE_CONFIG.c_str(), &deviceConfigObj) != Result::SUCCESS) {
        LOGE("Json root get %s node failed!", DEVICE_CONFIG.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetVersionObject(std::unique_ptr<PtJson>& versionObj)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!appObj->Contains(VERSION.c_str())) {
        LOGE("App node has no %s node!", VERSION.c_str());
        return false;
    }
    if (appObj->GetObject(VERSION.c_str(), &versionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", VERSION.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetModuleObject(std::unique_ptr<PtJson>& moduleObj)
{
    if (root_.get() == nullptr) {
        LOGE("Json root is null!");
        return false;
    }
    if (!root_->Contains(MODULE.c_str())) {
        LOGE("Json root has no %s node!", MODULE.c_str());
        return false;
    }
    if (root_->GetObject(MODULE.c_str(), &moduleObj) != Result::SUCCESS) {
        LOGE("Json root get %s node failed!", MODULE.c_str());
        return false;
    }
    return true;
}


bool ModuleJson::GetDistroObject(std::unique_ptr<PtJson>& distroObj)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetDistroObjectByModuleObj(moduleObj, distroObj);
}

bool ModuleJson::GetDistroObjectByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::unique_ptr<PtJson>& distroObj)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(DISTRO.c_str())) {
        LOGE("Module node has no %s node!", DISTRO.c_str());
        return false;
    }
    if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
        LOGE("Module node get %s node failed!", DISTRO.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetApiVersionObject(std::unique_ptr<PtJson>& apiVersionObj)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!appObj->Contains(API_VERSION.c_str())) {
        LOGE("App node has no %s node!", API_VERSION.c_str());
        return false;
    }
    if (appObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        LOGE("App node get %s node failed!", API_VERSION.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetModuleName(std::string& moduleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetModuleNameByModuleObj(moduleObj, moduleName);
}

bool ModuleJson::GetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleName)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("Module node has no %s node!", NAME.c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &moduleName) != Result::SUCCESS) {
        LOGE("Module node get %s failed!", NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetPatchModuleName(std::string& patchModuleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("Module node has no %s node!", NAME.c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &patchModuleName) != Result::SUCCESS) {
        LOGE("Module node get %s failed!", NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetBundleName(std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetBundleNameByAppObj(appObj, bundleName);
}

bool ModuleJson::GetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& bundleName)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        LOGE("App node has no %s node!", BUNDLE_NAME.c_str());
        return false;
    }
    if (appObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
        LOGE("App node get %s failed!", BUNDLE_NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::SetBundleName(const std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        LOGE("App node has no %s node!", BUNDLE_NAME.c_str());
        return false;
    }
    if (appObj->SetString(BUNDLE_NAME.c_str(), bundleName) != Result::SUCCESS) {
        LOGE("App node set %s failed!", BUNDLE_NAME.c_str());
        return false;
    }
    return true;
}

bool ModuleJson::GetVendor(std::string& vendor)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetVendorByAppObj(appObj, vendor);
}

bool ModuleJson::GetVendorByAppObj(std::unique_ptr<PtJson>& appObj, std::string& vendor)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(VENDOR.c_str())) {
        if (appObj->GetString(VENDOR.c_str(), &vendor) != Result::SUCCESS) {
            LOGE("App node get %s failed!", VENDOR.c_str());
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
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetTargetBundleNameByAppObj(appObj, targetBundleName);
}

bool ModuleJson::GetTargetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& targetBundleName)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(TARGET_BUNDLE_NAME.c_str())) {
        if (appObj->GetString(TARGET_BUNDLE_NAME.c_str(), &targetBundleName) != Result::SUCCESS) {
            LOGE("App node get %s failed!", TARGET_BUNDLE_NAME.c_str());
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
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetTargetPriorityByAppObj(appObj, targetPriority);
}

bool ModuleJson::GetTargetPriorityByAppObj(std::unique_ptr<PtJson>& appObj, int32_t& targetPriority)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(TARGET_PRIORITY.c_str())) {
        if (appObj->GetInt(TARGET_PRIORITY.c_str(), &targetPriority) != Result::SUCCESS) {
            LOGE("App node get %s failed!", TARGET_PRIORITY.c_str());
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
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetTargetModuleNameByModuleObj(moduleObj, targetModuleName);
}

bool ModuleJson::GetTargetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& targetModuleName)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(TARGET_MODULE_NAME.c_str())) {
        if (moduleObj->GetString(TARGET_MODULE_NAME.c_str(), &targetModuleName) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", TARGET_MODULE_NAME.c_str());
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
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetTargetModulePriorityByModuleObj(moduleObj, targetModulePriority);
}

bool ModuleJson::GetTargetModulePriorityByModuleObj(std::unique_ptr<PtJson>& moduleObj, int32_t& targetModulePriority)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(TARGET_PRIORITY.c_str())) {
        if (moduleObj->GetInt(TARGET_PRIORITY.c_str(), &targetModulePriority) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", TARGET_PRIORITY.c_str());
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
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos);
}

bool ModuleJson::GetModuleMetadataInfoByModuleMetadataInfoObj(std::unique_ptr<PtJson>& moduleMetadataInfoObj,
    const std::map<std::string, std::string>& resourceMap, ModuleMetadataInfo& moduleMetadataInfo)
{
    if (!moduleMetadataInfoObj) {
        LOGE("ModuleMetadataInfo node is null!");
        return false;
    }
    if (moduleMetadataInfoObj->Contains(NAME.c_str())) {
        if (moduleMetadataInfoObj->GetString(NAME.c_str(), &moduleMetadataInfo.name) != Result::SUCCESS) {
            LOGE("ModuleMetadataInfo node get %s failed!", NAME.c_str());
            return false;
        }
    }
    if (moduleMetadataInfoObj->Contains(VALUE.c_str())) {
        if (moduleMetadataInfoObj->GetString(VALUE.c_str(), &moduleMetadataInfo.value) != Result::SUCCESS) {
            LOGE("ModuleMetadataInfo node get %s failed!", VALUE.c_str());
            return false;
        }
    }
    if (moduleMetadataInfoObj->Contains(RESOURCE.c_str())) {
        std::string resource;
        if (moduleMetadataInfoObj->GetString(RESOURCE.c_str(), &resource) != Result::SUCCESS) {
            LOGE("ModuleMetadataInfo node get %s failed!", RESOURCE.c_str());
            return false;
        }
        std::string fileName = Utils::ReplaceAll(resource, PROFILE, "") + JSON_PREFIX;
        auto iter = resourceMap.find(fileName);
        if (iter == resourceMap.end()) {
            LOGE("find filename in resourceMap failed!");
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
        LOGE("Module node is null!");
        return false;
    }
    std::unique_ptr<PtJson> moduleMetadataInfosObj;
    if (moduleObj->Contains(METADATA.c_str())) {
        if (moduleObj->GetArray(METADATA.c_str(), &moduleMetadataInfosObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", METADATA.c_str());
            return false;
        }
        for (int32_t i = 0; i < moduleMetadataInfosObj->GetSize(); i++) {
            ModuleMetadataInfo moduleMetadataInfo;
            std::unique_ptr<PtJson> moduleMetadataInfoObj = moduleMetadataInfosObj->Get(i);
            if (!GetModuleMetadataInfoByModuleMetadataInfoObj(moduleMetadataInfoObj, resourceMap,
                moduleMetadataInfo)) {
                LOGE("GetModuleMetadataInfoByModuleMetadataInfoObj failed!");
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
            LOGE("DistroFilter node is null!");
            return false;
        }
        std::unique_ptr<PtJson> distroFilterObj;
        if (distroFilterJsonObj->Contains(DISTRIBUTION_FILTER.c_str())) {
            if (distroFilterJsonObj->GetObject(DISTRIBUTION_FILTER.c_str(), &distroFilterObj) != Result::SUCCESS) {
                LOGE("DistroFilter node get %s failed!", DISTRO_FILTER.c_str());
                return false;
            }
        } else if (distroFilterJsonObj->Contains(DISTRO_FILTER.c_str())) {
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

bool ModuleJson::GetAbilityNames(std::list<std::string>& abilityNames)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetAbilityNamesByModuleObj(moduleObj, abilityNames);
}

bool ModuleJson::GetAbilityNamesByAbilitiesObj(std::unique_ptr<PtJson>& abilitiesObj,
    std::list<std::string>& abilityNames)
{
    if (!abilitiesObj) {
        LOGE("Abilities node is null!");
        return false;
    }
    for (int32_t i = 0; i < abilitiesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> abilityObj = abilitiesObj->Get(i);
        if (abilityObj->Contains(NAME.c_str())) {
            std::string name;
            if (abilityObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
                LOGE("Ability node get %s failed!", NAME.c_str());
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
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(ABILITIES.c_str())) {
        std::unique_ptr<PtJson> abilitiesObj;
        if (moduleObj->GetArray(ABILITIES.c_str(), &abilitiesObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", ABILITIES.c_str());
            return false;
        }
        if (!GetAbilityNamesByAbilitiesObj(abilitiesObj, abilityNames)) {
            LOGE("GetAbilityNamesByAbilitiesObj failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetProxyDataUris(std::list<std::string>& proxyDataUris)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetProxyDataUrisByModuleObj(moduleObj, proxyDataUris);
}

bool ModuleJson::GetProxyDataUrisByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& proxyDataUris)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(PROXY_DATAS.c_str())) {
        std::unique_ptr<PtJson> proxyDatasObj;
        if (moduleObj->GetArray(PROXY_DATAS.c_str(), &proxyDatasObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", PROXY_DATAS.c_str());
            return false;
        }
        if (!GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris)) {
            LOGE("GetProxyDataUrisByProxyDatasObj failed!");
            return false;
        }
    } else if (moduleObj->Contains(PROXY_DATA.c_str())) {
        std::unique_ptr<PtJson> proxyDatasObj;
        if (moduleObj->GetArray(PROXY_DATA.c_str(), &proxyDatasObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", PROXY_DATA.c_str());
            return false;
        }
        if (!GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris)) {
            LOGE("GetProxyDataUrisByProxyDatasObj failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetProxyDataUrisByProxyDatasObj(std::unique_ptr<PtJson>& proxyDatasObj,
    std::list<std::string>& proxyDataUris)
{
    if (!proxyDatasObj || !proxyDatasObj->IsArray()) {
        LOGE("ProxyData node is null or is not array!");
        return false;
    }
    for (int32_t i = 0; i < proxyDatasObj->GetSize(); i++) {
        std::unique_ptr<PtJson> proxyDataObj = proxyDatasObj->Get(i);
        if (!proxyDataObj->Contains(PROXY_URI.c_str())) {
            LOGE("proxyData node has no %s node!", PROXY_URI.c_str());
            return false;
        }
        std::string proxyUri;
        if (proxyDataObj->GetString(PROXY_URI.c_str(), &proxyUri) != Result::SUCCESS) {
            LOGE("ProxyData node get %s failed!", PROXY_URI.c_str());
            return false;
        }
        proxyDataUris.push_back(proxyUri);
    }
    return true;
}

bool ModuleJson::GetExtensionAbilityNames(std::list<std::string>& extensionAbilityNames)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetAbilityNamesByModuleObj(moduleObj, extensionAbilityNames);
}

bool ModuleJson::GetExtensionAbilityNamesByExtensionAbilityObj(std::unique_ptr<PtJson>& extensionAbilitiesObj,
    std::list<std::string>& extensionAbilityNames)
{
    if (!extensionAbilitiesObj) {
        LOGE("ExtensionAbilities node is null!");
        return false;
    }
    for (int32_t i = 0; i < extensionAbilitiesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> extensionAbilityObj = extensionAbilitiesObj->Get(i);
        if (extensionAbilityObj->Contains(NAME.c_str())) {
            std::string extensionAbilityName;
            if (extensionAbilityObj->GetString(NAME.c_str(), &extensionAbilityName) != Result::SUCCESS) {
                LOGE("ExtensionAbility node get %s failed!", NAME.c_str());
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
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(EXTENSION_ABILITIES.c_str())) {
        std::unique_ptr<PtJson> extensionAbilitiesObj;
        if (moduleObj->GetArray(EXTENSION_ABILITIES.c_str(), &extensionAbilitiesObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", EXTENSION_ABILITIES.c_str());
            return false;
        }
        if (!GetExtensionAbilityNamesByExtensionAbilityObj(extensionAbilitiesObj, extensionAbilityNames)) {
            LOGE("GetExtensionAbilityNamesByExtensionAbilityObj failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetDependencyItems(std::list<DependencyItem>& dependencyItems, const std::string& defaultBundleName)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName);
}

bool ModuleJson::GetDependencyBundleNameByDependencyItemObj(std::unique_ptr<PtJson>& dependencyItemObj,
    std::string& bundleName, const std::string& defaultBundleName)
{
    if (!dependencyItemObj) {
        LOGE("dependencyItem node is null!");
        return false;
    }
    if (dependencyItemObj->Contains(BUNDLE_NAME.c_str())) {
        if (dependencyItemObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
            LOGE("DependencyItem node get %s failed!", BUNDLE_NAME.c_str());
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
        LOGE("dependencyItem node is null!");
        return false;
    }
    moduleName = "";
    if (dependencyItemObj->Contains(MODULE_NAME.c_str())) {
        if (dependencyItemObj->GetString(MODULE_NAME.c_str(), &moduleName) != Result::SUCCESS) {
            LOGE("DependencyItem node get %s failed!", MODULE_NAME.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetDependencyItemsByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<DependencyItem>& dependencyItems, const std::string& defaultBundleName)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(DEPENDENCIES.c_str())) {
        std::unique_ptr<PtJson> dependencyItemsObj;
        if (moduleObj->GetArray(DEPENDENCIES.c_str(), &dependencyItemsObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", DEPENDENCIES.c_str());
            return false;
        }
        for (int32_t i = 0; i < dependencyItemsObj->GetSize(); i++) {
            std::unique_ptr<PtJson> dependencyItemObj = dependencyItemsObj->Get(i);
            DependencyItem dependencyItem;
            if (!GetDependencyBundleNameByDependencyItemObj(dependencyItemObj, dependencyItem.bundleName,
                defaultBundleName)) {
                LOGE("GetDependencyBundleNameByDependencyItemObj failed!");
                return false;
            }
            if (!GetDependencyModuleNameByDependencyItemObj(dependencyItemObj, dependencyItem.moduleName)) {
                LOGE("GetDependencyModuleNameByDependencyItemObj failed!");
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
    if (bundleType.compare(APP) == 0) {
        if (installationFree) {
            LOGE("installationFree must be false in module %s when bundleType is app", moduleName.c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(ATOMIC_SERVICE) == 0) {
        if (!installationFree) {
            LOGE("installationfree must be true in module %s when bundleType is atomicService",
                moduleName.c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(SHARED) == 0) {
        if (moduleType.compare(SHARED) != 0) {
            LOGE("moduleType must be shared bundleType is shared[bundleType=%s][moduleType=%s]",
                bundleType.c_str(), moduleType.c_str());
            return false;
        }
        return true;
    } else if (bundleType.compare(APP_SERVICE) == 0) {
        return true;
    }
    return false;
}

bool ModuleJson::GetAtomicServicePreloads(std::list<PreloadItem>& preloadItems)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems);
}

bool ModuleJson::GetAtomicServicePreloadsByModuleObj(std::unique_ptr<PtJson>& moduleObj,
    std::list<PreloadItem>& preloadItems)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }

    if (!moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        return true;
    }
    std::unique_ptr<PtJson> atomicServiceObj;
    if (moduleObj->GetObject(ATOMIC_SERVICE.c_str(), &atomicServiceObj) != Result::SUCCESS) {
        LOGE("Module node get %s node failed!", ATOMIC_SERVICE.c_str());
        return false;
    }

    if (!atomicServiceObj->Contains(PRELOADS.c_str())) {
        return true;
    }
    std::unique_ptr<PtJson> preloadsObj;
    if (atomicServiceObj->GetArray(PRELOADS.c_str(), &preloadsObj) != Result::SUCCESS) {
        LOGE("Module node get %s array node failed!", PRELOADS.c_str());
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
        LOGE("GetModuleObject failed!");
        return false;
    }
    return GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap);
}

bool ModuleJson::GetAbilityNameByAbilityObj(std::unique_ptr<PtJson>& abilityObj,
    std::string& abilityName)
{
    if (!abilityObj) {
        LOGE("Ability node is null!");
        return false;
    }
    abilityName = "";
    if (abilityObj->Contains(NAME.c_str())) {
        if (abilityObj->GetString(NAME.c_str(), &abilityName) != Result::SUCCESS) {
            LOGE("Ability node get %s failed!", NAME.c_str());
            return false;
        }
    }
    return true;
}

bool ModuleJson::GetContinueTypesByAbilityObj(std::unique_ptr<PtJson>& abilityObj,
    std::list<std::string>& continueTypes)
{
    if (!abilityObj) {
        LOGE("Ability node is null!");
        return false;
    }
    if (abilityObj->Contains(CONTINUE_TYPE.c_str())) {
        std::unique_ptr<PtJson> continueTypeObj;
        if (abilityObj->GetArray(CONTINUE_TYPE.c_str(), &continueTypeObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", CONTINUE_TYPE.c_str());
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
        LOGE("Module node is null!");
        return false;
    }
    if (moduleObj->Contains(ABILITIES.c_str())) {
        std::unique_ptr<PtJson> abilitysObj;
        if (moduleObj->GetArray(ABILITIES.c_str(), &abilitysObj) != Result::SUCCESS) {
            LOGE("Module node get %s array node failed!", PRELOADS.c_str());
            return false;
        }
        for (int32_t i = 0; i < abilitysObj->GetSize(); i++) {
            std::unique_ptr<PtJson> abilityObj = abilitysObj->Get(i);
            std::string abilityName = "";
            if (!GetAbilityNameByAbilityObj(abilityObj, abilityName)) {
                LOGE("GetAbilityNameByAbilityObj failed!");
                return false;
            }
            std::list<std::string> continueTypes;
            if (!GetContinueTypesByAbilityObj(abilityObj, continueTypes)) {
                LOGE("GetContinueTypesByAbilityObj failed!");
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
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetMultiAppModeByAppObj(appObj, multiAppMode);
}
bool ModuleJson::GetMultiAppModeByAppObj(std::unique_ptr<PtJson>& appObj, MultiAppMode& multiAppMode)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(MULTI_APP_MODE.c_str())) {
        std::unique_ptr<PtJson> multiAppModeObj;
        if (appObj->GetObject(MULTI_APP_MODE.c_str(), &multiAppModeObj) != Result::SUCCESS) {
            LOGE("App node get %s node failed!", MULTI_APP_MODE.c_str());
            return false;
        }
        multiAppMode.multiAppModeType = "";
        multiAppMode.maxCount = 0;
        if (multiAppModeObj->Contains(MULTI_APP_MODE_TYPE.c_str())) {
            if (multiAppModeObj->GetString(MULTI_APP_MODE_TYPE.c_str(),
                &multiAppMode.multiAppModeType) != Result::SUCCESS) {
                LOGE("App node get %s failed!", MULTI_APP_MODE_TYPE.c_str());
                return false;
            }
        }
        if (multiAppModeObj->Contains(MULTI_APP_MODE_NUMBER.c_str())) {
            if (multiAppModeObj->GetInt(MULTI_APP_MODE_NUMBER.c_str(), &multiAppMode.maxCount) != Result::SUCCESS) {
                LOGE("App node get %s failed!", MULTI_APP_MODE_NUMBER.c_str());
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
        LOGE("GetModuleObject failed!");
        return false;
    }
    return moduleObj->Contains(REQUEST_PERMISSIONS.c_str());
}

bool ModuleJson::IsExistedStageModuleTargetPriority()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    return moduleObj->Contains(TARGET_PRIORITY.c_str());
}

bool ModuleJson::IsExistedStageAppTargetPriority()
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return appObj->Contains(TARGET_PRIORITY.c_str());
}

bool ModuleJson::IsModuleAtomicServiceValid()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    if (!moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        LOGE("Module node has no %s node!", ATOMIC_SERVICE.c_str());
        return true;
    }
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    if (moduleObj->Contains(ATOMIC_SERVICE.c_str())) {
        if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
            LOGE("Module cannotconfig atomicService when app node has no bundleType");
            return false;
        } else {
            std::string bundleType;
            if (appObj->GetString(BUNDLE_TYPE.c_str(), &bundleType) != Result::SUCCESS) {
                LOGE("App node get %s failed!", BUNDLE_TYPE.c_str());
                return false;
            }
            if (bundleType.compare(ATOMIC_SERVICE) != 0) {
                LOGE("Module can not config atomicService when bundleType is not atomicService.");
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
        LOGE("GetStageBundleType failed!");
        return false;
    }
    if (bundleType.compare(ATOMIC_SERVICE) != 0) {
        return true;
    }
    std::string moduleType;
    std::list<std::string> abilityNames;
    if (!GetStageModuleType(moduleType) || !GetAbilityNames(abilityNames)) {
        LOGE("entry module must contain at least one ability.");
        return false;
    }
    if (moduleType.compare(ENTRY) == 0 && abilityNames.empty()) {
        LOGE("entry module must contain at least one ability.");
        return false;
    }
    return true;
}

bool ModuleJson::CheckAtomicServiceInstallationFree()
{
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> appObj;
    if (!GetModuleObject(moduleObj) || !GetAppObject(appObj)) {
        LOGE("GetAppObject or module node failed!");
        return false;
    }
    bool installationFree = false;
    GetStageInstallationFreeByModuleObj(moduleObj, installationFree);
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (installationFree) {
            LOGE("installationFree must be false when app node has no %s", BUNDLE_TYPE.c_str());
            return false;
        }
        return true;
    }
    std::string bundleType;
    if (!GetStageBundleType(bundleType)) {
        LOGE("GetStageBundleType failed!");
        return false;
    }
    std::string moduleName;
    if (!GetStageModuleNameByModuleObj(moduleObj, moduleName)) {
        LOGE("GetStageModuleNameByModuleObj failed!");
        return false;
    }
    if ((bundleType.compare(APP) == 0) ||
        (bundleType.compare(SHARED) == 0) ||
        (bundleType.compare(APP_SERVICE) == 0)) {
        if (installationFree) {
            LOGE("installationFree must be false in module %s when bundleType is app/shared/appService",
                moduleName.c_str());
            return false;
        }
    } else if (bundleType.compare(ATOMIC_SERVICE) == 0) {
        if (!installationFree) {
            LOGE("installationfree must be true in module %s when bundleType is atomicService", moduleName.c_str());
            return false;
        }
    } else {
        LOGE("bundleType is not app/shared/appService/atomicService");
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageAsanTsanEnabledValid()
{
    bool asanEnabled = false;
    bool tsanEnabled = false;
    if (!GetStageAsanEnabled(asanEnabled) || !GetStageTsanEnabled(tsanEnabled)) {
        LOGE("GetStageAsanEnabled or GetStageTsanEnabled failed");
        return false;
    }
    
    if (asanEnabled && tsanEnabled) {
        LOGE("asanEnabled and tsanEnabled cannot be true at the same time.");
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageAtomicService()
{
    if (!IsModuleAtomicServiceValid()) {
        LOGE("IsModuleAtomicServiceValid failed!");
        return false;
    }
    if (!CheckEntryInAtomicService()) {
        LOGE("CheckEntryInAtomicService failed!");
        return false;
    }
    if (!CheckAtomicServiceInstallationFree()) {
        LOGE("CheckAtomicServiceInstallationFree failed!");
        return false;
    }
    return true;
}

bool ModuleJson::CheckStageOverlayCfg()
{
    std::string targetModuleName;
    if (!GetTargetModuleName(targetModuleName)) {
        LOGE("GetTargetModuleName failed.");
        return false;
    }
    if (!targetModuleName.empty()) {
        if (IsExistedStageRequestPermissions()) {
            LOGE("targetModuleName cannot be existed with requestPermissions.");
            return false;
        }
        std::string moduleName;
        if (!GetStageModuleName(moduleName)) {
            LOGE("GetModuleName failed.");
            return false;
        }
        if (targetModuleName == moduleName) {
            LOGE("targetModuleName cannot be same with name in the overlay module.");
            return false;
        }
    } else if (IsExistedStageModuleTargetPriority()) {
        LOGE("targetPriority cannot be existed without the targetModuleName in module.json.");
        return false;
    }

    std::string targetBundleName;
    if (!GetTargetBundleName(targetBundleName)) {
        LOGE("GetTargetBundleName failed.");
        return false;
    }
    if (!targetBundleName.empty()) {
        if (targetModuleName.empty()) {
            LOGE("targetModuleName is necessary in the overlay bundle.");
            return false;
        }
        std::string bundleName;
        if (!GetBundleName(bundleName)) {
            LOGE("GetModuleName failed.");
            return false;
        }
        if (targetBundleName == bundleName) {
            LOGE("targetBundleName cannot be same with the bundleName.");
            return false;
        }
    } else if (IsExistedStageAppTargetPriority()) {
        LOGE("targetPriority cannot be existed without the targetBundleName in app.json.");
        return false;
    }
    return true;
}

bool ModuleJson::GetGenerateBuildHash(bool& generateBuildHash)
{
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> appObj;
    if (!GetModuleObject(moduleObj) || !GetAppObject(appObj)) {
        LOGE("GetAppObject or module node failed!");
        return false;
    }
    if (appObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        if (appObj->GetBool(GENERATE_BUILD_HASH.c_str(), &generateBuildHash) != Result::SUCCESS) {
            LOGE("App node get %s failed!", GENERATE_BUILD_HASH.c_str());
            return false;
        }
    } else if (moduleObj->Contains(GENERATE_BUILD_HASH.c_str())) {
        if (moduleObj->GetBool(GENERATE_BUILD_HASH.c_str(), &generateBuildHash) != Result::SUCCESS) {
            LOGE("Module node get %s failed!", GENERATE_BUILD_HASH.c_str());
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
        LOGE("RemoveGenerateBuildHashFromAppObj or RemoveGenerateBuildHashFromModuleObj failed!");
        return false;
    }
    return true;
}

bool ModuleJson::RemoveGenerateBuildHashFromAppObj()
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
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
        LOGE("GetModuleObject failed!");
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
        LOGE("GetStageVersion failed!");
            return false;
        }
    } else {
        if (!GetFaVersion(version)) {
        LOGE("GetFaVersion failed!");
            return false;
        }
    }
    std::string moduleName;
    if (!GetModuleName(moduleName)) {
        LOGE("GetModuleName failed!");
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
            LOGE("SetStageVersionCode or SetStageVersionName failed!");
            return false;
        }
    } else {
        if (!SetFaVersionCode(versionCode) || !SetFaVersionName(versionName)) {
            LOGE("SetFaVersionCode or SetFaVersionName failed!");
            return false;
        }
    }
    return true;
}

bool ModuleJson::SetBuildHash(const std::string& buildHash)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("GetModuleObject failed!");
        return false;
    }
    if (moduleObj->Contains(BUILD_HASH.c_str())) {
        if (moduleObj->SetString(BUILD_HASH.c_str(), buildHash) != Result::SUCCESS) {
            LOGE("Module node set %s failed!", BUILD_HASH.c_str());
            return false;
        }
    } else {
        if (!moduleObj->Add(BUILD_HASH.c_str(), buildHash.c_str())) {
            LOGE("Module node add %s failed!", BUILD_HASH.c_str());
            return false;
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
