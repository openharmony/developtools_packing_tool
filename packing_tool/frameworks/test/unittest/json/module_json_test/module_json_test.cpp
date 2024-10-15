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
#include <numeric>
#include <string>

#define private public
#define protected public
#include "module_json.h"
#include "pt_json.h"
#include "log.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
using namespace std;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {
const std::string MODULE_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_TEST_STRING = "{"
    "\"app\": {"
        "\"apiVersion\": {"
        "}"
    "},"
    "\"module\": {"
        "\"requestPermissions\": true,"
        "\"proxyData\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"generateBuildHash\": true"
    "}"
"}";

const std::string MODULE_JSON_TEST_STRING_NOTHING = "{"
"}";

const std::string MODULE_JSON_TEST_STRING_ERROR = "{"
    "\"app\": {"
        "\"bundleType\": \"atomicService\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"descriptionId\": 16777220,"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string METADATA_NAME_TEST_JSON_STRING = "{"
    "\"module\": {"
        "\"metadata\": ["
            "{"
            "}"
        "]"
    "}"
"}";

const std::string METADATA_VALUE_TEST_JSON_STRING = "{"
    "\"module\": {"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\""
            "}"
        "]"
    "}"
"}";

const std::string METADATA_RESOURCE_TEST_JSON_STRING = "{"
    "\"module\": {"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\""
            "}"
        "]"
    "}"
"}";

const std::string DEPENDENCIES_NO_BUNDLE_NAME_JSON_STRING = "{"
    "\"app\": {"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"dependencies\": ["
            "{"
                "\"moduleName\": \"entry_1\""
            "}"
        "]"
    "}"
"}";

const std::string DEPENDENCIES_NO_MODULE_NAME_JSON_STRING = "{"
    "\"app\": {"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\""
            "}"
        "]"
    "}"
"}";

const std::string MODULE_NO_ATOMICSERVICE_JSON_STRING = "{"
    "\"module\": {"
    "}"
"}";

const std::string ATOMICSERVIC_NO_PRELOADS_JSON_STRING = "{"
    "\"module\": {"
        "\"atomicService\": {"
        "}"
    "}"
"}";

const std::string PRELOADS_NO_MODULE_NAME_JSON_STRING = "{"
    "\"module\": {"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"moduleName\": \"test_moduleName\","
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        "}"
    "}"
"}";

const std::string ABILITY_CONTINUE_TYPE_TEST_JSON_STRING = "{"
    "\"module\": {"
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"continueType\":[ \"test_1\", \"test_2\"]"
            "}"
        "]"
    "}"
"}";

const std::string NO_MULTI_APP_MODE_TYPE_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"multiAppMode\": {"
            "\"maxCount\": 9"
        "}"
    "}"
"}";

const std::string NO_MULTI_APP_MODE_NUMBER_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\""
        "}"
    "}"
"}";

const std::string NO_BUNDLE_TYPE_TEST_JSON_STRING = "{"
    "\"app\": {"
    "},"
    "\"module\": {"
        "\"installationFree\": true,"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        "}"
    "}"
"}";

const std::string APP_BUNDLE_TYPE_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleType\": \"test_atomicService\""
    "},"
    "\"module\": {"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        "}"
    "}"
"}";

const std::string NO_ABILITY_NAME_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleType\": \"atomicService\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"installationFree\": true,"
        "\"abilities\": ["
            "{"
            "}"
        "]"
    "}"
"}";

const std::string MODULE_TYPE_ERROE_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleType\": \"atomicService\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"test_entry\","
        "\"installationFree\": true,"
        "\"abilities\": ["
        "]"
    "}"
"}";

const std::string NO_ABILITY_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleType\": \"atomicService\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"installationFree\": true,"
        "\"abilities\": ["
        "]"
    "}"
"}";

const std::string SHARED_TYPE_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleType\": \"shared\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"shared\","
        "\"installationFree\": true"
    "}"
"}";

const std::string NO_ASAN_ENABLED_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"tsanEnabled\": \"test\""
    "}"
"}";

const std::string NO_TSAN_ENABLED_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"asanEnabled\": \"test\""
    "}"
"}";

const std::string NO_BOOL_ASAN_ENABLED_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"tsanEnabled\": \"test\","
        "\"asanEnabled\": \"test\""
    "}"
"}";

const std::string TSAN_ENABLED_TEST_JSON_STRING = "{"
    "\"app\": {"
        "\"tsanEnabled\": true,"
        "\"asanEnabled\": true"
    "}"
"}";
}
class ModuleJsonTest : public testing::Test {
public:
    ModuleJsonTest() {}
    virtual ~ModuleJsonTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ModuleJsonTest::SetUpTestCase() {}

void ModuleJsonTest::TearDownTestCase() {}

void ModuleJsonTest::SetUp() {}

void ModuleJsonTest::TearDown() {}

/*
 * @tc.name: ParseFromString_0100
 * @tc.desc: test parse from string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseFromString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
}

/*
 * @tc.name: ParseFromString_0200
 * @tc.desc: test parse from string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseFromString_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.ParseFromString(""));
}

/*
 * @tc.name: ParseFromFile_0100
 * @tc.desc: test parse from file
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseFromFile_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string test("test_module.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_STRING.c_str(), MODULE_JSON_STRING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(moduleJson.ParseFromFile(test));
        system("rm -f test_module.json");
    }
}

/*
 * @tc.name: ParseFromFile_0200
 * @tc.desc: test parse from file
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseFromFile_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.ParseFromFile(""));
}

/*
 * @tc.name: ToString
 * @tc.desc: test parse from string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ToString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string str =moduleJson.ToString();
    EXPECT_TRUE(!str.empty());
}

/*
 * @tc.name: Release
 * @tc.desc: test release
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, Release_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.root_ == nullptr);
}

/*
 * @tc.name: IsValid
 * @tc.desc: test is valid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.IsValid());
}

/*
 * @tc.name: GetApiVersionObject
 * @tc.desc: test get api version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetApiVersionObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> apiVersion;
    EXPECT_TRUE(moduleJson.GetApiVersionObject(apiVersion));
    EXPECT_NE(apiVersion, nullptr);
}

/*
 * @tc.name: GetApiVersionObject_0200
 * @tc.desc: test get api version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetApiVersionObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> apiVersion;
    EXPECT_FALSE(moduleJson.GetApiVersionObject(apiVersion));
    EXPECT_EQ(apiVersion, nullptr);
}

/*
 * @tc.name: GetApiVersionObject_0300
 * @tc.desc: test get api version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetApiVersionObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    std::unique_ptr<PtJson> apiVersion;
    EXPECT_FALSE(moduleJson.GetApiVersionObject(apiVersion));
    EXPECT_EQ(apiVersion, nullptr);
}

/*
 * @tc.name: GetVersionObject
 * @tc.desc: test get version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVersionObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> versionObj;
    EXPECT_TRUE(moduleJson.GetVersionObject(versionObj));
    EXPECT_NE(versionObj, nullptr);
}

/*
 * @tc.name: GetVersionObject_0200
 * @tc.desc: test get version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVersionObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    std::unique_ptr<PtJson> versionObj;
    EXPECT_FALSE(moduleJson.GetVersionObject(versionObj));
    EXPECT_EQ(versionObj, nullptr);
}

/*
 * @tc.name: GetVersionObject_0300
 * @tc.desc: test get version object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVersionObject_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::unique_ptr<PtJson> versionObj;
    EXPECT_FALSE(moduleJson.GetVersionObject(versionObj));
    EXPECT_EQ(versionObj, nullptr);
}

/*
 * @tc.name: GetFaHapVerifyInfo
 * @tc.desc: test get fa hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    HapVerifyInfo hapVerifyInfo;
    EXPECT_TRUE(moduleJson.GetFaHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetFaHapVerifyInfo
 * @tc.desc: test get fa hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaHapVerifyInfo_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.GetFaHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetFaBundleType
 * @tc.desc: test get fa bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string bundleType = "";
    EXPECT_TRUE(moduleJson.GetFaBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "app");
}

/*
 * @tc.name: GetFaBundleType
 * @tc.desc: test get fa bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string bundleType = "";
    EXPECT_FALSE(moduleJson.GetFaBundleType(bundleType));
}

/*
 * @tc.name: GetDistroObject
 * @tc.desc: test get distro object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDistroObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    unique_ptr<PtJson> distroObj;
    EXPECT_TRUE(moduleJson.GetDistroObject(distroObj));
    EXPECT_NE(distroObj, nullptr);
}

/*
 * @tc.name: GetDistroObject_0200
 * @tc.desc: test get distro object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDistroObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    unique_ptr<PtJson> distroObj;
    EXPECT_FALSE(moduleJson.GetDistroObject(distroObj));
    EXPECT_EQ(distroObj, nullptr);
}

/*
 * @tc.name: GetFaInstallationFree
 * @tc.desc: test get fa installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFree_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool installationFree = true;
    EXPECT_TRUE(moduleJson.GetFaInstallationFree(installationFree));
    EXPECT_EQ(installationFree, false);
}

/*
 * @tc.name: GetFaInstallationFree
 * @tc.desc: test get fa installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFree_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetFaInstallationFree(installationFree));
}

/*
 * @tc.name: GetStageVersion
 * @tc.desc: test get normalize version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    Version version;
    EXPECT_TRUE(moduleJson.GetStageVersion(version));
    EXPECT_EQ(version.versionCode, 1000000);
    EXPECT_STREQ(version.versionName.c_str(), "test_version_name");
    EXPECT_EQ(version.minCompatibleVersionCode, 99);
}

/*
 * @tc.name: SetStageVersionCode
 * @tc.desc: test set stage version code
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t versionCode = 99;
    EXPECT_TRUE(moduleJson.SetStageVersionCode(versionCode));
    Version version;
    EXPECT_TRUE(moduleJson.GetStageVersion(version));
    EXPECT_EQ(version.versionCode, 99);
}

/*
 * @tc.name: SetStageVersionName
 * @tc.desc: test set stage version name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageVersionName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string versionName = "2.2.2";
    EXPECT_TRUE(moduleJson.SetStageVersionName(versionName));
    Version version;
    EXPECT_TRUE(moduleJson.GetStageVersion(version));
    EXPECT_STREQ(version.versionName.c_str(), "2.2.2");
}

/*
 * @tc.name: GetStageModuleApiVersion
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleApiVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    ModuleApiVersion moduleApiVersion;
    EXPECT_TRUE(moduleJson.GetStageModuleApiVersion(moduleApiVersion));
    EXPECT_EQ(moduleApiVersion.compatibleApiVersion, 10);
    EXPECT_EQ(moduleApiVersion.targetApiVersion, 10);
    EXPECT_STREQ(moduleApiVersion.releaseType.c_str(), "Canary");
}

/*
 * @tc.name: GetStageModuleName
 * @tc.desc: test get stage module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string stageModuleName = "";
    EXPECT_TRUE(moduleJson.GetStageModuleName(stageModuleName));
    EXPECT_STREQ(stageModuleName.c_str(), "entry");
}

/*
 * @tc.name: GetStageEntry
 * @tc.desc: test get stage entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageEntry_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> deviceTypes;
    EXPECT_TRUE(moduleJson.GetStageEntry(deviceTypes));
    EXPECT_NE(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageDeviceTypes
 * @tc.desc: test get stage device types
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDeviceTypes_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> deviceTypes;
    EXPECT_TRUE(moduleJson.GetStageDeviceTypes(deviceTypes));
    EXPECT_NE(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    string str = "{"
    "\"distributionFilter\": {"
        "}"
    "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    EXPECT_TRUE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageDistroFilter
 * @tc.desc: test get stage distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDistroFilter_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    DistroFilter distroFilter;
    map<std::string, std::string> resourceMap;
    string str = "{"
    "\"distributionFilter\": {"
        "}"
    "}";
    resourceMap.insert((make_pair("test_resource.json", str)));
    EXPECT_TRUE(moduleJson.GetStageDistroFilter(distroFilter, resourceMap));
}

/*
 * @tc.name: GetStageInstallationFree
 * @tc.desc: test get stage installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageInstallationFree_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool installationFree = false;
    EXPECT_TRUE(moduleJson.GetStageInstallationFree(installationFree));
    EXPECT_EQ(installationFree, true);
}

/*
 * @tc.name: GetStageCompileSdkType
 * @tc.desc: test get stage compilation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string compileSdkType = "";
    EXPECT_TRUE(moduleJson.GetStageCompileSdkType(compileSdkType));
    EXPECT_STREQ(compileSdkType.c_str(), "OpenHarmony");
}

/*
 * @tc.name: GetStageCompileSdkVersion
 * @tc.desc: test get stage compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string compileSdkVersion = "";
    EXPECT_TRUE(moduleJson.GetStageCompileSdkVersion(compileSdkVersion));
    EXPECT_STREQ(compileSdkVersion.c_str(), "test_compileSdkVersion");
}

/*
 * @tc.name: GetStageModuleType
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string moduleType = "";
    EXPECT_TRUE(moduleJson.GetStageModuleType(moduleType));
    EXPECT_STREQ(moduleType.c_str(), "entry");
}

/*
 * @tc.name: GetStageDebug
 * @tc.desc: test get stage debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDebug_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool debug = false;
    EXPECT_TRUE(moduleJson.GetStageDebug(debug));
    EXPECT_EQ(debug, true);
}

/*
 * @tc.name: GetStageAsanEnabled
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageAsanEnabled_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool asanEnabled = false;
    EXPECT_TRUE(moduleJson.GetStageAsanEnabled(asanEnabled));
    EXPECT_EQ(asanEnabled, true);
}

/*
 * @tc.name: GetStageAsanEnabled
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageAsanEnabled_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    bool asanEnabled = true;
    EXPECT_TRUE(moduleJson.GetStageAsanEnabled(asanEnabled));
    EXPECT_EQ(asanEnabled, false);
}

/*
 * @tc.name: GetStageTsanEnabled
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageTsanEnabled_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool tsanEnabled = true;
    EXPECT_TRUE(moduleJson.GetStageTsanEnabled(tsanEnabled));
    EXPECT_EQ(tsanEnabled, false);
}

/*
 * @tc.name: GetStageTsanEnabled
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageTsanEnabled_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    bool tsanEnabled = true;
    EXPECT_TRUE(moduleJson.GetStageTsanEnabled(tsanEnabled));
    EXPECT_EQ(tsanEnabled, false);
}

/*
 * @tc.name: GetStageApiReleaseType
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageApiReleaseType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string apiReleaseType = "";
    EXPECT_TRUE(moduleJson.GetStageApiReleaseType(apiReleaseType));
    EXPECT_STREQ(apiReleaseType.c_str(), "Canary");
}

/*
 * @tc.name: GetStageApiReleaseType
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageApiReleaseType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string apiReleaseType = "test";
    EXPECT_TRUE(moduleJson.GetStageApiReleaseType(apiReleaseType));
    EXPECT_STREQ(apiReleaseType.c_str(), "");
}

/*
 * @tc.name: GetStageCompressNativeLibs
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompressNativeLibs_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool compressNativeLibs = false;
    EXPECT_TRUE(moduleJson.GetStageCompressNativeLibs(compressNativeLibs));
    EXPECT_EQ(compressNativeLibs, true);
}

/*
 * @tc.name: GetStageCompressNativeLibs
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompressNativeLibs_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    bool compressNativeLibs = true;
    EXPECT_TRUE(moduleJson.GetStageCompressNativeLibs(compressNativeLibs));
    EXPECT_EQ(compressNativeLibs, false);
}

/*
 * @tc.name: GetAbilityContinueTypeMap
 * @tc.desc: test get ability continue type map
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityContinueTypeMap_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    EXPECT_TRUE(moduleJson.GetAbilityContinueTypeMap(abilityContinueTypeMap));
    EXPECT_NE(abilityContinueTypeMap.size(), 0);
}

/*
 * @tc.name: GetExtensionAbilityNames
 * @tc.desc: test get extension ability names
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNames_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> extensionAbilityNames;
    EXPECT_TRUE(moduleJson.GetExtensionAbilityNames(extensionAbilityNames));
    EXPECT_NE(extensionAbilityNames.size(), 0);
}

/*
 * @tc.name: GetExtensionAbilityNames_0200
 * @tc.desc: test get extension ability names
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNames_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> extensionAbilityNames;
    EXPECT_FALSE(moduleJson.GetExtensionAbilityNames(extensionAbilityNames));
}

/*
 * @tc.name: IsExistedStageRequestPermissions
 * @tc.desc: test is existed stage request permissions
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageRequestPermissions_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_FALSE(moduleJson.IsExistedStageRequestPermissions());
}

/*
 * @tc.name: IsExistedStageModuleTargetPriority
 * @tc.desc: test is existed stage app target priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageModuleTargetPriority_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.IsExistedStageModuleTargetPriority());
}

/*
 * @tc.name: IsExistedStageAppTargetPriority
 * @tc.desc: test is existed stage app target priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageAppTargetPriority_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.IsExistedStageAppTargetPriority());
}

/*
 * @tc.name: GetFaVersion
 * @tc.desc: test get fa version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    Version version;
    EXPECT_TRUE(moduleJson.GetFaVersion(version));
    EXPECT_EQ(version.versionCode, 666);
    EXPECT_STREQ(version.versionName.c_str(), "test_version");
    EXPECT_EQ(version.minCompatibleVersionCode, 555);
}

/*
 * @tc.name: GetFaVersion
 * @tc.desc: test get fa version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    Version version;
    moduleJson.root_ = nullptr;
    EXPECT_FALSE(moduleJson.GetFaVersion(version));
}

/*
 * @tc.name: SetFaVersionCode
 * @tc.desc: test set fa version code
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t versionCode = 9;
    EXPECT_TRUE(moduleJson.SetFaVersionCode(versionCode));
    Version version;
    EXPECT_TRUE(moduleJson.GetFaVersion(version));
    EXPECT_EQ(version.versionCode, 9);
}

/*
 * @tc.name: SetFaVersionCode
 * @tc.desc: test set fa version code
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    int32_t versionCode = 9;
    EXPECT_FALSE(moduleJson.SetFaVersionCode(versionCode));
}

/*
 * @tc.name: SetFaVersionName
 * @tc.desc: test set fa version name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaVersionName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string versionName = "1.1.1";
    EXPECT_TRUE(moduleJson.SetFaVersionName(versionName));
    Version version;
    EXPECT_TRUE(moduleJson.GetFaVersion(version));
    EXPECT_STREQ(version.versionName.c_str(), "1.1.1");
}

/*
 * @tc.name: SetFaVersionName
 * @tc.desc: test set fa version name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaVersionName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string versionName = "";
    EXPECT_FALSE(moduleJson.SetFaVersionName(versionName));
}

/*
 * @tc.name: GetFaModuleApiVersion
 * @tc.desc: test get fa module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleApiVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    ModuleApiVersion moduleApiVersion;
    EXPECT_TRUE(moduleJson.GetFaModuleApiVersion(moduleApiVersion));
    EXPECT_EQ(moduleApiVersion.compatibleApiVersion, 7);
    EXPECT_EQ(moduleApiVersion.targetApiVersion, 10);
    EXPECT_STREQ(moduleApiVersion.releaseType.c_str(), "test_apiVersion_release");
}

/*
 * @tc.name: GetFaModuleApiVersion
 * @tc.desc: test get fa module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleApiVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetFaModuleApiVersion(moduleApiVersion));
}

/*
 * @tc.name: GetFaModuleName
 * @tc.desc: test get fa module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string faModuleName = "";
    EXPECT_TRUE(moduleJson.GetFaModuleName(faModuleName));
    EXPECT_STREQ(faModuleName.c_str(), "test_module_name");
}

/*
 * @tc.name: GetFaModuleName
 * @tc.desc: test get fa module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string faModuleName = "";
    EXPECT_FALSE(moduleJson.GetFaModuleName(faModuleName));
}

/*
 * @tc.name: GetFaPackageStr
 * @tc.desc: test get fa package string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaPackageStr_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string packageStr = "";
    EXPECT_TRUE(moduleJson.GetFaPackageStr(packageStr));
    EXPECT_STREQ(packageStr.c_str(), "test_package");
}

/*
 * @tc.name: GetFaPackageStr
 * @tc.desc: test get fa package string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaPackageStr_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string packageStr = "";
    EXPECT_FALSE(moduleJson.GetFaPackageStr(packageStr));
}

/*
 * @tc.name: GetFaEntry
 * @tc.desc: test get fa entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaEntry_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> deviceTypes;
    EXPECT_TRUE(moduleJson.GetFaEntry(deviceTypes));
    EXPECT_NE(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetFaEntry
 * @tc.desc: test get fa entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaEntry_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::list<std::string> deviceTypes;
    EXPECT_FALSE(moduleJson.GetFaEntry(deviceTypes));
}

/*
 * @tc.name: GetFaDeviceTypes
 * @tc.desc: test fa deviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDeviceTypes_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> deviceTypes;
    EXPECT_TRUE(moduleJson.GetFaDeviceTypes(deviceTypes));
    EXPECT_NE(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetFaDeviceTypes
 * @tc.desc: test fa deviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDeviceTypes_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::list<std::string> deviceTypes;
    EXPECT_FALSE(moduleJson.GetFaDeviceTypes(deviceTypes));
}

/*
 * @tc.name: GetFaDistroFilter
 * @tc.desc: test get fa distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDistroFilter_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    DistroFilter distroFilter;
    EXPECT_TRUE(moduleJson.GetFaDistroFilter(distroFilter));
}

/*
 * @tc.name: GetFaDistroFilter
 * @tc.desc: test get fa distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDistroFilter_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    DistroFilter distroFilter;
    EXPECT_FALSE(moduleJson.GetFaDistroFilter(distroFilter));
}

/*
 * @tc.name: GetFaCompileSdkType
 * @tc.desc: test get fa compile sdk type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string compileSdkType = "";
    EXPECT_TRUE(moduleJson.GetFaCompileSdkType(compileSdkType));
    EXPECT_STREQ(compileSdkType.c_str(), "OpenHarmony");
}

/*
 * @tc.name: GetFaCompileSdkType
 * @tc.desc: test get fa compile sdk type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string compileSdkType = "test";
    EXPECT_TRUE(moduleJson.GetFaCompileSdkType(compileSdkType));
    EXPECT_STREQ(compileSdkType.c_str(), "");
}

/*
 * @tc.name: GetFaCompileSdkType
 * @tc.desc: test get fa compile sdk type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string compileSdkType = "";
    moduleJson.root_ = nullptr;
    EXPECT_FALSE(moduleJson.GetFaCompileSdkType(compileSdkType));
}

/*
 * @tc.name: GetFaCompileSdkVersion
 * @tc.desc: test get fa compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string compileSdkVersion = "";
    EXPECT_TRUE(moduleJson.GetFaCompileSdkVersion(compileSdkVersion));
    EXPECT_STREQ(compileSdkVersion.c_str(), "test_apiVersion_compileSdkVersion");
}

/*
 * @tc.name: GetFaCompileSdkVersion
 * @tc.desc: test get fa compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string compileSdkVersion = "test";
    EXPECT_TRUE(moduleJson.GetFaCompileSdkVersion(compileSdkVersion));
    EXPECT_STREQ(compileSdkVersion.c_str(), "");
}

/*
 * @tc.name: GetFaCompileSdkVersion
 * @tc.desc: test get fa compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string compileSdkVersion = "";
    EXPECT_FALSE(moduleJson.GetFaCompileSdkVersion(compileSdkVersion));
}

/*
 * @tc.name: GetFaModuleType
 * @tc.desc: test get fa module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string moduleType = "";
    EXPECT_TRUE(moduleJson.GetFaModuleType(moduleType));
    EXPECT_STREQ(moduleType.c_str(), "entry");
}

/*
 * @tc.name: GetFaModuleType
 * @tc.desc: test get fa module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string moduleType = "";
    EXPECT_FALSE(moduleJson.GetFaModuleType(moduleType));
}

/*
 * @tc.name: GetFaDebug
 * @tc.desc: test get fa debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDebug_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool debug = false;
    EXPECT_TRUE(moduleJson.GetFaDebug(debug));
    EXPECT_EQ(debug, true);
}

/*
 * @tc.name: GetFaDebug
 * @tc.desc: test get fa debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDebug_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    bool debug = false;
    EXPECT_FALSE(moduleJson.GetFaDebug(debug));
}

/*
 * @tc.name: GetFaAsanEnabled
 * @tc.desc: test get fa asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaAsanEnabled_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool asanEnabled = false;
    EXPECT_TRUE(moduleJson.GetFaAsanEnabled(asanEnabled));
    EXPECT_EQ(asanEnabled, true);
}

/*
 * @tc.name: GetFaAsanEnabled
 * @tc.desc: test get fa asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaAsanEnabled_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    bool asanEnabled = true;
    EXPECT_TRUE(moduleJson.GetFaAsanEnabled(asanEnabled));
    EXPECT_EQ(asanEnabled, false);
}

/*
 * @tc.name: GetFaAsanEnabled
 * @tc.desc: test get fa asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaAsanEnabled_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    bool asanEnabled = true;
    EXPECT_FALSE(moduleJson.GetFaAsanEnabled(asanEnabled));
}

/*
 * @tc.name: GetFaReleaseType
 * @tc.desc: test get fa release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaReleaseType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string releaseType = "";
    EXPECT_TRUE(moduleJson.GetFaReleaseType(releaseType));
    EXPECT_STREQ(releaseType.c_str(), "test_apiVersion_release");
}

/*
 * @tc.name: GetFaReleaseType
 * @tc.desc: test get fa release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaReleaseType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.root_ = nullptr;
    std::string releaseType = "";
    EXPECT_FALSE(moduleJson.GetFaReleaseType(releaseType));
}

/*
 * @tc.name: GetBundleName
 * @tc.desc: test get bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string bundleName = "";
    EXPECT_TRUE(moduleJson.GetBundleName(bundleName));
    EXPECT_STREQ(bundleName.c_str(), "test_bundle_name");
}

/*
 * @tc.name: GetBundleName_0200
 * @tc.desc: test get bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleName = "";
    EXPECT_FALSE(moduleJson.GetBundleName(bundleName));
}

/*
 * @tc.name: SetBundleName
 * @tc.desc: test set bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string setBundleName = "com.example.test";
    std::string getBundleName = "";
    EXPECT_TRUE(moduleJson.SetBundleName(setBundleName));
    moduleJson.GetBundleName(getBundleName);
    EXPECT_STREQ(getBundleName.c_str(), setBundleName.c_str());
}

/*
 * @tc.name: SetBundleName_0200
 * @tc.desc: test set bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string setBundleName = "com.example.test";
    EXPECT_FALSE(moduleJson.SetBundleName(setBundleName));
}

/*
 * @tc.name: SetBundleName_0300
 * @tc.desc: test set bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetBundleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    std::string setBundleName = "com.example.test";
    std::string getBundleName = "";
    EXPECT_FALSE(moduleJson.SetBundleName(setBundleName));
}

/*
 * @tc.name: GetModuleName
 * @tc.desc: test get module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string moduleName = "";
    EXPECT_TRUE(moduleJson.GetModuleName(moduleName));
    EXPECT_STREQ(moduleName.c_str(), "entry");
}

/*
 * @tc.name: GetModuleName_0200
 * @tc.desc: test get module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    EXPECT_FALSE(moduleJson.GetModuleName(moduleName));
}

/*
 * @tc.name: GetVendor
 * @tc.desc: test get vendor
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVendor_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string vendor = "test";
    EXPECT_TRUE(moduleJson.GetVendor(vendor));
    EXPECT_STREQ(vendor.c_str(), "");
}

/*
 * @tc.name: GetVendor_0200
 * @tc.desc: test get vendor
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVendor_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string vendor = "test";
    EXPECT_FALSE(moduleJson.GetVendor(vendor));
}

/*
 * @tc.name: GetTargetBundleName
 * @tc.desc: test get target bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string targetBundleName = "";
    EXPECT_TRUE(moduleJson.GetTargetBundleName(targetBundleName));
    EXPECT_STREQ(targetBundleName.c_str(), "test_app_targetBundleName");
}

/*
 * @tc.name: GetTargetBundleName
 * @tc.desc: test get target bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string targetBundleName = "test";
    EXPECT_TRUE(moduleJson.GetTargetBundleName(targetBundleName));
    EXPECT_STREQ(targetBundleName.c_str(), "");
}

/*
 * @tc.name: GetTargetBundleName_0300
 * @tc.desc: test get target bundle name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetBundleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string targetBundleName = "test";
    EXPECT_FALSE(moduleJson.GetTargetBundleName(targetBundleName));
}

/*
 * @tc.name: GetTargetModuleName
 * @tc.desc: test get target module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModuleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string targetModuleName = "";
    EXPECT_TRUE(moduleJson.GetTargetModuleName(targetModuleName));
    EXPECT_STREQ(targetModuleName.c_str(), "test_module_targetBundleName");
}

/*
 * @tc.name: GetTargetModuleName
 * @tc.desc: test get target module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModuleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string targetModuleName = "test";
    EXPECT_TRUE(moduleJson.GetTargetModuleName(targetModuleName));
    EXPECT_STREQ(targetModuleName.c_str(), "");
}

/*
 * @tc.name: GetTargetModuleName_0300
 * @tc.desc: test get target module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModuleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string targetModuleName = "test";
    EXPECT_FALSE(moduleJson.GetTargetModuleName(targetModuleName));
}

/*
 * @tc.name: GetTargetPriority
 * @tc.desc: test get target priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetPriority_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t targetPriority = -1;
    EXPECT_TRUE(moduleJson.GetTargetPriority(targetPriority));
    EXPECT_EQ(targetPriority, 5);
}

/*
 * @tc.name: GetTargetPriority
 * @tc.desc: test get target priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetPriority_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    int32_t targetPriority = -1;
    EXPECT_TRUE(moduleJson.GetTargetPriority(targetPriority));
    EXPECT_EQ(targetPriority, 0);
}

/*
 * @tc.name: GetTargetPriority_0300
 * @tc.desc: test get target priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetPriority_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetPriority = -1;
    EXPECT_FALSE(moduleJson.GetTargetPriority(targetPriority));
}

/*
 * @tc.name: GetTargetModulePriority
 * @tc.desc: test target module priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModulePriority_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t targetModulePriority = -1;
    EXPECT_TRUE(moduleJson.GetTargetModulePriority(targetModulePriority));
    EXPECT_EQ(targetModulePriority, 6);
}

/*
 * @tc.name: GetTargetModulePriority
 * @tc.desc: test target module priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModulePriority_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    int32_t targetModulePriority = -1;
    EXPECT_TRUE(moduleJson.GetTargetModulePriority(targetModulePriority));
    EXPECT_EQ(targetModulePriority, 0);
}

/*
 * @tc.name: GetTargetModulePriority_0300
 * @tc.desc: test target module priority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModulePriority_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetModulePriority = -1;
    EXPECT_FALSE(moduleJson.GetTargetModulePriority(targetModulePriority));
}

/*
 * @tc.name: GetAbilityNames
 * @tc.desc: test get ability names
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNames_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> abilityNames;
    EXPECT_TRUE(moduleJson.GetAbilityNames(abilityNames));
    EXPECT_NE(abilityNames.size(), 0);
}

/*
 * @tc.name: GetAbilityNames_0200
 * @tc.desc: test get ability names
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNames_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::list<std::string> abilityNames;
    EXPECT_FALSE(moduleJson.GetAbilityNames(abilityNames));
}

/*
 * @tc.name: GetDependencyItems
 * @tc.desc: test get dependency items
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItems_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "defaultBundleName";
    EXPECT_TRUE(moduleJson.GetDependencyItems(dependencyItems, defaultBundleName));
    EXPECT_NE(dependencyItems.size(), 0);
}

/*
 * @tc.name: GetAtomicServicePreloads
 * @tc.desc: test get atomics service preloads
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloads_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<PreloadItem> preloadItems;
    EXPECT_TRUE(moduleJson.GetAtomicServicePreloads(preloadItems));
    EXPECT_NE(preloadItems.size(), 0);
}

/*
 * @tc.name: GetProxyDataUris
 * @tc.desc: test get proxy data uris
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUris_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<std::string> proxyDataUris;
    EXPECT_TRUE(moduleJson.GetProxyDataUris(proxyDataUris));
    EXPECT_NE(proxyDataUris.size(), 0);
}

/*
 * @tc.name: GetProxyDataUris
 * @tc.desc: test get proxy data uris
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUris_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::list<std::string> proxyDataUris;
    EXPECT_TRUE(moduleJson.GetProxyDataUris(proxyDataUris));
    EXPECT_NE(proxyDataUris.size(), 0);
}

/*
 * @tc.name: GetProxyDataUris_0300
 * @tc.desc: test get proxy data uris
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUris_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> proxyDataUris;
    EXPECT_FALSE(moduleJson.GetProxyDataUris(proxyDataUris));
}

/*
 * @tc.name: GetMultiAppMode
 * @tc.desc: test get multi app mode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetMultiAppMode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    MultiAppMode multiAppMode;
    EXPECT_TRUE(moduleJson.GetMultiAppMode(multiAppMode));
    EXPECT_STREQ(multiAppMode.multiAppModeType.c_str(), "test_multiAppMode");
    EXPECT_EQ(multiAppMode.maxCount, 9);
}

/*
 * @tc.name: IsModuleAtomicServiceValid
 * @tc.desc: test is module atomic service valid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsModuleAtomicServiceValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.IsModuleAtomicServiceValid());
}

/*
 * @tc.name: CheckEntryInAtomicService
 * @tc.desc: test check entry in atomic service
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckEntryInAtomicService_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckEntryInAtomicService());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree
 * @tc.desc: test check atomic service installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: GetGenerateBuildHash
 * @tc.desc: test get generate build hash
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetGenerateBuildHash_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    bool generateBuildHash = false;
    EXPECT_TRUE(moduleJson.GetGenerateBuildHash(generateBuildHash));
    EXPECT_EQ(generateBuildHash, true);
}

/*
 * @tc.name: GetGenerateBuildHash
 * @tc.desc: test get generate build hash
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetGenerateBuildHash_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    bool generateBuildHash = false;
    EXPECT_TRUE(moduleJson.GetGenerateBuildHash(generateBuildHash));
    EXPECT_EQ(generateBuildHash, true);
}

/*
 * @tc.name: RemoveGenerateBuildHash
 * @tc.desc: test remove generate build hash
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, RemoveGenerateBuildHash_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.RemoveGenerateBuildHash());
}

/*
 * @tc.name: RemoveGenerateBuildHashFromAppObj
 * @tc.desc: test remove generate build hash from app object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, RemoveGenerateBuildHashFromAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.RemoveGenerateBuildHashFromAppObj());
}

/*
 * @tc.name: RemoveGenerateBuildHashFromModuleObj
 * @tc.desc: test remove generate build hash from module object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, RemoveGenerateBuildHashFromModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.RemoveGenerateBuildHashFromModuleObj());
}

/*
 * @tc.name: GetNormalizeVersion
 * @tc.desc: test get normalize version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetNormalizeVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    NormalizeVersion normalizeVersion;
    bool isStage = false;
    EXPECT_TRUE(moduleJson.GetNormalizeVersion(normalizeVersion, isStage));
    EXPECT_EQ(normalizeVersion.originVersionCode, 666);
    EXPECT_STREQ(normalizeVersion.originVersionName.c_str(), "test_version");
    EXPECT_STREQ(normalizeVersion.moduleName.c_str(), "entry");
}

/*
 * @tc.name: GetNormalizeVersion
 * @tc.desc: test get normalize version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetNormalizeVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    NormalizeVersion normalizeVersion;
    bool isStage = true;
    EXPECT_TRUE(moduleJson.GetNormalizeVersion(normalizeVersion, isStage));
    EXPECT_EQ(normalizeVersion.originVersionCode, 1000000);
    EXPECT_STREQ(normalizeVersion.originVersionName.c_str(), "test_version_name");
    EXPECT_STREQ(normalizeVersion.moduleName.c_str(), "entry");
}

/*
 * @tc.name: SetVersionCodeAndName
 * @tc.desc: test set version code and name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetVersionCodeAndName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t versionCode = 10;
    std::string versionName = "1.1.1";
    bool isStage = true;
    EXPECT_TRUE(moduleJson.SetVersionCodeAndName(versionCode, versionName, isStage));
    Version version;
    EXPECT_TRUE(moduleJson.GetStageVersion(version));
    EXPECT_EQ(version.versionCode, 10);
    EXPECT_STREQ(version.versionName.c_str(), "1.1.1");
}

/*
 * @tc.name: SetVersionCodeAndName
 * @tc.desc: test set version code and name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetVersionCodeAndName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    int32_t versionCode = 8;
    std::string versionName = "2.2.2";
    bool isStage = false;
    EXPECT_TRUE(moduleJson.SetVersionCodeAndName(versionCode, versionName, isStage));
    Version version;
    EXPECT_TRUE(moduleJson.GetFaVersion(version));
    EXPECT_EQ(version.versionCode, 8);
    EXPECT_STREQ(version.versionName.c_str(), "2.2.2");
}

/*
 * @tc.name: SetBuildHash
 * @tc.desc: test set build hash
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetBuildHash_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string buildHash = "00000000000000000000000000000000";
    EXPECT_TRUE(moduleJson.SetBuildHash(buildHash));
}

/*
 * @tc.name: GetPatchModuleName
 * @tc.desc: test get patch module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetPatchModuleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string patchModuleName = "";
    EXPECT_TRUE(moduleJson.GetPatchModuleName(patchModuleName));
    EXPECT_STREQ(patchModuleName.c_str(), "entry");
}

/*
 * @tc.name: GetPatchModuleName_0200
 * @tc.desc: test get patch module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetPatchModuleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string patchModuleName = "";
    EXPECT_FALSE(moduleJson.GetPatchModuleName(patchModuleName));
}

/*
 * @tc.name: GetPatchModuleName_0300
 * @tc.desc: test get patch module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetPatchModuleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    std::string patchModuleName = "";
    EXPECT_FALSE(moduleJson.GetPatchModuleName(patchModuleName));
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::string bundleType = "";
    EXPECT_TRUE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "atomicService");
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid
 * @tc.desc: test check stage asan tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckStageAsanTsanEnabledValid());
}

/*
 * @tc.name: CheckStageAtomicService
 * @tc.desc: test check stage atomic service
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAtomicService_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckStageAtomicService());
}

/*
 * @tc.name: CheckStageOverlayCfg
 * @tc.desc: test check stage overlay config
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageOverlayCfg_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckStageOverlayCfg());
}

/*
 * @tc.name: GetModuleMetadatas
 * @tc.desc: test get module meta datas
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatas_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    map<std::string, std::string> resourceMap;
    string str = "{"
    "\"distributionFilter\": {"
        "}"
    "}";
    resourceMap.insert((make_pair("test_resource.json", str)));
    EXPECT_TRUE(moduleJson.GetModuleMetadatas(moduleMetadataInfos, resourceMap));
    EXPECT_NE(moduleMetadataInfos.size(), 0);
}

/*
 * @tc.name: GetModuleMetadatas_0200
 * @tc.desc: test get module meta datas
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatas_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    map<std::string, std::string> resourceMap;
    EXPECT_FALSE(moduleJson.GetModuleMetadatas(moduleMetadataInfos, resourceMap));
}

/*
 * @tc.name: SetStageHapVerifyInfoExtByModuleObj
 * @tc.desc: test set stage hap verify info ext by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageHapVerifyInfoExtByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    string str = "{"
    "\"distributionFilter\": {"
        "}"
    "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    EXPECT_TRUE(moduleJson.SetStageHapVerifyInfoExtByModuleObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: GetFaVersionByAppObj
 * @tc.desc: test GetFaVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    Version version;
    moduleObj = nullptr;
    EXPECT_FALSE(moduleJson.GetFaVersionByAppObj(moduleObj, version));
}

/*
 * @tc.name: GetFaVersionByAppObj
 * @tc.desc: test GetFaVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    Version version;
    EXPECT_FALSE(moduleJson.GetFaVersionByAppObj(moduleObj, version));
}

/*
 * @tc.name: GetFaVersionByVersionObj
 * @tc.desc: test GetFaVersionByVersionObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersionByVersionObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    Version version;
    EXPECT_FALSE(moduleJson.GetFaVersionByVersionObj(moduleObj, version));
}

/*
 * @tc.name: GetFaVersionByVersionObj
 * @tc.desc: test GetFaVersionByVersionObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersionByVersionObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    Version version;
    EXPECT_FALSE(moduleJson.GetFaVersionByVersionObj(moduleObj, version));
}

/*
 * @tc.name: GetFaVersionByVersionObj
 * @tc.desc: test GetFaVersionByVersionObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaVersionByVersionObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    Version version;
    EXPECT_FALSE(moduleJson.GetFaVersionByVersionObj(moduleObj, version));
}

/*
 * @tc.name: GetFaInstallationFreeByModuleObj
 * @tc.desc: test GetFaInstallationFreeByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFreeByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    bool installationFree = true;
    EXPECT_TRUE(moduleJson.GetFaInstallationFreeByModuleObj(moduleObj, installationFree));
}

/*
 * @tc.name: GetFaInstallationFreeByModuleObj
 * @tc.desc: test GetFaInstallationFreeByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFreeByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetFaInstallationFreeByModuleObj(moduleObj, installationFree));
}

/*
 * @tc.name: GetFaInstallationFreeByModuleObj
 * @tc.desc: test GetFaInstallationFreeByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFreeByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetFaInstallationFreeByModuleObj(moduleObj, installationFree));
}

/*
 * @tc.name: GetFaInstallationFreeByDistroObj
 * @tc.desc: test GetFaInstallationFreeByDistroObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaInstallationFreeByDistroObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetFaInstallationFreeByDistroObj(moduleObj, installationFree));
}

/*
 * @tc.name: GetFaModuleApiVersionByAppObj
 * @tc.desc: test GetFaModuleApiVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleApiVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetFaModuleApiVersionByAppObj(moduleObj, moduleApiVersion));
}

/*
 * @tc.name: GetFaModuleApiVersionByAppObj
 * @tc.desc: test GetFaModuleApiVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleApiVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetFaModuleApiVersionByAppObj(moduleObj, moduleApiVersion));
}

/*
 * @tc.name: GetFaModuleApiVersionByApiVersionObj
 * @tc.desc: test GetFaModuleApiVersionByApiVersionObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleApiVersionByApiVersionObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetFaModuleApiVersionByApiVersionObj(moduleObj, moduleApiVersion));
}

/*
 * @tc.name: GetFaModuleNameByModuleObj
 * @tc.desc: test GetFaModuleNameByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleNameByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string faModuleName = "";
    EXPECT_FALSE(moduleJson.GetFaModuleNameByModuleObj(moduleObj, faModuleName));
}

/*
 * @tc.name: GetFaModuleNameByModuleObj
 * @tc.desc: test GetFaModuleNameByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleNameByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::string faModuleName = "";
    EXPECT_FALSE(moduleJson.GetFaModuleNameByModuleObj(moduleObj, faModuleName));
}

/*
 * @tc.name: GetFaModuleNameByDistroObj
 * @tc.desc: test GetFaModuleNameByDistroObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleNameByDistroObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string faModuleName = "";
    EXPECT_FALSE(moduleJson.GetFaModuleNameByDistroObj(moduleObj, faModuleName));
}

/*
 * @tc.name: GetFaModuleNameByDistroObj
 * @tc.desc: test GetFaModuleNameByDistroObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleNameByDistroObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::string faModuleName = "";
    EXPECT_FALSE(moduleJson.GetFaModuleNameByDistroObj(moduleObj, faModuleName));
}

/*
 * @tc.name: GetFaPackageStrByModuleObj
 * @tc.desc: test GetFaPackageStrByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaPackageStrByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::string packageStr = "";
    EXPECT_FALSE(moduleJson.GetFaPackageStrByModuleObj(moduleObj, packageStr));
}

/*
 * @tc.name: GetFaPackageStrByModuleObj
 * @tc.desc: test GetFaPackageStrByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaPackageStrByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string packageStr = "";
    EXPECT_FALSE(moduleJson.GetFaPackageStrByModuleObj(moduleObj, packageStr));
}

/*
 * @tc.name: GetFaCompileSdkTypeByAppObj
 * @tc.desc: test GetFaCompileSdkTypeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkTypeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string compileSdkType = "";
    EXPECT_FALSE(moduleJson.GetFaCompileSdkTypeByAppObj(moduleObj, compileSdkType));
}

/*
 * @tc.name: GetFaCompileSdkTypeByAppObj
 * @tc.desc: test GetFaCompileSdkTypeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkTypeByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::string compileSdkType = "";
    EXPECT_FALSE(moduleJson.GetFaCompileSdkTypeByAppObj(moduleObj, compileSdkType));
}

/*
 * @tc.name: GetFaCompileSdkVersionByAppObj
 * @tc.desc: test GetFaCompileSdkVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string compileSdkVersion = "";
    EXPECT_FALSE(moduleJson.GetFaCompileSdkVersionByAppObj(moduleObj, compileSdkVersion));
}

/*
 * @tc.name: GetFaCompileSdkVersionByAppObj
 * @tc.desc: test GetFaCompileSdkVersionByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaCompileSdkVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::string compileSdkVersion = "";
    EXPECT_FALSE(moduleJson.GetFaCompileSdkVersionByAppObj(moduleObj, compileSdkVersion));
}

/*
 * @tc.name: GetFaDebugByDeviceConfigObj
 * @tc.desc: test GetFaDebugByDeviceConfigObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDebugByDeviceConfigObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    bool debug = false;
    EXPECT_FALSE(moduleJson.GetFaDebugByDeviceConfigObj(moduleObj, debug));
}

/*
 * @tc.name: GetFaDebugByDeviceConfigObj
 * @tc.desc: test GetFaDebugByDeviceConfigObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDebugByDeviceConfigObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    bool debug = false;
    EXPECT_FALSE(moduleJson.GetFaDebugByDeviceConfigObj(moduleObj, debug));
}

/*
 * @tc.name: GetFaDeviceTypesByModuleObj
 * @tc.desc: test GetFaDeviceTypesByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDeviceTypesByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::list<std::string> deviceTypes;
    EXPECT_FALSE(moduleJson.GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes));
}

/*
 * @tc.name: GetFaDeviceTypesByModuleObj
 * @tc.desc: test GetFaDeviceTypesByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDeviceTypesByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    std::list<std::string> deviceTypes;
    EXPECT_FALSE(moduleJson.GetFaDeviceTypesByModuleObj(moduleObj, deviceTypes));
}

/*
 * @tc.name: GetFaModuleTypeByModuleObj
 * @tc.desc: test GetFaModuleTypeByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaModuleTypeByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string moduleType = "";
    EXPECT_FALSE(moduleJson.GetFaModuleTypeByModuleObj(moduleObj, moduleType));
}

/*
 * @tc.name: GetFaReleaseTypeByAppObj
 * @tc.desc: test GetFaReleaseTypeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaReleaseTypeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    std::string releaseType = "";
    EXPECT_FALSE(moduleJson.GetFaReleaseTypeByAppObj(moduleObj, releaseType));
}

/*
 * @tc.name: GetFaAsanEnabledByAppObj
 * @tc.desc: test GetFaAsanEnabledByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaAsanEnabledByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    bool asanEnabled = true;
    EXPECT_FALSE(moduleJson.GetFaAsanEnabledByAppObj(moduleObj, asanEnabled));
}

/*
 * @tc.name: GetFaAsanEnabledByAppObj
 * @tc.desc: test GetFaAsanEnabledByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaAsanEnabledByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    bool asanEnabled = true;
    EXPECT_TRUE(moduleJson.GetFaAsanEnabledByAppObj(moduleObj, asanEnabled));
    EXPECT_FALSE(asanEnabled);
}

/*
 * @tc.name: GetFaDistroFilterByModuleObj
 * @tc.desc: test GetFaDistroFilterByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetFaDistroFilterByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    DistroFilter distroFilter;
    EXPECT_FALSE(moduleJson.GetFaDistroFilterByModuleObj(moduleObj, distroFilter));
}

/*
 * @tc.name: SetFaHapVerifyInfoByAppObj
 * @tc.desc: test SetFaHapVerifyInfoByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaHapVerifyInfoByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetFaHapVerifyInfoByAppObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: SetFaHapVerifyInfoByAppObj
 * @tc.desc: test SetFaHapVerifyInfoByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaHapVerifyInfoByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetFaHapVerifyInfoByAppObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: SetFaHapVerifyInfoByModuleObj
 * @tc.desc: test SetFaHapVerifyInfoByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaHapVerifyInfoByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    moduleObj = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetFaHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: SetFaHapVerifyInfoByModuleObj
 * @tc.desc: test SetFaHapVerifyInfoByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetFaHapVerifyInfoByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    moduleObj->object_ = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetFaHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: GetAppObject_0100
 * @tc.desc: test get app object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAppObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string test("test_module.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_STRING.c_str(), MODULE_JSON_STRING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(moduleJson.ParseFromFile(test));
        system("rm -f test_module.json");
    }
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_NE(appObj, nullptr);
}

/*
 * @tc.name: GetAppObject_0200
 * @tc.desc: test get app object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAppObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string test("test_module.json");
    FILE* fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_TEST_STRING_NOTHING.c_str(), MODULE_JSON_TEST_STRING_NOTHING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(moduleJson.ParseFromFile(test));
        system("rm -f test_module.json");
    }
    std::unique_ptr<PtJson> appObj;
    EXPECT_FALSE(moduleJson.GetAppObject(appObj));
    EXPECT_EQ(appObj, nullptr);
}

/*
 * @tc.name: GetAppObject_0300
 * @tc.desc: test get app object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAppObject_0300, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    EXPECT_FALSE(moduleJson.GetAppObject(appObj));
    EXPECT_EQ(appObj, nullptr);
}

/*
 * @tc.name: GetDeviceConfigObject_0100
 * @tc.desc: test get device config object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDeviceConfigObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string test("test_module.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_STRING.c_str(), MODULE_JSON_STRING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(moduleJson.ParseFromFile(test));
        system("rm -f test_module.json");
    }
    std::unique_ptr<PtJson> deviceConfigObj;
    EXPECT_TRUE(moduleJson.GetDeviceConfigObject(deviceConfigObj));
    EXPECT_NE(deviceConfigObj, nullptr);
}

/*
 * @tc.name: GetDeviceConfigObject_0200
 * @tc.desc: test get device config object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDeviceConfigObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string test("test_module.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_TEST_STRING_NOTHING.c_str(), MODULE_JSON_TEST_STRING_NOTHING.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(moduleJson.ParseFromFile(test));
        system("rm -f test_module.json");
    }
    std::unique_ptr<PtJson> deviceConfigObj;
    EXPECT_FALSE(moduleJson.GetDeviceConfigObject(deviceConfigObj));
    EXPECT_EQ(deviceConfigObj, nullptr);
}

/*
 * @tc.name: GetDeviceConfigObject_0300
 * @tc.desc: test get device config object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDeviceConfigObject_0300, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> deviceConfigObj;
    EXPECT_FALSE(moduleJson.GetDeviceConfigObject(deviceConfigObj));
    EXPECT_EQ(deviceConfigObj, nullptr);
}

/*
 * @tc.name: GetModuleObject_0100
 * @tc.desc: test get module object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleObject_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_NE(moduleObj, nullptr);
}

/*
 * @tc.name: GetModuleObject_0200
 * @tc.desc: test get module object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleObject_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_FALSE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_EQ(moduleObj, nullptr);
}

/*
 * @tc.name: GetModuleObject_0300
 * @tc.desc: test get module object
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleObject_0300, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_FALSE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_EQ(moduleObj, nullptr);
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0100
 * @tc.desc: test get distro object by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDistroObjectByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> distroObj;
    EXPECT_FALSE(moduleJson.GetDistroObjectByModuleObj(moduleObj, distroObj));
}

/*
 * @tc.name: GetDistroObjectByModuleObj_0200
 * @tc.desc: test get distro object by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDistroObjectByModuleObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::unique_ptr<PtJson> distroObj;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    moduleJson.GetModuleObject(moduleObj);
    EXPECT_FALSE(moduleJson.GetDistroObjectByModuleObj(moduleObj, distroObj));
}

/*
 * @tc.name: GetModuleNameByModuleObj_0100
 * @tc.desc: test get module name by moduleo bj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleNameByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::string moduleName;
    EXPECT_FALSE(moduleJson.GetModuleNameByModuleObj(moduleObj, moduleName));
}

/*
 * @tc.name: GetModuleNameByModuleObj_0200
 * @tc.desc: test get module name by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleNameByModuleObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::string moduleName;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    moduleJson.GetModuleObject(moduleObj);
    EXPECT_FALSE(moduleJson.GetModuleNameByModuleObj(moduleObj, moduleName));
}

/*
 * @tc.name: GetBundleNameByAppObj_0100
 * @tc.desc: test get bundle name by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetBundleNameByAppObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    std::string bundleName;
    EXPECT_FALSE(moduleJson.GetBundleNameByAppObj(appObj, bundleName));
}

/*
 * @tc.name: GetBundleNameByAppObj_0200
 * @tc.desc: test get bundle name by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetBundleNameByAppObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    std::string bundleName;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    moduleJson.GetModuleObject(appObj);
    EXPECT_FALSE(moduleJson.GetBundleNameByAppObj(appObj, bundleName));
}

/*
 * @tc.name: SetStageVersionCode
 * @tc.desc: test set stage version code
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    int32_t versionCode = 100000;
    EXPECT_FALSE(moduleJson.SetStageVersionCode(versionCode));
}

/*
 * @tc.name: SetStageVersionName
 * @tc.desc: test set stage version name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageVersionName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    std::string versionName = "1.0.0";
    EXPECT_FALSE(moduleJson.SetStageVersionName(versionName));
}

/*
 * @tc.name: GetStageVersion
 * @tc.desc: test get stage version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersion(version));
}

/*
 * @tc.name: GetStageModuleApiVersion
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleApiVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersion(moduleApiVersion));
}

/*
 * @tc.name: GetStageCompileSdkType
 * @tc.desc: test get stage compilation
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    std::string compileSdkType = "";
    EXPECT_FALSE(moduleJson.GetStageCompileSdkType(compileSdkType));
}

/*
 * @tc.name: GetStageCompileSdkVersion
 * @tc.desc: test get stage compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    std::string compileSdkVersion = "";
    EXPECT_FALSE(moduleJson.GetStageCompileSdkVersion(compileSdkVersion));
}

/*
 * @tc.name: GetStageDebug
 * @tc.desc: test get stage debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDebug_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    bool debug = true;
    EXPECT_FALSE(moduleJson.GetStageDebug(debug));
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    std::string bundleType = "";
    EXPECT_FALSE(moduleJson.GetStageBundleType(bundleType));
}

/*
 * @tc.name: GetStageAsanEnabled
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageAsanEnabled_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    bool asanEnabled = true;
    EXPECT_FALSE(moduleJson.GetStageAsanEnabled(asanEnabled));
}

/*
 * @tc.name: GetStageTsanEnabled
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageTsanEnabled_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    bool tsanEnabled = true;
    EXPECT_FALSE(moduleJson.GetStageTsanEnabled(tsanEnabled));
}

/*
 * @tc.name: GetStageCompressNativeLibs
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompressNativeLibs_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    bool compressNativeLibs = true;
    EXPECT_FALSE(moduleJson.GetStageCompressNativeLibs(compressNativeLibs));
}

/*
 * @tc.name: GetStageApiReleaseType
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageApiReleaseType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    std::string apiReleaseType = "";
    EXPECT_FALSE(moduleJson.GetStageApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageHapVerifyInfo_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageVersionByAppObj
 * @tc.desc: test get stage version by obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersionByAppObj(appObj, version));
}

/*
 * @tc.name: GetStageModuleApiVersionByAppObj
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleApiVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion));
}

/*
 * @tc.name: GetStageCompileSdkTypeByAppObj
 * @tc.desc: test get stage compile sdk type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkTypeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string compileSdkType = "";
    EXPECT_FALSE(moduleJson.GetStageCompileSdkTypeByAppObj(appObj, compileSdkType));
}

/*
 * @tc.name: GetStageCompileSdkVersionByAppObj
 * @tc.desc: test get stage compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompileSdkVersionByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string compileSdkVersion = "";
    EXPECT_FALSE(moduleJson.GetStageCompileSdkVersionByAppObj(appObj, compileSdkVersion));
}

/*
 * @tc.name: GetStageDebugByAppObj
 * @tc.desc: test get stage debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDebugByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool debug = true;
    EXPECT_FALSE(moduleJson.GetStageDebugByAppObj(appObj, debug));
}

/*
 * @tc.name: GetStageAsanEnabledByAppObj
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageAsanEnabledByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool asanEnabled = true;
    EXPECT_FALSE(moduleJson.GetStageAsanEnabledByAppObj(appObj, asanEnabled));
}

/*
 * @tc.name: GetStageTsanEnabledByAppObj
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageTsanEnabledByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool tsanEnabled = true;
    EXPECT_FALSE(moduleJson.GetStageTsanEnabledByAppObj(appObj, tsanEnabled));
}

/*
 * @tc.name: GetStageCompressNativeLibsByAppObj
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageCompressNativeLibsByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool compressNativeLibs = true;
    EXPECT_FALSE(moduleJson.GetStageCompressNativeLibsByAppObj(appObj, compressNativeLibs));
}

/*
 * @tc.name: GetStageApiReleaseTypeByAppObj
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageApiReleaseTypeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string apiReleaseType = "";
    EXPECT_FALSE(moduleJson.GetStageApiReleaseTypeByAppObj(appObj, apiReleaseType));
}

/*
 * @tc.name: SetStageHapVerifyInfoByAppObj
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageHapVerifyInfoByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetStageHapVerifyInfoByAppObj(appObj, hapVerifyInfo));
}

/*
 * @tc.name: GetStageInstallationFreeByModuleObj
 * @tc.desc: test get stage installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageInstallationFreeByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetStageInstallationFreeByModuleObj(moduleObj, installationFree));
}

/*
 * @tc.name: GetStageModuleNameByModuleObj
 * @tc.desc: test get stage module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleNameByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string stageModuleName = "";
    EXPECT_FALSE(moduleJson.GetStageModuleNameByModuleObj(moduleObj, stageModuleName));
}

/*
 * @tc.name: GetStageDeviceTypesByModuleObj
 * @tc.desc: test get stage device types
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageDeviceTypesByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::list<std::string> deviceTypes;
    EXPECT_FALSE(moduleJson.GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes));
}

/*
 * @tc.name: GetStageModuleTypeByModuleObj
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetStageModuleTypeByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string moduleType = "";
    EXPECT_FALSE(moduleJson.GetStageModuleTypeByModuleObj(moduleObj, moduleType));
}

/*
 * @tc.name: SetStageHapVerifyInfoByModuleObj
 * @tc.desc: test set stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageHapVerifyInfoByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetStageHapVerifyInfoByModuleObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: SetStageHapVerifyInfoExtByModuleObj
 * @tc.desc: test set stage hap verify info ext
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, SetStageHapVerifyInfoExtByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    HapVerifyInfo hapVerifyInfo;
    EXPECT_FALSE(moduleJson.SetStageHapVerifyInfoExtByModuleObj(moduleObj, hapVerifyInfo));
}

/*
 * @tc.name: GetVendorByAppObj_0100
 * @tc.desc: test get vendor by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVendorByAppObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    std::string vendor;
    EXPECT_FALSE(moduleJson.GetVendorByAppObj(appObj, vendor));
}

/*
 * @tc.name: GetVendorByAppObj_0200
 * @tc.desc: test get vendor by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetVendorByAppObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    std::string vendor = "test";
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    moduleJson.GetModuleObject(appObj);
    EXPECT_TRUE(moduleJson.GetVendorByAppObj(appObj, vendor));
    EXPECT_STREQ(vendor.c_str(), "");
}

/*
 * @tc.name: GetTargetBundleNameByAppObj_0100
 * @tc.desc: test get target bundle name by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetBundleNameByAppObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    std::string targetBundleName;
    EXPECT_FALSE(moduleJson.GetTargetBundleNameByAppObj(appObj, targetBundleName));
}

/*
 * @tc.name: GetTargetPriorityByAppObj_0100
 * @tc.desc: test get target priority by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetPriorityByAppObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    int32_t targetPriority = -1;
    EXPECT_FALSE(moduleJson.GetTargetPriorityByAppObj(appObj, targetPriority));
}

/*
 * @tc.name: GetTargetModuleNameByModuleObj_0100
 * @tc.desc: test get target module name by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModuleNameByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::string targetModuleName;
    EXPECT_FALSE(moduleJson.GetTargetModuleNameByModuleObj(moduleObj, targetModuleName));
}

/*
 * @tc.name: GetTargetModulePriorityByModuleObj_0100
 * @tc.desc: test get target priority by app obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetTargetModulePriorityByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    int32_t targetModulePriority;
    EXPECT_FALSE(moduleJson.GetTargetModulePriorityByModuleObj(moduleObj, targetModulePriority));
}

/*
 * @tc.name: GetModuleMetadataInfoByModuleMetadataInfoObj_0100
 * @tc.desc: test get module metadata info by module metadata info obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadataInfoByModuleMetadataInfoObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleMetadataInfoObj;
    const std::map<std::string, std::string> resourceMap;
    ModuleMetadataInfo moduleMetadataInfo;
    EXPECT_FALSE(moduleJson.GetModuleMetadataInfoByModuleMetadataInfoObj(
        moduleMetadataInfoObj, resourceMap, moduleMetadataInfo));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0100
 * @tc.desc: test get module metadata by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    const std::map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_FALSE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: ParseModuleMetadatasToDistroFilter_0100
 * @tc.desc: test parse module metadata to distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseModuleMetadatasToDistroFilter_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    ModuleMetadataInfo metadataInfo;
    moduleMetadataInfos.push_back(metadataInfo);
    DistroFilter distroFilter;
    EXPECT_TRUE(moduleJson.ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter));
}

/*
 * @tc.name: ParseModuleMetadatasToDistroFilter_0200
 * @tc.desc: test parse module metadata to distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, ParseModuleMetadatasToDistroFilter_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    ModuleMetadataInfo metadataInfo;
    metadataInfo.resource = "test";
    moduleMetadataInfos.push_back(metadataInfo);
    DistroFilter distroFilter;
    EXPECT_FALSE(moduleJson.ParseModuleMetadatasToDistroFilter(moduleMetadataInfos, distroFilter));
}

/*
 * @tc.name: GetAbilityNamesByAbilitiesObj_0100
 * @tc.desc: test get ability names by abilities obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNamesByAbilitiesObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> abilitiesObj;
    std::list<std::string> abilityNames;
    EXPECT_FALSE(moduleJson.GetAbilityNamesByAbilitiesObj(abilitiesObj, abilityNames));
}

/*
 * @tc.name: GetAbilityNamesByModuleObj_0100
 * @tc.desc: test get ability names by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNamesByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::list<std::string> abilityNames;
    EXPECT_FALSE(moduleJson.GetAbilityNamesByModuleObj(moduleObj, abilityNames));
}

/*
 * @tc.name: GetAbilityNamesByAbilitiesObj_0200
 * @tc.desc: test get ability names by module obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNamesByAbilitiesObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    std::unique_ptr<PtJson> moduleObj;
    moduleJson.GetModuleObject(moduleObj);
    std::list<std::string> abilityNames;
    EXPECT_TRUE(moduleJson.GetAbilityNamesByModuleObj(moduleObj, abilityNames));
}

/*
 * @tc.name: GetProxyDataUrisByModuleObj_0100
 * @tc.desc: test GetProxyDataUrisByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUrisByModuleObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::list<std::string> proxyDataUris;
    EXPECT_FALSE(moduleJson.GetProxyDataUrisByModuleObj(moduleObj, proxyDataUris));
}

/*
 * @tc.name: GetProxyDataUrisByProxyDatasObj_0100
 * @tc.desc: test GetProxyDataUrisByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUrisByProxyDatasObj_0100, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> proxyDatasObj;
    std::list<std::string> proxyDataUris;
    EXPECT_FALSE(moduleJson.GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris));
}

/*
 * @tc.name: GetProxyDataUrisByProxyDatasObj_0200
 * @tc.desc: test GetProxyDataUrisByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetProxyDataUrisByProxyDatasObj_0200, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    std::unique_ptr<PtJson> proxyDatasObj = PtJson::CreateArray();
    std::unique_ptr<PtJson> testJson = PtJson::CreateObject();
    proxyDatasObj->Push(testJson);
    std::list<std::string> proxyDataUris;
    EXPECT_FALSE(moduleJson.GetProxyDataUrisByProxyDatasObj(proxyDatasObj, proxyDataUris));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0200
 * @tc.desc: test GetModuleMetadatasByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_TRUE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0300
 * @tc.desc: test GetModuleMetadatasByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_FALSE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0400
 * @tc.desc: test GetModuleMetadatasByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(METADATA_NAME_TEST_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_TRUE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0500
 * @tc.desc: test GetModuleMetadatasByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(METADATA_VALUE_TEST_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_TRUE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: GetModuleMetadatasByModuleObj_0600
 * @tc.desc: test GetModuleMetadatasByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetModuleMetadatasByModuleObj_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(METADATA_RESOURCE_TEST_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    map<std::string, std::string> resourceMap;
    std::list<ModuleMetadataInfo> moduleMetadataInfos;
    EXPECT_TRUE(moduleJson.GetModuleMetadatasByModuleObj(moduleObj, resourceMap, moduleMetadataInfos));
}

/*
 * @tc.name: GetExtensionAbilityNamesByExtensionAbilityObj_0100
 * @tc.desc: test GetExtensionAbilityNamesByExtensionAbilityObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNamesByExtensionAbilityObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> extensionAbilitiesObj;
    std::list<std::string> extensionAbilityNames;
    EXPECT_FALSE(
        moduleJson.GetExtensionAbilityNamesByExtensionAbilityObj(extensionAbilitiesObj, extensionAbilityNames));
}

/*
 * @tc.name: GetExtensionAbilityNamesByModuleObj_0100
 * @tc.desc: test GetExtensionAbilityNamesByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNamesByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::list<std::string> extensionAbilityNames;
    EXPECT_FALSE(moduleJson.GetExtensionAbilityNamesByModuleObj(moduleObj, extensionAbilityNames));
}

/*
 * @tc.name: GetExtensionAbilityNamesByModuleObj_0200
 * @tc.desc: test GetExtensionAbilityNamesByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNamesByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::list<std::string> extensionAbilityNames;
    EXPECT_FALSE(moduleJson.GetExtensionAbilityNamesByModuleObj(moduleObj, extensionAbilityNames));
}

/*
 * @tc.name: GetExtensionAbilityNamesByModuleObj_0300
 * @tc.desc: test GetExtensionAbilityNamesByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetExtensionAbilityNamesByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<std::string> extensionAbilityNames;
    EXPECT_TRUE(moduleJson.GetExtensionAbilityNamesByModuleObj(moduleObj, extensionAbilityNames));
}

/*
 * @tc.name: GetDependencyItems_0200
 * @tc.desc: test GetDependencyItems
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItems_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "";
    EXPECT_FALSE(moduleJson.GetDependencyItems(dependencyItems, defaultBundleName));
}

/*
 * @tc.name: GetDependencyItemsByModuleObj_0100
 * @tc.desc: test GetDependencyItemsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItemsByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "";
    EXPECT_FALSE(moduleJson.GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName));
}

/*
 * @tc.name: GetDependencyItemsByModuleObj_0200
 * @tc.desc: test GetDependencyItemsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItemsByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(DEPENDENCIES_NO_BUNDLE_NAME_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "";
    EXPECT_TRUE(moduleJson.GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName));
}

/*
 * @tc.name: GetDependencyItemsByModuleObj_0300
 * @tc.desc: test GetDependencyItemsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItemsByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(DEPENDENCIES_NO_BUNDLE_NAME_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "";
    EXPECT_TRUE(moduleJson.GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName));
}

/*
 * @tc.name: GetDependencyItemsByModuleObj_0400
 * @tc.desc: test GetDependencyItemsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyItemsByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(DEPENDENCIES_NO_MODULE_NAME_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<DependencyItem> dependencyItems;
    std::string defaultBundleName = "";
    EXPECT_TRUE(moduleJson.GetDependencyItemsByModuleObj(moduleObj, dependencyItems, defaultBundleName));
}

/*
 * @tc.name: GetDependencyBundleNameByDependencyItemObj_0100
 * @tc.desc: test GetDependencyBundleNameByDependencyItemObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyBundleNameByDependencyItemObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> dependencyItemObj;
    std::string bundleName = "";
    std::string defaultBundleName = "";
    EXPECT_FALSE(
        moduleJson.GetDependencyBundleNameByDependencyItemObj(dependencyItemObj, bundleName, defaultBundleName));
}

/*
 * @tc.name: GetDependencyModuleNameByDependencyItemObj_0100
 * @tc.desc: test GetDependencyModuleNameByDependencyItemObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetDependencyModuleNameByDependencyItemObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> dependencyItemObj;
    std::string moduleName = "";
    EXPECT_FALSE(moduleJson.GetDependencyModuleNameByDependencyItemObj(dependencyItemObj, moduleName));
}

/*
 * @tc.name: CheckStageBundleType_0100
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "app";
    bool installationFree = false;
    EXPECT_TRUE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0200
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "app";
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0300
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "";
    bool installationFree = false;
    EXPECT_FALSE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0400
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "atomicService";
    bool installationFree = false;
    EXPECT_FALSE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0500
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "shared";
    std::string bundleType = "shared";
    bool installationFree = false;
    EXPECT_TRUE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0600
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "shared";
    bool installationFree = false;
    EXPECT_FALSE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: CheckStageBundleType_0700
 * @tc.desc: test CheckStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageBundleType_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleName = "";
    std::string moduleType = "";
    std::string bundleType = "appService";
    bool installationFree = false;
    EXPECT_TRUE(moduleJson.CheckStageBundleType(moduleName, moduleType, bundleType, installationFree));
}

/*
 * @tc.name: GetAtomicServicePreloads_0200
 * @tc.desc: test GetAtomicServicePreloads
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloads_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<PreloadItem> preloadItems;
    EXPECT_FALSE(moduleJson.GetAtomicServicePreloads(preloadItems));
}

/*
 * @tc.name: GetAtomicServicePreloadsByModuleObj_0100
 * @tc.desc: test GetAtomicServicePreloadsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloadsByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    std::list<PreloadItem> preloadItems;
    EXPECT_FALSE(moduleJson.GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems));
}

/*
 * @tc.name: GetAtomicServicePreloadsByModuleObj_0200
 * @tc.desc: test GetAtomicServicePreloadsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloadsByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_NO_ATOMICSERVICE_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<PreloadItem> preloadItems;
    EXPECT_TRUE(moduleJson.GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems));
}

/*
 * @tc.name: GetAtomicServicePreloadsByModuleObj_0300
 * @tc.desc: test GetAtomicServicePreloadsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloadsByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(ATOMICSERVIC_NO_PRELOADS_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<PreloadItem> preloadItems;
    EXPECT_TRUE(moduleJson.GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems));
}

/*
 * @tc.name: GetAtomicServicePreloadsByModuleObj_0400
 * @tc.desc: test GetAtomicServicePreloadsByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAtomicServicePreloadsByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(PRELOADS_NO_MODULE_NAME_JSON_STRING));
    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::list<PreloadItem> preloadItems;
    EXPECT_TRUE(moduleJson.GetAtomicServicePreloadsByModuleObj(moduleObj, preloadItems));
}

/*
 * @tc.name: GetAbilityContinueTypeMap_0200
 * @tc.desc: test GetAbilityContinueTypeMap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityContinueTypeMap_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    EXPECT_FALSE(moduleJson.GetAbilityContinueTypeMap(abilityContinueTypeMap));
}

/*
 * @tc.name: GetAbilityNameByAbilityObj_0100
 * @tc.desc: test GetAbilityNameByAbilityObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityNameByAbilityObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> abilityObj;
    std::string abilityName = "";
    EXPECT_FALSE(moduleJson.GetAbilityNameByAbilityObj(abilityObj, abilityName));
}

/*
 * @tc.name: GetContinueTypesByAbilityObj_0100
 * @tc.desc: test GetContinueTypesByAbilityObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetContinueTypesByAbilityObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> abilityObj;
    std::list<std::string> continueTypes;
    EXPECT_FALSE(moduleJson.GetContinueTypesByAbilityObj(abilityObj, continueTypes));
}

/*
 * @tc.name: GetAbilityContinueTypeMapByModuleObj_0100
 * @tc.desc: test GetAbilityContinueTypeMapByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityContinueTypeMapByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj;
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    EXPECT_FALSE(moduleJson.GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap));
}

/*
 * @tc.name: GetAbilityContinueTypeMapByModuleObj_0200
 * @tc.desc: test GetAbilityContinueTypeMapByModuleObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetAbilityContinueTypeMapByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(ABILITY_CONTINUE_TYPE_TEST_JSON_STRING));
    std::unique_ptr<PtJson> moduleObj;
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    std::map<std::string, std::list<std::string>> abilityContinueTypeMap;
    EXPECT_TRUE(moduleJson.GetAbilityContinueTypeMapByModuleObj(moduleObj, abilityContinueTypeMap));
}

/*
 * @tc.name: GetMultiAppMode_0200
 * @tc.desc: test GetMultiAppMode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetMultiAppMode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    MultiAppMode multiAppMode;
    EXPECT_FALSE(moduleJson.GetMultiAppMode(multiAppMode));
}

/*
 * @tc.name: GetMultiAppModeByAppObj_0100
 * @tc.desc: test GetMultiAppModeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetMultiAppModeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    MultiAppMode multiAppMode;
    EXPECT_FALSE(moduleJson.GetMultiAppModeByAppObj(appObj, multiAppMode));
}

/*
 * @tc.name: GetMultiAppModeByAppObj_0200
 * @tc.desc: test GetMultiAppModeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetMultiAppModeByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_MULTI_APP_MODE_TYPE_TEST_JSON_STRING));
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    MultiAppMode multiAppMode;
    EXPECT_TRUE(moduleJson.GetMultiAppModeByAppObj(appObj, multiAppMode));
}

/*
 * @tc.name: GetMultiAppModeByAppObj_0300
 * @tc.desc: test GetMultiAppModeByAppObj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, GetMultiAppModeByAppObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_MULTI_APP_MODE_NUMBER_TEST_JSON_STRING));
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    MultiAppMode multiAppMode;
    EXPECT_TRUE(moduleJson.GetMultiAppModeByAppObj(appObj, multiAppMode));
}

/*
 * @tc.name: IsExistedStageRequestPermissions_0200
 * @tc.desc: test IsExistedStageRequestPermissions
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageRequestPermissions_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.IsExistedStageRequestPermissions());
}

/*
 * @tc.name: IsExistedStageModuleTargetPriority_0200
 * @tc.desc: test IsExistedStageModuleTargetPriority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageModuleTargetPriority_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.IsExistedStageModuleTargetPriority());
}

/*
 * @tc.name: IsExistedStageAppTargetPriority_0200
 * @tc.desc: test IsExistedStageAppTargetPriority
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsExistedStageAppTargetPriority_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.IsExistedStageAppTargetPriority());
}

/*
 * @tc.name: IsModuleAtomicServiceValid_0200
 * @tc.desc: test IsModuleAtomicServiceValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsModuleAtomicServiceValid_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.IsModuleAtomicServiceValid());
}

/*
 * @tc.name: IsModuleAtomicServiceValid_0300
 * @tc.desc: test IsModuleAtomicServiceValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsModuleAtomicServiceValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_ABILITY_NAME_TEST_JSON_STRING));
    EXPECT_TRUE(moduleJson.IsModuleAtomicServiceValid());
}

/*
 * @tc.name: IsModuleAtomicServiceValid_0400
 * @tc.desc: test IsModuleAtomicServiceValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsModuleAtomicServiceValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_BUNDLE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.IsModuleAtomicServiceValid());
}

/*
 * @tc.name: IsModuleAtomicServiceValid_0500
 * @tc.desc: test IsModuleAtomicServiceValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, IsModuleAtomicServiceValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(APP_BUNDLE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.IsModuleAtomicServiceValid());
}

/*
 * @tc.name: CheckEntryInAtomicService_0200
 * @tc.desc: test CheckEntryInAtomicService
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckEntryInAtomicService_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.CheckEntryInAtomicService());
}

/*
 * @tc.name: CheckEntryInAtomicService_0300
 * @tc.desc: test CheckEntryInAtomicService
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckEntryInAtomicService_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_ABILITY_NAME_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckEntryInAtomicService());
}

/*
 * @tc.name: CheckEntryInAtomicService_0400
 * @tc.desc: test CheckEntryInAtomicService
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckEntryInAtomicService_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_TYPE_ERROE_TEST_JSON_STRING));
    EXPECT_TRUE(moduleJson.CheckEntryInAtomicService());
}

/*
 * @tc.name: CheckEntryInAtomicService_0500
 * @tc.desc: test CheckEntryInAtomicService
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckEntryInAtomicService_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_ABILITY_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckEntryInAtomicService());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0200
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0300
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_MULTI_APP_MODE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0400
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(ABILITY_CONTINUE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0500
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_BUNDLE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0600
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(APP_BUNDLE_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckAtomicServiceInstallationFree_0700
 * @tc.desc: test CheckAtomicServiceInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckAtomicServiceInstallationFree_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(SHARED_TYPE_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckAtomicServiceInstallationFree());
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid_0200
 * @tc.desc: test CheckStageAsanTsanEnabledValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_FALSE(moduleJson.CheckStageAsanTsanEnabledValid());
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid_0300
 * @tc.desc: test CheckStageAsanTsanEnabledValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_BOOL_ASAN_ENABLED_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckStageAsanTsanEnabledValid());
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid_0400
 * @tc.desc: test CheckStageAsanTsanEnabledValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_ASAN_ENABLED_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckStageAsanTsanEnabledValid());
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid_0500
 * @tc.desc: test CheckStageAsanTsanEnabledValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(NO_TSAN_ENABLED_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckStageAsanTsanEnabledValid());
}

/*
 * @tc.name: CheckStageAsanTsanEnabledValid_0600
 * @tc.desc: test CheckStageAsanTsanEnabledValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonTest, CheckStageAsanTsanEnabledValid_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromString(TSAN_ENABLED_TEST_JSON_STRING));
    EXPECT_FALSE(moduleJson.CheckStageAsanTsanEnabledValid());
}
}
