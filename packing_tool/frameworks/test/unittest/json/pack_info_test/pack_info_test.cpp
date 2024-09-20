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

const std::string JSON_STRING_NULL = "{}";

const std::string JSON_STRING_EMPTY = "{"
    "\"summary\": {"
        "\"app\": {"
            "}"
        "},"
        "\"modules\": ["
        "]"
    "},"
    "\"packages\":["
    "]"
"}";

const std::string EMPTY_APP_OBJECT_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
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

const std::string EMPTY_MODULES_OBJECT_JSON_STRING = "{"
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

const std::string EMPTY_APP_BUNDLE_TYPE_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"\","
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
 * @tc.name: ParseFromString_0200
 * @tc.desc: ParseFromString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, ParseFromString_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    EXPECT_FALSE(packInfo.ParseFromString(""));
}

/*
 * @tc.name: ParseFromFile_0100
 * @tc.desc: ParseFromFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, ParseFromFile_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string test("test_module.json");
    EXPECT_FALSE(packInfo.ParseFromFile(test));
}

/*
 * @tc.name: ParseFromFile_0200
 * @tc.desc: ParseFromFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, ParseFromFile_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string test("test_module.json");
    FILE* fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(JSON_STRING.c_str(), JSON_STRING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(packInfo.ParseFromFile(test));
        system("rm -f test_module.json");
    }
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
 * @tc.name: GetSummaryObject_0200
 * @tc.desc: GetSummaryObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSummaryObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetSummaryObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetSummaryObject_0300
 * @tc.desc: GetSummaryObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSummaryObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_NULL);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetSummaryObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetPackagesObject_0100
 * @tc.desc: GetPackagesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackagesObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_TRUE(packInfo.GetPackagesObject(summaryObj));
    EXPECT_NE(summaryObj, nullptr);
}

/*
 * @tc.name: GetPackagesObject_0200
 * @tc.desc: GetPackagesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackagesObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetPackagesObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetPackagesObject_0300
 * @tc.desc: GetPackagesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackagesObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_NULL);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetPackagesObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
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
 * @tc.name: GetAppObject_0200
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetAppObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetAppObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetAppObject_0300
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetAppObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_NULL);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetAppObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetModulesObject_0100
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_TRUE(packInfo.GetModulesObject(summaryObj));
    EXPECT_NE(summaryObj, nullptr);
}

/*
 * @tc.name: GetModulesObject_0200
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetModulesObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetModulesObject_0300
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_NULL);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetModulesObject(summaryObj));
    EXPECT_EQ(summaryObj, nullptr);
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
 * @tc.name: GetBundleName_0200
 * @tc.desc: GetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string bundleName = "";
    EXPECT_FALSE(packInfo.GetBundleName(bundleName));
}

/*
 * @tc.name: GetBundleNameByAppObj_0100
 * @tc.desc: GetBundleNameByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleNameByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleName = "";
    EXPECT_TRUE(packInfo.GetBundleNameByAppObj(appObj, bundleName));
}

/*
 * @tc.name: GetBundleNameByAppObj_0200
 * @tc.desc: GetBundleNameByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleNameByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    std::string bundleName = "";
    EXPECT_FALSE(packInfo.GetBundleNameByAppObj(appObj, bundleName));
}

/*
 * @tc.name: GetBundleNameByAppObj_0300
 * @tc.desc: GetBundleNameByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleNameByAppObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_APP_OBJECT_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleName = "";
    EXPECT_FALSE(packInfo.GetBundleNameByAppObj(appObj, bundleName));
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
 * @tc.name: SetBundleName_0200
 * @tc.desc: SetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetBundleName_0200, Function | MediumTest | Level1)
{
    std::string bundleName = "com.example.myapplication";
    OHOS::AppPackingTool::PackInfo packInfo;
    EXPECT_FALSE(packInfo.SetBundleName(bundleName));
}

/*
 * @tc.name: SetBundleName_0300
 * @tc.desc: SetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetBundleName_0300, Function | MediumTest | Level1)
{
    std::string bundleName = "com.example.myapplication";
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_EMPTY);
    EXPECT_FALSE(packInfo.SetBundleName(bundleName));
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
 * @tc.name: GetBundleType_0200
 * @tc.desc: GetBundleType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_FALSE(packInfo.GetBundleType(bundleType, defaultBundleType));
}

/*
 * @tc.name: GetBundleTypeByAppObj_0100
 * @tc.desc: GetBundleTypeByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleTypeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_TRUE(packInfo.GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType));
}

/*
 * @tc.name: GetBundleTypeByAppObj_0200
 * @tc.desc: GetBundleTypeByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleTypeByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_FALSE(packInfo.GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType));
}

/*
 * @tc.name: GetBundleTypeByAppObj_0300
 * @tc.desc: GetBundleTypeByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleTypeByAppObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_APP_BUNDLE_TYPE_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_TRUE(packInfo.GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType));
    EXPECT_EQ(bundleType, "");
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
 * @tc.name: GetVersionObject_0200
 * @tc.desc: GetVersionObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_FALSE(packInfo.GetVersionObject(versionObj));
    EXPECT_EQ(versionObj, nullptr);
}

/*
 * @tc.name: GetVersionObject_0300
 * @tc.desc: GetVersionObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_EMPTY);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_FALSE(packInfo.GetVersionObject(versionObj));
    EXPECT_EQ(versionObj, nullptr);
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
 * @tc.name: GetDistroObject_0200
 * @tc.desc: GetDistroObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObject(moduleIndex, distroObj));
    EXPECT_EQ(distroObj, nullptr);
}

/*
 * @tc.name: GetDistroObject_0300
 * @tc.desc: GetDistroObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING_EMPTY);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObject(moduleIndex, distroObj));
    EXPECT_EQ(distroObj, nullptr);
}

/*
 * @tc.name: GetDistroObjectByModulesObj_0100
 * @tc.desc: GetDistroObjectByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModulesObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObjectByModulesObj(modulesObj, moduleIndex, distroObj));
}

/*
 * @tc.name: GetDistroObjectByModulesObj_0200
 * @tc.desc: GetDistroObjectByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModulesObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObjectByModulesObj(modulesObj, moduleIndex, distroObj));
}

/*
 * @tc.name: GetDistroObjectByModulesObj_0300
 * @tc.desc: GetDistroObjectByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModulesObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_MODULES_OBJECT_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObjectByModulesObj(modulesObj, moduleIndex, distroObj));
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0100
 * @tc.desc: GetDistroObjectByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0200
 * @tc.desc: GetDistroObjectByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObjectByModuleObj(moduleObj, distroObj));
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0300
 * @tc.desc: GetDistroObjectByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_MODULES_OBJECT_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
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
