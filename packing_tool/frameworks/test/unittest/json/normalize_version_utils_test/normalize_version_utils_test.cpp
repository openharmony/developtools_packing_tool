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
#include <list>

#define private public
#define protected public
#include "normalize_version_utils.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {

class NormalizeVersionUtilsTest : public testing::Test {
public:
    NormalizeVersionUtilsTest() {}
    virtual ~NormalizeVersionUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void NormalizeVersionUtilsTest::SetUpTestCase() {}

void NormalizeVersionUtilsTest::TearDownTestCase() {}

void NormalizeVersionUtilsTest::SetUp() {}

void NormalizeVersionUtilsTest::TearDown() {}

/*
 * @tc.name: ToString_0100
 * @tc.desc: ToString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(NormalizeVersionUtilsTest, ToString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::NormalizeVersion normalizeVersion;
    normalizeVersion.originVersionCode = 100;
    normalizeVersion.originVersionName = "versionNameTest";
    normalizeVersion.moduleName = "nameTest";

    OHOS::AppPackingTool::NormalizeVersionUtils utils;
    EXPECT_EQ(utils.ToString(normalizeVersion), "{\"moduleName\":\"nameTest\",\"originVersionCode\":100,"
        "\"originVersionName\":\"versionNameTest\"}");
}

/*
 * @tc.name: ArrayToString_0100
 * @tc.desc: ArrayToString.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(NormalizeVersionUtilsTest, ArrayToString_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::NormalizeVersion normalizeVersion1;
    normalizeVersion1.originVersionCode = 100;
    normalizeVersion1.originVersionName = "versionNameTest";
    normalizeVersion1.moduleName = "nameTest";

    OHOS::AppPackingTool::NormalizeVersion normalizeVersion2;
    normalizeVersion2.originVersionCode = 200;
    normalizeVersion2.originVersionName = "versionNameTest2";
    normalizeVersion2.moduleName = "nameTest2";

    std::list<OHOS::AppPackingTool::NormalizeVersion> normalizeVersions;
    normalizeVersions.push_back(normalizeVersion1);
    normalizeVersions.push_back(normalizeVersion2);

    OHOS::AppPackingTool::NormalizeVersionUtils utils;
    EXPECT_EQ(utils.ArrayToString(normalizeVersions), "[{\"moduleName\":\"nameTest\",\"originVersionCode\":100,"
        "\"originVersionName\":\"versionNameTest\"},{\"moduleName\":\"nameTest2\",\"originVersionCode\":200,"
        "\"originVersionName\":\"versionNameTest2\"}]");
}
}
