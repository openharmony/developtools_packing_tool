/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "module_json.h"
#include "pt_json.h"
#include "log.h"

using namespace testing;
using namespace testing::ext;
using namespace std;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {
const std::string MODULE_JSON_TEST_STRING = "{"
"\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
        "},"
        "\"apiVersion\": {"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"distro\": {"
        "}"
    "}"
"}";

const std::string MODULE_JSON_TEST_STRING_NOTHING = "{"
"}";

const std::string MODULE_JSON_TEST_ONLYAPP = "{"
"\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
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
    "}"
"}";

const std::string MODULE_JSON_ERROR_APIRELEASETYPE = "{"
"\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": true,"
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
    "}"
"}";

const std::string MODULE_JSON_TEST_NOMODULETYPE = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
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
}

class ModuleJsonFaTest : public testing::Test {
public:
    ModuleJsonFaTest() {}
    virtual ~ModuleJsonFaTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ModuleJsonFaTest::SetUpTestCase() {}

void ModuleJsonFaTest::TearDownTestCase() {}

void ModuleJsonFaTest::SetUp() {}

void ModuleJsonFaTest::TearDown() {}

/*
 * @tc.name: SetFaBundleName_0100
 * @tc.desc: SetFaBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleName = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaBundleName(bundleName));
}

/*
 * @tc.name: SetFaBundleName_0200
 * @tc.desc: SetFaBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleName = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaBundleName(bundleName));
}

/*
 * @tc.name: SetFaBundleName_0300
 * @tc.desc: SetFaBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleName = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaBundleName(bundleName));
}

/*
 * @tc.name: SetFaBundleName_0400
 * @tc.desc: SetFaBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleName_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleName = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaBundleName(bundleName));
}

/*
 * @tc.name: SetFaMinCompatibleVersionCode_0100
 * @tc.desc: SetFaMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinCompatibleVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetFaMinCompatibleVersionCode_0200
 * @tc.desc: SetFaMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinCompatibleVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetFaMinCompatibleVersionCode_0300
 * @tc.desc: SetFaMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinCompatibleVersionCode_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetFaMinCompatibleVersionCode_0400
 * @tc.desc: SetFaMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinCompatibleVersionCode_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetFaMinAPIVersion_0100
 * @tc.desc: SetFaMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinAPIVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetFaMinAPIVersion_0200
 * @tc.desc: SetFaMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinAPIVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetFaMinAPIVersion_0300
 * @tc.desc: SetFaMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinAPIVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetFaMinAPIVersion_0400
 * @tc.desc: SetFaMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaMinAPIVersion_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetFaTargetAPIVersion_0100
 * @tc.desc: SetFaTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaTargetAPIVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetFaTargetAPIVersion_0200
 * @tc.desc: SetFaTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaTargetAPIVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(targetAPIVersion));
}


/*
 * @tc.name: SetFaTargetAPIVersion_0300
 * @tc.desc: SetFaTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaTargetAPIVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetFaTargetAPIVersion_0400
 * @tc.desc: SetFaTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaTargetAPIVersion_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetFaApiReleaseType_0100
 * @tc.desc: SetFaApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaApiReleaseType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetFaApiReleaseType_0200
 * @tc.desc: SetFaApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaApiReleaseType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetFaApiReleaseType_0300
 * @tc.desc: SetFaApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaApiReleaseType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetFaApiReleaseType_0400
 * @tc.desc: SetFaApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaApiReleaseType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetFaBundleType_0100
 * @tc.desc: SetFaBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaBundleType(bundleType));
}

/*
 * @tc.name: SetFaBundleType_0200
 * @tc.desc: SetFaBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaBundleType(bundleType));
}

/*
 * @tc.name: SetFaBundleType_0300
 * @tc.desc: SetFaBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaBundleType(bundleType));
}

/*
 * @tc.name: SetFaBundleType_0400
 * @tc.desc: SetFaBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaBundleType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetFaBundleType(bundleType));
}

/*
 * @tc.name: SetFaInstallationFree_0100
 * @tc.desc: SetFaInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaInstallationFree_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaInstallationFree(installationFree));
}

/*
 * @tc.name: SetFaInstallationFree_0200
 * @tc.desc: SetFaInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaInstallationFree_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaInstallationFree(installationFree));
}

/*
 * @tc.name: SetFaInstallationFree_0300
 * @tc.desc: SetFaInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaInstallationFree_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_TRUE(moduleJson.SetFaInstallationFree(installationFree));
}

/*
 * @tc.name: SetFaDeliveryWithInstall_0100
 * @tc.desc: SetFaDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeliveryWithInstall_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetFaDeliveryWithInstall_0200
 * @tc.desc: SetFaDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeliveryWithInstall_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetFaDeliveryWithInstall_0300
 * @tc.desc: SetFaDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeliveryWithInstall_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_TRUE(moduleJson.SetFaDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetFaDeviceTypes_0100
 * @tc.desc: SetFaDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeviceTypes_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetFaDeviceTypes(deviceTypes));
}

/*
 * @tc.name: SetFaDeviceTypes_0200
 * @tc.desc: SetFaDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeviceTypes_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetFaDeviceTypes(deviceTypes));
}

/*
 * @tc.name: SetFaDeviceTypes_0300
 * @tc.desc: SetFaDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonFaTest, SetFaDeviceTypes_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes = {"default", "tablet"};
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_TRUE(moduleJson.SetFaDeviceTypes(deviceTypes));
}
}