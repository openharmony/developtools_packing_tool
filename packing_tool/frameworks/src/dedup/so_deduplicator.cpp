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

#include "dedup/so_deduplicator.h"
#include "dedup/device_calculator.h"
#include "dedup/module_calculator.h"
#include "dedup/exact_solver.h"
#include "dedup/greedy_solver.h"
#include "dedup/report_generator.h"
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"
#include "zip_utils.h"
#include "utils.h"
#include <openssl/md5.h>
#include <algorithm>
#include <fstream>
#include <iomanip>

namespace OHOS {
namespace AppPackingTool {

SODeduplicator::SODeduplicator()
    : dedupSuccess_(false) {}

SODeduplicator::~SODeduplicator() {}

void SODeduplicator::SetError(const std::string& error) {
    errorMessage_ = error;
    dedupSuccess_ = false;
    LOG(ERROR) << FormatDedupError(error);
}

std::string SODeduplicator::CalculateFileMD5(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        LOG(ERROR) << FormatDedupError("Failed to open file for MD5 calculation: " + filePath);
        return "";
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    constexpr size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    while (file.read(buffer, BUFFER_SIZE)) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }

    // 处理剩余数据
    if (file.gcount() > 0) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }

    unsigned char md5Digest[MD5_DIGEST_LENGTH];
    MD5_Final(md5Digest, &md5Context);

    // 转换为十六进制字符串
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5Digest[i]);
    }

    std::string md5String = ss.str();
    LOG(INFO) << "Calculated MD5 for file: " << filePath << " -> " << md5String.substr(0, 8) << "...";

    return md5String;
}

std::vector<std::shared_ptr<ModuleJson>> SODeduplicator::FilterEntryModules(
    const std::vector<std::shared_ptr<ModuleJson>>& allModules) {

    std::vector<std::shared_ptr<ModuleJson>> entryModules;

    for (const auto& module : allModules) {
        if (!module) {
            continue;
        }

        std::string moduleType;
        if (module->GetStageModuleType(moduleType)) {
            if (moduleType == "entry") {
                entryModules.push_back(module);
            }
        } else {
            module->GetFaModuleType(moduleType);
            if (moduleType == "entry") {
                entryModules.push_back(module);
            }
        }
    }

    LOG(INFO) << "Filtered " << entryModules.size() << " entry modules from "
              << allModules.size() << " total modules";

    return entryModules;
}

std::vector<std::shared_ptr<ModuleJson>> SODeduplicator::FilterDedupEligibleModules(
    const std::vector<std::shared_ptr<ModuleJson>>& allModules) {

    std::vector<std::shared_ptr<ModuleJson>> eligibleModules;

    for (const auto& module : allModules) {
        if (!module) {
            continue;
        }

        try {
            // 检查模块是否满足去重条件
            ModuleConfig config = ModuleCalculator::ExtractModuleConfig(module);
            if (config.moduleName.empty()) {
                SetError("Failed to parse module configuration");
                break;
            }

            // 条件1：会解压SO (compressNativeLibs 或 extraNativeLibs 为 true)
            if (!config.compressNativeLibs && !config.extraNativeLibs) {
                LOG(INFO) << "Module " << config.moduleName << " not eligible: SO not extracted";
                continue;
            }

            // 条件2：libIsolation 为 false
            if (config.libIsolation) {
                LOG(INFO) << "Module " << config.moduleName << " not eligible: libIsolation enabled";
                continue;
            }

            eligibleModules.push_back(module);
            LOG(INFO) << "Module " << config.moduleName << " is eligible for deduplication";

        } catch (const std::exception& e) {
            LOG(WARNING) << "Failed to check module eligibility: " << e.what();
        }
    }

    LOG(INFO) << "Found " << eligibleModules.size() << " dedup-eligible modules from "
              << allModules.size() << " total modules";

    return eligibleModules;
}

