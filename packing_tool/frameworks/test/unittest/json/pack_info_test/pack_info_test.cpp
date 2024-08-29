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
#include <string>

#define private public
#define protected public
#include "pack_info.h"
#include "pt_json.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {
const std::string JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\": ["
            "{"
                "\"mainAbility\": \"EntryAbility\","
                "\"deviceType\": ["
                    "\"default\","
                    "\"tablet\""
                "],"
                "\"abilities\": ["
                    "{"
                        "\"name\": \"EntryAbility\","
                        "\"label\": \"$string:EntryAbility_label\""
                    "}"
                "],"
                "\"distro\": {"
                    "\"moduleType\": \"entry\","
                    "\"installationFree\": false,"
                    "\"deliveryWithInstall\": true,"
                    "\"moduleName\": \"entry\""
                "},"
                "\"extensionAbilities\": ["
                "],"
                "\"apiVersion\": {"
                    "\"compatible\": 12,"
                    "\"releaseType\": \"Canary2\","
                    "\"target\": 12"
                "}"
            "}"
        "]"
    "},"
    "\"packages\":["
        "{"
            "\"deviceType\": ["
                "\"default\","
                "\"tablet\""
            "],"
            "\"moduleType\": \"entry\","
            "\"deliveryWithInstall\": true,"
            "\"name\": \"entry-default\""
        "}"
    "]"
"}";
}

class PackInfoTest : public testing::Test {
public:
    PackInfoTest() {}
    virtual ~PackInfoTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PackInfoTest::SetUpTestCase() {}

void PackInfoTest::TearDownTestCase() {}

void PackInfoTest::SetUp() {}

void PackInfoTest::TearDown() {}

/*
 * @tc.name: ParseFromString_0100
 * @tc.desc: ParseFromString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, ParseFromString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    EXPECT_TRUE(packInfo.ParseFromString(JSON_STRING));
}

/*
 * @tc.name: GetSummaryObject_0100
 * @tc.desc: GetSummaryObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSummaryObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_TRUE(packInfo.GetSummaryObject(summaryObj));
    EXPECT_NE(summaryObj, nullptr);
}

/*
 * @tc.name: GetAppObject_0100
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetAppObject_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    EXPECT_NE(appObj, nullptr);
}

/*
 * @tc.name: GetBundleName_0100
 * @tc.desc: GetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::string bundleName = "";
    EXPECT_TRUE(packInfo.GetBundleName(bundleName));
    EXPECT_NE(bundleName, "");
}

/*
 * @tc.name: SetBundleName_0100
 * @tc.desc: SetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetBundleName_0100, Function | MediumTest | Level1)
{
    std::string bundleName = "com.example.myapplication";
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    EXPECT_TRUE(packInfo.SetBundleName(bundleName));
}

/*
 * @tc.name: GetBundleType_0100
 * @tc.desc: GetBundleType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_TRUE(packInfo.GetBundleType(bundleType, defaultBundleType));
    EXPECT_EQ(bundleType, "bundleApp");
}

/*
 * @tc.name: GetVersionObject_0100
 * @tc.desc: GetVersionObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    EXPECT_NE(versionObj, nullptr);
}

/*
 * @tc.name: GetDistroObject_0100
 * @tc.desc: GetDistroObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObject(moduleIndex, distroObj));
    EXPECT_NE(distroObj, nullptr);
}

/*
 * @tc.name: GetExtensionAbilitiesObj_0100
 * @tc.desc: GetExtensionAbilitiesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObj(moduleIndex, extensionAbilitiesObj));
    EXPECT_NE(extensionAbilitiesObj, nullptr);
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModulesObj_0100
 * @tc.desc: GetExtensionAbilitiesObjByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModulesObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    packInfo.GetModulesObject(modulesObj);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModulesObj(modulesObj, moduleIndex, extensionAbilitiesObj));
    EXPECT_NE(extensionAbilitiesObj, nullptr);
}

/*
 * @tc.name: GetPackageObject_0100
 * @tc.desc: GetPackageObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t packageIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    EXPECT_TRUE(packInfo.GetPackageObject(packageIndex, packageObj));
    EXPECT_NE(packageObj, nullptr);
}

/*
 * @tc.name: GetVersion_0100
 * @tc.desc: GetVersion.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoVersion version;
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    EXPECT_TRUE(packInfo.GetVersion(version));
    EXPECT_EQ(version.code, 1000000);
    EXPECT_EQ(version.name, "1.0.0");
}

/*
 * @tc.name: SetVersionCode_0100
 * @tc.desc: SetVersionCode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int versionCode = 1;
    EXPECT_TRUE(packInfo.SetVersionCode(versionCode));
}

/*
 * @tc.name: SetVersionName_0100
 * @tc.desc: SetVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::string versionName = "2.0.0";
    EXPECT_TRUE(packInfo.SetVersionName(versionName));
}

/*
 * @tc.name: GetNameByPackageObj_0100
 * @tc.desc: GetNameByPackageObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByPackageObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t packageIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    packInfo.GetPackageObject(packageIndex, packageObj);
    EXPECT_NE(packageObj, nullptr);
    std::string name = "";
    EXPECT_TRUE(packInfo.GetNameByPackageObj(packageObj, name));
    EXPECT_EQ(name, "entry-default");
}

/*
 * @tc.name: GetFormNames_0100
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_TRUE(packInfo.GetFormNames(formNames, formFullNames));
}
}
