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
#include <set>
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
int64_t g_liveJsonAllocations = 0;

void* CountJsonAllocation(size_t size)
{
    void* memory = std::malloc(size);
    if (memory != nullptr) {
        ++g_liveJsonAllocations;
    }
    return memory;
}

void CountJsonFree(void* memory)
{
    if (memory != nullptr) {
        --g_liveJsonAllocations;
    }
    std::free(memory);
}

class JsonAllocationGuard final {
public:
    JsonAllocationGuard()
    {
        g_liveJsonAllocations = 0;
        cJSON_Hooks hooks = {CountJsonAllocation, CountJsonFree};
        cJSON_InitHooks(&hooks);
    }
    ~JsonAllocationGuard()
    {
        cJSON_InitHooks(nullptr);
    }
    JsonAllocationGuard(const JsonAllocationGuard&) = delete;
    JsonAllocationGuard& operator=(const JsonAllocationGuard&) = delete;
};

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

/*
 * @tc.name: MergeSelectedVariantsOnce
 * @tc.desc: Keep all selected module variants once and filter packages by their physical names.
 * @tc.type: FUNC
 */
HWTEST_F(PackInfoUtilsTest, MergeSelectedVariantsOnce, Function | MediumTest | Level1)
{
    using OHOS::AppPackingTool::PackInfo;
    using OHOS::AppPackingTool::PackInfoUtils;
    using OHOS::AppPackingTool::PtJson;
    const std::string base = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[]},"packages":[]})";
    const std::string source = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["phone"]},
        {"distro":{"moduleName":"shared"},"deviceType":["tablet"]},
        {"distro":{"moduleName":"unused"},"deviceType":["tv"]}]},
        "packages":[{"name":"phone"},{"name":"tablet"},{"name":"unused"}]})";
    std::map<std::string, std::string> selected = {{"phone.hsp", "shared"}, {"tablet.hsp", "shared"}};
    std::string output;
    ASSERT_TRUE(PackInfoUtils::MergeTwoPackInfosByPackagePair(base, source, selected, output));
    PackInfo result;
    ASSERT_TRUE(result.ParseFromString(output));
    std::unique_ptr<PtJson> modules;
    std::unique_ptr<PtJson> packages;
    ASSERT_TRUE(result.GetModulesObject(modules));
    ASSERT_TRUE(result.GetPackagesObject(packages));
    EXPECT_EQ(modules->GetSize(), 2);
    EXPECT_EQ(packages->GetSize(), 2);
    EXPECT_EQ(output.find("unused"), std::string::npos);
    EXPECT_NE(output.find("phone"), std::string::npos);
    EXPECT_NE(output.find("tablet"), std::string::npos);
    selected["missing.hsp"] = "shared";
    EXPECT_FALSE(PackInfoUtils::MergeTwoPackInfosByPackagePair(base, source, selected, output));
    selected = {{"phone.hsp", "missing"}};
    EXPECT_FALSE(PackInfoUtils::MergeTwoPackInfosByPackagePair(base, source, selected, output));
}