std::map<std::string, std::vector<SoInfo>> SODeduplicator::CollectSoFiles(
    const std::vector<std::shared_ptr<ModuleJson>>& modules,
    const std::string& modulesRootPath) {

    std::map<std::string, std::vector<SoInfo>> moduleSoMap;

    if (modulesRootPath.empty()) {
        SetError("Modules root path is empty");
        return moduleSoMap;
    }

    LOG(INFO) << "Collecting SO files from " << modules.size() << " modules in " << modulesRootPath;

    for (const auto& module : modules) {
        if (!module) {
            continue;
        }

        try {
            std::string moduleName;
            if (!module->GetStageModuleName(moduleName)) {
                module->GetFaModuleName(moduleName);
            }

            std::string modulePath = modulesRootPath + "/" + moduleName;

            // 检查模块目录是否存在
            if (!std::filesystem::exists(modulePath)) {
                SetError("Module directory does not exist: " + modulePath);
                return {};
            }

            // 遍历模块目录，查找.so文件
            std::vector<SoInfo> soFiles;
            for (const auto& entry : std::filesystem::recursive_directory_iterator(modulePath)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    if (entry.path().extension() == ".so") {
                        // 计算MD5
                        std::string md5 = CalculateFileMD5(filePath);
                        if (md5.empty()) {
                            SetError("Failed to calculate MD5 for file: " + filePath);
                            return {};
                        }

                        // 获取相对路径
                        std::string relativePath = std::filesystem::relative(entry.path(), modulePath).generic_string();

                        // 获取文件大小
                        int64_t fileSize = std::filesystem::file_size(entry.path());

                        SoInfo soInfo;
                        soInfo.relativePath = relativePath;
                        soInfo.md5 = md5;
                        soInfo.sourceModule = moduleName;
                        soInfo.fileSize = fileSize;

                        soFiles.push_back(soInfo);
                        LOG(INFO) << "Found SO file: " << relativePath << " (MD5: " << md5.substr(0, 8) << "...)";
                    }
                }
            }

            if (!soFiles.empty()) {
                moduleSoMap[moduleName] = soFiles;
                LOG(INFO) << "Module " << moduleName << " contains " << soFiles.size() << " SO files";
            }

        } catch (const std::exception& e) {
            SetError("Failed to collect SO files from module: " + std::string(e.what()));
            return {};
        }
    }

    int32_t totalSoCount = 0;
    for (const auto& [moduleName, soList] : moduleSoMap) {
        totalSoCount += soList.size();
    }

    LOG(INFO) << "Collected " << totalSoCount << " SO files from "
              << moduleSoMap.size() << " modules";

    return moduleSoMap;
}

std::vector<DuplicateSoGroup> SODeduplicator::GroupDuplicateSos(
    const std::map<std::string, std::vector<SoInfo>>& moduleSoMap) {

    std::vector<DuplicateSoGroup> duplicateGroups;

    // 按模块内相对路径和MD5分组。同内容但安装路径不同的SO不能互相替代。
    std::map<std::pair<std::string, std::string>, std::vector<SoInfo>> duplicateMap;

    for (const auto& [moduleName, soList] : moduleSoMap) {
        for (const auto& so : soList) {
            duplicateMap[{so.relativePath, so.md5}].push_back(so);
        }
    }

    // 创建重复SO组
    for (const auto& [identity, soList] : duplicateMap) {
        if (soList.size() > 1) { // 只处理重复的SO
            DuplicateSoGroup group;
            group.relativePath = identity.first;
            group.md5 = identity.second;
            group.soList = soList;
            duplicateGroups.push_back(group);

            LOG(INFO) << "Found duplicate SO group " << group.relativePath << " with MD5 "
                      << group.md5.substr(0, 8)
                      << "... (" << soList.size() << " SOs)";
        }
    }

    LOG(INFO) << "Found " << duplicateGroups.size() << " duplicate SO groups";

    return duplicateGroups;
}

bool SODeduplicator::ApplyDedupPlan(const DedupPlan& plan, const std::string& modulesRootPath) {
    LOG(INFO) << "Applying deduplication plan";

    if (modulesRootPath.empty()) {
        SetError("Modules root path is empty for applying dedup plan");
        return false;
    }

    int32_t removedCount = 0;

    // 移除标记为删除的SO文件
    for (const auto& [moduleName, soPaths] : plan.removedSoMap) {
        for (const auto& soPath : soPaths) {
            std::string fullFilePath = modulesRootPath + "/" + moduleName + "/" + soPath;

            try {
                if (std::filesystem::exists(fullFilePath)) {
                    std::filesystem::remove(fullFilePath);
                    removedCount++;
                    LOG(INFO) << "Removed SO file: " << fullFilePath;
                } else {
                    LOG(WARNING) << "SO file does not exist, cannot remove: " << fullFilePath;
                }
            } catch (const std::exception& e) {
                LOG(ERROR) << FormatDedupError(
                    "Failed to remove SO file: " + fullFilePath + " - " + e.what());
                SetError("Failed to remove SO file: " + fullFilePath);
                return false;
            }
        }
    }

    LOG(INFO) << "Successfully applied deduplication plan: removed " << removedCount << " SO files";

    return true;
}

