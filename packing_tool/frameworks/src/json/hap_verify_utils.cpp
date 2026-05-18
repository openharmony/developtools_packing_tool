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

#include "hap_verify_utils.h"

#include <algorithm>
#include <optional>
#include <set>
#include <vector>

#include "constants.h"
#include "error/packing_tool_err_msg.h"
#include "hap_verify_info.h"
#include "log.h"
#include "utils.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string ENTRY = "entry";
const std::string FEATURE = "feature";
const std::string SHARED_LIBRARY = "shared";
const std::string EMPTY_STRING = "";
const std::string REFERENCE_LINK = "FAQ";
const std::string INCLUDE = "include";
const std::string EXCLUDE = "exclude";
const std::string MODULE_NAME = "moduleName";
const std::string PACKAGE_NAME = "packageName";
const std::string ENTRY_NAME = "entryName";
const std::string ATOMIC_SERVICE = "atomicService";
const std::string TYPE_SHARED = "shared";
const std::string HAR = "har";
const std::string HAP_SUFFIX = ".hap";
const std::string HSP_SUFFIX = ".hsp";
const std::string APP_PLUGIN = "appPlugin";
const std::string NULL_DEVICE_TYPE = "nullDeviceType";
const int32_t TWO = 2;
const long FILE_LENGTH_1KB = 1024L;

std::string FormatModuleNameList(const std::list<std::string>& moduleNames)
{
    std::string result = "[";
    for (auto iter = moduleNames.begin(); iter != moduleNames.end(); ++iter) {
        if (iter != moduleNames.begin()) {
            result += ", ";
        }
        result += *iter;
    }
    result += "]";
    return result;
}
}

bool HapVerifyUtils::CheckHapIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "Hap verify infos is null or empty.").c_str());
        return false;
    }
    if (!CheckIsPluginApp(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("CheckIsPluginApp failed").c_str());
        return false;
    }
    if (!CheckAppFieldsIsSame(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("CheckAppFieldsIsSame failed!").c_str());
        return false;
    }
    if (!CheckModuleNameIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_MODULE_NAME_INVALID.toStringWithArgs(
            "CheckModuleNameIsValid failed!").c_str());
        return false;
    }
    if (!CheckPackageNameIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_PACKAGE_NAME_INVALID.toStringWithArgs(
            "Package name is duplicated.").c_str());
        return false;
    }
    if (!CheckEntryIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_ENTRY_INVALID.toStringWithArgs("CheckEntryIsValid failed!").c_str());
        return false;
    }
    if (!CheckDependencyIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_DEPENDENCY_INVALID.toStringWithArgs(
            "CheckDependencyIsValid failed!").c_str());
        return false;
    }
    if (!CheckAtomicServiceIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("CheckAtomicServiceIsValid failed.").c_str());
        return false;
    }
    if (!CheckAbilityNameIsValid(hapVerifyInfos)) {
        LOGI("CheckAbilityNameIsValid failed. Ability name is duplicated.");
    }
    if (!CheckTargetModuleNameIsExisted(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::TARGET_MODULE_NAME_NOT_EXIST.toStringWithArgs(
            "The target modules are needed to pack with the overlay module.").c_str());
        return false;
    }
    if (!CheckCompileSdkIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::COMPILE_SDK_TYPE_DIFFERENT.toStringWithArgs(
            std::vector<std::string>{
                "CompileSdkType is not the same for all modules.",
                "Ensure that all modules has same compileSdkType."
            }).c_str());
        return false;
    }
    if (!CheckProxyDataUriIsUnique(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE.toStringWithArgs(
            "CheckProxyDataUriIsUnique failed. Uris in proxy data are not unique.").c_str());
        return false;
    }
    if (!CheckContinueTypeIsValid(hapVerifyInfos)) {
        LOGE("%s", PackingToolErrMsg::CONTINUE_TYPE_INVALID.toStringWithArgs(
            "CheckContinueTypeIsValid failed.").c_str());
        return false;
    }
    return true;
}

bool HapVerifyUtils::CheckIsPluginApp(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    auto it = std::find_if(hapVerifyInfos.begin(), hapVerifyInfos.end(),
        [](const HapVerifyInfo& hapVerifyInfo) {
            return hapVerifyInfo.GetBundleType() == APP_PLUGIN;
        });
    if (it != hapVerifyInfos.end()) {
        if (hapVerifyInfos.size() != 1) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                "plugin App must contain only one element").c_str());
            return false;
        }
        HapVerifyInfo hapVerifyInfo = *hapVerifyInfos.begin();
        if (hapVerifyInfo.GetFileType() != HSP_SUFFIX) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                "plugin App must be of type hsp").c_str());
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::CheckAppFieldsIsSame(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("Hap verify infos is empty.").c_str());
        return false;
    }
    auto itBase = std::find_if(hapVerifyInfos.begin(), hapVerifyInfos.end(),
        [](const HapVerifyInfo& h) {
            return h.GetModuleType() != TYPE_SHARED;
        });
    HapVerifyInfo baseInfo = (itBase != hapVerifyInfos.end()) ? *itBase : *hapVerifyInfos.begin();
    VerifyCollection verifyCollection = GetVerifyCollection(baseInfo);
    auto itEntry = std::find_if(hapVerifyInfos.begin(), hapVerifyInfos.end(),
        [](const HapVerifyInfo& h) {
            return h.GetModuleType() == ENTRY;
        });
    HapVerifyInfo entryInfo = (itEntry != hapVerifyInfos.end()) ? *itEntry : baseInfo;
    std::string moduleName = entryInfo.GetModuleName();
    std::list<std::string> assetAccessGroups = entryInfo.GetAssetAccessGroups();
    std::list<HapVerifyInfo> hapList;
    std::list<HapVerifyInfo> hspList;
    HapVerifyInfo minCompatibleVersionCodeMaxInfo;
    HapVerifyInfo targetApiVersionMaxInfo;
    HapVerifyInfo minApiVersionMaxInfo;
    for (const auto& hap : hapVerifyInfos) {
        if (!AppFieldsIsSame(verifyCollection, hap)) {
            LOGW("Module: (%s) and Module: (%s) has different values.",
                 verifyCollection.moduleName.c_str(), hap.GetModuleName().c_str());
            return false;
        }
        if (!AppAssetAccessGroupsIsSame(assetAccessGroups, hap)) {
            LOGW("Module: (%s) and Module: (%s) has different values.",
                 moduleName.c_str(), hap.GetModuleName().c_str());
        }
        if (hap.GetFileType() == HAP_SUFFIX) {
            hapList.emplace_back(hap);
        } else if (hap.GetFileType() == HSP_SUFFIX) {
            hspList.emplace_back(hap);
            if (hap.GetVersion().minCompatibleVersionCode >
                minCompatibleVersionCodeMaxInfo.GetVersion().minCompatibleVersionCode) {
                minCompatibleVersionCodeMaxInfo = hap;
            }
            if (hap.GetApiVersion().targetApiVersion > targetApiVersionMaxInfo.GetApiVersion().targetApiVersion) {
                targetApiVersionMaxInfo = hap;
            }
            if (hap.GetApiVersion().compatibleApiVersion > minApiVersionMaxInfo.GetApiVersion().compatibleApiVersion) {
                minApiVersionMaxInfo = hap;
            }
        }
    }

    return AppFieldsIsValid(hapList, minCompatibleVersionCodeMaxInfo,
                            targetApiVersionMaxInfo, minApiVersionMaxInfo) &&
           ModuleDebugValidation(hapList, hspList);
}

VerifyCollection HapVerifyUtils::GetVerifyCollection(const HapVerifyInfo& baseInfo)
{
    VerifyCollection verifyCollection;
    verifyCollection.bundleName = baseInfo.GetBundleName();
    verifyCollection.bundleType = baseInfo.GetBundleType();
    verifyCollection.vendor = baseInfo.GetVendor();
    verifyCollection.versionCode = baseInfo.GetVersion().versionCode;
    verifyCollection.buildVersion = baseInfo.GetVersion().buildVersion;
    verifyCollection.versionName = baseInfo.GetVersion().versionName;
    verifyCollection.compatibleApiVersion = baseInfo.GetApiVersion().compatibleApiVersion;
    verifyCollection.releaseType = baseInfo.GetApiVersion().releaseType;
    verifyCollection.targetBundleName = baseInfo.GetTargetBundleName();
    verifyCollection.targetPriority = baseInfo.GetTargetPriority();
    verifyCollection.debug = baseInfo.IsDebug();
    verifyCollection.moduleName = baseInfo.GetModuleName();
    verifyCollection.moduleType = baseInfo.GetModuleType();
    verifyCollection.multiAppMode = baseInfo.GetMultiAppMode();
    return verifyCollection;
}

