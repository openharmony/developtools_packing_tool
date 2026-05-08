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

#include "pack_info_utils.h"

#include <fstream>

#include "error/packing_tool_err_msg.h"
#include "log.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string DEFAULT_BUNDLE_TYPE = "APP";
const std::string MODULE = "module";
const std::string PACKAGES = "packages";
const char DOT = '.';
}

// java : mergeTwoPackInfo
bool PackInfoUtils::MergeTwoPackInfos(const std::string& srcPackInfoJsonStr1, const std::string& srcPackInfoJsonStr2,
    std::string& dstPackInfoJsonStr)
{
    PackInfo srcPackInfo1;
    PackInfo srcPackInfo2;
    if (!srcPackInfo1.ParseFromString(srcPackInfoJsonStr1)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Parse from string1 failed![") + srcPackInfoJsonStr1 + "]").c_str());
        return false;
    }
    if (!srcPackInfo2.ParseFromString(srcPackInfoJsonStr2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Parse from string2 failed![") + srcPackInfoJsonStr2 + "]").c_str());
        return false;
    }
    if (!VerifyPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("VerifyPackInfos failed!").c_str());
        return false;
    }
    if (!MergeTwoPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("MergeTwoPackInfos failed!").c_str());
        return false;
    }
    dstPackInfoJsonStr = srcPackInfo1.ToString();
    return true;
}

// java : mergePackInfoObj
bool PackInfoUtils::MergeTwoPackInfos(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2)
{
    std::unique_ptr<PtJson> modulesObj1;
    std::unique_ptr<PtJson> modulesObj2;
    if (!srcPackInfo1.GetModulesObject(modulesObj1) || !srcPackInfo2.GetModulesObject(modulesObj2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Get module node from srcPackInfo1 or srcPackInfo2 failed!").c_str());
        return false;
    }
    if (!modulesObj1->IsArray() || !modulesObj2->IsArray()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is not array!").c_str());
        return false;
    }
    for (int32_t i = 0; i < modulesObj2->GetSize(); i++) {
        if (!modulesObj1->Push(modulesObj2->Get(i))) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Push module node failed!").c_str());
            return false;
        }
    }

    std::unique_ptr<PtJson> packagesObj1;
    std::unique_ptr<PtJson> packagesObj2;
    if (!srcPackInfo1.GetPackagesObject(packagesObj1) || !srcPackInfo2.GetPackagesObject(packagesObj2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetPackagesObject failed!").c_str());
        return false;
    }
    if (!packagesObj1->IsArray() || !packagesObj2->IsArray()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Package node is not array!").c_str());
        return false;
    }
    for (int32_t i = 0; i < packagesObj2->GetSize(); i++) {
        if (!packagesObj1->Push(packagesObj2->Get(i))) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Push package node failed!").c_str());
            return false;
        }
    }
    return true;
}

// java : mergeTwoPackInfoByPackagePair
bool PackInfoUtils::MergeTwoPackInfosByPackagePair(const std::string& srcPackInfoJsonStr1,
    const std::string& srcPackInfoJsonStr2, const std::map<std::string, std::string>& packagesMap,
    std::string& dstPackInfoJsonStr)
{
    PackInfo srcPackInfo1;
    PackInfo srcPackInfo2;
    if (!srcPackInfo1.ParseFromString(srcPackInfoJsonStr1)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Parse from string1 failed![") + srcPackInfoJsonStr1 + "]").c_str());
        return false;
    }
    if (!srcPackInfo2.ParseFromString(srcPackInfoJsonStr2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Parse from string2 failed![") + srcPackInfoJsonStr2 + "]").c_str());
        return false;
    }
    if (!VerifyPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("VerifyPackInfos failed!").c_str());
        return false;
    }
    auto iter = packagesMap.begin();
    while (iter != packagesMap.end()) {
        std::string packageName = iter->first;
        std::string moduleName = iter->second;
        std::string tmpStr = packageName.substr(0, packageName.find_last_of(DOT));
        if (!MergeTwoPackInfosByPackagePair(srcPackInfo1, srcPackInfo2, tmpStr, moduleName)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("MergeTwoPackInfosByPackagePair failed!").c_str());
            return false;
        }
        ++iter;
    }
    dstPackInfoJsonStr = srcPackInfo1.ToString();
    return true;
}