bool SODeduplicator::DeduplicateModules(std::list<std::string>& modulePaths,
    bool deduplicateSo, const std::string& workDir, const std::string& reportDir)
{
    if (!deduplicateSo) {
        LOG(INFO) << "[SO_DEDUP] SO deduplication skipped: disabled.";
        return true;
    }
    if (modulePaths.empty()) {
        return true;
    }

    try {
        std::filesystem::path root = std::filesystem::path(workDir) /
            ("so_dedup_" + Utils::GenerateUUID());
        std::filesystem::path modulesRoot = root / "modules";
        std::filesystem::path repackedRoot = root / "repacked";
        std::filesystem::create_directories(modulesRoot);
        std::filesystem::create_directories(repackedRoot);

        std::vector<std::shared_ptr<ModuleJson>> modules;
        std::vector<std::pair<std::filesystem::path, std::filesystem::path>> moduleFiles;
        size_t index = 0;
        for (const auto& modulePath : modulePaths) {
            std::filesystem::path source(modulePath);
            std::filesystem::path extractDir = root / ("extract_" + std::to_string(index++));
            std::filesystem::create_directories(extractDir);
            if (ZipUtils::Unzip(source.string(), extractDir.string()) != ZIP_ERR_SUCCESS) {
                SetError("Failed to unzip module: " + source.string());
                return false;
            }
            std::filesystem::path jsonPath = extractDir / "module.json";
            if (!std::filesystem::exists(jsonPath)) {
                jsonPath = extractDir / "config.json";
            }
            auto module = std::make_shared<ModuleJson>();
            if (!std::filesystem::exists(jsonPath) || !module->ParseFromFile(jsonPath.string())) {
                SetError("Failed to parse module configuration: " + source.string());
                return false;
            }
            ModuleConfig config = ModuleCalculator::ExtractModuleConfig(module);
            if (config.moduleName.empty()) {
                SetError("Module name is empty: " + source.string());
                return false;
            }
            std::filesystem::path finalExtractDir = modulesRoot / config.moduleName;
            if (std::filesystem::exists(finalExtractDir)) {
                SetError("Duplicate module name: " + config.moduleName);
                return false;
            }
            std::filesystem::rename(extractDir, finalExtractDir);
            modules.push_back(module);
            moduleFiles.push_back({finalExtractDir, repackedRoot / source.filename()});
        }

        std::string effectiveReportDir = reportDir.empty() ? "." : reportDir;
        if (!ExecuteDeduplication(modules, root.string(), effectiveReportDir, true)) {
            return false;
        }

        std::list<std::string> repackedPaths;
        for (const auto& [moduleDir, repackedPath] : moduleFiles) {
            if (ZipUtils::Zip(moduleDir.string(), repackedPath.string(), "") != ZIP_ERR_SUCCESS) {
                SetError("Failed to repack module: " + repackedPath.string());
                return false;
            }
            repackedPaths.push_back(repackedPath.string());
        }
        modulePaths.swap(repackedPaths);
        size_t removedCount = 0;
        for (const auto& [moduleName, paths] : dedupPlan_.removedSoMap) {
            removedCount += paths.size();
        }
        LOG(INFO) << "[SO_DEDUP] SO deduplication completed: removed=" << removedCount
                  << ", savedBytes=" << dedupPlan_.totalSavedSize << ", report=" << reportFilePath_;
        return true;
    } catch (const std::exception& exception) {
        SetError("Exception during deduplication: " + std::string(exception.what()));
        return false;
    }
}

