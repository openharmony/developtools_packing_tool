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
#include "distro_filter.h"
#include "pt_json.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string policyValueJsonString = "{"
    "\"policy\": \"\","
    "\"value\": ["
        "\"xxx\""
    "]"
"}";

const std::string distroFilterJsonString = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string policyValueJsonString2 = "{"
    "\"policy\": \"xxx\","
    "\"value\": ["
        "\"xxx\""
    "]"
"}";

const std::string distroFilterJsonString2 = "{"
    "\"apiVersion\": {"
        "\"policy\": \"xxx\","
        "\"value\": ["
            "1,2,3"
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"xxx\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"xxx\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"xxx\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"xxx\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string VALUE_NOT_ARRAY_TEST_JSON_STRING = "{"
    "\"value\": \"xxx\""
"}";

const std::string POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"policy\": ["
    "]"
"}";

const std::string API_VERSION_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"apiVersion\": \"xxx\","
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string API_VERSION_POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": ["
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_SHAPE_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": \"xxx\","
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_SHAPE_POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": ["
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_DENSITY_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": \"xxx\","
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_DENSITY_POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": ["
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_WINDOW_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": \"xxx\","
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string SCREEN_WINDOW_POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": ["
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "}"
"}";

const std::string COUNTRY_CODE_NOT_OBJECT_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": \"xxx\""
"}";

const std::string COUNTRY_CODE_POLICY_NOT_STRING_TEST_JSON_STRING = "{"
    "\"apiVersion\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenShape\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenDensity\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"screenWindow\": {"
        "\"policy\": \"\","
        "\"value\": ["
            "\"xxx\""
        "]"
    "},"
    "\"countryCode\": {"
        "\"policy\": ["
        "]"
    "}"
"}";
}

