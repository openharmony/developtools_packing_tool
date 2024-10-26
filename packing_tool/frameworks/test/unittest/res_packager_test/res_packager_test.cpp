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
#include <sstream>
#include <string>

#include "constants.h"
#define private public
#define protected public
#include "res_packager.h"
#undef private
#undef protected

using namespace OHOS::AppPackingTool;
using namespace testing;
using namespace testing::ext;

namespace OHOS {
const std::string OUT_PATH = "/data/test/test1.res";
const std::string ENTRY_CARD_PATH = "/data/test/EntryCard";
const std::string PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";

class ResPackagerTest : public testing::Test {
public:
    ResPackagerTest() {}
    virtual ~ResPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ResPackagerTest::SetUpTestCase() {}

void ResPackagerTest::TearDownTestCase() {}

void ResPackagerTest::SetUp() {}

void ResPackagerTest::TearDown() {}

/*
 * @tc.name: InitAllowedParam_0100
 * @tc.desc: test init allowed param
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, InitAllowedParam_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    Parameter parameter1;
    resPackager.allowedParameters_.push_back(parameter1);
    Parameter parameter2;
    resPackager.allowedParameters_.push_back(parameter2);
    EXPECT_EQ(resPackager.InitAllowedParam(), 0);
    EXPECT_EQ(resPackager.allowedParameters_.size(), 1);
}

/*
 * @tc.name: PostProcess_0100
 * @tc.desc: test post process success
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, PostProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    EXPECT_EQ(resPackager.PostProcess(), 0);
}

/*
 * @tc.name: Process_0100
 * @tc.desc: test process fail
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, Process_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    resPackager.outPath_ = "";
    EXPECT_EQ(resPackager.Process(), 1);
}

/*
 * @tc.name: Process_0200
 * @tc.desc: test process success
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, Process_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    resPackager.outPath_ = OUT_PATH;
    resPackager.entryCardPath_ = ENTRY_CARD_PATH;
    EXPECT_EQ(resPackager.Process(), 0);
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: test pre process fail
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
        std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, ""},
    };
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    EXPECT_EQ(resPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: test pre process success
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ResPackagerTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
        std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, ENTRY_CARD_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::ResPackager resPackager(parameterMap, resultReceiver);
    system("mv /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    system("mkdir /data/test/EntryCard");
    EXPECT_EQ(resPackager.PreProcess(), 0);

    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    cmd += " ";
    cmd += ENTRY_CARD_PATH;
    system(cmd.c_str());
}
} // namespace OHOS