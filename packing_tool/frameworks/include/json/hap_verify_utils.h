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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_UTILS_H

#include <list>
#include <map>
#include <string>

#include "distro_filter.h"
#include "hap_verify_info.h"
#include "multi_app_mode.h"

namespace OHOS {
namespace AppPackingTool {
struct VerifyCollection {
    std::string bundleName = "";
    std::string vendor = "";
    int32_t versionCode = -1;
    std::string versionName = "";
    int32_t minCompatibleVersionCode = -1;
    int32_t compatibleApiVersion = -1;
    int32_t targetApiVersion = -1;
    std::string releaseType = "";
    std::string targetBundleName = "";
    int32_t targetPriority = 0;
    bool debug = false;
    std::list<std::string> moduleNames;
    std::list<std::string> packageNames;

    bool split = true;
    std::string main = "";
    bool sharedApp = false;
    std::string bundleType = "app";
    std::string moduleName = "";
    std::string moduleType = "";
    MultiAppMode multiAppMode;
};

enum PolicyValueType {
    API_VERSION = 0,
    SCREEN_SHAPE,
    SCREEN_DENSITY,
    SCREEN_WINDOW,
    COUNTRY_CODE
};

class HapVerifyUtils {
public:
    HapVerifyUtils() {};
    virtual ~HapVerifyUtils() {};

    HapVerifyUtils(const HapVerifyUtils &) = delete;
    HapVerifyUtils &operator=(const HapVerifyUtils &) = delete;

    static bool CheckHapIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckSharedAppIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);

private:
    static bool CheckAppFieldsIsSame(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool AppFieldsIsSame(const VerifyCollection& verifyCollection, const HapVerifyInfo& hapVerifyInfo);
    static bool IsEntryOrFeature(const std::string& moduleType);
    static bool CheckModuleNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckDuplicatedIsValid(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2);
    static bool CheckDistroFilterDisjoint(const DistroFilter& distroFilter1, const DistroFilter& distroFilter2);
    static bool CheckPolicyValueDisjoint(const PolicyValue& policyValue1, const PolicyValue& policyValue2);
    static bool CheckPackageNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static void ShowCheckTips(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2,
        const std::string& tip);
    static bool CheckEntryIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static void ClassifyEntry(const std::list<HapVerifyInfo>& entryHapVerifyInfos,
        std::map<std::string, std::list<HapVerifyInfo>>& deviceHaps);
    static bool CheckFeature(const HapVerifyInfo& featureHap,
        const std::map<std::string, std::list<HapVerifyInfo>>& deviceHap);
    static bool CheckFeatureDistroFilter(const HapVerifyInfo& featureHap,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckApiVersionCovered(const ApiVersion& apiVersion,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckScreenShapeCovered(const ScreenShape& screenShape,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckScreenWindowCovered(const ScreenWindow& screenWindow,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckScreenDensityCovered(const ScreenDensity& screenDensity,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckCountryCodeCovered(const CountryCode& countryCode,
        const std::list<HapVerifyInfo>& entryHaps);
    static bool CheckEntryPolicyValueCoverAll(const std::list<std::string>& include,
        const std::list<std::string>& exclude);
    static bool CheckPolicyValueCovered(const PolicyValue& policyValue,
        const std::list<std::string>& include, std::list<std::string>& exclude);
    static bool CheckCoveredExcludePolicyValue(const std::list<std::string>& value,
        const std::list<std::string>& include, std::list<std::string>& exclude);
    static bool CheckCoveredIncludePolicyValue(const std::list<std::string>& value,
        const std::list<std::string>& include, std::list<std::string>& exclude);
    static bool CheckDependencyIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool DfsTraverseDependency(const HapVerifyInfo& hapVerifyInfo,
        const std::list<HapVerifyInfo>& hapVerifyInfos, std::list<HapVerifyInfo>& dependencyList);
    static bool CheckDependencyListCirculate(const std::list<HapVerifyInfo>& dependencyList);
    static bool IsSameHapVerifyInfo(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2);
    static bool CheckModuleJoint(const HapVerifyInfo& infoLeft, const HapVerifyInfo& infoRight);
    static void GetHapVerifyInfosNames(const std::list<HapVerifyInfo>& hapVerifyInfos,
        std::list<std::string>& moduleNames);
    static bool CheckDependencyInFileList(const DependencyItem& dependencyItem,
        const std::list<HapVerifyInfo>& hapVerifyInfos);
    static void GetLayerHapVerifyInfos(const std::string& moduleName, const HapVerifyInfo& hapVerifyInfo,
        const std::list<HapVerifyInfo>& hapVerifyInfos, std::list<HapVerifyInfo>& layerHapVerifyInfos);
    static bool CheckAtomicServiceIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool GetDeviceHapVerifyInfoMap(const std::list<HapVerifyInfo>& hapVerifyInfos,
        std::map<std::string, std::list<HapVerifyInfo>>& deviceInfoMap);
    static bool CheckAtomicServicePreloadsIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckFileSizeIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckAtomicServiceModuleSize(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static void GetModuleDependency(const HapVerifyInfo& hapVerifyInfo,
        const std::list<HapVerifyInfo>& hapVerifyInfoList, std::list<std::string>& moduleDependency);
    static const HapVerifyInfo* FindAtomicServiceHapVerifyInfo(const std::string& moduleName,
        const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckAbilityNameIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckTargetModuleNameIsExisted(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckCompileSdkIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckProxyDataUriIsUnique(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckContinueTypeIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool CheckContinueTypeIsValid(const HapVerifyInfo& hapVerifyInfo);
    static bool CheckContinueTypeIsValid(const HapVerifyInfo& hapVerifyInfo1, const HapVerifyInfo& hapVerifyInfo2);
    static bool AppFieldsIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos,
        int32_t minCompatibleVersionCode, int32_t targetApiVersion);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_UTILS_H
