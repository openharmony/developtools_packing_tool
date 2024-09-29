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
#include "hap_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/entry-default-unsigned.hap";
const std::string STAGE_INDEX_PATH = "/data/test/resource/packingtool/test_file/stage/resources.index";
const std::string STAGE_PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/stage/pack.info";
const std::string STAGE_ETS_PATH = "/data/test/resource/packingtool/test_file/stage/ets";
const std::string STAGE_RESOURCES_PATH = "/data/test/resource/packingtool/test_file/stage/resources";
const std::string STAGE_JSON_PATH = "/data/test/resource/packingtool/test_file/stage/module.json";
const std::string STAGE_RPCID_PATH = "/data/test/resource/packingtool/test_file/stage/rpcid.sc";

const std::string FA_INDEX_PATH = "/data/test/resource/packingtool/test_file/fa/resources.index";
const std::string FA_PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/fa/pack.info";
const std::string FA_RESOURCES_PATH = "/data/test/resource/packingtool/test_file/fa/resources";
const std::string FA_JSON_PATH = "/data/test/resource/packingtool/test_file/fa/config.json";
const std::string FA_ASSETS_PATH = "/data/test/resource/packingtool/test_file/fa/assets_jsbundle/default";
const std::string TEST_PATH = "/data/test/resources.index";
}