bool HapVerifyUtils::AppFieldsIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos,
                                      const HapVerifyInfo& minCompatibleVersionCodeMaxInfo,
                                      const HapVerifyInfo& targetApiVersionMaxInfo,
                                      const HapVerifyInfo& minApiVersionMaxInfo)
{
    if (hapVerifyInfos.empty()) {
        LOGD("Hap verify infos is empty");
        return true;
    }
    const HapVerifyInfo& baseHap = *hapVerifyInfos.begin();
    int32_t baseMinCompatibleVersionCode = baseHap.GetVersion().minCompatibleVersionCode;
    int32_t baseTargetApiVersion = baseHap.GetApiVersion().targetApiVersion;
    int32_t baseCompatibleApiVersion = baseHap.GetApiVersion().compatibleApiVersion;
    for (const auto& hap : hapVerifyInfos) {
        if (baseMinCompatibleVersionCode != hap.GetVersion().minCompatibleVersionCode) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                std::string("The minCompatibleVersionCode attribute values of two Hap are different. ") +
                "Hap[" + baseHap.GetModuleName() + "]: (minCompatibleVersionCode: " +
                std::to_string(baseMinCompatibleVersionCode) + "); " +
                "Hap[" + hap.GetModuleName() + "]: (minCompatibleVersionCode: " +
                std::to_string(hap.GetVersion().minCompatibleVersionCode) + "). " +
                "Solution: Ensure values of minCompatibleVersionCode are same in each HAP.").c_str());
            return false;
        }
        if (baseTargetApiVersion != hap.GetApiVersion().targetApiVersion) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                std::string("The targetApiVersion attribute values of two Hap are different. ") +
                "Hap[" + baseHap.GetModuleName() + "]: (targetApiVersion: " +
                std::to_string(baseTargetApiVersion) + "); " +
                "Hap[" + hap.GetModuleName() + "]: (targetApiVersion: " +
                std::to_string(hap.GetApiVersion().targetApiVersion) + "). " +
                "Solution: Ensure values of targetApiVersion are same in each HAP.").c_str());
            return false;
        }
        if (baseCompatibleApiVersion != hap.GetApiVersion().compatibleApiVersion) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                std::string("The minApiVersion attribute values of two Hap are different. ") +
                "Hap[" + baseHap.GetModuleName() + "]: (minApiVersion: " +
                std::to_string(baseCompatibleApiVersion) + "); " +
                "Hap[" + hap.GetModuleName() + "]: (minApiVersion: " +
                std::to_string(hap.GetApiVersion().compatibleApiVersion) + "). " +
                "Solution: Ensure values of minApiVersion are same in each HAP.").c_str());
            return false;
        }
    }
    return CheckField("minCompatibleVersionCode", baseMinCompatibleVersionCode, baseHap,
                      minCompatibleVersionCodeMaxInfo.GetVersion().minCompatibleVersionCode,
                      minCompatibleVersionCodeMaxInfo) &&
           CheckField("targetApiVersion", baseTargetApiVersion, baseHap,
                      targetApiVersionMaxInfo.GetApiVersion().targetApiVersion,
                      targetApiVersionMaxInfo) &&
           CheckField("minApiVersion", baseCompatibleApiVersion, baseHap,
                      minApiVersionMaxInfo.GetApiVersion().compatibleApiVersion,
                      minApiVersionMaxInfo);
}

bool HapVerifyUtils::CheckField(const std::string& field, int32_t hapVal, const HapVerifyInfo& hap,
                                int32_t hspVal, const HapVerifyInfo& hsp)
{
    if (hapVal < hspVal) {
        LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_INVALID.toStringWithArgs(
            std::vector<std::string>{
                std::string("The values of ") + field + "[" + std::to_string(hapVal) +
                "] in HAP[" + hap.GetModuleName() + "] < " +
                field + "[" + std::to_string(hspVal) + "] of HSP[" + hsp.GetModuleName() + "].",
                "Ensure that the values of " + field + " in HAP >= HSP."
            }).c_str());
        return false;
    }
    return true;
}

bool HapVerifyUtils::ModuleDebugValidation(const std::list<HapVerifyInfo> hapVerifyInfos,
    const std::list<HapVerifyInfo> hspVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGD("Hap verify infos is empty.");
        return true;
    }
    HapVerifyInfo hap = *hapVerifyInfos.begin();
    for (const HapVerifyInfo& hapInfo : hapVerifyInfos) {
        if (hap.IsDebug() != hapInfo.IsDebug()) {
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_INVALID.toStringWithArgs(
                std::vector<std::string>{
                    std::string("The debug fields of Hap[") + hap.GetModuleName() +
                    "] and Hap[" + hapInfo.GetModuleName() + "] are different.",
                    "Ensure that the values of 'debug' in the module.json file of each HAP module are the same."
                }).c_str());
            return false;
        }
    }
    if (hap.IsDebug() || hspVerifyInfos.empty()) {
        LOGI("Hap debug is true or hspVerifyInfos empty");
        return true;
    }
    std::list<std::string> hspDebugNames;
    for (const HapVerifyInfo& hapInfo : hspVerifyInfos) {
        if (hapInfo.IsDebug()) {
            hspDebugNames.push_back(hapInfo.GetModuleName());
        }
    }
    if (!hspDebugNames.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_INVALID.toStringWithArgs(
            std::vector<std::string>{
                "Detected HAP(s) with debug=false, but some HSP" + FormatModuleNameList(hspDebugNames) +
                " are debug=true.",
                "When the debug value of Hap is false,the debug value of Hsp should also be false."
            }).c_str());
        return false;
    }
    return true;
}

bool HapVerifyUtils::AppFieldsIsSame(const VerifyCollection& verifyCollection, const HapVerifyInfo& hapVerifyInfo)
{
    if (hapVerifyInfo.GetBundleName().empty() || verifyCollection.bundleName != hapVerifyInfo.GetBundleName()) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The bundleName parameter values are different.",
                "Check if the bundleName is the same in different modules."
            }).c_str());
        return false;
    }
    if (verifyCollection.bundleType != hapVerifyInfo.GetBundleType()) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The bundleType parameter values are different.",
                "Check if the bundleType is the same in different modules."
            }).c_str());
        return false;
    }
    if (verifyCollection.versionCode != hapVerifyInfo.GetVersion().versionCode) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The versionCode parameter values are different.",
                "Check if the versionCode is the same in different modules."
            }).c_str());
        return false;
    }
    if (verifyCollection.buildVersion != hapVerifyInfo.GetVersion().buildVersion) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The buildVersion parameter values are different.",
                std::string("Module: (") + verifyCollection.moduleName + ") and Module: (" +
                hapVerifyInfo.GetModuleName() + ") has different buildVersion."
            }).c_str());
        return false;
    }
    if (verifyCollection.releaseType != hapVerifyInfo.GetApiVersion().releaseType) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            "input module releaseType is different.").c_str());
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            "Solutions: > Check if the releaseType is the same in different modules.").c_str());
        return false;
    }
    if (verifyCollection.targetBundleName != hapVerifyInfo.GetTargetBundleName()) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The targetBundleName parameter values are different.",
                "Check if the targetBundleName is the same in different modules."
            }).c_str());
        return false;
    }
    if (verifyCollection.targetPriority != hapVerifyInfo.GetTargetPriority()) {
        LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
            std::vector<std::string>{
                "The targetPriority parameter values are different.",
                "Check if the targetPriority is the same in different modules."
            }).c_str());
        return false;
    }
    if (IsEntryOrFeature(verifyCollection.moduleType) && IsEntryOrFeature(hapVerifyInfo.GetModuleType())) {
        if (verifyCollection.multiAppMode != (hapVerifyInfo.GetMultiAppMode())) {
            LOGE("%s", PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR.toStringWithArgs(
                std::vector<std::string>{
                    "The multiAppMode parameter values are different.",
                    "Check if the multiAppMode is the same in different modules."
                }).c_str());
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::AppAssetAccessGroupsIsSame(const std::list<std::string>& assetAccessGroups,
    const HapVerifyInfo& hapVerifyInfo)
{
    std::set<std::string> inputSet(assetAccessGroups.begin(), assetAccessGroups.end());
    std::set<std::string> infoSet(hapVerifyInfo.GetAssetAccessGroups().begin(),
        hapVerifyInfo.GetAssetAccessGroups().end());

    if (inputSet != infoSet) {
        LOGW("input module assetAccessGroups is different.");
        return false;
    }
    return true;
}

bool HapVerifyUtils::IsEntryOrFeature(const std::string& moduleType)
{
    return ((moduleType == ENTRY) || (moduleType == FEATURE));
}

void HapVerifyUtils::ShowCheckTips(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2,
    const std::string& tip)
{
    LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
        std::string("Module: (") + hapVerifyInfo1.GetModuleName() + ") and Module: (" +
        hapVerifyInfo2.GetModuleName() + ") have the same " + tip + ", " +
        "please check deviceType or distroFilter/distributionFilter of the module.").c_str());
    LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
        std::string("Module: ") + hapVerifyInfo1.GetModuleName() + " has deviceType " +
        Utils::ListToString(hapVerifyInfo1.GetDeviceTypes()) + ".").c_str());

    LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
        std::string("Another Module: ") + hapVerifyInfo2.GetModuleName() + " has deviceType " +
        Utils::ListToString(hapVerifyInfo2.GetDeviceTypes()) + ".").c_str());
    if (hapVerifyInfo1.GetDistroFilter().Dump() != EMPTY_STRING) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
            std::string("Module: ") + hapVerifyInfo1.GetModuleName() +
            " DistroFilter/DistributionFilter is : " + hapVerifyInfo1.GetDistroFilter().Dump() + ".").c_str());
    }
    if (hapVerifyInfo2.GetDistroFilter().Dump() != EMPTY_STRING) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
            std::string("Another Module: ") + hapVerifyInfo2.GetModuleName() +
            " DistroFilter/DistributionFilter is " + hapVerifyInfo2.GetDistroFilter().Dump() + ".").c_str());
    }

    LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
        std::string("Solution: Make sure the ") + tip + " is valid and unique.").c_str());

    LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
        std::string("Reference: ") + REFERENCE_LINK + ".").c_str());
}

