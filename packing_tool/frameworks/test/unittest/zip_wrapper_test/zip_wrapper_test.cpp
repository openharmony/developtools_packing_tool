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
#include <fstream>
#include <iostream>
#include <string>

#include "constants.h"
#include "zip_constants.h"
#define private public
#define protected public
#include "zip_wrapper.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test_1.hqf";
const std::string FILE_PATH = "/data/packager_test";
const std::string CONTENT = "{"
   "\"name\": \"xxx.CN\","
   "\"url\": \"http://www.xxx.cn\","
   "\"page\": 88,"
   "\"isNonProfit\": true"
"}";
}

class ZipWrapperTest : public testing::Test {
public:
    ZipWrapperTest() {}
    virtual ~ZipWrapperTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ZipWrapperTest::SetUpTestCase() {}

void ZipWrapperTest::TearDownTestCase() {}

void ZipWrapperTest::SetUp() {}

void ZipWrapperTest::TearDown() {}

/*
 * @tc.name: Open_0100
 * @tc.desc: Open.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipWrapperTest, Open_0100, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper;
    zipWrapper.Open(path);
    EXPECT_TRUE(zipWrapper.IsOpen());

    zipWrapper.Close();
    EXPECT_FALSE(zipWrapper.IsOpen());
}

/*
 * @tc.name: Open_0200
 * @tc.desc: Open.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipWrapperTest, Open_0200, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper(path);
    zipWrapper.Open(APPEND_STATUS_CREATE);
    EXPECT_TRUE(zipWrapper.IsOpen());

    zipWrapper.Close();
    EXPECT_FALSE(zipWrapper.IsOpen());
}

/*
 * @tc.name: AddFileOrDirectoryToZip_0400
 * @tc.desc: AddFileOrDirectoryToZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipWrapperTest, AddFileOrDirectoryToZip_0400, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper(path);
    
    zipWrapper.Open(APPEND_STATUS_CREATE);
    std::string filepath(FILE_PATH);
    std::string zippath(OUT_PATH);
    int ret = zipWrapper.AddFileOrDirectoryToZip(filepath, zippath);
    EXPECT_EQ(ret, 0);

    zipWrapper.Close();
}

/*
 * @tc.name: WriteStringToZip_0500
 * @tc.desc: WriteStringToZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipWrapperTest, WriteStringToZip_0500, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper(path);
    
    zipWrapper.Open(APPEND_STATUS_CREATE);
    int ret = zipWrapper.WriteStringToZip(CONTENT, OHOS::AppPackingTool::Constants::PARAM_JSON_PATH);
    EXPECT_EQ(ret, 0);
    
    std::string filepath(FILE_PATH);
    ret = zipWrapper.AddFileOrDirectoryToZip(filepath, OHOS::AppPackingTool::Constants::PARAM_LIB_PATH);
    EXPECT_EQ(ret, 0);

    ret = zipWrapper.AddFileOrDirectoryToZip(filepath, OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH);
    EXPECT_EQ(ret, 0);

    ret = zipWrapper.AddFileOrDirectoryToZip(filepath, OHOS::AppPackingTool::Constants::PARAM_ETS_PATH);
    EXPECT_EQ(ret, 0);

    zipWrapper.Close();

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: AddEmptyDirToZip_0600
 * @tc.desc: AddEmptyDirToZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipWrapperTest, AddEmptyDirToZip_0600, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    OHOS::AppPackingTool::ZipWrapper zipWrapper(path);
    fs::create_directories("temp");
    std::string emptypath("./temp");
    zipWrapper.Open(APPEND_STATUS_CREATE);
    int ret = zipWrapper.AddEmptyDirToZip(emptypath);
    EXPECT_EQ(ret, ZIP_OK);

    zipWrapper.Close();
    
    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: Open_0300
 * @tc.desc: Open
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, Open_0300, Function | MediumTest | Level1)
{
    std::string path(OUT_PATH);
    AppPackingTool::ZipWrapper zipWrapper(path);
    zipWrapper.Close();
    int32_t ret = zipWrapper.Open(APPEND_STATUS_CREATE);
    ret = zipWrapper.Open(APPEND_STATUS_CREATE);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_SUCCESS);
}

/*
 * @tc.name: Open_0400
 * @tc.desc: Open
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, Open_0400, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper;
    int32_t ret = zipWrapper.Open(APPEND_STATUS_CREATE);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: WriteStringToZip_0100
 * @tc.desc: WriteStringToZip
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, WriteStringToZip_0100, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper;

    std::string content = "content";
    std::string zipPath;
    int32_t ret = zipWrapper.WriteStringToZip(content, zipPath);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: AddEmptyDirToZip_0100
 * @tc.desc: AddEmptyDirToZip
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, AddEmptyDirToZip_0100, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper;

    fs::path fsZipPath;
    int32_t ret = zipWrapper.AddEmptyDirToZip(fsZipPath);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: AddFileToZip_0100
 * @tc.desc: AddFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, AddFileToZip_0100, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper;

    std::string filePath = "./test.txt";
    std::string zipPath = "./test.zip";
    int32_t ret = zipWrapper.AddFileToZip(filePath, zipPath);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: AddFileToZip_0200
 * @tc.desc: AddFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, AddFileToZip_0200, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper;

    fs::path fsFilePath;
    fs::path fsZipPath;
    int32_t ret = zipWrapper.AddFileToZip(fsFilePath, fsZipPath);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: AddFileToZip_0300
 * @tc.desc: AddFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(ZipWrapperTest, AddFileToZip_0300, Function | MediumTest | Level1)
{
    AppPackingTool::ZipWrapper zipWrapper("./out.zip");
    zipWrapper.Open(APPEND_STATUS_CREATE);

    std::ofstream zipf("./test.zip");
    fs::path fsFilePath("./app");
    fs::path fsZipPath("./test.zip");
    int32_t ret = zipWrapper.AddFileToZip(fsFilePath, fsZipPath);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}
} // namespace OHOS