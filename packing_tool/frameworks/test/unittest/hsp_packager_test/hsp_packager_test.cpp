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
#include "hsp_packager.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string OUT_PATH = "/data/test/library-default-unsigned.hsp";
const std::string INDEX_PATH = "/data/test/resource/packingtool/test_file/resources.index";
const std::string PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";
const std::string ETS_PATH = "/data/test/resource/packingtool/test_file/ets";
const std::string RESOURCES_PATH = "/data/test/resource/packingtool/test_file/resources";
const std::string JSON_PATH = "/data/test/resource/packingtool/test_file/module.json";
}

class HspPackagerTest : public testing::Test {
public:
    HspPackagerTest() {}
    virtual ~HspPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void HspPackagerTest::SetUpTestCase() {}

void HspPackagerTest::TearDownTestCase() {}

void HspPackagerTest::SetUp() {}

void HspPackagerTest::TearDown() {}

/*
 * @tc.name: hspPackager_0100
 * @tc.desc: hspPackager.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HspPackagerTest, hspPackager_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, ETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, RESOURCES_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, INDEX_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    system("touch /data/test/resource/packingtool/test_file/resources.index");
    system("mkdir /data/test/resource/packingtool/test_file/ets");
    system("mv /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    EXPECT_EQ(hspPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hspPackager.PreProcess(), 0);
    EXPECT_EQ(hspPackager.Process(), 0);
    EXPECT_EQ(hspPackager.PostProcess(), 0);
    system("rm -rf /data/test/resource/packingtool/test_file/ets");
    system("rm -f /data/test/resource/packingtool/test_file/resources.index");

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}
} // namespace OHOS