bool HapVerifyUtils::CheckModuleNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto iter1 = hapVerifyInfos.begin(); iter1 != hapVerifyInfos.end(); iter1++) {
        const HapVerifyInfo& hapVerifyInfo1 = *iter1;
        if (hapVerifyInfo1.GetModuleName().empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_MODULE_NAME_INVALID.toStringWithArgs(
                "module name can not be empty.").c_str());
            return false;
        }
        for (auto iter2 = std::next(iter1); iter2 != hapVerifyInfos.end(); iter2++) {
            const HapVerifyInfo& hapVerifyInfo2 = *iter2;
            if (hapVerifyInfo1.GetModuleName() == hapVerifyInfo2.GetModuleName() &&
                !CheckDuplicatedIsValid(hapVerifyInfo1, hapVerifyInfo2)) {
                ShowCheckTips(hapVerifyInfo1, hapVerifyInfo2, MODULE_NAME);
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckDuplicatedIsValid(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2)
{
    if (Utils::CheckDisjoint(hapVerifyInfo1.GetDeviceTypes(), hapVerifyInfo2.GetDeviceTypes())) {
        return true;
    }
    if (CheckDistroFilterDisjoint(hapVerifyInfo1.GetDistroFilter(), hapVerifyInfo2.GetDistroFilter())) {
        return true;
    }
    return false;
}

bool HapVerifyUtils::CheckDistroFilterDisjoint(const DistroFilter& distroFilter1, const DistroFilter& distroFilter2)
{
    if (distroFilter1.IsEmpty() || distroFilter2.IsEmpty()) {
        return false;
    }
    if (!distroFilter1.apiVersion.IsEmpty() && !distroFilter2.apiVersion.IsEmpty()) {
        if (CheckPolicyValueDisjoint(distroFilter1.apiVersion, distroFilter2.apiVersion)) {
            return true;
        }
    }
    if (!distroFilter1.screenShape.IsEmpty() && !distroFilter2.screenShape.IsEmpty()) {
        if (CheckPolicyValueDisjoint(distroFilter1.screenShape, distroFilter2.screenShape)) {
            return true;
        }
    }
    if (!distroFilter1.screenDensity.IsEmpty() && !distroFilter2.screenDensity.IsEmpty()) {
        if (CheckPolicyValueDisjoint(distroFilter1.screenDensity, distroFilter2.screenDensity)) {
            return true;
        }
    }
    if (!distroFilter1.screenWindow.IsEmpty() && !distroFilter2.screenWindow.IsEmpty()) {
        if (CheckPolicyValueDisjoint(distroFilter1.screenWindow, distroFilter2.screenWindow)) {
            return true;
        }
    }
    if (!distroFilter1.countryCode.IsEmpty() && !distroFilter2.countryCode.IsEmpty()) {
        if (CheckPolicyValueDisjoint(distroFilter1.countryCode, distroFilter2.countryCode)) {
            return true;
        }
    }
    return false;
}

bool HapVerifyUtils::CheckPolicyValueDisjoint(const PolicyValue& policyValue1, const PolicyValue& policyValue2)
{
    if (policyValue1.policy == EXCLUDE && policyValue2.policy == INCLUDE) {
        if (policyValue2.value.empty() || Utils::CheckContainsAll(policyValue1.value, policyValue2.value)) {
            return true;
        }
    } else if (policyValue1.policy == INCLUDE && policyValue2.policy == INCLUDE) {
        if (Utils::CheckDisjoint(policyValue1.value, policyValue2.value)) {
            return true;
        }
    } else if (policyValue1.policy == INCLUDE && policyValue2.policy == EXCLUDE) {
        if (policyValue1.value.empty() || Utils::CheckContainsAll(policyValue2.value, policyValue1.value)) {
            return true;
        }
    } else if (policyValue1.policy == EXCLUDE && policyValue2.policy == EXCLUDE) {
        LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
            "input policys are both exclude.").c_str());
        return false;
    } else {
        LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
            "input policy is invalid.").c_str());
    }
    return false;
}

bool HapVerifyUtils::CheckPackageNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto iter1 = hapVerifyInfos.begin(); iter1 != hapVerifyInfos.end(); iter1++) {
        const HapVerifyInfo& hapVerifyInfo1 = *iter1;
        if (hapVerifyInfo1.GetPackageName().empty()) {
            continue;
        }
        for (auto iter2 = std::next(iter1); iter2 != hapVerifyInfos.end(); iter2++) {
            const HapVerifyInfo& hapVerifyInfo2 = *iter2;
            if (hapVerifyInfo1.GetPackageName() == hapVerifyInfo2.GetPackageName() &&
                !CheckDuplicatedIsValid(hapVerifyInfo1, hapVerifyInfo2)) {
                ShowCheckTips(hapVerifyInfo1, hapVerifyInfo2, PACKAGE_NAME);
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckEntryIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    std::list<HapVerifyInfo> entryHapVerifyInfos;
    std::list<HapVerifyInfo> featureHapVerifyInfos;
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetModuleType() == ENTRY) {
            entryHapVerifyInfos.push_back(hapVerifyInfo);
        } else if (hapVerifyInfo.GetModuleType() == FEATURE) {
            featureHapVerifyInfos.push_back(hapVerifyInfo);
        } else if (hapVerifyInfo.GetModuleType() != SHARED_LIBRARY &&
            hapVerifyInfo.GetModuleType() != Constants::TYPE_SKILL) {
            LOGW("Input wrong type module.");
        }
    }
    if (hapVerifyInfos.empty() ||
        (entryHapVerifyInfos.empty() && (*hapVerifyInfos.begin()).GetBundleType() != SHARED_LIBRARY &&
            (*hapVerifyInfos.begin()).GetBundleType() != Constants::TYPE_SKILL)) {
        LOGW("has no entry module.");
    }
    for (auto iter1 = entryHapVerifyInfos.begin(); iter1 != entryHapVerifyInfos.end(); iter1++) {
        const HapVerifyInfo& entryHapVerifyInfo1 = *iter1;
        for (auto iter2 = std::next(iter1); iter2 != entryHapVerifyInfos.end(); iter2++) {
            const HapVerifyInfo& entryHapVerifyInfo2 = *iter2;
            if (!CheckDuplicatedIsValid(entryHapVerifyInfo1, entryHapVerifyInfo2)) {
                ShowCheckTips(entryHapVerifyInfo1, entryHapVerifyInfo2, ENTRY_NAME);
                return false;
            }
        }
    }
    std::map<std::string, std::list<HapVerifyInfo>> deviceHaps;
    ClassifyEntry(entryHapVerifyInfos, deviceHaps);
    for (auto& hapVerifyInfo : featureHapVerifyInfos) {
        if (!CheckFeature(hapVerifyInfo, deviceHaps)) {
            LOGW("%s can not be covered by entry.", hapVerifyInfo.GetModuleName().c_str());
        }
    }
    return true;
}

