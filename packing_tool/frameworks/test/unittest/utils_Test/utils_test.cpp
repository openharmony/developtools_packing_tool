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
#include "utils.h"
#include "pt_json.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string FILE_PATH = "/data/test/resource/packingtool/utils_Test/";
const std::string FILE_NAME = "module.json";
const std::string TEMP_PATH = "/data/test/resource/packingtool/utils_Test/temp/";
}

class UtilsTest : public testing::Test {
public:
    UtilsTest() {}
    virtual ~UtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();

    bool CheckFilePath(const std::string& filePath);

    void CreateFilePath(const std::string& filePath);

    void CreateFile(const std::string& filePath);

    void DeleteFilePath(const std::string& filePath);
};

void UtilsTest::SetUpTestCase() {}

void UtilsTest::TearDownTestCase() {}

void UtilsTest::SetUp()
{
    CreateFilePath(TEMP_PATH);

    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    CreateFile(filePath);
}

void UtilsTest::TearDown() {}

bool UtilsTest::CheckFilePath(const std::string& filePath)
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

void UtilsTest::CreateFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("mkdir -p ") + filePath;
    system(cmdStr.c_str());
}

void UtilsTest::CreateFile(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("touch ") + filePath;
    system(cmdStr.c_str());
    cmdStr = std::string("echo \"{\\\"name\\\": \\\"packing_tool\\\"}\" > ") + filePath;
    system(cmdStr.c_str());
}

void UtilsTest::DeleteFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("rm -rf ") + filePath;
    system(cmdStr.c_str());
}

