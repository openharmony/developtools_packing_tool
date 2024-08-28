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

#include "constants.h"
#define private public
#define protected public
#include "packager.h"
#include "app_packager.h"
#include "zip_wrapper.h"
#include "log.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string TEST_PATH = "/data/test/";
const std::string TEST_FILE_PATH = TEST_PATH + std::string("resource/packingtool/test_file/");
const std::string OUT_PATH = "/data/test.app";
const std::string HAP_PATH = TEST_FILE_PATH + std::string("hap/appPackagerHapTest.hap");
const std::string HSP_PATH = TEST_FILE_PATH + std::string("hsp/appPackagerHspTest.hsp");
const std::string PACK_INFO_PATH = TEST_FILE_PATH + std::string("packinfo/pack.info");
const std::string SIGNATURE_PATH = TEST_FILE_PATH + std::string("signature/com.test.apppackagertest.csr");
const std::string CERTIFICATE_PATH = TEST_FILE_PATH + std::string("certificate/com.test.apppackagertest.p7b");
const std::string PACK_RES_PATH = TEST_FILE_PATH + std::string("packres/pack.res");
}

class AppPackagerTest : public testing::Test {
public:
    AppPackagerTest() {}
    virtual ~AppPackagerTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void AppPackagerTest::SetUpTestCase() {}

void AppPackagerTest::TearDownTestCase() {}

void AppPackagerTest::SetUp() {}

void AppPackagerTest::TearDown() {}

/*
 * @tc.name: InitAllowedParam_0100
 * @tc.desc: InitAllowedParam.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, InitAllowedParam_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, HSP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, SIGNATURE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, CERTIFICATE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, PACK_RES_PATH},
    };

    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_EQ(appPackager.InitAllowedParam(), 0);
    EXPECT_EQ(appPackager.PreProcess(), 0);
    EXPECT_EQ(appPackager.Process(), 0);
    EXPECT_EQ(appPackager.PostProcess(), 0);

    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}
} // namespace OHOS