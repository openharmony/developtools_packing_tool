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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_EXACT_SOLVER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_EXACT_SOLVER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "dedup/so_info.h"
#include "dedup/device_calculator.h"

namespace OHOS {
namespace AppPackingTool {

// 前向声明
class ModuleCalculator;

/**
 * @brief 精确去重求解器
 *
 * 使用整数规划求解最小击中集问题（Set Cover Problem）
 * 适用于模块数 ≤ 20的场景
 *
 * 问题定义：
 * - 输入：重复SO组列表，每个设备的必然安装模块集合
 * - 约束：每个设备i的A(i)中，每个重复SO至少保留一个
 * - 目标：最小化移除的SO总数（或最大化节省的空间）
 */
class ExactSolver {
public:
    ExactSolver();
    ~ExactSolver();

    /**
     * @brief 求解去重方案
     * @param duplicateSoGroups 重复SO组列表
     * @param mandatoryModuleMap 每个设备的必然安装模块集合
     * @return 去重方案
     */
    DedupPlan Solve(const std::vector<DuplicateSoGroup>& duplicateSoGroups,
                   const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
                   const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap = {});

    /**
     * @brief 检查是否可以使用精确算法
     * @param totalModuleCount 总模块数
     * @return 是否可以使用精确算法
     */
    static bool CanUseExactAlgorithm(int32_t totalModuleCount);

private:
    /**
     * @brief 验证去重方案是否满足约束
     * @param plan 去重方案
     * @param duplicateSoGroups 重复SO组列表
     * @param mandatoryModuleMap 每个设备的必然安装模块集合
     * @return 是否满足约束
     */
    bool ValidatePlan(const DedupPlan& plan,
                     const std::vector<DuplicateSoGroup>& duplicateSoGroups,
                     const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
                     const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const;

    /**
     * @brief 为单个重复SO组计算最优去重方案
     * @param group 重复SO组
     * @param mandatoryModuleMap 每个设备的必然安装模块集合
     * @return 该组的去重方案（保留的SO列表）
     */
    std::vector<SoInfo> SolveSingleGroup(const DuplicateSoGroup& group,
                                         const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
                                         const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap);

    bool SatisfiesConstraints(
        const std::vector<SoInfo>& keptSo,
        const DuplicateSoGroup& group,
        const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
        const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_EXACT_SOLVER_H
