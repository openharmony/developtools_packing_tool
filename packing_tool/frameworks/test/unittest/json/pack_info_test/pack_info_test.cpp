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

const std::string NO_VERSION_CODE_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\": ["
        "]"
    "}"
"}";

const std::string ERR_VERSION_CODE_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": \"1.0.0\","
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\": ["
        "]"
    "}"
"}";

const std::string NO_VERSION_NAME_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000"
            "}"
        "},"
        "\"modules\": ["
        "]"
    "}"
"}";

const std::string ERR_VERSION_NAME_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": 1000000"
            "}"
        "},"
        "\"modules\": ["
        "]"
    "}"
"}";

const std::string COMMON_JSON_STRING = "{"
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
                    "\"deliveryWithInstall\": true"
                "},"
                "\"extensionAbilities\": ["
                    "{"
                        "\"name\": \"EntryFormAbility1\","
                        "\"type\": \"form\","
                        "\"forms\": ["
                            "{"
                                "\"name\": \"form1\","
                                "\"defaultDimension\": \"2*2\","
                                "\"supportDimensions\": ["
                                    "\"2*2\""
                                "]"
                            "},"
                            "{"
                                "\"defaultDimension\": \"2*2\","
                                "\"supportDimensions\": ["
                                    "\"2*2\""
                                "]"
                            "},"
                           "{"
                                "\"name\": \"form3\""
                            "},"
                            "{"
                                "\"name\": \"form4\","
                                "\"defaultDimension\": \"defaultDimension\","
                                "\"supportDimensions\": \"supportDimensions\""
                            "}"
                        "]"
                    "},"
                    "{"
                        "\"name\": \"EntryFormAbility2\","
                        "\"type\": \"form\","
                        "\"forms\": \"form\""
                    "},"
                    "{"
                        "\"name\": \"EntryFormAbility3\","
                        "\"type\": \"form\","
                        "\"forms\": ["
                            "{"
                                "\"name\": \"form1\","
                                "\"defaultDimension\": \"2*2\","
                                "\"supportDimensions\": ["
                                    "\"2*2\""
                                "]"
                            "}"
                        "]"
                    "},"
                    "{"
                        "\"name\": \"EntryFormAbility4\","
                        "\"type\": \"form\","
                        "\"forms\": ["
                            "{"
                                "\"name\": \"form1\","
                                "\"supportDimensions\": ["
                                    "\"2*2\""
                                "]"
                            "}"
                        "]"
                    "},"
                    "{"
                        "\"name\": \"EntryFormAbility5\","
                        "\"type\": \"form\","
                        "\"forms\": ["
                            "{"
                                "\"name\": \"form1\","
                                "\"defaultDimension\": \"2*2\""
                            "}"
                        "]"
                    "}"
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
            "\"deliveryWithInstall\": true"
        "}"
    "]"
"}";

const std::string NO_DISTRO_JSON_STRING = "{"
    "\"summary\": {"
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
                "]"
            "}"
        "]"
    "}"
"}";

const std::string NO_EXTENSION_JSON_STRING = "{"
    "\"summary\": {"
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
                "\"apiVersion\": {"
                    "\"compatible\": 12,"
                    "\"releaseType\": \"Canary2\","
                    "\"target\": 12"
                "}"
            "}"
        "]"
    "}"
"}";

const std::string NO_FORM_NAME_JSON_STRING = "{"
    "\"summary\": {"
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
                    "{"
                        "\"name\": \"EntryFormAbility2\","
                        "\"type\": \"form\","
                        "\"forms\": \"form\""
                    "}"
                "],"
                "\"apiVersion\": {"
                    "\"compatible\": 12,"
                    "\"releaseType\": \"Canary2\","
                    "\"target\": 12"
                "}"
            "}"
        "]"
    "}"
"}";

const std::string MODULES_NOT_ARRAY_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": \"test\""
    "}"
"}";

const std::string APP_EMPTY_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
        "}"
    "}"
"}";

const std::string APP_BUNDLE_NAME_NOT_STRING_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": ["
            "]"
        "}"
    "}"
"}";

const std::string APP_VERSION_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"version\": \"test\""
        "}"
    "}"
"}";

const std::string MODULES_DISTRO_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": ["
            "{"
                "\"distro\": \"test\""
            "}"
        "]"
    "}"
"}";

const std::string MODULES_EXTENSION_ABILITIES_NOT_ARRAY_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": ["
            "{"
                "\"extensionAbilities\": \"test\""
            "}"
        "]"
    "}"
