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
#include <fstream>

#include "constants.h"
#include "zip_constants.h"
#define private public
#define protected public
#include "utils.h"
#include "unzip_wrapper.h"
#include "zip_wrapper.h"
#include "zip_utils.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/zip_utils_test.hqf";
const std::string FILE_PATH = "/data/test/resource/packingtool/zip_utils_test/";
const std::string FILE_NAME = "module.json";
const std::string TEMP_PATH = "/data/test/resource/packingtool/";
}

class ZipUtilsTest : public testing::Test {
public:
    ZipUtilsTest() {}
    virtual ~ZipUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();

    bool CheckFilePath(const std::string& filePath);

    void CreateFilePath(const std::string& filePath);

    void CreateFile(const std::string& filePath);

    void DeleteFilePath(const std::string& filePath);
};

void ZipUtilsTest::SetUpTestCase() {}

void ZipUtilsTest::TearDownTestCase() {}

void ZipUtilsTest::SetUp()
{
    CreateFilePath(FILE_PATH);

    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    CreateFile(filePath);
}

void ZipUtilsTest::TearDown() {}

bool ZipUtilsTest::CheckFilePath(const std::string& filePath)
{
    std::string prefix = "/data/test/";
    if (filePath.empty()) {
        return false;
    }
    if (filePath.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), filePath.begin())) {
        return true;
    }
    return false;
}

void ZipUtilsTest::CreateFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("mkdir -p ") + filePath;
    system(cmdStr.c_str());
}

void ZipUtilsTest::CreateFile(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("touch ") + filePath;
    system(cmdStr.c_str());
    cmdStr = std::string("echo \"{\\\"name\\\": \\\"packing_tool\\\"}\" > ") + filePath;
    system(cmdStr.c_str());
}

void ZipUtilsTest::DeleteFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("rm -rf ") + filePath;
    system(cmdStr.c_str());
}

