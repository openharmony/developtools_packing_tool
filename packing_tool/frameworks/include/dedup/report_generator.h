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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_REPORT_GENERATOR_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_REPORT_GENERATOR_H

#include <string>
#include <map>
#include <vector>
#include "dedup/so_info.h"

namespace OHOS {
namespace AppPackingTool {

// 去重报告数据结构
struct DedupReport {
    // 模块去重信息
    struct ModuleDedupInfo {
        std::vector<std::string> kept;     // 保留的so路径
        std::vector<std::string> removed;  // 移除的so路径
    };

    std::string version;              // 报告格式版本
    std::string timestamp;            // 生成时间
    std::string strategy;             // 策略类型（"exact" 或 "greedy"）
    std::map<std::string, ModuleDedupInfo> modules; // 模块去重信息
};

/**
 * @brief 去重报告生成器
 *
 * 生成JSON格式的去重报告，输出到output目录
 */
class ReportGenerator {
public:
    ReportGenerator();
    ~ReportGenerator();

    /**
     * @brief 生成去重报告
     * @param plan 去重方案
     * @param strategy 使用的策略
     * @param outputPath 输出目录路径
     * @return 生成的报告文件路径
     */
    std::string GenerateReport(const DedupPlan& plan, DedupStrategy strategy, const std::string& outputPath);

    /**
     * @brief 生成报告的JSON字符串
     * @param plan 去重方案
     * @param strategy 使用的策略
     * @return JSON字符串
     */
    std::string GenerateReportJson(const DedupPlan& plan, DedupStrategy strategy);

private:
    /**
     * @brief 获取当前时间戳字符串
     * @return ISO 8601格式的时间戳
     */
    std::string GetCurrentTimestamp() const;

    /**
     * @brief 将策略枚举转换为字符串
     * @param strategy 策略枚举
     * @return 策略字符串
     */
    std::string StrategyToString(DedupStrategy strategy) const;

    /**
     * @brief 生成报告文件名
     * @return 报告文件名
     */
    std::string GenerateReportFileName() const;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_REPORT_GENERATOR_H