"}";

const std::string MODULES_DISTRO_MODULE_NAME_NOT_STRING_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": ["
            "{"
                "\"distro\": {"
                    "\"moduleName\": {"
                    "}"
                "}"
            "}"
        "]"
    "}"
"}";

const std::string PACKAGES_NAME_NOT_STRING_TEST_JSON_STRING = "{"
    "\"packages\":["
        "{"
            "\"name\": ["
            "]"
        "}"
    "]"
"}";

const std::string MODULES_EXTENSION_ABILITIES_FORMS_NAME_NOT_STRING_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": ["
            "{"
                "\"extensionAbilities\": ["
                    "{"
                        "\"forms\": ["
                            "{"
                                "\"name\": ["
                                "]"
                            "}"
                        "]"
                    "}"
                "]"
            "}"
        "]"
    "}"
"}";

const std::string MODULES_EXTENSION_ABILITIES_FORMS_DEFAULT_DIMENSION_NOT_STRING_TEST_JSON_STRING = "{"
    "\"summary\": {"
        "\"modules\": ["
            "{"
                "\"extensionAbilities\": ["
                    "{"
                        "\"forms\": ["
                            "{"
                                "\"name\": \"test\","
                                "\"defaultDimension\": ["
                                "]"
                            "}"
                        "]"
                    "}"
                "]"
            "}"
        "]"
    "}"
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
 * @tc.name: GetSummaryObject_0400
 * @tc.desc: GetSummaryObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSummaryObject_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "summary": "invalid_object"
    })";
    packInfo.ParseFromString(jsonString);
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
 * @tc.name: GetPackagesObject_0400
 * @tc.desc: GetPackagesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackagesObject_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "packages": "invalid_array"
    })";
    packInfo.ParseFromString(jsonString);
    std::unique_ptr<AppPackingTool::PtJson> packagesObj;
    EXPECT_FALSE(packInfo.GetPackagesObject(packagesObj));
    EXPECT_EQ(packagesObj, nullptr);
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
 * @tc.name: GetAppObject_0400
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetAppObject_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "summary": {}
    })";
    packInfo.ParseFromString(jsonString);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_FALSE(packInfo.GetAppObject(appObj));
    EXPECT_EQ(appObj, nullptr);
}

