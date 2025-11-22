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

#include "constants.h"
#define private public
#define protected public
#include "app_packager.h"
#include "mock/mock_module_json_utils.h"
#include "scan_statdulpicate.h"
#include "utils.h"
#include "zip_wrapper.h"
#undef private
#undef protected

using namespace OHOS::AppPackingTool;
using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string TEST_PATH = "/data/test/";
const std::string TEST_FILE_PATH = TEST_PATH + std::string("resource/packingtool/test_file/");
const std::string OUT_PATH = "/data/test.app";
const std::string REPORT_PATH = "/data/scan_result";
const std::string HAP_PATH = TEST_FILE_PATH + std::string("hap/appPackagerHapTest.hap");
const std::string HSP_PATH = TEST_FILE_PATH + std::string("hsp/appPackagerHspTest.hsp");
const std::string PACK_INFO_PATH = TEST_FILE_PATH + std::string("packinfo/pack.info");
const std::string SIGNATURE_PATH = TEST_FILE_PATH + std::string("signature/com.test.apppackagertest.csr");
const std::string CERTIFICATE_PATH = TEST_FILE_PATH + std::string("certificate/com.test.apppackagertest.p7b");
const std::string PACK_RES_PATH = TEST_FILE_PATH + std::string("packres/pack.res");
const std::string UNPACK_NAME = "unpack";
const std::string SCAN_RESULT = "scan_result";
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

void AppPackagerTest::TearDown()
{
    MockModuleJsonUtils::Reset();
    std::string cmd = {"rm -f "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

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
    MockModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(appPackager.InitAllowedParam(), ERR_OK);
    EXPECT_EQ(appPackager.PreProcess(), ERR_OK);
    EXPECT_EQ(appPackager.Process(), ERR_OK);
    EXPECT_EQ(appPackager.PostProcess(), ERR_OK);
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: PreProcess.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "invalidValue"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, HSP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, SIGNATURE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, CERTIFICATE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, PACK_RES_PATH},
    };

    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_EQ(appPackager.PreProcess(), ERR_INVALID_VALUE);
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: PreProcess.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HSP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, SIGNATURE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, CERTIFICATE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, PACK_RES_PATH},
    };

    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_EQ(appPackager.PreProcess(), ERR_INVALID_VALUE);
}