class HapPackagerTest : public testing::Test {
public:
    HapPackagerTest() {}
    virtual ~HapPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HapPackagerTest::SetUpTestCase() {}

void HapPackagerTest::TearDownTestCase() {}

void HapPackagerTest::SetUp() {}

void HapPackagerTest::TearDown() {}

/*
 * @tc.name: hapPackager_0100
 * @tc.desc: hapPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapPackagerTest, hapPackager_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
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
    system("touch /data/test/resource/packingtool/test_file/stage/resources.index");
    EXPECT_EQ(hapPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hapPackager.PreProcess(), 0);
    EXPECT_EQ(hapPackager.Process(), 0);
    EXPECT_EQ(hapPackager.PostProcess(), 0);
    system("rm -f /data/test/resource/packingtool/test_file/stage/resources.index");

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: hapPackager_0200
 * @tc.desc: hapPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapPackagerTest, hapPackager_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, FA_JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ASSETS_PATH, FA_ASSETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, FA_RESOURCES_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, FA_INDEX_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, FA_PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("touch /data/test/resource/packingtool/test_file/fa/resources.index");
    system("mkdir /data/test/resource/packingtool/test_file/fa/resources");
    system("mv /data/test/resource/packingtool/test_file/fa/pack.json "
        "/data/test/resource/packingtool/test_file/fa/pack.info");
    EXPECT_EQ(hapPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hapPackager.PreProcess(), 0);
    EXPECT_EQ(hapPackager.Process(), 0);
    EXPECT_EQ(hapPackager.PostProcess(), 0);
    system("rm -rf /data/test/resource/packingtool/test_file/fa/resources");
    system("rm -f /data/test/resource/packingtool/test_file/fa/resources.index");

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: IsValidRpcid_0300
 * @tc.desc: IsValidRpcid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapPackagerTest, IsValidRpcid_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RPCID_PATH, STAGE_RPCID_PATH},
    };
    system("touch /data/test/resource/packingtool/test_file/stage/rpcid.sc");
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hapPackager.IsValidRpcid());
    system("rm -f /data/test/resource/packingtool/test_file/stage/rpcid.sc");
}

/*
 * @tc.name: hapPackager_0300
 * @tc.desc: IsValidRpcid.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RPCID_PATH, resultReceiver},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.IsValidRpcid());
}

/*
 * @tc.name: hapPackager_0400
 * @tc.desc: IsValidRpcid.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RPCID_PATH, TEST_PATH},
    };
    system("touch /data/test/resources.index");
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.IsValidRpcid());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_0500
 * @tc.desc: IsValidPackInfo.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, resultReceiver},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.IsValidPackInfo());
}

/*
 * @tc.name: hapPackager_0600
 * @tc.desc: IsValidPackInfo.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
    };
    system("touch /data/test/resources.index");
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.IsValidPackInfo());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_0700
 * @tc.desc: IsHapPathValid.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MAPLE_SO_DIR, "/data/test/test1"},
        {OHOS::AppPackingTool::Constants::PARAM_LIB_PATH, "/data/test/test1/test2"},
        {OHOS::AppPackingTool::Constants::PARAM_HNP_PATH, "/data/test/test1/test3"},
        {OHOS::AppPackingTool::Constants::PARAM_RES_PATH, "/data/test/test1/test4"},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, "/data/test/test1/test5"},
        {OHOS::AppPackingTool::Constants::PARAM_ASSETS_PATH, "/data/test/test1/test6"},
        {OHOS::AppPackingTool::Constants::PARAM_SHAREDLIBS_PATH, "/data/test/test1/test7"},
        {OHOS::AppPackingTool::Constants::PARAM_AN_PATH, "/data/test/test1/test8"},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test2");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test3");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test4");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test5");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test6");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("mkdir /data/test/test1/test7");
    EXPECT_FALSE(hapPackager.IsHapPathValid());
    system("rm -rf /data/test/test1");
}

/*
 * @tc.name: hapPackager_0800
 * @tc.desc: CompressHap.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(hapPackager.CompressHap());
}

/*
 * @tc.name: hapPackager_0900
 * @tc.desc: CompressHap.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.CompressHap());
}

/*
 * @tc.name: hapPackager_1000
 * @tc.desc: CompressHap.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.jsonPath_ = "/data/test/test1";
    system("mkdir /data/test/test1");
    EXPECT_FALSE(hapPackager.CompressHap());
    system("rm -rf /data/test/test1");
}

/*
 * @tc.name: hapPackager_1100
 * @tc.desc: CheckStageHap.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.CheckStageHap(resultReceiver));
}

/*
 * @tc.name: hapPackager_1200
 * @tc.desc: CompressHapModeForModule.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.CompressHapModeForModule(resultReceiver));
}

/*
 * @tc.name: hapPackager_1300
 * @tc.desc: CompressHapMode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.CompressHapMode());
}

/*
 * @tc.name: hapPackager_1400
 * @tc.desc: CompressHapModeMultiple.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.formattedSoPathList_.emplace_back("/data/");
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.CompressHapModeMultiple());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_1500
 * @tc.desc: CompressHapModeMultiple.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_MAPLE_SO_DIR, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.CompressHapModeMultiple());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_1600
 * @tc.desc: CompressHapModeMultiple.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.formattedAbilitySoPathList_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.CompressHapModeMultiple());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_1700
 * @tc.desc: CompressHapModeMultiple.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_SHAREDLIBS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.CompressHapModeMultiple());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_1800
 * @tc.desc: AddFileListToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_SHAREDLIBS_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    std::list<std::string> pathList_;
    pathList_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddFileListToZip(pathList_));
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_1900
 * @tc.desc: AddCommonFileOrDirectoryToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_1900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {TEST_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddCommonFileOrDirectoryToZip(TEST_PATH, resultReceiver));
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2000
 * @tc.desc: AddResFileAndDirLsitToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.moduleName_ = OHOS::AppPackingTool::Constants::DEVICE_TYPE_FITNESSWATCH;
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddResFileAndDirLsitToZip());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2100
 * @tc.desc: AddResFileAndDirLsitToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.formatedDirList_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddResFileAndDirLsitToZip());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2200
 * @tc.desc: AddIndexToZipForFaMaode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.moduleName_ = TEST_PATH;
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddIndexToZipForFaMaode());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2300
 * @tc.desc: AddPkgAndBinFileToZipForStageMaode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddPkgAndBinFileToZipForStageMaode());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2400
 * @tc.desc: AddPkgAndBinFileToZipForStageMaode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PKG_CONTEXT_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddPkgAndBinFileToZipForStageMaode());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2500
 * @tc.desc: AddParamFileToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FILE_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.AddParamFileToZip());
}

/*
 * @tc.name: hapPackager_2600
 * @tc.desc: AddParamFileToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FILE_PATH, "/data/test/resources.index"},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddParamFileToZip());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2700
 * @tc.desc: AddResFileAndDirLsitToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.moduleName_ = OHOS::AppPackingTool::Constants::DEVICE_TYPE_FITNESSWATCH;
    hapPackager.deviceTypes_.emplace_back(OHOS::AppPackingTool::Constants::DEVICE_TYPE_FITNESSWATCH);
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    system("touch /data/test/resources.index");
    EXPECT_FALSE(hapPackager.AddResFileAndDirLsitToZip());
    system("rm -f /data/test/resources.index");
}

/*
 * @tc.name: hapPackager_2800
 * @tc.desc: AddResFileAndDirLsitToZip.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_RES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.moduleName_ = OHOS::AppPackingTool::Constants::DEVICE_TYPE_FITNESSWATCH;
    hapPackager.deviceTypes_.emplace_back(OHOS::AppPackingTool::Constants::DEVICE_TYPE_FITNESSWATCH_NEW);
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    hapPackager.formatedDirList_.emplace_back(TEST_PATH);
    EXPECT_TRUE(hapPackager.AddResFileAndDirLsitToZip());
}

/*
 * @tc.name: hapPackager_2900
 * @tc.desc: AddIndexToZipForFaMaode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_2900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    hapPackager.moduleName_ = TEST_PATH;
    hapPackager.deviceTypes_.emplace_back(TEST_PATH);
    EXPECT_TRUE(hapPackager.AddIndexToZipForFaMaode());
}

/*
 * @tc.name: hapPackager_3000
 * @tc.desc: AddPkgAndBinFileToZipForStageMaode.
 * @tc.type: FUNC
 */
HWTEST_F(HapPackagerTest, hapPackager_3000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BIN_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::HapPackager hapPackager(parameterMap, resultReceiver);
    EXPECT_FALSE(hapPackager.AddPkgAndBinFileToZipForStageMaode());
}
} // namespace OHOS
