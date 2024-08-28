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

#include "constants.h"
#define private public
#define protected public
#include "hqf_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string TEST_PATH = "/data/test/";
const std::string TEST_FILE_PATH = TEST_PATH + std::string("resource/packingtool/test_file/");
const std::string HQF_FILE_EXAMPLE_PATH = TEST_FILE_PATH + std::string("hqf_packager_file/hqfFileExample1/");
const std::string HQF_ETS_PATH_ONE = HQF_FILE_EXAMPLE_PATH + std::string("ets");
const std::string HQF_JSON_PATH_ONE = HQF_FILE_EXAMPLE_PATH + std::string("patch.json");
const std::string HQF_RESOURCES_PATH_ONE = HQF_FILE_EXAMPLE_PATH + std::string("resources");
const std::string OUT_PATH = TEST_PATH + std::string("testPack/testHqfPack.hqf");
}

class HqfPackagetTest : public testing::Test {
public:
    HqfPackagetTest() {}
    virtual ~HqfPackagetTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HqfPackagetTest::SetUpTestCase() {}

void HqfPackagetTest::TearDownTestCase() {}

void HqfPackagetTest::SetUp() {}

void HqfPackagetTest::TearDown() {}

/*
 * @tc.name: HqfPackage_0100
 * @tc.desc: HqfPackage
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfPackagetTest, HqfPackage_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, HQF_ETS_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, HQF_RESOURCES_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 0);
    EXPECT_EQ(hqfPackager.Process(), 0);
    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: PreProcess_0100.
 * @tc.type: FUNC
 * @tc.require: out file not end with .hqf.
 */
HWTEST_F(HqfPackagetTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, HQF_ETS_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, HQF_RESOURCES_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/testPack/testHqfPack/testHqfPack.hqfx"}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: param json path is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfPackagetTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH,
            "/data/test/resource/packingtool/test_file/hqf_packager_file/hqfFileExample1/patcherr.json"},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0400
 * @tc.desc: libs path is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfPackagetTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_LIB_PATH,
            "/data/test/resource/packingtool/test_file/hqf_packager_file/hqfFileExample1/errlibs"},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0500
 * @tc.desc: ets path is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfPackagetTest, PreProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH,
            "/data/test/resource/packingtool/test_file/hqf_packager_file/hqfFileExample1/errets"},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0600
 * @tc.desc: resources path is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HqfPackagetTest, PreProcess_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH,
            "/data/test/resource/packingtool/test_file/hqf_packager_file/hqfFileExample1/errresources"},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager(parameterMap, resultReceiver);
    EXPECT_EQ(hqfPackager.PreProcess(), 1);
}
} // namespace OHOS