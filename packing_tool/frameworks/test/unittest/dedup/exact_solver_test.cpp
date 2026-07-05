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
#include "dedup/exact_solver.h"
#include "dedup/dedup_error.h"
#include "dedup/device_calculator.h"

using testing::ext::TestSize;

class ExactSolverTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::ExactSolver solver_;
};

HWTEST_F(ExactSolverTest, DedupErrorCode_FormatAligned, TestSize.Level0)
{
    std::string message = OHOS::AppPackingTool::FormatDedupError("smoke cause");
    EXPECT_NE(message.find("10021001 SO deduplication failed."), std::string::npos);
    EXPECT_NE(message.find("Error Message: smoke cause"), std::string::npos);
}

void ExactSolverTest::SetUp() {}
void ExactSolverTest::TearDown() {}

// 测试1：判断是否可以使用精确算法
HWTEST_F(ExactSolverTest, CanUseExactAlgorithm_ValidThreshold, TestSize.Level0) {
    EXPECT_TRUE(OHOS::AppPackingTool::ExactSolver::CanUseExactAlgorithm(20));
    EXPECT_TRUE(OHOS::AppPackingTool::ExactSolver::CanUseExactAlgorithm(15));
    EXPECT_TRUE(OHOS::AppPackingTool::ExactSolver::CanUseExactAlgorithm(1));
    EXPECT_FALSE(OHOS::AppPackingTool::ExactSolver::CanUseExactAlgorithm(21));
    EXPECT_FALSE(OHOS::AppPackingTool::ExactSolver::CanUseExactAlgorithm(100));
}

// 测试2：空重复SO组
HWTEST_F(ExactSolverTest, Solve_EmptyDuplicateGroups, TestSize.Level0) {
    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> emptyGroups;
    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> emptyMandatoryMap;

    auto plan = solver_.Solve(emptyGroups, emptyMandatoryMap);

    EXPECT_EQ(plan.keptSoMap.size(), 0);
    EXPECT_EQ(plan.removedSoMap.size(), 0);
    EXPECT_EQ(plan.totalSavedSize, 0);
}

// 测试3：单个重复SO组 - 单个SO
HWTEST_F(ExactSolverTest, Solve_SingleSoInGroup, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.md5 = "abc123";
    group.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group};

    // 创建phone设备的必然安装模块
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1"};

    auto plan = solver_.Solve(groups, mandatoryMap);

    // 单个SO必须保留
    EXPECT_EQ(plan.keptSoMap.size(), 1);
    EXPECT_EQ(plan.keptSoMap["module1"].size(), 1);
    EXPECT_EQ(plan.removedSoMap.size(), 0);
}

// 测试4：单个重复SO组 - 多个SO
HWTEST_F(ExactSolverTest, Solve_MultipleSoInGroup, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.md5 = "abc123";
    group.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module2", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module3", 1000});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group};

    // 创建phone设备的必然安装模块（只有module1和module2）
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1", "module2"};

    auto plan = solver_.Solve(groups, mandatoryMap);

    // module3不在A(phone)中，当前C++接口无法判断其支持设备，保守保留；
    // module1/module2中只需再保留一个。
    EXPECT_EQ(plan.keptSoMap.size(), 2);
    EXPECT_EQ(plan.removedSoMap.size(), 1);
    EXPECT_TRUE(plan.keptSoMap.find("module3") != plan.keptSoMap.end());
}

// 测试5：多设备场景
HWTEST_F(ExactSolverTest, Solve_MultipleDevices, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.md5 = "abc123";
    group.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module2", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module3", 1000});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group};

    // 创建phone和tablet设备
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    OHOS::AppPackingTool::DeviceInstance tabletDevice;
    tabletDevice.type = OHOS::AppPackingTool::DeviceType::TABLET;
    tabletDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1", "module2"};    // phone需要module1和module2
    mandatoryMap[tabletDevice] = {"module2", "module3"};  // tablet需要module2和module3

    auto plan = solver_.Solve(groups, mandatoryMap);

    // 由于phone需要module1或module2，tablet需要module2或module3
    // 最优解是保留module2（满足两个设备），移除module1和module3
    EXPECT_EQ(plan.keptSoMap.size(), 1);
    EXPECT_TRUE(plan.keptSoMap.find("module2") != plan.keptSoMap.end());

    EXPECT_EQ(plan.removedSoMap.size(), 2);
    EXPECT_TRUE(plan.removedSoMap.find("module1") != plan.removedSoMap.end());
    EXPECT_TRUE(plan.removedSoMap.find("module3") != plan.removedSoMap.end());
}

// 测试6：多个重复SO组
HWTEST_F(ExactSolverTest, Solve_MultipleDuplicateGroups, TestSize.Level0) {
    // 创建两个重复SO组
    OHOS::AppPackingTool::DuplicateSoGroup group1;
    group1.md5 = "abc123";
    group1.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});
    group1.soList.push_back({"libs/liba.so", "abc123", "module2", 1000});

    OHOS::AppPackingTool::DuplicateSoGroup group2;
    group2.md5 = "def456";
    group2.soList.push_back({"libs/libb.so", "def456", "module2", 2000});
    group2.soList.push_back({"libs/libb.so", "def456", "module3", 2000});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group1, group2};

    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1", "module2", "module3"};

    auto plan = solver_.Solve(groups, mandatoryMap);

    // 对于group1，应该保留module1或module2中的一个
    // 对于group2，应该保留module2或module3中的一个
    EXPECT_TRUE(plan.keptSoMap.size() >= 1);
    EXPECT_TRUE(plan.removedSoMap.size() >= 1);
}

