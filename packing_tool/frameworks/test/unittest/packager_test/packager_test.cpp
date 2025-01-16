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
#include "hap_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/entry-default-unsigned.hap";
std::string g_stageJsonPath = "/data/test/module.json";
const std::string SUFFIX = ".hap";
}

class PackagerTest : public testing::Test {
public:
    PackagerTest() {}
    virtual ~PackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PackagerTest::SetUpTestCase() {}

void PackagerTest::TearDownTestCase() {}

void PackagerTest::SetUp() {}

void PackagerTest::TearDown() {}

/*
 * @tc.name: CheckForceFlag_0100
 * @tc.desc: CheckForceFlag.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, CheckForceFlag_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
    };

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_TRUE(packager.CheckForceFlag());
}

/*
 * @tc.name: IsPathValid_0200
 * @tc.desc: IsPathValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, IsPathValid_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    system("touch /data/test/entry-default-unsigned.hap");
    EXPECT_TRUE(packager.IsPathValid(OUT_PATH, true, SUFFIX));
    system("rm -f /data/test/entry-default-unsigned.hap");
}

/*
 * @tc.name: SplitDirList_0300
 * @tc.desc: SplitDirList.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, SplitDirList_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    const std::string dirList;
    std::list<std::string> fileList;
    EXPECT_TRUE(packager.SplitDirList(dirList, fileList));
}

/*
 * @tc.name: CompatibleProcess_0400
 * @tc.desc: CompatibleProcess.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, CompatibleProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    const std::string inPutPath;
    std::list<std::string> fileList;
    const std::string suffix;
    EXPECT_TRUE(packager.CompatibleProcess(inPutPath, fileList, suffix));
}

/*
 * @tc.name: IsOutPathValid_0500
 * @tc.desc: IsOutPathValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, IsOutPathValid_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    system("touch /data/test/entry-default-unsigned.hap");
    system("touch /data/test/entry-default-unsigned.har");
    system("touch /data/test/entry-default-unsigned.hsp");
    system("touch /data/test/entry-default-unsigned.app");
    system("touch /data/test/entry-default-unsigned.res");
    system("touch /data/test/entry-default-unsigned.hhh");
    std::string outPath = "/data/test/entry-default-unsigned.hap";
    std::string suffix = ".hap";

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_TRUE(packager.IsOutPathValid(outPath, "true", suffix));
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "false", suffix));
    outPath = "/data/test/entry-default-unsigned.har";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));
    suffix = ".har";
    EXPECT_TRUE(packager.IsOutPathValid(outPath, "true", suffix));
    outPath = "/data/test/entry-default-unsigned.hsp";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));
    suffix = ".hsp";
    EXPECT_TRUE(packager.IsOutPathValid(outPath, "true", suffix));
    outPath = "/data/test/entry-default-unsigned.app";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));
    suffix = ".app";
    EXPECT_TRUE(packager.IsOutPathValid(outPath, "true", suffix));
    outPath = "/data/test/entry-default-unsigned.res";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));
    suffix = ".res";
    EXPECT_TRUE(packager.IsOutPathValid(outPath, "true", suffix));
    outPath = "/data/test/entry-default-unsigned.hhh";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));
    suffix = ".abc";
    EXPECT_FALSE(packager.IsOutPathValid(outPath, "true", suffix));

    system("rm -f /data/test/entry-default-unsigned.hap");
    system("rm -f /data/test/entry-default-unsigned.har");
    system("rm -f /data/test/entry-default-unsigned.hsp");
    system("rm -f /data/test/entry-default-unsigned.app");
    system("rm -f /data/test/entry-default-unsigned.res");
    system("rm -f /data/test/entry-default-unsigned.hhh");
}

/*
 * @tc.name: CheckForceFlag_0600
 * @tc.desc: CheckForceFlag.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, CheckForceFlag_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "normal"},
    };

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_FALSE(packager.CheckForceFlag());
}

/*
 * @tc.name: SetGenerateBuildHash_0700
 * @tc.desc: SetGenerateBuildHash.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, SetGenerateBuildHash_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, g_stageJsonPath},
    };
    bool generateBuildHash = true;
    bool buildHashFinish = true;
    system("touch /data/test/module.json");

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_TRUE(packager.SetGenerateBuildHash(g_stageJsonPath, generateBuildHash, buildHashFinish));

    system("rm -f /data/test/module.json");
    EXPECT_FALSE(packager.SetGenerateBuildHash(g_stageJsonPath, generateBuildHash, buildHashFinish));
    EXPECT_FALSE(packager.CopyFileToTempDir(g_stageJsonPath));
}

/*
 * @tc.name: BuildHash_0800
 * @tc.desc: BuildHash.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, BuildHash_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    bool generateBuildHash = true;
    bool buildHashFinish = false;

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_FALSE(packager.BuildHash(buildHashFinish, generateBuildHash, parameterMap, g_stageJsonPath));
}

/*
 * @tc.name: PutBuildHash_0900
 * @tc.desc: PutBuildHash.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, PutBuildHash_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    bool buildHashFinish = true;
    std::string hash;

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_TRUE(packager.PutBuildHash(g_stageJsonPath, hash, buildHashFinish));
}

/*
 * @tc.name: IsModuleHap_1000
 * @tc.desc: IsModuleHap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, IsModuleHap_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    std::string hapPath;
    system("touch /data/test/entry-default-unsigned.hap");

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_FALSE(packager.IsModuleHap(hapPath));
    EXPECT_FALSE(packager.IsModuleHap(OUT_PATH));
    system("rm -f /data/test/entry-default-unsigned.hap");
}

/*
 * @tc.name: IsOutDirectoryValid_1100
 * @tc.desc: IsOutDirectoryValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, IsOutDirectoryValid_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_FALSE(packager.IsOutDirectoryValid());
}

/*
 * @tc.name: IsOutDirectoryValid_1200
 * @tc.desc: IsOutDirectoryValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, IsOutDirectoryValid_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
    };

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_FALSE(packager.IsOutDirectoryValid());
}

/*
 * @tc.name: MakePackage_1300
 * @tc.desc: MakePackage.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, MakePackage_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_EQ(packager.MakePackage(), AppPackingTool::ERR_INVALID_VALUE);
}

/*
 * @tc.name: CompatibleProcess_1400
 * @tc.desc: CompatibleProcess.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, CompatibleProcess_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    const std::string inPutPath = "/data/test";
    std::list<std::string> fileList;
    system("touch /data/test/module.json");
    EXPECT_TRUE(packager.CompatibleProcess(inPutPath, fileList, AppPackingTool::Constants::MODULE_JSON));
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: CompatibleProcess_1500
 * @tc.desc: CompatibleProcess.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackagerTest, CompatibleProcess_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    const std::string inPutPath = "/data/test";
    std::list<std::string> fileList;
    std::string extraSuffix = ".hhh";
    system("touch /data/test/module.json");
    EXPECT_TRUE(packager.CompatibleProcess(
        inPutPath, fileList, AppPackingTool::Constants::MODULE_JSON, extraSuffix));
    EXPECT_TRUE(packager.CompatibleProcess(
        inPutPath, fileList, extraSuffix, AppPackingTool::Constants::MODULE_JSON));
    EXPECT_TRUE(packager.CompatibleProcess(
        g_stageJsonPath, fileList, AppPackingTool::Constants::MODULE_JSON, extraSuffix));
    EXPECT_TRUE(packager.CompatibleProcess(
        g_stageJsonPath, fileList, extraSuffix, AppPackingTool::Constants::MODULE_JSON));
    EXPECT_FALSE(packager.CompatibleProcess(
        g_stageJsonPath, fileList, extraSuffix, extraSuffix));
    system("rm -f /data/test/module.json");
}
} // namespace OHOS