bool PackInfoUtils::FindAndMergeModulesByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
    const std::string& moduleName)
{
    std::unique_ptr<PtJson> modulesObj1;
    std::unique_ptr<PtJson> modulesObj2;
    if (!srcPackInfo1.GetModulesObject(modulesObj1) || !srcPackInfo2.GetModulesObject(modulesObj2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Get module node from srcPackInfo1 or srcPackInfo2 failed!").c_str());
        return false;
    }
    if (!modulesObj1->IsArray() || !modulesObj2->IsArray()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is not array!").c_str());
        return false;
    }
    bool isFind = false;
    for (int32_t i = 0; i < modulesObj2->GetSize(); i++) {
        std::unique_ptr<PtJson> distroObj;
        if (!srcPackInfo2.GetDistroObject(i, distroObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetDistroObject failed!").c_str());
            return false;
        }
        std::string moduleNameInDistroObj;
        if (!srcPackInfo2.GetModuleNameByDistroObj(distroObj, moduleNameInDistroObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleNameByDistroObj failed!").c_str());
            return false;
        }
        if (moduleNameInDistroObj.compare(moduleName) == 0) {
            if (!modulesObj1->Push(modulesObj2->Get(i))) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Push module node failed!").c_str());
                return false;
            }
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Cannot find same moduleName in module node!").c_str());
        return false;
    }
    return true;
}

bool PackInfoUtils::FindAndMergePackagesByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
    const std::string& packageName)
{
    std::unique_ptr<PtJson> packagesObj1;
    std::unique_ptr<PtJson> packagesObj2;
    if (!srcPackInfo1.GetPackagesObject(packagesObj1) || !srcPackInfo2.GetPackagesObject(packagesObj2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Get package node from srcPackInfo1 or srcPackInfo2 failed!").c_str());
        return false;
    }
    if (!packagesObj1->IsArray() || !packagesObj2->IsArray()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Package node is not array!").c_str());
        return false;
    }
    bool isFind = false;
    for (int32_t i = 0; i < packagesObj2->GetSize(); i++) {
        std::string packageNameInPackageObj;
        if (!srcPackInfo2.GetNameByPackageObj(packagesObj2->Get(i), packageNameInPackageObj)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetNameByPackageObj failed!").c_str());
            return false;
        }
        if (packageNameInPackageObj.compare(packageName) == 0) {
            if (!packagesObj1->Push(packagesObj2->Get(i))) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Push Pacakge node failed!").c_str());
                return false;
            }
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Cannot find same packageName in package node!").c_str());
        return false;
    }
    return true;
}

// java : mergeTwoPackInfoObjByPackagePair
bool PackInfoUtils::MergeTwoPackInfosByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
    const std::string& packageName, const std::string& moduleName)
{
    if (!FindAndMergeModulesByPackagePair(srcPackInfo1, srcPackInfo2, moduleName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("FindAndMergeModulesByPackagePair failed!").c_str());
        return false;
    }
    if (!FindAndMergePackagesByPackagePair(srcPackInfo1, srcPackInfo2, packageName)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("FindAndMergePackagesByPackagePair failed!").c_str());
        return false;
    }
    return true;
}

// java : verifyPackInfo
bool PackInfoUtils::VerifyPackInfos(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    std::unique_ptr<PtJson> appObj1;
    std::unique_ptr<PtJson> appObj2;
    if (!const_cast<PackInfo&>(packInfo1).GetAppObject(appObj1) ||
        !const_cast<PackInfo&>(packInfo2).GetAppObject(appObj2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    if (!CheckBundleNameInPackInfo(packInfo1, packInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("CheckBundleNameInPackInfo failed!").c_str());
        return false;
    }
    if (!CheckBundleTypeInPackInfo(packInfo1, packInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("CheckBundleTypeInPackInfo failed!").c_str());
        return false;
    }
    if (!CheckVersionCodeInPackInfo(packInfo1, packInfo2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("CheckVersionCodeInPackInfo failed!").c_str());
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckBundleNameInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    std::string bundleName1;
    std::string bundleName2;
    if (!const_cast<PackInfo&>(packInfo1).GetBundleName(bundleName1)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetBundleName from packInfo1 failed!").c_str());
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetBundleName(bundleName2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetBundleName from packInfo2 failed!").c_str());
        return false;
    }
    if (bundleName1.compare(bundleName2) != 0) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("bundleName is not same").c_str());
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckBundleTypeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    std::string bundleType1;
    std::string bundleType2;
    if (!const_cast<PackInfo&>(packInfo1).GetBundleType(bundleType1, DEFAULT_BUNDLE_TYPE)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetBundleType from packInfo1 failed!").c_str());
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetBundleType(bundleType2, DEFAULT_BUNDLE_TYPE)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetBundleType from packInfo2 failed!").c_str());
        return false;
    }
    if (bundleType1.compare(bundleType2) != 0) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("bundleType is not same").c_str());
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckVersionCodeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    PackInfoVersion version1;
    PackInfoVersion version2;
    if (!const_cast<PackInfo&>(packInfo1).GetVersion(version1)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersion from packInfo1 failed!").c_str());
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetVersion(version2)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetVersion from packInfo2 failed!").c_str());
        return false;
    }
    if (version1.code != version2.code) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Version is not same").c_str());
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
