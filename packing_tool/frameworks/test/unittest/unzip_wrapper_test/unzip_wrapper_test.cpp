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
#include "zip_wrapper.h"
#include "unzip_wrapper.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test_1.hqf";
const std::string FILE_PATH = "/data/test/resource/packingtool/unzip_wrapper_test";
const std::string TEMP_PATH = "/data/temp";
}

class UnzipWrapperTest : public testing::Test {
public:
    UnzipWrapperTest() {}
    virtual ~UnzipWrapperTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void UnzipWrapperTest::SetUpTestCase() {}

void UnzipWrapperTest::TearDownTestCase() {}

void UnzipWrapperTest::SetUp() {}

void UnzipWrapperTest::TearDown() {}

/*
 * @tc.name: Open_0100
 * @tc.desc: Open.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UnzipWrapperTest, Open_0100, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    std::string filepath(FILE_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper(path);
    
    zipWrapper.Open(APPEND_STATUS_CREATE);
    int ret = zipWrapper.AddFileOrDirectoryToZip(filepath, OHOS::AppPackingTool::Constants::PARAM_ETS_PATH);
    EXPECT_EQ(ret, 0);
    zipWrapper.Close();

    OHOS::AppPackingTool::UnzipWrapper unzipWrapper;
    unzipWrapper.Open(path);
    EXPECT_TRUE(unzipWrapper.IsOpen());
    
    unzipWrapper.Close();
}

/*
 * @tc.name: Open_0200
 * @tc.desc: Open.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UnzipWrapperTest, Open_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::UnzipWrapper unzipWrapper(OUT_PATH);
    unzipWrapper.Open();
    EXPECT_TRUE(unzipWrapper.IsOpen());
    
    unzipWrapper.Close();
}

/*
 * @tc.name: UnzipFile_0300
 * @tc.desc: UnzipFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UnzipWrapperTest, UnzipFile_0300, Function | MediumTest | Level1)
{
    std::string filepath(TEMP_PATH);
    OHOS::AppPackingTool::UnzipWrapper unzipWrapper(OUT_PATH);
    unzipWrapper.Open();
    int ret = unzipWrapper.UnzipFile(TEMP_PATH);
    EXPECT_EQ(ret, 0);
    
    unzipWrapper.Close();

    std::string cmd("rm -rf ");
    cmd += TEMP_PATH;
    system(cmd.c_str());
    std::string cmd2("rm -rf ");
    cmd2 += OUT_PATH;
    system(cmd2.c_str());
}
} // namespace OHOS