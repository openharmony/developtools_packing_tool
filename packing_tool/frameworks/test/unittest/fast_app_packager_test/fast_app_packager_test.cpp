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
#include "fast_app_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/packingToolDemo-default-unsigned.app";
const std::string HAP_PATH = "/data/test/resource/packingtool/test_file/hap/entry";
const std::string PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";
const std::string HSP_PATH = "/data/test/resource/packingtool/test_file/fastAppPackagerHspTest.hsp";
}

class FastAppPackagerTest : public testing::Test {
public:
    FastAppPackagerTest() {}
    virtual ~FastAppPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void FastAppPackagerTest::SetUpTestCase() {}

void FastAppPackagerTest::TearDownTestCase() {}

void FastAppPackagerTest::SetUp() {}

void FastAppPackagerTest::TearDown() {}

/*
 * @tc.name: fastAppPackager_0100
 * @tc.desc: fastAppPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, HSP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("mv /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    system("mv /data/test/resource/packingtool/test_file/hap/entry/pack.json "
        "/data/test/resource/packingtool/test_file/hap/entry/pack.info");
    EXPECT_EQ(fastAppackager.InitAllowedParam(), 0);
    EXPECT_EQ(fastAppackager.PreProcess(), 0);
    EXPECT_EQ(fastAppackager.Process(), 0);
    EXPECT_EQ(fastAppackager.PostProcess(), 0);

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}
} // namespace OHOS