void HapVerifyUtils::ClassifyEntry(const std::list<HapVerifyInfo>& entryHapVerifyInfos,
    std::map<std::string, std::list<HapVerifyInfo>>& deviceHaps)
{
    for (auto& hapVerifyInfo : entryHapVerifyInfos) {
        for (std::string device : hapVerifyInfo.GetDeviceTypes()) {
            if (deviceHaps.count(device) > 0) {
                deviceHaps[device].push_back(hapVerifyInfo);
            } else {
                std::list<HapVerifyInfo> hapVerifyInfos;
                hapVerifyInfos.push_back(hapVerifyInfo);
                deviceHaps.insert(std::make_pair(device, hapVerifyInfos));
            }
        }
    }
}

bool HapVerifyUtils::CheckFeature(const HapVerifyInfo& featureHap,
    const std::map<std::string, std::list<HapVerifyInfo>>& deviceHap)
{
    for (auto& device : featureHap.GetDeviceTypes()) {
        if (deviceHap.count(device) <= 0) {
            LOGW("Warning: device %s has feature but has no entry.", device.c_str());
            return false;
        }
        if (!CheckFeatureDistroFilter(featureHap, deviceHap.at(device))) {
            LOGW("%s's distroFilter has not covered by entry.", featureHap.GetModuleName().c_str());
            if (featureHap.GetDistroFilter().Dump() != EMPTY_STRING) {
                LOGW("%s has %s.", featureHap.GetModuleName().c_str(),
                    featureHap.GetDistroFilter().Dump().c_str());
            }
            return false;
        }
    }
    return true;
}


bool HapVerifyUtils::CheckFeatureDistroFilter(const HapVerifyInfo& featureHap,
    const std::list<HapVerifyInfo>& entryHaps)
{
    if (featureHap.GetDistroFilter().IsEmpty()) {
        if (CheckApiVersionCovered(featureHap.GetDistroFilter().apiVersion, entryHaps) &&
            CheckScreenShapeCovered(featureHap.GetDistroFilter().screenShape, entryHaps) &&
            CheckScreenWindowCovered(featureHap.GetDistroFilter().screenWindow, entryHaps) &&
            CheckScreenDensityCovered(featureHap.GetDistroFilter().screenDensity, entryHaps) &&
            CheckCountryCodeCovered(featureHap.GetDistroFilter().countryCode, entryHaps)) {
            return true;
        } else {
            return false;
        }
    }
    if (!CheckApiVersionCovered(featureHap.GetDistroFilter().apiVersion, entryHaps)) {
        LOGW("CheckApiVersionCovered failed, apiVersion is not covered.");
        return false;
    }
    if (!CheckScreenShapeCovered(featureHap.GetDistroFilter().screenShape, entryHaps)) {
        LOGW("CheckScreenShapeCovered failed, screenShape is not covered.");
        return false;
    }
    if (!CheckScreenWindowCovered(featureHap.GetDistroFilter().screenWindow, entryHaps)) {
        LOGW("CheckScreenWindowCovered failed, screenWindow is not covered.");
        return false;
    }
    if (!CheckScreenDensityCovered(featureHap.GetDistroFilter().screenDensity, entryHaps)) {
        LOGW("CheckScreenDensityCovered failed, screenDensity is not covered.");
        return false;
    }
    if (!CheckCountryCodeCovered(featureHap.GetDistroFilter().countryCode, entryHaps)) {
        LOGW("HapVerify::checkFeatureDistroFilter failed, countryCode is not covered.");
        return false;
    }
    return true;
}

bool HapVerifyUtils::CheckApiVersionCovered(const ApiVersion& apiVersion, const std::list<HapVerifyInfo>& entryHaps)
{
    std::set<std::string> includeSet;
    std::set<std::string> excludeSet;
    for (auto& hapVerifyInfo : entryHaps) {
        if (hapVerifyInfo.GetDistroFilter().IsEmpty() || hapVerifyInfo.GetDistroFilter().apiVersion.IsEmpty()) {
            LOGW("distro filter is empty or apiVersion is empty.");
            return true;
        }
        if (hapVerifyInfo.GetDistroFilter().apiVersion.policy.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs("input none policy.").c_str());
            return false;
        }
        if (hapVerifyInfo.GetDistroFilter().apiVersion.policy == INCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().apiVersion.value, includeSet);
        } else if (hapVerifyInfo.GetDistroFilter().apiVersion.policy == EXCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().apiVersion.value, excludeSet);
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
                "input policy is invalid.").c_str());
            return false;
        }
    }
    std::list<std::string> include(includeSet.begin(), includeSet.end());
    std::list<std::string> exclude(excludeSet.begin(), excludeSet.end());
    if (apiVersion.IsEmpty()) {
        return CheckEntryPolicyValueCoverAll(include, exclude);
    }
    return CheckPolicyValueCovered(apiVersion, include, exclude);
}

bool HapVerifyUtils::CheckScreenShapeCovered(const ScreenShape& screenShape, const std::list<HapVerifyInfo>& entryHaps)
{
    std::set<std::string> includeSet;
    std::set<std::string> excludeSet;
    for (HapVerifyInfo hapVerifyInfo : entryHaps) {
        if (hapVerifyInfo.GetDistroFilter().IsEmpty() || hapVerifyInfo.GetDistroFilter().screenShape.IsEmpty()) {
            LOGW("distro filter is empty or screenShape is empty.");
            return true;
        }
        if (hapVerifyInfo.GetDistroFilter().screenShape.policy.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs("input none policy.").c_str());
            return false;
        }
        if (hapVerifyInfo.GetDistroFilter().screenShape.policy == INCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenShape.value, includeSet);
        } else if (hapVerifyInfo.GetDistroFilter().screenShape.policy == EXCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenShape.value, excludeSet);
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
                "input policy is invalid.").c_str());
            return false;
        }
    }
    std::list<std::string> include(includeSet.begin(), includeSet.end());
    std::list<std::string> exclude(excludeSet.begin(), excludeSet.end());
    if (screenShape.IsEmpty()) {
        return CheckEntryPolicyValueCoverAll(include, exclude);
    }
    return CheckPolicyValueCovered(screenShape, include, exclude);
}

bool HapVerifyUtils::CheckScreenWindowCovered(const ScreenWindow& screenWindow,
    const std::list<HapVerifyInfo>& entryHaps)
{
    std::set<std::string> includeSet;
    std::set<std::string> excludeSet;
    for (HapVerifyInfo hapVerifyInfo : entryHaps) {
        if (hapVerifyInfo.GetDistroFilter().IsEmpty() || hapVerifyInfo.GetDistroFilter().screenWindow.IsEmpty()) {
            LOGW("distro filter is empty or screenWindow is empty.");
            return true;
        }
        if (hapVerifyInfo.GetDistroFilter().screenWindow.policy.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs("input none policy.").c_str());
            return false;
        }
        if (hapVerifyInfo.GetDistroFilter().screenWindow.policy == INCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenWindow.value, includeSet);
        } else if (hapVerifyInfo.GetDistroFilter().screenWindow.policy == EXCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenWindow.value, excludeSet);
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
                "input policy is invalid.").c_str());
            return false;
        }
    }
    std::list<std::string> include(includeSet.begin(), includeSet.end());
    std::list<std::string> exclude(excludeSet.begin(), excludeSet.end());
    if (screenWindow.IsEmpty()) {
        return CheckEntryPolicyValueCoverAll(include, exclude);
    }
    return CheckPolicyValueCovered(screenWindow, include, exclude);
}

