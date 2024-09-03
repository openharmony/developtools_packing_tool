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

#include "log.h"

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
        LOGE("Parse from string1 failed![%s]", srcPackInfoJsonStr1.c_str());
        return false;
    }
    if (!srcPackInfo2.ParseFromString(srcPackInfoJsonStr2)) {
        LOGE("Parse from string2 failed![%s]", srcPackInfoJsonStr2.c_str());
        return false;
    }
    if (!VerifyPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("VerifyPackInfos failed!");
        return false;
    }
    if (!MergeTwoPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("MergeTwoPackInfos failed!");
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
        LOGE("Get module node from srcPackInfo1 or srcPackInfo2 failed!");
        return false;
    }
    if (!modulesObj1->IsArray() || !modulesObj2->IsArray()) {
        LOGE("Module node is not array!");
        return false;
    }
    for (int32_t i = 0; i < modulesObj2->GetSize(); i++) {
        if (!modulesObj1->Push(modulesObj2->Get(i))) {
            LOGE("Push module node failed!");
            return false;
        }
    }

    std::unique_ptr<PtJson> packagesObj1;
    std::unique_ptr<PtJson> packagesObj2;
    if (!srcPackInfo1.GetPackagesObject(packagesObj1) || !srcPackInfo2.GetPackagesObject(packagesObj2)) {
        LOGE("GetPackagesObject failed!");
        return false;
    }
    if (!packagesObj1->IsArray() || !packagesObj2->IsArray()) {
        LOGE("Package node is not array!");
        return false;
    }
    for (int32_t i = 0; i < packagesObj2->GetSize(); i++) {
        if (!packagesObj1->Push(packagesObj2->Get(i))) {
            LOGE("Push package node failed!");
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
        LOGE("Parse from string1 failed![%s]", srcPackInfoJsonStr1.c_str());
        return false;
    }
    if (!srcPackInfo2.ParseFromString(srcPackInfoJsonStr2)) {
        LOGE("Parse from string2 failed![%s]", srcPackInfoJsonStr2.c_str());
        return false;
    }
    if (!VerifyPackInfos(srcPackInfo1, srcPackInfo2)) {
        LOGE("VerifyPackInfos failed!");
        return false;
    }
    auto iter = packagesMap.begin();
    while (iter != packagesMap.end()) {
        std::string packageName = iter->first;
        std::string moduleName = iter->second;
        std::string tmpStr = packageName.substr(0, packageName.find_last_of(DOT));
        if (!MergeTwoPackInfosByPackagePair(srcPackInfo1, srcPackInfo2, tmpStr, moduleName)) {
            LOGE("MergeTwoPackInfosByPackagePair failed!");
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
        LOGE("Get module node from srcPackInfo1 or srcPackInfo2 failed!");
        return false;
    }
    if (!modulesObj1->IsArray() || !modulesObj2->IsArray()) {
        LOGE("Module node is not array!");
        return false;
    }
    bool isFind = false;
    for (int32_t i = 0; i < modulesObj2->GetSize(); i++) {
        std::unique_ptr<PtJson> distroObj;
        if (!srcPackInfo2.GetDistroObject(i, distroObj)) {
            LOGE("GetDistroObject failed!");
            return false;
        }
        std::string moduleNameInDistroObj;
        if (!srcPackInfo2.GetModuleNameByDistroObj(distroObj, moduleNameInDistroObj)) {
            LOGE("GetModuleNameByDistroObj failed!");
            return false;
        }
        if (moduleNameInDistroObj.compare(moduleName) == 0) {
            if (!modulesObj1->Push(modulesObj2->Get(i))) {
                LOGE("Push module node failed!");
                return false;
            }
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        LOGE("Cannot find same moduleName in module node!");
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
        LOGE("Get package node from srcPackInfo1 or srcPackInfo2 failed!");
        return false;
    }
    if (!packagesObj1->IsArray() || !packagesObj2->IsArray()) {
        LOGE("Package node is not array!");
        return false;
    }
    bool isFind = false;
    for (int32_t i = 0; i < packagesObj2->GetSize(); i++) {
        std::string packageNameInPackageObj;
        if (!srcPackInfo2.GetNameByPackageObj(packagesObj2->Get(i), packageNameInPackageObj)) {
            LOGE("GetNameByPackageObj failed!");
            return false;
        }
        if (packageNameInPackageObj.compare(packageName) == 0) {
            if (!packagesObj1->Push(packagesObj2->Get(i))) {
                LOGE("Push Pacakge node failed!");
                return false;
            }
            isFind = true;
            break;
        }
    }
    if (!isFind) {
        LOGE("Cannot find same packageName in package node!");
        return false;
    }
    return true;
}

// java : mergeTwoPackInfoObjByPackagePair
bool PackInfoUtils::MergeTwoPackInfosByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
    const std::string& packageName, const std::string& moduleName)
{
    if (!FindAndMergeModulesByPackagePair(srcPackInfo1, srcPackInfo2, moduleName)) {
        LOGE("FindAndMergeModulesByPackagePair failed!");
        return false;
    }
    if (!FindAndMergePackagesByPackagePair(srcPackInfo1, srcPackInfo2, packageName)) {
        LOGE("FindAndMergePackagesByPackagePair failed!");
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
        LOGE("GetAppObject failed!");
        return false;
    }
    if (!CheckBundleNameInPackInfo(packInfo1, packInfo2)) {
        LOGE("CheckBundleNameInPackInfo failed!");
        return false;
    }
    if (!CheckBundleTypeInPackInfo(packInfo1, packInfo2)) {
        LOGE("CheckBundleTypeInPackInfo failed!");
        return false;
    }
    if (!CheckVersionCodeInPackInfo(packInfo1, packInfo2)) {
        LOGE("CheckVersionCodeInPackInfo failed!");
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckBundleNameInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    std::string bundleName1;
    std::string bundleName2;
    if (!const_cast<PackInfo&>(packInfo1).GetBundleName(bundleName1)) {
        LOGE("GetBundleName from packInfo1 failed!");
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetBundleName(bundleName2)) {
        LOGE("GetBundleName from packInfo2 failed!");
        return false;
    }
    if (bundleName1.compare(bundleName2) != 0) {
        LOGE("bundleName is not same");
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckBundleTypeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    std::string bundleType1;
    std::string bundleType2;
    if (!const_cast<PackInfo&>(packInfo1).GetBundleType(bundleType1, DEFAULT_BUNDLE_TYPE)) {
        LOGE("GetBundleType from packInfo1 failed!");
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetBundleType(bundleType2, DEFAULT_BUNDLE_TYPE)) {
        LOGE("GetBundleType from packInfo2 failed!");
        return false;
    }
    if (bundleType1.compare(bundleType2) != 0) {
        LOGE("bundleType is not same");
        return false;
    }
    return true;
}

bool PackInfoUtils::CheckVersionCodeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2)
{
    PackInfoVersion version1;
    PackInfoVersion version2;
    if (!const_cast<PackInfo&>(packInfo1).GetVersion(version1)) {
        LOGE("GetVersion from packInfo1 failed!");
        return false;
    }
    if (!const_cast<PackInfo&>(packInfo2).GetVersion(version2)) {
        LOGE("GetVersion from packInfo2 failed!");
        return false;
    }
    if (version1.code != version2.code) {
        LOGE("Version is not same");
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
