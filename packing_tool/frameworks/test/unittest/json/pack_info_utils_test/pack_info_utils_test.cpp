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

const std::string PACKING_INFO_TEST_STRING_NOTHING = "{"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_1 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_2 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": {},"
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_3 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\""
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_NOT_SAME_1 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.application\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_NOT_SAME_2 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 2000000,"
                "\"name\": \"2.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_NOT_SAME_3 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"test\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_4 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "}"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_5 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\":[]"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_6 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\":[{}]"
    "}"
"}";

const std::string PACKING_INFO_TEST_STRING_ERROR_7 = "{"
    "\"summary\": {"
        "\"app\": {"
            "\"bundleName\": \"com.example.myapplication\","
            "\"bundleType\": \"bundleApp\","
            "\"version\": {"
                "\"code\": 1000000,"
                "\"name\": \"1.0.0\""
            "}"
        "},"
        "\"modules\":["
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
    "}"
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
 * @tc.name: MergeTwoPackInfos_0200
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos("", PACKING_INFO_STR_2, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0300
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1, "", dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0400
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_NOTHING,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0500
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_ERROR_1,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0600
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1,
                                                 PACKING_INFO_TEST_STRING_ERROR_1,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0700
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_ERROR_2,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0800
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1,
                                                 PACKING_INFO_TEST_STRING_ERROR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_0900
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_NOT_SAME_3,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1000
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1000, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_ERROR_3,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1100
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1,
                                                 PACKING_INFO_TEST_STRING_ERROR_3,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1200
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_NOT_SAME_1,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1300
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_NOT_SAME_2,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1400
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_ERROR_4,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1500
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1,
                                                 PACKING_INFO_TEST_STRING_ERROR_4,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1600
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_TEST_STRING_ERROR_5,
                                                 PACKING_INFO_STR_2,
                                                 dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfos_1700
 * @tc.desc: MergeTwoPackInfos.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_1700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::string dstPackInfoJsonStr;
    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfos(PACKING_INFO_STR_1,
                                                 PACKING_INFO_TEST_STRING_ERROR_5,
                                                 dstPackInfoJsonStr));
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

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0200
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair("", PACKING_INFO_STR_2,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0300
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, "",
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0400
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_TEST_STRING_NOTHING, PACKING_INFO_STR_2,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0500
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_TEST_STRING_NOTHING,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0600
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_TEST_STRING_ERROR_4, PACKING_INFO_STR_2,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0700
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_TEST_STRING_ERROR_4,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0800
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_TEST_STRING_ERROR_5,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_0900
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_TEST_STRING_ERROR_6,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_1000
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_1000, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_TEST_STRING_ERROR_5, PACKING_INFO_STR_2,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_1100
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_1100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-default", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_TEST_STRING_ERROR_7,
                                                              packagesMap, dstPackInfoJsonStr));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_1200
 * @tc.desc: MergeTwoPackInfosByPackagePair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_1200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PackInfoUtils packInfoUtils;
    std::map<std::string, std::string> packagesMap;
    std::string dstPackInfoJsonStr = "";
    packagesMap.insert((make_pair("entry-error", "entry")));

    EXPECT_FALSE(packInfoUtils.MergeTwoPackInfosByPackagePair(PACKING_INFO_STR_1, PACKING_INFO_STR_2,
                                                              packagesMap, dstPackInfoJsonStr));
}
}
