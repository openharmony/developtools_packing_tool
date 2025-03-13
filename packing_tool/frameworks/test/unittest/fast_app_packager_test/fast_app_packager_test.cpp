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
#include <fstream>

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
const std::string FAIL_PATH = "/data/test/resource/packingtool/test_file/hap/fail";
const std::string PACK_JSON_PATH = "/data/test/resource/packingtool/test_file/pack.json";
const std::string PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";
const std::string ENTRY_PATH = "/data/test/resource/packingtool/test_file/entry";
const std::string HSP_PATH = "/data/test/resource/packingtool/test_file/fastAppPackagerHspTest.hsp";
const std::string TEST_PATH = "/data/test/pack.info";
const std::string MODULE_JSON_TEST_STRING = "{"
    "\"app\": {"
        "\"apiVersion\": {"
        "}"
    "},"
    "\"module\": {"
        "\"type\": \"entry\","
        "\"name\": \"entry\","
        "\"packages\": \"entry\","
        "\"requestPermissions\": true,"
        "\"proxyData\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"generateBuildHash\": true"
    "}"
"}";
const std::string JSON_STRING = "{"
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

void CopyFile(const std::string& srcFile, const std::string& dstFile)
{
    std::string cmd = "cp -f " + srcFile + " " + dstFile;
    system(cmd.c_str());
}

void TouchFile(const std::string& file)
{
    std::string cmd = "touch " + file;
    system(cmd.c_str());
}

void DeleteFile(const std::string& file)
{
    std::string cmd = "rm -f " + file;
    system(cmd.c_str());
}

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
    system("cp -f /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    system("cp -f /data/test/resource/packingtool/test_file/hap/entry/pack.json "
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
    hspPathList.push_back("/data/test");
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
    hapPathList.push_back("/data/test");
    system("touch /data/test/module.json");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/module.json");
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
    hspPathList.push_back("/data/test");
    system("touch /data/test/module.json");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/module.json");
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
    system("touch /data/test/module.json");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/module.json");
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
    fs::path path("/data/test/test.hsp");
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/test.hsp");
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
    fs::path path("/data/test/test.json");
    system("touch /data/test/test.json");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/test.json");
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
    fs::path path("/data/test/test.json");
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
    fs::path path("/data/test/test.hsp");
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.IsPackInfoValid(path, hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.hsp");
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
    pathList.push_back("/data/test/test.hsp");
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.IsPackInfoPathListValid(pathList, packages, allPackageSet);
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.hsp");
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
    pathList.push_back("/data/test/test.hsp");
    packages.emplace("test");
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.IsPackInfoPathListValid(pathList, packages, allPackageSet);
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.hsp");
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
    fs::path path("/data/test/test.json");
    system("touch /data/test/test.json");
    std::list<std::string> packageNameList;
    auto res = fastAppackager.GetPackageNameFromPath(path, packageNameList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.json");
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
    fs::path path("/data/test/test.hsp");
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.GetPackInfoContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/test.hsp");
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
    fs::path path("/data/test/test.json");
    system("touch /data/test/test.json");
    auto res = fastAppackager.GetPackInfoContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/test.json");
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
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/test.json");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/test.json");
    auto res = fastAppackager.CompressFastAppMode();
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.json");
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
    fs::path inputPath("/data/test/inputPath.json");
    fs::path appPackInfo("/data/test/appPackInfo.json");
    fs::path outPath("");
    fs::path path("");
    system("touch /data/test/inputPath.json");
    system("touch /data/test/appPackInfo.json");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/test/inputPath.json");
    system("rm -f /data/test/appPackInfo.json");
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
    fs::path inputPath("/data/test/inputPath.json");
    fs::path appPackInfo("/data/test/appPackInfo.json");
    fs::path outPath("");
    fs::path path("");
    system("touch /data/test/inputPath.json");
    system("touch /data/test/appPackInfo.json");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/test/inputPath.json");
    system("rm -f /data/test/appPackInfo.json");
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
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, "demo_test");
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, "demo_test");
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
    fs::path inputPath("/data/test");
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
    fs::path inputPath("/data/test/test");
    fs::path appPackInfo("");
    fs::path outHap(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH);
    fs::path path("");
    fastAppackager.isGenerateBuildHash_ = true;
    system("mkdir /data/test/test");
    system("touch /data/test/test/module.json");
    auto res = fastAppackager.packSingleThread(inputPath, appPackInfo, outHap, path);
    EXPECT_TRUE(res);
    system("rm -rf /data/test/test");
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
    fs::path entry("/data/test/module.json");
    system("touch /data/test/module.json");
    fastAppackager.isGenerateBuildHash_ = true;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_TRUE(res);
    system("rm -f /data/test/module.json");
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
    fs::path entry("/data/test/module.json");
    system("touch /data/test/module.json");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_FALSE(res);
    system("rm -f /data/test/module.json");
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
    fs::path entry("/data/test/libs");
    system("mkdir /data/test/libs");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_TRUE(res);
    system("rm -rf /data/test/libs");
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
    fs::path entry("/data/test/libss");
    system("mkdir /data/test/libss");
    fastAppackager.isGenerateBuildHash_ = false;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_TRUE(res);
    system("rm -rf /data/test/libss");
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
    fs::path Path("/data/test");
    system("mkdir /data/test/module.json");
    fastAppackager.GenBuildHash(Path, jsonString);
    EXPECT_TRUE(jsonString.empty());
    system("rm -rf /data/test/module.json");
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
    std::string unzipPathString = "/data/test";
    system("mkdir /data/test/pack.info");
    fastAppackager.UzipHspAndRemovePackInfo(hspPath, unzipPathString);
    EXPECT_EQ(unzipPathString, "/data/test");
    system("rm -rf /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4200
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4300
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, ""},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4400
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, "/data/test/pack.res"},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, "/data/test"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.res");
    system("touch /data/test/pack.info");
    EXPECT_TRUE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
    system("rm -f /data/test/pack.res");
}