class DistroFilterTest : public testing::Test {
public:
    DistroFilterTest() {}
    virtual ~DistroFilterTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void DistroFilterTest::SetUpTestCase() {}

void DistroFilterTest::TearDownTestCase() {}

void DistroFilterTest::SetUp() {}

void DistroFilterTest::TearDown() {}

/*
 * @tc.name: PolicyValue_ParseFromJson_0100
 * @tc.desc: PolicyValue Function Test.
 *           ParseFromJson/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJson_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(policyValueJsonString);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_TRUE(policyValue.ParseFromJson(root));
    EXPECT_TRUE(policyValue.IsEmpty());
}

/*
 * @tc.name: PolicyValue_ParseFromJson_0200
 * @tc.desc: PolicyValue Function Test.
 *           ParseFromJson/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJson_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(policyValueJsonString2);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_TRUE(policyValue.ParseFromJson(root));
    EXPECT_FALSE(policyValue.IsEmpty());
}

/*
 * @tc.name: PolicyValue_ParseFromJson_0300
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJson_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJson(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJsonApiVersion_0100
 * @tc.desc: ParseFromJsonApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJsonApiVersion_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJsonApiVersion(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJsonApiVersion_0200
 * @tc.desc: ParseFromJsonApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJsonApiVersion_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(policyValueJsonString2);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_TRUE(policyValue.ParseFromJsonApiVersion(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0100
 * @tc.desc: DistroFilter Function Test.
 *           ParseFromJson/ParseApiVersion/ParseScreenShape/ParseScreenDensity/ParseScreenWindow/ParseCountryCode
 *           /IsEmpty/Dump
 *           PolicyValue Function Test.
 *           ParseFromJson/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(distroFilterJsonString);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_TRUE(distroFilter.ParseFromJson(root));
    EXPECT_TRUE(distroFilter.IsEmpty());
    EXPECT_EQ(distroFilter.Dump(), "");
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0200
 * @tc.desc: DistroFilter Function Test.
 *           ParseFromJson/ParseApiVersion/ParseScreenShape/ParseScreenDensity/ParseScreenWindow/ParseCountryCode
 *           /IsEmpty/Dump
 *           PolicyValue Function Test.
 *           ParseFromJson/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(distroFilterJsonString2);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_TRUE(distroFilter.ParseFromJson(root));
    EXPECT_FALSE(distroFilter.IsEmpty());
    EXPECT_EQ(distroFilter.Dump(),
    "distroFilter: apiVersion: policy is xxx, "
    "value is 123 screenShape: policy is xxx, "
    "value is xxx screenDensity: policy is xxx, "
    "value is xxx screenWindow: policy is xxx, "
    "value is xxx countryCode: policy is xxx, "
    "value is xxx");
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0300
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}

/*
 * @tc.name: DistroFilter_ParseApiVersion_0100
 * @tc.desc: ParseApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseApiVersion_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseApiVersion(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenShape_0100
 * @tc.desc: ParseScreenShape Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenShape_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenShape(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenDensity_0100
 * @tc.desc: ParseScreenDensity Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenDensity_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenDensity(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenWindow_0100
 * @tc.desc: ParseScreenWindow Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenWindow_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenWindow(root));
}

/*
 * @tc.name: DistroFilter_ParseCountryCode_0100
 * @tc.desc: ParseCountryCode Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseCountryCode_0100, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = nullptr;
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseCountryCode(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJson_0400
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJson_0400, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(VALUE_NOT_ARRAY_TEST_JSON_STRING);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJson(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJson_0500
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJson_0500, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJson(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJsonApiVersion_0300
 * @tc.desc: ParseFromJsonApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJsonApiVersion_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(VALUE_NOT_ARRAY_TEST_JSON_STRING);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJsonApiVersion(root));
}

/*
 * @tc.name: PolicyValue_ParseFromJsonApiVersion_0400
 * @tc.desc: ParseFromJsonApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromJsonApiVersion_0400, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root = AppPackingTool::PtJson::Parse(POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::PolicyValue policyValue;
    EXPECT_FALSE(policyValue.ParseFromJsonApiVersion(root));
}

/*
 * @tc.name: DistroFilter_ParseApiVersion_0200
 * @tc.desc: ParseApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseApiVersion_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(API_VERSION_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseApiVersion(root));
}

/*
 * @tc.name: DistroFilter_ParseApiVersion_0300
 * @tc.desc: ParseApiVersion Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseApiVersion_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(API_VERSION_POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseApiVersion(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenShape_0200
 * @tc.desc: ParseScreenShape Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenShape_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_SHAPE_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenShape(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenShape_0300
 * @tc.desc: ParseScreenShape Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenShape_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_SHAPE_POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenShape(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenDensity_0200
 * @tc.desc: ParseScreenDensity Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenDensity_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_DENSITY_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenDensity(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenDensity_0300
 * @tc.desc: ParseScreenDensity Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenDensity_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_DENSITY_POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenDensity(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenWindow_0200
 * @tc.desc: ParseScreenWindow Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenWindow_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_WINDOW_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenWindow(root));
}

/*
 * @tc.name: DistroFilter_ParseScreenWindow_0300
 * @tc.desc: ParseScreenWindow Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseScreenWindow_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_WINDOW_POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseScreenWindow(root));
}

/*
 * @tc.name: DistroFilter_ParseCountryCode_0200
 * @tc.desc: ParseCountryCode Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseCountryCode_0200, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(COUNTRY_CODE_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseCountryCode(root));
}

/*
 * @tc.name: DistroFilter_ParseCountryCode_0300
 * @tc.desc: ParseCountryCode Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseCountryCode_0300, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(COUNTRY_CODE_POLICY_NOT_STRING_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseCountryCode(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0400
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0400, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(API_VERSION_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0500
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0500, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_SHAPE_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0600
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0600, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_DENSITY_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0700
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0700, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(SCREEN_WINDOW_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}

/*
 * @tc.name: DistroFilter_ParseFromJson_0800
 * @tc.desc: ParseFromJson Function Test.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromJson_0800, Function | MediumTest | Level1)
{
    std::unique_ptr<AppPackingTool::PtJson> root =
        AppPackingTool::PtJson::Parse(COUNTRY_CODE_NOT_OBJECT_TEST_JSON_STRING);
    AppPackingTool::DistroFilter distroFilter;
    EXPECT_FALSE(distroFilter.ParseFromJson(root));
}
}
