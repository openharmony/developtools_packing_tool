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
}