/*
 * @tc.name: GetAppObject_0500
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetAppObject_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "summary": {
            "app": "invalid_object"
        }
    })";
    packInfo.ParseFromString(jsonString);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_FALSE(packInfo.GetAppObject(appObj));
    EXPECT_EQ(appObj, nullptr);
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
 * @tc.name: GetModulesObject_0400
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "summary": {}
    })";
    packInfo.ParseFromString(jsonString);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_FALSE(packInfo.GetModulesObject(modulesObj));
    EXPECT_EQ(modulesObj, nullptr);
}

/*
 * @tc.name: GetModulesObject_0500
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string jsonString = R"({
        "summary": {
            "modules": "invalid_array"
        }
    })";
    packInfo.ParseFromString(jsonString);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_FALSE(packInfo.GetModulesObject(modulesObj));
    EXPECT_EQ(modulesObj, nullptr);
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
 * @tc.name: GetExtensionAbilitiesObj_0200
 * @tc.desc: GetExtensionAbilitiesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObj(moduleIndex, extensionAbilitiesObj));
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
 * @tc.name: GetExtensionAbilitiesObjByModulesObj_0200
 * @tc.desc: GetExtensionAbilitiesObjByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModulesObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObjByModulesObj(modulesObj, moduleIndex, extensionAbilitiesObj));
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModulesObj_0300
 * @tc.desc: GetExtensionAbilitiesObjByModulesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModulesObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_MODULES_OBJECT_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    packInfo.GetModulesObject(modulesObj);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObjByModulesObj(modulesObj, moduleIndex, extensionAbilitiesObj));
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModuleObj_0100
 * @tc.desc: GetExtensionAbilitiesObjByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModuleObj_0200
 * @tc.desc: GetExtensionAbilitiesObjByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObjByModuleObj(moduleObj, extensionAbilitiesObj));
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModuleObj_0300
 * @tc.desc: GetExtensionAbilitiesObjByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(EMPTY_MODULES_OBJECT_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
}

/*
 * @tc.name: GetFormsObjByExtensionAbilityObj_0100
 * @tc.desc: GetFormsObjByExtensionAbilityObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormsObjByExtensionAbilityObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
}

/*
 * @tc.name: GetFormsObjByExtensionAbilityObj_0200
 * @tc.desc: GetFormsObjByExtensionAbilityObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormsObjByExtensionAbilityObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_FALSE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj, formsObj));
}

/*
 * @tc.name: GetFormsObjByExtensionAbilityObj_0300
 * @tc.desc: GetFormsObjByExtensionAbilityObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormsObjByExtensionAbilityObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_FALSE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
}

/*
 * @tc.name: GetFormsObjByExtensionAbilityObj_0400
 * @tc.desc: GetFormsObjByExtensionAbilityObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormsObjByExtensionAbilityObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_FALSE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(1), formsObj));
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
 * @tc.name: GetPackageObject_0200
 * @tc.desc: GetPackageObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    int32_t packageIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    EXPECT_FALSE(packInfo.GetPackageObject(packageIndex, packageObj));
}

/*
 * @tc.name: GetPackageObject_0300
 * @tc.desc: GetPackageObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t packageIndex = 5;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    EXPECT_FALSE(packInfo.GetPackageObject(packageIndex, packageObj));
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
 * @tc.name: GetVersion_0200
 * @tc.desc: GetVersion.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoVersion version;
    OHOS::AppPackingTool::PackInfo packInfo;
    EXPECT_FALSE(packInfo.GetVersion(version));
}

/*
 * @tc.name: GetVersionByVersionObj_0100
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_TRUE(packInfo.GetVersionByVersionObj(versionObj, version));
}

/*
 * @tc.name: GetVersionByVersionObj_0200
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_FALSE(packInfo.GetVersionByVersionObj(versionObj, version));
}

/*
 * @tc.name: GetVersionByVersionObj_0300
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_VERSION_CODE_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_FALSE(packInfo.GetVersionByVersionObj(versionObj, version));
}

/*
 * @tc.name: GetVersionByVersionObj_0400
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(ERR_VERSION_CODE_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_FALSE(packInfo.GetVersionByVersionObj(versionObj, version));
}

/*
 * @tc.name: GetVersionByVersionObj_0500
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_VERSION_NAME_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_FALSE(packInfo.GetVersionByVersionObj(versionObj, version));
}

/*
 * @tc.name: GetVersionByVersionObj_0600
 * @tc.desc: GetVersionByVersionObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionByVersionObj_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(ERR_VERSION_NAME_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    OHOS::AppPackingTool::PackInfoVersion version;
    EXPECT_FALSE(packInfo.GetVersionByVersionObj(versionObj, version));
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
 * @tc.name: SetVersionCode_0200
 * @tc.desc: SetVersionCode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    int versionCode = 1;
    EXPECT_FALSE(packInfo.SetVersionCode(versionCode));
}

/*
 * @tc.name: SetVersionCode_0300
 * @tc.desc: SetVersionCode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionCode_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_VERSION_CODE_JSON_STRING);
    int versionCode = 1;
    EXPECT_FALSE(packInfo.SetVersionCode(versionCode));
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
 * @tc.name: SetVersionName_0200
 * @tc.desc: SetVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::string versionName = "2.0.0";
    EXPECT_FALSE(packInfo.SetVersionName(versionName));
}

/*
 * @tc.name: SetVersionName_0300
 * @tc.desc: SetVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_VERSION_NAME_JSON_STRING);
    std::string versionName = "2.0.0";
    EXPECT_FALSE(packInfo.SetVersionName(versionName));
}

/*
 * @tc.name: SetVersionName_0400
 * @tc.desc: SetVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, SetVersionName_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(ERR_VERSION_NAME_JSON_STRING);
    std::string versionName = "2.0.0";
    EXPECT_FALSE(packInfo.SetVersionName(versionName));
}

/*
 * @tc.name: GetModuleNameByDistroObj_0100
 * @tc.desc: GetModuleNameByDistroObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModuleNameByDistroObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
    std::string moduleName = "";
    EXPECT_TRUE(packInfo.GetModuleNameByDistroObj(distroObj, moduleName));
    EXPECT_EQ(moduleName, "entry");
}

/*
 * @tc.name: GetModuleNameByDistroObj_0200
 * @tc.desc: GetModuleNameByDistroObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModuleNameByDistroObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    std::string moduleName = "";
    EXPECT_FALSE(packInfo.GetModuleNameByDistroObj(distroObj, moduleName));
}

/*
 * @tc.name: GetModuleNameByDistroObj_0300
 * @tc.desc: GetModuleNameByDistroObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModuleNameByDistroObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
    std::string moduleName = "";
    EXPECT_TRUE(packInfo.GetModuleNameByDistroObj(distroObj, moduleName));
    EXPECT_EQ(moduleName, "");
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
 * @tc.name: GetNameByPackageObj_0200
 * @tc.desc: GetNameByPackageObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByPackageObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    std::string name = "";
    EXPECT_FALSE(packInfo.GetNameByPackageObj(packageObj, name));
}

/*
 * @tc.name: GetNameByPackageObj_0300
 * @tc.desc: GetNameByPackageObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByPackageObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    int32_t packageIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    packInfo.GetPackageObject(packageIndex, packageObj);
    EXPECT_NE(packageObj, nullptr);
    std::string name = "entry-default";
    EXPECT_TRUE(packInfo.GetNameByPackageObj(packageObj, name));
    EXPECT_EQ(name, "");
}

/*
 * @tc.name: GetNameByFormObj_0100
 * @tc.desc: GetNameByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByFormObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string name = "";
    EXPECT_TRUE(packInfo.GetNameByFormObj(formsObj->Get(0), name));
    EXPECT_EQ(name, "form1");
}

/*
 * @tc.name: GetNameByFormObj_0200
 * @tc.desc: GetNameByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByFormObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    std::string name = "";
    EXPECT_FALSE(packInfo.GetNameByFormObj(formsObj, name));
}

/*
 * @tc.name: GetNameByFormObj_0300
 * @tc.desc: GetNameByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByFormObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string name = "form";
    EXPECT_TRUE(packInfo.GetNameByFormObj(formsObj->Get(1), name));
    EXPECT_EQ(name, "");
}

/*
 * @tc.name: GetDefaultDimensionByFormObj_0100
 * @tc.desc: GetDefaultDimensionByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDefaultDimensionByFormObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string defaultDimension = "";
    EXPECT_TRUE(packInfo.GetDefaultDimensionByFormObj(formsObj->Get(0), defaultDimension));
}

/*
 * @tc.name: GetDefaultDimensionByFormObj_0200
 * @tc.desc: GetDefaultDimensionByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDefaultDimensionByFormObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    std::string defaultDimension = "";
    EXPECT_FALSE(packInfo.GetDefaultDimensionByFormObj(formsObj, defaultDimension));
}

/*
 * @tc.name: GetDefaultDimensionByFormObj_0300
 * @tc.desc: GetDefaultDimensionByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDefaultDimensionByFormObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string defaultDimension = "";
    EXPECT_FALSE(packInfo.GetDefaultDimensionByFormObj(formsObj->Get(2), defaultDimension));
}

/*
 * @tc.name: GetDefaultDimensionByFormObj_0400
 * @tc.desc: GetDefaultDimensionByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDefaultDimensionByFormObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string defaultDimension = "";
    EXPECT_FALSE(packInfo.GetDefaultDimensionByFormObj(formsObj->Get(3), defaultDimension));
}

/*
 * @tc.name: GetSupportDimensionsByFormObj_0100
 * @tc.desc: GetSupportDimensionsByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSupportDimensionsByFormObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::list<std::string> supportDimensions;
    EXPECT_TRUE(packInfo.GetSupportDimensionsByFormObj(formsObj->Get(0), supportDimensions));
}

/*
 * @tc.name: GetSupportDimensionsByFormObj_0200
 * @tc.desc: GetSupportDimensionsByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSupportDimensionsByFormObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    std::list<std::string> supportDimensions;
    EXPECT_FALSE(packInfo.GetSupportDimensionsByFormObj(formsObj, supportDimensions));
}

/*
 * @tc.name: GetSupportDimensionsByFormObj_0300
 * @tc.desc: GetSupportDimensionsByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSupportDimensionsByFormObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::list<std::string> supportDimensions;
    EXPECT_FALSE(packInfo.GetSupportDimensionsByFormObj(formsObj->Get(2), supportDimensions));
}

/*
 * @tc.name: GetSupportDimensionsByFormObj_0400
 * @tc.desc: GetSupportDimensionsByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetSupportDimensionsByFormObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::list<std::string> supportDimensions;
    EXPECT_FALSE(packInfo.GetSupportDimensionsByFormObj(formsObj->Get(3), supportDimensions));
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

/*
 * @tc.name: GetFormNames_0200
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNames(formNames, formFullNames));
}

/*
 * @tc.name: GetFormNames_0300
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_DISTRO_JSON_STRING);
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNames(formNames, formFullNames));
}

/*
 * @tc.name: GetFormNames_0400
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_TRUE(packInfo.GetFormNames(formNames, formFullNames));
}

/*
 * @tc.name: GetFormNames_0500
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_EXTENSION_JSON_STRING);
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNames(formNames, formFullNames));
}

/*
 * @tc.name: GetFormNames_0600
 * @tc.desc: GetFormNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNames_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(NO_FORM_NAME_JSON_STRING);
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNames(formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByExtensionAbilitiesObj_0100
 * @tc.desc: GetFormNamesByExtensionAbilitiesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByExtensionAbilitiesObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    int32_t moduleIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObj(moduleIndex, extensionAbilitiesObj));
    EXPECT_NE(extensionAbilitiesObj, nullptr);
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(
        packInfo.GetFormNamesByExtensionAbilitiesObj(extensionAbilitiesObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByFormsObj_0100
 * @tc.desc: GetFormNamesByFormsObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByFormsObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(2), formsObj));
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_TRUE(packInfo.GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByFormsObj_0200
 * @tc.desc: GetFormNamesByFormsObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByFormsObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByFormsObj_0300
 * @tc.desc: GetFormNamesByFormsObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByFormsObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(3), formsObj));
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByFormsObj_0400
 * @tc.desc: GetFormNamesByFormsObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByFormsObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(COMMON_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(4), formsObj));
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(packInfo.GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetPackageNames_0100
 * @tc.desc: GetPackageNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageNames_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::list<std::string> packageNames;
    EXPECT_TRUE(packInfo.GetPackageNames(packageNames));
}

/*
 * @tc.name: GetPackageNames_0200
 * @tc.desc: GetPackageNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageNames_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::list<std::string> packageNames;
    EXPECT_FALSE(packInfo.GetPackageNames(packageNames));
}

/*
 * @tc.name: GetPackageNamesByPackagesObj_0100
 * @tc.desc: GetPackageNamesByPackagesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageNamesByPackagesObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> packagesObj;
    EXPECT_TRUE(packInfo.GetPackagesObject(packagesObj));
    std::list<std::string> packageNames;
    EXPECT_TRUE(packInfo.GetPackageNamesByPackagesObj(packagesObj, packageNames));
}

/*
 * @tc.name: GetPackageNamesByPackagesObj_0200
 * @tc.desc: GetPackageNamesByPackagesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageNamesByPackagesObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    std::unique_ptr<AppPackingTool::PtJson> packagesObj;
    std::list<std::string> packageNames;
    EXPECT_FALSE(packInfo.GetPackageNamesByPackagesObj(packagesObj, packageNames));
}

/*
 * @tc.name: Release_0100
 * @tc.desc: Release.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, Release_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    ASSERT_TRUE(packInfo.ParseFromString(JSON_STRING));
    EXPECT_NE(packInfo.root_, nullptr);
    packInfo.Release();
    EXPECT_EQ(packInfo.root_, nullptr);
}

/*
 * @tc.name: GetModulesObject_0600
 * @tc.desc: GetModulesObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModulesObject_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_NOT_ARRAY_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> summaryObj;
    EXPECT_FALSE(packInfo.GetModulesObject(summaryObj));
    ASSERT_EQ(summaryObj, nullptr);
}

/*
 * @tc.name: GetBundleNameByAppObj_0400
 * @tc.desc: GetBundleNameByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleNameByAppObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(APP_BUNDLE_NAME_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleName = "";
    EXPECT_FALSE(packInfo.GetBundleNameByAppObj(appObj, bundleName));
}

/*
 * @tc.name: GetBundleTypeByAppObj_0400
 * @tc.desc: GetBundleTypeByAppObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetBundleTypeByAppObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(APP_EMPTY_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    std::string bundleType = "";
    std::string defaultBundleType = "app";
    EXPECT_TRUE(packInfo.GetBundleTypeByAppObj(appObj, bundleType, defaultBundleType));
}

/*
 * @tc.name: GetVersionObject_0400
 * @tc.desc: GetVersionObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetVersionObject_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(APP_VERSION_NOT_OBJECT_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> versionObj;
    EXPECT_FALSE(packInfo.GetVersionObject(versionObj));
    ASSERT_EQ(versionObj, nullptr);
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0400
 * @tc.desc: GetDistroObjectByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDistroObjectByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_DISTRO_NOT_OBJECT_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_FALSE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
}

/*
 * @tc.name: GetExtensionAbilitiesObjByModuleObj_0400
 * @tc.desc: GetExtensionAbilitiesObjByModuleObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetExtensionAbilitiesObjByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_EXTENSION_ABILITIES_NOT_ARRAY_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_FALSE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
}

/*
 * @tc.name: GetModuleNameByDistroObj_0400
 * @tc.desc: GetModuleNameByDistroObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetModuleNameByDistroObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_DISTRO_MODULE_NAME_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> distroObj;
    EXPECT_TRUE(packInfo.GetDistroObjectByModuleObj(modulesObj->Get(0), distroObj));
    std::string moduleName = "";
    EXPECT_FALSE(packInfo.GetModuleNameByDistroObj(distroObj, moduleName));
}

/*
 * @tc.name: GetNameByPackageObj_0400
 * @tc.desc: GetNameByPackageObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByPackageObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(PACKAGES_NAME_NOT_STRING_TEST_JSON_STRING);
    int32_t packageIndex = 0;
    std::unique_ptr<AppPackingTool::PtJson> packageObj;
    packInfo.GetPackageObject(packageIndex, packageObj);
    EXPECT_NE(packageObj, nullptr);
    std::string name = "entry-default";
    EXPECT_FALSE(packInfo.GetNameByPackageObj(packageObj, name));
}

/*
 * @tc.name: GetNameByFormObj_0400
 * @tc.desc: GetNameByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetNameByFormObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_EXTENSION_ABILITIES_FORMS_NAME_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    ASSERT_NE(extensionAbilitiesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    ASSERT_NE(formsObj, nullptr);
    std::string name = "form";
    EXPECT_FALSE(packInfo.GetNameByFormObj(formsObj->Get(0), name));
}

/*
 * @tc.name: GetDefaultDimensionByFormObj_0500
 * @tc.desc: GetDefaultDimensionByFormObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetDefaultDimensionByFormObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_EXTENSION_ABILITIES_FORMS_DEFAULT_DIMENSION_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    ASSERT_NE(extensionAbilitiesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    ASSERT_NE(formsObj, nullptr);
    std::string defaultDimension = "";
    EXPECT_FALSE(packInfo.GetDefaultDimensionByFormObj(formsObj->Get(0), defaultDimension));
}

/*
 * @tc.name: GetFormNamesByExtensionAbilitiesObj_0200
 * @tc.desc: GetFormNamesByExtensionAbilitiesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByExtensionAbilitiesObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_EXTENSION_ABILITIES_FORMS_DEFAULT_DIMENSION_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_FALSE(
        packInfo.GetFormNamesByExtensionAbilitiesObj(extensionAbilitiesObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetFormNamesByFormsObj_0500
 * @tc.desc: GetFormNamesByFormsObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetFormNamesByFormsObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(MODULES_EXTENSION_ABILITIES_FORMS_NAME_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> modulesObj;
    EXPECT_TRUE(packInfo.GetModulesObject(modulesObj));
    ASSERT_NE(modulesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    EXPECT_TRUE(packInfo.GetExtensionAbilitiesObjByModuleObj(modulesObj->Get(0), extensionAbilitiesObj));
    ASSERT_NE(extensionAbilitiesObj, nullptr);
    std::unique_ptr<AppPackingTool::PtJson> formsObj;
    EXPECT_TRUE(packInfo.GetFormsObjByExtensionAbilityObj(extensionAbilitiesObj->Get(0), formsObj));
    std::string moduleName = "";
    std::list<std::string> formNames;
    std::list<std::string> formFullNames;
    EXPECT_TRUE(packInfo.GetFormNamesByFormsObj(formsObj, moduleName, formNames, formFullNames));
}

/*
 * @tc.name: GetPackageNamesByPackagesObj_0300
 * @tc.desc: GetPackageNamesByPackagesObj.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoTest, GetPackageNamesByPackagesObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfo packInfo;
    packInfo.ParseFromString(PACKAGES_NAME_NOT_STRING_TEST_JSON_STRING);
    std::unique_ptr<AppPackingTool::PtJson> packagesObj;
    EXPECT_TRUE(packInfo.GetPackagesObject(packagesObj));
    std::list<std::string> packageNames;
    EXPECT_FALSE(packInfo.GetPackageNamesByPackagesObj(packagesObj, packageNames));
}
} // namespace OHOS
