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
}