bool HapVerifyUtils::CheckScreenDensityCovered(const ScreenDensity& screenDensity,
    const std::list<HapVerifyInfo>& entryHaps)
{
    std::set<std::string> includeSet;
    std::set<std::string> excludeSet;
    for (HapVerifyInfo hapVerifyInfo : entryHaps) {
        if (hapVerifyInfo.GetDistroFilter().IsEmpty() || hapVerifyInfo.GetDistroFilter().screenDensity.IsEmpty()) {
            LOGW("distro filter is empty or screenDensity is empty.");
            return true;
        }
        if (hapVerifyInfo.GetDistroFilter().screenDensity.policy.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs("input none policy.").c_str());
            return false;
        }
        if (hapVerifyInfo.GetDistroFilter().screenDensity.policy == INCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenDensity.value, includeSet);
        } else if (hapVerifyInfo.GetDistroFilter().screenDensity.policy == EXCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().screenDensity.value, excludeSet);
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
                "input policy is invalid.").c_str());
            return false;
        }
    }
    std::list<std::string> include(includeSet.begin(), includeSet.end());
    std::list<std::string> exclude(excludeSet.begin(), excludeSet.end());
    if (screenDensity.IsEmpty()) {
        return CheckEntryPolicyValueCoverAll(include, exclude);
    }
    return CheckPolicyValueCovered(screenDensity, include, exclude);
}

bool HapVerifyUtils::CheckCountryCodeCovered(const CountryCode& countryCode, const std::list<HapVerifyInfo>& entryHaps)
{
    std::set<std::string> includeSet;
    std::set<std::string> excludeSet;
    for (HapVerifyInfo hapVerifyInfo : entryHaps) {
        if (hapVerifyInfo.GetDistroFilter().IsEmpty() || hapVerifyInfo.GetDistroFilter().countryCode.IsEmpty()) {
            LOGW("distro filter is empty or countryCode is empty.");
            return true;
        }
        if (hapVerifyInfo.GetDistroFilter().countryCode.policy.empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs("input none policy.").c_str());
            return false;
        }
        if (hapVerifyInfo.GetDistroFilter().countryCode.policy == INCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().countryCode.value, includeSet);
        } else if (hapVerifyInfo.GetDistroFilter().countryCode.policy == EXCLUDE) {
            Utils::CopyListToSet(hapVerifyInfo.GetDistroFilter().countryCode.value, excludeSet);
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR.toStringWithArgs(
                "input policy is invalid.").c_str());
            return false;
        }
    }
    std::list<std::string> include(includeSet.begin(), includeSet.end());
    std::list<std::string> exclude(excludeSet.begin(), excludeSet.end());
    if (countryCode.IsEmpty()) {
        return CheckEntryPolicyValueCoverAll(include, exclude);
    }
    return CheckPolicyValueCovered(countryCode, include, exclude);
}

bool HapVerifyUtils::CheckEntryPolicyValueCoverAll(const std::list<std::string>& include,
    const std::list<std::string>& exclude)
{
    if (include.empty() && exclude.empty()) {
        return true;
    }
    return !exclude.empty() && Utils::CheckContainsAll(include, exclude);
}

bool HapVerifyUtils::CheckPolicyValueCovered(const PolicyValue& policyValue,
    const std::list<std::string>& include, std::list<std::string>& exclude)
{
    if (policyValue.policy == EXCLUDE) {
        return CheckCoveredExcludePolicyValue(policyValue.value, include, exclude);
    } else if (policyValue.policy == INCLUDE) {
        return CheckCoveredIncludePolicyValue(policyValue.value, include, exclude);
    }
    return false;
}


bool HapVerifyUtils::CheckCoveredExcludePolicyValue(const std::list<std::string>& value,
    const std::list<std::string>& include, std::list<std::string>& exclude)
{
    if (include.empty()) {
        return exclude.empty() || Utils::CheckContainsAll(value, exclude);
    }
    if (exclude.empty()) {
        return false;
    }
    exclude.remove_if([&include](const std::string& value) {
        return std::find(include.begin(), include.end(), value) != include.end();
    });
    return Utils::CheckContainsAll(value, exclude);
}

bool HapVerifyUtils::CheckCoveredIncludePolicyValue(const std::list<std::string>& value,
    const std::list<std::string>& include, std::list<std::string>& exclude)
{
    if (include.empty()) {
        return exclude.empty() || Utils::CheckDisjoint(exclude, value);
    }
    if (exclude.empty()) {
        return Utils::CheckContainsAll(include, value);
    }
    exclude.remove_if([&include](const std::string& value) {
        return std::find(include.begin(), include.end(), value) != include.end();
    });
    return Utils::CheckDisjoint(exclude, value);
}

bool HapVerifyUtils::CheckDependencyIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_DEPENDENCY_INVALID.toStringWithArgs("input none hap.").c_str());
        return false;
    }
    bool isInstallationFree = hapVerifyInfos.begin()->IsInstallationFree();
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (isInstallationFree != hapVerifyInfo.IsInstallationFree()) {
            LOGE("%s", PackingToolErrMsg::CHECK_DEPENDENCY_INVALID.toStringWithArgs(
                "installationFree is different in input hap.").c_str());
            return false;
        }
    }
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        std::list<HapVerifyInfo> dependencyList;
        dependencyList.push_back(hapVerifyInfo);
        if (!DfsTraverseDependency(hapVerifyInfo, hapVerifyInfos, dependencyList)) {
            return false;
        }
        dependencyList.pop_back();
    }
    return true;
}

bool HapVerifyUtils::DfsTraverseDependency(const HapVerifyInfo& hapVerifyInfo,
    const std::list<HapVerifyInfo>& hapVerifyInfos, std::list<HapVerifyInfo>& dependencyList)
{
    if (CheckDependencyListCirculate(dependencyList)) {
        return false;
    }
    for (const DependencyItem& dependency : hapVerifyInfo.GetDependencyItemList()) {
        if (dependency.bundleName != hapVerifyInfo.GetBundleName()) {
            continue;
        }
        if (!CheckDependencyInFileList(dependency, hapVerifyInfos)) {
            LOGW("Dependent module %s missing, check the HSP-Path.", dependency.bundleName.c_str());
            continue;
        }
        std::list<HapVerifyInfo> layerHapVerifyInfos;
        GetLayerHapVerifyInfos(dependency.moduleName, hapVerifyInfo, hapVerifyInfos, layerHapVerifyInfos);
        for (auto& layerHapVerifyInfo : layerHapVerifyInfos) {
            if (layerHapVerifyInfo.GetModuleType() == FEATURE ||
                layerHapVerifyInfo.GetModuleType() == ENTRY) {
                LOGE("%s", PackingToolErrMsg::DEPENDENCY_LIST_INVALID.toStringWithArgs(
                    std::string("HAP or HSP cannot depend on HAP ") +
                    layerHapVerifyInfo.GetModuleName() + ".").c_str());
                return false;
            }
            dependencyList.push_back(layerHapVerifyInfo);
            if (!DfsTraverseDependency(layerHapVerifyInfo, hapVerifyInfos, dependencyList)) {
                return false;
            }
            dependencyList.pop_back();
        }
    }
    return true;
}

bool HapVerifyUtils::CheckDependencyListCirculate(const std::list<HapVerifyInfo>& dependencyList)
{
    for (auto iter1 = dependencyList.begin(); iter1 != dependencyList.end(); iter1++) {
        const HapVerifyInfo& dependency1 = *iter1;
        for (auto iter2 = std::next(iter1); iter2 != dependencyList.end(); iter2++) {
            const HapVerifyInfo& dependency2 = *iter2;
            if (IsSameHapVerifyInfo(dependency1, dependency2)) {
                std::list<std::string> moduleNames;
                GetHapVerifyInfosNames(dependencyList, moduleNames);
                LOGE("%s", PackingToolErrMsg::DEPENDENCY_LIST_INVALID.toStringWithArgs(
                    std::string("circular dependency, dependencyList is ") +
                    Utils::ListToString(moduleNames) + ".").c_str());
                return true;
            }
        }
    }
    return false;
}

bool HapVerifyUtils::IsSameHapVerifyInfo(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2)
{
    if (hapVerifyInfo1.GetModuleName() != hapVerifyInfo2.GetModuleName()) {
        return false;
    }
    return CheckModuleJoint(hapVerifyInfo1, hapVerifyInfo2);
}

bool HapVerifyUtils::CheckModuleJoint(const HapVerifyInfo& infoLeft, const HapVerifyInfo& infoRight)
{
    return !CheckDuplicatedIsValid(infoLeft, infoRight);
}

void HapVerifyUtils::GetHapVerifyInfosNames(const std::list<HapVerifyInfo>& hapVerifyInfos,
    std::list<std::string>& moduleNames)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        moduleNames.push_back(hapVerifyInfo.GetModuleName());
    }
}

bool HapVerifyUtils::CheckDependencyInFileList(const DependencyItem& dependencyItem,
    const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (dependencyItem.moduleName == hapVerifyInfo.GetModuleName() &&
            dependencyItem.bundleName == hapVerifyInfo.GetBundleName()) {
            return true;
        }
    }
    return false;
}