// 测试7：同MD5但路径不同的SO属于不同重复组，上游不应将其放入同一组
HWTEST_F(ExactSolverTest, Solve_PathIsPartOfDuplicateIdentity, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup libaGroup;
    libaGroup.relativePath = "libs/arm64-v8a/liba.so";
    libaGroup.md5 = "same-md5";
    libaGroup.soList.push_back({"libs/arm64-v8a/liba.so", "same-md5", "module1", 1000});
    libaGroup.soList.push_back({"libs/arm64-v8a/liba.so", "same-md5", "module2", 1000});

    OHOS::AppPackingTool::DuplicateSoGroup libbGroup;
    libbGroup.relativePath = "libs/arm64-v8a/libb.so";
    libbGroup.md5 = "same-md5";
    libbGroup.soList.push_back({"libs/arm64-v8a/libb.so", "same-md5", "module1", 1000});
    libbGroup.soList.push_back({"libs/arm64-v8a/libb.so", "same-md5", "module2", 1000});

    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1", "module2"};

    auto plan = solver_.Solve({libaGroup, libbGroup}, mandatoryMap);

    EXPECT_EQ(plan.keptSoMap.size(), 1);
    EXPECT_EQ(plan.removedSoMap.size(), 1);
    EXPECT_EQ(plan.totalSavedSize, 2000);
}

HWTEST_F(ExactSolverTest, Solve_ModuleOutsideMandatorySetUsesSupportedDevices, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.relativePath = "libs/arm64-v8a/liba.so";
    group.md5 = "same-md5";
    group.soList.push_back({group.relativePath, group.md5, "entry", 1000});
    group.soList.push_back({group.relativePath, group.md5, "ondemand", 1000});

    OHOS::AppPackingTool::DeviceInstance phone {OHOS::AppPackingTool::DeviceType::PHONE, ""};
    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phone] = {"entry"};
    std::map<std::string, std::vector<OHOS::AppPackingTool::DeviceInstance>> supportMap;
    supportMap["entry"] = {phone};
    supportMap["ondemand"] = {phone};

    auto plan = solver_.Solve({group}, mandatoryMap, supportMap);

    EXPECT_EQ(plan.keptSoMap.size(), 1);
    EXPECT_TRUE(plan.keptSoMap.find("entry") != plan.keptSoMap.end());
    EXPECT_TRUE(plan.removedSoMap.find("ondemand") != plan.removedSoMap.end());
}

HWTEST_F(ExactSolverTest, DedupPlan_MergeSameModulePaths, TestSize.Level0)
{
    OHOS::AppPackingTool::DedupPlan smallPlan;
    smallPlan.AddKeptSo("feature1", "libs/liba.so");
    smallPlan.AddRemovedSo("feature1", "libs/liba.so", 100);

    OHOS::AppPackingTool::DedupPlan largePlan;
    largePlan.AddKeptSo("feature1", "libs/libb.so");
    largePlan.AddRemovedSo("feature1", "libs/libb.so", 200);

    smallPlan.Merge(largePlan);

    ASSERT_EQ(smallPlan.keptSoMap["feature1"].size(), 2);
    EXPECT_EQ(smallPlan.keptSoMap["feature1"][0], "libs/liba.so");
    EXPECT_EQ(smallPlan.keptSoMap["feature1"][1], "libs/libb.so");
    ASSERT_EQ(smallPlan.removedSoMap["feature1"].size(), 2);
    EXPECT_EQ(smallPlan.removedSoMap["feature1"][0], "libs/liba.so");
    EXPECT_EQ(smallPlan.removedSoMap["feature1"][1], "libs/libb.so");
    EXPECT_EQ(smallPlan.totalSavedSize, 300);
}

HWTEST_F(ExactSolverTest, ResolveDedupStrategy_AllCombinations, TestSize.Level0)
{
    using OHOS::AppPackingTool::DedupStrategy;
    using OHOS::AppPackingTool::ResolveDedupStrategy;

    EXPECT_EQ(ResolveDedupStrategy(false, false), DedupStrategy::NONE);
    EXPECT_EQ(ResolveDedupStrategy(true, false), DedupStrategy::EXACT);
    EXPECT_EQ(ResolveDedupStrategy(false, true), DedupStrategy::GREEDY);
    EXPECT_EQ(ResolveDedupStrategy(true, true), DedupStrategy::MIXED);
}

HWTEST_F(ExactSolverTest, Solve_TwentyCopiesKeepsSingleOptimalCopy, TestSize.Level0)
{
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.relativePath = "libs/arm64-v8a/libshared.so";
    group.md5 = "same-md5";

    OHOS::AppPackingTool::DeviceInstance phone {OHOS::AppPackingTool::DeviceType::PHONE, ""};
    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    for (size_t index = 0; index < 20; ++index) {
        std::string moduleName = "module" + std::to_string(index);
        group.soList.push_back({group.relativePath, group.md5, moduleName, 1000});
        mandatoryMap[phone].push_back(moduleName);
    }

    auto plan = solver_.Solve({group}, mandatoryMap);

    EXPECT_EQ(plan.keptSoMap.size(), 1);
    EXPECT_EQ(plan.removedSoMap.size(), 19);
    EXPECT_EQ(plan.totalSavedSize, 19000);
}