/*
 * @tc.name: CheckBundleTypeConsistency_0400
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_0500
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_0600
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    series.emplace_back(false, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_0700
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_0800
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_0900
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    series.emplace_back(false, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_1000
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_1100
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_1200
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath(HAP_PATH);
    const std::string hspPath(HSP_PATH);

    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: CheckBundleTypeConsistency_1300
 * @tc.desc: CheckBundleTypeConsistency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckBundleTypeConsistency_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath;

    EXPECT_TRUE(appPackager.CheckBundleTypeConsistency(hapPath, hspPath));
}

/*
 * @tc.name: VerifyIsSharedApp_1400
 * @tc.desc: VerifyIsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsSharedApp_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> hspPath;

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_FALSE(appPackager.VerifyIsSharedApp(hspPath));
}

/*
 * @tc.name: VerifyIsSharedApp_1500
 * @tc.desc: VerifyIsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsSharedApp_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> hspPath;

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_FALSE(appPackager.VerifyIsSharedApp(hspPath));
}

/*
 * @tc.name: VerifyIsSharedApp_1600
 * @tc.desc: VerifyIsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsSharedApp_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> hspPath;

    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_TRUE(appPackager.VerifyIsSharedApp(hspPath));
}

/*
 * @tc.name: IsSharedApp_1700
 * @tc.desc: IsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsSharedApp_1700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath("not empty");
    const std::string hspPath;

    EXPECT_FALSE(appPackager.IsSharedApp(hapPath, hspPath));
    EXPECT_FALSE(appPackager.isSharedApp_);
}

/*
 * @tc.name: IsSharedApp_1800
 * @tc.desc: IsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsSharedApp_1800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath;

    EXPECT_FALSE(appPackager.IsSharedApp(hapPath, hspPath));
    EXPECT_FALSE(appPackager.isSharedApp_);
}

/*
 * @tc.name: IsSharedApp_1900
 * @tc.desc: IsSharedApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsSharedApp_1900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_TRUE(appPackager.IsSharedApp(hapPath, hspPath));
    EXPECT_TRUE(appPackager.isSharedApp_);
}

/*
 * @tc.name: VerifyIsAppService_2000
 * @tc.desc: VerifyIsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsAppService_2000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    std::list<std::string> modulePathList;

    EXPECT_FALSE(appPackager.VerifyIsAppService(modulePathList));
}

/*
 * @tc.name: VerifyIsAppService_2100
 * @tc.desc: VerifyIsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsAppService_2100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> modulePathList = { "dummyPath" };

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_FALSE(appPackager.VerifyIsAppService(modulePathList));
}

/*
 * @tc.name: VerifyIsAppService_2200
 * @tc.desc: VerifyIsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsAppService_2200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> modulePathList = { "dummyPath" };

    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_FALSE(appPackager.VerifyIsAppService(modulePathList));
}

/*
 * @tc.name: VerifyIsAppService_2300
 * @tc.desc: VerifyIsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, VerifyIsAppService_2300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    std::list<std::string> modulePathList = { "dummyPath" };

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    MockModuleJsonUtils::MockIsModuleHap(true);
    EXPECT_TRUE(appPackager.VerifyIsAppService(modulePathList));
}

/*
 * @tc.name: IsAppService_2400
 * @tc.desc: IsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsAppService_2400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_TRUE(appPackager.IsAppService(hapPath, hspPath));
    EXPECT_TRUE(appPackager.isAppService_);
}

/*
 * @tc.name: IsAppService_2500
 * @tc.desc: IsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsAppService_2500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath;
    const std::string hspPath;

    EXPECT_FALSE(appPackager.IsAppService(hapPath, hspPath));
    EXPECT_FALSE(appPackager.isAppService_);
}

/*
 * @tc.name: IsAppService_2600
 * @tc.desc: IsAppService.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsAppService_2600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_TRUE(appPackager.IsAppService(hapPath, hspPath));
    EXPECT_TRUE(appPackager.isAppService_);
}

/*
 * @tc.name: CheckInputModulePath_2700
 * @tc.desc: CheckInputModulePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckInputModulePath_2700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_FALSE(appPackager.CheckInputModulePath(hapPath, hspPath));
}

/*
 * @tc.name: CheckInputModulePath_2800
 * @tc.desc: CheckInputModulePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckInputModulePath_2800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_FALSE(appPackager.CheckInputModulePath(hapPath, hspPath));
}

/*
 * @tc.name: CheckInputModulePath_2900
 * @tc.desc: CheckInputModulePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckInputModulePath_2900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath;
    const std::string hspPath(HSP_PATH);

    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    EXPECT_TRUE(appPackager.CheckInputModulePath(hapPath, hspPath));
}

/*
 * @tc.name: CheckInputModulePath_3000
 * @tc.desc: CheckInputModulePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckInputModulePath_3000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    HapVerifyInfo hapVerifyInfo;
    const std::string hapPath(HAP_PATH);
    const std::string hspPath;

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(false, hapVerifyInfo);
    EXPECT_TRUE(appPackager.CheckInputModulePath(hapPath, hspPath));
}

/*
 * @tc.name: CheckInputModulePath_3100
 * @tc.desc: CheckInputModulePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckInputModulePath_3100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    const std::string hapPath(HAP_PATH);
    const std::string hspPath(HSP_PATH);

    EXPECT_TRUE(appPackager.CheckInputModulePath(hapPath, hspPath));
}

/*
 * @tc.name: IsVerifyValidInAppMode_4000
 * @tc.desc: IsVerifyValidInAppMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4100
 * @tc.desc: IsVerifyValidInAppMode, CheckInputModulePath returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);
    hapVerifyInfo.SetBundleType(Constants::BUNDLE_TYPE_APP_SERVICE);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4200
 * @tc.desc: IsVerifyValidInAppMode, CompatibleProcess returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, "invalid.file"},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4300
 * @tc.desc: IsVerifyValidInAppMode, CompatibleProcess returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, "invalid.file"},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    ResultSeries series;
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(Constants::TYPE_SHARED);
    series.emplace_back(true, hapVerifyInfo);

    MockModuleJsonUtils::MockGetStageHapVerifyInfo(series);
    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4400
 * @tc.desc: IsVerifyValidInAppMode, IsFileValid returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, TEST_FILE_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4500
 * @tc.desc: IsVerifyValidInAppMode, IsOutPathValid returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "false"},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: IsVerifyValidInAppMode_4600
 * @tc.desc: IsVerifyValidInAppMode, IsOutPathValid returns true.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, IsVerifyValidInAppMode_4600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_TRUE(appPackager.IsVerifyValidInAppMode());
}

/*
 * @tc.name: PrepareDirectoriesAndFiles_4700
 * @tc.desc: PrepareDirectoriesAndFiles, CompressHapAndHspFiles returns true.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, PrepareDirectoriesAndFiles_4700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    MockModuleJsonUtils::MockCheckHapsIsValid(true);
    EXPECT_TRUE(appPackager.PrepareDirectoriesAndFiles(OUT_PATH));
}

/*
 * @tc.name: PrepareDirectoriesAndFiles_4800
 * @tc.desc: PrepareDirectoriesAndFiles, CompressHapAndHspFiles returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, PrepareDirectoriesAndFiles_4800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.PrepareDirectoriesAndFiles(OUT_PATH));
}

/*
 * @tc.name: CompressHapAndHspFiles_4900
 * @tc.desc: CompressHapAndHspFiles
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressHapAndHspFiles_4900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    auto tempPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    auto hspTempDirPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    MockModuleJsonUtils::MockCheckHapsIsValid(true);
    EXPECT_TRUE(appPackager.CompressHapAndHspFiles(tempPath, hspTempDirPath));
}

/*
 * @tc.name: CompressHapAndHspFiles_5000
 * @tc.desc: CompressHapAndHspFiles, CheckHapsIsValid returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressHapAndHspFiles_5000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    auto tempPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    auto hspTempDirPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    MockModuleJsonUtils::MockCheckHapsIsValid(false);
    EXPECT_FALSE(appPackager.CompressHapAndHspFiles(tempPath, hspTempDirPath));
}

/*
 * @tc.name: CompressHapAndHspFiles_5100
 * @tc.desc: CompressHapAndHspFiles, AddHapListToApp returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressHapAndHspFiles_5100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    auto tempPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    auto hspTempDirPath = fs::path(OUT_PATH).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
        Utils::GenerateUUID());
    if (!fs::exists(tempPath)) {
        fs::create_directories(tempPath);
    }
    if (!fs::exists(hspTempDirPath)) {
        fs::create_directories(hspTempDirPath);
    }
    MockModuleJsonUtils::MockCheckHapsIsValid(true);
    appPackager.formattedHapPathList_.emplace_back(HAP_PATH);
    EXPECT_FALSE(appPackager.CompressHapAndHspFiles(tempPath, hspTempDirPath));
}

/*
 * @tc.name: AddHapListToApp_5200
 * @tc.desc: AddHapListToApp.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, AddHapListToApp_5200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_TRUE(appPackager.AddHapListToApp(appPackager.formattedHapPathList_));
    EXPECT_EQ(appPackager.zipWrapper_.zipLevel_, ZipLevel::ZIP_LEVEL_DEFAULT);
}

/*
 * @tc.name: AddHapListToApp_5300
 * @tc.desc: AddHapListToApp, AddFileOrDirectoryToZip returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, AddHapListToApp_5300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    appPackager.formattedHapPathList_.emplace_back(HAP_PATH);
    EXPECT_FALSE(appPackager.AddHapListToApp(appPackager.formattedHapPathList_));
    EXPECT_EQ(appPackager.zipWrapper_.zipLevel_, ZipLevel::ZIP_LEVEL_DEFAULT);
}

/*
 * @tc.name: AddHapListToApp_5400
 * @tc.desc: AddHapListToApp, AddFileOrDirectoryToZip returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, AddHapListToApp_5400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    MockModuleJsonUtils::MockIsModuleHap(true);
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDebug(true);
    MockModuleJsonUtils::MockGetStageHapVerifyInfo(true, hapVerifyInfo);
    appPackager.formattedHapPathList_.emplace_back(HAP_PATH);
    MockModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_FALSE(appPackager.AddHapListToApp(appPackager.formattedHapPathList_));
    EXPECT_EQ(appPackager.zipWrapper_.zipLevel_, ZipLevel::ZIP_LEVEL_DEFAULT);
}

/*
 * @tc.name: AddHapListToApp_5500
 * @tc.desc: AddHapListToApp, AddFileOrDirectoryToZip returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, AddHapListToApp_5500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    MockModuleJsonUtils::MockIsModuleHap(false);
    HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDebug(true);
    MockModuleJsonUtils::MockGetFaHapVerifyInfo(true, hapVerifyInfo);
    appPackager.formattedHapPathList_.emplace_back(HAP_PATH);
    MockModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_FALSE(appPackager.AddHapListToApp(appPackager.formattedHapPathList_));
    EXPECT_EQ(appPackager.zipWrapper_.zipLevel_, ZipLevel::ZIP_LEVEL_DEFAULT);
}

/*
 * @tc.name: CompressOtherFiles_5600
 * @tc.desc: CompressOtherFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressOtherFiles_5600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_TRUE(appPackager.CompressOtherFiles());
}

/*
 * @tc.name: CompressOtherFiles_5700
 * @tc.desc: CompressOtherFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressOtherFiles_5700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, PACK_INFO_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    appPackager.formattedEntryCardPathList_.emplace_back(HAP_PATH);
    EXPECT_FALSE(appPackager.CompressOtherFiles());
}

/*
 * @tc.name: CompressOtherFiles_5800
 * @tc.desc: CompressOtherFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressOtherFiles_5800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.CompressOtherFiles());
}

/*
 * @tc.name: CompressOtherFiles_5900
 * @tc.desc: CompressOtherFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressOtherFiles_5900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, SIGNATURE_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.CompressOtherFiles());
}

/*
 * @tc.name: CompressOtherFiles_6000
 * @tc.desc: CompressOtherFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressOtherFiles_6000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, CERTIFICATE_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    EXPECT_FALSE(appPackager.CompressOtherFiles());
}

/*
 * @tc.name: CompressAppMode_6100
 * @tc.desc: CompressAppMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressAppMode_6100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    MockModuleJsonUtils::MockCheckHapsIsValid(true);
    MockModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_TRUE(appPackager.CompressAppMode());
}

/*
 * @tc.name: CompressAppMode_6200
 * @tc.desc: CompressAppMode, PrepareDirectoriesAndFiles returns false.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CompressAppMode_6200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);

    MockModuleJsonUtils::MockCheckHapsIsValid(false);
    EXPECT_FALSE(appPackager.CompressAppMode());
}

/*
 * @tc.name: GetAndCheckOutPath_6300
 * @tc.desc: GetAndCheckOutPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, GetAndCheckOutPath_6300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    std::string outPath;
    EXPECT_FALSE(appPackager.GetAndCheckOutPath(outPath));
}

/*
 * @tc.name: GetAndCheckHapPathAndHspPath_6400
 * @tc.desc: GetAndCheckHapPathAndHspPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, GetAndCheckHapPathAndHspPath_6400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    std::string hapPath;
    std::string hspPath;
    EXPECT_FALSE(appPackager.GetAndCheckHapPathAndHspPath(hapPath, hspPath));
}

/*
 * @tc.name: GetAndCheckPackInfoPath_6500
 * @tc.desc: GetAndCheckPackInfoPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, GetAndCheckPackInfoPath_6500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    std::string packInfoPath;
    EXPECT_FALSE(appPackager.GetAndCheckPackInfoPath(packInfoPath));
}

/*
 * @tc.name: CheckSignaturePath_6600
 * @tc.desc: CheckSignaturePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckSignaturePath_6600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(appPackager.CheckSignaturePath());
}

/*
 * @tc.name: CheckCertificatePath_6700
 * @tc.desc: CheckCertificatePath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckCertificatePath_6700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(appPackager.CheckCertificatePath());
}

/*
 * @tc.name: CheckPackResPath_6800
 * @tc.desc: CheckPackResPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, CheckPackResPath_6800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(appPackager.CheckPackResPath());
}

/*
 * @tc.name: GetAndCheckReplacePackInfo_0100
 * @tc.desc: GetAndCheckReplacePackInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, GetAndCheckReplacePackInfo_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::AppPackager appPackager(parameterMap, resultReceiver);
    EXPECT_TRUE(appPackager.GetAndCheckReplacePackInfo());
    EXPECT_TRUE(appPackager.isReplacePackInfo_);

    std::map<std::string, std::string> parameterMap2 = {
        {OHOS::AppPackingTool::Constants::PARAM_REPLACE_PACK_INFO, "true"},
    };
    OHOS::AppPackingTool::AppPackager appPackager2(parameterMap2, resultReceiver);
    EXPECT_TRUE(appPackager2.GetAndCheckReplacePackInfo());
    EXPECT_TRUE(appPackager2.isReplacePackInfo_);

    std::map<std::string, std::string> parameterMap3 = {
        {OHOS::AppPackingTool::Constants::PARAM_REPLACE_PACK_INFO, "false"},
    };
    OHOS::AppPackingTool::AppPackager appPackager3(parameterMap3, resultReceiver);
    EXPECT_TRUE(appPackager3.GetAndCheckReplacePackInfo());
    EXPECT_FALSE(appPackager3.isReplacePackInfo_);

    std::map<std::string, std::string> parameterMap4 = {
        {OHOS::AppPackingTool::Constants::PARAM_REPLACE_PACK_INFO, "test"},
    };
    OHOS::AppPackingTool::AppPackager appPackager4(parameterMap4, resultReceiver);
    EXPECT_FALSE(appPackager4.GetAndCheckReplacePackInfo());
    EXPECT_TRUE(appPackager4.isReplacePackInfo_);
}

/*
 * @tc.name: ScanSoFiles_0100
 * @tc.desc: ScanSoFiles.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, ScanSoFiles_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::AppPackager packager(parameterMap, resultReceiver);
    packager.ScanSoFiles();
    parameterMap.insert(std::make_pair(OHOS::AppPackingTool::Constants::PARAM_STAT_DUPLICATE, ""));
    packager.ScanSoFiles();
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_STAT_DUPLICATE] = "false";
    packager.ScanSoFiles();
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_STAT_DUPLICATE] = "true";
    packager.ScanSoFiles();
    parameterMap.insert(std::make_pair(OHOS::AppPackingTool::Constants::PARAM_MODE, ""));
    packager.ScanSoFiles();
    parameterMap.insert(std::make_pair(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""));
    packager.ScanSoFiles();
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_MODE] = OHOS::AppPackingTool::Constants::MODE_APP;
    packager.ScanSoFiles();
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_MODE] = OHOS::AppPackingTool::Constants::MODE_MULTIAPP;
    packager.ScanSoFiles();
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_MODE] = OHOS::AppPackingTool::Constants::MODE_HAP;
    packager.ScanSoFiles();
    std::map<std::string, std::string> parameterMap1 = {
        {OHOS::AppPackingTool::Constants::PARAM_MODE, OHOS::AppPackingTool::Constants::MODE_APP},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_STAT_DUPLICATE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, HSP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, SIGNATURE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, CERTIFICATE_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, PACK_RES_PATH},
    };

    OHOS::AppPackingTool::AppPackager appPackager(parameterMap1, resultReceiver);
    MockModuleJsonUtils::MockCheckAppAtomicServiceCompressedSizeValid(true);
    MockModuleJsonUtils::MockGetHapVerifyInfosMapfromFileList(true);
    EXPECT_EQ(appPackager.InitAllowedParam(), ERR_OK);
    EXPECT_EQ(appPackager.PreProcess(), ERR_OK);
    EXPECT_EQ(appPackager.Process(), ERR_OK);
    EXPECT_EQ(appPackager.PostProcess(), ERR_OK);
    EXPECT_TRUE(fs::exists(OUT_PATH));
    EXPECT_FALSE(fs::exists(fs::path(REPORT_PATH)));
    appPackager.ScanSoFiles();
    ScanStatDuplicate scanStatDuplicate;
    std::vector<std::string> fileList;
    fileList = scanStatDuplicate.GetAllInputFileList("", OUT_PATH);
    EXPECT_EQ(fileList.size(), 0);
    EXPECT_TRUE(fs::exists(fs::path(REPORT_PATH)));
    fs::remove_all(REPORT_PATH);
}

/*
 * @tc.name: WriteFile_0100
 * @tc.desc: WriteFile.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, WriteFile_0100, Function | MediumTest | Level1)
{
    std::string filepath(PATH_MAX + 1, 'c');
    std::string data = "";
    ScanStatDuplicate scanStatDuplicate;
    EXPECT_EQ(scanStatDuplicate.WriteFile(filepath, data), false);
    EXPECT_EQ(scanStatDuplicate.WriteFile(OUT_PATH, data), false);
}

/*
 * @tc.name: SplitPath_0100
 * @tc.desc: SplitPath.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(AppPackagerTest, SplitPath_0100, Function | MediumTest | Level1)
{
    ScanStatDuplicate scanStatDuplicate;
    std::string filePath = UNPACK_NAME + Constants::LINUX_FILE_SEPARATOR + SCAN_RESULT;
    std::string retPath = Constants::LINUX_FILE_SEPARATOR + SCAN_RESULT;
    EXPECT_EQ(scanStatDuplicate.SplitPath(filePath, UNPACK_NAME), retPath);

    std::string pathName = "pathName";
    EXPECT_EQ(scanStatDuplicate.SplitPath(filePath, pathName), filePath);
}
} // namespace OHOS