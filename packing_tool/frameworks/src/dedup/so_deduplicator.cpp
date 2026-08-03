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
#include "zip_wrapper.h"
#include "unzip_wrapper.h"
#include "utils.h"
#include <openssl/md5.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <regex>
#include <set>
#include <sstream>
#include <tuple>

namespace OHOS {
namespace AppPackingTool {

SODeduplicator::SODeduplicator()
    : dedupSuccess_(false) {}

SODeduplicator::~SODeduplicator()
{}

void SODeduplicator::SetError(const std::string& error)
{
    errorMessage_ = error;
    dedupSuccess_ = false;
    LOG(ERROR) << FormatDedupError(error);
}

bool SODeduplicator::IsSoFile(const std::string& fileName)
{
    try {
        // Match all .so files and versioned .so files (e.g., libtest.so.1.2.3)
        static const std::regex SO_PATTERN(".*\\.so(\\.\\d+)*", std::regex_constants::icase);
        return std::regex_match(fileName, SO_PATTERN);
    } catch (const std::exception& e) {
        LOG(ERROR) << FormatDedupError("Failed to match SO file name: " + std::string(e.what()));
        throw;
    }
}

std::string SODeduplicator::CalculateFileMD5(
    const std::string& filePath, std::vector<std::string>* processedSoPaths)
{
    if (processedSoPaths != nullptr) {
        processedSoPaths->push_back(filePath);
    }
    std::string realFilePath;
    if (!Utils::GetRealPath(filePath, realFilePath) || !std::filesystem::is_regular_file(realFilePath)) {
        LOG(ERROR) << FormatDedupError("Invalid file path for MD5 calculation: " + filePath);
        return "";
    }
    std::ifstream file(realFilePath, std::ios::binary);
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
        ss << std::hex << std::setw(MD5_HEX_WIDTH) << std::setfill('0') << static_cast<int>(md5Digest[i]);
    }

    std::string md5String = ss.str();
    LOG(DEBUG) << "Calculated MD5 for file: " << filePath << " -> " << md5String.substr(0, MD5_DISPLAY_LENGTH) << "...";

