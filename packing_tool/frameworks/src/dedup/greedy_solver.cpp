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

namespace OHOS {
namespace AppPackingTool {

GreedySolver::GreedySolver() {}
GreedySolver::~GreedySolver() {}

bool GreedySolver::CanUseGreedyAlgorithm(int32_t totalModuleCount) {
    return totalModuleCount > 20;
}

std::vector<size_t> GreedySolver::SortBySavedSize(const std::vector<SoInfo>& soList) const {
    std::vector<size_t> indices(soList.size());
    for (size_t i = 0; i < soList.size(); ++i) {
        indices[i] = i;
    }

    // 按文件大小降序排序
    std::sort(indices.begin(), indices.end(), [&soList](size_t a, size_t b) {
        return soList[a].fileSize > soList[b].fileSize;
    });

    return indices;
}

std::vector<SoInfo> GreedySolver::SolveSingleGroupGreedy(
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) {

    std::vector<SoInfo> solution;

    if (group.soList.empty()) {
        LOG(WARNING) << "Duplicate SO group is empty";
        return solution;
    }

    if (group.soList.size() == 1) {
        // 只有一个SO，必须保留
        solution.push_back(group.soList[0]);
        return solution;
    }

    LOG(INFO) << "Solving duplicate SO group with MD5: " << group.md5.substr(0, 8)
              << "... (" << group.soList.size() << " SOs) using greedy algorithm";

    // 初始时保留所有SO
    std::vector<SoInfo> remainingSo = group.soList;

    // 按文件大小降序排序（贪心策略：优先移除大的文件）
    std::vector<size_t> sortedIndices = SortBySavedSize(group.soList);

    // 贪心地尝试移除SO
    for (size_t idx : sortedIndices) {
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
                LOG(INFO) << "Greedy: removing SO " << soToRemove.relativePath
                          << " from module " << soToRemove.sourceModule
                          << " (size: " << soToRemove.fileSize << " bytes)";
                remainingSo.erase(it);
            }
        }
    }

    LOG(INFO) << "Greedy solution for MD5 " << group.md5.substr(0, 8)
              << "...: " << remainingSo.size() << " SOs to keep (removed "
              << (group.soList.size() - remainingSo.size()) << " SOs)";

    return remainingSo;
}

DedupPlan GreedySolver::Solve(
    const std::vector<DuplicateSoGroup>& duplicateSoGroups,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) {

    DedupPlan plan;

    if (duplicateSoGroups.empty()) {
        LOG(INFO) << "No duplicate SO groups to solve";
        return plan;
    }

    LOG(INFO) << "Solving " << duplicateSoGroups.size() << " duplicate SO groups using greedy algorithm";

    // 对每个重复SO组求解
    for (const auto& group : duplicateSoGroups) {
        // 求解该组的贪心保留方案
        std::vector<SoInfo> keptSo = SolveSingleGroupGreedy(group, mandatoryModuleMap, moduleSupportMap);

        // 根据求解结果构建去重方案
        std::set<std::pair<std::string, std::string>> keptSoPaths;
        for (const auto& so : keptSo) {
            keptSoPaths.insert({so.sourceModule, so.relativePath});
            plan.AddKeptSo(so.sourceModule, so.relativePath);
        }

        // 添加移除的SO
        for (const auto& so : group.soList) {
            if (keptSoPaths.find({so.sourceModule, so.relativePath}) == keptSoPaths.end()) {
                plan.AddRemovedSo(so.sourceModule, so.relativePath, so.fileSize);
            }
        }
    }

    LOG(INFO) << "Greedy solver completed: kept " << plan.keptSoMap.size() << " modules, "
              << "removed " << plan.removedSoMap.size() << " modules, "
              << "saved " << plan.totalSavedSize << " bytes";

    return plan;
}

bool GreedySolver::SatisfiesConstraints(
    const std::vector<SoInfo>& keptSo,
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const {
    if (keptSo.empty()) {
        return false;
    }
    std::set<std::string> mandatoryUnion;
    for (const auto& [device, mandatoryModules] : mandatoryModuleMap) {
        mandatoryUnion.insert(mandatoryModules.begin(), mandatoryModules.end());
        bool originallyInstalled = std::any_of(group.soList.begin(), group.soList.end(), [&](const SoInfo& so) {
            return std::find(mandatoryModules.begin(), mandatoryModules.end(), so.sourceModule) !=
                mandatoryModules.end();
        });
        bool stillInstalled = std::any_of(keptSo.begin(), keptSo.end(), [&](const SoInfo& so) {
            return std::find(mandatoryModules.begin(), mandatoryModules.end(), so.sourceModule) !=
                mandatoryModules.end();
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