/*
 * @tc.name: MergeTwoPackInfosByPackagePair_001
 * @tc.desc: Consecutive sources retain every same-name module variant and its package metadata once.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfosByPackagePair_001, Function | MediumTest | Level1)
{
    using namespace OHOS::AppPackingTool;
    const std::string base = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[]},"packages":[]})";
    const std::string first = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["phone"]},
        {"distro":{"moduleName":"shared"},"deviceType":["tablet"]}]},
        "packages":[{"name":"phone","deviceType":["phone"]},
        {"name":"tablet","deviceType":["tablet"]}]})";
    const std::string second = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["tv"]},
        {"distro":{"moduleName":"shared"},"deviceType":["wearable"]}]},
        "packages":[{"name":"tv","deviceType":["tv"]},
        {"name":"wearable","deviceType":["wearable"]}]})";
    std::string intermediate;
    std::string output;
    ASSERT_TRUE(PackInfoUtils::MergeTwoPackInfosByPackagePair(base, first,
        {{"phone.hsp", "shared"}, {"tablet.hsp", "shared"}}, intermediate));
    ASSERT_TRUE(PackInfoUtils::MergeTwoPackInfosByPackagePair(intermediate, second,
        {{"tv.hsp", "shared"}, {"wearable.hsp", "shared"}}, output));
    PackInfo result;
    ASSERT_TRUE(result.ParseFromString(output));
    std::unique_ptr<PtJson> modules;
    std::unique_ptr<PtJson> packages;
    ASSERT_TRUE(result.GetModulesObject(modules));
    ASSERT_TRUE(result.GetPackagesObject(packages));
    ASSERT_EQ(modules->GetSize(), 4);
    ASSERT_EQ(packages->GetSize(), 4);
    std::set<std::string> moduleDevices;
    std::set<std::string> packageNames;
    for (int32_t i = 0; i < 4; ++i) {
        std::unique_ptr<PtJson> distro;
        std::unique_ptr<PtJson> devices;
        ASSERT_EQ(modules->Get(i)->GetObject("distro", &distro), Result::SUCCESS);
        std::string name;
        ASSERT_EQ(distro->GetString("moduleName", &name), Result::SUCCESS);
        EXPECT_EQ(name, "shared");
        ASSERT_EQ(modules->Get(i)->GetArray("deviceType", &devices), Result::SUCCESS);
        ASSERT_EQ(devices->GetSize(), 1);
        EXPECT_TRUE(moduleDevices.insert(devices->Get(0)->GetString()).second);
        ASSERT_EQ(packages->Get(i)->GetString("name", &name), Result::SUCCESS);
        EXPECT_TRUE(packageNames.insert(name).second);
        ASSERT_EQ(packages->Get(i)->GetArray("deviceType", &devices), Result::SUCCESS);
        ASSERT_EQ(devices->GetSize(), 1);
        EXPECT_EQ(devices->Get(0)->GetString(), name);
    }
    const std::set<std::string> expected = {"phone", "tablet", "tv", "wearable"};
    EXPECT_EQ(moduleDevices, expected);
    EXPECT_EQ(packageNames, expected);
}

/*
 * @tc.name: MergeTwoPackInfos_001
 * @tc.desc: Both merge paths copy nested metadata without modifying or sharing the source nodes.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_001, Function | MediumTest | Level1)
{
    using namespace OHOS::AppPackingTool;
    const std::string base = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[]},"packages":[]})";
    const std::string sourceJson = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["phone"],"custom":{"tag":"keep"}},
        {"distro":{"moduleName":"shared"},"deviceType":["tablet"]}]},
        "packages":[{"name":"phone","custom":{"tag":"keep"}},{"name":"tablet"}]})";
    for (const bool selectedOnly : {false, true}) {
        SCOPED_TRACE(selectedOnly);
        PackInfo destination;
        PackInfo source;
        ASSERT_TRUE(destination.ParseFromString(base));
        ASSERT_TRUE(source.ParseFromString(sourceJson));
        const auto original = source.ToString();
        if (selectedOnly) {
            ASSERT_TRUE(PackInfoUtils::MergeTwoPackInfosByPackagePair(destination, source, "phone", "shared"));
        } else {
            ASSERT_TRUE(PackInfoUtils::MergeTwoPackInfos(destination, source));
        }
        EXPECT_EQ(source.ToString(), original);
        std::unique_ptr<PtJson> modules;
        std::unique_ptr<PtJson> packages;
        std::unique_ptr<PtJson> sourceModules;
        std::unique_ptr<PtJson> sourcePackages;
        ASSERT_TRUE(destination.GetModulesObject(modules));
        ASSERT_TRUE(destination.GetPackagesObject(packages));
        ASSERT_TRUE(source.GetModulesObject(sourceModules));
        ASSERT_TRUE(source.GetPackagesObject(sourcePackages));
        ASSERT_EQ(modules->GetSize(), 2);
        ASSERT_EQ(packages->GetSize(), selectedOnly ? 1 : 2);
        EXPECT_EQ(modules->Get(0)->Stringify(), sourceModules->Get(0)->Stringify());
        EXPECT_EQ(modules->Get(1)->Stringify(), sourceModules->Get(1)->Stringify());
        EXPECT_EQ(packages->Get(0)->Stringify(), sourcePackages->Get(0)->Stringify());
        if (!selectedOnly) {
            EXPECT_EQ(packages->Get(1)->Stringify(), sourcePackages->Get(1)->Stringify());
        }
        std::unique_ptr<PtJson> moduleCustom;
        std::unique_ptr<PtJson> packageCustom;
        ASSERT_EQ(modules->Get(0)->GetObject("custom", &moduleCustom), Result::SUCCESS);
        ASSERT_EQ(packages->Get(0)->GetObject("custom", &packageCustom), Result::SUCCESS);
        ASSERT_EQ(moduleCustom->SetString("tag", "changed"), Result::SUCCESS);
        ASSERT_EQ(packageCustom->SetString("tag", "changed"), Result::SUCCESS);
        EXPECT_EQ(source.ToString(), original);
    }
}

/*
 * @tc.name: MergeTwoPackInfos_002
 * @tc.desc: String merge entry points release JSON trees on success and every failure stage exercised here.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackInfoUtilsTest, MergeTwoPackInfos_002, Function | MediumTest | Level2)
{
    using namespace OHOS::AppPackingTool;
    const std::string base = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[]},"packages":[]})";
    const std::string source = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["phone"]}]},"packages":[{"name":"phone"}]})";
    const std::string noPackages = R"({"summary":{"app":{"bundleName":"test","version":{"code":1,"name":"1.0"}},
        "modules":[{"distro":{"moduleName":"shared"},"deviceType":["phone"]}]}})";
    struct MergeCase {
        std::string first;
        std::string second;
        bool expected;
    };
    const MergeCase cases[] = {
        {base, source, true}, {"{", source, false}, {base, "{", false},
        {base, "{}", false}, {base, noPackages, false}
    };
    // This GTest target runs cases sequentially; restore the default hooks before leaving the test.
    JsonAllocationGuard guard;
    for (const auto& input : cases) {
        SCOPED_TRACE(input.second);
        std::string output;
        EXPECT_EQ(PackInfoUtils::MergeTwoPackInfos(input.first, input.second, output), input.expected);
        EXPECT_EQ(g_liveJsonAllocations, 0);
        EXPECT_EQ(PackInfoUtils::MergeTwoPackInfosByPackagePair(input.first, input.second,
            {{"phone.hsp", "shared"}}, output), input.expected);
        EXPECT_EQ(g_liveJsonAllocations, 0);
    }
    std::string output;
    EXPECT_FALSE(PackInfoUtils::MergeTwoPackInfosByPackagePair(base, source,
        {{"phone.hsp", "shared"}, {"zmissing.hsp", "shared"}}, output));
    EXPECT_EQ(g_liveJsonAllocations, 0);
}
}
