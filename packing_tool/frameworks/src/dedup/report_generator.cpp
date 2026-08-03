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

#include "dedup/report_generator.h"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "cJSON.h"
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"

namespace OHOS {
namespace AppPackingTool {
namespace {

bool AddStringArray(cJSON* object, const char* key, const std::vector<std::string>& values)
{
    if (object == nullptr || key == nullptr) {
        return false;
    }
    cJSON* array = cJSON_CreateArray();
    if (array == nullptr) {
        return false;
    }
    for (const auto& value : values) {
        cJSON* item = cJSON_CreateString(value.c_str());
        if (item == nullptr || cJSON_AddItemToArray(array, item) == 0) {
            cJSON_Delete(item);
            cJSON_Delete(array);
            return false;
        }
    }
    if (cJSON_AddItemToObject(object, key, array) == 0) {
        cJSON_Delete(array);
        return false;
    }
    return true;
}

}  // namespace

ReportGenerator::ReportGenerator()
{}
ReportGenerator::~ReportGenerator()
{}

std::string ReportGenerator::GetCurrentTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t_now), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

std::string ReportGenerator::StrategyToString(DedupStrategy strategy) const
{
    switch (strategy) {
        case DedupStrategy::NONE:
            return "none";
        case DedupStrategy::EXACT:
            return "exact";
        case DedupStrategy::GREEDY:
            return "greedy";
        case DedupStrategy::MIXED:
            return "mixed";
        default:
            return "unknown";
    }
}

std::string ReportGenerator::GenerateReportFileName() const
{
    return "so_dedup_report.json";
}

std::string ReportGenerator::GenerateReportJson(const DedupPlan& plan)
{
    cJSON* report = cJSON_CreateObject();
    cJSON* modules = cJSON_CreateObject();
    if (report == nullptr || modules == nullptr ||
        cJSON_AddStringToObject(report, "timestamp", GetCurrentTimestamp().c_str()) == nullptr ||
        cJSON_AddNumberToObject(report, "totalSavedSize", plan.totalSavedSize) == nullptr) {
        cJSON_Delete(report);
        cJSON_Delete(modules);
        return "";
    }

    // Build module deduplication information
    std::map<std::string, DedupReport::ModuleDedupInfo> moduleDedupMap;

    auto fillModuleMetadata = [&plan](const std::string& moduleId, DedupReport::ModuleDedupInfo& dedupInfo) {
        auto nameIt = plan.moduleNames.find(moduleId);
        dedupInfo.moduleName = nameIt == plan.moduleNames.end() ? moduleId : nameIt->second;
        auto pathIt = plan.modulePaths.find(moduleId);
        dedupInfo.filePath = pathIt == plan.modulePaths.end() ? "" : pathIt->second;
    };

    // Add retained SO information
    for (const auto& [moduleId, soPaths] : plan.keptSoMap) {
        auto& dedupInfo = moduleDedupMap[moduleId];
        fillModuleMetadata(moduleId, dedupInfo);
        dedupInfo.kept.insert(dedupInfo.kept.end(), soPaths.begin(), soPaths.end());
    }

    // Add removed SO information
    for (const auto& [moduleId, soPaths] : plan.removedSoMap) {
        auto& dedupInfo = moduleDedupMap[moduleId];
        fillModuleMetadata(moduleId, dedupInfo);
        dedupInfo.removed.insert(dedupInfo.removed.end(), soPaths.begin(), soPaths.end());
    }

    for (const auto& [moduleId, dedupInfo] : moduleDedupMap) {
        cJSON* module = cJSON_CreateObject();
        if (module == nullptr ||
            cJSON_AddStringToObject(module, "moduleName", dedupInfo.moduleName.c_str()) == nullptr ||
            cJSON_AddStringToObject(module, "filePath", dedupInfo.filePath.c_str()) == nullptr ||
            !AddStringArray(module, "kept", dedupInfo.kept) ||
            !AddStringArray(module, "removed", dedupInfo.removed) ||
            cJSON_AddItemToObject(modules, moduleId.c_str(), module) == 0) {
            cJSON_Delete(module);
            cJSON_Delete(modules);
            cJSON_Delete(report);
            return "";
        }
    }

    if (cJSON_AddItemToObject(report, "modules", modules) == 0) {
        cJSON_Delete(modules);
        cJSON_Delete(report);
        return "";
    }

    char* jsonString = cJSON_PrintUnformatted(report);
    std::string json = jsonString == nullptr ? "" : std::string(jsonString);
    cJSON_free(jsonString);
    cJSON_Delete(report);
    return json;
}

std::string ReportGenerator::GenerateReport(
    const DedupPlan& plan, DedupStrategy strategy, const std::string& outputPath)
{
    if (outputPath.empty()) {
        LOG(ERROR) << FormatDedupError("Output path is empty");
        return "";
    }

    LOG(DEBUG) << "Generating SO deduplication report in directory: " << outputPath;

    // Generate JSON content
    std::string jsonContent = GenerateReportJson(plan);
    if (jsonContent.empty()) {
        LOG(ERROR) << FormatDedupError("Failed to generate report JSON content");
        return "";
    }

    // 生成文件路径
    std::string reportFileName = GenerateReportFileName();
    std::string reportFilePath = outputPath + "/" + reportFileName;

    // 确保输出目录存在
    std::filesystem::path outputDir(outputPath);
    if (!std::filesystem::exists(outputDir)) {
        try {
            std::filesystem::create_directories(outputDir);
        } catch (const std::exception& e) {
            LOG(ERROR) << FormatDedupError("Failed to create output directory: " + std::string(e.what()));
            return "";
        }
    }

    // 写入文件
    try {
        std::ofstream outFile(reportFilePath);
        if (!outFile.is_open()) {
            LOG(ERROR) << FormatDedupError("Failed to open report file for writing: " + reportFilePath);
            return "";
        }

        outFile << jsonContent;
        outFile.close();

        LOG(DEBUG) << "SO deduplication report generated successfully: " << reportFilePath;
        LOG(DEBUG) << "  - Strategy: " << StrategyToString(strategy);
        LOG(DEBUG) << "  - Total saved size: " << plan.totalSavedSize << " bytes";
        LOG(DEBUG) << "  - Modules with kept SOs: " << plan.keptSoMap.size();
        LOG(DEBUG) << "  - Modules with removed SOs: " << plan.removedSoMap.size();

        return reportFilePath;
    } catch (const std::exception& e) {
        LOG(ERROR) << FormatDedupError("Failed to write report file: " + std::string(e.what()));
        return "";
    }
}
}  // namespace AppPackingTool
}  // namespace OHOS
