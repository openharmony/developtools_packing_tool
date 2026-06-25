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

#include <gtest/gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "dedup/report_generator.h"

class ReportGeneratorTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::ReportGenerator generator_;
    std::string testOutputDir_;

    void CleanTestOutputDir();
};

void ReportGeneratorTest::SetUp() {
    // 设置测试输出目录
    testOutputDir_ = "test_output_reports";
    CleanTestOutputDir();
}

void ReportGeneratorTest::TearDown() {
    CleanTestOutputDir();
}

void ReportGeneratorTest::CleanTestOutputDir() {
    if (std::filesystem::exists(testOutputDir_)) {
        try {
            std::filesystem::remove_all(testOutputDir_);
        } catch (const std::exception& e) {
            // 忽略清理错误
        }
    }
}

// 测试1：生成空去重方案的报告
HWTEST_F(ReportGeneratorTest, GenerateReport_EmptyPlan, TestSize.Level0) {
    OHOS::AppPackingTool::DedupPlan emptyPlan;

    std::string reportPath = generator_.GenerateReport(
        emptyPlan, OHOS::AppPackingTool::DedupStrategy::EXACT, testOutputDir_);

    EXPECT_FALSE(reportPath.empty());
    EXPECT_TRUE(std::filesystem::exists(reportPath));
}

// 测试2：生成包含去重信息的报告
HWTEST_F(ReportGeneratorTest, GenerateReport_WithDedupInfo, TestSize.Level0) {
    OHOS::AppPackingTool::DedupPlan plan;
    plan.AddKeptSo("module1", "libs/liba.so");
    plan.AddKeptSo("module2", "libs/liba.so");
    plan.AddRemovedSo("module3", "libs/liba.so", 1000);
    plan.AddRemovedSo("module4", "libs/libb.so", 2000);

    std::string reportPath = generator_.GenerateReport(
        plan, OHOS::AppPackingTool::DedupStrategy::GREEDY, testOutputDir_);

    EXPECT_FALSE(reportPath.empty());
    EXPECT_TRUE(std::filesystem::exists(reportPath));

    // 验证文件内容
    std::ifstream inFile(reportPath);
    std::string content((std::istreambuf_iterator<char>(inFile)),
                       std::istreambuf_iterator<char>());

    EXPECT_NE(content.find("\"version\": \"1.0\""), std::string::npos);
    EXPECT_NE(content.find("\"strategy\": \"greedy\""), std::string::npos);
    EXPECT_NE(content.find("\"module1\""), std::string::npos);
    EXPECT_NE(content.find("\"module2\""), std::string::npos);
    EXPECT_NE(content.find("\"module3\""), std::string::npos);
    EXPECT_NE(content.find("\"module4\""), std::string::npos);
    EXPECT_NE(content.find("\"totalSavedSize\": 3000"), std::string::npos);
}

// 测试3：生成JSON字符串
HWTEST_F(ReportGeneratorTest, GenerateReportJson_ValidFormat, TestSize.Level0) {
    OHOS::AppPackingTool::DedupPlan plan;
    plan.AddKeptSo("module1", "libs/liba.so");
    plan.AddRemovedSo("module2", "libs/liba.so", 1000);

    std::string jsonStr = generator_.GenerateReportJson(
        plan, OHOS::AppPackingTool::DedupStrategy::EXACT);

    EXPECT_FALSE(jsonStr.empty());
    EXPECT_NE(jsonStr.find("\"version\": \"1.0\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"strategy\": \"exact\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"timestamp\":"), std::string::npos);
    EXPECT_NE(jsonStr.find("\"modules\":"), std::string::npos);
}

// 测试4：验证JSON格式完整性
HWTEST_F(ReportGeneratorTest, GenerateReportJson_CompleteStructure, TestSize.Level0) {
    OHOS::AppPackingTool::DedupPlan plan;
    plan.AddKeptSo("entry_module", "libs/entry_lib.so");
    plan.AddKeptSo("feature_module", "libs/feature_lib.so");
    plan.AddRemovedSo("unused_module", "libs/unused_lib.so", 5000);

    std::string jsonStr = generator_.GenerateReportJson(
        plan, OHOS::AppPackingTool::DedupStrategy::GREEDY);

    // 验证JSON结构包含所有必需字段
    EXPECT_NE(jsonStr.find("\"version\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"timestamp\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"strategy\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"totalSavedSize\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"modules\""), std::string::npos);

    // 验证模块信息
    EXPECT_NE(jsonStr.find("\"entry_module\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"feature_module\""), std::string::npos);
    EXPECT_NE(jsonStr.find("\"unused_module\""), std::string::npos);

    // 验证kept和removed数组
    EXPECT_NE(jsonStr.find("\"kept\":"), std::string::npos);
    EXPECT_NE(jsonStr.find("\"removed\":"), std::string::npos);
}

// 测试5：输出目录不存在时自动创建
HWTEST_F(ReportGeneratorTest, GenerateReport_CreateOutputDir, TestSize.Level0) {
    std::string nonExistentDir = "test_nonexistent_dir_reports";

    // 确保目录不存在
    if (std::filesystem::exists(nonExistentDir)) {
        std::filesystem::remove_all(nonExistentDir);
    }

    OHOS::AppPackingTool::DedupPlan plan;
    plan.AddKeptSo("module1", "libs/liba.so");

    std::string reportPath = generator_.GenerateReport(
        plan, OHOS::AppPackingTool::DedupStrategy::EXACT, nonExistentDir);

    EXPECT_FALSE(reportPath.empty());
    EXPECT_TRUE(std::filesystem::exists(nonExistentDir));
    EXPECT_TRUE(std::filesystem::exists(reportPath));

    // 清理
    std::filesystem::remove_all(nonExistentDir);
}