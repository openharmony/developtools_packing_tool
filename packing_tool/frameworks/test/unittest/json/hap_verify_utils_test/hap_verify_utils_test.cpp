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

class HapVerifyUtilsTest : public testing::Test {
public:
    HapVerifyUtilsTest() {}
    virtual ~HapVerifyUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HapVerifyUtilsTest::SetUpTestCase() {}

void HapVerifyUtilsTest::TearDownTestCase() {}

void HapVerifyUtilsTest::SetUp() {}

void HapVerifyUtilsTest::TearDown() {}

namespace {
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo1;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo2;
}

void InitHapVerifyInfo1()
{
    // hapVerifyInfo1
    hapVerifyInfo1.SetBundleName("test");
    hapVerifyInfo1.SetBundleType("hap");
    hapVerifyInfo1.SetVendor("vendor");

    OHOS::AppPackingTool::Version version1;
    version1.versionCode = 1;
    version1.versionName = "1.0.0";
    version1.minCompatibleVersionCode = 1;
    hapVerifyInfo1.SetVersion(version1);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion1;
    moduleApiVersion1.compatibleApiVersion = 1;
    moduleApiVersion1.targetApiVersion = 1;
    moduleApiVersion1.releaseType = "release";
    hapVerifyInfo1.SetApiVersion(moduleApiVersion1);

    hapVerifyInfo1.SetTargetBundleName("test");
    hapVerifyInfo1.SetTargetPriority(1);
    hapVerifyInfo1.SetDebug(false);
    hapVerifyInfo1.SetModuleName("test");
    hapVerifyInfo1.SetModuleType("entry");

    OHOS::AppPackingTool::MultiAppMode multiAppMode1;
    multiAppMode1.multiAppModeType = "standard";
    multiAppMode1.maxCount = 1;
    hapVerifyInfo1.SetMultiAppMode(multiAppMode1);

    const std::list<std::string> deviceTypes1 = {"phone", "watch"};
    hapVerifyInfo1.SetDeviceTypes(deviceTypes1);

    OHOS::AppPackingTool::ApiVersion apiVersion1;
    apiVersion1.policy = "include";

    OHOS::AppPackingTool::ScreenShape screenShape1;
    screenShape1.policy = "include";

    OHOS::AppPackingTool::ScreenDensity screenDensity1;
    screenDensity1.policy = "include";

    OHOS::AppPackingTool::ScreenWindow screenWindow1;
    screenWindow1.policy = "include";

    OHOS::AppPackingTool::CountryCode countryCode1;
    countryCode1.policy = "include";

    OHOS::AppPackingTool::DistroFilter distroFilter1;
    distroFilter1.apiVersion = apiVersion1;
    distroFilter1.screenShape = screenShape1;
    distroFilter1.screenDensity = screenDensity1;
    distroFilter1.screenWindow = screenWindow1;
    distroFilter1.countryCode = countryCode1;

    hapVerifyInfo1.SetDistroFilter(distroFilter1);
}

void InitHapVerifyInfo2()
{
    // hapVerifyInfo2
    hapVerifyInfo2.SetBundleName("test");
    hapVerifyInfo2.SetBundleType("hap");
    hapVerifyInfo2.SetVendor("vendor");

    OHOS::AppPackingTool::Version version2;
    version2.versionCode = 1;
    version2.versionName = "1.0.0";
    version2.minCompatibleVersionCode = 1;
    hapVerifyInfo2.SetVersion(version2);

    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion2;
    moduleApiVersion2.compatibleApiVersion = 1;
    moduleApiVersion2.targetApiVersion = 1;
    moduleApiVersion2.releaseType = "release";
    hapVerifyInfo2.SetApiVersion(moduleApiVersion2);

    hapVerifyInfo2.SetTargetBundleName("test");
    hapVerifyInfo2.SetTargetPriority(1);
    hapVerifyInfo2.SetDebug(false);
    hapVerifyInfo2.SetModuleName("test2");
    hapVerifyInfo2.SetModuleType("entry");

    OHOS::AppPackingTool::MultiAppMode multiAppMode2;
    multiAppMode2.multiAppModeType = "standard";
    multiAppMode2.maxCount = 1;
    hapVerifyInfo2.SetMultiAppMode(multiAppMode2);

    const std::list<std::string> deviceTypes2 = {"computer", "ipad"};
    hapVerifyInfo2.SetDeviceTypes(deviceTypes2);

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
    countryCode2.policy = "exclude";
    countryCode2.value = {"c", "d"};

    OHOS::AppPackingTool::DistroFilter distroFilter2;
    distroFilter2.apiVersion = apiVersion2;
    distroFilter2.screenShape = screenShape2;
    distroFilter2.screenDensity = screenDensity2;
    distroFilter2.screenWindow = screenWindow2;
    distroFilter2.countryCode = countryCode2;

    hapVerifyInfo2.SetDistroFilter(distroFilter2);
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
 * @tc.name: CheckHapIsValid_0100
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckHapIsValid_0100, Function | MediumTest | Level1)
{
    InitHapVerifyInfo1();
    InitHapVerifyInfo2();

    // hapVerifyInfos
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo2);

    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_TRUE(utils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckHapIsValid_0200
 * @tc.desc: CheckHapIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckHapIsValid_0200, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckHapIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0100
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0100, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0200
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0200, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfos.push_back(hapVerifyInfo);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0300
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0300, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test3");
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0400
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0400, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap3");
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0500
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0500, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 3;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));

    std::list<OHOS::AppPackingTool::HapVerifyInfo>::iterator it;
    it = --hapVerifyInfos.end();
    hapVerifyInfos.erase(it);
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 2;
    hapVerifyInfo3.SetVersion(version);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0600
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0600, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.compatibleApiVersion = 2;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));

    std::list<OHOS::AppPackingTool::HapVerifyInfo>::iterator it;
    it = --hapVerifyInfos.end();
    hapVerifyInfos.erase(it);
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 2;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));

    it = --hapVerifyInfos.end();
    hapVerifyInfos.erase(it);
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "debug";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0700
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0700, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfo3.SetTargetBundleName("test3");
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0800
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0800, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfo3.SetTargetBundleName("test");
    hapVerifyInfo3.SetTargetPriority(2);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_0900
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_0900, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfo3.SetTargetBundleName("test");
    hapVerifyInfo3.SetTargetPriority(1);
    hapVerifyInfo3.SetDebug(true);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckAppFieldsIsSame_1000
 * @tc.desc: CheckAppFieldsIsSame
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckAppFieldsIsSame_1000, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetBundleName("test");
    hapVerifyInfo3.SetBundleType("hap");
    OHOS::AppPackingTool::Version version;
    version.versionCode = 1;
    version.versionName = "1.0.0";
    version.minCompatibleVersionCode = 1;
    hapVerifyInfo3.SetVersion(version);
    OHOS::AppPackingTool::ModuleApiVersion moduleApiVersion;
    moduleApiVersion.compatibleApiVersion = 1;
    moduleApiVersion.targetApiVersion = 1;
    moduleApiVersion.releaseType = "release";
    hapVerifyInfo3.SetApiVersion(moduleApiVersion);
    hapVerifyInfo3.SetTargetBundleName("test");
    hapVerifyInfo3.SetTargetPriority(1);
    hapVerifyInfo3.SetDebug(false);
    hapVerifyInfo3.SetModuleType("entry");
    OHOS::AppPackingTool::MultiAppMode multiAppMode;
    multiAppMode.multiAppModeType = "nostandard";
    multiAppMode.maxCount = 2;
    hapVerifyInfo3.SetMultiAppMode(multiAppMode);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckAppFieldsIsSame(hapVerifyInfos));
}

