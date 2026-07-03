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

#include "dedup/exact_solver.h"
#include <algorithm>
#include <set>
#include <stdexcept>
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"

namespace OHOS {
namespace AppPackingTool {

ExactSolver::ExactSolver() {}
ExactSolver::~ExactSolver() {}

bool ExactSolver::CanUseExactAlgorithm(size_t duplicateCopyCount) {
    return duplicateCopyCount <= 20;
}

std::vector<SoInfo> ExactSolver::SolveSingleGroup(
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) {

    std::vector<SoInfo> optimalSolution;

    if (group.soList.empty()) {
        LOG(WARNING) << "Duplicate SO group is empty";
        return optimalSolution;
    }

    if (group.soList.size() == 1) {
        // 只有一个SO，必须保留
        optimalSolution.push_back(group.soList[0]);
        return optimalSolution;
    }

    LOG(DEBUG) << "Solving duplicate SO group with MD5: " << group.md5.substr(0, 8)
              << "... (" << group.soList.size() << " SOs)";

    // 模块数不超过20，枚举所有非空保留子集，得到真正的最优解。
    std::vector<SoInfo> bestSolution = group.soList;
    const uint64_t subsetCount = 1ULL << group.soList.size();
    for (uint64_t mask = 1; mask < subsetCount; ++mask) {
        size_t candidateSize = 0;
        for (uint64_t bits = mask; bits != 0; bits &= (bits - 1)) {
            ++candidateSize;
        }
        if (candidateSize >= bestSolution.size()) {
            continue;
        }
        std::vector<SoInfo> candidate;
        candidate.reserve(candidateSize);
        for (size_t index = 0; index < group.soList.size(); ++index) {
            if ((mask & (1ULL << index)) != 0) {
                candidate.push_back(group.soList[index]);
            }
        }
        if (SatisfiesConstraints(candidate, group, mandatoryModuleMap, moduleSupportMap)) {
            bestSolution = candidate;
            if (bestSolution.size() == 1) {
                break;
            }
        }
    }

    LOG(DEBUG) << "Optimal solution for MD5 " << group.md5.substr(0, 8)
              << "...: " << bestSolution.size() << " SOs to keep (removed "
              << (group.soList.size() - bestSolution.size()) << " SOs)";

    return bestSolution;
}

DedupPlan ExactSolver::Solve(
    const std::vector<DuplicateSoGroup>& duplicateSoGroups,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) {

    DedupPlan plan;

    if (duplicateSoGroups.empty()) {
        LOG(DEBUG) << "No duplicate SO groups to solve";
        return plan;
    }

    LOG(DEBUG) << "Solving " << duplicateSoGroups.size() << " duplicate SO groups using exact algorithm";

    // 对每个重复SO组求解
    for (const auto& group : duplicateSoGroups) {
        // 求解该组的最优保留方案
        std::vector<SoInfo> keptSo = SolveSingleGroup(group, mandatoryModuleMap, moduleSupportMap);

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

    LOG(DEBUG) << "Exact solver completed: kept " << plan.keptSoMap.size() << " modules, "
              << "removed " << plan.removedSoMap.size() << " modules, "
              << "saved " << plan.totalSavedSize << " bytes";

    // 验证方案
    if (!ValidatePlan(plan, duplicateSoGroups, mandatoryModuleMap, moduleSupportMap)) {
        LOG(ERROR) << FormatDedupError("Generated deduplication plan does not satisfy constraints");
        // 返回空方案表示失败
        return DedupPlan();
    }

    return plan;
}

bool ExactSolver::ValidatePlan(
    const DedupPlan& plan,
    const std::vector<DuplicateSoGroup>& duplicateSoGroups,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const {

    LOG(DEBUG) << "Validating deduplication plan";

    // 对每个设备检查约束
    for (const auto& [device, mandatoryModules] : mandatoryModuleMap) {
        // 显式创建引用以避免结构化绑定捕获问题
        const std::vector<std::string>& modulesRef = mandatoryModules;

        // 对每个重复SO组检查
        for (const auto& group : duplicateSoGroups) {
            // 检查该设备的必然安装模块中是否有至少一个包含该SO
            bool hasSoInMandatoryModules = false;

            for (const auto& moduleName : mandatoryModules) {
                // 检查该模块的保留SO中是否有该MD5的SO
                auto keptIt = plan.keptSoMap.find(moduleName);
                if (keptIt != plan.keptSoMap.end()) {
                    for (const auto& soPath : keptIt->second) {
                        // 检查该SO路径是否在重复组中
                        for (const auto& so : group.soList) {
                            if (so.relativePath == soPath && so.sourceModule == moduleName) {
                                hasSoInMandatoryModules = true;
                                break;
                            }
                        }
                        if (hasSoInMandatoryModules) {
                            break;
                        }
                    }
                }
                if (hasSoInMandatoryModules) {
                    break;
                }
            }

            bool originallyInstalled = std::any_of(
                group.soList.begin(), group.soList.end(), [&modulesRef](const SoInfo& so) {
                return std::find(modulesRef.begin(), modulesRef.end(), so.sourceModule) !=
                    modulesRef.end();
            });
            if (originallyInstalled && !hasSoInMandatoryModules) {
                std::string cause = "Device " + DeviceCalculator::DeviceTypeToString(device.type) +
                    " does not have SO with MD5 " + group.md5.substr(0, 8) +
                    "... in its mandatory modules";
                LOG(ERROR) << FormatDedupError(cause);
                return false;
            }
        }
    }

    for (const auto& group : duplicateSoGroups) {
        std::vector<SoInfo> kept;
        for (const auto& so : group.soList) {
            auto keptIt = plan.keptSoMap.find(so.sourceModule);
            if (keptIt != plan.keptSoMap.end() &&
                std::find(keptIt->second.begin(), keptIt->second.end(), so.relativePath) != keptIt->second.end()) {
                kept.push_back(so);
            }
        }
        if (!SatisfiesConstraints(kept, group, mandatoryModuleMap, moduleSupportMap)) {
            return false;
        }
    }
    LOG(DEBUG) << "Deduplication plan validation passed";
    return true;
}

bool ExactSolver::SatisfiesConstraints(
    const std::vector<SoInfo>& keptSo,
    const DuplicateSoGroup& group,
    const std::map<DeviceInstance, std::vector<std::string>>& mandatoryModuleMap,
    const std::map<std::string, std::vector<DeviceInstance>>& moduleSupportMap) const {
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
    // S外模块仅在其支持的每个设备上都有必装模块副本时才允许删除。
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
