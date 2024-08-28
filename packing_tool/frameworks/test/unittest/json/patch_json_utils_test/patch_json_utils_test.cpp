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
#include "patch_json_utils.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
using namespace std;

namespace OHOS {

std::string content = "{"
   "\"name\": \"Json.CN\","
   "\"app\": {"
       "\"bundleName\": \"bundleNametest\","
       "\"versionCode\": 100,"
       "\"versionName\": \"versionNametest\","
       "\"patchVersionCode\": 200,"
       "\"patchVersionName\": \"patchVersionNametest\""
   "},"
   "\"module\": {"
        "\"name\": \"nametest\","
        "\"type\": \"typetest\","
        "\"originalModuleHash\": \"originalModuleHashtest\","
        "\"deviceTypes\": ["
            "\"aaaaaaaaaaa\","
            "\"bbbbbbbbbbb\","
            "\"ccccccccccc\""
       "]"
   "}"
"}";

class PatchJsonUtilsTest : public testing::Test {
public:
    PatchJsonUtilsTest() {}
    virtual ~PatchJsonUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PatchJsonUtilsTest::SetUpTestCase() {}

void PatchJsonUtilsTest::TearDownTestCase() {}

void PatchJsonUtilsTest::SetUp() {}

void PatchJsonUtilsTest::TearDown() {}

 /*
 * @@tc.name: ParsePatchByJsonFilePath_0100
 * @tc.desc: test parse patch by json file path
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonUtilsTest, ParsePatchByJsonFilePath_0100, Function | MediumTest | Level1)
{
    std::string patchJsonFilePath("test.json");
    OHOS::AppPackingTool::HqfInfo hqfInfo;
    
    FILE *fp = fopen(patchJsonFilePath.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr)
    {
        fwrite(content.c_str(), content.size(), 1, fp);
        fclose(fp);
    }
    EXPECT_TRUE(OHOS::AppPackingTool::PatchJsonUtils::ParsePatchByJsonFilePath(patchJsonFilePath, hqfInfo));

    system("rm -rf test.json");
}

 /*
 * @@tc.name: ParsePatchByJsonStr_0100
 * @tc.desc: test parse patch by json string
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonUtilsTest, ParsePatchByJsonStr_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::HqfInfo hqfInfo;
    EXPECT_TRUE(OHOS::AppPackingTool::PatchJsonUtils::ParsePatchByJsonStr(content, hqfInfo));
}

}