void HapVerifyUtils::GetLayerHapVerifyInfos(const std::string& moduleName, const HapVerifyInfo& hapVerifyInfo,
    const std::list<HapVerifyInfo>& hapVerifyInfos, std::list<HapVerifyInfo>& layerHapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetModuleName() == moduleName && CheckModuleJoint(hapVerifyInfo, hapVerifyInfo)) {
            layerHapVerifyInfos.push_back(hapVerifyInfo);
        }
    }
}

bool HapVerifyUtils::CheckAtomicServiceIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "hapVerifyInfos is empty!").c_str());
        return false;
    }
    if (hapVerifyInfos.begin()->GetBundleType() != ATOMIC_SERVICE) {
        return true;
    }
    if (!hapVerifyInfos.begin()->IsStageModule()) {
        return true;
    }
    std::map<std::string, std::list<HapVerifyInfo>> deviceInfosMap;
    if (!GetDeviceHapVerifyInfoMap(hapVerifyInfos, deviceInfosMap)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("GetDeviceHapVerifyInfoMap failed!").c_str());
        return false;
    }
    std::map<std::string, std::list<HapVerifyInfo>>::iterator iter;
    for (iter = deviceInfosMap.begin(); iter != deviceInfosMap.end(); iter++) {
        const std::string& deviceType = iter->first;
        const std::list<HapVerifyInfo>& deviceInfos = iter->second;
        if (!CheckAtomicServicePreloadsIsValid(deviceInfos)) {
            LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                std::string("CheckAtomicServicePreloadsIsValid failed on device ") + deviceType + ".").c_str());
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::GetDeviceHapVerifyInfoMap(const std::list<HapVerifyInfo>& hapVerifyInfos,
    std::map<std::string, std::list<HapVerifyInfo>>& deviceInfosMap)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("hapVerifyInfos is empty!").c_str());
        return false;
    }
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        const std::list<std::string>& deviceTypes = hapVerifyInfo.GetDeviceTypes();
        for (auto& deviceType : deviceTypes) {
            if (deviceInfosMap.count(deviceType) <= 0) {
                std::list<HapVerifyInfo> deviceInfos;
                deviceInfos.push_back(hapVerifyInfo);
                deviceInfosMap.insert(std::make_pair(deviceType, deviceInfos));
            } else {
                deviceInfosMap[deviceType].push_back(hapVerifyInfo);
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckAtomicServicePreloadsIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID.toStringWithArgs(
            "hapVerifyInfos is empty!").c_str());
        return false;
    }
    std::list<std::string> moduleNames;
    GetHapVerifyInfosNames(hapVerifyInfos, moduleNames);
    // check preload module is existed and not self
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        std::list<std::string> preloadModuleNames;
        const std::list<PreloadItem>& preloadItems = hapVerifyInfo.GetPreloadItems();
        for (PreloadItem preloadItem : preloadItems) {
            if (Utils::CheckListContain(preloadModuleNames, preloadItem.moduleName)) {
                LOGE("%s", PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID.toStringWithArgs(
                    std::string("Preloads a duplicate module, module(") + hapVerifyInfo.GetModuleName() +
                    ") cannot preloads module (" + preloadItem.moduleName + ").").c_str());
                return false;
            }
            preloadModuleNames.push_back(preloadItem.moduleName);
            if (!Utils::CheckListContain(moduleNames, preloadItem.moduleName)) {
                LOGE("%s", PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID.toStringWithArgs(
                    std::string("Preloads a not exist module, module(") + hapVerifyInfo.GetModuleName() +
                    ") cannot preloads module(" + preloadItem.moduleName + ").").c_str());
                return false;
            }
            if (preloadItem.moduleName == hapVerifyInfo.GetModuleName()) {
                LOGE("%s", PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID.toStringWithArgs(
                    std::string("Cannot preload self, module ") + hapVerifyInfo.GetModuleName() +
                    " cannot preloads self.").c_str());
                return false;
            }
        }
    }
    // check feature preload is valid
    std::map<std::string, std::string> moduleNameWithType;
    for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
        moduleNameWithType.insert(std::make_pair(hapVerifyInfo.GetModuleName(), hapVerifyInfo.GetModuleType()));
    }
    for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
        const std::list<PreloadItem>& preloadItems = hapVerifyInfo.GetPreloadItems();
        for (auto& preloadItem : preloadItems) {
            if ((moduleNameWithType.count(preloadItem.moduleName) > 0) &&
                (moduleNameWithType[preloadItem.moduleName] == ENTRY ||
                moduleNameWithType[preloadItem.moduleName] == HAR)) {
                LOGE("%s", PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID.toStringWithArgs(
                    std::string("feature or shared cannot preload entry or har, module(") +
                    hapVerifyInfo.GetModuleName() + ") cannot preloads module(" +
                    preloadItem.moduleName + ").").c_str());
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckFileSizeIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("hapVerifyInfos is empty!").c_str());
        return false;
    }
    // check single file length
    int32_t entryLimit = hapVerifyInfos.begin()->GetEntrySizeLimit();
    int32_t notEntryLimit = hapVerifyInfos.begin()->GetNotEntrySizeLimit();
    for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetModuleType() == ENTRY &&
            entryLimit != 0 &&
            (hapVerifyInfo.GetFileLength() >= entryLimit * FILE_LENGTH_1KB)) {
            LOGE("%s", PackingToolErrMsg::CHECK_FILE_SIZE_INVALID.toStringWithArgs(
                ("module " + hapVerifyInfo.GetModuleName() + "'s size is overlarge").c_str()).c_str());
            return false;
        }
        if (hapVerifyInfo.GetModuleType() != ENTRY &&
            notEntryLimit != 0 &&
            (hapVerifyInfo.GetFileLength() >= notEntryLimit * FILE_LENGTH_1KB)) {
            LOGE("%s", PackingToolErrMsg::CHECK_FILE_SIZE_INVALID.toStringWithArgs(
                ("module " + hapVerifyInfo.GetModuleName() + "'s size is overlarge").c_str()).c_str());
            return false;
        }
    }

    std::map<std::string, std::list<HapVerifyInfo>> deviceInfosMap;
    if (!GetDeviceHapVerifyInfoMap(hapVerifyInfos, deviceInfosMap)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
            "HapVerifyUtils::CheckFileSizeIsValid GetDeviceHapVerifyInfoMap failed!").c_str());
        return false;
    }
    std::map<std::string, std::list<HapVerifyInfo>>::iterator iter;
    for (iter = deviceInfosMap.begin(); iter != deviceInfosMap.end(); iter++) {
        const std::string& deviceType = iter->first;
        const std::list<HapVerifyInfo>& deviceInfos = iter->second;
        if (!CheckAtomicServiceModuleSize(deviceInfos)) {
            LOGE("%s", PackingToolErrMsg::CHECK_ATOMIC_SERVICE_MODULE_SIZE.toStringWithArgs(
                ("checkAtomicServiceModuleSize failed on device " + deviceType).c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::CheckAtomicServiceModuleSize(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "hapVerifyInfos is empty!").c_str());
        return false;
    }
    int32_t entryLimit = hapVerifyInfos.begin()->GetEntrySizeLimit();
    int32_t notEntryLimit = hapVerifyInfos.begin()->GetNotEntrySizeLimit();
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        std::list<std::string> dependencies;
        GetModuleDependency(hapVerifyInfo, hapVerifyInfos, dependencies);
        std::list<HapVerifyInfo> dependenciesInfos;
        for (auto& module : dependencies) {
            const HapVerifyInfo *info = FindAtomicServiceHapVerifyInfo(module, hapVerifyInfos);
            if (info != nullptr) {
                dependenciesInfos.push_back(*info);
            }
        }
        long fileSize = hapVerifyInfo.GetFileLength();
        for (auto& dependency : dependenciesInfos) {
            fileSize += dependency.GetFileLength();
        }
        if (hapVerifyInfo.GetModuleType() == ENTRY &&
            entryLimit != 0 &&
            (fileSize >= entryLimit * FILE_LENGTH_1KB)) {
            LOGE("%s", PackingToolErrMsg::CHECK_FILE_SIZE_INVALID.toStringWithArgs(
                ("module " + hapVerifyInfo.GetModuleName() +
                " and it's dependencies size is overlarge").c_str()).c_str());
            return false;
        }
        if (hapVerifyInfo.GetModuleType() != ENTRY &&
            notEntryLimit != 0 &&
            (fileSize >= notEntryLimit * FILE_LENGTH_1KB)) {
            LOGE("%s", PackingToolErrMsg::CHECK_FILE_SIZE_INVALID.toStringWithArgs(
                ("module " + hapVerifyInfo.GetModuleName() +
                " and it's dependencies size is overlarge").c_str()).c_str());
            return false;
        }
    }
    return true;
}