/*
 * @tc.name: CheckModuleNameIsValid_0100
 * @tc.desc: CheckModuleNameIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckModuleNameIsValid_0100, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfos.push_back(hapVerifyInfo3);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckModuleNameIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckModuleNameIsValid_0200
 * @tc.desc: CheckModuleNameIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckModuleNameIsValid_0200, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfo3.SetModuleName("test");
    const std::list<std::string> deviceTypes = {"phone", "watch"};
    hapVerifyInfo3.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckModuleNameIsValid(hapVerifyInfos));
}

/*
 * @tc.name: CheckDistroFilterDisjoint_0100
 * @tc.desc: CheckDistroFilterDisjoint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckDistroFilterDisjoint_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter1;
    OHOS::AppPackingTool::DistroFilter distroFilter2;
    initDistroFilter(distroFilter1, distroFilter2);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));

    OHOS::AppPackingTool::CountryCode countryCode2;
    countryCode2.policy = "exclude";
    countryCode2.value = {"c", "d"};
    distroFilter2.countryCode = countryCode2;
    EXPECT_TRUE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));

    OHOS::AppPackingTool::ScreenWindow screenWindow1;
    screenWindow1.policy = "include";
    distroFilter1.screenWindow = screenWindow1;
    EXPECT_TRUE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));

    OHOS::AppPackingTool::ScreenDensity screenDensity1;
    screenDensity1.policy = "include";
    distroFilter1.screenDensity = screenDensity1;
    EXPECT_TRUE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));

    OHOS::AppPackingTool::ScreenShape screenShape1;
    screenShape1.policy = "include";
    distroFilter1.screenShape = screenShape1;
    EXPECT_TRUE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));

    OHOS::AppPackingTool::ApiVersion apiVersion1;
    apiVersion1.policy = "include";
    distroFilter1.apiVersion = apiVersion1;
    EXPECT_TRUE(utils.CheckDistroFilterDisjoint(distroFilter1, distroFilter2));
}

/*
 * @tc.name: CheckPolicyValueDisjoint_0100
 * @tc.desc: CheckPolicyValueDisjoint
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckPolicyValueDisjoint_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PolicyValue policyValue1;
    OHOS::AppPackingTool::PolicyValue policyValue2;
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_FALSE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.policy = "exclude";
    policyValue2.policy = "exclude";
    EXPECT_FALSE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.policy = "include";
    policyValue1.value = {"c", "d"};
    policyValue2.policy = "exclude";
    EXPECT_FALSE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.policy = "include";
    policyValue1.value = {"c", "d"};
    policyValue2.policy = "include";
    policyValue2.value = {"c", "d"};
    EXPECT_FALSE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.value.clear();
    EXPECT_TRUE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.policy = "exclude";
    policyValue2.policy = "include";
    EXPECT_FALSE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));

    policyValue1.value = {"c", "d"};
    EXPECT_TRUE(utils.CheckPolicyValueDisjoint(policyValue1, policyValue2));
}

/*
 * @tc.name: CheckPackageNameIsValid_0100
 * @tc.desc: CheckPackageNameIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyUtilsTest, CheckPackageNameIsValid_0100, Function | MediumTest | Level1)
{
    std::list<OHOS::AppPackingTool::HapVerifyInfo> hapVerifyInfos;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo3;
    hapVerifyInfos.push_back(hapVerifyInfo3);
    hapVerifyInfos.push_back(hapVerifyInfo1);
    OHOS::AppPackingTool::HapVerifyUtils utils;
    EXPECT_TRUE(utils.CheckPackageNameIsValid(hapVerifyInfos));
    hapVerifyInfos.clear();
    const std::list<std::string> deviceTypes = {"computer", "ipad"};
    hapVerifyInfo3.SetDeviceTypes(deviceTypes);
    hapVerifyInfo3.SetPackageName("test");
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo4;
    hapVerifyInfo4.SetPackageName("test");
    hapVerifyInfo4.SetDeviceTypes(deviceTypes);
    hapVerifyInfos.push_back(hapVerifyInfo3);
    hapVerifyInfos.push_back(hapVerifyInfo4);
    EXPECT_FALSE(utils.CheckPackageNameIsValid(hapVerifyInfos));
}
}
