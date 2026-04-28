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

#include "pack_info.h"

#include <algorithm>
#include <fstream>

#include "error/packing_tool_err_msg.h"
#include "log.h"
#include "utils.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string SUMMARY = "summary";
const std::string PACKAGES = "packages";
const std::string APP = "app";
const std::string MODULES = "modules";
const std::string BUNDLE_NAME = "bundleName";
const std::string BUNDLE_TYPE = "bundleType";
const std::string VERSION = "version";
const std::string CODE = "code";
const std::string BUILD = "build";
const std::string NAME = "name";
const std::string DISTRO = "distro";
const std::string MODULE_NAME = "moduleName";
const std::string EXTENSION_ABILITIES = "extensionAbilities";
const std::string FORMS = "forms";
const std::string DEFAULT_DIMENSION = "defaultDimension";
const std::string SUPPORT_DIMENSIONS = "supportDimensions";
const char ASTERISK = '*';
const std::string MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
const std::string MIN_API_VERSION = "minAPIVersion";
const std::string TARGET_API_VERSION = "targetAPIVersion";
const std::string API_RELEASE_TYPE = "apiReleaseType";
const std::string INSTALLATION_FREE = "installationFree";
const std::string DELIVERY_WITH_INSTALL = "deliveryWithInstall";
const std::string COMPILE_SDK_TYPE = "compileSdkType";
const std::string DEVICE_TYPE = "deviceType";
const std::string COMPATIBLE = "compatible";
const std::string RELEASE_TYPE = "releaseType";
const std::string TARGET = "target";
const std::string API_VERSION = "apiVersion";
}

bool PackInfo::ParseFromString(const std::string& jsonString)
{
    Release();
    if (jsonString.length() == 0) {
        // LOGE("Json length is zero!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json length is zero!").c_str());
        return false;
    }
    root_ = PtJson::Parse(jsonString);
    return IsValid();
}

