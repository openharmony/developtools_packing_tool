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
#include <string>
#include <vector>

#define private public
#define protected public
#include "hqf_verify.h"
#undef private
#undef protected
#include "hqf_info.h"
#include "utils.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {

class HqfVerifyTest : public testing::Test {
public:
    HqfVerifyTest() {}
    virtual ~HqfVerifyTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HqfVerifyTest::SetUpTestCase() {}

void HqfVerifyTest::TearDownTestCase() {}

void HqfVerifyTest::SetUp() {}

void HqfVerifyTest::TearDown() {}

using namespace AppPackingTool;

void CreateHqfInfo(HqfInfo &hqfInfo)
{
    std::string bundleName("bundleName");
    hqfInfo.SetBundleName(bundleName);
    int32_t versionCode = 1;
    hqfInfo.SetVersionCode(versionCode);
    std::string versionName("2.1");
    hqfInfo.SetVersionName(versionName);
    hqfInfo.SetPatchVersionCode(versionCode);
    hqfInfo.SetPatchVersionName(versionName);
    std::string moduleName("moduleName");
    hqfInfo.SetModuleName(moduleName);
    std::list<std::string> deviceTypes = {"deviceTypes"};
    hqfInfo.SetDeviceTypes(deviceTypes);
}

/*
 * @tc.name: CheckHQFIsValid_0100
 * @tc.desc: CheckHQFIsValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfVerifyTest, CheckHQFIsValid_0100, Function | MediumTest | Level1)
{
    std::vector<HqfInfo> hqfInfos;
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckHQFIsValid(hqfInfos));

    HqfInfo hqfInfo;
    CreateHqfInfo(hqfInfo);
    hqfInfos.emplace_back(hqfInfo);
    EXPECT_TRUE(OHOS::AppPackingTool::HQFVerify::CheckHQFIsValid(hqfInfos));

    HqfInfo hqfInfo1;
    CreateHqfInfo(hqfInfo1);
    hqfInfos.emplace_back(hqfInfo1);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckHQFIsValid(hqfInfos));

    std::string bundleName = "";
    hqfInfos.at(0).SetBundleName(bundleName);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckHQFIsValid(hqfInfos));
}

/*
 * @tc.name: CheckAppFields_0200
 * @tc.desc: CheckAppFields.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfVerifyTest, CheckAppFields_0200, Function | MediumTest | Level1)
{
    std::vector<HqfInfo> hqfInfos;
    HqfInfo hqfInfo;
    std::string bundleName("bundleName");
    hqfInfo.SetBundleName(bundleName);
    int32_t versionCode = 2;
    hqfInfo.SetVersionCode(versionCode);
    hqfInfo.SetPatchVersionCode(versionCode);
    std::string versionName;
    hqfInfo.SetVersionName(versionName);
    hqfInfo.SetPatchVersionName(versionName);
    hqfInfos.emplace_back(hqfInfo);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckAppFields(hqfInfos));

    versionName = "2.1";
    hqfInfos.at(0).SetVersionName(versionName);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckAppFields(hqfInfos));

    std::string patchVersionName = "2.1";
    hqfInfos.at(0).SetPatchVersionName(patchVersionName);
    HqfInfo hqfInfo1;
    CreateHqfInfo(hqfInfo1);
    hqfInfos.emplace_back(hqfInfo1);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckAppFields(hqfInfos));

    versionCode = 1;
    hqfInfos.at(0).SetVersionCode(versionCode);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckAppFields(hqfInfos));
}

/*
 * @tc.name: CheckModuleIsDuplicated_0300
 * @tc.desc: CheckModuleIsDuplicated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfVerifyTest, CheckHQFIsValid_0300, Function | MediumTest | Level1)
{
    HqfInfo hqfInfo;
    CreateHqfInfo(hqfInfo);
    std::string moduleName("test");
    hqfInfo.SetModuleName(moduleName);

    HqfInfo hqfInfo1;
    CreateHqfInfo(hqfInfo1);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckModuleIsDuplicated(hqfInfo, hqfInfo1));

    moduleName = "moduleName";
    hqfInfo.SetModuleName(moduleName);
    EXPECT_TRUE(OHOS::AppPackingTool::HQFVerify::CheckModuleIsDuplicated(hqfInfo, hqfInfo1));

    std::list<std::string> deviceTypes = {"test"};
    hqfInfo1.SetDeviceTypes(deviceTypes);
    EXPECT_FALSE(OHOS::AppPackingTool::HQFVerify::CheckModuleIsDuplicated(hqfInfo, hqfInfo1));
}
}