/*
 * @tc.name: GetFileContent_0100
 * @tc.desc: GetFileContent.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFileContent_0100, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string content = OHOS::AppPackingTool::Utils::GetFileContent(filePath);
    EXPECT_TRUE(!content.empty());
}

/*
 * @tc.name: ListToString_0100
 * @tc.desc: ListToString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ListToString_0100, Function | MediumTest | Level1)
{
    std::list<std::string> lst = {
        "aaaaaaa",
        "bbbbbbb",
        "ccccccc"
    };

    std::string str = OHOS::AppPackingTool::Utils::ListToString(lst);
    EXPECT_TRUE(!str.empty());
}

/*
 * @tc.name: ReplaceAll_0200
 * @tc.desc: ReplaceAll.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ReplaceAll_0200, Function | MediumTest | Level1)
{
    std::string str = {"1234567890"};
    std::string from = {"456"};
    std::string to = {"AAA"};
    str = OHOS::AppPackingTool::Utils::ReplaceAll(str, from, to);
    EXPECT_NE(str.find(to), 0);
}

/*
 * @tc.name: GetFileLength_0300
 * @tc.desc: GetFileLength.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFileLength_0300, Function | MediumTest | Level1)
{
    int64_t len = OHOS::AppPackingTool::Utils::GetFileLength(FILE_PATH);
    EXPECT_NE(len, 0);
}

/*
 * @tc.name: EndsWith_0400
 * @tc.desc: EndsWith.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, EndsWith_0400, Function | MediumTest | Level1)
{
    std::string str = {"test.txt"};
    std::string suffix = {".txt"};
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::EndsWith(str, suffix));
}

/*
 * @tc.name: CheckDisjoint_0500
 * @tc.desc: CheckDisjoint.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckDisjoint_0500, Function | MediumTest | Level1)
{
    std::list<std::string> list1 = {"111111", "2222222"};
    std::list<std::string> list2 = {"3333333", "4444444"};
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CheckDisjoint(list1, list2));
}

/*
 * @tc.name: CheckContainsAll_0600
 * @tc.desc: CheckContainsAll.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckContainsAll_0600, Function | MediumTest | Level1)
{
    std::list<std::string> list1 = {"111111", "2222222", "3333333", "4444444"};
    std::list<std::string> list2 = {"111111", "2222222"};
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CheckContainsAll(list1, list2));
}

/*
 * @tc.name: GetSha256Str_0700
 * @tc.desc: GetSha256Str.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetSha256Str_0700, Function | MediumTest | Level1)
{
    std::string str = {"1111112222222"};
    EXPECT_TRUE(!OHOS::AppPackingTool::Utils::GetSha256Str(str).empty());
}

/*
 * @tc.name: GetSha256File_0800
 * @tc.desc: GetSha256File.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetSha256File_0800, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    EXPECT_TRUE(!OHOS::AppPackingTool::Utils::GetSha256File(filePath).empty());
}

/*
 * @tc.name: IsFileExists_0900
 * @tc.desc: IsFileExists.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, IsFileExists_0900, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::IsFileExists(filePath));
}

/*
 * @tc.name: IsFile_1000
 * @tc.desc: IsFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, IsFile_1000, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::IsFile(filePath));
}

/*
 * @tc.name: IsDirectory_1100
 * @tc.desc: IsDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, IsDirectory_1100, Function | MediumTest | Level1)
{
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::IsDirectory(TEMP_PATH));
}

/*
 * @tc.name: RemoveFile_1200
 * @tc.desc: RemoveFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, RemoveFile_1200, Function | MediumTest | Level1)
{
    system("touch patch.json");
    std::string file("patch.json");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::RemoveFile(file));
}

/*
 * @tc.name: RemoveDirectory_1300
 * @tc.desc: RemoveDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, RemoveDirectory_1300, Function | MediumTest | Level1)
{
    system("mkdir tempdir");
    std::string path("tempdir");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::RemoveDirectory(path));
}

/*
 * @tc.name: GetFilePathByDir_1400
 * @tc.desc: GetFilePathByDir.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFilePathByDir_1400, Function | MediumTest | Level1)
{
    std::string path("tempdir");
    std::string file("testfile");
    std::string filePath("tempdir/testfile");
    std::string temp = OHOS::AppPackingTool::Utils::GetFilePathByDir(path, file);
    EXPECT_EQ(temp.compare(filePath), 0);
}

/*
 * @tc.name: ForceCreateDirectory_1500
 * @tc.desc: ForceCreateDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ForceCreateDirectory_1500, Function | MediumTest | Level1)
{
    std::string path("tempdir");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::ForceCreateDirectory(path));
}

/*
 * @tc.name: ForceRemoveDirectory_1600
 * @tc.desc: ForceRemoveDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ForceRemoveDirectory_1600, Function | MediumTest | Level1)
{
    std::string path("tempdir");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::ForceCreateDirectory(path));

    EXPECT_TRUE(OHOS::AppPackingTool::Utils::ForceRemoveDirectory(path));
}

/*
 * @tc.name: CopyListToSet_1700
 * @tc.desc: CopyListToSet.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CopyListToSet_1700, Function | MediumTest | Level1)
{
    std::list<std::string> lst;
    lst.push_front("test");
    std::set<std::string> st;
    OHOS::AppPackingTool::Utils::CopyListToSet(lst, st);
    auto it = st.find("test");
    EXPECT_TRUE(it != st.end());
}

/*
 * @tc.name: CheckListContain_1800
 * @tc.desc: CheckListContain.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckListContain_1800, Function | MediumTest | Level1)
{
    std::string value("test");
    std::list<std::string> lst;
    lst.push_front(value.c_str());

    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CheckListContain(lst, std::string(value)));
}

/*
 * @tc.name: GetListDistinctCount_1900
 * @tc.desc: GetListDistinctCount.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetListDistinctCount_1900, Function | MediumTest | Level1)
{
    std::string value("test");
    std::list<std::string> lst;
    lst.push_front(value.c_str());

    EXPECT_EQ(OHOS::AppPackingTool::Utils::GetListDistinctCount(lst), 1);
}

/*
 * @tc.name: GetCeilFileSize_2000
 * @tc.desc: GetCeilFileSize.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetCeilFileSize_2000, Function | MediumTest | Level1)
{
    long fileSize = 12 * 1024 * 1014L;
    int sizeLimit = 12;

    EXPECT_TRUE(OHOS::AppPackingTool::Utils::GetCeilFileSize(fileSize, sizeLimit) < 12);
}

/*
 * @tc.name: IsPositiveInteger_2100
 * @tc.desc: IsPositiveInteger.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, IsPositiveInteger_2100, Function | MediumTest | Level1)
{
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::IsPositiveInteger(std::string("123"), 0, 200));
}

/*
 * @tc.name: CheckFileName_2200
 * @tc.desc: CheckFileName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckFileName_2200, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CheckFileName(filePath, FILE_NAME));
}

/*
 * @tc.name: CheckFileSuffix_2300
 * @tc.desc: CheckFileSuffix.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckFileSuffix_2300, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string suffix("json");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CheckFileSuffix(filePath, suffix));
}

/*
 * @tc.name: GenerateUUID_2400
 * @tc.desc: GenerateUUID.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GenerateUUID_2400, Function | MediumTest | Level1)
{
    std::string id = OHOS::AppPackingTool::Utils::GenerateUUID();
    EXPECT_TRUE(!id.empty());
}

/*
 * @tc.name: CopyFile_2500
 * @tc.desc: CopyFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CopyFile_2500, Function | MediumTest | Level1)
{
    std::string srcPath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string dstPath = std::string(FILE_PATH) + std::string("dstFile");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::CopyFile(srcPath, dstPath));
    DeleteFilePath(dstPath);
}

/*
 * @tc.name: GetFormattedPath_2600
 * @tc.desc: GetFormattedPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFormattedPath_2600, Function | MediumTest | Level1)
{
    std::string path(FILE_PATH);
    std::string formattedPath;
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::GetFormattedPath(path, formattedPath));
}

/*
 * @tc.name: CheckContainsAll_2700
 * @tc.desc: CheckContainsAll.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckContainsAll_2700, Function | MediumTest | Level1)
{
    std::list<std::string> list1 = {"111111"};
    std::list<std::string> list2 = {"111111", "2222222"};
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::CheckContainsAll(list1, list2));
}

/*
 * @tc.name: GetSha256File_2800
 * @tc.desc: GetSha256File.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetSha256File_2800, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + "no.txt";
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::GetSha256File(filePath).empty());
}

/*
 * @tc.name: GetSha256Folder_2900
 * @tc.desc: GetSha256Folder.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetSha256File_2900, Function | MediumTest | Level1)
{
    std::string filePath(FILE_PATH);
    EXPECT_TRUE(!OHOS::AppPackingTool::Utils::GetSha256File(filePath).empty());
}

/*
 * @tc.name: GetFilePathByDir_3000
 * @tc.desc: GetFilePathByDir.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFilePathByDir_3000, Function | MediumTest | Level1)
{
    std::string dir;
    std::string filePath(FILE_PATH);
    std::string ret = OHOS::AppPackingTool::Utils::GetFilePathByDir(dir, filePath);
    EXPECT_EQ(ret, filePath);
}

/*
 * @tc.name: ForceCreateDirectory_3100
 * @tc.desc: ForceCreateDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ForceCreateDirectory_3100, Function | MediumTest | Level1)
{
    std::string path("tempdir/new");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::ForceCreateDirectory(path));
}

/*
 * @tc.name: ForceRemoveDirectory_3200
 * @tc.desc: ForceRemoveDirectory.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, ForceRemoveDirectory_3200, Function | MediumTest | Level1)
{
    std::string path;
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::ForceRemoveDirectory(path));
    path = "etc";
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::ForceRemoveDirectory(path));
}

/*
 * @tc.name: GetCeilFileSize_3300
 * @tc.desc: GetCeilFileSize.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetCeilFileSize_3300, Function | MediumTest | Level1)
{
    long fileSize = 12 * 1024 * 1024L;
    int sizeLimit = 12;

    EXPECT_TRUE(OHOS::AppPackingTool::Utils::GetCeilFileSize(fileSize, sizeLimit) >= 12);
}

/*
 * @tc.name: IsPositiveInteger_3400
 * @tc.desc: IsPositiveInteger.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, IsPositiveInteger_3400, Function | MediumTest | Level1)
{
    std::string number;
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::IsPositiveInteger(number, 0, 200));
    number = "123456789123456789";
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::IsPositiveInteger(number, 0, 200));
}

/*
 * @tc.name: CheckFileSuffix_3500
 * @tc.desc: CheckFileSuffix.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, CheckFileSuffix_3500, Function | MediumTest | Level1)
{
    std::string filePath = std::string(FILE_PATH) + std::string(FILE_NAME);
    std::string suffix("xxx");
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::CheckFileSuffix(filePath, suffix));
}

/*
 * @tc.name: GetFormattedPath_3600
 * @tc.desc: GetFormattedPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, GetFormattedPath_3600, Function | MediumTest | Level1)
{
    std::string path;
    std::string formattedPath;
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::GetFormattedPath(path, formattedPath));
}

/*
 * @tc.name: StringToBool_0100
 * @tc.desc: StringToBool.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, StringToBool_0100, Function | MediumTest | Level1)
{
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::StringToBool("true"));
}

/*
 * @tc.name: StringToBool_0200
 * @tc.desc: StringToBool.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, StringToBool_0200, Function | MediumTest | Level1)
{
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::StringToBool("false"));
}

/*
 * @tc.name: StringToBool_0300
 * @tc.desc: StringToBool.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, StringToBool_0300, Function | MediumTest | Level1)
{
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::StringToBool("error"));
}

/*
 * @tc.name: StringToArray_0100
 * @tc.desc: StringToArray.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, StringToArray_0100, Function | MediumTest | Level1)
{
    std::list<std::string> array;
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::StringToArray("1\n2,3\n4, 5", array));
}

/*
 * @tc.name: RemoveAllFilesInDirectory_0100
 * @tc.desc: RemoveAllFilesInDirectory
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(UtilsTest, RemoveAllFilesInDirectory_0100, Function | MediumTest | Level1)
{
    const std::string directoryPath = "/data/test/dirpath";
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::RemoveAllFilesInDirectory(directoryPath));
    system("touch /data/test/dirpath");
    EXPECT_FALSE(OHOS::AppPackingTool::Utils::RemoveAllFilesInDirectory(directoryPath));
    system("rm -rf /data/test/dirpath");
    system("mkdir /data/test/dirpath");
    system("touch /data/test/dirpath/file1");
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::RemoveAllFilesInDirectory(directoryPath));
}
} // namespace OHOS
