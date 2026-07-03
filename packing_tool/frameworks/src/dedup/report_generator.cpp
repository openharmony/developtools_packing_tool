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
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"

namespace OHOS {
namespace AppPackingTool {

ReportGenerator::ReportGenerator() {}
ReportGenerator::~ReportGenerator() {}

std::string ReportGenerator::GetCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t_now), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

std::string ReportGenerator::StrategyToString(DedupStrategy strategy) const {
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

std::string ReportGenerator::GenerateReportFileName() const {
    return "so_dedup_report.json";
}

std::string ReportGenerator::GenerateReportJson(const DedupPlan& plan) {
    std::stringstream json;

    json << "{\n";
    json << "  \"timestamp\": \"" << GetCurrentTimestamp() << "\",\n";
    json << "  \"totalSavedSize\": " << plan.totalSavedSize << ",\n";
    json << "  \"modules\": {\n";

    // 构建模块去重信息
    std::map<std::string, DedupReport::ModuleDedupInfo> moduleDedupMap;

    // 添加保留的SO信息
    for (const auto& [moduleName, soPaths] : plan.keptSoMap) {
        auto& dedupInfo = moduleDedupMap[moduleName];
        dedupInfo.kept = soPaths;
    }

    // 添加移除的SO信息
    for (const auto& [moduleName, soPaths] : plan.removedSoMap) {
        auto& dedupInfo = moduleDedupMap[moduleName];
        dedupInfo.removed = soPaths;
    }

    // 生成JSON
    bool firstModule = true;
    for (const auto& [moduleName, dedupInfo] : moduleDedupMap) {
        if (!firstModule) {
            json << ",\n";
        }
        firstModule = false;

        json << "    \"" << moduleName << "\": {\n";
        json << "      \"kept\": [";

        for (size_t i = 0; i < dedupInfo.kept.size(); ++i) {
            if (i > 0) {
                json << ", ";
            }
            json << "\"" << dedupInfo.kept[i] << "\"";
        }

        json << "],\n";
        json << "      \"removed\": [";

        for (size_t i = 0; i < dedupInfo.removed.size(); ++i) {
            if (i > 0) {
                json << ", ";
            }
            json << "\"" << dedupInfo.removed[i] << "\"";
        }

        json << "]\n";
        json << "    }";
    }

    json << "\n  }\n";
    json << "}\n";

    return json.str();
}

std::string ReportGenerator::GenerateReport(
    const DedupPlan& plan, DedupStrategy strategy, const std::string& outputPath) {
    if (outputPath.empty()) {
        LOG(ERROR) << FormatDedupError("Output path is empty");
        return "";
    }

    LOG(DEBUG) << "Generating SO deduplication report in directory: " << outputPath;

    // 生成JSON内容
    std::string jsonContent = GenerateReportJson(plan);

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
