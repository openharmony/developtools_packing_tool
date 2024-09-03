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
} // namespace OHOS