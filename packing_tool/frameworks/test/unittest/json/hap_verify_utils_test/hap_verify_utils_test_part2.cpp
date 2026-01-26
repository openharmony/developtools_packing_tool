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

#include <gtest/gtest.h>
#include <cstdlib>

#define private public
#define protected public
#include "hap_verify_utils.h"
#include "hap_verify_info.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

class HapVerifyUtilsTestPart2 : public testing::Test {
public:
    HapVerifyUtilsTestPart2() {}
    virtual ~HapVerifyUtilsTestPart2() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HapVerifyUtilsTestPart2::SetUpTestCase() {}

void HapVerifyUtilsTestPart2::TearDownTestCase() {}

void HapVerifyUtilsTestPart2::SetUp() {}

void HapVerifyUtilsTestPart2::TearDown() {}

namespace {
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo1;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo2;
    const int64_t FILE_LENGTH_1M = 1024 * 1024;
}

void initDistroFilter(
    OHOS::AppPackingTool::DistroFilter& distroFilter1, OHOS::AppPackingTool::DistroFilter& distroFilter2)
{
    OHOS::AppPackingTool::ApiVersion apiVersion1;
    apiVersion1.policy = "";
    OHOS::AppPackingTool::ScreenShape screenShape1;
    screenShape1.policy = "";
    OHOS::AppPackingTool::ScreenDensity screenDensity1;
    screenDensity1.policy = "";
    OHOS::AppPackingTool::ScreenWindow screenWindow1;
    screenWindow1.policy = "";
    OHOS::AppPackingTool::CountryCode countryCode1;
    countryCode1.policy = "include";
    distroFilter1.apiVersion = apiVersion1;
    distroFilter1.screenShape = screenShape1;
    distroFilter1.screenDensity = screenDensity1;
    distroFilter1.screenWindow = screenWindow1;
    distroFilter1.countryCode = countryCode1;

    OHOS::AppPackingTool::ApiVersion apiVersion2;
    apiVersion2.policy = "exclude";
    apiVersion2.value = {"c", "d"};
    OHOS::AppPackingTool::ScreenShape screenShape2;
    screenShape2.policy = "exclude";
    screenShape2.value = {"c", "d"};
    OHOS::AppPackingTool::ScreenDensity screenDensity2;
    screenDensity2.policy = "exclude";
    screenDensity2.value = {"c", "d"};
    OHOS::AppPackingTool::ScreenWindow screenWindow2;
    screenWindow2.policy = "exclude";
    screenWindow2.value = {"c", "d"};
    OHOS::AppPackingTool::CountryCode countryCode2;
    countryCode2.policy = "";
    distroFilter2.apiVersion = apiVersion2;
    distroFilter2.screenShape = screenShape2;
    distroFilter2.screenDensity = screenDensity2;
    distroFilter2.screenWindow = screenWindow2;
    distroFilter2.countryCode = countryCode2;
}

/*
 * @tc.name: GetDependencies_0100
 * @tc.desc: GetDependencies
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetDependencies_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    std::list<std::string> dependencies;
    dependencies.push_front("test");
    info.SetDependencies(dependencies);

    std::list<std::string> ret = info.GetDependencies();
    EXPECT_EQ(ret.size(), dependencies.size());
}

/*
 * @tc.name: GetProfileStr_0100
 * @tc.desc: GetProfileStr
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetProfileStr_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    std::string ret = info.GetProfileStr();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: GetAtomicServiceType_0100
 * @tc.desc: GetAtomicServiceType
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetAtomicServiceType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    std::string ret = info.GetAtomicServiceType();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: SetAtomicServiceType_0100
 * @tc.desc: SetAtomicServiceType
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, SetAtomicServiceType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    std::string atomicServiceType = "test";
    info.SetAtomicServiceType(atomicServiceType);

    std::string ret = info.GetAtomicServiceType();
    EXPECT_EQ(ret, atomicServiceType);
}

/*
 * @tc.name: GetPreloadItems_0100
 * @tc.desc: GetPreloadItems
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetPreloadItems_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    std::list<OHOS::AppPackingTool::PreloadItem> ret = info.GetPreloadItems();
    EXPECT_EQ(ret.size(), 0);
}

/*
 * @tc.name: GetTargetModulePriority_0100
 * @tc.desc: GetTargetModulePriority
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetTargetModulePriority_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    int32_t ret = info.GetTargetModulePriority();
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: GetFileLength_0100
 * @tc.desc: GetFileLength
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetFileLength_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    int64_t ret = info.GetFileLength();
    EXPECT_EQ(ret, 0L);
}

/*
 * @tc.name: GetEntrySizeLimit_0100
 * @tc.desc: GetEntrySizeLimit
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetEntrySizeLimit_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    int32_t limit = 100;
    info.SetEntrySizeLimit(limit);

    int32_t ret = info.GetEntrySizeLimit();
    EXPECT_EQ(ret, limit);
}

/*
 * @tc.name: GetNotEntrySizeLimit_0100
 * @tc.desc: GetNotEntrySizeLimit
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetNotEntrySizeLimit_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    int32_t notEntrySizeLimit = 100;
    info.SetNotEntrySizeLimit(notEntrySizeLimit);

    int32_t ret = info.GetNotEntrySizeLimit();
    EXPECT_EQ(ret, notEntrySizeLimit);
}

/*
 * @tc.name: GetSumSizeLimit_0100
 * @tc.desc: GetSumSizeLimit
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetSumSizeLimit_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo info;

    int32_t sumSizeLimit = 100;
    info.SetSumSizeLimit(sumSizeLimit);

    int32_t ret = info.GetSumSizeLimit();
    EXPECT_EQ(ret, sumSizeLimit);
}

/*
 * @tc.name: CheckPolicyValueCovered_0100
 * @tc.desc: CheckPolicyValueCovered
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckPolicyValueCovered_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    OHOS::AppPackingTool::PolicyValue policyValue;
    policyValue.policy = "exclude";
    std::list<std::string> include;
    std::list<std::string> exclude;
    EXPECT_TRUE(hapVerifyUtils.CheckPolicyValueCovered(policyValue, include, exclude));
}

/*
 * @tc.name: CheckPolicyValueCovered_0200
 * @tc.desc: CheckPolicyValueCovered
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckPolicyValueCovered_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    OHOS::AppPackingTool::PolicyValue policyValue;
    policyValue.policy = "include";
    std::list<std::string> include;
    std::list<std::string> exclude;
    EXPECT_TRUE(hapVerifyUtils.CheckPolicyValueCovered(policyValue, include, exclude));
}

/*
 * @tc.name: CheckCoveredExcludePolicyValue_0100
 * @tc.desc: CheckCoveredExcludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredExcludePolicyValue_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    std::list<std::string> exclude;
    EXPECT_TRUE(hapVerifyUtils.CheckCoveredExcludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: CheckCoveredExcludePolicyValue_0200
 * @tc.desc: CheckCoveredExcludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredExcludePolicyValue_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    include.push_back("test");
    std::list<std::string> exclude;
    EXPECT_FALSE(hapVerifyUtils.CheckCoveredExcludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: CheckCoveredExcludePolicyValue_0300
 * @tc.desc: CheckCoveredExcludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredExcludePolicyValue_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    include.push_back("test");
    std::list<std::string> exclude;
    exclude.push_back("test");
    EXPECT_TRUE(hapVerifyUtils.CheckCoveredExcludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: CheckCoveredIncludePolicyValue_0100
 * @tc.desc: CheckCoveredIncludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredIncludePolicyValue_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    std::list<std::string> exclude;
    EXPECT_TRUE(hapVerifyUtils.CheckCoveredIncludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: CheckCoveredIncludePolicyValue_0200
 * @tc.desc: CheckCoveredIncludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredIncludePolicyValue_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    include.push_back("test");
    std::list<std::string> exclude;
    EXPECT_TRUE(hapVerifyUtils.CheckCoveredIncludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: CheckCoveredIncludePolicyValue_0300
 * @tc.desc: CheckCoveredIncludePolicyValue
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCoveredIncludePolicyValue_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<std::string> value;
    std::list<std::string> include;
    include.push_back("test");
    std::list<std::string> exclude;
    exclude.push_back("test");
    EXPECT_TRUE(hapVerifyUtils.CheckCoveredIncludePolicyValue(value, include, exclude));
}

/*
 * @tc.name: IsSameHapVerifyInfo_0100
 * @tc.desc: IsSameHapVerifyInfo
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, IsSameHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    hapVerifyInfo1.SetModuleName("test1");
    hapVerifyInfo2.SetModuleName("test2");
    EXPECT_FALSE(hapVerifyUtils.IsSameHapVerifyInfo(hapVerifyInfo1, hapVerifyInfo2));
}

/*
 * @tc.name: GetDeviceHapVerifyInfoMap_0100
 * @tc.desc: GetDeviceHapVerifyInfoMap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetDeviceHapVerifyInfoMap_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    std::map<std::string, std::list<OHOS::AppPackingTool::HapVerifyInfo>> deviceInfosMap;
    EXPECT_FALSE(hapVerifyUtils.GetDeviceHapVerifyInfoMap(hapVerifyInfos, deviceInfosMap));
}

/*
 * @tc.name: GetDeviceHapVerifyInfoMap_0200
 * @tc.desc: GetDeviceHapVerifyInfoMap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, GetDeviceHapVerifyInfoMap_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    std::list<std::string> deviceTypes = { "phone" };
    hapVerifyInfo3.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    std::map<std::string, std::list<OHOS::AppPackingTool::HapVerifyInfo>> deviceInfosMap;
    deviceInfosMap.insert(std::make_pair("phone", hapVerifyInfos));
    EXPECT_TRUE(hapVerifyUtils.GetDeviceHapVerifyInfoMap(hapVerifyInfos, deviceInfosMap));
}

/*
 * @tc.name: CheckAtomicServicePreloadsIsValid_0100
 * @tc.desc: CheckAtomicServicePreloadsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServicePreloadsIsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServicePreloadsIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServicePreloadsIsValid_0200
 * @tc.desc: CheckAtomicServicePreloadsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServicePreloadsIsValid_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("module3");
    std::list<OHOS::AppPackingTool::PreloadItem> preloadItems;
    OHOS::AppPackingTool::PreloadItem preloadItem;
    preloadItem.moduleName = "module3";
    preloadItems.push_back(preloadItem);
    hapVerifyInfo3.SetPreloadItems(preloadItems);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServicePreloadsIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServicePreloadsIsValid_0300
 * @tc.desc: CheckAtomicServicePreloadsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServicePreloadsIsValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("module3");
    std::list<OHOS::AppPackingTool::PreloadItem> preloadItems;
    OHOS::AppPackingTool::PreloadItem preloadItem1;
    preloadItem1.moduleName = "module4";
    preloadItems.push_back(preloadItem1);
    OHOS::AppPackingTool::PreloadItem preloadItem2;
    preloadItem2.moduleName = "module4";
    preloadItems.push_back(preloadItem2);
    hapVerifyInfo3.SetPreloadItems(preloadItems);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleName("module4");
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServicePreloadsIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServicePreloadsIsValid_0400
 * @tc.desc: CheckAtomicServicePreloadsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServicePreloadsIsValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("module3");
    std::list<OHOS::AppPackingTool::PreloadItem> preloadItems;
    OHOS::AppPackingTool::PreloadItem preloadItem1;
    preloadItem1.moduleName = "module4";
    preloadItems.push_back(preloadItem1);
    hapVerifyInfo3.SetPreloadItems(preloadItems);
    hapVerifyInfo3.SetModuleType("entry3");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleName("module4");
    hapVerifyInfo4.SetModuleType("entry");
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServicePreloadsIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServicePreloadsIsValid_0500
 * @tc.desc: CheckAtomicServicePreloadsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServicePreloadsIsValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("module3");
    std::list<OHOS::AppPackingTool::PreloadItem> preloadItems;
    OHOS::AppPackingTool::PreloadItem preloadItem1;
    preloadItem1.moduleName = "module4";
    preloadItems.push_back(preloadItem1);
    hapVerifyInfo3.SetPreloadItems(preloadItems);
    hapVerifyInfo3.SetModuleType("entry3");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleName("module4");
    hapVerifyInfo4.SetModuleType("entry4");
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_TRUE(hapVerifyUtils.CheckAtomicServicePreloadsIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckFileSizeIsValid_0100
 * @tc.desc: CheckFileSizeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckFileSizeIsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    EXPECT_FALSE(hapVerifyUtils.CheckFileSizeIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckFileSizeIsValid_0200
 * @tc.desc: CheckFileSizeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckFileSizeIsValid_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetEntrySizeLimit(1);
    hapVerifyInfo3.SetNotEntrySizeLimit(1);
    hapVerifyInfo3.SetModuleType("moduleType");
    hapVerifyInfo3.SetFileLength(FILE_LENGTH_1M);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckFileSizeIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckFileSizeIsValid_0300
 * @tc.desc: CheckFileSizeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckFileSizeIsValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetEntrySizeLimit(100);
    hapVerifyInfo3.SetNotEntrySizeLimit(100);
    hapVerifyInfo3.SetModuleType("moduleType");
    hapVerifyInfo3.SetFileLength(1024);
    std::list<std::string> deviceTypes = { "phone" };
    hapVerifyInfo3.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_TRUE(hapVerifyUtils.CheckFileSizeIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServiceModuleSize_0100
 * @tc.desc: CheckAtomicServiceModuleSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServiceModuleSize_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServiceModuleSize(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServiceModuleSize_0200
 * @tc.desc: CheckAtomicServiceModuleSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServiceModuleSize_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetEntrySizeLimit(1);
    hapVerifyInfo3.SetNotEntrySizeLimit(1);
    hapVerifyInfo3.SetModuleType("entry");
    hapVerifyInfo3.SetFileLength(FILE_LENGTH_1M);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServiceModuleSize(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServiceModuleSize_0300
 * @tc.desc: CheckAtomicServiceModuleSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServiceModuleSize_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetEntrySizeLimit(1);
    hapVerifyInfo3.SetNotEntrySizeLimit(1);
    hapVerifyInfo3.SetModuleType("moduleType");
    hapVerifyInfo3.SetFileLength(FILE_LENGTH_1M);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckAtomicServiceModuleSize(hapVerifyInfos));
}

/*
 * @tc.name: CheckAtomicServiceModuleSize_0400
 * @tc.desc: CheckAtomicServiceModuleSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckAtomicServiceModuleSize_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetEntrySizeLimit(100);
    hapVerifyInfo3.SetNotEntrySizeLimit(100);
    hapVerifyInfo3.SetModuleType("moduleType");
    hapVerifyInfo3.SetFileLength(100);
    std::list<std::string> dependencies;
    dependencies.push_back("test4");
    hapVerifyInfo3.SetDependencies(dependencies);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetBundleName("test4");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_TRUE(hapVerifyUtils.CheckAtomicServiceModuleSize(hapVerifyInfos));
}

/*
 * @tc.name: FindAtomicServiceHapVerifyInfo_0100
 * @tc.desc: FindAtomicServiceHapVerifyInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, FindAtomicServiceHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::string moduleName;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    ASSERT_EQ(hapVerifyUtils.FindAtomicServiceHapVerifyInfo(moduleName, hapVerifyInfos), nullptr);
}

/*
 * @tc.name: FindAtomicServiceHapVerifyInfo_0200
 * @tc.desc: FindAtomicServiceHapVerifyInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, FindAtomicServiceHapVerifyInfo_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::string moduleName = "test";
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("test");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    ASSERT_NE(hapVerifyUtils.FindAtomicServiceHapVerifyInfo(moduleName, hapVerifyInfos), nullptr);
}

/*
 * @tc.name: CheckTargetModuleNameIsExisted_0200
 * @tc.desc: CheckTargetModuleNameIsExisted
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckTargetModuleNameIsExisted_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetTargetModuleName("moduleName");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleType("moduleType");
    hapVerifyInfo4.SetModuleName("moduleName");
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_TRUE(hapVerifyUtils.CheckTargetModuleNameIsExisted(hapVerifyInfos));
}

/*
 * @tc.name: CheckContinueTypeIsValid_0300
 * @tc.desc: CheckContinueTypeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckContinueTypeIsValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    std::list<std::string> abilityNames = { "abilityName1", "abilityName2" };
    hapVerifyInfo3.SetAbilityNames(abilityNames);
    std::map<std::string, std::list<std::string>> continueTypeMap;
    std::string type1 = "test1";
    std::list<std::string> strList = { "test" };
    continueTypeMap.insert(std::make_pair(type1, strList));
    hapVerifyInfo3.SetContinueTypeMap(continueTypeMap);
    EXPECT_TRUE(hapVerifyUtils.CheckContinueTypeIsValid(hapVerifyInfo3));
}

/*
 * @tc.name: CheckContinueTypeIsValid_0400
 * @tc.desc: CheckContinueTypeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckContinueTypeIsValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    std::list<std::string> abilityNames = { "abilityName1", "abilityName2" };
    hapVerifyInfo3.SetAbilityNames(abilityNames);
    std::map<std::string, std::list<std::string>> continueTypeMap;
    std::string type1 = "abilityName1";
    std::list<std::string> strList = { "test" };
    continueTypeMap.insert(std::make_pair(type1, strList));
    hapVerifyInfo3.SetContinueTypeMap(continueTypeMap);
    EXPECT_TRUE(hapVerifyUtils.CheckContinueTypeIsValid(hapVerifyInfo3));
}

/*
 * @tc.name: CheckContinueTypeIsValid_0500
 * @tc.desc: CheckContinueTypeIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckContinueTypeIsValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    std::list<std::string> abilityNames = { "abilityName1", "abilityName2" };
    hapVerifyInfo3.SetAbilityNames(abilityNames);
    std::map<std::string, std::list<std::string>> continueTypeMap;
    std::string type1 = "abilityName1";
    std::list<std::string> strList1 = { "test1" };
    continueTypeMap.insert(std::make_pair(type1, strList1));
    std::string type2 = "abilityName2";
    std::list<std::string> strList2 = { "test2" };
    continueTypeMap.insert(std::make_pair(type2, strList2));
    hapVerifyInfo3.SetContinueTypeMap(continueTypeMap);
    EXPECT_TRUE(hapVerifyUtils.CheckContinueTypeIsValid(hapVerifyInfo3));
}

/*
 * @tc.name: CheckSharedAppIsValid_0100
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckSharedAppIsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    EXPECT_FALSE(hapVerifyUtils.CheckSharedAppIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckSharedAppIsValid_0200
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckSharedAppIsValid_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("moduleName3");
    OHOS::AppPackingTool::DependencyItem dependencyItem;
    dependencyItem.bundleName = "test";
    dependencyItem.moduleName = "test";
    std::list<OHOS::AppPackingTool::DependencyItem> dependencyItemList;
    dependencyItemList.push_back(dependencyItem);
    hapVerifyInfo3.SetDependencyItemList(dependencyItemList);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckSharedAppIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckSharedAppIsValid_0300
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckSharedAppIsValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("moduleName3");
    hapVerifyInfo3.SetModuleType("test");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(hapVerifyUtils.CheckSharedAppIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckSharedAppIsValid_0400
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckSharedAppIsValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("moduleName3");
    hapVerifyInfo3.SetModuleType("shared");
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleName("moduleName4");
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_FALSE(hapVerifyUtils.CheckSharedAppIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckSharedAppIsValid_0500
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckSharedAppIsValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("moduleName3");
    hapVerifyInfo3.SetModuleType("shared");
    std::list<std::string> deviceTypes;
    deviceTypes.push_back("test");
    hapVerifyInfo3.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetModuleName("moduleName3");
    hapVerifyInfo4.SetModuleType("shared");
    hapVerifyInfo4.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_FALSE(hapVerifyUtils.CheckSharedAppIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0300
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0400
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");
    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0500
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetPackageName("packageName");
    std::list<std::string> deviceType;
    deviceType.push_back("deviceType");
    hapVerifyInfo.SetDeviceTypes(deviceType);
    OHOS::AppPackingTool::CountryCode countryCode;
    countryCode.policy = "exclude";
    OHOS::AppPackingTool::DistroFilter distroFilter;
    distroFilter.countryCode = countryCode;
    hapVerifyInfo.SetDistroFilter(distroFilter);

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleNameDiff");
    hapVerifyInfoDiff.SetPackageName("packageName");
    hapVerifyInfoDiff.SetDeviceTypes(deviceType);
    hapVerifyInfoDiff.SetDistroFilter(distroFilter);

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0600
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetModuleType("entry");
    std::list<std::string> deviceType;
    deviceType.push_back("deviceType");
    hapVerifyInfo.SetDeviceTypes(deviceType);
    OHOS::AppPackingTool::CountryCode countryCode;
    countryCode.policy = "exclude";
    OHOS::AppPackingTool::DistroFilter distroFilter;
    distroFilter.countryCode = countryCode;
    hapVerifyInfo.SetDistroFilter(distroFilter);

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleNameDiff");
    hapVerifyInfoDiff.SetModuleType("entry");
    hapVerifyInfoDiff.SetDeviceTypes(deviceType);
    hapVerifyInfoDiff.SetDistroFilter(distroFilter);

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0700
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetInstallationFree(true);

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleNameDiff");
    hapVerifyInfoDiff.SetInstallationFree(false);

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0800
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckHapIsValid_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetBundleType("appPlugin");

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleNameDiff");

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckIsPluginApp_0100
 * @tc.desc: CheckIsPluginApp
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckIsPluginApp_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetBundleType("appPlugin");

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleNameDiff");

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckIsPluginApp(hapVerifyInfos));
}

/*
 * @tc.name: CheckIsPluginApp_0200
 * @tc.desc: CheckIsPluginApp
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckIsPluginApp_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetBundleType("appPlugin");
    hapVerifyInfo.SetFileType(".hap");

    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_FALSE(hapVerifyUtils.CheckIsPluginApp(hapVerifyInfos));
}

/*
 * @tc.name: CheckIsPluginApp_0300
 * @tc.desc: CheckIsPluginApp
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckIsPluginApp_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfo.SetBundleType("appPlugin");
    hapVerifyInfo.SetFileType(".hsp");

    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_TRUE(hapVerifyUtils.CheckIsPluginApp(hapVerifyInfos));
}

/*
 * @tc.name: CheckIsPluginApp_0400
 * @tc.desc: CheckIsPluginApp
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckIsPluginApp_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");

    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_TRUE(hapVerifyUtils.CheckIsPluginApp(hapVerifyInfos));
}

/*
 * @tc.name: ShowCheckTips_0100
 * @tc.desc: ShowCheckTips
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, ShowCheckTips_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    std::list<std::string> deviceType;
    deviceType.push_back("deviceType");
    hapVerifyInfo.SetDeviceTypes(deviceType);
    OHOS::AppPackingTool::CountryCode countryCode;
    countryCode.policy = "exclude";
    OHOS::AppPackingTool::DistroFilter distroFilter;
    distroFilter.countryCode = countryCode;
    hapVerifyInfo.SetDistroFilter(distroFilter);

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleName");
    hapVerifyInfoDiff.SetDeviceTypes(deviceType);
    hapVerifyInfoDiff.SetDistroFilter(distroFilter);

    hapVerifyInfos.push_back(hapVerifyInfo);
    hapVerifyInfos.push_back(hapVerifyInfoDiff);
    EXPECT_FALSE(hapVerifyUtils.CheckModuleNameIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckDuplicatedIsValid_0100
 * @tc.desc: CheckDuplicatedIsValid
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDuplicatedIsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils hapVerifyUtils;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetModuleName("moduleName");
    std::list<std::string> deviceType;
    deviceType.push_back("deviceType");
    hapVerifyInfo.SetDeviceTypes(deviceType);
    OHOS::AppPackingTool::ApiVersion apiVersionExclude;
    apiVersionExclude.policy = "exclude";
    OHOS::AppPackingTool::DistroFilter distroFilterExclude;
    distroFilterExclude.apiVersion = apiVersionExclude;
    hapVerifyInfo.SetDistroFilter(distroFilterExclude);

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfoDiff;
    hapVerifyInfoDiff.SetBundleName("bundleName");
    hapVerifyInfoDiff.SetModuleName("moduleName");
    hapVerifyInfoDiff.SetDeviceTypes(deviceType);
    OHOS::AppPackingTool::ApiVersion apiVersionInclude;
    apiVersionInclude.policy = "include";
    OHOS::AppPackingTool::DistroFilter distroFilterInclude;
    distroFilterInclude.apiVersion = apiVersionInclude;
    hapVerifyInfoDiff.SetDistroFilter(distroFilterInclude);

    EXPECT_TRUE(hapVerifyUtils.CheckDuplicatedIsValid(hapVerifyInfo, hapVerifyInfoDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0200
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;
    initDistroFilter(distroFilter, distroFilterDiff);
    OHOS::AppPackingTool::HapVerifyUtils utils;

    OHOS::AppPackingTool::ApiVersion apiVersion;
    apiVersion.policy = "exclude";
    distroFilter.apiVersion = apiVersion;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0300
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;
    initDistroFilter(distroFilter, distroFilterDiff);
    OHOS::AppPackingTool::HapVerifyUtils utils;

    OHOS::AppPackingTool::ScreenShape screenShape;
    screenShape.policy = "exclude";
    distroFilter.screenShape = screenShape;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0400
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;
    initDistroFilter(distroFilter, distroFilterDiff);
    OHOS::AppPackingTool::HapVerifyUtils utils;

    OHOS::AppPackingTool::ScreenDensity screenDensity;
    screenDensity.policy = "exclude";
    distroFilter.screenDensity = screenDensity;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0500
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;
    initDistroFilter(distroFilter, distroFilterDiff);
    OHOS::AppPackingTool::HapVerifyUtils utils;

    OHOS::AppPackingTool::ScreenWindow screenWindow;
    screenWindow.policy = "exclude";
    distroFilter.screenWindow = screenWindow;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0600
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;
    initDistroFilter(distroFilter, distroFilterDiff);
    OHOS::AppPackingTool::HapVerifyUtils utils;

    OHOS::AppPackingTool::CountryCode countryCode;
    countryCode.policy = "exclude";
    distroFilter.countryCode = countryCode;
    distroFilterDiff.countryCode = countryCode;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0700
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;

    OHOS::AppPackingTool::CountryCode countryCode;
    countryCode.policy = "";
    distroFilter.countryCode = countryCode;
    distroFilterDiff.countryCode = countryCode;
    OHOS::AppPackingTool::HapVerifyUtils utils;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0800
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDistroFilterDisjoint_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    OHOS::AppPackingTool::DistroFilter distroFilterDiff;

    OHOS::AppPackingTool::ApiVersion apiVersion;
    apiVersion.policy = "";
    distroFilter.apiVersion = apiVersion;
    distroFilterDiff.apiVersion = apiVersion;
    OHOS::AppPackingTool::HapVerifyUtils utils;

    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter, distroFilterDiff));
}

/*
 * @tc.name: CheckCountryCodeCovered_0200
 * @tc.desc: CheckCountryCodeCovered
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCountryCodeCovered_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    OHOS::AppPackingTool::CountryCode countryCode;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> entryHaps;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    OHOS::AppPackingTool::ApiVersion apiVersion;
    apiVersion.policy = "include";
    OHOS::AppPackingTool::DistroFilter distroFilter;
    distroFilter.apiVersion = apiVersion;
    hapVerifyInfo.SetDistroFilter(distroFilter);

    entryHaps.push_back(hapVerifyInfo);
    EXPECT_TRUE(utils.CheckCountryCodeCovered(countryCode, entryHaps));
}

/*
 * @tc.name: CheckCountryCodeCovered_0300
 * @tc.desc: CheckCountryCodeCovered
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckCountryCodeCovered_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    OHOS::AppPackingTool::CountryCode countryCode;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> entryHaps;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;

    entryHaps.push_back(hapVerifyInfo);
    EXPECT_TRUE(utils.CheckCountryCodeCovered(countryCode, entryHaps));
}

/*
 * @tc.name: CheckEntryPolicyValueCoverAll_0100
 * @tc.desc: CheckEntryPolicyValueCoverAll
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckEntryPolicyValueCoverAll_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<std::string> include;
    std::list<std::string> exclude;
    include.push_back("test");
    exclude.push_back("test");

    EXPECT_TRUE(utils.CheckEntryPolicyValueCoverAll(include, exclude));
}

/*
 * @tc.name: CheckEntryPolicyValueCoverAll_0200
 * @tc.desc: CheckEntryPolicyValueCoverAll
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckEntryPolicyValueCoverAll_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<std::string> include;
    std::list<std::string> exclude;
    exclude.push_back("test");

    EXPECT_FALSE(utils.CheckEntryPolicyValueCoverAll(include, exclude));
}

/*
 * @tc.name: CheckEntryPolicyValueCoverAll_0300
 * @tc.desc: CheckEntryPolicyValueCoverAll
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckEntryPolicyValueCoverAll_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<std::string> include;
    std::list<std::string> exclude;
    include.push_back("test");

    EXPECT_FALSE(utils.CheckEntryPolicyValueCoverAll(include, exclude));
}

/*
 * @tc.name: CheckDependencyInFileList_0100
 * @tc.desc: CheckDependencyInFileList
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDependencyInFileList_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    OHOS::AppPackingTool::DependencyItem dependencyItem;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetModuleName("moduleName");
    hapVerifyInfos.push_back(hapVerifyInfo);

    dependencyItem.bundleName = "bundleName";
    dependencyItem.moduleName = "moduleName";

    EXPECT_FALSE(utils.CheckDependencyInFileList(dependencyItem, hapVerifyInfos));
}

/*
 * @tc.name: CheckDependencyInFileList_0200
 * @tc.desc: CheckDependencyInFileList
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDependencyInFileList_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    OHOS::AppPackingTool::DependencyItem dependencyItem;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfos.push_back(hapVerifyInfo);

    dependencyItem.bundleName = "bundleName";
    dependencyItem.moduleName = "moduleName";

    EXPECT_FALSE(utils.CheckDependencyInFileList(dependencyItem, hapVerifyInfos));
}

/*
 * @tc.name: CheckDependencyInFileList_0300
 * @tc.desc: CheckDependencyInFileList
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckDependencyInFileList_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    OHOS::AppPackingTool::DependencyItem dependencyItem;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfos.push_back(hapVerifyInfo);

    dependencyItem.bundleName = "bundleName";
    dependencyItem.moduleName = "moduleName";

    EXPECT_FALSE(utils.CheckDependencyInFileList(dependencyItem, hapVerifyInfos));
}

/*
 * @tc.name: AppFieldsIsSame_0100
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("");

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0200
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundlenamefail");

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0300
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleTypefail");

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0400
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 0;

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0500
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "0";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0600
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "1";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.releaseType = "releaseTypefail";
    hapVerifyInfo.SetApiVersion(moduleApiVersion);

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";
    verifyCollection.releaseType = "releaseType";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0700
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "1";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.releaseType = "releaseType";
    hapVerifyInfo.SetApiVersion(moduleApiVersion);

    hapVerifyInfo.SetTargetBundleName("targetBundleNamefail");

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";
    verifyCollection.releaseType = "releaseType";
    verifyCollection.targetBundleName = "targetBundleName";

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0800
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "1";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.releaseType = "releaseType";
    hapVerifyInfo.SetApiVersion(moduleApiVersion);

    hapVerifyInfo.SetTargetBundleName("targetBundleName");
    hapVerifyInfo.SetTargetPriority(0);

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";
    verifyCollection.releaseType = "releaseType";
    verifyCollection.targetBundleName = "targetBundleName";
    verifyCollection.targetPriority = 1;

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_0900
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "1";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.releaseType = "releaseType";
    hapVerifyInfo.SetApiVersion(moduleApiVersion);

    hapVerifyInfo.SetTargetBundleName("targetBundleName");
    hapVerifyInfo.SetTargetPriority(1);
    hapVerifyInfo.SetModuleType("entry");

    OHOS::AppPackingTool::MultiAppMode tddMultiAppMode;
    tddMultiAppMode.maxCount = 0;
    tddMultiAppMode.multiAppModeType = "multiAppModeType";

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";
    verifyCollection.releaseType = "releaseType";
    verifyCollection.targetBundleName = "targetBundleName";
    verifyCollection.targetPriority = 1;
    verifyCollection.moduleType = "entry";
    OHOS::AppPackingTool::MultiAppMode testMultiAppMode;
    testMultiAppMode.maxCount = 1;
    testMultiAppMode.multiAppModeType = "multiAppModeType";
    verifyCollection.multiAppMode = testMultiAppMode;

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/*
 * @tc.name: AppFieldsIsSame_1000
 * @tc.desc: AppFieldsIsSame
 * @tc.type: FUNC
 */
HWTEST_F(HapVerifyUtilsTestPart2, AppFieldsIsSame_1000, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName("bundleName");
    hapVerifyInfo.SetBundleType("bundleType");

    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.buildVersion = "1";
    hapVerifyInfo.SetVersion(version);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.releaseType = "releaseType";
    hapVerifyInfo.SetApiVersion(moduleApiVersion);

    hapVerifyInfo.SetTargetBundleName("targetBundleName");
    hapVerifyInfo.SetTargetPriority(1);
    hapVerifyInfo.SetModuleType("entry");

    OHOS::AppPackingTool::MultiAppMode tddMultiAppMode;
    tddMultiAppMode.maxCount = 1;
    tddMultiAppMode.multiAppModeType = "multiAppModeType";

    OHOS::AppPackingTool::VerifyCollection verifyCollection;
    verifyCollection.bundleName = "bundleName";
    verifyCollection.bundleType = "bundleType";
    verifyCollection.versionCode = 1;
    verifyCollection.buildVersion = "1";
    verifyCollection.releaseType = "releaseType";
    verifyCollection.targetBundleName = "targetBundleName";
    verifyCollection.targetPriority = 1;
    verifyCollection.moduleType = "entry";
    OHOS::AppPackingTool::MultiAppMode testMultiAppMode;
    testMultiAppMode.maxCount = 1;
    testMultiAppMode.multiAppModeType = "multiAppModeType";
    verifyCollection.multiAppMode = testMultiAppMode;

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.AppFieldsIsSame(verifyCollection, hapVerifyInfo));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0100
 * @tc.desc: Test empty hapVerifyInfos list and duplicate URIs in same module
 * @tc.type: FUNC
 * @tc.require: patch_1439_basic_functionality
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0100,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));

    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    std::list<std::string> proxyDataUris = {"test", "test"};
    hapVerifyInfo.SetProxyDataUris(proxyDataUris);
    hapVerifyInfos.push_back(hapVerifyInfo);
    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0200
 * @tc.desc: Test different modules with same URI in same deviceType returns false
 * @tc.type: FUNC
 * @tc.require: patch_1439_duplicate_uri_cross_modules_same_devicetype
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0200,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"phone"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0300
 * @tc.desc: Test different modules with same URI in different deviceTypes returns true
 * @tc.type: FUNC
 * @tc.require: patch_1439_same_uri_different_devicetype
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0300,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"tablet"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_TRUE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0400
 * @tc.desc: Test module with empty URIs is skipped
 * @tc.type: FUNC
 * @tc.require: patch_1439_skip_empty_uris
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0400,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"phone"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_TRUE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0500
 * @tc.desc: Test module with empty deviceTypes uses NULL_DEVICE_TYPE
 * @tc.type: FUNC
 * @tc.require: patch_1439_null_devicetype_handling
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0500,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0600
 * @tc.desc: Test module without deviceType conflicts with module with deviceType
 * @tc.type: FUNC
 * @tc.require: patch_1439_null_devicetype_vs_devicetype_conflict
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0600,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"phone"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0700
 * @tc.desc: Test different modules with different URIs in same deviceType returns true
 * @tc.type: FUNC
 * @tc.require: patch_1439_different_uris_same_devicetype
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0700,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"phone"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service2"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_TRUE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0800
 * @tc.desc: Test multiple modules with multiple deviceTypes and URIs
 * @tc.type: FUNC
 * @tc.require: patch_1439_complex_scenario
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0800,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1", "uri://service2"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"tablet"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service1", "uri://service3"};
    info2.SetProxyDataUris(uris2);

    OHOS::AppPackingTool::HapVerifyInfo info3;
    info3.SetModuleName("moduleC");
    std::list<std::string> deviceTypes3 = {"phone"};
    info3.SetDeviceTypes(deviceTypes3);
    std::list<std::string> uris3 = {"uri://service3", "uri://service4"};
    info3.SetProxyDataUris(uris3);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);
    hapVerifyInfos.push_back(info3);

    EXPECT_TRUE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_0900
 * @tc.desc: Test module with multiple URIs where one is duplicate across modules
 * @tc.type: FUNC
 * @tc.require: patch_1439_partial_duplicate
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_0900,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {"uri://service1", "uri://service2", "uri://service3"};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"phone"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {"uri://service4", "uri://service2", "uri://service5"};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_FALSE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}

/**
 * @tc.name: CheckProxyDataUriIsUnique_1000
 * @tc.desc: Test all modules with empty URIs
 * @tc.type: FUNC
 * @tc.require: patch_1439_all_empty_uris
 */
HWTEST_F(HapVerifyUtilsTestPart2, CheckProxyDataUriIsUnique_1000,
    Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HapVerifyUtils utils;
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;

    OHOS::AppPackingTool::HapVerifyInfo info1;
    info1.SetModuleName("moduleA");
    std::list<std::string> deviceTypes1 = {"phone"};
    info1.SetDeviceTypes(deviceTypes1);
    std::list<std::string> uris1 = {};
    info1.SetProxyDataUris(uris1);

    OHOS::AppPackingTool::HapVerifyInfo info2;
    info2.SetModuleName("moduleB");
    std::list<std::string> deviceTypes2 = {"tablet"};
    info2.SetDeviceTypes(deviceTypes2);
    std::list<std::string> uris2 = {};
    info2.SetProxyDataUris(uris2);

    hapVerifyInfos.push_back(info1);
    hapVerifyInfos.push_back(info2);

    EXPECT_TRUE(utils.CheckProxyDataUriIsUnique(hapVerifyInfos));
}
} // namespace OHOS
