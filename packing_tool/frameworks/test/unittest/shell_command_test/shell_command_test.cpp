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
#include "zip_constants.h"
#define private public
#define protected public
#include "packager.h"
#include "shell_command.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

class ShellCommandTest : public testing::Test {
public:
    ShellCommandTest() {}
    virtual ~ShellCommandTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ShellCommandTest::SetUpTestCase() {}

void ShellCommandTest::TearDownTestCase() {}

void ShellCommandTest::SetUp() {}

void ShellCommandTest::TearDown() {}

/*
 * @tc.name: CreateCommandMap_0100
 * @tc.desc: CreateCommandMap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, CreateCommandMap_0100, Function | MediumTest | Level1)
{
    int32_t argc = 2;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    int32_t ret = shellcmd.CreateCommandMap();
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: getPackager_0200
 * @tc.desc: getPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, getPackager_0200, Function | MediumTest | Level1)
{
    int32_t argc = 16;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
        "--mode",
        "hap",
        "--json-path",
        "/data/testunpack/test_hqf_unpacking/patch.json",
        "--lib-path",
        "/data/testunpack/test_hqf_unpacking/libs",
        "--resources-path",
        "/data/testunpack/test_hqf_unpacking/resources",
        "--ets-pat",
        "/data/testunpack/test_hqf_unpacking/ets",
        "--out-path",
        "/data/hqfpacking/test_1.hqf",
        "--force",
        "true",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    int32_t ret = shellcmd.ParseParam();
    EXPECT_EQ(ret, 0);
    std::unique_ptr<OHOS::AppPackingTool::Packager> packager = shellcmd.getPackager();
    EXPECT_TRUE(packager != nullptr);
    shellcmd.parameterMap_[OHOS::AppPackingTool::Constants::PARAM_MODE] = "hsp";
    std::unique_ptr<OHOS::AppPackingTool::Packager> packager2 = shellcmd.getPackager();
    EXPECT_TRUE(packager2 != nullptr);
    shellcmd.parameterMap_[OHOS::AppPackingTool::Constants::PARAM_MODE] = "";
    std::unique_ptr<OHOS::AppPackingTool::Packager> packager3 = shellcmd.getPackager();
    EXPECT_TRUE(packager3 == nullptr);
}

/*
 * @tc.name: OnCommand_0300
 * @tc.desc: OnCommand.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, OnCommand_0300, Function | MediumTest | Level1)
{
    int32_t argc = 2;
    const char *argv[] = {
        "ohos_packing_tool",
        "help",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    int32_t ret = shellcmd.CreateCommandMap();
    EXPECT_EQ(ret, 0);
    ret = shellcmd.OnCommand();
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: ExecCommand_0400
 * @tc.desc: ExecCommand.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, ExecCommand_0400, Function | MediumTest | Level1)
{
    int32_t argc = 16;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
        "--mode",
        "hqf",
        "--json-path",
        "/data/testunpack/test_hqf_unpacking/patch.json",
        "--lib-path",
        "/data/testunpack/test_hqf_unpacking/libs",
        "--resources-path",
        "/data/testunpack/test_hqf_unpacking/resources",
        "--ets-pat",
        "/data/testunpack/test_hqf_unpacking/ets",
        "--out-path",
        "/data/hqfpacking/test_1.hqf",
        "--force",
        "true",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    std::string resultReceiver = shellcmd.ExecCommand();
    EXPECT_FALSE(resultReceiver.empty());
}

/*
 * @tc.name: RunAsHelpCommand_0500
 * @tc.desc: RunAsHelpCommand.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, RunAsHelpCommand_0500, Function | MediumTest | Level1)
{
    int32_t argc = 4;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
        "--mode",
        "hqf",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    EXPECT_EQ(shellcmd.RunAsHelpCommand(), 0);
}

/*
 * @tc.name: RunAsPackCommand_0600
 * @tc.desc: RunAsPackCommand.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, RunAsPackCommand_0600, Function | MediumTest | Level1)
{
    int32_t argc = 4;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
        "--mode",
        "hqf",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    EXPECT_EQ(shellcmd.RunAsPackCommand(), 0);
}

/*
 * @tc.name: RunAsUnpackCommand_0700
 * @tc.desc: RunAsUnpackCommand.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ShellCommandTest, RunAsUnpackCommand_0700, Function | MediumTest | Level1)
{
    int32_t argc = 4;
    const char *argv[] = {
        "ohos_packing_tool",
        "pack",
        "--mode",
        "hqf",
    };

    OHOS::AppPackingTool::ShellCommand shellcmd(argc, const_cast<char**>(argv), OHOS::AppPackingTool::TOOL_NAME);
    EXPECT_EQ(shellcmd.RunAsUnpackCommand(), 0);
}
} // namespace OHOS