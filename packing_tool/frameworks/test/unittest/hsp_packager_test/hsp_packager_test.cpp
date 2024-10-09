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
#include <fstream>
#include <string>

#define private public
#define protected public
#include "hsp_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/library-default-unsigned.hsp";
const std::string INDEX_PATH = "/data/test/resource/packingtool/test_file/resources.index";
const std::string PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";
const std::string ETS_PATH = "/data/test/resource/packingtool/test_file/ets";
const std::string RESOURCES_PATH = "/data/test/resource/packingtool/test_file/resources";
const std::string JSON_PATH = "/data/test/resource/packingtool/test_file/module.json";
const std::string TEST_PATH = "/data/test/resources.index";
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

class HspPackagerTest : public testing::Test {
public:
    HspPackagerTest() {}
    virtual ~HspPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HspPackagerTest::SetUpTestCase() {}

void HspPackagerTest::TearDownTestCase() {}

void HspPackagerTest::SetUp()
{
    system("mkdir -p /data/test/resource/packingtool/test_file");
}

void HspPackagerTest::TearDown()
{
    system("rm -rf /data/test/resource");
}

/*
 * @tc.name: hspPackager_0100
 * @tc.desc: hspPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, hspPackager_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, ETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, RESOURCES_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, INDEX_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    system("touch /data/test/resource/packingtool/test_file/resources.index");
    system("mkdir /data/test/resource/packingtool/test_file/ets");
    system("mv /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    EXPECT_EQ(hspPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hspPackager.PreProcess(), 0);
    EXPECT_EQ(hspPackager.Process(), 0);
    EXPECT_EQ(hspPackager.PostProcess(), 0);
    system("rm -rf /data/test/resource/packingtool/test_file/ets");
    system("rm -f /data/test/resource/packingtool/test_file/resources.index");

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: IsVerifyValidInHspMode_0200
 * @tc.desc: IsVerifyValidInHspMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, IsVerifyValidInHspMode_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, ETS_PATH},
    };

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.IsVerifyValidInHspMode());
}

/*
 * @tc.name: CompressHsp_0300
 * @tc.desc: CompressHsp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHsp_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.CompressHsp());

    system("touch /data/test/resource/packingtool/test_file/module.json");
    hspPackager.jsonPath_ = JSON_PATH;
    EXPECT_FALSE(hspPackager.CompressHsp());
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
}

/*
 * @tc.name: CompressHsp_0400
 * @tc.desc: CompressHsp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHsp_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    system("touch /data/test/resource/packingtool/test_file/module.json");
    system("chmod 777 /data/test/resource/packingtool/test_file/module.json");
    std::ofstream file("/data/test/resource/packingtool/test_file/module.json");
    if (file.is_open()) {
        file << MODULE_JSON_TEST_STRING;
        file.close();
    }
    hspPackager.jsonPath_ = JSON_PATH;
    EXPECT_FALSE(hspPackager.CompressHsp());
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
}

/*
 * @tc.name: CompressHspMode_0500
 * @tc.desc: CompressHspMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspMode_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
    };

    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_FALSE(hspPackager1.CompressHspMode(JSON_PATH));

    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, TEST_PATH},
    };
    system("touch /data/test/library-default-unsigned.hsp");
    system("touch /data/test/resources.index");
    system("touch /data/test/resource/packingtool/test_file/module.json");
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    hspPackager.moduleJson_.ParseFromString(MODULE_JSON_TEST_STRING);
    EXPECT_TRUE(hspPackager.CompressHspMode(JSON_PATH));
    std::string jsonPath = "aa";
    EXPECT_TRUE(hspPackager.CompressHspMode(jsonPath));
    system("rm -f /data/test/library-default-unsigned.hsp");
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: CompressHspModePartSecond_0600
 * @tc.desc: CompressHspModePartSecond.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartSecond_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_LIB_PATH, TEST_PATH},
    };
    system("touch /data/test/resources.index");
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.CompressHspModePartSecond(JSON_PATH));

    std::string filepath = "/data/test/test";
    system("mkdir /data/test/test");
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_FILE_PATH, filepath}};
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_FALSE(hspPackager1.CompressHspModePartSecond(JSON_PATH));

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, TEST_PATH}};
    OHOS::AppPackingTool::HspPackager hspPackager2(parameterMap2, resultReceiver);
    EXPECT_TRUE(hspPackager2.CompressHspModePartSecond(JSON_PATH));
    system("touch /data/test/resource/packingtool/test_file/module.json");
    EXPECT_FALSE(hspPackager2.CompressHspModePartSecond(JSON_PATH));
    system("rm -f /data/test/resources.index");
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
    system("rm -rf /data/test/test");
}

/*
 * @tc.name: CompressHspModePartThird_0700
 * @tc.desc: CompressHspModePartThird.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartThird_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, TEST_PATH},
    };
    system("touch /data/test/resources.index");
    std::list<std::string> deviceTypes = {"fitnessWatch"};

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    hspPackager.deviceTypes_ = deviceTypes;
    EXPECT_FALSE(hspPackager.CompressHspModePartThird(JSON_PATH));

    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_JS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_TRUE(hspPackager1.CompressHspModePartThird(JSON_PATH));
    system("touch /data/test/resource/packingtool/test_file/module.json");
    EXPECT_FALSE(hspPackager1.CompressHspModePartThird(JSON_PATH));
    system("rm -f /data/test/resource/packingtool/test_file/module.json");

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_JS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager2(parameterMap2, resultReceiver);
    EXPECT_TRUE(hspPackager2.CompressHspModePartThird(JSON_PATH));
    system("touch /data/test/resource/packingtool/test_file/module.json");
    EXPECT_FALSE(hspPackager2.CompressHspModePartThird(JSON_PATH));
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: CompressHspModePartFourth_0800
 * @tc.desc: CompressHspModePartFourth.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartFourth_0800, Function | MediumTest | Level1)
{
    system("touch /data/test/resources.index");
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.CompressHspModePartFourth());

    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_FALSE(hspPackager1.CompressHspModePartFourth());

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_PKG_CONTEXT_PATH, JSON_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager2(parameterMap2, resultReceiver);
    hspPackager2.formatedDirList_.push_back(TEST_PATH);
    EXPECT_FALSE(hspPackager2.CompressHspModePartFourth());
    hspPackager2.formatedDirList_.clear();
    EXPECT_FALSE(hspPackager2.CompressHspModePartFourth());
    system("touch /data/test/resource/packingtool/test_file/module.json");
    system("chmod 777 /data/test/resource/packingtool/test_file/module.json");
    std::ofstream file("/data/test/resource/packingtool/test_file/module.json");
    if (file.is_open())
    {
        file << MODULE_JSON_TEST_STRING;
        file.close();
    }
    EXPECT_FALSE(hspPackager2.CompressHspModePartFourth());
    system("rm -f /data/test/resources.index");
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
}

/*
 * @tc.name: CompressHspModeMultiple_0900
 * @tc.desc: CompressHspModeMultiple.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModeMultiple_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MAPLE_SO_DIR, TEST_PATH},
    };
    system("touch /data/test/resources.index");

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.CompressHspModeMultiple());

    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_SHAREDLIBS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    hspPackager1.formattedJarPathList_.push_back(TEST_PATH);
    hspPackager1.formattedTxtPathList_.push_back(TEST_PATH);
    EXPECT_FALSE(hspPackager1.CompressHspModeMultiple());
    hspPackager1.formattedJarPathList_.clear();
    EXPECT_FALSE(hspPackager1.CompressHspModeMultiple());
    hspPackager1.formattedTxtPathList_.clear();
    EXPECT_FALSE(hspPackager1.CompressHspModeMultiple());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: CompressHspModeMultiple_1000
 * @tc.desc: CompressHspModeMultiple.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModeMultiple_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MAPLE_SO_DIR, ""},
        {OHOS::AppPackingTool::Constants::PARAM_SHAREDLIBS_PATH, ""},
    };

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hspPackager.CompressHspModeMultiple());
    hspPackager.formattedJarPathList_.push_back(TEST_PATH);
    hspPackager.formattedTxtPathList_.push_back(TEST_PATH);
    hspPackager.formattedSoPathList_.push_back(TEST_PATH);
    EXPECT_TRUE(hspPackager.CompressHspModeMultiple());
}

/*
 * @tc.name: CompressHspModePartFourth_1100
 * @tc.desc: CompressHspModePartFourth.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartFourth_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PKG_CONTEXT_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hspPackager.CompressHspModePartFourth());
}

/*
 * @tc.name: CompressHspModePartThird_1200
 * @tc.desc: CompressHspModePartThird.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartThird_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_JS_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_JS_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hspPackager.CompressHspModePartThird(JSON_PATH));
}

/*
 * @tc.name: IsHspPathValid_1300
 * @tc.desc: IsHspPathValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, IsHspPathValid_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_LIB_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.IsHspPathValid());

    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_RES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_FALSE(hspPackager1.IsHspPathValid());

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager2(parameterMap2, resultReceiver);
    EXPECT_FALSE(hspPackager2.IsHspPathValid());

    std::map<std::string, std::string> parameterMap3 = {
        {OHOS::AppPackingTool::Constants::PARAM_ASSETS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager3(parameterMap3, resultReceiver);
    EXPECT_FALSE(hspPackager3.IsHspPathValid());

    std::map<std::string, std::string> parameterMap4 = {
        {OHOS::AppPackingTool::Constants::PARAM_AP_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager4(parameterMap4, resultReceiver);
    EXPECT_FALSE(hspPackager4.IsHspPathValid());

    std::map<std::string, std::string> parameterMap5 = {
        {OHOS::AppPackingTool::Constants::PARAM_AN_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager5(parameterMap5, resultReceiver);
    EXPECT_FALSE(hspPackager5.IsHspPathValid());
}

/*
 * @tc.name: CompressHspModePartSecond_1400
 * @tc.desc: CompressHspModePartSecond.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, CompressHspModePartSecond_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FILE_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hspPackager.CompressHspModePartSecond(JSON_PATH));
}

/*
 * @tc.name: IsVerifyValidInHspCommonMode_1500
 * @tc.desc: IsVerifyValidInHspCommonMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, IsVerifyValidInHspCommonMode_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hspPackager.IsVerifyValidInHspCommonMode());

    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager1(parameterMap1, resultReceiver);
    EXPECT_FALSE(hspPackager1.IsVerifyValidInHspCommonMode());

    system("touch /data/test/resource/packingtool/test_file/module.json");
    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_DIR_LIST, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PROFILE_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager2(parameterMap2, resultReceiver);
    EXPECT_FALSE(hspPackager2.IsVerifyValidInHspCommonMode());

    std::map<std::string, std::string> parameterMap3 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_DIR_LIST, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PROFILE_PATH, JSON_PATH},
    };
    OHOS::AppPackingTool::HspPackager hspPackager3(parameterMap3, resultReceiver);
    EXPECT_FALSE(hspPackager3.IsVerifyValidInHspCommonMode());

    std::string profilePath = "/data/test/CAPABILITY.profile";
    system("touch /data/test/CAPABILITY.profile");
    std::map<std::string, std::string> parameterMap4 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_DIR_LIST, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PROFILE_PATH, profilePath},
        {OHOS::AppPackingTool::Constants::PARAM_PKG_CONTEXT_PATH, ""},
    };
    OHOS::AppPackingTool::HspPackager hspPackager4(parameterMap4, resultReceiver);
    EXPECT_TRUE(hspPackager4.IsVerifyValidInHspCommonMode());

    std::map<std::string, std::string> parameterMap5 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_DIR_LIST, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PROFILE_PATH, profilePath},
        {OHOS::AppPackingTool::Constants::PARAM_PKG_CONTEXT_PATH, profilePath},
    };
    OHOS::AppPackingTool::HspPackager hspPackager5(parameterMap5, resultReceiver);
    EXPECT_FALSE(hspPackager5.IsVerifyValidInHspCommonMode());
    system("rm -f /data/test/resource/packingtool/test_file/module.json");
    system("rm -f /data/test/CAPABILITY.profile");
}
} // namespace OHOS