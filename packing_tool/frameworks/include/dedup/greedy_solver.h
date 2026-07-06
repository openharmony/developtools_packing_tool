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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_GREEDY_SOLVER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_GREEDY_SOLVER_H

#include <string>
#include <vector>
#include <map>
#include "dedup/so_info.h"
#include "dedup/device_calculator.h"

namespace OHOS {
namespace AppPackingTool {

/**
 * @brief 贪心去重求解器
 *
 * 使用贪心启发式算法求解大规模去重问题
 * 适用于模块数 > 20的场景
 *
 * 算法流程：
 * 1. 初始化：保留所有SO
 * 2. 对于每个重复SO集合R：
 *    a. 找到包含该SO的所有模块
 *    b. 按模块顺序检查移除后是否满足约束
 *    c. 移除满足约束的副本
 * 3. 重复直到无法进一步去重
 */
class GreedySolver {
public:
    GreedySolver();
    ~GreedySolver();

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
     * @brief 检查是否可以使用贪心算法
     * @param duplicateCopyCount 单个SO的重复副本数
     * @return 是否可以使用贪心算法
     */
    static bool CanUseGreedyAlgorithm(size_t duplicateCopyCount);

private:
    /**
     * @brief 为单个重复SO组计算贪心去重方案
     * @param group 重复SO组
     * @param mandatoryModuleMap 每个设备的必然安装模块集合
     * @return 该组的去重方案（保留的SO列表）
     */
    std::vector<SoInfo> SolveSingleGroupGreedy(
        const DuplicateSoGroup& group,
        const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
        const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap);

    /**
     * @brief 检查移除指定SO后是否满足约束
     * @param soToRemove 要移除的SO
     * @param group 重复SO组
     * @param mandatoryModuleMap 每个设备的必然安装模块集合
     * @return 是否可以移除
     */
    bool SatisfiesConstraints(
        const std::vector<SoInfo>& keptSo,
        const DuplicateSoGroup& group,
        const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
        const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_GREEDY_SOLVER_H
