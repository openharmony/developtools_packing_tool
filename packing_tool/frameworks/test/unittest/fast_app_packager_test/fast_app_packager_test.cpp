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
const std::string PARAM_OUT_PATH = "out-path";
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

/*
 * @tc.name: fastAppPackager_0200
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.IsVerifyValid();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_0300
 * @tc.desc: IsHapPathValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> formatPathList;
    formatPathList.push_back(resultReceiver);
    auto res = fastAppackager.IsHapPathValid(formatPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_0400
 * @tc.desc: IsHspPathValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> formatPathList;
    formatPathList.push_back(resultReceiver);
    auto res = fastAppackager.IsHspPathValid(formatPathList, resultReceiver);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_0500
 * @tc.desc: ModuleJsonAndPackInfoExists
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hapPathList.push_back(resultReceiver);
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_0600
 * @tc.desc: ModuleJsonAndPackInfoExists
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hspPathList.push_back("/data");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_0700
 * @tc.desc: ModuleJsonAndPackInfoExists
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hapPathList.push_back("/data");
    system("touch /data/module.json");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/module.json");
}

/*
 * @tc.name: fastAppPackager_0800
 * @tc.desc: ModuleJsonAndPackInfoExists
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hspPathList.push_back("/data");
    system("touch /data/module.json");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/module.json");
}

/*
 * @tc.name: fastAppPackager_0900
 * @tc.desc: CheckBundleTypeConsistency
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_0900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hspPathList.push_back(resultReceiver);
    auto res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_1000
 * @tc.desc: GetBundleTypeFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/packingToolDemo-default-unsigned.app");
    auto res = fastAppackager.GetBundleTypeFromPath(OUT_PATH);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/packingToolDemo-default-unsigned.app");
}

/*
 * @tc.name: fastAppPackager_1100
 * @tc.desc: GetModuleJsonContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("module.json");
    system("touch /data/module.json");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/module.json");
}

/*
 * @tc.name: fastAppPackager_1200
 * @tc.desc: GetModuleJsonContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.hsp");
    system("touch /data/test.hsp");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test.hsp");
}

/*
 * @tc.name: fastAppPackager_1300
 * @tc.desc: GetModuleJsonContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.json");
    system("touch /data/test.json");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test.json");
}

/*
 * @tc.name: fastAppPackager_1400
 * @tc.desc: IsPackInfoValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.json");
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    auto res = fastAppackager.IsPackInfoValid(path, hapPathList, hspPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_1500
 * @tc.desc: IsPackInfoValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.hsp");
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    system("touch /data/test.hsp");
    auto res = fastAppackager.IsPackInfoValid(path, hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test.hsp");
}

/*
 * @tc.name: fastAppPackager_1600
 * @tc.desc: IsPackInfoPathListValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> pathList;
    std::set<std::string> packages;
    std::set<std::string> allPackageSet;
    pathList.push_back(resultReceiver);
    auto res = fastAppackager.IsPackInfoPathListValid(pathList, packages, allPackageSet);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_1700
 * @tc.desc: IsPackInfoPathListValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> pathList;
    std::set<std::string> packages;
    std::set<std::string> allPackageSet;
    pathList.push_back("/data/test.hsp");
    system("touch /data/test.hsp");
    auto res = fastAppackager.IsPackInfoPathListValid(pathList, packages, allPackageSet);
    EXPECT_FALSE(res);
    system("rm -f /data/test.hsp");
}

/*
 * @tc.name: fastAppPackager_1800
 * @tc.desc: IsPackInfoPathListValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> pathList;
    std::set<std::string> packages;
    std::set<std::string> allPackageSet;
    allPackageSet.emplace("test");
    pathList.push_back("/data/test.hsp");
    packages.emplace("test");
    system("touch /data/test.hsp");
    auto res = fastAppackager.IsPackInfoPathListValid(pathList, packages, allPackageSet);
    EXPECT_FALSE(res);
    system("rm -f /data/test.hsp");
}

/*
 * @tc.name: fastAppPackager_1900
 * @tc.desc: GetPackageNameFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_1900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.json");
    system("touch /data/test.json");
    std::list<std::string> packageNameList;
    auto res = fastAppackager.GetPackageNameFromPath(path, packageNameList);
    EXPECT_FALSE(res);
    system("rm -f /data/test.json");
}

/*
 * @tc.name: fastAppPackager_2000
 * @tc.desc: GetPackInfoContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.hsp");
    system("touch /data/test.hsp");
    auto res = fastAppackager.GetPackInfoContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test.hsp");
}

/*
 * @tc.name: fastAppPackager_2100
 * @tc.desc: GetPackInfoContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.json");
    system("touch /data/test.json");
    auto res = fastAppackager.GetPackInfoContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test.json");
}

/*
 * @tc.name: fastAppPackager_2200
 * @tc.desc: CompressFastAppMode
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(PARAM_OUT_PATH, "/data/test.json");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test.json");
    auto res = fastAppackager.CompressFastAppMode();
    EXPECT_FALSE(res);
    system("rm -f /data/test.json");
}

/*
 * @tc.name: fastAppPackager_2300
 * @tc.desc: CheckHapAndPackFastApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tmpDir("");
    auto res = fastAppackager.CheckHapAndPackFastApp(fileList, tmpDir);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_2400
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fs::path tmpDir("testDir");
    fastAppackager.formattedHapPathList_.push_back("testDir");
    auto res = fastAppackager.Pack(tmpDir, fileList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_2500
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("");
    fs::path appPackInfo("");
    fs::path outPath("");
    fs::path path("");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_2600
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("/data/inputPath.json");
    fs::path appPackInfo("/data/appPackInfo.json");
    fs::path outPath("");
    fs::path path("");
    system("touch /data/inputPath.json");
    system("touch /data/appPackInfo.json");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/inputPath.json");
    system("rm -f /data/appPackInfo.json");
}

/*
 * @tc.name: fastAppPackager_2700
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("/data/inputPath.json");
    fs::path appPackInfo("/data/appPackInfo.json");
    fs::path outPath("");
    fs::path path("");
    system("touch /data/inputPath.json");
    system("touch /data/appPackInfo.json");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/inputPath.json");
    system("rm -f /data/appPackInfo.json");
}

/*
 * @tc.name: fastAppPackager_2800
 * @tc.desc: PackDir
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("");
    fs::path appPackInfo("");
    fs::path outPath("");
    fs::path path("");
    auto res = fastAppackager.PackDir(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_2900
 * @tc.desc: PackFastApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_2900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fastAppackager.zipWrapper_.zipFile_ = nullptr;
    auto res = fastAppackager.PackFastApp(fileList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_3000
 * @tc.desc: AddHapListToApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fileList.push_back("/data/test");
    auto res = fastAppackager.AddHapListToApp(fileList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_3100
 * @tc.desc: AddSignatureAndCertificateToApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(PARAM_CERTIFICATE_PATH, "demo_test");
    parameterMap.emplace(PARAM_SIGNATURE_PATH, "demo_test");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.AddSignatureAndCertificateToApp();
    EXPECT_TRUE(res);
}

/*
 * @tc.name: fastAppPackager_3200
 * @tc.desc: packMultiThread
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("");
    fs::path appPackInfo("");
    fs::path outHap("");
    fs::path path("");
    auto res = fastAppackager.packMultiThread(inputPath, appPackInfo, outHap, path);
    EXPECT_TRUE(res);
}

/*
 * @tc.name: fastAppPackager_3300
 * @tc.desc: packSingleThread
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("");
    fs::path appPackInfo("");
    fs::path outHap("");
    fs::path path("");
    auto res = fastAppackager.packSingleThread(inputPath, appPackInfo, outHap, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_3400
 * @tc.desc: packSingleThread
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("/data");
    fs::path appPackInfo("");
    fs::path outHap("");
    fs::path path("");
    auto res = fastAppackager.packSingleThread(inputPath, appPackInfo, outHap, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_3500
 * @tc.desc: packSingleThread
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("/data");
    fs::path appPackInfo("");
    fs::path outHap(PARAM_OUT_PATH);
    fs::path path("");
    fastAppackager.isGenerateBuildHash_ = true;
    auto res = fastAppackager.packSingleThread(inputPath, appPackInfo, outHap, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_3600
 * @tc.desc: AddOtherFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path entry("/data/module.json");
    system("touch /data/module.json");
    fastAppackager.isGenerateBuildHash_ = true;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_TRUE(res);
    system("rm -f /data/module.json");
}

/*
 * @tc.name: fastAppPackager_3700
 * @tc.desc: AddOtherFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path entry("/data/module.json");
    system("touch /data/module.json");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_FALSE(res);
    system("rm -f /data/module.json");
}

/*
 * @tc.name: fastAppPackager_3800
 * @tc.desc: AddOtherFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path entry("/data/libs");
    system("mkdir /data/libs");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_FALSE(res);
    system("rm -rf /data/libs");
}

/*
 * @tc.name: fastAppPackager_3900
 * @tc.desc: AddOtherFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_3900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path entry("/data/libss");
    system("mkdir /data/libss");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_FALSE(res);
    system("rm -rf /data/libss");
}

/*
 * @tc.name: fastAppPackager_4000
 * @tc.desc: GenBuildHash
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::string jsonString;
    fs::path Path("/data");
    system("mkdir /data/module.json");
    fastAppackager.GenBuildHash(Path, jsonString);
    EXPECT_TRUE(jsonString.empty());
    system("rm -rf /data/module.json");
}

/*
 * @tc.name: fastAppPackager_4100
 * @tc.desc: UzipHspAndRemovePackInfo
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::string hspPath;
    std::string unzipPathString = "/data";
    system("mkdir /data/pack.info");
    fastAppackager.UzipHspAndRemovePackInfo(hspPath, unzipPathString);
    EXPECT_EQ(unzipPathString, "/data");
    system("rm -rf /data/pack.info");
}
} // namespace OHOS