bool PackInfo::ParseFromFile(const std::string& jsonFile)
{
    Release();
    std::string realJsonFile;
    if (!Utils::GetRealPath(jsonFile, realJsonFile)) {
        // LOGE("get real json file failed! jsonFile=%s", jsonFile.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("get real json file failed! jsonFile=") + jsonFile).c_str());
        return false;
    }
    std::ifstream inFile(realJsonFile, std::ios::in);
    if (!inFile.is_open()) {
        // LOGE("Open json file failed! jsonFile=%s, realJsonFile=%s", jsonFile.c_str(), realJsonFile.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Open json file failed! jsonFile=") + jsonFile + ", realJsonFile=" + realJsonFile).c_str());
        return false;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    root_ = PtJson::Parse(fileContent);
    return IsValid();
}

std::string PackInfo::ToString()
{
    return root_->Stringify();
}

void PackInfo::Release()
{
    if (root_) {
        root_->ReleaseRoot();
        root_ = nullptr;
    }
}

bool PackInfo::IsValid()
{
    return (root_.get() != nullptr);
}

bool PackInfo::GetSummaryObject(std::unique_ptr<PtJson>& summaryObj)
{
    if (root_.get() == nullptr) {
        // LOGE("Json root is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(SUMMARY.c_str())) {
        // LOGE("Json root has no %s node!", SUMMARY.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root has no ") + SUMMARY + " node!").c_str());
        return false;
    }
    if (root_->GetObject(SUMMARY.c_str(), &summaryObj) != Result::SUCCESS) {
        // LOGE("Json root get %s node failed!", SUMMARY.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root get ") + SUMMARY + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetPackagesObject(std::unique_ptr<PtJson>& packagesObj)
{
    if (root_.get() == nullptr) {
        // LOGE("Json root is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(PACKAGES.c_str())) {
        // LOGE("Json root has no %s node!", PACKAGES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root has no ") + PACKAGES + " node!").c_str());
        return false;
    }
    if (root_->GetArray(PACKAGES.c_str(), &packagesObj) != Result::SUCCESS) {
        // LOGE("Json root get %s array node failed!", PACKAGES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root get ") + PACKAGES + " array node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetAppObject(std::unique_ptr<PtJson>& appObj)
{
    std::unique_ptr<PtJson> summaryObj;
    if (!GetSummaryObject(summaryObj)) {
        // LOGE("GetSummaryObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetSummaryObject failed!").c_str());
        return false;
    }
    if (!summaryObj->Contains(APP.c_str())) {
        // LOGE("Summary node has no %s node!", APP.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Summary node has no ") + APP + " node!").c_str());
        return false;
    }
    if (summaryObj->GetObject(APP.c_str(), &appObj) != Result::SUCCESS) {
        // LOGE("Summary node get %s node failed!", APP.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Summary node get ") + APP + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetModulesObject(std::unique_ptr<PtJson>& modulesObj)
{
    std::unique_ptr<PtJson> summaryObj;
    if (!GetSummaryObject(summaryObj)) {
        // LOGE("GetSummaryObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetSummaryObject failed!").c_str());
        return false;
    }
    if (!summaryObj->Contains(MODULES.c_str())) {
        // LOGE("Summary node has no %s node!", MODULES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Summary node has no ") + MODULES + " node!").c_str());
        return false;
    }
    if (summaryObj->GetArray(MODULES.c_str(), &modulesObj) != Result::SUCCESS) {
        // LOGE("Summary node get %s node failed!", MODULES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Summary node get ") + MODULES + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetBundleName(std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        // LOGE("GetAppObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetBundleNameByAppObj(appObj, bundleName);
}

bool PackInfo::GetBundleNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleName)
{
    if (!appObj) {
        // LOGE("App node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        // LOGE("App node has no %s node!", BUNDLE_NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + BUNDLE_NAME + " node!").c_str());
        return false;
    }
    if (appObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
        // LOGE("App node get %s failed!", BUNDLE_NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + BUNDLE_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetBundleName(const std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        // LOGE("GetAppObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        // LOGE("App node has no %s node!", BUNDLE_NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + BUNDLE_NAME + " node!").c_str());
        return false;
    }
    if (appObj->SetString(BUNDLE_NAME.c_str(), bundleName) != Result::SUCCESS) {
        // LOGE("App node set %s failed!", BUNDLE_NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetBundleType(std::string& bundleType, const std::string& defaultBundleType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        // LOGE("GetAppObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType);
}

bool PackInfo::GetBundleTypeByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleType,
    const std::string& defaultBundleType)
{
    if (!appObj) {
        // LOGE("App node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (appObj->GetString(BUNDLE_TYPE.c_str(), &bundleType) != Result::SUCCESS) {
            // LOGE("App node get %s failed!", BUNDLE_TYPE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("App node get ") + BUNDLE_TYPE + " failed!").c_str());
            return false;
        }
    } else {
        bundleType = defaultBundleType;
    }
    return true;
}

bool PackInfo::GetVersionObject(std::unique_ptr<PtJson>& versionObj)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        // LOGE("GetAppObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSION.c_str())) {
        // LOGE("App node has no %s node!", VERSION.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSION + " node!").c_str());
        return false;
    }
    if (appObj->GetObject(VERSION.c_str(), &versionObj) != Result::SUCCESS) {
        // LOGE("App node get %s node failed!", VERSION.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSION + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetDistroObject(int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    return GetDistroObjectByModulesObj(modulesObj, moduleIndex, distroObj);
}

bool PackInfo::GetDistroObjectByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
    int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj)
{
    if (!modulesObj || !modulesObj->IsArray()) {
        // LOGE("Module node is null or is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null or is not array!").c_str());
        return false;
    }
    if (moduleIndex >= modulesObj->GetSize()) {
        // LOGE("Module node index error![moduleIndex=%d][modulesObjSize=%d]", moduleIndex, modulesObj->GetSize());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node index error![moduleIndex=") + std::to_string(moduleIndex) +
            "][modulesObjSize=" + std::to_string(modulesObj->GetSize()) + "]").c_str());
        return false;
    }
    std::unique_ptr<PtJson> moduleObj = modulesObj->Get(moduleIndex);
    return GetDistroObjectByModuleObj(moduleObj, distroObj);
}

bool PackInfo::GetDistroObjectByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::unique_ptr<PtJson>& distroObj)
{
    if (!moduleObj) {
        // LOGE("Module node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DISTRO.c_str())) {
        // LOGE("Module node has no %s node!", DISTRO.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + DISTRO + " node!").c_str());
        return false;
    }
    if (moduleObj->GetObject(DISTRO.c_str(), &distroObj) != Result::SUCCESS) {
        // LOGE("Module node get %s node failed!", DISTRO.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + DISTRO + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetApiVersionObjectByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::unique_ptr<PtJson>& apiVersionObj)
{
    if (!moduleObj) {
        // LOGE("Module node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(API_VERSION.c_str())) {
        cJSON *apiVersion = cJSON_CreateObject();
        apiVersionObj = std::make_unique<PtJson>(apiVersion);
        if (!moduleObj->Add(API_VERSION.c_str(), apiVersionObj)) {
            // LOGE("Module node add %s node failed!", API_VERSION.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node add ") + API_VERSION + " node failed!").c_str());
            return false;
        }
        return true;
    }
    if (moduleObj->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
        // LOGE("Module node get %s node failed!", API_VERSION.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + API_VERSION + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetExtensionAbilitiesObj(int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    return GetDistroObjectByModulesObj(modulesObj, moduleIndex, extensionAbilitiesObj);
}

bool PackInfo::GetExtensionAbilitiesObjByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
    int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    if (!modulesObj || !modulesObj->IsArray()) {
        // LOGE("Module node is null or is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null or is not array!").c_str());
        return false;
    }
    if (moduleIndex >= modulesObj->GetSize()) {
        // LOGE("Module node index error![moduleIndex=%d][modulesObjSize=%d]", moduleIndex, modulesObj->GetSize());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node index error![moduleIndex=") + std::to_string(moduleIndex) +
            "][modulesObjSize=" + std::to_string(modulesObj->GetSize()) + "]").c_str());
        return false;
    }
    std::unique_ptr<PtJson> moduleObj = modulesObj->Get(moduleIndex);
    return GetExtensionAbilitiesObjByModuleObj(moduleObj, extensionAbilitiesObj);
}

bool PackInfo::GetExtensionAbilitiesObjByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    if (!moduleObj) {
        // LOGE("Module node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(EXTENSION_ABILITIES.c_str())) {
        // LOGE("Module node has no %s node!", EXTENSION_ABILITIES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + EXTENSION_ABILITIES + " node!").c_str());
        return false;
    }
    if (moduleObj->GetArray(EXTENSION_ABILITIES.c_str(), &extensionAbilitiesObj) != Result::SUCCESS) {
        // LOGE("Module node get %s array node failed!", EXTENSION_ABILITIES.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + EXTENSION_ABILITIES + " array node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetFormsObjByExtensionAbilityObj(const std::unique_ptr<PtJson>& extensionAbilityObj,
    std::unique_ptr<PtJson>& formsObj)
{
    if (!extensionAbilityObj) {
        // LOGE("ExtensionAbility node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ExtensionAbility node is null!").c_str());
        return false;
    }
    if (!extensionAbilityObj->Contains(FORMS.c_str())) {
        // LOGE("ExtensionAbility node has no %s node!", FORMS.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("ExtensionAbility node has no ") + FORMS + " node!").c_str());
        return false;
    }
    if (extensionAbilityObj->GetArray(FORMS.c_str(), &formsObj) != Result::SUCCESS) {
        // LOGE("ExtensionAbility node get %s array node failed!", FORMS.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("ExtensionAbility node get ") + FORMS + " array node failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetPackageObject(int32_t packageIndex, std::unique_ptr<PtJson>& packageObj)
{
    std::unique_ptr<PtJson> packagesObj;
    if (!GetPackagesObject(packagesObj)) {
        // LOGE("GetPackagesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetPackagesObject failed!").c_str());
        return false;
    }
    if (!packagesObj->IsArray()) {
        // LOGE("Packages node is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Packages node is not array!").c_str());
        return false;
    }
    if (packageIndex >= packagesObj->GetSize()) {
        // LOGE("Packages node index error![packageIndex=%d][packagesObjSize=%d]", packageIndex, packagesObj->GetSize());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Packages node index error![packageIndex=") + std::to_string(packageIndex) +
            "][packagesObjSize=" + std::to_string(packagesObj->GetSize()) + "]").c_str());
        return false;
    }
    packageObj = packagesObj->Get(packageIndex);
    return true;
}

bool PackInfo::GetVersion(PackInfoVersion& version)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        // LOGE("GetVersionObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    return GetVersionByVersionObj(versionObj, version);
}

