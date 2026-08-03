/*
 * Copyright (c) 2024-2026 Huawei Device Co., Ltd.
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
#include <filesystem>
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "cJSON.h"
#include "dedup/so_deduplicator.h"
#include "zip_utils.h"
#include "zip_wrapper.h"

using testing::ext::TestSize;

namespace {

constexpr const char* TEST_ROOT = "so_deduplicator_security_test";
constexpr const char* MALICIOUS_MODULE_NAME = "../../target";
constexpr const char* SO_PATH = "libs/arm64-v8a/libsame.so";

std::string EscapeJsonString(const std::string& value)
{
    std::string escaped;
    for (char character : value) {
        if (character == '\\' || character == '"') {
            escaped.push_back('\\');
        }
        escaped.push_back(character);
    }
    return escaped;
}

std::string BuildModuleJson(
    const std::string& moduleName,
    const std::string& deviceType = "phone",
    const std::string& moduleType = "entry",
    bool deliveryWithInstall = true)
{
    return "{"
        "\"app\":{\"bundleType\":\"app\"},"
        "\"module\":{"
            "\"name\":\"" + EscapeJsonString(moduleName) + "\","
            "\"type\":\"" + moduleType + "\","
            "\"deviceTypes\":[\"" + deviceType + "\"],"
            "\"deliveryWithInstall\":" + (deliveryWithInstall ? "true" : "false") + ","
            "\"extractNativeLibs\":true"
        "}"
    "}";
}

void RemoveTestRoot()
{
    std::filesystem::remove_all(TEST_ROOT);
}

bool CreateModuleZip(
    const std::filesystem::path& zipPath,
    const std::string& moduleJson,
    bool includeSo = false)
{
    OHOS::AppPackingTool::ZipWrapper zipWrapper(zipPath.string());
    if (zipWrapper.Open() != OHOS::AppPackingTool::ZIP_ERR_SUCCESS) {
        return false;
    }
    bool success = zipWrapper.WriteStringToZip(moduleJson, "module.json") ==
        OHOS::AppPackingTool::ZIP_ERR_SUCCESS;
    if (includeSo) {
        success = success && zipWrapper.WriteStringToZip("same so content", SO_PATH) ==
            OHOS::AppPackingTool::ZIP_ERR_SUCCESS;
    }
    zipWrapper.Close();
    return success;
}

}  // namespace

class SODeduplicatorTest : public testing::Test {
protected:
    void SetUp() override
    {
        RemoveTestRoot();
        std::filesystem::create_directories(TEST_ROOT);
    }

    void TearDown() override
    {
        RemoveTestRoot();
    }
};

HWTEST_F(SODeduplicatorTest, DeduplicateModules_ModuleNameDoesNotControlExtractPath, TestSize.Level0)
{
    std::filesystem::path root(TEST_ROOT);
    std::filesystem::path workDir = root / "work";
    std::filesystem::path reportDir = root / "report";
    std::filesystem::path hapPath = root / "malicious.hap";
    std::filesystem::path escapedTarget = workDir / "target";
    std::filesystem::create_directories(workDir);
    std::filesystem::create_directories(reportDir);

    ASSERT_TRUE(CreateModuleZip(hapPath, BuildModuleJson(MALICIOUS_MODULE_NAME), true));

    std::list<std::string> modulePaths = {hapPath.string()};
    const std::list<std::string> originalModulePaths = modulePaths;
    OHOS::AppPackingTool::SODeduplicator deduplicator;
    EXPECT_TRUE(deduplicator.DeduplicateModules(modulePaths, originalModulePaths, true,
        workDir.string(), reportDir.string()));
    EXPECT_FALSE(std::filesystem::exists(escapedTarget));

    size_t dedupRootCount = 0;
    for (const auto& entry : std::filesystem::directory_iterator(workDir)) {
        ASSERT_TRUE(entry.is_directory());
        EXPECT_EQ(entry.path().filename().string().find("so_dedup_"), 0);
        std::filesystem::path modulesRoot = entry.path() / "modules";
        ASSERT_TRUE(std::filesystem::is_directory(modulesRoot));
        std::vector<std::string> instanceDirectories;
        for (const auto& moduleEntry : std::filesystem::directory_iterator(modulesRoot)) {
            ASSERT_TRUE(moduleEntry.is_directory());
            instanceDirectories.push_back(moduleEntry.path().filename().string());
        }
        EXPECT_EQ(instanceDirectories, std::vector<std::string>({"0"}));
        ++dedupRootCount;
    }
    EXPECT_EQ(dedupRootCount, 1);
}

HWTEST_F(SODeduplicatorTest, DeduplicateModules_RejectsMismatchedOriginalPathsWhenDisabled, TestSize.Level0)
{
    std::list<std::string> modulePaths = {"module.hap"};
    const std::list<std::string> originalModulePaths;
    OHOS::AppPackingTool::SODeduplicator deduplicator;
    EXPECT_FALSE(deduplicator.DeduplicateModules(
        modulePaths, originalModulePaths, false, TEST_ROOT, TEST_ROOT));
    EXPECT_EQ(deduplicator.GetErrorMessage(), "Module path count does not match original module path count");
}

HWTEST_F(SODeduplicatorTest, DeduplicateModules_AllModuleDirectoriesUseInstanceIds, TestSize.Level0)
{
    const std::vector<std::string> moduleNames = {
        "../../target", "a/b", "a\\b", "1entry", ".entry", "_entry", "entry-name", "entry name", ""
    };
    std::filesystem::path root(TEST_ROOT);
    std::filesystem::path workDir = root / "work";
    std::filesystem::path reportDir = root / "report";
    std::filesystem::create_directories(workDir);
    std::filesystem::create_directories(reportDir);
    std::list<std::string> modulePaths;
    size_t index = 0;
    for (const auto& moduleName : moduleNames) {
        std::filesystem::path hapPath = root / ("module_" + std::to_string(index) + ".hap");
        ASSERT_TRUE(CreateModuleZip(hapPath, BuildModuleJson(moduleName)));
        modulePaths.push_back(hapPath.string());
        ++index;
    }

    OHOS::AppPackingTool::SODeduplicator deduplicator;
    ASSERT_TRUE(deduplicator.DeduplicateModules(modulePaths, true, workDir.string(), reportDir.string()));

    std::filesystem::path dedupRoot;
    for (const auto& entry : std::filesystem::directory_iterator(workDir)) {
        if (entry.is_directory() && entry.path().filename().string().find("so_dedup_") == 0) {
            ASSERT_TRUE(dedupRoot.empty());
            dedupRoot = entry.path();
        }
    }
    ASSERT_FALSE(dedupRoot.empty());
    std::set<std::string> actualDirectories;
    for (const auto& moduleEntry : std::filesystem::directory_iterator(dedupRoot / "modules")) {
        ASSERT_TRUE(moduleEntry.is_directory());
        actualDirectories.insert(moduleEntry.path().filename().string());
    }
    std::set<std::string> expectedDirectories;
    for (size_t moduleIndex = 0; moduleIndex < moduleNames.size(); ++moduleIndex) {
        expectedDirectories.insert(std::to_string(moduleIndex));
    }
    EXPECT_EQ(actualDirectories, expectedDirectories);
    EXPECT_FALSE(std::filesystem::exists(workDir / "target"));
}

HWTEST_F(SODeduplicatorTest, DeduplicateModules_AllowsDuplicateModuleNames, TestSize.Level0)
{
    std::filesystem::path root(TEST_ROOT);
    std::filesystem::path workDir = root / "work";
    std::filesystem::path reportDir = root / "report";
    std::filesystem::path phoneHap = root / "phone.hap";
    std::filesystem::path tabletHap = root / "tablet.hap";
    std::filesystem::create_directories(workDir);
    std::filesystem::create_directories(reportDir);

    ASSERT_TRUE(CreateModuleZip(phoneHap, BuildModuleJson("entry", "phone"), true));
    ASSERT_TRUE(CreateModuleZip(tabletHap, BuildModuleJson("entry", "tablet"), true));

    std::list<std::string> modulePaths = {phoneHap.string(), tabletHap.string()};
    std::list<std::string> originalModulePaths = {"original/phone.hap", "original/tablet.hap"};
    OHOS::AppPackingTool::SODeduplicator deduplicator;
    ASSERT_TRUE(deduplicator.DeduplicateModules(
        modulePaths, originalModulePaths, true, workDir.string(), reportDir.string()));
    ASSERT_EQ(modulePaths.size(), 2);
    for (const auto& modulePath : modulePaths) {
        EXPECT_TRUE(OHOS::AppPackingTool::ZipUtils::IsFileExistsInZip(modulePath, SO_PATH));
    }

    std::ifstream reportFile(reportDir / "so_dedup_report.json");
    std::string report((std::istreambuf_iterator<char>(reportFile)), std::istreambuf_iterator<char>());
    cJSON* parsed = cJSON_Parse(report.c_str());
    ASSERT_NE(parsed, nullptr);
    cJSON* modules = cJSON_GetObjectItemCaseSensitive(parsed, "modules");
    ASSERT_NE(modules, nullptr);
    cJSON* phoneModule = cJSON_GetObjectItemCaseSensitive(modules, "0");
    cJSON* tabletModule = cJSON_GetObjectItemCaseSensitive(modules, "1");
    ASSERT_NE(phoneModule, nullptr);
    ASSERT_NE(tabletModule, nullptr);
    EXPECT_STREQ(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(phoneModule, "moduleName")), "entry");
    EXPECT_STREQ(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(phoneModule, "filePath")),
        "original/phone.hap");
    EXPECT_STREQ(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(tabletModule, "moduleName")), "entry");
    EXPECT_STREQ(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(tabletModule, "filePath")),
        "original/tablet.hap");
    cJSON_Delete(parsed);
}

HWTEST_F(SODeduplicatorTest, DeduplicateModules_RemovesSoFromOnlyOneDuplicateNameModule, TestSize.Level0)
{
    std::filesystem::path root(TEST_ROOT);
    std::filesystem::path workDir = root / "work";
    std::filesystem::path reportDir = root / "report";
    std::filesystem::path phoneEntry = root / "phone_entry.hap";
    std::filesystem::path tabletEntry = root / "tablet_entry.hap";
    std::filesystem::path phoneFeature = root / "phone_feature.hap";
    std::filesystem::path tabletFeature = root / "tablet_feature.hap";
    std::filesystem::create_directories(workDir);
    std::filesystem::create_directories(reportDir);

    ASSERT_TRUE(CreateModuleZip(phoneEntry, BuildModuleJson("phoneEntry", "phone"), true));
    ASSERT_TRUE(CreateModuleZip(tabletEntry, BuildModuleJson("tabletEntry", "tablet")));
    ASSERT_TRUE(CreateModuleZip(phoneFeature, BuildModuleJson("shared", "phone", "feature", false), true));
    ASSERT_TRUE(CreateModuleZip(tabletFeature, BuildModuleJson("shared", "tablet", "feature", false), true));

    std::list<std::string> modulePaths = {
        phoneEntry.string(), tabletEntry.string(), phoneFeature.string(), tabletFeature.string()
    };
    OHOS::AppPackingTool::SODeduplicator deduplicator;
    ASSERT_TRUE(deduplicator.DeduplicateModules(modulePaths, true, workDir.string(), reportDir.string()));
    ASSERT_EQ(modulePaths.size(), 4);

    auto moduleIt = modulePaths.begin();
    EXPECT_TRUE(OHOS::AppPackingTool::ZipUtils::IsFileExistsInZip(*moduleIt++, SO_PATH));
    EXPECT_FALSE(OHOS::AppPackingTool::ZipUtils::IsFileExistsInZip(*moduleIt++, SO_PATH));
    EXPECT_FALSE(OHOS::AppPackingTool::ZipUtils::IsFileExistsInZip(*moduleIt++, SO_PATH));
    EXPECT_TRUE(OHOS::AppPackingTool::ZipUtils::IsFileExistsInZip(*moduleIt, SO_PATH));
}