/*
 * @tc.name: Zip_0100
 * @tc.desc: zip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, Zip_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath(FILE_PATH);
    std::string zipPath(OUT_PATH);

    int ret = zipUtils.Zip(filePath, zipPath, OHOS::AppPackingTool::Constants::LIB_PATH);
    EXPECT_EQ(ret, 0);

    DeleteFilePath(OUT_PATH);
}
 
/*
 * @tc.name: Unzip_0200
 * @tc.desc: Unzip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, Unzip_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath(FILE_PATH);
    std::string tempPath(TEMP_PATH);
    std::string zipPath(OUT_PATH);

    int ret = zipUtils.Zip(filePath, zipPath, OHOS::AppPackingTool::Constants::LIB_PATH);

    ret = zipUtils.Unzip(zipPath, tempPath);
    EXPECT_EQ(ret, 0);

    DeleteFilePath(TEMP_PATH);
    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: IsFileExistsInZip_0300
 * @tc.desc: IsFileExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileExistsInZip_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);

    bool exist = zipUtils.IsFileExistsInZip(zipPath, FILE_NAME);
    EXPECT_TRUE(exist);

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: IsFileNameExistsInZip_0400
 * @tc.desc: IsFileNameExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileNameExistsInZip_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);

    auto exist = zipUtils.IsFileNameExistsInZip(zipPath, FILE_NAME);
    EXPECT_TRUE(exist);

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: GetFileContentFromZip_0500
 * @tc.desc: GetFileContentFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetFileContentFromZip_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::string fileContent;
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);

    EXPECT_TRUE(zipUtils.GetFileContentFromZip(zipPath, FILE_NAME, fileContent));
    EXPECT_TRUE(!fileContent.empty());

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: GetUnzipCurrentFileContent_0600
 * @tc.desc: GetUnzipCurrentFileContent.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetUnzipCurrentFileContent_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::string fileContent;
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);
    
    unzFile unzipFile = unzOpen64(zipPath.c_str());
    EXPECT_TRUE(zipUtils.GetUnzipCurrentFileContent(unzipFile, fileContent));
    EXPECT_TRUE(!fileContent.empty());

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: GetResourceMapFromZip_0700
 * @tc.desc: GetResourceMapFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetResourceMapFromZip_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::map<std::string, std::string> resourceMap;
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);

    EXPECT_TRUE(zipUtils.GetResourceMapFromZip(zipPath, resourceMap));

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: AddToResourceMap_0800
 * @tc.desc: AddToResourceMap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, AddToResourceMap_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath(FILE_PATH);
    std::string zipPath(OUT_PATH);
    std::string filePathInZip(FILE_NAME);
    std::map<std::string, std::string> resourceMap;
    
    int ret = zipUtils.Zip(filePath, zipPath, FILE_NAME);
    EXPECT_EQ(ret, 0);
    unzFile unzipFile = unzOpen64(zipPath.c_str());
    EXPECT_TRUE(zipUtils.AddToResourceMap(unzipFile, filePathInZip, resourceMap));

    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: Zip_0200
 * @tc.desc: Zip
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, Zip_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath(FILE_PATH);
    std::string zipPath(OUT_PATH);
    int ret = zipUtils.Zip(filePath,
                           zipPath,
                           OHOS::AppPackingTool::Constants::LIB_PATH,
                           OHOS::AppPackingTool::ZipLevel::ZIP_LEVEL_DEFAULT,
                           -1);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: Zip_0300
 * @tc.desc: Zip
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, Zip_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath(FILE_PATH);
    std::string zipPath(OUT_PATH);
    int ret = zipUtils.Zip(filePath,
                           zipPath,
                           OHOS::AppPackingTool::Constants::LIB_PATH,
                           OHOS::AppPackingTool::ZipLevel::ZIP_LEVEL_2,
                           0);
    EXPECT_EQ(ret, AppPackingTool::ZipErrCode::ZIP_ERR_SUCCESS);
    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: Unzip_0300
 * @tc.desc: Unzip
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, Unzip_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string invalidZipPath = "/invalid/path/to/zipfile.zip";
    std::string tempPath(TEMP_PATH);
    int ret = zipUtils.Unzip(invalidZipPath, tempPath);
    EXPECT_EQ(ret, OHOS::AppPackingTool::ZipErrCode::ZIP_ERR_FAILURE);
}

/*
 * @tc.name: IsFileExistsInZip_0400
 * @tc.desc: IsFileExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileExistsInZip_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string invalidZipPath = "invalid.zip";
    bool exist = zipUtils.IsFileExistsInZip(invalidZipPath, FILE_NAME);
    EXPECT_FALSE(exist);
}

/*
 * @tc.name: IsFileExistsInZip_0500
 * @tc.desc: IsFileExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileExistsInZip_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string emptyZipPath = std::string(OUT_PATH) + "/empty.zip";
    std::ofstream ofs(emptyZipPath);
    ofs.close();
    bool exist = zipUtils.IsFileExistsInZip(emptyZipPath, FILE_NAME);
    EXPECT_FALSE(exist);
    DeleteFilePath(emptyZipPath);
}

/*
 * @tc.name: IsFileExistsInZip_0600
 * @tc.desc: IsFileExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileExistsInZip_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::string nonExistentFile = "nonexistent.txt";
    zipUtils.Zip(filePath, zipPath, FILE_NAME);
    bool exist = zipUtils.IsFileExistsInZip(zipPath, nonExistentFile);
    EXPECT_FALSE(exist);
    DeleteFilePath(zipPath);
}

/*
 * @tc.name: IsFileNameExistsInZip_0500
 * @tc.desc: IsFileNameExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileNameExistsInZip_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string invalidZipPath = "invalid.zip";
    bool exist = zipUtils.IsFileNameExistsInZip(invalidZipPath, FILE_NAME);
    EXPECT_FALSE(exist);
}

/*
 * @tc.name: IsFileNameExistsInZip_0600
 * @tc.desc: IsFileNameExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileNameExistsInZip_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string emptyZipPath = std::string(OUT_PATH) + "/empty.zip";
    std::ofstream ofs(emptyZipPath);
    ofs.close();
    bool exist = zipUtils.IsFileNameExistsInZip(emptyZipPath, FILE_NAME);
    EXPECT_FALSE(exist);
    DeleteFilePath(emptyZipPath);
}

/*
 * @tc.name: IsFileNameExistsInZip_0700
 * @tc.desc: IsFileNameExistsInZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, IsFileNameExistsInZip_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::string nonExistentFile = "nonexistent.txt";
    bool exist = zipUtils.IsFileNameExistsInZip(zipPath, nonExistentFile);
    EXPECT_FALSE(exist);
    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: GetFileContentFromZip_0600
 * @tc.desc: GetFileContentFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetFileContentFromZip_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string invalidZipPath = "invalid.zip";
    std::string fileContent;
    bool result = zipUtils.GetFileContentFromZip(invalidZipPath, FILE_NAME, fileContent);
    EXPECT_FALSE(result);
    EXPECT_TRUE(fileContent.empty());
}

/*
 * @tc.name: GetFileContentFromZip_0700
 * @tc.desc: GetFileContentFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetFileContentFromZip_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string zipPath(OUT_PATH);
    std::string nonExistentFile = "nonexistent.txt";
    std::string fileContent;
    bool result = zipUtils.GetFileContentFromZip(zipPath, nonExistentFile, fileContent);
    EXPECT_FALSE(result);
    EXPECT_TRUE(fileContent.empty());
    DeleteFilePath(OUT_PATH);
}
/*
 * @tc.name: GetUnzipCurrentFileContent_0700
 * @tc.desc: GetUnzipCurrentFileContent.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetUnzipCurrentFileContent_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    unzFile unzipFile = nullptr;
    std::string fileContent;
    bool result = zipUtils.GetUnzipCurrentFileContent(unzipFile, fileContent);
    EXPECT_FALSE(result);
    EXPECT_TRUE(fileContent.empty());
}

/*
 * @tc.name: GetUnzipCurrentFileContent_0800
 * @tc.desc: GetUnzipCurrentFileContent.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetUnzipCurrentFileContent_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string zipPath(OUT_PATH);
    std::string fileContent;
    zipUtils.Zip(filePath, zipPath, FILE_NAME);
    unzFile unzipFile = unzOpen64(zipPath.c_str());
    bool result = zipUtils.GetUnzipCurrentFileContent(unzipFile, fileContent);
    unzClose(unzipFile);
    EXPECT_TRUE(result);
    EXPECT_FALSE(fileContent.empty());
    DeleteFilePath(OUT_PATH);
}

/*
 * @tc.name: AddToResourceMap_0900
 * @tc.desc: AddToResourceMap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, AddToResourceMap_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    unzFile unzipFile = nullptr;
    std::map<std::string, std::string> resourceMap;
    bool result = zipUtils.AddToResourceMap(unzipFile, FILE_NAME, resourceMap);
    EXPECT_FALSE(result);
    EXPECT_TRUE(resourceMap.empty());
}

/*
 * @tc.name: GetResourceMapFromZip_0800
 * @tc.desc: GetResourceMapFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetResourceMapFromZip_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string invalidZipPath = "invalid.zip";
    std::map<std::string, std::string> resourceMap;
    bool result = zipUtils.GetResourceMapFromZip(invalidZipPath, resourceMap);
    EXPECT_FALSE(result);
    EXPECT_TRUE(resourceMap.empty());
}

/*
 * @tc.name: GetResourceMapFromZip_0900
 * @tc.desc: GetResourceMapFromZip.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ZipUtilsTest, GetResourceMapFromZip_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ZipUtils zipUtils;
    std::string nonRegularFilePath = "/path/to/non_regular_file.zip";
    std::map<std::string, std::string> resourceMap;
    bool result = zipUtils.GetResourceMapFromZip(nonRegularFilePath, resourceMap);
    EXPECT_FALSE(result);
    EXPECT_TRUE(resourceMap.empty());
}
} // namespace OHOS