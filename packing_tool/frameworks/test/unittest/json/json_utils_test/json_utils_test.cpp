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
#define private public
#define protected public
#include "json_utils.h"
#include "utils.h"
#include "pt_json.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

#define FILE_PATH "/data/test/resource/packingtool/json_utils_test/"
const std::string MODULE_JSON_NAME = "module.json";
const std::string CONFIG_JSON_NAME = "config.json";
const std::string PATCH_JSON_NAME = "patch.json";
const std::string TEST_JSON_NAME = "test.json";

const std::string JSON_STRING = "{"
   "\"name\": \"Json.CN\","
   "\"app\": \"apptest\","
   "\"module\": {"
        "\"name\": \"nametest\","
        "\"type\": \"typetest\","
        "\"deviceTypes\": ["
            "\"aaaaaaaaaaa\","
            "\"bbbbbbbbbbb\","
            "\"ccccccccccc\""
       "]"
   "}"
"}";

class JsonUtilsTest : public testing::Test {
public:
    JsonUtilsTest() {}
    virtual ~JsonUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();

    bool CheckFilePath(const std::string& filePath);

    void CreateFilePath(const std::string& filePath);

    void CreateFile(const std::string& filePath);

    void DeleteFilePath(const std::string& filePath);
};

void JsonUtilsTest::SetUpTestCase() {}

void JsonUtilsTest::TearDownTestCase() {}

void JsonUtilsTest::SetUp()
{
    CreateFilePath(FILE_PATH);

    std::string moduleJsonPath = std::string(FILE_PATH) + MODULE_JSON_NAME;
    CreateFile(moduleJsonPath);
    std::string configJsonPath = std::string(FILE_PATH) + CONFIG_JSON_NAME;
    CreateFile(configJsonPath);
    std::string patchJsonPath = std::string(FILE_PATH) + PATCH_JSON_NAME;
    CreateFile(patchJsonPath);
}

void JsonUtilsTest::TearDown() {}


bool JsonUtilsTest::CheckFilePath(const std::string& filePath)
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

void JsonUtilsTest::CreateFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("mkdir -p ") + filePath;
    system(cmdStr.c_str());
}

void JsonUtilsTest::CreateFile(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("touch ") + filePath;
    system(cmdStr.c_str());
    FILE *fp = fopen(filePath.c_str(), "w");
    if (fp != nullptr) {
        fwrite(JSON_STRING.c_str(), JSON_STRING.size(), 1, fp);
        fclose(fp);
        fp = nullptr;
    }
}

void JsonUtilsTest::DeleteFilePath(const std::string& filePath)
{
    if (!CheckFilePath(filePath)) {
        return;
    }
    std::string cmdStr = std::string("rm -rf ") + filePath;
    system(cmdStr.c_str());
}

/*
 * @tc.name: IsModuleJson_0100
 * @tc.desc: IsModuleJson/CheckFileName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(JsonUtilsTest, IsModuleJson_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::JsonUtils jsonUtils;
    std::string moduleJsonPath = std::string(FILE_PATH) + MODULE_JSON_NAME;
    EXPECT_TRUE(jsonUtils.IsModuleJson(moduleJsonPath));
}

/*
 * @tc.name: IsConfigJson_0100
 * @tc.desc: IsConfigJson/CheckFileName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(JsonUtilsTest, IsConfigJson_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::JsonUtils jsonUtils;
    std::string configJsonPath = std::string(FILE_PATH) + CONFIG_JSON_NAME;
    EXPECT_TRUE(jsonUtils.IsConfigJson(configJsonPath));
}

/*
 * @tc.name: IsPatchJson_0100
 * @tc.desc: IsPatchJson/CheckFileName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(JsonUtilsTest, IsPatchJson_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::JsonUtils jsonUtils;
    std::string patchJsonPath = std::string(FILE_PATH) + PATCH_JSON_NAME;
    EXPECT_TRUE(jsonUtils.IsPatchJson(patchJsonPath));
}

/*
 * @tc.name: JsonFromFile_0100
 * @tc.desc: JsonFromFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(JsonUtilsTest, JsonFromFile_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::JsonUtils jsonUtils;
    std::string testJsonPath = std::string(FILE_PATH) + TEST_JSON_NAME;
    CreateFile(testJsonPath);
    EXPECT_TRUE(OHOS::AppPackingTool::Utils::IsFileExists(testJsonPath));
    EXPECT_NE(jsonUtils.JsonFromFile(testJsonPath), nullptr);
    DeleteFilePath(testJsonPath);
}

/*
 * @tc.name: JsonToFile_0100
 * @tc.desc: JsonToFile/StrToFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(JsonUtilsTest, JsonToFile_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::JsonUtils jsonUtils;
    std::string testJsonPath = std::string(FILE_PATH) + TEST_JSON_NAME;
    CreateFile(testJsonPath);
    EXPECT_TRUE(jsonUtils.JsonToFile(OHOS::AppPackingTool::PtJson::Parse(JSON_STRING), testJsonPath));
    DeleteFilePath(testJsonPath);
}
}ls -