void HapVerifyUtils::GetModuleDependency(const HapVerifyInfo& hapVerifyInfo,
    const std::list<HapVerifyInfo>& hapVerifyInfoList, std::list<std::string>& moduleDependency)
{
    moduleDependency.insert(moduleDependency.end(),
        hapVerifyInfo.GetDependencies().begin(), hapVerifyInfo.GetDependencies().end());
    const std::list<std::string>& dependencyItems = hapVerifyInfo.GetDependencies();
    for (auto& dependencyItem : dependencyItems) {
        const HapVerifyInfo *dependencyHapVerifyInfo =
            FindAtomicServiceHapVerifyInfo(dependencyItem, hapVerifyInfoList);
        if (dependencyHapVerifyInfo == nullptr) {
            continue;
        }
        std::list<std::string> childDependencies;
        GetModuleDependency(*dependencyHapVerifyInfo, hapVerifyInfoList, childDependencies);
        for (auto& childDependency : childDependencies) {
            if (std::find(moduleDependency.begin(), moduleDependency.end(), childDependency) ==
                moduleDependency.end()) {
                moduleDependency.push_back(childDependency);
            }
        }
    }
}

const HapVerifyInfo* HapVerifyUtils::FindAtomicServiceHapVerifyInfo(const std::string& moduleName,
    const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetModuleName() == moduleName) {
            return &hapVerifyInfo;
        }
    }
    return nullptr;
}

bool HapVerifyUtils::CheckAbilityNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        long noDuplicatedCount = Utils::GetListDistinctCount(hapVerifyInfo.GetAbilityNames());
        if (noDuplicatedCount != (long)hapVerifyInfo.GetAbilityNames().size()) {
            LOGW("%s ability duplicated, please rename ability name.", hapVerifyInfo.GetModuleName().c_str());
            return false;
        }
    }
    for (auto iter1 = hapVerifyInfos.begin(); iter1 != hapVerifyInfos.end(); iter1++) {
        const HapVerifyInfo& hapVerifyInfo1 = *iter1;
        if (hapVerifyInfo1.GetAbilityNames().empty()) {
            continue;
        }
        for (auto iter2 = std::next(iter1); iter2 != hapVerifyInfos.end(); iter2++) {
            const HapVerifyInfo& hapVerifyInfo2 = *iter2;
            if (!Utils::CheckDisjoint(hapVerifyInfo1.GetAbilityNames(), hapVerifyInfo2.GetAbilityNames()) &&
                !CheckDuplicatedIsValid(hapVerifyInfo1, hapVerifyInfo2)) {
                LOGW("Module: (%s) and Module: (%s) have the same ability name.",
                    hapVerifyInfo1.GetModuleName().c_str(),
                    hapVerifyInfo2.GetModuleName().c_str());
                LOGW("Module: %s has ability %s.",
                    hapVerifyInfo1.GetModuleName().c_str(),
                    Utils::ListToString(hapVerifyInfo1.GetAbilityNames()).c_str());
                LOGW("Module: %s has ability %s.",
                    hapVerifyInfo2.GetModuleName().c_str(),
                    Utils::ListToString(hapVerifyInfo2.GetAbilityNames()).c_str());
                LOGW("Solution: Make sure ability name is valid and unique.");
                LOGW("Reference: %s.", REFERENCE_LINK.c_str());
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckTargetModuleNameIsExisted(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    std::list<HapVerifyInfo> internalOverlayHaps;
    std::list<HapVerifyInfo> nonOverlayHaps;
    std::list<std::string> targetModules;
    std::list<std::string> modules;
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (!hapVerifyInfo.GetTargetBundleName().empty()) {
            return true;
        }
        if (!hapVerifyInfo.GetTargetModuleName().empty()) {
            internalOverlayHaps.push_back(hapVerifyInfo);
            targetModules.push_back(hapVerifyInfo.GetTargetModuleName());
            continue;
        }
        nonOverlayHaps.push_back(hapVerifyInfo);
        if (hapVerifyInfo.GetModuleType() != SHARED_LIBRARY) {
            modules.push_back(hapVerifyInfo.GetModuleName());
        }
    }
    if (internalOverlayHaps.empty()) {
        return true;
    }
    if (nonOverlayHaps.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "target modules are needed to pack with overlay module.").c_str());
        return false;
    }
    if (!Utils::CheckContainsAll(modules, targetModules)) {
        LOGE("%s", PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED.toStringWithArgs(
            "target modules are needed to pack with overlay module.").c_str());
        return false;
    }
    return true;
}

bool HapVerifyUtils::CheckCompileSdkIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "hapVerifyInfos is empty!").c_str());
        return false;
    }
    std::string compileSdkVersion = hapVerifyInfos.begin()->GetCompileSdkVersion();
    std::string compileSdkType = hapVerifyInfos.begin()->GetCompileSdkType();
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetCompileSdkType() != compileSdkType) {
            LOGE("%s", PackingToolErrMsg::COMPILE_SDK_TYPE_DIFFERENT.toStringWithArgs(
                std::vector<std::string>{
                    "CompileSdkType is not the same for all modules.",
                    "Ensure that all modules has same compileSdkType."
                }).c_str());
            return false;
        }
        if (hapVerifyInfo.GetCompileSdkVersion() != compileSdkVersion) {
            LOGE("%s", PackingToolErrMsg::COMPILE_SDK_TYPE_DIFFERENT.toStringWithArgs(
                "compile sdk version is not same.").c_str());
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::CheckProxyDataUriIsUnique(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "Hap verify infos is empty").c_str());
        return false;
    }
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> usedUrisByDeviceType;
    for (const auto& info : hapVerifyInfos) {
        if (!CheckAndInsertUris(info, usedUrisByDeviceType)) {
            return false;
        }
    }
    return true;
}

bool HapVerifyUtils::CheckAndInsertUris(const HapVerifyInfo& info,
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& usedUrisByDeviceType)
{
    const auto& uris = info.GetProxyDataUris();
    if (uris.empty()) {
        return true;
    }
    const std::string& moduleName = info.GetModuleName();
    const auto& deviceTypes = info.GetDeviceTypes();

    // Handle empty deviceTypes
    if (deviceTypes.empty()) {
        // Use NULL_DEVICE_TYPE as a special key for modules without deviceType
        auto& nullDeviceTypeUris = usedUrisByDeviceType[NULL_DEVICE_TYPE];
        for (const auto& uri : uris) {
            // Check if uri already exists in nullDeviceType
            auto existingIter = nullDeviceTypeUris.find(uri);
            if (existingIter != nullDeviceTypeUris.end()) {
                LOGE("%s", PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE.toStringWithArgs(
                    std::vector<std::string>{
                        "The uri(" + uri + ") is duplicated between module(" + existingIter->second +
                        ") and module(" + moduleName + ") for deviceType(" + NULL_DEVICE_TYPE + ").",
                        "Ensure that the uri in proxyData is unique across different modules."
                    }).c_str());
                return false;
            }

            // Check if uri exists in other deviceType collections
            // (because modules without deviceType apply to all devices)
            if (CheckUriExistsInOtherDeviceTypes(usedUrisByDeviceType, uri, moduleName)) {
                return false;
            }

            nullDeviceTypeUris[uri] = moduleName;
        }
        return true;
    }

    // Normal case: deviceTypes is not empty
    for (const auto& deviceType : deviceTypes) {
        auto& uriToModuleMap = usedUrisByDeviceType[deviceType];
        for (const auto& uri : uris) {
            if (CheckUriInNullDeviceType(usedUrisByDeviceType, uri, moduleName)) {
                return false;
            }
            if (CheckUriInCurrentDeviceType(uriToModuleMap, uri, moduleName, deviceType)) {
                return false;
            }
            uriToModuleMap[uri] = moduleName;
        }
    }
    return true;
}