bool PackInfo::GetVersionByVersionObj(const std::unique_ptr<PtJson>& versionObj, PackInfoVersion& version)
{
    if (!versionObj) {
        // LOGE("Version node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Version node is null!").c_str());
        return false;
    }
    if (!versionObj->Contains(CODE.c_str()) || !versionObj->Contains(NAME.c_str())) {
        // LOGE("Version node has no %s node or %s node!", CODE.c_str(), NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + CODE + " node or " + NAME + " node!").c_str());
        return false;
    }
    if (versionObj->GetInt(CODE.c_str(), &version.code) != Result::SUCCESS) {
        // LOGE("Version node get %s failed!", CODE.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node get ") + CODE + " failed!").c_str());
        return false;
    }
    if (versionObj->GetString(NAME.c_str(), &version.name) != Result::SUCCESS) {
        // LOGE("Version node get %s failed!", NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node get ") + NAME + " failed!").c_str());
        return false;
    }
    if (versionObj->Contains(BUILD.c_str())) {
        if (versionObj->GetString(BUILD.c_str(), &version.buildVersion) != Result::SUCCESS) {
            // LOGE("Version node get %s failed!", BUILD.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Version node get ") + BUILD + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetVersionCode(const int32_t& versionCode)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        // LOGE("GetVersionObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(CODE.c_str())) {
        // LOGE("Version node has no %s node!", CODE.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + CODE + " node!").c_str());
        return false;
    }
    if (versionObj->SetInt(CODE.c_str(), versionCode) != Result::SUCCESS) {
        // LOGE("Version node set %s failed!", CODE.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetBuildVersion(const std::string& buildVersion)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        // LOGE("GetVersionObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(BUILD.c_str())) {
        if (!versionObj->Add(BUILD.c_str(), buildVersion.c_str())) {
            // LOGE("Version node add %s failed!", BUILD.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Version node add ") + BUILD + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (versionObj->SetString(BUILD.c_str(), buildVersion) != Result::SUCCESS) {
        // LOGE("Version node set %s failed!", BUILD.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + BUILD + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetVersionName(const std::string& versionName)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        // LOGE("GetVersionObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(NAME.c_str())) {
        // LOGE("Version node has no %s node!", NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node has no ") + NAME + " node!").c_str());
        return false;
    }
    if (versionObj->SetString(NAME.c_str(), versionName) != Result::SUCCESS) {
        // LOGE("Version node set %s failed!", NAME.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetModuleNameByDistroObj(const std::unique_ptr<PtJson>& distroObj, std::string& moduleName)
{
    if (!distroObj) {
        // LOGE("Distro node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Distro node is null!").c_str());
        return false;
    }
    if (distroObj->Contains(MODULE_NAME.c_str())) {
        if (distroObj->GetString(MODULE_NAME.c_str(), &moduleName) != Result::SUCCESS) {
            // LOGE("Distro node get %s failed!", MODULE_NAME.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Distro node get ") + MODULE_NAME + " failed!").c_str());
            return false;
        }
    } else {
        moduleName = "";
    }
    return true;
}

bool PackInfo::GetNameByPackageObj(const std::unique_ptr<PtJson>& packageObj, std::string& name)
{
    if (!packageObj) {
        // LOGE("Package node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Package node is null!").c_str());
        return false;
    }
    if (packageObj->Contains(NAME.c_str())) {
        if (packageObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
            // LOGE("Package node get %s failed!", NAME.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Package node get ") + NAME + " failed!").c_str());
            return false;
        }
    } else {
        name = "";
    }
    return true;
}

bool PackInfo::GetNameByFormObj(const std::unique_ptr<PtJson>& formObj, std::string& name)
{
    if (!formObj) {
        // LOGE("Form node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Form node is null!").c_str());
        return false;
    }
    if (formObj->Contains(NAME.c_str())) {
        if (formObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
            // LOGE("Form node get %s failed!", NAME.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Form node get ") + NAME + " failed!").c_str());
            return false;
        }
    } else {
        name = "";
    }
    return true;
}

bool PackInfo::GetDefaultDimensionByFormObj(const std::unique_ptr<PtJson>& formObj, std::string& defaultDimension)
{
    if (!formObj) {
        // LOGE("Form node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Form node is null!").c_str());
        return false;
    }
    if (!formObj->Contains(DEFAULT_DIMENSION.c_str())) {
        // LOGE("Form node has no %s node!", DEFAULT_DIMENSION.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Form node has no ") + DEFAULT_DIMENSION + " node!").c_str());
        return false;
    }
    if (formObj->GetString(DEFAULT_DIMENSION.c_str(), &defaultDimension) != Result::SUCCESS) {
        // LOGE("Form node get %s failed!", DEFAULT_DIMENSION.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Form node get ") + DEFAULT_DIMENSION + " failed!").c_str());
        return false;
    }
    if (std::count(defaultDimension.begin(), defaultDimension.end(), ASTERISK) != 1) {
        // LOGE("there must be only one '%c'", ASTERISK);
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("there must be only one '") + ASTERISK + "'").c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetSupportDimensionsByFormObj(const std::unique_ptr<PtJson>& formObj,
    std::list<std::string>& supportDimensions)
{
    if (!formObj) {
        // LOGE("Form node is null!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Form node is null!").c_str());
        return false;
    }
    if (!formObj->Contains(SUPPORT_DIMENSIONS.c_str())) {
        // LOGE("Form node has no %s node!", SUPPORT_DIMENSIONS.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Form node has no ") + SUPPORT_DIMENSIONS + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> supportDimensionsObj;
    if (formObj->GetArray(SUPPORT_DIMENSIONS.c_str(), &supportDimensionsObj) != Result::SUCCESS) {
        // LOGE("Form node get %s array node failed!", SUPPORT_DIMENSIONS.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Form node get ") + SUPPORT_DIMENSIONS + " array node failed!").c_str());
        return false;
    }
    for (int32_t i = 0; i < supportDimensionsObj->GetSize(); i++) {
        supportDimensions.push_back(supportDimensionsObj->Get(i)->GetString());
    }
    return true;
}

// java : parsePackInfoFormsName
bool PackInfo::GetFormNames(std::list<std::string>& formNames, std::list<std::string>& formFullNames)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    if (!modulesObj->IsArray()) {
        // LOGE("Module node is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is not array!").c_str());
        return false;
    }
    for (int32_t i = 0; i < modulesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> distroObj;
        if (!GetDistroObjectByModuleObj(modulesObj->Get(i), distroObj)) {
            // LOGE("GetDistroObjectByModuleObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetDistroObjectByModuleObj failed!").c_str());
            return false;
        }
        std::string moduleName;
        if (!GetModuleNameByDistroObj(distroObj, moduleName) || moduleName.empty()) {
            // LOGE("GetModuleNameByDistroObj failed or moduleName is empty!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleNameByDistroObj failed or moduleName is empty!").c_str());
            continue;
        }
        std::unique_ptr<PtJson> extensionAbilitiesObj;
        if (!GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(i), extensionAbilitiesObj)) {
            // LOGE("GetExtensionAbilitiesObjByModuleObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetExtensionAbilitiesObjByModuleObj failed!").c_str());
            return false;
        }
        if (!GetFormNamesByExtensionAbilitiesObj(extensionAbilitiesObj, moduleName, formNames, formFullNames)) {
            // LOGE("GetFormNamesByExtensionAbilitiesObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetFormNamesByExtensionAbilitiesObj failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::GetFormNamesByExtensionAbilitiesObj(const std::unique_ptr<PtJson>& extensionAbilitiesObj,
    std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames)
{
    if (!extensionAbilitiesObj || !extensionAbilitiesObj->IsArray()) {
        // LOGE("ExtensionAbilities node is null or is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ExtensionAbilities node is null or is not array!").c_str());
        return false;
    }
    for (int32_t j = 0; j < extensionAbilitiesObj->GetSize(); j++) {
        std::unique_ptr<PtJson> formsObj;
        if (!GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(j), formsObj)) {
            // LOGE("GetFormsObjByExtensionAbilityObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetFormsObjByExtensionAbilityObj failed!").c_str());
            return false;
        }
        if (!GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames)) {
            // LOGE("GetFormNamesByFormsObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetFormNamesByFormsObj failed!").c_str());
            return false;
        }
    }
    return true;
}


bool PackInfo::GetFormNamesByFormsObj(const std::unique_ptr<PtJson>& formsObj,
    std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames)
{
    if (!formsObj || !formsObj->IsArray()) {
        // LOGE("Form node is null or is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Form node is null or is not array!").c_str());
        return false;
    }
    for (int32_t k = 0; k < formsObj->GetSize(); k++) {
        std::string formName;
        std::string defaultDimension;
        std::list<std::string> supportDimensions;
        std::string formFullName;
        if (!GetNameByFormObj(formsObj->Get(k), formName) || formName.empty()) {
            continue;
        }
        formNames.push_back(formName);
        if (!GetDefaultDimensionByFormObj(formsObj->Get(k), defaultDimension)) {
            // LOGE("GetDefaultDimensionByFormObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetDefaultDimensionByFormObj failed!").c_str());
            return false;
        }
        if (!GetSupportDimensionsByFormObj(formsObj->Get(k), supportDimensions)) {
            // LOGE("GetSupportDimensionsByFormObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetSupportDimensionsByFormObj failed!").c_str());
            return false;
        }
        for (std::string supportDimension : supportDimensions) {
            formFullName = moduleName + "/" + formName + "-" + Utils::ReplaceAll(supportDimension, "*", "x");
            formFullNames.push_back(formFullName);
        }
    }
    return true;
}

bool PackInfo::GetPackageNames(std::list<std::string> &packageNames)
{
    std::unique_ptr<PtJson> packagesObj;
    if (!GetPackagesObject(packagesObj)) {
        // LOGE("GetPackagesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetPackagesObject failed!").c_str());
        return false;
    }
    return GetPackageNamesByPackagesObj(packagesObj, packageNames);
}

bool PackInfo::GetPackageNamesByPackagesObj(const std::unique_ptr<PtJson>& packagesObj,
    std::list<std::string> &packageNames)
{
    if (!packagesObj || !packagesObj->IsArray()) {
        // LOGE("Packages node is null or is not array!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Packages node is null or is not array!").c_str());
        return false;
    }
    for (int i = 0; i < packagesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> packageObj;
        if (!GetPackageObject(i, packageObj)) {
            // LOGE("GetPackageObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetPackageObject failed!").c_str());
            return false;
        }
        std::string name;
        if (!GetNameByPackageObj(packageObj, name)) {
            // LOGE("GetNameByPackageObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetNameByPackageObj failed!").c_str());
            return false;
        }
        packageNames.push_back(name);
    }
    return true;
}

bool PackInfo::SetMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        // LOGE("GetVersionObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersionObject failed!").c_str());
        return false;
    }
    if (!versionObj->Contains(MIN_COMPATIBLE_VERSION_CODE.c_str())) {
        if (!versionObj->Add(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode)) {
            // LOGE("App node add %s failed!", MIN_COMPATIBLE_VERSION_CODE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + MIN_COMPATIBLE_VERSION_CODE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (versionObj->SetInt(MIN_COMPATIBLE_VERSION_CODE.c_str(), minCompatibleVersionCode) != Result::SUCCESS) {
        // LOGE("Version node set %s failed!", CODE.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Version node set ") + CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetMinAPIVersion(const int32_t& minAPIVersion)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> apiVersionObj;
        if (!GetApiVersionObjectByModuleObj(moduleObj, apiVersionObj)) {
            // LOGE("GetApiVersionObjectByModuleObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetApiVersionObjectByModuleObj failed!").c_str());
            return false;
        }
        if (!apiVersionObj->Contains(COMPATIBLE.c_str())) {
            if (!apiVersionObj->Add(COMPATIBLE.c_str(), minAPIVersion)) {
                // LOGE("App node add %s failed!", COMPATIBLE.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + COMPATIBLE + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (apiVersionObj->SetInt(COMPATIBLE.c_str(), minAPIVersion) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", COMPATIBLE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + COMPATIBLE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetTargetAPIVersion(const int32_t& targetAPIVersion)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> apiVersionObj;
        if (!GetApiVersionObjectByModuleObj(moduleObj, apiVersionObj)) {
            // LOGE("GetApiVersionObjectByModuleObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetApiVersionObjectByModuleObj failed!").c_str());
            return false;
        }
        if (!apiVersionObj->Contains(TARGET.c_str())) {
            if (!apiVersionObj->Add(TARGET.c_str(), targetAPIVersion)) {
                // LOGE("App node add %s failed!", TARGET.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + TARGET + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (apiVersionObj->SetInt(TARGET.c_str(), targetAPIVersion) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", TARGET.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + TARGET + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetApiReleaseType(const std::string& apiReleaseType)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> apiVersionObj;
        if (!GetApiVersionObjectByModuleObj(moduleObj, apiVersionObj)) {
            // LOGE("GetApiVersionObjectByModuleObj failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetApiVersionObjectByModuleObj failed!").c_str());
            return false;
        }
        if (!apiVersionObj->Contains(RELEASE_TYPE.c_str())) {
            if (!apiVersionObj->Add(RELEASE_TYPE.c_str(), apiReleaseType.c_str())) {
                // LOGE("App node add %s failed!", RELEASE_TYPE.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + RELEASE_TYPE + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (apiVersionObj->SetString(RELEASE_TYPE.c_str(), apiReleaseType) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", RELEASE_TYPE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + RELEASE_TYPE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetBundleType(const std::string& bundleType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        // LOGE("GetAppObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (!appObj->Add(BUNDLE_TYPE.c_str(), bundleType.c_str())) {
            // LOGE("App node add %s failed!", BUNDLE_TYPE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("App node add ") + BUNDLE_TYPE + " failed!").c_str());
            return false;
        }
        return true;
    }
    if (appObj->SetString(BUNDLE_TYPE.c_str(), bundleType) != Result::SUCCESS) {
        // LOGE("App node set %s failed!", BUNDLE_TYPE.c_str());
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node set ") + BUNDLE_TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetInstallationFree(const bool& installationFree)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroObj;
        if (!GetDistroObject(i, distroObj)) {
            // LOGE("GetDistroObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
            return false;
        }
        if (!distroObj->Contains(INSTALLATION_FREE.c_str())) {
            if (!distroObj->Add(INSTALLATION_FREE.c_str(), installationFree)) {
                // LOGE("App node add %s failed!", INSTALLATION_FREE.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + INSTALLATION_FREE + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (distroObj->SetBool(INSTALLATION_FREE.c_str(), installationFree) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", INSTALLATION_FREE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + INSTALLATION_FREE + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetDeliveryWithInstall(const bool& deliveryWithInstall)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModulesObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModulesObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        std::unique_ptr<PtJson> distroObj;
        if (!GetDistroObject(i, distroObj)) {
            // LOGE("GetDistroObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
            return false;
        }
        if (!distroObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
            if (!distroObj->Add(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall)) {
                // LOGE("App node add %s failed!", DELIVERY_WITH_INSTALL.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (distroObj->SetBool(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", DELIVERY_WITH_INSTALL.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
            return false;
        }
    }
    std::unique_ptr<PtJson> packagesObj;
    if (!GetPackagesObject(packagesObj)) {
        // LOGE("GetModuleObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    int32_t packagesSize = packagesObj->GetSize();
    for (int i = 0; i < packagesSize; i++) {
        std::unique_ptr<PtJson> packageObj = packagesObj->Get(i);
        if (packageObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        if (!packageObj->Contains(DELIVERY_WITH_INSTALL.c_str())) {
            if (!packageObj->Add(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall)) {
                // LOGE("App node add %s failed!", DELIVERY_WITH_INSTALL.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (packageObj->SetBool(DELIVERY_WITH_INSTALL.c_str(), deliveryWithInstall) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", DELIVERY_WITH_INSTALL.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + DELIVERY_WITH_INSTALL + " failed!").c_str());
            return false;
        }
    }
    return true;
}

bool PackInfo::SetDeviceTypes(const std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        // LOGE("GetModuleObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    int32_t modulesSize = modulesObj->GetSize();
    for (int i = 0; i < modulesSize; i++) {
        std::unique_ptr<PtJson> moduleObj = modulesObj->Get(i);
        if (moduleObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        if (!moduleObj->Contains(DEVICE_TYPE.c_str())) {
            if (!moduleObj->Add(DEVICE_TYPE.c_str(), deviceTypes)) {
                // LOGE("App node add %s failed!", DEVICE_TYPE.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + DEVICE_TYPE + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (moduleObj->SetArray(DEVICE_TYPE.c_str(), deviceTypes) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", DEVICE_TYPE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + DEVICE_TYPE + " failed!").c_str());
            return false;
        }
    }
    std::unique_ptr<PtJson> packagesObj;
    if (!GetPackagesObject(packagesObj)) {
        // LOGE("GetModuleObject failed!");
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    int32_t packagesSize = packagesObj->GetSize();
    for (int i = 0; i < packagesSize; i++) {
        std::unique_ptr<PtJson> packageObj = packagesObj->Get(i);
        if (packageObj == nullptr) {
            // LOGE("GetModuleObject failed!");
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
            return false;
        }
        if (!packageObj->Contains(DEVICE_TYPE.c_str())) {
            if (!packageObj->Add(DEVICE_TYPE.c_str(), deviceTypes)) {
                // LOGE("App node add %s failed!", DEVICE_TYPE.c_str());
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    std::string("App node add ") + DEVICE_TYPE + " failed!").c_str());
                return false;
            }
            return true;
        }
        if (packageObj->SetArray(DEVICE_TYPE.c_str(), deviceTypes) != Result::SUCCESS) {
            // LOGE("Module node set %s failed!", DEVICE_TYPE.c_str());
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                std::string("Module node set ") + DEVICE_TYPE + " failed!").c_str());
            return false;
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
