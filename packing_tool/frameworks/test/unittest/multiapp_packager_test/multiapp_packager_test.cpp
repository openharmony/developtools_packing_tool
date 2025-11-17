/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
#include "packager.h"
#include "multiapp_packager.h"
#include "mock/mock_multiapp_module_json_utils.h"
#include "zip_wrapper.h"
#include "zip_utils.h"
#include "log.h"
#include <filesystem>
#include <fstream>
#include "utils.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test_1.app";
const std::string OUT_PATH_WITHOUT_SUFFIX = "/data/test_1";
const std::string OUT_PATH_WITHOUT_APP_SUFFIX = "/data/test_1.hap";
const std::string HAP_LIST = "/data/test/resource/packingtool/test_file/multiApp/hap/multiappPackagerHapTest.hap";
const std::string HAP_LIST_WITHOUT_SUFFIX = "/data/test/resource/packingtool/test_file/multiApp/hap/hapTest";
const std::string HSP_LIST = "/data/test/resource/packingtool/test_file/multiApp/hsp/multiappPackagerHspTest.hsp";
const std::string HSP_LIST_WITHOUT_SUFFIX = "/data/test/resource/packingtool/test_file/multiApp/hsp/hspTest";
const std::string APP_LIST = "/data/test/resource/packingtool/test_file/multiApp/hsp/multiappPackagerHspTest.app";
}
namespace fs = std::filesystem;
class MultiAppPackagerTest : public testing::Test {
public:
    MultiAppPackagerTest() {}
    virtual ~MultiAppPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void MultiAppPackagerTest::SetUpTestCase()
{}

void MultiAppPackagerTest::TearDownTestCase()
{}

void MultiAppPackagerTest::SetUp()
{}

void MultiAppPackagerTest::TearDown()
{}

void CopyFile(const std::string& srcFile, const std::string& dstFile)
{
    std::string cmd = "cp -f " + srcFile + " " + dstFile;
    system(cmd.c_str());
}

void TouchFile(const std::string& file)
{
    std::string cmd = "touch " + file;
    system(cmd.c_str());
}

void DeleteFile(const std::string& file)
{
    std::string cmd = "rm -f " + file;
    system(cmd.c_str());
}

/*
 * @tc.name: MultiAppPackager_0100
 * @tc.desc: InitAllowedParam.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, MultiAppPackager_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    MockMultiAppModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockMultiAppModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(multiAppPackager.InitAllowedParam(), 0);
    EXPECT_EQ(multiAppPackager.PreProcess(), 0);
    EXPECT_EQ(multiAppPackager.Process(), 0);
    EXPECT_EQ(multiAppPackager.PostProcess(), 0);
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, ""},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 0);
}

/*
 * @tc.name: PreProcess_0400
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, ""},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0500
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 0);
}

/*
 * @tc.name: PreProcess_0600
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, ""},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0700
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, ""},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 0);
}

/*
 * @tc.name: PreProcess_0800
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 0);
}

/*
 * @tc.name: PreProcess_0900
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_1000
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    MockMultiAppModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockMultiAppModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_1100
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_1200
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_1300
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH_WITHOUT_APP_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_1400
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    MockMultiAppModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockMultiAppModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
    EXPECT_EQ(multiAppPackager.CompressAppModeForMultiProject(), true);
}

/*
 * @tc.name: PreProcess_1500
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
    EXPECT_EQ(multiAppPackager.CompressAppModeForMultiProject(), false);
}

/*
 * @tc.name: PreProcess_1600
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    MockMultiAppModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockMultiAppModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
    EXPECT_EQ(multiAppPackager.CompressAppModeForMultiProject(), true);
}

/*
 * @tc.name: PreProcess_1700
 * @tc.desc: test pre process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PreProcess_1700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_STAT_DUPLICATE, "fail"},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.PreProcess(), 1);
}

/*
 * @tc.name: Process_0100
 * @tc.desc: test process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, Process_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.Process(), 1);
}

/*
 * @tc.name: Process_0200
 * @tc.desc: test process.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, Process_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.Process(), 1);
}

/*
 * @tc.name: ReadModuleNameFromHap_0100
 * @tc.desc: test read module name from hap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, ReadModuleNameFromHap_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.ReadModuleNameFromHap(HAP_LIST), "entry");
}

/*
 * @tc.name: ReadModuleNameFromHap_0200
 * @tc.desc: test read module name from hap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, ReadModuleNameFromHap_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.ReadModuleNameFromHap(HAP_LIST_WITHOUT_SUFFIX), "");
}

/*
 * @tc.name: ReadModuleNameFromHap_0300
 * @tc.desc: test read module name from hap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, ReadModuleNameFromHap_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST_WITHOUT_SUFFIX},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.ReadModuleNameFromHap(HSP_LIST), "library1");
}

/*
 * @tc.name: IsVerifyValidInMultiAppMode_0100
 * @tc.desc: test is valid in multi app mode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, IsVerifyValidInMultiAppMode_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.IsVerifyValidInMultiAppMode(), false);
}

/*
 * @tc.name: IsVerifyValidInMultiAppMode_0200
 * @tc.desc: test is valid in multi app mode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, IsVerifyValidInMultiAppMode_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.IsVerifyValidInMultiAppMode(), true);
}

/*
 * @tc.name: IsVerifyValidInMultiAppMode_0300
 * @tc.desc: test is valid in multi app mode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, IsVerifyValidInMultiAppMode_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, "invalid_app"},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.IsVerifyValidInMultiAppMode(), false);
}

/*
 * @tc.name: IsVerifyValidInMultiAppMode_0400
 * @tc.desc: test is valid in multi app mode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, IsVerifyValidInMultiAppMode_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, "invalid_hap"},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.IsVerifyValidInMultiAppMode(), false);
}

/*
 * @tc.name: IsVerifyValidInMultiAppMode_0500
 * @tc.desc: test is valid in multi app mode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, IsVerifyValidInMultiAppMode_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, "invalid_hsp"},
        {OHOS::AppPackingTool::Constants::PARAM_APP_LIST, APP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.IsVerifyValidInMultiAppMode(), false);
}

/*
 * @tc.name: DisposeHapAndHsp_0100
 * @tc.desc: test dispose hap and hsp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, DisposeHapAndHsp_0100, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps;
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    EXPECT_EQ(multiAppPackager.DisposeHapAndHsp(selectedHaps, "temp_test_dir", ""), "");
}

/*
 * @tc.name: DisposeHapAndHsp_0200
 * @tc.desc: test dispose hap and hsp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, DisposeHapAndHsp_0200, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps = {HAP_LIST};
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    multiAppPackager.formattedHapAndHspList_ = {HAP_LIST};
    EXPECT_NO_THROW(multiAppPackager.DisposeHapAndHsp(selectedHaps, "temp_test_dir", ""));
}

/*
 * @tc.name: DisposeHapAndHsp_0300
 * @tc.desc: test dispose hap and hsp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, DisposeHapAndHsp_0300, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps;
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    multiAppPackager.formattedHapAndHspList_ = {HAP_LIST};
    std::string finalPackInfoStr = multiAppPackager.DisposeHapAndHsp(selectedHaps, "temp_test_dir", HAP_LIST);
    EXPECT_TRUE(finalPackInfoStr.empty());
    EXPECT_EQ(selectedHaps.size(), 1);
}

/*
 * @tc.name: WritePackInfo_0100
 * @tc.desc: write pack info.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, WritePackInfo_0100, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps;
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string filePath = "temp_test_dir/pack.info";
    std::string content = "test content";
    multiAppPackager.WritePackInfo(filePath, content);
    std::ifstream freader(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(freader)), std::istreambuf_iterator<char>());
    EXPECT_EQ(fileContent, "");
}

/*
 * @tc.name: WritePackInfo_0200
 * @tc.desc: write pack info.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, WritePackInfo_0200, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps;
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string filePath = "";
    std::string content = "test content";
    multiAppPackager.WritePackInfo(filePath, content);
    std::ifstream freader(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(freader)), std::istreambuf_iterator<char>());
    EXPECT_EQ(fileContent, "");
}

/*
 * @tc.name: WritePackInfo_0300
 * @tc.desc: write pack info.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, WritePackInfo_0300, Function | MediumTest | Level1)
{
    std::list<std::string> selectedHaps;
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_LIST, HAP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string filePath = "temp_test_dir/pack.info";
    std::string content = "";
    multiAppPackager.WritePackInfo(filePath, content);
    std::ifstream freader(filePath);
    std::string fileContent((std::istreambuf_iterator<char>(freader)), std::istreambuf_iterator<char>());
    EXPECT_EQ(fileContent, content);
}

/*
 * @tc.name: PrepareFilesForCompression_0100
 * @tc.desc: prepare files for compression.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PrepareFilesForCompression_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "/data/test/resource/packingtool/test_file/temp_test_dir";
    fs::path parentDir = fs::path(tempDir).parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    multiAppPackager.PrepareFilesForCompression(fileList, tempHapDirPath,
        tempSelectedHapDirPath, finalPackInfoStr, finalPackInfoPath);
    EXPECT_TRUE(finalPackInfoStr.empty());
    EXPECT_FALSE(finalPackInfoPath.empty());
}

/*
 * @tc.name: PrepareFilesForCompression_0200
 * @tc.desc: prepare files for compression.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PrepareFilesForCompression_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    multiAppPackager.PrepareFilesForCompression(fileList, tempHapDirPath, tempSelectedHapDirPath,
     finalPackInfoStr, finalPackInfoPath);
    EXPECT_TRUE(fs::exists(tempHapDirPath));
    EXPECT_TRUE(fs::exists(tempSelectedHapDirPath));
}

/*
 * @tc.name: PrepareFilesForCompression_0300
 * @tc.desc: prepare files for compression.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PrepareFilesForCompression_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "/data/test/resource/packingtool/test_file/temp_test_dir";
    fs::path parentDir = fs::path(tempDir).parent_path();
    if (!fs::exists(parentDir)) {
        fs::create_directories(parentDir);
    }
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    multiAppPackager.PrepareFilesForCompression(fileList, tempHapDirPath,
        tempSelectedHapDirPath, finalPackInfoStr, finalPackInfoPath);
    EXPECT_TRUE(finalPackInfoStr.empty());
    EXPECT_FALSE(finalPackInfoPath.empty());
}

/*
 * @tc.name: PrepareFilesForCompression_0400
 * @tc.desc: prepare files for compression.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, PrepareFilesForCompression_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath},
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    EXPECT_TRUE(multiAppPackager.PrepareFilesForCompression(fileList, tempHapDirPath,
       tempSelectedHapDirPath, finalPackInfoStr, finalPackInfoPath));
    EXPECT_TRUE(fileList.empty());
}

/*
 * @tc.name: DisposeApp_0100
 * @tc.desc: dispose app.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, DisposeApp_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> selectedHaps;
    std::string tempDir = "temp_test_dir";
    std::string result = multiAppPackager.DisposeApp(selectedHaps, tempDir);
    EXPECT_TRUE(result.empty());
}

/*
 * @tc.name: CompressAppModeForMultiProject_0100
 * @tc.desc: compress appmode for multiproject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CompressAppModeForMultiProject_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        { OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath },
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    EXPECT_FALSE(multiAppPackager.CompressAppModeForMultiProject());
}

/*
 * @tc.name: CompressAppModeForMultiProject_0200
 * @tc.desc: compress appmode for multiproject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CompressAppModeForMultiProject_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        { OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath },
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    fs::path tempHapDirPath = tempDir + "/tempHapDir";
    fs::path tempSelectedHapDirPath = tempDir + "/tempSelectedHapDir";
    fs::create_directory(tempHapDirPath);
    fs::create_directory(tempSelectedHapDirPath);
    std::ofstream(tempHapDirPath / "hap1").close();
    std::ofstream(tempHapDirPath / "hap2").close();
    std::list<std::string> fileList;
    fileList.push_back((tempHapDirPath / "hap1").string());
    fileList.push_back((tempHapDirPath / "hap2").string());
    EXPECT_FALSE(multiAppPackager.CompressAppModeForMultiProject());
    fs::remove_all(tempDir);
}

/*
 * @tc.name: CompressAppModeForMultiProject_0300
 * @tc.desc: compress appmode for multiproject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CompressAppModeForMultiProject_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        { OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath },
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    fs::path tempHapDirPath = tempDir + "/tempHapDir";
    fs::path tempSelectedHapDirPath = tempDir + "/tempSelectedHapDir";
    fs::create_directory(tempHapDirPath);
    fs::create_directory(tempSelectedHapDirPath);
    std::ofstream(tempHapDirPath / "hap1").close();
    std::ofstream(tempHapDirPath / "hap2").close();
    std::list<std::string> fileList;
    fileList.push_back((tempHapDirPath / "hap1").string());
    fileList.push_back((tempHapDirPath / "hap2").string());
    std::string invalidHapPath = tempHapDirPath / "invalid_hap";
    fileList.push_back(invalidHapPath);
    EXPECT_FALSE(multiAppPackager.CompressAppModeForMultiProject());
    fs::remove_all(tempDir);
}

/*
 * @tc.name: CompressAppModeForMultiProject_0400
 * @tc.desc: compress appmode for multiproject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CompressAppModeForMultiProject_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::string tempDir = "temp_test_dir";
    fs::create_directory(tempDir);
    std::string outPath = tempDir + "/output.app";
    std::map<std::string, std::string> parameterMap = {
        { OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, outPath },
    };

    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    fs::path tempHapDirPath = tempDir + "/tempHapDir";
    fs::path tempSelectedHapDirPath = tempDir + "/tempSelectedHapDir";
    fs::create_directory(tempHapDirPath);
    fs::create_directory(tempSelectedHapDirPath);
    std::ofstream(tempHapDirPath / "hap1").close();
    std::ofstream(tempHapDirPath / "hap2").close();
    std::list<std::string> fileList;
    fileList.push_back((tempHapDirPath / "hap1").string());
    fileList.push_back((tempHapDirPath / "hap2").string());
    std::string finalPackInfoPath = tempDir + "/finalPackInfo";
    std::ofstream(finalPackInfoPath).close();
    std::string invalidFinalPackInfoPath = tempDir + "/invalid_finalPackInfo";
    EXPECT_FALSE(multiAppPackager.CompressAppModeForMultiProject());
    fs::remove_all(tempDir);
}

/*
 * @tc.name: GetAndCheckOutPath_0100
 * @tc.desc: GetAndCheckOutPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, GetAndCheckOutPath_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string outPath;
    EXPECT_FALSE(multiAppPackager.GetAndCheckOutPath(outPath));
}

/*
 * @tc.name: GetAndCheckHapAndHspAndAppListStr_0100
 * @tc.desc: GetAndCheckHapAndHspAndAppListStr.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, GetAndCheckHapAndHspAndAppListStr_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string hapListStr;
    std::string hspListStr;
    std::string appListStr;
    EXPECT_FALSE(multiAppPackager.GetAndCheckHapAndHspAndAppListStr(hapListStr, hspListStr, appListStr));
}

/*
 * @tc.name: GetJsonInZips_0100
 * @tc.desc: GetJsonInZips.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, GetJsonInZips_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string filePath;
    std::string jsonName;
    EXPECT_TRUE(multiAppPackager.GetJsonInZips(filePath, jsonName).empty());
}

/*
 * @tc.name: SelectHapInApp_0100
 * @tc.desc: SelectHapInApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, SelectHapInApp_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string appPath("/data/test/resource/packingtool/test_file/multiApp/hap/multiappPackagerHapTest.hap");
    std::list<std::string> selectedHaps;
    std::string tempDir("/data/test/resource/packingtool/");
    std::string finalAppPackInfo;
    EXPECT_TRUE(multiAppPackager.SelectHapInApp(appPath, selectedHaps, tempDir, finalAppPackInfo).empty());
}

/*
 * @tc.name: SelectHapInApp_0200
 * @tc.desc: SelectHapInApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, SelectHapInApp_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string appPath("/data/test/resource/packingtool/test_file/multiApp/hap/multiappPackagerHapTest.hap");
    std::list<std::string> selectedHaps;
    std::string tempDir("/data/test/resource/packingtool/");
    std::string finalAppPackInfo("pack.info");
    EXPECT_TRUE(multiAppPackager.SelectHapInApp(appPath, selectedHaps, tempDir, finalAppPackInfo).empty());
}

/*
 * @tc.name: CopyHapAndHspFromApp_0100
 * @tc.desc: CopyHapAndHspFromApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CopyHapAndHspFromApp_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string appPath("/data/test/resource/packingtool/test_file/multiApp/hap/multiappPackagerHapTest.hap");
    std::list<std::string> selectedHaps;
    std::list<std::string> selectedHapsInApp;
    std::string tempDir("/data/test/resource/packingtool/");
    EXPECT_TRUE(multiAppPackager.CopyHapAndHspFromApp(appPath, selectedHapsInApp, selectedHaps, tempDir));
}

/*
 * @tc.name: CopyHapAndHspFromApp_0200
 * @tc.desc: CopyHapAndHspFromApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(MultiAppPackagerTest, CopyHapAndHspFromApp_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::MultiAppPackager multiAppPackager(parameterMap, resultReceiver);
    std::string appPath("/data/test/resource/packingtool/test_file/multiApp/hap/multiappPackagerHapTest.hap");
    std::list<std::string> selectedHaps;
    std::list<std::string> selectedHapsInApp;
    std::string tempDir("/data/");
    EXPECT_TRUE(multiAppPackager.CopyHapAndHspFromApp(appPath, selectedHapsInApp, selectedHaps, tempDir));
}
} // namespace OHOS