bool HapVerifyUtils::CheckUriInNullDeviceType(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& usedUrisByDeviceType,
    const std::string& uri,
    const std::string& moduleName)
{
    auto nullDeviceTypeIter = usedUrisByDeviceType.find(NULL_DEVICE_TYPE);
    if (nullDeviceTypeIter != usedUrisByDeviceType.end()) {
        const auto& nullDeviceTypeUris = nullDeviceTypeIter->second;
        auto existingIter = nullDeviceTypeUris.find(uri);
        if (existingIter != nullDeviceTypeUris.end()) {
            LOGE("%s", PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE.toStringWithArgs(
                std::vector<std::string>{
                    "The uri(" + uri + ") is duplicated between module(" + existingIter->second +
                    ") and module(" + moduleName + ") for deviceType(" + NULL_DEVICE_TYPE + ").",
                    "Ensure that the uri in proxyData is unique across different modules."
                }).c_str());
            return true;
        }
    }
    return false;
}

bool HapVerifyUtils::CheckUriInCurrentDeviceType(
    const std::unordered_map<std::string, std::string>& uriToModuleMap,
    const std::string& uri,
    const std::string& moduleName,
    const std::string& deviceType)
{
    auto existingIter = uriToModuleMap.find(uri);
    if (existingIter != uriToModuleMap.end()) {
        LOGE("%s", PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE.toStringWithArgs(
            std::vector<std::string>{
                "The uri(" + uri + ") is duplicated between module(" + existingIter->second +
                ") and module(" + moduleName + ") for deviceType(" + deviceType + ").",
                "Ensure that the uri in proxyData is unique across different modules "
                "when deviceType has intersection."
            }).c_str());
        return true;
    }
    return false;
}

bool HapVerifyUtils::CheckUriExistsInOtherDeviceTypes(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& usedUrisByDeviceType,
    const std::string& uri,
    const std::string& moduleName)
{
    for (const auto& mapEntry : usedUrisByDeviceType) {
        if (mapEntry.first != NULL_DEVICE_TYPE) {
            const auto& uriToModuleMap = mapEntry.second;
            auto existingIter = uriToModuleMap.find(uri);
            if (existingIter != uriToModuleMap.end()) {
                LOGE("%s", PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE.toStringWithArgs(
                    std::vector<std::string>{
                        "The uri(" + uri + ") is duplicated between module(" + existingIter->second +
                        ") and module(" + moduleName + ") for deviceType(" + mapEntry.first + ").",
                        "Ensure that the uri in proxyData is unique across different modules."
                    }).c_str());
                return true;
            }
        }
    }
    return false;
}

bool HapVerifyUtils::CheckContinueTypeIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (!CheckContinueTypeIsValid(hapVerifyInfo)) {
            LOGE("%s", PackingToolErrMsg::CONTINUE_TYPE_INVALID.toStringWithArgs(
                "CheckContinueTypeIsValid failed!").c_str());
            return false;
        }
    }
    for (auto iter1 = hapVerifyInfos.begin(); iter1 != hapVerifyInfos.end(); iter1++) {
        for (auto iter2 = std::next(iter1); iter2 != hapVerifyInfos.end(); iter2++) {
            if (!CheckContinueTypeIsValid(*iter1, *iter2)) {
                LOGE("%s", PackingToolErrMsg::CONTINUE_TYPE_INVALID.toStringWithArgs(
                    "CheckContinueTypeIsValid failed!").c_str());
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckContinueTypeIsValid(const HapVerifyInfo& hapVerifyInfo)
{
    const std::list<std::string>& abilityNames = hapVerifyInfo.GetAbilityNames();
    if (abilityNames.size() < TWO) {
        return true;
    }
    for (auto iter1 = abilityNames.begin(); iter1 != abilityNames.end(); iter1++) {
        std::list<std::string> typeList1;
        if (hapVerifyInfo.GetContinueTypeMap().find(*iter1) != hapVerifyInfo.GetContinueTypeMap().end()) {
            typeList1 = hapVerifyInfo.GetContinueTypeMap().at(*iter1);
        }
        if (typeList1.empty()) {
            continue;
        }
        for (auto iter2 = std::next(iter1); iter2 != abilityNames.end(); iter2++) {
            std::list<std::string> typeList2;
            if (hapVerifyInfo.GetContinueTypeMap().find(*iter2) != hapVerifyInfo.GetContinueTypeMap().end()) {
                typeList2 = hapVerifyInfo.GetContinueTypeMap().at(*iter2);
            }
            if (typeList2.empty()) {
                continue;
            }
            if (!Utils::CheckDisjoint(typeList1, typeList2)) {
                std::string cause = "Module(" + hapVerifyInfo.GetModuleName() + "), Ability(" +
                    *iter1 + ") and Ability(" + *iter2 + ") have same continueType.\n";
                cause += "Ability(" + *iter1 + ") have continueType: " + FormatModuleNameList(typeList1) + ", ";
                cause += "Another Ability(" + *iter2 + ") have continueType: " + FormatModuleNameList(typeList2) + ".";
                LOGE("%s", PackingToolErrMsg::CONTINUE_TYPE_INVALID.toStringWithArgs(
                    std::vector<std::string>{
                        cause,
                        "Please ensure that the continueType for different abilities does not overlap."
                    }).c_str());
                return false;
            }
        }
    }
    return true;
}

bool HapVerifyUtils::CheckContinueTypeIsValid(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2)
{
    if (Utils::CheckDisjoint(hapVerifyInfo1.GetDeviceTypes(), hapVerifyInfo2.GetDeviceTypes())) {
        return true;
    }
    const std::map<std::string, std::list<std::string>>& typeMap1 = hapVerifyInfo1.GetContinueTypeMap();
    const std::map<std::string, std::list<std::string>>& typeMap2 = hapVerifyInfo2.GetContinueTypeMap();
    std::list<std::string> typeList1;
    std::list<std::string> typeList2;
    for (auto iter = typeMap1.begin(); iter != typeMap1.end(); iter++) {
        typeList1.insert(typeList1.end(), iter->second.begin(), iter->second.end());
    }
    for (auto iter = typeMap2.begin(); iter != typeMap2.end(); iter++) {
        typeList2.insert(typeList2.end(), iter->second.begin(), iter->second.end());
    }
    if (!Utils::CheckDisjoint(typeList1, typeList2)) {
        std::string cause = "Conflict detected between modules due to overlapping deviceType and continueType:\n";
        cause += "- Module(" + hapVerifyInfo1.GetModuleName() + ") with deviceType: " +
            FormatModuleNameList(hapVerifyInfo1.GetDeviceTypes()) +
            " and continueType: " + FormatModuleNameList(typeList1) + "\n";
        cause += "- Module(" + hapVerifyInfo2.GetModuleName() + ") with deviceType: " +
            FormatModuleNameList(hapVerifyInfo2.GetDeviceTypes()) +
            " and continueType: " + FormatModuleNameList(typeList2);
        LOGE("%s", PackingToolErrMsg::CONTINUE_TYPE_INVALID.toStringWithArgs(
            std::vector<std::string>{
                cause,
                "Ensure that the continueType fields in these modules are different. Update either (" +
                hapVerifyInfo1.GetModuleName() + ") or (" + hapVerifyInfo2.GetModuleName() +
                ") to avoid continueType or deviceType overlap."
            }).c_str());
        return false;
    }
    return true;
}

// java : Compressor::checkSharedAppIsValid / HapVerify::checkSharedApppIsValid
bool HapVerifyUtils::CheckSharedAppIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toStringWithArgs(
            "hapVerifyInfos is empty!").c_str());
        return false;
    }
    std::string moduleName = hapVerifyInfos.begin()->GetModuleName();
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetModuleName() != moduleName) {
            LOGE("%s", PackingToolErrMsg::CHECK_MODULE_NAME_INVALID.toStringWithArgs(
                "moduleName is not same").c_str());
            return false;
        }
        if (!hapVerifyInfo.GetDependencyItemList().empty()) {
            LOGE("%s", PackingToolErrMsg::CHECK_DEPENDENCY_INVALID.toStringWithArgs(
                "dependencyItems is empty").c_str());
            return false;
        }
        if (hapVerifyInfo.GetModuleType() != TYPE_SHARED) {
            LOGE("%s", PackingToolErrMsg::CHECK_SHARED_APP_INVALID.toStringWithArgs(
                std::string("moduleType is not ") + TYPE_SHARED).c_str());
            return false;
        }
    }
    for (auto iter1 = hapVerifyInfos.begin(); iter1 != hapVerifyInfos.end(); iter1++) {
        for (auto iter2 = std::next(iter1); iter2 != hapVerifyInfos.end(); iter2++) {
            if (!CheckDuplicatedIsValid(*iter1, *iter2)) {
                LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs(
                    "CheckDuplicatedIsValid failed!").c_str());
                return false;
            }
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
