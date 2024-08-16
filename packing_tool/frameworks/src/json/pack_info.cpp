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

#include "pack_info.h"

#include <algorithm>
#include <fstream>

#include "log.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
    const std::string SUMMARY = "summary";
    const std::string PACKAGES = "packages";
    const std::string APP = "app";
    const std::string MODULES = "modules";
    // const std::string MODULE = "module";
    const std::string BUNDLE_NAME = "bundleName";
    const std::string BUNDLE_TYPE = "bundleType";
    const std::string VERSION = "version";
    const std::string CODE = "code";
    const std::string NAME = "name";
    const std::string DISTRO = "distro";
    const std::string MODULE_NAME = "moduleName";
    const std::string EXTENSION_ABILITIES = "extensionAbilities";
    const std::string FORMS = "forms";
    const std::string DEFAULT_DIMENSION = "defaultDimension";
    const std::string SUPPORT_DIMENSIONS = "supportDimensions";
    const char ASTERISK = '*';
}

bool PackInfo::ParseFromString(const std::string& jsonString)
{
    Release();
    if (jsonString.length() == 0) {
        LOGE("Json length is zero!");
        return false;
    }
    root_ = PtJson::Parse(jsonString);
    return IsValid();
}

bool PackInfo::ParseFromFile(const std::string& jsonFile)
{
    Release();
    std::ifstream inFile(jsonFile, std::ios::in);
    if (!inFile.is_open()) {
        LOGE("Open json file failed! jsonFile=%s", jsonFile.c_str());
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
        LOGE("Json root is null!");
        return false;
    }
    if (!root_->Contains(SUMMARY.c_str())) {
        LOGE("Json root has no %s node!", SUMMARY.c_str());
        return false;
    }
    if (root_->GetObject(SUMMARY.c_str(), &summaryObj) != Result::SUCCESS) {
        LOGE("Json root get %s node failed!", SUMMARY.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetPackagesObject(std::unique_ptr<PtJson>& packagesObj)
{
    if (root_.get() == nullptr) {
        LOGE("Json root is null!");
        return false;
    }
    if (!root_->Contains(PACKAGES.c_str())) {
        LOGE("Json root has no %s node!", PACKAGES.c_str());
        return false;
    }
    if (root_->GetArray(PACKAGES.c_str(), &packagesObj) != Result::SUCCESS) {
        LOGE("Json root get %s array node failed!", PACKAGES.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetAppObject(std::unique_ptr<PtJson>& appObj)
{
    std::unique_ptr<PtJson> summaryObj;
    if (!GetSummaryObject(summaryObj)) {
        LOGE("GetSummaryObject failed!");
        return false;
    }
    if (!summaryObj->Contains(APP.c_str())) {
        LOGE("Summary node has no %s node!", APP.c_str());
        return false;
    }
    if (summaryObj->GetObject(APP.c_str(), &appObj) != Result::SUCCESS) {
        LOGE("Summary node get %s node failed!", APP.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetModulesObject(std::unique_ptr<PtJson>& modulesObj)
{
    std::unique_ptr<PtJson> summaryObj;
    if (!GetSummaryObject(summaryObj)) {
        LOGE("GetSummaryObject failed!");
        return false;
    }
    if (!summaryObj->Contains(MODULES.c_str())) {
        LOGE("Summary node has no %s node!", MODULES.c_str());
        return false;
    }
    if (summaryObj->GetArray(MODULES.c_str(), &modulesObj) != Result::SUCCESS) {
        LOGE("Summary node get %s node failed!", MODULES.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetBundleName(std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetBundleNameByAppObj(appObj, bundleName);
}

bool PackInfo::GetBundleNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleName)
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

bool PackInfo::SetBundleName(const std::string& bundleName)
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

bool PackInfo::GetBundleType(std::string& bundleType, const std::string& defaultBundleType)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("GetAppObject failed!");
        return false;
    }
    return GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType);
}

bool PackInfo::GetBundleTypeByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleType,
    const std::string& defaultBundleType)
{
    if (!appObj) {
        LOGE("App node is null!");
        return false;
    }
    if (appObj->Contains(BUNDLE_TYPE.c_str())) {
        if (appObj->GetString(BUNDLE_TYPE.c_str(), &bundleType) != Result::SUCCESS) {
            LOGE("App node get %s failed!", BUNDLE_TYPE.c_str());
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

bool PackInfo::GetDistroObject(int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        LOGE("GetModulesObject failed!");
        return false;
    }
    return GetDistroObjectByModulesObj(modulesObj, moduleIndex, distroObj);
}

bool PackInfo::GetDistroObjectByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
    int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj)
{
    if (!modulesObj || !modulesObj->IsArray()) {
        LOGE("Module node is null or is not array!");
        return false;
    }
    if (moduleIndex >= modulesObj->GetSize()) {
        LOGE("Module node index error![moduleIndex=%d][modulesObjSize=%d]", moduleIndex, modulesObj->GetSize());
        return false;
    }
    std::unique_ptr<PtJson> moduleObj = modulesObj->Get(moduleIndex);
    return GetDistroObjectByModuleObj(moduleObj, distroObj);
}

bool PackInfo::GetDistroObjectByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::unique_ptr<PtJson>& distroObj)
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

bool PackInfo::GetExtensionAbilitiesObj(int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    std::unique_ptr<PtJson> modulesObj;
    if (!GetModulesObject(modulesObj)) {
        LOGE("GetModulesObject failed!");
        return false;
    }
    return GetDistroObjectByModulesObj(modulesObj, moduleIndex, extensionAbilitiesObj);
}

bool PackInfo::GetExtensionAbilitiesObjByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
    int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    if (!modulesObj || !modulesObj->IsArray()) {
        LOGE("Module node is null or is not array!");
        return false;
    }
    if (moduleIndex >= modulesObj->GetSize()) {
        LOGE("Module node index error![moduleIndex=%d][modulesObjSize=%d]", moduleIndex, modulesObj->GetSize());
        return false;
    }
    std::unique_ptr<PtJson> moduleObj = modulesObj->Get(moduleIndex);
    return GetExtensionAbilitiesObjByModuleObj(moduleObj, extensionAbilitiesObj);
}

bool PackInfo::GetExtensionAbilitiesObjByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::unique_ptr<PtJson>& extensionAbilitiesObj)
{
    if (!moduleObj) {
        LOGE("Module node is null!");
        return false;
    }
    if (!moduleObj->Contains(EXTENSION_ABILITIES.c_str())) {
        LOGE("Module node has no %s node!", EXTENSION_ABILITIES.c_str());
        return false;
    }
    if (moduleObj->GetArray(EXTENSION_ABILITIES.c_str(), &extensionAbilitiesObj) != Result::SUCCESS) {
        LOGE("Module node get %s array node failed!", EXTENSION_ABILITIES.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetFormsObjByExtensionAbilityObj(const std::unique_ptr<PtJson>& extensionAbilityObj,
    std::unique_ptr<PtJson>& formsObj)
{
    if (!extensionAbilityObj) {
        LOGE("ExtensionAbility node is null!");
        return false;
    }
    if (!extensionAbilityObj->Contains(FORMS.c_str())) {
        LOGE("ExtensionAbility node has no %s node!", FORMS.c_str());
        return false;
    }
    if (extensionAbilityObj->GetArray(FORMS.c_str(), &formsObj) != Result::SUCCESS) {
        LOGE("ExtensionAbility node get %s array node failed!", FORMS.c_str());
        return false;
    }
    return true;
}

bool PackInfo::GetPackageObject(int32_t packageIndex, std::unique_ptr<PtJson>& packageObj)
{
    std::unique_ptr<PtJson> packagesObj;
    if (!GetPackagesObject(packagesObj)) {
        LOGE("GetPackagesObject failed!");
        return false;
    }
    if (!packagesObj->IsArray()) {
        LOGE("Packages node is not array!");
        return false;
    }
    if (packageIndex >= packagesObj->GetSize()) {
        LOGE("Packages node index error![packageIndex=%d][packagesObjSize=%d]", packageIndex, packagesObj->GetSize());
        return false;
    }
    packageObj = packagesObj->Get(packageIndex);
    return true;
}

bool PackInfo::GetVersion(PackInfoVersion& version)
{
    std::unique_ptr<PtJson> versionObj;
    if (!GetVersionObject(versionObj)) {
        LOGE("GetVersionObject failed!");
        return false;
    }
    return GetVersionByVersionObj(versionObj, version);
}

bool PackInfo::GetVersionByVersionObj(const std::unique_ptr<PtJson>& versionObj, PackInfoVersion& version)
{
    if (!versionObj) {
        LOGE("Version node is null!");
        return false;
    }
    if (!versionObj->Contains(CODE.c_str()) || !versionObj->Contains(NAME.c_str())) {
        LOGE("Version node has no %s node or %s node!", CODE.c_str(), NAME.c_str());
        return false;
    }
    if (versionObj->GetInt(CODE.c_str(), &version.code) != Result::SUCCESS) {
        LOGE("Version node get %s failed!", CODE.c_str());
        return false;
    }
    if (versionObj->GetString(NAME.c_str(), &version.name) != Result::SUCCESS) {
        LOGE("Version node get %s failed!", NAME.c_str());
        return false;
    }
    return true;
}

bool PackInfo::SetVersionCode(const int32_t& versionCode)
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

bool PackInfo::SetVersionName(const std::string& versionName)
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

bool PackInfo::GetModuleNameByDistroObj(const std::unique_ptr<PtJson>& distroObj, std::string& moduleName)
{
    if (!distroObj) {
        LOGE("Distro node is null!");
        return false;
    }
    if (distroObj->Contains(MODULE_NAME.c_str())) {
        if (distroObj->GetString(MODULE_NAME.c_str(), &moduleName) != Result::SUCCESS) {
            LOGE("Distro node get %s failed!", MODULE_NAME.c_str());
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
        LOGE("Package node is null!");
        return false;
    }
    if (packageObj->Contains(NAME.c_str())) {
        if (packageObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
            LOGE("Package node get %s failed!", NAME.c_str());
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
        LOGE("Form node is null!");
        return false;
    }
    if (formObj->Contains(NAME.c_str())) {
        if (formObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
            LOGE("Form node get %s failed!", NAME.c_str());
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
        LOGE("Form node is null!");
        return false;
    }
    if (!formObj->Contains(DEFAULT_DIMENSION.c_str())) {
        LOGE("Form node has no %s node!", DEFAULT_DIMENSION.c_str());
        return false;
    }
    if (formObj->GetString(DEFAULT_DIMENSION.c_str(), &defaultDimension) != Result::SUCCESS) {
        LOGE("Form node get %s failed!", DEFAULT_DIMENSION.c_str());
        return false;
    }
    if (std::count(defaultDimension.begin(), defaultDimension.end(), ASTERISK) != 1) {
        LOGE("there must be only one '%c'", ASTERISK);
        return false;
    }
    return true;
}

bool PackInfo::GetSupportDimensionsByFormObj(const std::unique_ptr<PtJson>& formObj,
    std::list<std::string>& supportDimensions)
{
    if (!formObj) {
        LOGE("Form node is null!");
        return false;
    }
    if (!formObj->Contains(SUPPORT_DIMENSIONS.c_str())) {
        LOGE("Form node has no %s node!", SUPPORT_DIMENSIONS.c_str());
        return false;
    }
    std::unique_ptr<PtJson> supportDimensionsObj;
    if (formObj->GetArray(SUPPORT_DIMENSIONS.c_str(), &supportDimensionsObj) != Result::SUCCESS) {
        LOGE("Form node get %s array node failed!", SUPPORT_DIMENSIONS.c_str());
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
        LOGE("GetModulesObject failed!");
        return false;
    }
    if (!modulesObj->IsArray()) {
        LOGE("Module node is not array!");
        return false;
    }
    for (int32_t i = 0; i < modulesObj->GetSize(); i++) {
        std::unique_ptr<PtJson> distroObj;
        if (!GetDistroObjectByModuleObj(modulesObj->Get(i), distroObj)) {
            LOGE("GetDistroObjectByModuleObj failed!");
            return false;
        }
        std::string moduleName;
        if (!GetModuleNameByDistroObj(distroObj, moduleName) || moduleName.empty()) {
            LOGE("GetModuleNameByDistroObj failed or moduleName is empty!");
            continue;
        }
        std::unique_ptr<PtJson> extensionAbilitiesObj;
        if (!GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(i), extensionAbilitiesObj)) {
            LOGE("GetExtensionAbilitiesObjByModuleObj failed!");
            return false;
        }
        if (!GetFormNamesByExtensionAbilitiesObj(extensionAbilitiesObj, moduleName, formNames, formFullNames)) {
            LOGE("GetFormNamesByExtensionAbilitiesObj failed!");
            return false;
        }
    }
    return true;
}

bool PackInfo::GetFormNamesByExtensionAbilitiesObj(const std::unique_ptr<PtJson>& extensionAbilitiesObj,
    std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames)
{
    if (!extensionAbilitiesObj || !extensionAbilitiesObj->IsArray()) {
        LOGE("ExtensionAbilities node is null or is not array!");
        return false;
    }
    for (int32_t j = 0; j < extensionAbilitiesObj->GetSize(); j++) {
        std::unique_ptr<PtJson> formsObj;
        if (!GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(j), formsObj)) {
            LOGE("GetFormsObjByExtensionAbilityObj failed!");
            return false;
        }
        if (!GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames)) {
            LOGE("GetFormNamesByFormsObj failed!");
            return false;
        }
    }
    return true;
}


bool PackInfo::GetFormNamesByFormsObj(const std::unique_ptr<PtJson>& formsObj,
    std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames)
{
    if (!formsObj || !formsObj->IsArray()) {
        LOGE("Form node is null or is not array!");
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
            LOGE("GetDefaultDimensionByFormObj failed!");
            return false;
        }
        if (!GetSupportDimensionsByFormObj(formsObj->Get(k), supportDimensions)) {
            LOGE("GetSupportDimensionsByFormObj failed!");
            return false;
        }
        for (std::string supportDimension : supportDimensions) {
            formFullName = moduleName + "/" + formName + "-" + Utils::ReplaceAll(supportDimension, "*", "x");
            formFullNames.push_back(formFullName);
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
