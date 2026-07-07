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
#include "dedup/greedy_solver.h"
#include "dedup/device_calculator.h"

using testing::ext::TestSize;

class GreedySolverTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::GreedySolver solver_;
};

void GreedySolverTest::SetUp() {}
void GreedySolverTest::TearDown() {}

// 测试1：判断是否可以使用贪心算法
HWTEST_F(GreedySolverTest, CanUseGreedyAlgorithm_ValidThreshold, TestSize.Level0) {
    EXPECT_FALSE(OHOS::AppPackingTool::GreedySolver::CanUseGreedyAlgorithm(20));
    EXPECT_FALSE(OHOS::AppPackingTool::GreedySolver::CanUseGreedyAlgorithm(15));
    EXPECT_TRUE(OHOS::AppPackingTool::GreedySolver::CanUseGreedyAlgorithm(21));
    EXPECT_TRUE(OHOS::AppPackingTool::GreedySolver::CanUseGreedyAlgorithm(100));
}

// 测试2：空重复SO组
HWTEST_F(GreedySolverTest, Solve_EmptyDuplicateGroups, TestSize.Level0) {
    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> emptyGroups;
    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> emptyMandatoryMap;

    auto plan = solver_.Solve(emptyGroups, emptyMandatoryMap);

    EXPECT_EQ(plan.keptSoMap.size(), 0);
    EXPECT_EQ(plan.removedSoMap.size(), 0);
    EXPECT_EQ(plan.totalSavedSize, 0);
}

// 测试3：单个重复SO组 - 单个SO
HWTEST_F(GreedySolverTest, Solve_SingleSoInGroup, TestSize.Level0) {
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

// 测试4：单个重复SO组 - 多个等价SO副本
HWTEST_F(GreedySolverTest, Solve_MultipleSoInGroup_GreedySelection, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.md5 = "abc123";
    group.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module2", 1000});
    group.soList.push_back({"libs/liba.so", "abc123", "module3", 1000});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group};

    // 创建phone设备的必然安装模块（只有module2和module3）
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module2", "module3"};

    auto plan = solver_.Solve(groups, mandatoryMap);

    // module1位于S外且没有设备支持信息，因此必须保守保留。
    // module2/module3是等价副本，保留其中任意一个即可满足phone设备约束。
    EXPECT_EQ(plan.keptSoMap.size(), 2);
    EXPECT_EQ(plan.removedSoMap.size(), 1);
    EXPECT_TRUE(plan.keptSoMap.find("module1") != plan.keptSoMap.end());
    size_t mandatoryKeptCount = plan.keptSoMap.count("module2") + plan.keptSoMap.count("module3");
    EXPECT_EQ(mandatoryKeptCount, 1);
}

// 测试5：大规模场景（模拟单个SO有21个重复副本）
HWTEST_F(GreedySolverTest, Solve_LargeScaleScenario, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.md5 = "abc123";

    // 在21个模块中创建同一路径、同一MD5的SO，即21个重复副本
    for (int i = 1; i <= 21; ++i) {
        std::string moduleName = "module" + std::to_string(i);
        group.soList.push_back({"libs/liba.so", "abc123", moduleName, 100});
    }

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group};

    // 创建phone设备的必然安装模块（前20个模块）
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    for (int i = 1; i <= 20; ++i) {
        mandatoryMap[phoneDevice].push_back("module" + std::to_string(i));
    }

    auto plan = solver_.Solve(groups, mandatoryMap);

    // module21不在任何A(i)中，当前C++接口无法判断其支持设备，保守保留；
    // 前20个等价的必然安装模块中保留任意一个副本即可。
    EXPECT_EQ(plan.keptSoMap.size(), 2);
    EXPECT_EQ(plan.removedSoMap.size(), 19);
    EXPECT_TRUE(plan.keptSoMap.find("module21") != plan.keptSoMap.end());
    size_t mandatoryKeptCount = 0;
    for (int i = 1; i <= 20; ++i) {
        mandatoryKeptCount += plan.keptSoMap.count("module" + std::to_string(i));
    }
    EXPECT_EQ(mandatoryKeptCount, 1);
}

// 测试6：多个重复SO组
HWTEST_F(GreedySolverTest, Solve_MultipleDuplicateGroups, TestSize.Level0) {
    // 创建两个重复SO组
    OHOS::AppPackingTool::DuplicateSoGroup group1;
    group1.md5 = "abc123";
    group1.soList.push_back({"libs/liba.so", "abc123", "module1", 1000});
    group1.soList.push_back({"libs/liba.so", "abc123", "module2", 2000});
    group1.soList.push_back({"libs/liba.so", "abc123", "module3", 1500});

    OHOS::AppPackingTool::DuplicateSoGroup group2;
    group2.md5 = "def456";
    group2.soList.push_back({"libs/libb.so", "def456", "module2", 3000});
    group2.soList.push_back({"libs/libb.so", "def456", "module4", 2500});
    group2.soList.push_back({"libs/libb.so", "def456", "module5", 3500});

    std::vector<OHOS::AppPackingTool::DuplicateSoGroup> groups = {group1, group2};

    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::map<OHOS::AppPackingTool::DeviceInstance, std::vector<std::string>> mandatoryMap;
    mandatoryMap[phoneDevice] = {"module1", "module2", "module3", "module4", "module5"};

    auto plan = solver_.Solve(groups, mandatoryMap);

    // 贪心算法应该为每个组选择最优解
    EXPECT_TRUE(plan.keptSoMap.size() >= 2);
    EXPECT_TRUE(plan.removedSoMap.size() >= 1);

    // 验证节省的空间计算
    EXPECT_GT(plan.totalSavedSize, 0);
}

HWTEST_F(GreedySolverTest, Solve_ModuleOutsideMandatorySetUsesSupportedDevices, TestSize.Level0) {
    OHOS::AppPackingTool::DuplicateSoGroup group;
    group.relativePath = "libs/arm64-v8a/liba.so";
    group.md5 = "same-md5";
    group.soList.push_back({group.relativePath, group.md5, "entry", 1000});
    group.soList.push_back({group.relativePath, group.md5, "ondemand", 2000});

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