bool SODeduplicator::ExecuteDeduplication(
    const std::vector<std::shared_ptr<ModuleJson>>& allModules,
    const std::string& outputRootPath,
    const std::string& reportOutputPath,
    bool dedupEnabled) {

    LOG(INFO) << "[SO_DEDUP] SO deduplication started.";
    LOG(INFO) << "Deduplication enabled: " << (dedupEnabled ? "true" : "false");
    LOG(INFO) << "Total modules: " << allModules.size();

    // 重置状态
    dedupSuccess_ = false;
    errorMessage_ = "";
    dedupPlan_ = DedupPlan();

    if (!dedupEnabled) {
        LOG(INFO) << "[SO_DEDUP] SO deduplication skipped: disabled.";
        dedupSuccess_ = true; // 不启用去重不算失败
        return true;
    }

    if (allModules.empty()) {
        SetError("No modules provided for deduplication");
        return false;
    }

    try {
        // 第1步：过滤entry模块
        std::vector<std::shared_ptr<ModuleJson>> entryModules = FilterEntryModules(allModules);

        // 第2步：计算设备集合
        DeviceCalculator deviceCalculator;
        std::vector<DeviceInstance> devices = deviceCalculator.CalculateDevices(entryModules);

        if (devices.empty()) {
            SetError("No devices found for deduplication");
            return false;
        }

        // 第3步：过滤参与去重的模块
        std::vector<std::shared_ptr<ModuleJson>> eligibleModules = FilterDedupEligibleModules(allModules);
        if (!errorMessage_.empty()) {
            return false;
        }
        std::string strategyName = ExactSolver::CanUseExactAlgorithm(eligibleModules.size()) ? "exact" : "greedy";
        LOG(INFO) << "[SO_DEDUP] SO deduplication strategy: " << strategyName
                  << ", eligibleModules=" << eligibleModules.size() << ".";

        if (eligibleModules.empty()) {
            LOG(INFO) << "No modules eligible for deduplication";
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, DedupStrategy::EXACT, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // 第4步：计算必然安装模块集合
        ModuleCalculator moduleCalculator;
        std::map<DeviceInstance, std::vector<std::string>> mandatoryModuleMap =
            moduleCalculator.CalculateMandatoryModules(eligibleModules, devices);
        std::map<std::string, std::vector<DeviceInstance>> moduleSupportMap;
        for (const auto& module : eligibleModules) {
            ModuleConfig config = ModuleCalculator::ExtractModuleConfig(module);
            for (const auto& device : devices) {
                bool supportsType = std::find(config.deviceTypes.begin(), config.deviceTypes.end(), device.type) !=
                    config.deviceTypes.end();
                bool supportsFilter = config.distributionFilter.empty() ||
                    config.distributionFilter == device.distributionFilter;
                if (supportsType && supportsFilter) {
                    moduleSupportMap[config.moduleName].push_back(device);
                }
            }
        }

        // 第5步：收集SO文件信息
        std::string modulesRootPath = outputRootPath + "/modules"; // 假设模块在此目录
        std::map<std::string, std::vector<SoInfo>> moduleSoMap =
            CollectSoFiles(eligibleModules, modulesRootPath);
        if (!errorMessage_.empty()) {
            return false;
        }

        if (moduleSoMap.empty()) {
            LOG(INFO) << "No SO files found for deduplication";
            DedupStrategy emptyStrategy = ExactSolver::CanUseExactAlgorithm(eligibleModules.size()) ?
                DedupStrategy::EXACT : DedupStrategy::GREEDY;
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, emptyStrategy, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // 第6步：分组重复SO
        std::vector<DuplicateSoGroup> duplicateGroups = GroupDuplicateSos(moduleSoMap);

        if (duplicateGroups.empty()) {
            LOG(INFO) << "No duplicate SO groups found";
            DedupStrategy emptyStrategy = ExactSolver::CanUseExactAlgorithm(eligibleModules.size()) ?
                DedupStrategy::EXACT : DedupStrategy::GREEDY;
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, emptyStrategy, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // 第7步：选择去重算法并求解
        DedupStrategy strategy;
        if (ExactSolver::CanUseExactAlgorithm(eligibleModules.size())) {
            LOG(INFO) << "Using EXACT algorithm for " << eligibleModules.size() << " modules";
            strategy = DedupStrategy::EXACT;

            ExactSolver exactSolver;
            dedupPlan_ = exactSolver.Solve(duplicateGroups, mandatoryModuleMap, moduleSupportMap);
        } else {
            LOG(INFO) << "Using GREEDY algorithm for " << eligibleModules.size() << " modules";
            strategy = DedupStrategy::GREEDY;

            GreedySolver greedySolver;
            dedupPlan_ = greedySolver.Solve(duplicateGroups, mandatoryModuleMap, moduleSupportMap);
        }

        if (dedupPlan_.keptSoMap.empty() && dedupPlan_.removedSoMap.empty()) {
            SetError("Failed to generate deduplication plan");
            return false;
        }

        // 第8步：应用去重方案
        if (!ApplyDedupPlan(dedupPlan_, modulesRootPath)) {
            return false;
        }

        // 第9步：生成去重报告
        ReportGenerator reportGenerator;
        reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, strategy, reportOutputPath);

        if (reportFilePath_.empty()) {
            SetError("Failed to generate deduplication report");
            return false;
        }

        dedupSuccess_ = true;
        return true;

    } catch (const std::exception& e) {
        SetError("Exception during deduplication: " + std::string(e.what()));
        return false;
    }
}

}  // namespace AppPackingTool
}  // namespace OHOS
