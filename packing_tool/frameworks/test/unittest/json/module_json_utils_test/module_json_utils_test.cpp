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
#include "module_json_utils.h"
#include "constants.h"
#include "pt_json.h"
#include "log.h"
#include "zip_wrapper.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {
const std::string HAP_FILE_PATH_FA = "/data/test.hap";
const std::string HAP_FILE_PATH_TEST = "/data/demo.hap";
const std::string MODULE_JSON_FILE = "/data/module.json";
const std::string CONFIG_JSON_FILE = "/data/config.json";

const std::string MODULE_JSON_STR = "{"
    "\"app\": {"
        "\"iconId\":16777217,"
        "\"debug\":true,"
        "\"minAPIVersion\":9,"
        "\"icon\":\"media:app_icon\","
        "\"label\":\"string:app_name\","
        "\"versionName\":\"1.0.0\","
        "\"versionCode\":1000000,"
        "\"compileSdkType\":\"OpenHarmony\","
        "\"labelId\":16777216,"
        "\"compileSdkVersion\":\"test_compileSdkVersion\","
        "\"targetAPIVersion\":9,"
        "\"vendor\":\"\","
        "\"bundleName\":\"com.example.demo\","
        "\"distributedNotificationEnabled\":true,"
        "\"apiReleaseType\":\"Release\""
        "},"
    "\"module\":{"
        "\"virtualMachine\":\"test_virtualMachine\","
        "\"mainElement\":\"EntryAbility\","
        "\"installationFree\":false,"
        "\"deliveryWithInstall\":true,"
        "\"description\":\"string:module_desc\","
        "\"compileMode\":\"esmodule\","
        "\"type\":\"entry\","
        "\"dependencies\":[],"
        "\"abilities\":["
            "{"
                "\"iconId\":16777222,"
                "\"startWindowIconId\":16777222,"
                "\"visible\":true,"
                "\"icon\":\"media:icon\","
                "\"startWindowBackgroundId\":16777221,"
                "\"startWindowIcon\":\"media:icon\","
                "\"description\":\"string:EntryAbility_desc\","
                "\"label\":\"string:EntryAbility_label\","
                "\"skills\":[{\"entities\":[\"entity.system.home\"],"
                "\"actions\":[\"action.system.home\"]}],"
                "\"descriptionId\":16777218,"
                "\"labelId\":16777219,"
                "\"startWindowBackground\":\"color:start_window_background\","
                "\"srcEntrance\":\"./ets/entryability/EntryAbility.ts\","
                "\"name\":\"EntryAbility\""
            "}"
        "],"
        "\"descriptionId\":16777220,"
        "\"deviceTypes\":[\"default\",\"tablet\"],"
        "\"pages\":\"$profile:main_pages\","
        "\"name\":\"entry\""
    "}"
"}";

const std::string CONFIG_JSON_STR = "{"
    "\"app\" :{"
        "\"apiVersion\" :{"
            "\"compatible\" : 9,"
            "\"releaseType\" : \"Release\","
            "\"target\" : 9"
        "},"
        "\"bundleName\" : \"test_bundl_name\","
        "\"vendor\" : \"\","
        "\"version\" :{"
            "\"code\" : 1000000,"
            "\"name\" : \"test_name\""
        "}"
    "},"
    "\"deviceConfig\": {"
        "\"default\": true"
    "},"
    "\"module\" :{"
        "\"abilities\" :["
            "{"
                "\"description\" : \"string:MainAbility_desc\","
                "\"descriptionId\" : 16777216,"
                "\"formsEnabled\" : false,"
                "\"icon\" : \"media:icon\","
                "\"iconId\" : 16777220,"
                "\"label\" : \"string:MainAbility_label\","
                "\"labelId\" : 16777217,"
                "\"launchType\" : \"standard\","
                "\"name\" : \".MainAbility\","
                "\"orientation\" : \"unspecified\","
                "\"skills\" :["
                    "{"
                        "\"actions\" :["
                        "\"action.system.home\""
                        "],"
                        "\"entities\" :["
                        "\"entity.system.home\""
                        "]"
                    "}"
                "],"
                "\"srcLanguage\" : \"ets\","
                "\"srcPath\" : \"MainAbility\","
                "\"type\" : \"page\","
                "\"visible\" : true"
            "}"
        "],"
        "\"deviceType\" :["
        "\"phone\","
        "\"tablet\""
        "],"
        "\"distro\" :{"
            "\"deliveryWithInstall\" : true,"
            "\"installationFree\" : false,"
            "\"moduleName\" : \"entry\","
            "\"moduleType\" : \"entry\","
            "\"virtualMachine\" : \"test_virtualMachine\""
        "},"
        "\"js\" :["
            "{"
                "\"mode\" :{"
                    "\"syntax\" : \"ets\","
                    "\"type\" : \"pageAbility\""
                "},"
                "\"name\" : \".MainAbility\","
                "\"pages\" :["
                "\"pages/index\""
                "],"
                "\"window\" :{"
                    "\"autoDesignWidth\" : false,"
                    "\"designWidth\" : 720"
                "}"
            "}"
        "],"
        "\"mainAbility\" : \".MainAbility\","
        "\"name\" : \".entry\","
        "\"package\" : \"test_package\""
    "}"
"}";
}

