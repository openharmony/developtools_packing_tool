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

#define private public
#define protected public
#include "pt_json.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
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
   "},"
   "\"bundleName\": \"bundleNametest\","
   "\"versionCode\": 100,"
   "\"versionName\": \"versionNametest\","
   "\"patchVersionCode\": 200,"
   "\"patchVersionName\": \"patchVersionNametest\","
   "\"originalModuleHash\": \"originalModuleHashtest\""
"}";
}

class PtJsonTest : public testing::Test {
public:
    PtJsonTest() {}
    virtual ~PtJsonTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PtJsonTest::SetUpTestCase() {}

void PtJsonTest::TearDownTestCase() {}

void PtJsonTest::SetUp() {}

void PtJsonTest::TearDown() {}

/*
 * @tc.name: CreateObject_0100
 * @tc.desc: CreateObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, CreateObject_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonObject = ptJson.CreateObject();
    EXPECT_TRUE(ptjsonObject != NULL);
}

/*
 * @tc.name: CreateArray_0200
 * @tc.desc: CreateArray.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, CreateArray_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonArray = ptJson.CreateArray();
    EXPECT_TRUE(ptjsonArray != NULL);
}

/*
 * @tc.name: ReleaseRoot_0300
 * @tc.desc: ReleaseRoot.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, ReleaseRoot_0300, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    ptJson.ReleaseRoot();
    EXPECT_TRUE(ptJson.object_ == NULL);
}

/*
 * @tc.name: Parse_0400
 * @tc.desc: Parse.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Parse_0400, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjson = ptJson.Parse(JSON_STRING);
    EXPECT_TRUE(ptjson != NULL);
}

/*
 * @tc.name: Stringify_0500
 * @tc.desc: Stringify.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Stringify_0500, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjson = ptJson.Parse(JSON_STRING);
    EXPECT_TRUE(ptjson != NULL);
    EXPECT_TRUE(!ptjson->Stringify().empty());
}

/*
 * @tc.name: Add_0600
 * @tc.desc: Add.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Add_0600, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonObject = ptJson.CreateObject();
    EXPECT_TRUE(ptjsonObject != NULL);

    std::list<std::string> values = {"a", "b", "c"};

    EXPECT_TRUE(ptJson.Add("AAA", true));
    EXPECT_TRUE(ptJson.Add("BBB", 123));
    EXPECT_TRUE(ptJson.Add("CCC", 123.5));
    EXPECT_TRUE(ptJson.Add("DDD", "ABC"));
    EXPECT_TRUE(ptJson.Add("EEE", ptjsonObject));
    EXPECT_TRUE(ptJson.Add("FFF", values));
}

/*
 * @tc.name: Push_0700
 * @tc.desc: Push.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Push_0700, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonObject = ptJson.CreateObject();
    EXPECT_TRUE(ptjsonObject != NULL);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonArray = ptJson.CreateArray();
    EXPECT_TRUE(ptjsonArray != NULL);

    EXPECT_TRUE(ptJson.Add("AAA", true));
    EXPECT_TRUE(ptJson.Add("BBB", 123));
    EXPECT_TRUE(ptJson.Add("CCC", 123.5));
    EXPECT_TRUE(ptJson.Add("DDD", "ABC"));
    EXPECT_TRUE(ptJson.Add("EEE", ptjsonObject));

    EXPECT_TRUE(ptJson.Push(true));
    EXPECT_TRUE(ptJson.Push(123));
    EXPECT_TRUE(ptJson.Push(123.5));
    EXPECT_TRUE(ptJson.Push("ABC"));
    EXPECT_TRUE(ptJson.Push(ptjsonArray));
}

/*
 * @tc.name: Contains_0800
 * @tc.desc: Contains.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Contains_0800, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);
    EXPECT_TRUE(ptJson.Add("AAA", true));
    EXPECT_TRUE(ptJson.Remove("AAA"));
}

/*
 * @tc.name: Contains_0900
 * @tc.desc: Contains.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, Contains_0900, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);
    EXPECT_TRUE(ptJson.Add("AAA", true));
    EXPECT_TRUE(ptJson.Contains("AAA"));
}

/*
 * @tc.name: GetKey_1000
 * @tc.desc: GetKey.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, GetKey_1000, Function | MediumTest | Level1)
{
    char a[] = "{\"firstName\":\"Brett\"}";
    cJSON*root = cJSON_Parse(a);
    cJSON*item = cJSON_GetObjectItem(root, "firstName");
    OHOS::AppPackingTool::PtJson ptJson(item);
    EXPECT_TRUE(!ptJson.GetKey().empty());
    EXPECT_TRUE(!ptJson.Stringify().empty());
}

/*
 * @tc.name: GetJson_1100
 * @tc.desc: GetJson.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, GetJson_1100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    EXPECT_TRUE(ptJson.GetJson() != nullptr);
}

/*
 * @tc.name: IsBool_1200
 * @tc.desc: IsBool.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsBool_1200, Function | MediumTest | Level1)
{
    cJSON *node = cJSON_CreateBool(true);
    OHOS::AppPackingTool::PtJson ptJsonBool(node);

    EXPECT_TRUE(ptJsonBool.IsBool());
    EXPECT_TRUE(ptJsonBool.GetBool(true));
}

/*
 * @tc.name: IsNumber_1300
 * @tc.desc: IsNumber.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsNumber_1300, Function | MediumTest | Level1)
{
    cJSON *node = cJSON_CreateNumber(12345);
    OHOS::AppPackingTool::PtJson ptJsonNumber(node);

    EXPECT_TRUE(ptJsonNumber.IsNumber());
    EXPECT_EQ(ptJsonNumber.GetInt(12345), 12345);
    EXPECT_EQ(ptJsonNumber.GetInt64(12345), 12345);
    EXPECT_EQ(ptJsonNumber.GetUInt(12345), 12345);
    EXPECT_EQ(ptJsonNumber.GetUInt64(12345), 12345);
    EXPECT_EQ(ptJsonNumber.GetDouble(12345.5), 12345);
}

/*
 * @tc.name: IsString_1400
 * @tc.desc: IsString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsString_1400, Function | MediumTest | Level1)
{
    cJSON *node = cJSON_CreateString("abcd");
    OHOS::AppPackingTool::PtJson ptJsonString(node);

    EXPECT_TRUE(ptJsonString.IsString());
    EXPECT_STREQ(ptJsonString.GetString().c_str(), "abcd");
}

/*
 * @tc.name: IsObject_1500
 * @tc.desc: IsObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsObject_1500, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonObject = ptJson.CreateObject();
    EXPECT_TRUE(ptjsonObject != NULL);
    EXPECT_TRUE(ptjsonObject->IsObject());
}

/*
 * @tc.name: IsArray_1600
 * @tc.desc: IsArray.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsArray_1600, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonArray = ptJson.CreateArray();
    EXPECT_TRUE(ptjsonArray != NULL);
    EXPECT_TRUE(ptjsonArray->IsArray());
    ptjsonArray->Push("11111");
    ptjsonArray->Push("22222");
    EXPECT_STREQ(ptjsonArray->Get(1)->GetString().c_str(), "22222");
    EXPECT_TRUE(ptjsonArray->GetSize() > 0);
}

/*
 * @tc.name: IsNull_1700
 * @tc.desc: IsNull.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, IsNull_1700, Function | MediumTest | Level1)
{
    cJSON *cjson = cJSON_CreateNull();
    OHOS::AppPackingTool::PtJson ptJson(cjson);
    EXPECT_TRUE(ptJson.IsNull());
}

/*
 * @tc.name: SetBool_1800
 * @tc.desc: SetBool.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, SetBool_1800, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool flag = false;
    ptJson.Add("AAA", true);
    EXPECT_EQ(ptJson.SetBool("AAA", flag), OHOS::AppPackingTool::Result::SUCCESS);
    ptJson.GetBool("AAA", &flag);
    EXPECT_FALSE(flag);
}

/*
 * @tc.name: SetInt_1900
 * @tc.desc: SetInt.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, SetInt_1900, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    EXPECT_TRUE(ptJson.Add("BBB", 123));
    int number = 0;
    EXPECT_EQ(ptJson.SetInt("BBB", 321), OHOS::AppPackingTool::Result::SUCCESS);
    ptJson.GetInt("BBB", &number);
    EXPECT_EQ(number, 321);
}

/*
 * @tc.name: SetInt64_2000
 * @tc.desc: SetInt64.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, SetInt64_2000, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    EXPECT_TRUE(ptJson.Add("BBB", 123));
    EXPECT_TRUE(ptJson.Add("CCC", 123.5));

    int64_t value64 = 11111;
    EXPECT_EQ(ptJson.SetInt64("BBB", value64), OHOS::AppPackingTool::Result::SUCCESS);
    int64_t value642;
    EXPECT_EQ(ptJson.GetInt64("BBB", &value642), OHOS::AppPackingTool::Result::SUCCESS);
    EXPECT_EQ(value642, 11111);

    uint32_t value32 = 2222;
    EXPECT_EQ(ptJson.SetUInt("BBB", value32), OHOS::AppPackingTool::Result::SUCCESS);
    uint32_t value322;
    EXPECT_EQ(ptJson.GetUInt("BBB", &value322), OHOS::AppPackingTool::Result::SUCCESS);
    EXPECT_EQ(value322, 2222);

    uint64_t valueInt64 = 3333;
    EXPECT_EQ(ptJson.SetUInt64("BBB", valueInt64), OHOS::AppPackingTool::Result::SUCCESS);
    uint64_t valueInt642;
    EXPECT_EQ(ptJson.GetUInt64("BBB", &valueInt642), OHOS::AppPackingTool::Result::SUCCESS);
    EXPECT_EQ(valueInt642, 3333);

    double valueDouble = 4444;
    EXPECT_EQ(ptJson.SetDouble("CCC", valueDouble), OHOS::AppPackingTool::Result::SUCCESS);
    double valueDouble2;
    EXPECT_EQ(ptJson.GetDouble("CCC", &valueDouble2), OHOS::AppPackingTool::Result::SUCCESS);
    EXPECT_EQ(valueDouble2, 4444);
}

/*
 * @tc.name: SetString_2100
 * @tc.desc: SetString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, SetString_2100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    EXPECT_TRUE(ptJson.Add("DDD", "ABC"));
    std::string str("1234567890");
    EXPECT_EQ(ptJson.SetString("DDD", str), OHOS::AppPackingTool::Result::SUCCESS);
    std::string str2;
    EXPECT_EQ(ptJson.GetString("DDD", &str2), OHOS::AppPackingTool::Result::SUCCESS);
    EXPECT_STREQ(str2.c_str(), str.c_str());
}

/*
 * @tc.name: GetObject_2200
 * @tc.desc: GetObject.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, GetObject_2200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<OHOS::AppPackingTool::PtJson> ptjsonObject = ptJson.CreateObject();
    EXPECT_TRUE(ptjsonObject != NULL);
    EXPECT_TRUE(ptJson.Add("EEE", ptjsonObject));
    std::unique_ptr<OHOS::AppPackingTool::PtJson> Object;
    EXPECT_EQ(ptJson.GetObject("EEE", &Object), OHOS::AppPackingTool::Result::SUCCESS);
}

/*
 * @tc.name: GetArray_2300
 * @tc.desc: GetArray.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, GetArray_2300, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    ptJson.Add("FFF", ptJson.CreateArray());
    std::unique_ptr<OHOS::AppPackingTool::PtJson> Array;
    EXPECT_EQ(ptJson.GetArray("FFF", &Array), OHOS::AppPackingTool::Result::SUCCESS);
}

/*
 * @tc.name: GetAny_2400
 * @tc.desc: GetAny.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, GetAny_2400, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    ptJson.Add("FFF", ptJson.CreateArray());
    std::unique_ptr<OHOS::AppPackingTool::PtJson> Object;
    EXPECT_EQ(ptJson.GetAny("FFF", &Object), OHOS::AppPackingTool::Result::SUCCESS);
}

/*
 * @tc.name: Parse_0100
 * @tc.desc: Parse
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Parse_0100, Function | MediumTest | Level1)
{
    std::string data;
    auto ret = OHOS::AppPackingTool::PtJson::Parse(data);
    EXPECT_EQ(ret, nullptr);
}

/*
 * @tc.name: Stringify_0100
 * @tc.desc: Stringify
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Stringify_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    std::string ret = ptJson.Stringify();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: Stringify_0200
 * @tc.desc: Stringify
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Stringify_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::string ret = ptJson.Stringify();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: Add_0100
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool ret = ptJson.Add(nullptr, true);
    EXPECT_FALSE(ret);

    ret = ptJson.Add("addf", true);
    ret = ptJson.Add("addf", false);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Add_0200
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    int64_t value = 100;
    bool ret = ptJson.Add("adda", value);
    EXPECT_TRUE(ret);
}

/*
 * @tc.name: Add_0300
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0300, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    uint32_t value = 100;
    bool ret = ptJson.Add("addb", value);
    EXPECT_TRUE(ret);
}

/*
 * @tc.name: Add_0400
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0400, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    double value = 100.0;
    bool ret = ptJson.Add(nullptr, value);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Add_0500
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0500, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool ret = ptJson.Add(nullptr, "value");
    EXPECT_FALSE(ret);

    ret = ptJson.Add("addd", nullptr);
    EXPECT_FALSE(ret);

    ptJson.Add("nullptr", "nullptr");
    ret = ptJson.Add("nullptr", "nullptr");
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Add_0700
 * @tc.desc: Add
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Add_0700, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<AppPackingTool::PtJson> value = std::make_unique<AppPackingTool::PtJson>();
    bool ret = ptJson.Add(nullptr, value);
    EXPECT_FALSE(ret);

    ret = ptJson.Add("aaa", value);
    EXPECT_FALSE(ret);

    ptJson.Add("bbb", value);
    ret = ptJson.Add("bbb", value);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Push_0100
 * @tc.desc: Push
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Push_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    int64_t value = 100;
    bool ret = ptJson.Push(value);
    EXPECT_TRUE(ret);
}

/*
 * @tc.name: Push_0200
 * @tc.desc: Push
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Push_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    uint32_t value = 100;
    bool ret = ptJson.Push(value);
    EXPECT_TRUE(ret);
}

/*
 * @tc.name: Push_0300
 * @tc.desc: Push
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Push_0300, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    cJSON *node = nullptr;
    bool ret = ptJson.Push(node);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Push_0400
 * @tc.desc: Push
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Push_0400, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<AppPackingTool::PtJson> value = nullptr;
    bool ret = ptJson.Push(value);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: Remove_0100
 * @tc.desc: Remove
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, Remove_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool ret = ptJson.Remove(nullptr);
    EXPECT_FALSE(ret);
}

/*
 * @tc.name: GetKey_0100
 * @tc.desc: GetKey
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetKey_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::string ret = ptJson.GetKey();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: GetBool_0100
 * @tc.desc: GetBool
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetBool_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    bool ret = ptJson.GetBool(true);
    EXPECT_TRUE(ret);
}

/*
 * @tc.name: GetInt64_0100
 * @tc.desc: GetInt64
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetInt64_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    int64_t value = 100;
    int64_t ret = ptJson.GetInt64(value);
    EXPECT_EQ(ret, value);
}

/*
 * @tc.name: GetUInt_0100
 * @tc.desc: GetUInt
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetUInt_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    uint32_t value = 100;
    uint32_t ret = ptJson.GetUInt(value);
    EXPECT_EQ(ret, value);
}

/*
 * @tc.name: GetUInt64_0100
 * @tc.desc: GetUInt64
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetUInt64_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    uint64_t value = 100;
    uint64_t ret = ptJson.GetUInt64(value);
    EXPECT_EQ(ret, value);
}

/*
 * @tc.name: GetDouble_0100
 * @tc.desc: GetDouble
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetDouble_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    double value = 100;
    double ret = ptJson.GetDouble(value);
    EXPECT_EQ(ret, value);
}

/*
 * @tc.name: GetString_0100
 * @tc.desc: GetString
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PtJson ptJson;

    std::string ret = ptJson.GetString();
    EXPECT_EQ(ret, "");
}

/*
 * @tc.name: GetBool_0200
 * @tc.desc: GetBool
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetBool_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool * value = new bool();
    *value = true;
    AppPackingTool::Result ret = ptJson.GetBool("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);

    ptJson.Add("key", "value");
    ret = ptJson.GetBool("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::TYPE_ERROR);
}

/*
 * @tc.name: SetBool_0100
 * @tc.desc: SetBool
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetBool_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    bool value = true;
    AppPackingTool::Result ret = ptJson.SetBool("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: SetInt_0100
 * @tc.desc: SetInt
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetInt_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    int32_t value = 100;
    AppPackingTool::Result ret = ptJson.SetInt("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: SetInt64_0100
 * @tc.desc: SetInt64
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetInt64_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    int32_t value = 100;
    AppPackingTool::Result ret = ptJson.SetInt64("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: SetUInt_0100
 * @tc.desc: SetUInt
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetUInt_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    uint32_t value = 100;
    AppPackingTool::Result ret = ptJson.SetUInt("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: SetUInt64_0100
 * @tc.desc: SetUInt64
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetUInt64_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    uint32_t value = 100;
    AppPackingTool::Result ret = ptJson.SetUInt64("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: GetDouble_0200
 * @tc.desc: GetDouble
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetDouble_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    double *value = new double();
    *value = 100.0;
    AppPackingTool::Result ret = ptJson.GetDouble("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);

    ptJson.Add("key", "value");
    ret = ptJson.GetDouble("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::TYPE_ERROR);
}

/*
 * @tc.name: SetDouble_0100
 * @tc.desc: SetDouble
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetDouble_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    double value = 100.0;
    AppPackingTool::Result ret = ptJson.SetDouble("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: GetString_0200
 * @tc.desc: GetString
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetString_0200, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::string *value = new std::string();
    *value = "value";
    AppPackingTool::Result ret = ptJson.GetString("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);

    ptJson.Add("key", true);
    ret = ptJson.GetString("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::TYPE_ERROR);
}

/*
 * @tc.name: SetString_0100
 * @tc.desc: SetString
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, SetString_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::string value = "value";
    AppPackingTool::Result ret = ptJson.SetString("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);

    ptJson.Add("key", true);
    ret = ptJson.SetString("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::TYPE_ERROR);
}

/*
 * @tc.name: GetObject_0100
 * @tc.desc: GetObject
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetObject_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<AppPackingTool::PtJson> *value = new std::unique_ptr<AppPackingTool::PtJson>();
    AppPackingTool::Result ret = ptJson.GetObject("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: GetArray_0100
 * @tc.desc: GetArray
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetArray_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<AppPackingTool::PtJson> *value = new std::unique_ptr<AppPackingTool::PtJson>();
    AppPackingTool::Result ret = ptJson.GetArray("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);

    ptJson.Add("key", true);
    ret = ptJson.GetArray("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::TYPE_ERROR);
}

/*
 * @tc.name: GetAny_0100
 * @tc.desc: GetAny
 * @tc.type: FUNC
 */
HWTEST_F(PtJsonTest, GetAny_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);

    std::unique_ptr<AppPackingTool::PtJson> *value = new std::unique_ptr<AppPackingTool::PtJson>();
    AppPackingTool::Result ret = ptJson.GetAny("key", value);
    EXPECT_EQ(ret, AppPackingTool::Result::NOT_EXIST);
}

/*
 * @tc.name: SetArray_0100
 * @tc.desc: SetArray.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PtJsonTest, SetArray_0100, Function | MediumTest | Level1)
{
    cJSON *cjson = new cJSON();
    OHOS::AppPackingTool::PtJson ptJson(cjson);
    std::list<std::string> value;
    EXPECT_EQ(ptJson.SetArray("AAA", value), OHOS::AppPackingTool::Result::NOT_EXIST);
    
    ptJson.Add("AAA", true);
    EXPECT_EQ(ptJson.SetArray("AAA", value), OHOS::AppPackingTool::Result::TYPE_ERROR);

    value.push_back("123");
    value.push_back("456");
    ptJson.Add("BBB", value);
    EXPECT_EQ(ptJson.SetArray("BBB", value), OHOS::AppPackingTool::Result::SUCCESS);
}
} // namespace OHOS
