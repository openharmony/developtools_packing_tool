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

#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <numeric>
#include <string>

#define private public
#define protected public
#include "module_json.h"
#include "general_normalize.h"
#include "general_normalize_version_utils.h"
#include "hap_packager.h"
#include "log.h"
#include "pt_json.h"
#undef private
#undef protected

#include "hqf_packager.h"
#include "hsp_packager.h"
#include "json/json_utils.h"
#include "json/normalize_version_utils.h"
#include "json/pack_info.h"
#include "utils.h"
#include "zip_utils.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/testouthap";
const std::string HAP_OUT_PATH = "/data/test/entry-default-unsigned.hap";
const std::string HAP_INPUT_LIST = "/data/test/entry-default-unsigned.hap";
const std::string INPUT_LIST = "/data/test/testinputhap";
const std::string VERSION_CODE = "9999999";
const std::string VERSION_NAME = "30.11.22";
const std::string STAGE_INDEX_PATH = "/data/test/resource/packingtool/test_file/stage/resources.index";
const std::string STAGE_PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/stage/pack.info";
const std::string STAGE_ETS_PATH = "/data/test/resource/packingtool/test_file/stage/ets";
const std::string STAGE_RESOURCES_PATH = "/data/test/resource/packingtool/test_file/stage/resources";
const std::string STAGE_JSON_PATH = "/data/test/resource/packingtool/test_file/stage/module.json";
const std::string STAGE_RPCID_PATH = "/data/test/resource/packingtool/test_file/stage/rpcid.sc";
const std::string PATH = "/data/test/resource/packingtool/test_file/stage";
const std::string BUNDLE_NAME = "com.example.packingtoolfademo";
const std::string BUILD_VERSION_PATTERN = "^(?=.{1,18}$)(?:0|[1-9]\\d*)(?:\\.(?:0|[1-9]\\d*)){0,2}$";
const std::string MIN_COMPATIBLE_VERSION_CODE = "99";
const std::string MIN_API_VERSION = "11";
const std::string TARGET_API_VERSION = "12";
const std::string API_RELEASE_TYPE = "Canary1";
const std::string INVAILD_INT_NUM = "2147483648";
const std::string INVAILD_PATTERN = "-123";
const std::string TOO_LONG_VERSION_NAME(OHOS::AppPackingTool::Constants::MAX_VERSION_NAME_LENGTH + 1, '1');
const std::string TOO_SHORT_BUNDLE_NAME(OHOS::AppPackingTool::Constants::BUNDLE_NAME_LEN_MIN - 1, '1');
const std::string TOO_LONG_BUNDLE_NAME(OHOS::AppPackingTool::Constants::BUNDLE_NAME_LEN_MAX + 1, '1');
const std::string INVALID_DEVICE_TYPES =
"\"default\", \"tablet\", \"tv\", \"wearable\", \"car\", \"2in1\", \"abc\"";
const std::string CONFIG_JSON_STRING = "{"
    "\"app\": {"
        "\"apiVersion\": {"
            "\"compatible\": 9,"
            "\"releaseType\": \"Release\","
            "\"target\": 9"
        "},"
        "\"bundleName\": \"com.example.packingtoolfademo\","
        "\"vendor\": \"example\","
        "\"version\": {"
            "\"code\" : 1000000,"
            "\"name\" : \"1.0.0\""
        "}"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {"
            "\"debug\": true"
        "}"
    "},"
    "\"module\": {"
        "\"abilities\": ["
            "{"
                "\"description\": \"$string:MainAbility_desc\","
                "\"descriptionId\": 16777216,"
                "\"formsEnabled\": false,"
                "\"icon\": \"$media:icon\","
                "\"iconId\": 16777220,"
                "\"label\": \"$string:MainAbility_label\","
                "\"labelId\": 16777217,"
                "\"launchType\": \"standard\","
                "\"name\": \".MainAbility\","
                "\"orientation\": \"unspecified\","
                "\"skills\": ["
                    "{"
                        "\"actions\": ["
                            "\"action.system.home\""
                        "],"
                        "\"entities\": ["
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
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"distro\": {"
            "\"deliveryWithInstall\": true,"
            "\"installationFree\": false,"
            "\"moduleName\": \"entry\","
            "\"moduleType\": \"entry\","
            "\"virtualMachine\": \"ark9.0.0.0\""
        "},"
        "\"js\": ["
            "{"
                "\"mode\": {"
                    "\"syntax\": \"ets\","
                    "\"type\": \"pageAbility\""
                "},"
                "\"name\": \".MainAbility\","
                "\"pages\": ["
                    "\"pages/index\""
                "],"
                "\"window\": {"
                    "\"autoDesignWidth\": false,"
                    "\"designWidth\": 720"
                "}"
            "}"
        "],"
        "\"mainAbility\" : \".MainAbility\","
        "\"name\" : \".entry\","
        "\"package\" : \"com.example.packingtoolfademo\""
    "}"
"}";
const std::string CONFIG_JSON_STRING_NO_MODULE_NAME = "{"
    "\"app\": {"
        "\"apiVersion\": {"
            "\"compatible\": 9,"
            "\"releaseType\": \"Release\","
            "\"target\": 9"
        "},"
        "\"bundleName\": \"com.example.packingtoolfademo\","
        "\"vendor\": \"example\","
        "\"version\": {"
            "\"code\" : 1000000,"
            "\"name\" : \"1.0.0\""
        "}"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {"
            "\"debug\": true"
        "}"
    "},"
    "\"module\": {"
        "\"abilities\": ["
            "{"
                "\"description\": \"$string:MainAbility_desc\","
                "\"descriptionId\": 16777216,"
                "\"formsEnabled\": false,"
                "\"icon\": \"$media:icon\","
                "\"iconId\": 16777220,"
                "\"label\": \"$string:MainAbility_label\","
                "\"labelId\": 16777217,"
                "\"launchType\": \"standard\","
                "\"name\": \".MainAbility\","
                "\"orientation\": \"unspecified\","
                "\"skills\": ["
                    "{"
                        "\"actions\": ["
                            "\"action.system.home\""
                        "],"
                        "\"entities\": ["
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
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"distro\": {"
            "\"deliveryWithInstall\": true,"
            "\"installationFree\": false,"
            "\"moduleName\": \"entry\","
            "\"moduleType\": \"entry\","
            "\"virtualMachine\": \"ark9.0.0.0\""
        "},"
        "\"js\": ["
            "{"
                "\"mode\": {"
                    "\"syntax\": \"ets\","
                    "\"type\": \"pageAbility\""
                "},"
                "\"name\": \".MainAbility\","
                "\"pages\": ["
                    "\"pages/index\""
                "],"
                "\"window\": {"
                    "\"autoDesignWidth\": false,"
                    "\"designWidth\": 720"
                "}"
            "}"
        "],"
        "\"mainAbility\" : \".MainAbility\","
        "\"package\" : \"com.example.packingtoolfademo\""
    "}"
"}";
const std::string CONFIG_JSON_STRING_NO_APP_VERSION = "{"
    "\"app\": {"
        "\"apiVersion\": {"
            "\"compatible\": 9,"
            "\"releaseType\": \"Release\","
            "\"target\": 9"
        "},"
        "\"bundleName\": \"com.example.packingtoolfademo\","
        "\"vendor\": \"example\""
    "},"
    "\"deviceConfig\": {"
        "\"default\": {"
            "\"debug\": true"
        "}"
    "},"
    "\"module\": {"
        "\"abilities\": ["
            "{"
                "\"description\": \"$string:MainAbility_desc\","
                "\"descriptionId\": 16777216,"
                "\"formsEnabled\": false,"
                "\"icon\": \"$media:icon\","
                "\"iconId\": 16777220,"
                "\"label\": \"$string:MainAbility_label\","
                "\"labelId\": 16777217,"
                "\"launchType\": \"standard\","
                "\"name\": \".MainAbility\","
                "\"orientation\": \"unspecified\","
                "\"skills\": ["
                    "{"
                        "\"actions\": ["
                            "\"action.system.home\""
                        "],"
                        "\"entities\": ["
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
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"distro\": {"
            "\"deliveryWithInstall\": true,"
            "\"installationFree\": false,"
            "\"moduleName\": \"entry\","
            "\"moduleType\": \"entry\","
            "\"virtualMachine\": \"ark9.0.0.0\""
        "},"
        "\"js\": ["
            "{"
                "\"mode\": {"
                    "\"syntax\": \"ets\","
                    "\"type\": \"pageAbility\""
                "},"
                "\"name\": \".MainAbility\","
                "\"pages\": ["
                    "\"pages/index\""
                "],"
                "\"window\": {"
                    "\"autoDesignWidth\": false,"
                    "\"designWidth\": 720"
                "}"
            "}"
        "],"
        "\"mainAbility\" : \".MainAbility\","
        "\"name\" : \".entry\","
        "\"package\" : \"com.example.packingtoolfademo\""
    "}"
"}";
const std::string CONFIG_JSON_STRING_NO_BUNDLE_NAME = "{"
    "\"app\": {"
        "\"apiVersion\": {"
            "\"compatible\": 9,"
            "\"releaseType\": \"Release\","
            "\"target\": 9"
        "},"
        "\"vendor\": \"example\","
        "\"version\": {"
            "\"code\" : 1000000,"
            "\"name\" : \"1.0.0\""
        "}"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {"
            "\"debug\": true"
        "}"
    "},"
    "\"module\": {"
        "\"abilities\": ["
            "{"
                "\"description\": \"$string:MainAbility_desc\","
                "\"descriptionId\": 16777216,"
                "\"formsEnabled\": false,"
                "\"icon\": \"$media:icon\","
                "\"iconId\": 16777220,"
                "\"label\": \"$string:MainAbility_label\","
                "\"labelId\": 16777217,"
                "\"launchType\": \"standard\","
                "\"name\": \".MainAbility\","
                "\"orientation\": \"unspecified\","
                "\"skills\": ["
                    "{"
                        "\"actions\": ["
                            "\"action.system.home\""
                        "],"
                        "\"entities\": ["
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
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"distro\": {"
            "\"deliveryWithInstall\": true,"
            "\"installationFree\": false,"
            "\"moduleName\": \"entry\","
            "\"moduleType\": \"entry\","
            "\"virtualMachine\": \"ark9.0.0.0\""
        "},"
        "\"js\": ["
            "{"
                "\"mode\": {"
                    "\"syntax\": \"ets\","
                    "\"type\": \"pageAbility\""
                "},"
                "\"name\": \".MainAbility\","
                "\"pages\": ["
                    "\"pages/index\""
                "],"
                "\"window\": {"
                    "\"autoDesignWidth\": false,"
                    "\"designWidth\": 720"
                "}"
            "}"
        "],"
        "\"mainAbility\" : \".MainAbility\","
        "\"name\" : \".entry\","
        "\"package\" : \"com.example.packingtoolfademo\""
    "}"
"}";
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

const std::string JSON_STRING_NO_APP_VERSION = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\""
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
const std::string MODULE_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"buildVersion\": 1,"
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
const std::string TEST_STRING_NOTHING = "{"
"}";
const std::string EXPECT_CJSON_RESULT = "["
    "{"
        "\"moduleName\":\"moduleName\","
        "\"deviceTypes\":[\"phone\",\"tablet\"],"
        "\"bundleName\":\"originBundleName\","
        "\"versionCode\":1,"
        "\"buildVersion\":\"1\","
        "\"versionName\":\"originVersionName\","
        "\"minCompatibleVersionCode\":1,"
        "\"minAPIVersion\":1,"
        "\"targetAPIVersion\":1,"
        "\"apiReleaseType\":\"1.0.0\","
        "\"bundleType\":\"originBundleType\","
        "\"installationFree\":false,"
        "\"deliveryWithInstall\":false"
    "}"
"]";
}

class GeneralNormalizeTest : public testing::Test {
public:
    GeneralNormalizeTest() {}
    virtual ~GeneralNormalizeTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void GeneralNormalizeTest::SetUpTestCase()
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HAP_OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, STAGE_JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, STAGE_ETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, STAGE_RESOURCES_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, STAGE_INDEX_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, STAGE_PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("mv /data/test/resource/packingtool/test_file/stage/pack.json "
        "/data/test/resource/packingtool/test_file/stage/pack.info");
    system("mkdir /data/test/resource/packingtool/test_file/stage/ets");
    system("touch /data/test/resource/packingtool/test_file/stage/resources.index");
    EXPECT_EQ(hapPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hapPackager.PreProcess(), 0);
    EXPECT_EQ(hapPackager.Process(), 0);
    EXPECT_EQ(hapPackager.PostProcess(), 0);
    system("rm -rf /data/test/resource/packingtool/test_file/stage");
    system("rm -rf /data/test/resource/packingtool/test_file/fa");
}

void GeneralNormalizeTest::TearDownTestCase()
{
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/entry-default-unsigned.hap");
}

void GeneralNormalizeTest::SetUp() {}

void GeneralNormalizeTest::TearDown() {}

/*
 * @tc.name: InitAllowedParam_0100
 * @tc.desc: InitAllowedParam.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, InitAllowedParam_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.InitAllowedParam(), 0);
}

/*
 * @tc.name: Process_0100
 * @tc.desc: Process
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, Process_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, HAP_INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 0);
    EXPECT_EQ(generalNormalize.Process(), 0);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: Process_0200
 * @tc.desc: Process
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, Process_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, HAP_INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 0);
    system("rm -rf /data/test/entry-default-unsigned.hap");
    system("touch /data/test/entry-default-unsigned.hap");
    EXPECT_EQ(generalNormalize.Process(), 1);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: Process_0300
 * @tc.desc: Process
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, Process_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, HAP_INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_BUILD_VERSION, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 0);
    EXPECT_EQ(generalNormalize.Process(), 1);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, "module.json"},
    };
    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: PreProcess_0400
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_0500
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME] = TOO_LONG_VERSION_NAME;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_0600
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE] = INVAILD_INT_NUM;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_0700
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME] = TOO_SHORT_BUNDLE_NAME;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME] = TOO_LONG_BUNDLE_NAME;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize3(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_0800
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE] = INVAILD_INT_NUM;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_0900
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION] = INVAILD_INT_NUM;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1000
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION] = INVAILD_INT_NUM;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize2(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1100
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, TARGET_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_API_RELEASE_TYPE, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1200
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, TARGET_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_API_RELEASE_TYPE, API_RELEASE_TYPE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_TYPE, INVAILD_PATTERN},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1300
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, TARGET_API_VERSION},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 0);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1400
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, TARGET_API_VERSION},
        {OHOS::AppPackingTool::Constants::PARAM_DEVICE_TYPES, INVALID_DEVICE_TYPES},
    };
    system("mkdir /data/test/testouthap");
    system("mkdir /data/test/testinputhap");
    system("touch /data/test/testinputhap/test.hap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
    system("rm -rf /data/test/testinputhap");
}

/*
 * @tc.name: PreProcess_1500
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, HAP_INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUILD_VERSION, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 0);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: PreProcess_1600
 * @tc.desc: PreProcess
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, PreProcess_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, HAP_INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUILD_VERSION, INVAILD_PATTERN},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    system("mkdir /data/test/testouthap");
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(generalNormalize.PreProcess(), 1);
    system("rm -rf /data/test/testouthap");
}

/*
 * @tc.name: ModifyModuleJson_0100
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0100, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "";
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
}

/*
 * @tc.name: ModifyModuleJson_0200
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0200, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << MODULE_JSON_STRING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_TRUE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0300
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0300, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0400
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0400, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0500
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0500, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0600
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0600, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE, MIN_COMPATIBLE_VERSION_CODE},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0700
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0700, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MIN_API_VERSION, MIN_API_VERSION},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0800
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0800, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_TARGET_API_VERSION, TARGET_API_VERSION},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0900
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0900, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_API_RELEASE_TYPE, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_1000
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_1000, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_TYPE, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_1100
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_1100, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INSTALLATION_FREE, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_1200
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_1200, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_DELIVERY_WITH_INSTALL, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_1300
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_1300, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_DEVICE_TYPES, ""},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyModuleJson_0140
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyModuleJson_0140, Function | MediumTest | Level1)
{
    std::string bundleName = "";
    std::string moduleName = "";
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BUILD_VERSION, INVAILD_PATTERN},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string moduleJsonPath = "/data/test/module.json";
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: ModifyPackInfo_0100
 * @tc.desc: ModifyPackInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ModifyPackInfo_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BUILD_VERSION, INVAILD_PATTERN},
    };
    OHOS::AppPackingTool::GeneralNormalize generalNormalize(parameterMap, resultReceiver);
    std::string packInfoPath = "/data/test/resource/packingtool/test_file/stage/pack.info";
    system("touch /data/test/resource/packingtool/test_file/stage/pack.info");
    system("chmod 777 /data/test/resource/packingtool/test_file/stage/pack.info");
    std::ofstream file("/data/test/resource/packingtool/test_file/stage/pack.info");
    if (file.is_open()) {
        file << TEST_STRING_NOTHING;
        file.close();
    }
    GeneralNormalizeVersion generalNormalizeVersion;
    EXPECT_FALSE(generalNormalize.ModifyPackInfo(packInfoPath));
    system("rm -f /data/test/resource/packingtool/test_file/stage/pack.info");
}

/*
 * @tc.name: Check_BUILD_VERSION_PATTERN_0001
 * @tc.desc: Check_BUILD_VERSION_PATTERN with valid cases
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, Check_BUILD_VERSION_PATTERN_0001, Function | MediumTest | Level1)
{
    std::string validVersion1 = "1";
    EXPECT_TRUE(std::regex_match(validVersion1, std::regex(BUILD_VERSION_PATTERN)));
    std::string validVersion2 = "1.0.0";
    EXPECT_TRUE(std::regex_match(validVersion2, std::regex(BUILD_VERSION_PATTERN)));
    std::string validVersion3 = "123.456.789";
    EXPECT_TRUE(std::regex_match(validVersion3, std::regex(BUILD_VERSION_PATTERN)));
    std::string validVersion4 = "0.0.0";
    EXPECT_TRUE(std::regex_match(validVersion4, std::regex(BUILD_VERSION_PATTERN)));
    std::string maxLengthVersion = "123456789012345678";
    EXPECT_TRUE(std::regex_match(maxLengthVersion, std::regex(BUILD_VERSION_PATTERN)));
    std::string maxVersionWithDots = "12345678901234.1.1";
    EXPECT_TRUE(std::regex_match(maxVersionWithDots, std::regex(BUILD_VERSION_PATTERN)));
}

/*
 * @tc.name: Check_BUILD_VERSION_PATTERN_0002
 * @tc.desc: Check_BUILD_VERSION_PATTERN with invalid cases
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, Check_BUILD_VERSION_PATTERN_0002, Function | MediumTest | Level1)
{
    std::string invalidLeadingZero1 = "1.01";
    EXPECT_FALSE(std::regex_match(invalidLeadingZero1, std::regex(BUILD_VERSION_PATTERN)));
    std::string invalidLeadingZero2 = "1.1.01";
    EXPECT_FALSE(std::regex_match(invalidLeadingZero2, std::regex(BUILD_VERSION_PATTERN)));
    std::string invalidLeadingZero3 = "1.00.1";
    EXPECT_FALSE(std::regex_match(invalidLeadingZero3, std::regex(BUILD_VERSION_PATTERN)));
    std::string negativeVersion1 = "-1";
    EXPECT_FALSE(std::regex_match(negativeVersion1, std::regex(BUILD_VERSION_PATTERN)));
    std::string specialChar1 = "1.a.0";
    EXPECT_FALSE(std::regex_match(specialChar1, std::regex(BUILD_VERSION_PATTERN)));
    std::string specialChar2 = "1_0";
    EXPECT_FALSE(std::regex_match(specialChar2, std::regex(BUILD_VERSION_PATTERN)));
    std::string specialChar3 = "1 0";
    EXPECT_FALSE(std::regex_match(specialChar3, std::regex(BUILD_VERSION_PATTERN)));
    std::string multipleDots1 = "1..0";
    EXPECT_FALSE(std::regex_match(multipleDots1, std::regex(BUILD_VERSION_PATTERN)));
    std::string multipleDots2 = "1.0..0";
    EXPECT_FALSE(std::regex_match(multipleDots2, std::regex(BUILD_VERSION_PATTERN)));
    std::string multipleDots3 = "..1";
    EXPECT_FALSE(std::regex_match(multipleDots3, std::regex(BUILD_VERSION_PATTERN)));
    std::string overLengthVersion = "1234567890123456789";
    EXPECT_FALSE(std::regex_match(overLengthVersion, std::regex(BUILD_VERSION_PATTERN)));
}

/*
 * @tc.name: ArrayToString_0100
 * @tc.desc: ArrayToString
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(GeneralNormalizeTest, ArrayToString, Function | MediumTest | Level1)
{
    GeneralNormalizeVersion generalNormalizeVersion;
    generalNormalizeVersion.moduleName = "moduleName";
    generalNormalizeVersion.originDeviceTypes = {"phone", "tablet"};
    generalNormalizeVersion.originBundleName = "originBundleName";
    generalNormalizeVersion.originVersionCode = 1;
    generalNormalizeVersion.originBuildVersion = "1";
    generalNormalizeVersion.originVersionName = "originVersionName";
    generalNormalizeVersion.originMinCompatibleVersionCode = 1;
    generalNormalizeVersion.originMinAPIVersion = 1;
    generalNormalizeVersion.originTargetAPIVersion = 1;
    generalNormalizeVersion.originApiReleaseType = "1.0.0";
    generalNormalizeVersion.originBundleType = "originBundleType";
    generalNormalizeVersion.modifyInstallationFree = true;
    generalNormalizeVersion.modifyDeliveryWithInstall = true;
    std::list<GeneralNormalizeVersion> generalNormalizeVersions;
    generalNormalizeVersions.push_back(generalNormalizeVersion);
    OHOS::AppPackingTool::GeneralNormalizeVersionUtils generalNormalizeVersionUtils;
    EXPECT_EQ(generalNormalizeVersionUtils.ArrayToString(generalNormalizeVersions), EXPECT_CJSON_RESULT);
}
}