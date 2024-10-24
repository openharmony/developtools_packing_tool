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
#include <iostream>

#include "constants.h"
#define private public
#define protected public
#include "hqf_packager.h"
#include "appqf_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string TEST_PATH = "/data/test/";
const std::string TEST_FILE_PATH = TEST_PATH + std::string("resource/packingtool/test_file/");
const std::string APPQF_PACKAGER_FILE_PATH = TEST_FILE_PATH + std::string("appqf_packager_file/");
const std::string HQF_ETS_PATH_ONE = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample1/ets");
const std::string HQF_ETS_PATH_TWO = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample2/ets");
const std::string HQF_LIST = "/data/test/testPack/testHqfPack1.hqf,/data/test/testPack/testHqfPack2.hqf";
const std::string HQF_JSON_PATH_ONE = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample1/patch.json");
const std::string HQF_JSON_PATH_TWO = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample2/patch.json");
const std::string HQF_OUT_PATH_ONE = "/data/test/testPack/testHqfPack1.hqf";
const std::string HQF_OUT_PATH_TWO = "/data/test/testPack/testHqfPack2.hqf";
const std::string HQF_RESOURCES_PATH_ONE = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample2/resources");
const std::string HQF_RESOURCES_PATH_TWO = APPQF_PACKAGER_FILE_PATH + std::string("hqfFileExample2/resources");
const std::string OUT_PATH = "/data/test/testAppQFPack/testappqfPack.appqf";
const std::string TEST_HQF_LIST = "/data/test/testPack/testHqfPackerr.hqf";
}

class AppqfPackagetTest : public testing::Test {
public:
    AppqfPackagetTest() {}
    virtual ~AppqfPackagetTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void AppqfPackagetTest::SetUpTestCase()
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, HQF_ETS_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, HQF_RESOURCES_PATH_ONE},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HQF_OUT_PATH_ONE},
    };

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HQF_JSON_PATH_TWO},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, HQF_ETS_PATH_TWO},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, HQF_RESOURCES_PATH_TWO},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HQF_OUT_PATH_TWO},
    };

    OHOS::AppPackingTool::HqfPackager hqfPackager1(parameterMap1, resultReceiver);
    EXPECT_EQ(hqfPackager1.PreProcess(), 0);
    EXPECT_EQ(hqfPackager1.Process(), 0);
    OHOS::AppPackingTool::HqfPackager hqfPackager2(parameterMap2, resultReceiver);
    EXPECT_EQ(hqfPackager2.PreProcess(), 0);
    EXPECT_EQ(hqfPackager2.Process(), 0);
}

void AppqfPackagetTest::TearDownTestCase() {}

void AppqfPackagetTest::SetUp() {}

void AppqfPackagetTest::TearDown() {}

/*
 * @tc.name: AppqfPackage_0100
 * @tc.desc: AppqfPackage
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, AppqfPackage_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::APPQFPackager appQFPackager(parameterMap1, resultReceiver);
    EXPECT_EQ(appQFPackager.PreProcess(), 0);
    EXPECT_EQ(appQFPackager.Process(), 0);
    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: Output file not end with .apphqf.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/testPack/testAppQFPack"}
    };
    OHOS::AppPackingTool::APPQFPackager appQFPackager(parameterMap1, resultReceiver);
    EXPECT_EQ(appQFPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: Input hqf list is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::APPQFPackager appQFPackager(parameterMap1, resultReceiver);
    EXPECT_EQ(appQFPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: Input hqf list is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, "/data/test/testPack/testHqfPackerr.hqf"},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };

    OHOS::AppPackingTool::APPQFPackager appQFPackager(parameterMap1, resultReceiver);
    EXPECT_EQ(appQFPackager.PreProcess(), 1);
}

/*
 * @tc.name: PostProcess_0100
 * @tc.desc: PostProcessTest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, PostProcessTest_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, TEST_HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::APPQFPackager localAPPQFPackager(parameterMap1, resultReceiver);
    auto ret = localAPPQFPackager.PostProcess();
    EXPECT_EQ(ret, OHOS::AppPackingTool::ERR_OK);
}

/*
 * @tc.name: InitAllowedParam_0100
 * @tc.desc: InitAllowedParamTest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, InitAllowedParam_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, TEST_HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::APPQFPackager localAPPQFPackager(parameterMap1, resultReceiver);
    auto ret = localAPPQFPackager.InitAllowedParam();
    EXPECT_EQ(ret, OHOS::AppPackingTool::ERR_OK);
}

/*
 * @tc.name: CheckHqfList_0100
 * @tc.desc: CheckHqfListTest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, Casdsdasdsast_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, TEST_HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::APPQFPackager localAPPQFPackager(parameterMap1, resultReceiver);
    std::list<std::string> hqfList;
    auto localHqfList_ = localAPPQFPackager.hqfList_;
    localAPPQFPackager.hqfList_.clear();
    auto ret = localAPPQFPackager.CheckHqfList(hqfList);
    localAPPQFPackager.hqfList_ = localHqfList_;
    EXPECT_EQ(ret, false);
    hqfList.push_back(TEST_HQF_LIST);
    localAPPQFPackager.hqfList_.push_back(TEST_HQF_LIST);
    ret = localAPPQFPackager.CheckHqfList(hqfList);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Process_0100
 * @tc.desc: ProcessTest
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppqfPackagetTest, ProcessTest_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_HQF_LIST, TEST_HQF_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::APPQFPackager localAPPQFPackager(parameterMap1, resultReceiver);
    auto ret = localAPPQFPackager.Process();
    EXPECT_EQ(ret, OHOS::AppPackingTool::ERR_OK);
}
} // namespace OHOS