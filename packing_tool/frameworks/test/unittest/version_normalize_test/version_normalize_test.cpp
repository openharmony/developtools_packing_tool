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
#include "constants.h"
#define private public
#define protected public
#include "version_normalize.h"
#include "hap_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

#define HAP_OUT_PATH "/data/test/entry-default-unsigned.hap"
#define INPUT_LIST  "/data/test/entry-default-unsigned.hap"
#define VERSION_CODE "9999999"
#define VERSION_NAME "30.11.22"
#define OUT_PATH "/data/test/packagenormalize_packing"
#define STAGE_INDEX_PATH "/data/test/resource/packingtool/test_file/stage/resources.index"
#define STAGE_PACK_INFO_PATH "/data/test/resource/packingtool/test_file/stage/pack.info"
#define STAGE_ETS_PATH "/data/test/resource/packingtool/test_file/stage/ets"
#define STAGE_RESOURCES_PATH "/data/test/resource/packingtool/test_file/stage/resources"
#define STAGE_JSON_PATH "/data/test/resource/packingtool/test_file/stage/module.json"
#define STAGE_RPCID_PATH "/data/test/resource/packingtool/test_file/stage/rpcid.sc"
class VersionNormalizeTest : public testing::Test {
public:
    VersionNormalizeTest() {}
    virtual ~VersionNormalizeTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void VersionNormalizeTest::SetUpTestCase()
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
    system("rm -rf /data/test/resource/packingtool/test_file/stage/ets");
    system("rm -f /data/test/resource/packingtool/test_file/stage/resources.index");
}

void VersionNormalizeTest::TearDownTestCase()
{
    std::string cmd = {"rm -f /data/test/packagenormalize_packing/*"};
    system(cmd.c_str());
}

void VersionNormalizeTest::SetUp() {}

void VersionNormalizeTest::TearDown() {}

/*
 * @tc.name: VersionNormalize_0100
 * @tc.desc: VersionNormalize_0100.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, VersionNormalize_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 0);
    EXPECT_EQ(versionNormalize.Process(), 0);
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: input-list is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: input-list is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST,
            "/data/test/resource/packingtool/test_file/thumbnailTest2err.hap"},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: version-name is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0400
 * @tc.desc: version-name is not valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, "aabbc"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0500
 * @tc.desc: version-code is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0600
 * @tc.desc: version-code is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(VersionNormalizeTest, PreProcess_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_INPUT_LIST, INPUT_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, "-123456"},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_NAME, VERSION_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::VersionNormalize versionNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(versionNormalize.PreProcess(), 1);
}
} // namespace OHOS