/*
 * @tc.name: fastAppPackager_4500
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, "/data/test/pack.res"},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.res");
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
    system("rm -f /data/test/pack.res");
}

/*
 * @tc.name: fastAppPackager_4600
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4700
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4800
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, TEST_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_4900
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_4900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_5000
 * @tc.desc: IsVerifyValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, ""},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    system("touch /data/test/pack.info");
    EXPECT_FALSE(fastAppackager.IsVerifyValid());
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_5100
 * @tc.desc: IsHapPathValid and IsHspPathValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> formatPathList;
    formatPathList.push_back("/data/test");
    auto res = fastAppackager.IsHapPathValid(formatPathList);
    EXPECT_TRUE(res);
    std::list<std::string> pathList;
    pathList.push_back(TEST_PATH);
    system("touch /data/test/pack.info");
    res = fastAppackager.IsHspPathValid(pathList, resultReceiver);
    EXPECT_TRUE(res);
    res = fastAppackager.IsHspPathValid(pathList, TEST_PATH);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_5200
 * @tc.desc: ModuleJsonAndPackInfoExists
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hspPathList;
    std::list<std::string> hapPathList;
    hspPathList.push_back("/data/test");
    hapPathList.push_back("/data/test");
    system("touch /data/test/pack.info");
    system("touch /data/test/module.json");
    auto res = fastAppackager.ModuleJsonAndPackInfoExists(hapPathList, hspPathList);
    EXPECT_TRUE(res);
    system("rm -f /data/test/pack.info");
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: fastAppPackager_5300
 * @tc.desc: CheckBundleTypeConsistency
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hapPathList;
    std::list<std::string> hspPathList;
    hapPathList.push_back(TEST_PATH);
    auto res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    res = fastAppackager.CheckBundleTypeConsistency(hspPathList, hapPathList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_5400
 * @tc.desc: GetBundleTypeFromModuleJson
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.GetBundleTypeFromModuleJson(JSON_STRING);
    EXPECT_TRUE(res.empty());
    res = fastAppackager.GetBundleTypeFromModuleJson(TEST_PATH);
    EXPECT_TRUE(res.empty());
}

/*
 * @tc.name: fastAppPackager_5500
 * @tc.desc: GetPackageNameFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test.json");
    std::list<std::string> packageNameList;
    auto res = fastAppackager.GetPackageNameFromPath(path, packageNameList);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_5600
 * @tc.desc: GetPackageNameFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test/pack.info");
    system("touch /data/test/pack.info");
    system("chmod 777 /data/test/pack.info");
    std::ofstream file("/data/test/pack.info");
    if (file.is_open()) {
        file << TEST_PATH;
        file.close();
    }
    std::list<std::string> packageNameList;
    auto res = fastAppackager.GetPackageNameFromPath(path, packageNameList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_5700
 * @tc.desc: CompressFastAppMode
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/mockdirectory/test/");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.CompressFastAppMode();
    EXPECT_FALSE(res);
    system("rm -rf /mockdirectory");
}

/*
 * @tc.name: fastAppPackager_5800
 * @tc.desc: CompressFastAppMode
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back("das");
    auto res = fastAppackager.CompressFastAppMode();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_5900
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_5900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHspPathList_.emplace_back(TEST_PATH);
    fs::path tmpDir(TEST_PATH);
    std::list<std::string> fileList;
    system("touch /data/test/pack.info");
    auto res = fastAppackager.Pack(tmpDir, fileList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_6000
 * @tc.desc: Pack
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath(TEST_PATH);
    fs::path appPackInfo("");
    fs::path packInf(TEST_PATH);
    fs::path outPath("");
    fs::path path("");
    system("touch /data/test/pack.info");
    auto res = fastAppackager.Pack(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    res = fastAppackager.Pack(inputPath, packInf, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_6100
 * @tc.desc: PackFastApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/pack.info"},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, "/data/test/pack.info"},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, "/data/test/pack.info"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fileList.emplace_back(TEST_PATH);
    fastAppackager.packInfoPath_ = "/data/test/pack.info";
    system("touch /data/test/pack.info");
    fastAppackager.formattedEntryCardPathList_.emplace_back(TEST_PATH);
    auto res = fastAppackager.PackFastApp(fileList);
    EXPECT_TRUE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_6200
 * @tc.desc: AddSignatureAndCertificateToApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, "/data/test");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.AddSignatureAndCertificateToApp();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_6300
 * @tc.desc: AddSignatureAndCertificateToApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, "/data/test");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.AddSignatureAndCertificateToApp();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_6400
 * @tc.desc: packSingleThread
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath(TEST_PATH);
    fs::path appPackInfo("");
    fs::path outHap("");
    fs::path path("");
    system("touch /data/test/pack.info");
    auto res = fastAppackager.packSingleThread(inputPath, appPackInfo, outHap, path);
    EXPECT_FALSE(res);
    system("rm -f  /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_6500
 * @tc.desc: AddOtherFileToZip
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path entry("/data/test/pack.txt");
    system("touch /data/test/pack.txt");
    fastAppackager.isGenerateBuildHash_ = true;
    auto res = fastAppackager.AddOtherFileToZip(entry);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.txt");
}

/*
 * @tc.name: fastAppPackager_6600
 * @tc.desc: GenBuildHash
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::string jsonString;
    fs::path Path("/data/test");
    system("touch /data/test/module.json");
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << MODULE_JSON_TEST_STRING;
        file.close();
    }
    fastAppackager.GenBuildHash(Path, jsonString);
    EXPECT_FALSE(jsonString.empty());
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: fastAppPackager_6700
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back(TEST_PATH);
    auto res = fastAppackager.FormatPath();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_6800
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back(TEST_PATH);
    system("touch /data/test/pack.info");
    auto res = fastAppackager.FormatPath();
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_6900
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_6900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, "/data/test"},
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, "/data/test"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.FormatPath();
    EXPECT_TRUE(res);
}

/*
 * @tc.name: fastAppPackager_7000
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back(TEST_PATH);
    auto res = fastAppackager.FormatPath();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_7100
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_PATH, TEST_PATH},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back(TEST_PATH);
    system("touch /data/test/pack.info");
    auto res = fastAppackager.FormatPath();
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_7200
 * @tc.desc: FormatPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.FormatPath();
    EXPECT_FALSE(res);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_HAP_PATH] = "";
    OHOS::AppPackingTool::FastAppPackager appPackager(parameterMap, resultReceiver);
    res = appPackager.FormatPath();
    EXPECT_FALSE(res);
    OHOS::AppPackingTool::FastAppPackager fastPackager(parameterMap, resultReceiver);
    res = fastPackager.FormatPath();
    EXPECT_FALSE(res);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_HSP_PATH] = "";
    OHOS::AppPackingTool::FastAppPackager fap(parameterMap, resultReceiver);
    res = fap.FormatPath();
    EXPECT_FALSE(res);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_HSP_PATH] = "/data/test";
    OHOS::AppPackingTool::FastAppPackager packager(parameterMap, resultReceiver);
    res = packager.FormatPath();
    EXPECT_TRUE(res);
}

/*
 * @tc.name: fastAppPackager_7300
 * @tc.desc: CheckBundleTypeConsistency
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hapPathList;
    std::list<std::string> hspPathList;
    hapPathList.push_back("/data/test/module.json");
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << MODULE_JSON_TEST_STRING;
        file.close();
    }
    auto res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_TRUE(res);
    hapPathList.push_back("/data/test/pack.info");
    res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: fastAppPackager_7400
 * @tc.desc: CheckBundleTypeConsistency
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> hapPathList;
    std::list<std::string> hspPathList;
    hspPathList.push_back("/data/test/module.json");
    system("touch /data/test/module.json");
    system("chmod 777 /data/test/module.json");
    std::ofstream file("/data/test/module.json");
    if (file.is_open()) {
        file << MODULE_JSON_TEST_STRING;
        file.close();
    }
    auto res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_TRUE(res);
    hspPathList.push_back("/data/test/pack.info");
    res = fastAppackager.CheckBundleTypeConsistency(hapPathList, hspPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: fastAppPackager_7500
 * @tc.desc: GetModuleJsonContentFromPath
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test/module.json");
    system("touch /data/test/module.json");
    auto res = fastAppackager.GetModuleJsonContentFromPath(path);
    EXPECT_TRUE(res.empty());
    system("rm -f /data/test/module.json");
}

/*
 * @tc.name: fastAppPackager_7600
 * @tc.desc: IsPackInfoValid
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path path("/data/test/test.hsp");
    std::list<std::string> hspPathList;
    hspPathList.emplace_back(TEST_PATH);
    std::list<std::string> hapPathList;
    system("touch /data/test/test.hsp");
    auto res = fastAppackager.IsPackInfoValid(path, hapPathList, hspPathList);
    EXPECT_FALSE(res);
    res = fastAppackager.IsPackInfoValid(path, hspPathList, hapPathList);
    EXPECT_FALSE(res);
    system("rm -f /data/test/test.hsp");
}

/*
 * @tc.name: fastAppPackager_7700
 * @tc.desc: CompressFastAppMode
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    parameterMap.emplace(OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "");
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fastAppackager.formattedHapPathList_.emplace_back("das");
    auto res = fastAppackager.CompressFastAppMode();
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_7800
 * @tc.desc: PackDir
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("/data/test");
    fs::path appPackInfo("");
    fs::path outPath("");
    fs::path path("");
    system("touch /data/test/pack.info");
    system("chmod 777 /data/test/pack.info");
    std::ofstream file("/data/test/pack.info");
    if (file.is_open()) {
        file << JSON_STRING;
        file.close();
    }
    auto res = fastAppackager.PackDir(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_7900
 * @tc.desc: RepackHsp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_7900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    fs::path inputPath("");
    fs::path appPackInfo("");
    fs::path outPath("");
    fs::path path("");
    auto res = fastAppackager.RepackHsp(inputPath, appPackInfo, outPath, path);
    EXPECT_FALSE(res);
}

/*
 * @tc.name: fastAppPackager_8000
 * @tc.desc: PackFastApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_8000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, "/data/test/pack.info"},
        {OHOS::AppPackingTool::Constants::PARAM_ENTRYCARD_PATH, ""},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, ""},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    std::list<std::string> fileList;
    fileList.emplace_back(TEST_PATH);
    fastAppackager.packInfoPath_ = "/data/test/pack.info";
    system("touch /data/test/pack.info");
    fastAppackager.formattedEntryCardPathList_.emplace_back(TEST_PATH);
    auto res = fastAppackager.PackFastApp(fileList);
    EXPECT_TRUE(res);
    system("rm -f /data/test/pack.info");
}

/*
 * @tc.name: fastAppPackager_8100
 * @tc.desc: AddSignatureAndCertificateToApp
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, fastAppPackager_8100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap;
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    auto res = fastAppackager.AddSignatureAndCertificateToApp();
    EXPECT_TRUE(res);
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH] = "";
    parameterMap[OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH] = "";
    OHOS::AppPackingTool::FastAppPackager fastPackager(parameterMap, resultReceiver);
    res = fastPackager.AddSignatureAndCertificateToApp();
    EXPECT_TRUE(res);
}

/*
 * @tc.name: PreProcess_0001
 * @tc.desc: ProProcess.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, preProcess_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "fail"},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    EXPECT_EQ(fastAppackager.PreProcess(), 1);
}

/*
 * @tc.name: PreProcess_0002
 * @tc.desc: ProProcess.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, preProcess_0002, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, ""},
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);
    EXPECT_EQ(fastAppackager.PreProcess(), 1);
}

/*
 * @tc.name: process_0001
 * @tc.desc: Process.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, process_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    TouchFile(OUT_PATH);

    EXPECT_EQ(fastAppackager.Process(), 1);

    DeleteFile(OUT_PATH);
}

/*
 * @tc.name: isVerifyValidInFastAppMode_0001
 * @tc.desc: IsVerifyValidInFastAppMode.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isVerifyValidInFastAppMode_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    CopyFile(PACK_JSON_PATH, PACK_INFO_PATH);

    EXPECT_FALSE(fastAppackager.IsVerifyValidInFastAppMode());

    DeleteFile(PACK_INFO_PATH);
    DeleteFile(OUT_PATH);
}

/*
 * @tc.name: isVerifyValidInFastAppMode_0002
 * @tc.desc: IsVerifyValidInFastAppMode.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isVerifyValidInFastAppMode_0002, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HAP_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_HAP_PATH, HSP_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    CopyFile(PACK_JSON_PATH, PACK_INFO_PATH);

    EXPECT_FALSE(fastAppackager.IsVerifyValidInFastAppMode());

    DeleteFile(PACK_INFO_PATH);
    DeleteFile(OUT_PATH);
}

/*
 * @tc.name: isVerifyValid_0001
 * @tc.desc: IsVerifyValid.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isVerifyValid_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_SIGNATURE_PATH, FAIL_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    CopyFile(PACK_JSON_PATH, PACK_INFO_PATH);

    EXPECT_FALSE(fastAppackager.IsVerifyValid());

    DeleteFile(PACK_INFO_PATH);
}

/*
 * @tc.name: isVerifyValid_0002
 * @tc.desc: IsVerifyValid.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isVerifyValid_0002, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_CERTIFICATE_PATH, FAIL_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    CopyFile(PACK_JSON_PATH, PACK_INFO_PATH);

    EXPECT_FALSE(fastAppackager.IsVerifyValid());

    DeleteFile(PACK_INFO_PATH);
}

/*
 * @tc.name: isVerifyValid_0003
 * @tc.desc: IsVerifyValid.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isVerifyValid_0003, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, PACK_INFO_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_RES_PATH, FAIL_PATH}
    };
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    CopyFile(PACK_JSON_PATH, PACK_INFO_PATH);

    EXPECT_FALSE(fastAppackager.IsVerifyValid());

    DeleteFile(PACK_INFO_PATH);
}

/*
 * @tc.name: isHspPathValid_0001
 * @tc.desc: IsHspPathValid.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, isHspPathValid_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    std::list<std::string> formatPathList;
    formatPathList.emplace_back(HSP_PATH);
    EXPECT_FALSE(fastAppackager.IsHspPathValid(formatPathList, OHOS::AppPackingTool::Constants::HAP_SUFFIX));
}

/*
 * @tc.name: getBundleTypeFromModuleJson_0001
 * @tc.desc: GetBundleTypeFromModuleJson.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, getBundleTypeFromModuleJson_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    std::string content = "";
    EXPECT_EQ(fastAppackager.GetBundleTypeFromModuleJson(content), "");
}

/*
 * @tc.name: addOtherFileToZip_0001
 * @tc.desc: AddOtherFileToZip.
 * @tc.type: FUNC
 */
HWTEST_F(FastAppPackagerTest, addOtherFileToZip_0001, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {};
    OHOS::AppPackingTool::FastAppPackager fastAppackager(parameterMap, resultReceiver);

    TouchFile(PACK_JSON_PATH);

    EXPECT_FALSE(fastAppackager.AddOtherFileToZip(PACK_JSON_PATH));

    DeleteFile(PACK_JSON_PATH);
}
} // namespace OHOS
