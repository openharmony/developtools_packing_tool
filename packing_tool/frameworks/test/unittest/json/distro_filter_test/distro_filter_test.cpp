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
#include "distro_filter.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

std::string policyValueJsonString = "{"
    "\"policy\": \"\","
    "\"value\": ["
        "\"xxx\""
    "]"
"}";

std::string policyValueString = "{"
    "\\\"policy\\\": \\\"\\\","
    "\\\"value\\\": ["
        "\\\"xxx\\\""
    "]"
"}";

std::string distroFilterJsonString = "{"
    "\"apiVersion\": \"" + policyValueString + "\","
    "\"screenShape\": \"" + policyValueString + "\","
    "\"screenDensity\": \"" + policyValueString + "\","
    "\"screenWindow\": \"" + policyValueString + "\","
    "\"countryCode\": \"" + policyValueString + "\""
"}";

std::string policyValueJsonString2 = "{"
    "\"policy\": \"xxx\","
    "\"value\": ["
        "\"xxx\""
    "]"
"}";

std::string policyValueString2 = "{"
    "\\\"policy\\\": \\\"xxx\\\","
    "\\\"value\\\": ["
        "\\\"xxx\\\""
    "]"
"}";

std::string distroFilterJsonString2 = "{"
    "\"apiVersion\": \"" + policyValueString2 + "\","
    "\"screenShape\": \"" + policyValueString2 + "\","
    "\"screenDensity\": \"" + policyValueString2 + "\","
    "\"screenWindow\": \"" + policyValueString2 + "\","
    "\"countryCode\": \"" + policyValueString2 + "\""
"}";

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
 * @tc.name: PolicyValue_ParseFromString_0100
 * @tc.desc: PolicyValue Function Test.
 *           ParseFromString/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PolicyValue policyValue;
    EXPECT_TRUE(policyValue.ParseFromString(policyValueJsonString));
    EXPECT_TRUE(policyValue.IsEmpty());
}

/*
 * @tc.name: PolicyValue_ParseFromString_0200
 * @tc.desc: PolicyValue Function Test.
 *           ParseFromString/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, PolicyValue_ParseFromString_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PolicyValue policyValue;
    EXPECT_TRUE(policyValue.ParseFromString(policyValueJsonString2));
    EXPECT_FALSE(policyValue.IsEmpty());
}

/*
 * @tc.name: DistroFilter_ParseFromString_0100
 * @tc.desc: DistroFilter Function Test.
 *           ParseFromString/ParseApiVersion/ParseScreenShape/ParseScreenDensity/ParseScreenWindow/ParseCountryCode
 *           /IsEmpty/Dump
 *           PolicyValue Function Test.
 *           ParseFromString/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    EXPECT_TRUE(distroFilter.ParseFromString(distroFilterJsonString));
    EXPECT_TRUE(distroFilter.IsEmpty());
    EXPECT_EQ(distroFilter.Dump(), "");
}

/*
 * @tc.name: DistroFilter_ParseFromString_0200
 * @tc.desc: DistroFilter Function Test.
 *           ParseFromString/ParseApiVersion/ParseScreenShape/ParseScreenDensity/ParseScreenWindow/ParseCountryCode
 *           /IsEmpty/Dump
 *           PolicyValue Function Test.
 *           ParseFromString/IsEmpty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(DistroFilterTest, DistroFilter_ParseFromString_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DistroFilter distroFilter;
    EXPECT_TRUE(distroFilter.ParseFromString(distroFilterJsonString2));
    EXPECT_FALSE(distroFilter.IsEmpty());
    EXPECT_EQ(distroFilter.Dump(),
    "distroFilter: apiVersion: policy is xxx, "
    "value is xxx screenShape: policy is xxx, "
    "value is xxx screenDensity: policy is xxx, "
    "value is xxx screenWindow: policy is xxx, "
    "value is xxx countryCode: policy is xxx, "
    "value is xxx");
}
}
