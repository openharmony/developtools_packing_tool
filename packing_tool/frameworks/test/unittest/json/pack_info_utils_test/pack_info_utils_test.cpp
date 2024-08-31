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
#include "pack_info_utils.h"
#include "pt_json.h"
#include "utils.h"
#include "log.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace {
const string PACKING_INFO_STR_1 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\": ["
            "{"
                "\"mainAbility\": \"EntryAbility\","
                "\"deviceType\": ["
                    "\"default\","
                    "\"tablet\""
                "],"
                "\"abilities\": ["
                    "{"
                        "\"name\": \"EntryAbility\","
                        "\"label\": \"$string:EntryAbility_label\""
                    "}"
                "],"
                "\"distro\": {"
                    "\"moduleType\": \"entry\","
                    "\"installationFree\": false,"
                    "\"deliveryWithInstall\": true,"
                    "\"moduleName\": \"entry\""
                "},"
                "\"extensionAbilities\": ["
                "],"
                "\"apiVersion\": {"
                    "\"compatible\": 12,"
                    "\"releaseType\": \"Canary2\","
                    "\"target\": 12"
                "}"
            "}"
        "]"
    "},"
    "\"packages\":["
        "{"
            "\"deviceType\": ["
                "\"default\","
                "\"tablet\""
            "],"
            "\"moduleType\": \"entry\","
            "\"deliveryWithInstall\": true,"
            "\"name\": \"entry-default\""
        "}"
    "]"
"}";

const string PACKING_INFO_STR_2 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\": ["
            "{"
                "\"mainAbility\": \"EntryAbility\","
                "\"deviceType\": ["
                    "\"default\","
                    "\"tablet\""
                "],"
                "\"abilities\": ["
                    "{"
                        "\"name\": \"EntryAbility\","
                        "\"label\": \"$string:EntryAbility_label\""
                    "}"
                "],"
                "\"distro\": {"
                    "\"moduleType\": \"entry\","
                    "\"installationFree\": false,"
                    "\"deliveryWithInstall\": true,"
                    "\"moduleName\": \"entry\""
                "},"
                "\"extensionAbilities\": ["
                "],"
                "\"apiVersion\": {"
                    "\"compatible\": 12,"
                    "\"releaseType\": \"Canary2\","
                    "\"target\": 12"
                "}"
            "}"
        "]"
    "},"
    "\"packages\":["
        "{"
            "\"deviceType\": ["
                "\"default\","
                "\"tablet\""
            "],"
            "\"moduleType\": \"entry\","
            "\"deliveryWithInstall\": true,"
            "\"name\": \"entry-default\""
        "}"
    "]"
"}";
}

class PackInfoUtilsTest : public testing::Test {
public:
    PackInfoUtilsTest() {}
    virtual ~PackInfoUtilsTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PackInfoUtilsTest::SetUpTestCase() {}

void PackInfoUtilsTest::TearDownTestCase() {}

void PackInfoUtilsTest::SetUp() {}

void PackInfoUtilsTest::TearDown() {}

/*
 * @tc.name: MergeTwoPackInfos_0100
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_TRUE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1, PACKING_INFO_STR_2, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0100
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_TRUE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_STR_2,
        packagesMap, dstPackInfoJsonStr));
}
}