class ModuleJsonUtilsTest : public testing::Test {
public:
    ModuleJsonUtilsTest() {}
    virtual ~ModuleJsonUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ModuleJsonUtilsTest::SetUpTestCase() {}

void ModuleJsonUtilsTest::TearDownTestCase() {}

void ModuleJsonUtilsTest::SetUp() {}

void ModuleJsonUtilsTest::TearDown() {}

bool CreateModuleJsonFile(std::string filename)
{
    std::string module_test(filename);
    FILE *fp = fopen(module_test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(MODULE_JSON_STR.c_str(), MODULE_JSON_STR.size(), 1, fp);
        fclose(fp);
    } else {
        return false;
    }
    return true;
}

bool CreateModuleJsonHap(std::string hapFilePath, std::string filename)
{
    EXPECT_TRUE(CreateModuleJsonFile(filename));
    ZipWrapper zipWrapper(hapFilePath);
    zipWrapper.Open(APPEND_STATUS_CREATE);
    std::string filepath(filename);
    std::string zippath(OHOS::AppPackingTool::Constants::MODULE_JSON);
    zipWrapper.AddFileOrDirectoryToZip(filepath, zippath);
    zipWrapper.Close();
    return true;
}

bool CreateConfigJsonFile(std::string filename)
{
    std::string config_test(filename);
    FILE *fp = fopen(config_test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr) {
        fwrite(CONFIG_JSON_STR.c_str(), CONFIG_JSON_STR.size(), 1, fp);
        fclose(fp);
    } else {
        return false;
    }
    return true;
}

bool CreateConfigJsonHap(std::string hapFilePath, std::string filename)
{
    EXPECT_TRUE(CreateConfigJsonFile(filename));
    ZipWrapper zipWrapper(hapFilePath);
    zipWrapper.Open(APPEND_STATUS_CREATE);
    std::string filepath(filename);
    std::string zippath(OHOS::AppPackingTool::Constants::CONFIG_JSON);
    zipWrapper.AddFileOrDirectoryToZip(filepath, zippath);
    zipWrapper.Close();
    return true;
}

 /*
 * @tc.name: GetStageHapVerifyInfo_0100
 * @tc.desc: GetStageHapVerifyInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonUtilsTest, GetStageHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    std::string hapFilePath(HAP_FILE_PATH_TEST);
    EXPECT_TRUE(CreateModuleJsonHap(hapFilePath, MODULE_JSON_FILE));
    HapVerifyInfo hapVerifyInfo;
    EXPECT_TRUE(ModuleJsonUtils::GetStageHapVerifyInfo(hapFilePath, hapVerifyInfo));

    std::string cmd = {"rm -f "};
    cmd += HAP_FILE_PATH_TEST;
    cmd += " ";
    cmd += MODULE_JSON_FILE;
    system(cmd.c_str());
}

 /*
 * @tc.name: GetFaHapVerifyInfo_0100
 * @tc.desc: GetFaHapVerifyInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonUtilsTest, GetFaHapVerifyInfo_0100, Function | MediumTest | Level1)
{
    std::string hapFilePath(HAP_FILE_PATH_FA);
    EXPECT_TRUE(CreateConfigJsonHap(hapFilePath, CONFIG_JSON_FILE));
    HapVerifyInfo hapVerifyInfo;
    EXPECT_TRUE(ModuleJsonUtils::GetFaHapVerifyInfo(hapFilePath, hapVerifyInfo));

    std::string cmd = {"rm -f "};
    cmd += HAP_FILE_PATH_FA;
    cmd += " ";
    cmd += CONFIG_JSON_FILE;
    system(cmd.c_str());
}

 /*
 * @tc.name: CheckSharedAppIsValid_0100
 * @tc.desc: CheckSharedAppIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonUtilsTest, CheckSharedAppIsValid_0100, Function | MediumTest | Level1)
{
    std::list<HapVerifyInfo> hapVerifyInfos;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetTargetBundleName("test1_target_bundle_name");
    hapVerifyInfo.SetModuleName("test1_module_name");

    std::list<DependencyItem> dependencyItemList;
    DependencyItem dependencyItem;
    dependencyItem.bundleName = "test1_bundle_name";
    dependencyItem.moduleName = "test1_module_name";
    dependencyItem.versionCode = 5;
    dependencyItemList.emplace_back(dependencyItem);

    hapVerifyInfo.SetDependencyItemList(dependencyItemList);
    hapVerifyInfo.SetModuleType("test1_module_type");
    hapVerifyInfos.emplace_back(hapVerifyInfo);

    bool isOverlay = false;
    EXPECT_TRUE(ModuleJsonUtils::CheckSharedAppIsValid(hapVerifyInfos, isOverlay));
}

 /*
 * @tc.name: CheckHapsIsValid_0100
 * @tc.desc: CheckHapsIsValid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonUtilsTest, CheckHapsIsValid_0100, Function | MediumTest | Level1)
{
    std::string hapFilePath(HAP_FILE_PATH_TEST);
    EXPECT_TRUE(CreateModuleJsonHap(hapFilePath, MODULE_JSON_FILE));

    std::list<std::string> fileList;
    fileList.emplace_back(hapFilePath);
    bool isSharedApp = false;
    EXPECT_TRUE(ModuleJsonUtils::CheckHapsIsValid(fileList, isSharedApp));

    std::string cmd = {"rm -f "};
    cmd += HAP_FILE_PATH_TEST;
    cmd += " ";
    cmd += MODULE_JSON_FILE;
    system(cmd.c_str());
}

 /*
 * @tc.name: IsModuleHap_0100
 * @tc.desc: IsModuleHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonUtilsTest, IsModuleHap_0100, Function | MediumTest | Level1)
{
    std::string hapFilePath(HAP_FILE_PATH_TEST);
    EXPECT_TRUE(CreateModuleJsonHap(hapFilePath, MODULE_JSON_FILE));
    EXPECT_TRUE(ModuleJsonUtils::IsModuleHap(hapFilePath));

    std::string cmd = {"rm -f "};
    cmd += HAP_FILE_PATH_TEST;
    cmd += " ";
    cmd += MODULE_JSON_FILE;
    system(cmd.c_str());
}
}