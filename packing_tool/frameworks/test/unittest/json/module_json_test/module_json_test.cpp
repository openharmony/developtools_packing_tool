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
                "\"value\": \"test_value\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": true"
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
    HapVerifyInfo hapVerifyInfo;
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
    EXPECT_TRUE(moduleJson.GetStageDistroFilter(distroFilter));
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
    EXPECT_TRUE(moduleJson.GetModuleMetadatas(moduleMetadataInfos));
    EXPECT_NE(moduleMetadataInfos.size(), 0);
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
    HapVerifyInfo hapVerifyInfo;
    EXPECT_TRUE(moduleJson.SetStageHapVerifyInfoExtByModuleObj(moduleObj, hapVerifyInfo));
}

}