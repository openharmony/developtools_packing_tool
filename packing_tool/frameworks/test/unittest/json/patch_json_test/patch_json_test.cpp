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
#include "patch_json.h"
#include "pt_json.h"
#include "log.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

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

class PatchJsonTest : public testing::Test {
public:
    PatchJsonTest() {}
    virtual ~PatchJsonTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PatchJsonTest::SetUpTestCase() {}

void PatchJsonTest::TearDownTestCase() {}

void PatchJsonTest::SetUp() {}

void PatchJsonTest::TearDown() {}

/*
 * @tc.name: ParseFromString_0100
 * @tc.desc: ParseFromString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, ParseFromString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));
}

/*
 * @tc.name: ParseFromFile_0100
 * @tc.desc: ParseFromFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, ParseFromFile_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string test("test.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr)
    {
        fwrite(content.c_str(), content.size(), 1, fp);
        fclose(fp);
        EXPECT_TRUE(patchJson.ParseFromFile(test));
        system("rm -f test.json");
    }
}

/*
 * @tc.name: Release_0100
 * @tc.desc: Release.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, Release_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    patchJson.Release();
    EXPECT_TRUE(patchJson.root_ == nullptr);
}

/*
 * @tc.name: IsValid_0100
 * @tc.desc: IsValid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, IsValid_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    EXPECT_TRUE(patchJson.IsValid());
}

/*
 * @tc.name: ToString_0100
 * @tc.desc: ToString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, ToString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string contents = patchJson.ToString();
    EXPECT_TRUE(!contents.empty());
    EXPECT_TRUE(patchJson.IsValid());
}

/*
 * @tc.name: GetAppObject_0100
 * @tc.desc: GetAppObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetAppObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::unique_ptr<OHOS::AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(patchJson.GetAppObject(appObj));
}

/*
 * @tc.name: GetModuleObject_0100
 * @tc.desc: GetModuleObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetModuleObject_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::unique_ptr<OHOS::AppPackingTool::PtJson> appObj;
    EXPECT_TRUE(patchJson.GetModuleObject(appObj));
}

/*
 * @tc.name: GetBundleName_0100
 * @tc.desc: GetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string bundleName;
    EXPECT_TRUE(patchJson.GetBundleName(bundleName));
    EXPECT_STREQ(bundleName.c_str(), "bundleNametest");
}

/*
 * @tc.name: GetVersionCode_0100
 * @tc.desc: GetVersionCode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    int32_t versionCode;
    EXPECT_TRUE(patchJson.GetVersionCode(versionCode));
    EXPECT_EQ(versionCode, 100);
}

/*
 * @tc.name: GetVersionName_0100
 * @tc.desc: GetVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetVersionName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string versionName;
    EXPECT_TRUE(patchJson.GetVersionName(versionName));
    EXPECT_STREQ(versionName.c_str(), "versionNametest");;
}

/*
 * @tc.name: GetPatchVersionCode_0100
 * @tc.desc: GetPatchVersionCode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetPatchVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    int32_t patchVersionCode;
    EXPECT_TRUE(patchJson.GetPatchVersionCode(patchVersionCode));
    EXPECT_EQ(patchVersionCode, 200);
}

/*
 * @tc.name: GetPatchVersionName_0100
 * @tc.desc: GetPatchVersionName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetPatchVersionName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string patchVersionName;
    EXPECT_TRUE(patchJson.GetPatchVersionName(patchVersionName));
    EXPECT_STREQ(patchVersionName.c_str(), "patchVersionNametest");
}

/*
 * @tc.name: GetName_0100
 * @tc.desc: GetName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string name;
    EXPECT_TRUE(patchJson.GetName(name));
    EXPECT_STREQ(name.c_str(), "nametest");
}

/*
 * @tc.name: GetType_0100
 * @tc.desc: GetType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string type;
    EXPECT_TRUE(patchJson.GetType(type));
    EXPECT_STREQ(type.c_str(), "typetest");
}

/*
 * @tc.name: GetDeviceTypes_0100
 * @tc.desc: GetDeviceTypes.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetDeviceTypes_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::list<std::string> deviceTypes;
    EXPECT_TRUE(patchJson.GetDeviceTypes(deviceTypes));
    EXPECT_STREQ(deviceTypes.front().c_str(), "aaaaaaaaaaa");
}

/*
 * @tc.name: GetOriginalModuleHash_0100
 * @tc.desc: GetOriginalModuleHash.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetOriginalModuleHash_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));

    std::string originalModuleHash;
    EXPECT_TRUE(patchJson.GetOriginalModuleHash(originalModuleHash));
    EXPECT_STREQ(originalModuleHash.c_str(), "originalModuleHashtest");
}

/*
 * @tc.name: GetHqfInfo_0100
 * @tc.desc: GetHqfInfo.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PatchJsonTest, GetHqfInfo_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    EXPECT_TRUE(patchJson.ParseFromString(content));
    
    OHOS::AppPackingTool::HqfInfo hqfInfo;
    EXPECT_TRUE(patchJson.GetHqfInfo(hqfInfo));
    EXPECT_STREQ(hqfInfo.GetBundleName().c_str(), "bundleNametest");
}
} // namespace OHOS