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
std::string OUT_PATH = "/data/test/entry-default-unsigned.hap";
std::string SUFFIX = ".hap";
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

    OHOS::AppPackingTool::HapPackager packager(parameterMap, resultReceiver);
    EXPECT_TRUE(packager.IsOutPathValid(OUT_PATH, "true", SUFFIX));
}
} // namespace OHOS