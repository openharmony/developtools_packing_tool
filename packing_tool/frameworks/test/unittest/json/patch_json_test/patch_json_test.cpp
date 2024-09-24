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
namespace {
const std::string JSON_STRING = "{"
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
}

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));
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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

    std::string test("test.json");
    FILE *fp = fopen(test.c_str(), "w");
    EXPECT_TRUE(fp != nullptr);
    if (fp != nullptr)
    {
        fwrite(JSON_STRING.c_str(), JSON_STRING.size(), 1, fp);
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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));

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
    EXPECT_TRUE(patchJson.ParseFromString(JSON_STRING));
    
    OHOS::AppPackingTool::HqfInfo hqfInfo;
    EXPECT_TRUE(patchJson.GetHqfInfo(hqfInfo));
    EXPECT_STREQ(hqfInfo.GetBundleName().c_str(), "bundleNametest");
}

/*
 * @tc.name: ParseFromString_0200
 * @tc.desc: ParseFromString
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, ParseFromString_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string jsonFile;
    bool ret = patchJson.ParseFromString(jsonFile);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: ParseFromFile_0200
 * @tc.desc: ParseFromFile
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, ParseFromFile_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string jsonFile;
    bool ret = patchJson.ParseFromFile(jsonFile);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetAppObject_0200
 * @tc.desc: GetAppObject
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetAppObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> appObj;
    bool ret = patchJson.GetAppObject(appObj);
    EXPECT_FALSE(ret);

    patchJson.ParseFromString("{\"name\":\"Json.cn\"}");
    ret = patchJson.GetAppObject(appObj);
    EXPECT_FALSE(ret);

    patchJson.ParseFromString("{\"name\": \"Json.CN\",\"app\": {}}}");
    ret = patchJson.GetAppObject(appObj);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetModuleObject_0200
 * @tc.desc: GetModuleObject
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetModuleObject_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> moduleObj;
    bool ret = patchJson.GetModuleObject(moduleObj);
    EXPECT_FALSE(ret);

    patchJson.ParseFromString("{\"name\":\"Json.cn\"}");
    ret = patchJson.GetModuleObject(moduleObj);
    EXPECT_FALSE(ret);

    patchJson.ParseFromString("{\"name\": \"Json.CN\",\"module\": {}}}");
    ret = patchJson.GetModuleObject(moduleObj);
    EXPECT_FALSE(ret);
}


/*
 * @tc.name: GetBundleName_0200
 * @tc.desc: GetBundleName
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string bundleName = "com.example.test";
    bool ret = patchJson.GetBundleName(bundleName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetBundleNameByAppObj_0100
 * @tc.desc: GetBundleNameByAppObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetBundleNameByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    
    std::unique_ptr<AppPackingTool::PtJson> appObj = nullptr;
    std::string bundleName;
    bool ret = patchJson.GetBundleNameByAppObj(appObj, bundleName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {}}");
    ret = patchJson.GetBundleNameByAppObj(appObj, bundleName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {\"bundleName\": {}}}");
    ret = patchJson.GetBundleNameByAppObj(appObj, bundleName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetVersionCode_0200
 * @tc.desc: GetVersionCode
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    int32_t versionCode;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetVersionCode(versionCode);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetVersionCodeByAppObj_0100
 * @tc.desc: GetVersionCodeByAppObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetVersionCodeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> appObj = nullptr;
    int32_t versionCode;
    bool ret = patchJson.GetVersionCodeByAppObj(appObj, versionCode);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {}}");
    ret = patchJson.GetVersionCodeByAppObj(appObj, versionCode);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {\"versionCode\": {}}}");
    ret = patchJson.GetVersionCodeByAppObj(appObj, versionCode);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetVersionName_0200
 * @tc.desc: GetVersionName
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetVersionName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string versionName;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetVersionName(versionName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetVersionNameByAppObj_0100
 * @tc.desc: GetVersionNameByAppObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetVersionNameByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    
    std::unique_ptr<AppPackingTool::PtJson> appObj = nullptr;
    std::string versionName;
    bool ret = patchJson.GetVersionNameByAppObj(appObj, versionName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {}}");
    ret = patchJson.GetVersionNameByAppObj(appObj, versionName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {\"versionName\":{}}}");
    ret = patchJson.GetVersionNameByAppObj(appObj, versionName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetPatchVersionCode_0200
 * @tc.desc: GetPatchVersionCode
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetPatchVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    
    int32_t patchVersionCode;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetPatchVersionCode(patchVersionCode);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetPatchVersionCodeByAppObj_0100
 * @tc.desc: GetPatchVersionCodeByAppObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetPatchVersionCodeByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    
    std::unique_ptr<AppPackingTool::PtJson> appObj = nullptr;
    int32_t patchVersionCode;
    bool ret = patchJson.GetPatchVersionCodeByAppObj(appObj, patchVersionCode);
    EXPECT_FALSE(ret);
	
    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {}}");
    ret = patchJson.GetPatchVersionCodeByAppObj(appObj, patchVersionCode);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {\"patchVersionCode\": {}}}");
    ret = patchJson.GetPatchVersionCodeByAppObj(appObj, patchVersionCode);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetPatchVersionName_0200
 * @tc.desc: GetPatchVersionName
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetPatchVersionName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string patchVersionName;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetPatchVersionName(patchVersionName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetPatchVersionNameByAppObj_0100
 * @tc.desc: GetPatchVersionNameByAppObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetPatchVersionNameByAppObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> appObj = nullptr;
    std::string patchVersionName;
    bool ret = patchJson.GetPatchVersionNameByAppObj(appObj, patchVersionName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {}}");
    ret = patchJson.GetPatchVersionNameByAppObj(appObj, patchVersionName);
    EXPECT_FALSE(ret);

    appObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"app\": {\"patchVersionName\": {}}}");
    ret = patchJson.GetPatchVersionNameByAppObj(appObj, patchVersionName);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetName_0200
 * @tc.desc: GetName
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string name;
    patchJson.ParseFromString("{\"name1\": \"Json.CN\"}");
    bool ret = patchJson.GetName(name);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetNameByModuleObj_0100
 * @tc.desc: GetNameByModuleObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetNameByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> moduleObj = nullptr;
    std::string name;
    bool ret = patchJson.GetNameByModuleObj(moduleObj, name);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{}");
    ret = patchJson.GetNameByModuleObj(moduleObj, name);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": {}}");
    ret = patchJson.GetNameByModuleObj(moduleObj, name);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetType_0200
 * @tc.desc: GetType
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string type;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetType(type);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetTypeByModuleObj_0100
 * @tc.desc: GetTypeByModuleObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetTypeByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;
    
    std::unique_ptr<AppPackingTool::PtJson> moduleObj = nullptr;
    std::string type;
    bool ret = patchJson.GetTypeByModuleObj(moduleObj, type);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {}}");
    ret = patchJson.GetTypeByModuleObj(moduleObj, type);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {\"type\": {}}}");
    ret = patchJson.GetTypeByModuleObj(moduleObj, type);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetDeviceTypes_0200
 * @tc.desc: GetDeviceTypes
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetDeviceTypes_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::list<std::string> deviceTypes;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetDeviceTypes(deviceTypes);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetDeviceTypesByModuleObj_0100
 * @tc.desc: GetDeviceTypesByModuleObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetDeviceTypesByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> moduleObj = nullptr;
    std::list<std::string> deviceTypes;
    bool ret = patchJson.GetDeviceTypesByModuleObj(moduleObj, deviceTypes);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {}}");
    ret = patchJson.GetDeviceTypesByModuleObj(moduleObj, deviceTypes);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {\"deviceTypes\": {}}}");
    ret = patchJson.GetDeviceTypesByModuleObj(moduleObj, deviceTypes);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetOriginalModuleHash_0200
 * @tc.desc: GetOriginalModuleHash
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetOriginalModuleHash_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::string originalModuleHash;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetOriginalModuleHash(originalModuleHash);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetOriginalModuleHashByModuleObj_0100
 * @tc.desc: GetOriginalModuleHashByModuleObj
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetOriginalModuleHashByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    std::unique_ptr<AppPackingTool::PtJson> moduleObj = nullptr;
    std::string originalModuleHash;
    bool ret = patchJson.GetOriginalModuleHashByModuleObj(moduleObj, originalModuleHash);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {}}");
    ret = patchJson.GetOriginalModuleHashByModuleObj(moduleObj, originalModuleHash);
    EXPECT_FALSE(ret);

    moduleObj = AppPackingTool::PtJson::Parse("{\"name\": \"Json.CN\",\"module\": {\"originalModuleHash\": {}}}");
    ret = patchJson.GetOriginalModuleHashByModuleObj(moduleObj, originalModuleHash);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetHqfInfo_0200
 * @tc.desc: GetHqfInfo
 * @tc.type: FUNC
 */
HWTEST_F(PatchJsonTest, GetHqfInfo_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PatchJson patchJson;

    AppPackingTool::HqfInfo hqfInfo;
    patchJson.ParseFromString("{\"name\": \"Json.CN\"}");
    bool ret = patchJson.GetHqfInfo(hqfInfo);
    EXPECT_FALSE(ret);

    patchJson.ParseFromString("{}");
    ret = patchJson.GetHqfInfo(hqfInfo);
    EXPECT_FALSE(ret);
}
} // namespace OHOS