    return md5String;
}

std::vector<const SODeduplicator::ModuleDedupContext*> SODeduplicator::FilterEntryModules(
    const std::vector<ModuleDedupContext>& allModules)
{
    std::vector<const ModuleDedupContext*> entryModules;

    for (const auto& context : allModules) {
        if (!context.module) {
            continue;
        }

        if (ModuleCalculator::IsValidForDedup(context.config) && context.config.moduleType == "entry") {
            entryModules.push_back(&context);
        }
    }

    LOG(DEBUG) << "Filtered " << entryModules.size() << " entry modules from "
              << allModules.size() << " total modules";

    return entryModules;
}

std::vector<const SODeduplicator::ModuleDedupContext*> SODeduplicator::FilterDedupEligibleModules(
    const std::vector<ModuleDedupContext>& allModules)
{
    std::vector<const ModuleDedupContext*> eligibleModules;

    for (const auto& context : allModules) {
        if (!context.module) {
            continue;
        }

        try {
            const ModuleConfig& config = context.config;
            if (!config.stageModel) {
                continue;
            }
            if (!ModuleCalculator::IsValidForDedup(config)) {
                continue;
            }

            // Condition 1: Will extract SO (compressNativeLibs or extractNativeLibs is true)
            if (!config.compressNativeLibs && !config.extractNativeLibs) {
                LOG(DEBUG) << "Module " << config.moduleName << " not eligible: SO not extracted";
                continue;
            }

            // Condition 2: libIsolation is false
            if (config.libIsolation) {
                LOG(DEBUG) << "Module " << config.moduleName << " not eligible: libIsolation enabled";
                continue;
            }

            // Condition 3: compileSdkType cannot be "OpenHarmony"
            if (config.compileSdkType == "OpenHarmony") {
                continue;
            }

            eligibleModules.push_back(&context);
        } catch (const std::exception& e) {
            LOG(WARNING) << "Failed to check module eligibility: " << e.what();
        }
    }

    LOG(DEBUG) << "Found " << eligibleModules.size() << " dedup-eligible modules from "
              << allModules.size() << " total modules";

    return eligibleModules;
}

std::map<std::string, std::vector<SoInfo>> SODeduplicator::CollectSoFiles(
    const std::vector<const ModuleDedupContext*>& modules, std::vector<std::string>* processedSoPaths)
{
    std::map<std::string, std::vector<SoInfo>> moduleSoMap;

    LOG(DEBUG) << "Collecting SO files from " << modules.size() << " modules";

    for (const auto* context : modules) {
        if (context == nullptr || !context->module) {
            continue;
        }

        try {
            const std::filesystem::path& modulePath = context->extractedPath;

            // Check if module directory exists
            if (!std::filesystem::exists(modulePath)) {
                SetError("Module directory does not exist: " + modulePath.string());
                return {};
            }

            // Traverse module directory to find .so files
            std::vector<SoInfo> soFiles;
            for (const auto& entry : std::filesystem::recursive_directory_iterator(modulePath)) {
                if (!entry.is_regular_file()) {
                    continue;
                }
                std::string filePath = entry.path().string();
                std::string fileName = entry.path().filename().string();
                if (!IsSoFile(fileName)) {
                    continue;
                }
                std::string md5 = CalculateFileMD5(filePath, processedSoPaths);
                if (md5.empty()) {
                    SetError("Failed to calculate MD5 for file: " + filePath);
                    return {};
                }

                std::string relativePath = std::filesystem::relative(entry.path(), modulePath).generic_string();
                uintmax_t rawFileSize = std::filesystem::file_size(entry.path());
                if (rawFileSize > static_cast<uintmax_t>(std::numeric_limits<int64_t>::max())) {
                    SetError("SO file size exceeds the supported range: " + filePath);
                    return {};
                }
                SoInfo soInfo;
                soInfo.relativePath = relativePath;
                soInfo.md5 = md5;
                soInfo.sourceModule = context->moduleId;
                soInfo.fileSize = static_cast<int64_t>(rawFileSize);

                soFiles.push_back(soInfo);
                LOG(DEBUG) << "Found SO file: " << relativePath << " (MD5: "
                          << md5.substr(0, MD5_DISPLAY_LENGTH) << "...)";
            }

            if (!soFiles.empty()) {
                moduleSoMap[context->moduleId] = soFiles;
                LOG(DEBUG) << "Module " << context->config.moduleName << " (" << context->moduleId
                           << ") contains " << soFiles.size() << " SO files";
            }
        } catch (const std::exception& e) {
            SetError("Failed to collect SO files from module: " + std::string(e.what()));
            return {};
        }
    }

    size_t totalSoCount = 0;
    for (const auto& [moduleName, soList] : moduleSoMap) {
        totalSoCount += soList.size();
    }

    LOG(DEBUG) << "Collected " << totalSoCount << " SO files from "
              << moduleSoMap.size() << " modules";

    return moduleSoMap;
}

std::vector<DuplicateSoGroup> SODeduplicator::GroupDuplicateSos(
    const std::map<std::string, std::vector<SoInfo>>& moduleSoMap)
{

    std::vector<DuplicateSoGroup> duplicateGroups;
    // Group by module-relative path and MD5.
    // SOs with same content but different install paths cannot replace each other.
    std::map<std::pair<std::string, std::string>, std::vector<SoInfo>> duplicateMap;

    for (const auto& [moduleName, soList] : moduleSoMap) {
        for (const auto& so : soList) {
            duplicateMap[{so.relativePath, so.md5}].push_back(so);
        }
    }

    // Create duplicate SO groups
    for (const auto& [identity, soList] : duplicateMap) {
        if (soList.size() > 1) { // Only process duplicate SOs
            DuplicateSoGroup group;
            group.relativePath = identity.first;
            group.md5 = identity.second;
            group.soList = soList;
            duplicateGroups.push_back(group);

            LOG(DEBUG) << "Found duplicate SO group " << group.relativePath << " with MD5 "
                      << group.md5.substr(0, MD5_DISPLAY_LENGTH)
                      << "... (" << soList.size() << " SOs)";
        }
    }

    LOG(DEBUG) << "Found " << duplicateGroups.size() << " duplicate SO groups";

    return duplicateGroups;
}

bool SODeduplicator::ApplyDedupPlan(
    const DedupPlan& plan,
    const std::vector<ModuleDedupContext>& modules)
{
    LOG(DEBUG) << "Applying deduplication plan";

    std::map<std::string, std::filesystem::path> extractedPaths;
    for (const auto& context : modules) {
        extractedPaths[context.moduleId] = context.extractedPath;
    }

    int32_t removedCount = 0;

    for (const auto& [moduleId, soPaths] : plan.removedSoMap) {
        auto moduleIt = extractedPaths.find(moduleId);
        if (moduleIt == extractedPaths.end()) {
            SetError("Module instance does not exist for deduplication plan: " + moduleId);
            return false;
        }
        for (const auto& soPath : soPaths) {
            std::filesystem::path fullFilePath = moduleIt->second / soPath;

            try {
                if (std::filesystem::exists(fullFilePath)) {
                    removedCount++;
                    LOG(DEBUG) << "SO file scheduled for exclusion: " << fullFilePath.string();
                } else {
                    LOG(WARNING) << "SO file does not exist, cannot exclude: " << fullFilePath.string();
                }
            } catch (const std::exception& e) {
                LOG(ERROR) << FormatDedupError(
                    "Failed to check SO file: " + fullFilePath.string() + " - " + e.what());
                SetError("Failed to check SO file: " + fullFilePath.string());
                return false;
            }
        }
    }

    LOG(DEBUG) << "Successfully applied deduplication plan: scheduled " << removedCount
               << " SO files for exclusion";

    return true;
}

bool SODeduplicator::RepackModuleExcludingSOs(
    const std::string& sourceZip,
    const std::string& targetZip,
    const std::string& moduleId,
    const DedupPlan& plan)
{
    LOG(DEBUG) << "Repacking module instance " << moduleId
              << " from " << sourceZip << " to " << targetZip;

    // Collect SO file paths to exclude for this module
    std::set<std::string> excludedPaths;
    auto it = plan.removedSoMap.find(moduleId);
    if (it != plan.removedSoMap.end()) {
        for (const auto& soPath : it->second) {
            std::string normalized = soPath;
            std::replace(normalized.begin(), normalized.end(), '\\', '/');
            excludedPaths.insert(normalized);
        }
    }

    // Open source ZIP file
    UnzipWrapper srcUnzip;
    std::string srcPath = sourceZip;
    if (srcUnzip.Open(srcPath) != UNZ_OK) {
        SetError("Failed to open source ZIP: " + sourceZip);
        return false;
    }

    // Create target ZIP file
    ZipWrapper destZip;
    std::string targetPath = targetZip;
    if (destZip.Open(targetPath, APPEND_STATUS_CREATE) != ZIP_OK) {
        srcUnzip.Close();
        SetError("Failed to create target ZIP: " + targetZip);
        return false;
    }

    // Traverse all files in source ZIP
    unzFile srcFile = srcUnzip.GetUnzipFile();
    zipFile destFile = destZip.GetZipFile();

    if (unzGoToFirstFile(srcFile) != UNZ_OK) {
        srcUnzip.Close();
        destZip.Close();
        SetError("Failed to iterate source ZIP entries");
        return false;
    }

    int32_t copiedCount = 0;
    int32_t skippedCount = 0;
    bool hasError = false;

    do {
        unz_file_info64 fileInfo;
        if (unzGetCurrentFileInfo64(srcFile, &fileInfo, nullptr, 0,
            nullptr, 0, nullptr, 0) != UNZ_OK) {
            LOG(ERROR) << "Failed to get file info for entry";
            hasError = true;
            break;
        }
        std::vector<char> fileName(fileInfo.size_filename + 1, '\0');
        if (unzGetCurrentFileInfo64(srcFile, &fileInfo, fileName.data(), fileName.size(),
            nullptr, 0, nullptr, 0) != UNZ_OK) {
            LOG(ERROR) << "Failed to get file name for entry";
            hasError = true;
            break;
        }

        std::string entryName(fileName.data(), fileInfo.size_filename);
        // 标准化路径分隔符为统一的格式
        std::replace(entryName.begin(), entryName.end(), '\\', '/');

        // Check if this file should be skipped (removed SO)
        if (excludedPaths.find(entryName) != excludedPaths.end()) {
            LOG(DEBUG) << "Skipping removed SO: " << entryName;
            skippedCount++;
            continue;
        }

        // Use AddRawEntryToZip to directly copy ZIP entries
        if (destZip.AddRawEntryToZip(destFile, srcFile, entryName) != ZIP_ERR_SUCCESS) {
            LOG(ERROR) << "Failed to copy entry: " << entryName;
            hasError = true;
            break;
        }

        copiedCount++;
    } while (unzGoToNextFile(srcFile) == UNZ_OK);

    srcUnzip.Close();
    destZip.Close();

    // 如果有错误，删除不完整的文件
    if (hasError) {
        LOG(ERROR) << "Repacking failed with errors, cleaning up incomplete file";
        try {
            std::filesystem::remove(targetZip);
        } catch (const std::exception& e) {
            LOG(WARNING) << "Failed to remove incomplete file: " << e.what();
        }
        SetError("Failed to repack module: error during copying entries");
        return false;
    }

    LOG(DEBUG) << "Repacking completed successfully: copied=" << copiedCount
              << ", skipped=" << skippedCount;

    return true;
}

bool SODeduplicator::DeduplicateModules(std::list<std::string>& modulePaths,
    bool deduplicateSo, const std::string& workDir, const std::string& reportDir)
{
    return DeduplicateModules(modulePaths, modulePaths, deduplicateSo, workDir, reportDir);
}

bool SODeduplicator::DeduplicateModules(std::list<std::string>& modulePaths,
    const std::list<std::string>& originalModulePaths, bool deduplicateSo,
    const std::string& workDir, const std::string& reportDir)
{
    return DeduplicateModulesInternal(
        modulePaths, originalModulePaths, deduplicateSo, workDir, reportDir, nullptr);
}

bool SODeduplicator::DeduplicateModules(std::list<std::string>& modulePaths,
    const std::list<std::string>& originalModulePaths, bool deduplicateSo,
    const std::string& workDir, const std::string& reportDir,
    std::vector<std::string>& processedSoPaths)
{
    processedSoPaths.clear();
    return DeduplicateModulesInternal(
        modulePaths, originalModulePaths, deduplicateSo, workDir, reportDir, &processedSoPaths);
}

bool SODeduplicator::DeduplicateModulesInternal(std::list<std::string>& modulePaths,
    const std::list<std::string>& originalModulePaths, bool deduplicateSo,
    const std::string& workDir, const std::string& reportDir,
    std::vector<std::string>* processedSoPaths)
{
    if (!modulePaths.empty() && modulePaths.size() != originalModulePaths.size()) {
        SetError("Module path count does not match original module path count");
        return false;
    }
    if (!deduplicateSo) {
        LOG(INFO) << "SO deduplication skipped: disabled.";
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

        std::vector<ModuleDedupContext> modules;
        using ModuleFile = std::tuple<std::string, std::filesystem::path, std::string>;
        std::vector<ModuleFile> moduleFiles;
        size_t index = 0;
        auto originalPathIt = originalModulePaths.begin();
        for (const auto& modulePath : modulePaths) {
            std::filesystem::path source(modulePath);
            std::string sourcePathStr = source.string();
            size_t moduleIndex = index++;
            std::string moduleId = std::to_string(moduleIndex);
            std::filesystem::path extractDir = modulesRoot / moduleId;
            std::filesystem::create_directories(extractDir);
            if (ZipUtils::Unzip(sourcePathStr, extractDir.string()) != ZIP_ERR_SUCCESS) {
                SetError("Failed to unzip module: " + sourcePathStr);
                return false;
            }
            std::filesystem::path jsonPath = extractDir / "module.json";
            bool stageModel = std::filesystem::exists(jsonPath);
            if (!stageModel) {
                jsonPath = extractDir / "config.json";
            }
            auto module = std::make_shared<ModuleJson>();
            if (!std::filesystem::exists(jsonPath) || !module->ParseFromFile(jsonPath.string())) {
                LOG(WARNING) << "SO deduplication skipped for module: failed to parse configuration, module="
                             << sourcePathStr;
                module.reset();
            }
            if (!stageModel) {
                module.reset();
            }
            ModuleConfig config;
            if (module) {
                config = ModuleCalculator::ExtractModuleConfig(module, stageModel);
            }
            modules.push_back({module, config, moduleId, extractDir, *originalPathIt});
            ++originalPathIt;
            std::filesystem::path repackedPath = repackedRoot / std::to_string(moduleIndex) / source.filename();
            std::filesystem::create_directories(repackedPath.parent_path());
            moduleFiles.push_back({sourcePathStr, repackedPath, moduleId});
        }

        std::string effectiveReportDir = reportDir.empty() ? "." : reportDir;
        if (!ExecuteDeduplication(modules, effectiveReportDir, true, processedSoPaths)) {
            return false;
        }

        std::list<std::string> repackedPaths;
        for (const auto& [sourcePath, repackedPath, moduleId] : moduleFiles) {
            // Use optimized repack method: directly copy ZIP entries, excluding removed SOs
            if (!RepackModuleExcludingSOs(sourcePath, repackedPath.string(), moduleId, dedupPlan_)) {
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
        LOG(WARNING) << "SO deduplication completed: removed=" << removedCount
                  << ", savedBytes=" << dedupPlan_.totalSavedSize << ", report=" << reportFilePath_;
        return true;
    } catch (const std::exception& exception) {
        SetError("Exception during deduplication: " + std::string(exception.what()));
        return false;
    }
}

bool SODeduplicator::ExecuteDeduplication(
    const std::vector<ModuleDedupContext>& allModules,
    const std::string& reportOutputPath,
    bool dedupEnabled,
    std::vector<std::string>* processedSoPaths)
{
    LOG(INFO) << "SO deduplication started.";
    LOG(DEBUG) << "Deduplication enabled: " << (dedupEnabled ? "true" : "false");
    LOG(DEBUG) << "Total modules: " << allModules.size();

    // 重置状态
    dedupSuccess_ = false;
    errorMessage_ = "";
    dedupPlan_ = DedupPlan();

    if (!dedupEnabled) {
        LOG(INFO) << "SO deduplication skipped: disabled.";
        dedupSuccess_ = true; // 不启用去重不算失败
        return true;
    }

    if (allModules.empty()) {
        SetError("No modules provided for deduplication");
        return false;
    }

    try {
        // Step 1: Filter entry modules
        std::vector<const ModuleDedupContext*> entryModules = FilterEntryModules(allModules);

        // Step 2: Calculate device set
        DeviceCalculator deviceCalculator;
        std::vector<std::shared_ptr<ModuleJson>> entryModuleJsons;
        for (const auto* context : entryModules) {
            entryModuleJsons.push_back(context->module);
        }
        std::vector<DeviceInstance> devices = deviceCalculator.CalculateDevices(entryModuleJsons);

        if (devices.empty()) {
            LOG(INFO) << "SO deduplication skipped: no valid entry devices.";
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(
                dedupPlan_, DedupStrategy::NONE, reportOutputPath);
            dedupSuccess_ = !reportFilePath_.empty();
            return dedupSuccess_;
        }

        // Step 3: Filter modules for deduplication
        std::vector<const ModuleDedupContext*> eligibleModules = FilterDedupEligibleModules(allModules);
        if (!errorMessage_.empty()) {
            return false;
        }

        if (eligibleModules.empty()) {
            LOG(DEBUG) << "No modules eligible for deduplication";
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, DedupStrategy::NONE, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // Step 4: Calculate mandatory module set
        std::map<DeviceInstance, std::vector<std::string>> mandatoryModuleMap;
        for (const auto& device : devices) {
            auto& mandatoryModules = mandatoryModuleMap[device];
            for (const auto* context : eligibleModules) {
                if (ModuleCalculator::IsMandatoryModule(context->config, device)) {
                    mandatoryModules.push_back(context->moduleId);
                }
            }
        }
        std::map<std::string, std::vector<DeviceInstance>> moduleSupportMap;
        for (const auto* context : eligibleModules) {
            const ModuleConfig& config = context->config;
            for (const auto& device : devices) {
                bool supportsType = std::find(config.deviceTypes.begin(), config.deviceTypes.end(), device.type) !=
                    config.deviceTypes.end();
                bool supportsFilter = config.distributionFilter.empty() ||
                    config.distributionFilter == device.distributionFilter;
                if (supportsType && supportsFilter) {
                    moduleSupportMap[context->moduleId].push_back(device);
                }
            }
        }

        // Step 5: Collect SO file information
        std::map<std::string, std::vector<SoInfo>> moduleSoMap = CollectSoFiles(
            eligibleModules, processedSoPaths);
        if (!errorMessage_.empty()) {
            return false;
        }

        if (moduleSoMap.empty()) {
            LOG(DEBUG) << "No SO files found for deduplication";
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, DedupStrategy::NONE, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // Step 6: Group duplicate SOs
        std::vector<DuplicateSoGroup> duplicateGroups = GroupDuplicateSos(moduleSoMap);

        if (duplicateGroups.empty()) {
            LOG(DEBUG) << "No duplicate SO groups found";
            ReportGenerator reportGenerator;
            reportFilePath_ = reportGenerator.GenerateReport(dedupPlan_, DedupStrategy::NONE, reportOutputPath);
            if (reportFilePath_.empty()) {
                SetError("Failed to generate deduplication report");
                return false;
            }
            dedupSuccess_ = true;
            return true;
        }

        // Step 7: Select algorithm based on SO duplicate count and solve
        std::vector<DuplicateSoGroup> smallGroups;  // Groups with <=20 duplicates
        std::vector<DuplicateSoGroup> largeGroups;  // Groups with >20 duplicates

        for (const auto& group : duplicateGroups) {
            if (ExactSolver::CanUseExactAlgorithm(group.soList.size())) {
                smallGroups.push_back(group);
            } else if (GreedySolver::CanUseGreedyAlgorithm(group.soList.size())) {
                largeGroups.push_back(group);
            }
        }

        LOG(DEBUG) << "Using EXACT algorithm for " << smallGroups.size() << " small groups (size <= 20)";
        LOG(DEBUG) << "Using GREEDY algorithm for " << largeGroups.size() << " large groups (size > 20)";

        // Use exact algorithm for small groups
        DedupPlan smallGroupsPlan;
        if (!smallGroups.empty()) {
            ExactSolver exactSolver;
            smallGroupsPlan = exactSolver.Solve(smallGroups, mandatoryModuleMap, moduleSupportMap);
        }

        // Use greedy algorithm for large groups
        DedupPlan largeGroupsPlan;
        if (!largeGroups.empty()) {
            GreedySolver greedySolver;
            largeGroupsPlan = greedySolver.Solve(largeGroups, mandatoryModuleMap, moduleSupportMap);
        }

        // Merge results
        dedupPlan_.Merge(smallGroupsPlan);
        dedupPlan_.Merge(largeGroupsPlan);
        for (const auto& context : allModules) {
            dedupPlan_.moduleNames[context.moduleId] = context.config.moduleName;
            dedupPlan_.modulePaths[context.moduleId] = context.sourcePath;
        }

        if (dedupPlan_.keptSoMap.empty() && dedupPlan_.removedSoMap.empty()) {
            SetError("Failed to generate deduplication plan");
            return false;
        }

        // Step 8: Apply deduplication plan
        if (!ApplyDedupPlan(dedupPlan_, allModules)) {
            return false;
        }

        // Step 9: Generate deduplication report
        ReportGenerator reportGenerator;
        DedupStrategy actualStrategy = ResolveDedupStrategy(!smallGroups.empty(), !largeGroups.empty());
        std::string generatedReportPath = reportGenerator.GenerateReport(dedupPlan_, actualStrategy, reportOutputPath);
        if (generatedReportPath.empty()) {
            SetError("Failed to generate deduplication report");
            return false;
        }

        reportFilePath_ = generatedReportPath;
        dedupSuccess_ = true;
        return true;
    } catch (const std::exception& e) {
        SetError("Exception during deduplication: " + std::string(e.what()));
        return false;
    }
}

}  // namespace AppPackingTool
}  // namespace OHOS
