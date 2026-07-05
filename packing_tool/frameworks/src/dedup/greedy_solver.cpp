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

#include "dedup/greedy_solver.h"
#include <algorithm>
#include <set>
#include "dedup/dedup_log.h"
#include "dedup/so_deduplicator.h"

namespace OHOS {
namespace AppPackingTool {

GreedySolver::GreedySolver()
{}
GreedySolver::~GreedySolver()
{}

bool GreedySolver::CanUseGreedyAlgorithm(size_t duplicateCopyCount)
{
    return duplicateCopyCount > DEDUP_ALGORITHM_THRESHOLD;
}

std::vector<SoInfo> GreedySolver::SolveSingleGroupGreedy(
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap)
{
    std::vector<SoInfo> solution;

    if (group.soList.empty()) {
        LOG(WARNING) << "Duplicate SO group is empty";
        return solution;
    }

    if (group.soList.size() == 1) {
    // Only one SO, must keep
        solution.push_back(group.soList[0]);
        return solution;
    }

    LOG(DEBUG) << "Solving duplicate SO group with MD5: " << group.md5.substr(0, MD5_DISPLAY_LENGTH)
              << "... (" << group.soList.size() << " SOs) using greedy algorithm";

    // Initially keep all SOs
    std::vector<SoInfo> remainingSo = group.soList;

    // Greedily try to remove SOs
    for (size_t idx = 0; idx < group.soList.size(); ++idx) {
        const SoInfo& soToRemove = group.soList[idx];

        auto it = std::find_if(remainingSo.begin(), remainingSo.end(),
            [&soToRemove](const SoInfo& so) {
                return so.relativePath == soToRemove.relativePath &&
                       so.sourceModule == soToRemove.sourceModule;
            });
        if (it != remainingSo.end()) {
            std::vector<SoInfo> candidate = remainingSo;
            candidate.erase(candidate.begin() + std::distance(remainingSo.begin(), it));
            if (SatisfiesConstraints(candidate, group, mandatoryModuleMap, moduleSupportMap)) {
                LOG(DEBUG) << "Greedy: removing SO " << soToRemove.relativePath
                          << " from module " << soToRemove.sourceModule
                          << " (size: " << soToRemove.fileSize << " bytes)";
                remainingSo.erase(it);
            }
        }
    }

    LOG(DEBUG) << "Greedy solution for MD5 " << group.md5.substr(0, MD5_DISPLAY_LENGTH)
              << "...: " << remainingSo.size() << " SOs to keep (removed "
              << (group.soList.size() - remainingSo.size()) << " SOs)";

    return remainingSo;
}

DedupPlan GreedySolver::Solve(
    const std::vector<DuplicateSoGroup>& duplicateSoGroups,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap)
{
    DedupPlan plan;

    if (duplicateSoGroups.empty()) {
        LOG(DEBUG) << "No duplicate SO groups to solve";
        return plan;
    }

    LOG(DEBUG) << "Solving " << duplicateSoGroups.size() << " duplicate SO groups using greedy algorithm";

    // Solve for each duplicate SO group
    for (const auto& group : duplicateSoGroups) {
        // 求解该组的贪心保留方案
        std::vector<SoInfo> keptSo = SolveSingleGroupGreedy(group, mandatoryModuleMap, moduleSupportMap);
        // 根据求解结果构建去重方案
        std::set<std::pair<std::string, std::string>> keptSoPaths;
        for (const auto& so : keptSo) {
            keptSoPaths.insert({so.sourceModule, so.relativePath});
            plan.AddKeptSo(so.sourceModule, so.relativePath);
        }
        // Add removed SOs
        for (const auto& so : group.soList) {
            if (keptSoPaths.find({so.sourceModule, so.relativePath}) == keptSoPaths.end()) {
                plan.AddRemovedSo(so.sourceModule, so.relativePath, so.fileSize);
            }
        }
    }

    LOG(DEBUG) << "Greedy solver completed: kept " << plan.keptSoMap.size() << " modules, "
              << "removed " << plan.removedSoMap.size() << " modules, "
              << "saved " << plan.totalSavedSize << " bytes";

    return plan;
}

bool GreedySolver::SatisfiesConstraints(
    const std::vector<SoInfo>& keptSo,
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const
{
    if (keptSo.empty()) {
        return false;
    }
    std::set<std::string> mandatoryUnion;
    for (const auto& [device, mandatoryModules] : mandatoryModuleMap) {
        // 显式创建引用以避免结构化绑定捕获问题
        const std::vector<std::string>& modulesRef = mandatoryModules;
        mandatoryUnion.insert(modulesRef.begin(), modulesRef.end());
        bool originallyInstalled = std::any_of(
            group.soList.begin(), group.soList.end(), [&modulesRef](const SoInfo& so) {
            return std::find(modulesRef.begin(), modulesRef.end(), so.sourceModule) !=
                modulesRef.end();
        });
        bool stillInstalled = std::any_of(keptSo.begin(), keptSo.end(), [&modulesRef](const SoInfo& so) {
            return std::find(modulesRef.begin(), modulesRef.end(), so.sourceModule) !=
                modulesRef.end();
        });
        if (originallyInstalled && !stillInstalled) {
            return false;
        }
    }
    for (const auto& so : group.soList) {
        if (mandatoryUnion.find(so.sourceModule) == mandatoryUnion.end() &&
            std::none_of(keptSo.begin(), keptSo.end(), [&](const SoInfo& kept) {
                return kept.sourceModule == so.sourceModule;
            })) {
            auto supportIt = moduleSupportMap.find(so.sourceModule);
            if (supportIt == moduleSupportMap.end()) {
                return false;
            }
            for (const auto& supportedDevice : supportIt->second) {
                auto mandatoryIt = mandatoryModuleMap.find(supportedDevice);
                if (mandatoryIt == mandatoryModuleMap.end() ||
                    !std::any_of(keptSo.begin(), keptSo.end(), [&](const SoInfo& kept) {
                        return std::find(mandatoryIt->second.begin(), mandatoryIt->second.end(),
                            kept.sourceModule) != mandatoryIt->second.end();
                    })) {
                    return false;
                }
            }
        }
    }
    return true;
}
}  // namespace AppPackingTool
}  // namespace OHOS
