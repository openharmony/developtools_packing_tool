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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_DEDUPLICATOR_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_DEDUPLICATOR_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <filesystem>
#include "json/module_json.h"
#include "dedup/so_info.h"
#include "dedup/device_calculator.h"
#include "dedup/module_calculator.h"

namespace OHOS {
namespace AppPackingTool {

// 去重算法选择阈值：重复副本数量超过此值时使用贪心算法，否则使用精确算法
constexpr size_t DEDUP_ALGORITHM_THRESHOLD = 20;

// MD5哈希显示相关常量
constexpr size_t MD5_HEX_WIDTH = 2;           // MD5十六进制宽度
constexpr size_t MD5_DISPLAY_LENGTH = 8;      // MD5哈希显示长度（前N个字符）

/**
 * @brief SO去重器
 *
 * 串联去重流程，接收IDE去重开关参数，执行去重操作
 */
class SODeduplicator {
public:
    SODeduplicator();
    ~SODeduplicator();

    bool DeduplicateModules(std::list<std::string>& modulePaths,
                            bool deduplicateSo,
                            const std::string& workDir,
                            const std::string& reportDir);

    bool DeduplicateModules(std::list<std::string>& modulePaths,
                            const std::list<std::string>& originalModulePaths,
                            bool deduplicateSo,
                            const std::string& workDir,
                            const std::string& reportDir);

    /**
     * @brief 获取最终的去重方案
     * @return 去重方案
     */
    DedupPlan GetDedupPlan() const { return dedupPlan_; }

    /**
     * @brief 获取去重是否成功
     * @return 是否成功
     */
    bool IsDedupSuccess() const { return dedupSuccess_; }

    /**
     * @brief 获取错误信息
     * @return 错误信息
     */
    std::string GetErrorMessage() const { return errorMessage_; }

private:
    struct ModuleDedupContext {
        std::shared_ptr<ModuleJson> module;
        ModuleConfig config;
        std::string moduleId;
        std::filesystem::path extractedPath;
        std::string sourcePath;
    };

    /**
     * @brief 过滤entry模块
     * @param allModules 所有模块列表
     * @return entry模块列表
     */
    std::vector<const ModuleDedupContext*> FilterEntryModules(
        const std::vector<ModuleDedupContext>& allModules);

    /**
     * @brief 过滤参与去重的模块
     * @param allModules 所有模块列表
     * @return 参与去重的模块列表
     */
    std::vector<const ModuleDedupContext*> FilterDedupEligibleModules(
        const std::vector<ModuleDedupContext>& allModules);

    /**
     * @brief 收集SO文件信息
     * @param modules 模块实例上下文列表
     * @return SO文件信息列表（按模块实例ID -> SO列表）
     */
    std::map<std::string, std::vector<SoInfo>> CollectSoFiles(
        const std::vector<const ModuleDedupContext*>& modules);

    /**
     * @brief 按MD5分组重复的SO
     * @param moduleSoMap 模块SO映射
     * @return 重复SO组列表
     */
    std::vector<DuplicateSoGroup> GroupDuplicateSos(
        const std::map<std::string, std::vector<SoInfo>>& moduleSoMap);

    /**
     * @brief 计算文件的MD5值
     * @param filePath 文件路径
     * @return MD5值（失败返回空字符串）
     */
    std::string CalculateFileMD5(const std::string& filePath);

    /**
     * @brief 应用去重方案到文件系统
     * @param plan 去重方案
     * @param modules 模块实例上下文列表
     * @return 是否成功应用
     */
    bool ApplyDedupPlan(
        const DedupPlan& plan,
        const std::vector<ModuleDedupContext>& modules);

    /**
     * @brief 重新打包模块，排除指定的SO文件
     * @param sourceZip 源ZIP文件路径
     * @param targetZip 目标ZIP文件路径
     * @param moduleName 模块名
     * @param plan 去重方案
     * @return 是否成功
     */
    bool RepackModuleExcludingSOs(
        const std::string& sourceZip,
        const std::string& targetZip,
        const std::string& moduleId,
        const DedupPlan& plan);

    bool ExecuteDeduplication(
        const std::vector<ModuleDedupContext>& allModules,
        const std::string& reportOutputPath,
        bool dedupEnabled);

    /**
     * @brief 设置错误信息
     * @param error 错误信息
     */
    void SetError(const std::string& error);

    /**
     * @brief 判断文件是否为.so文件（包括版本号文件如.so.1）
     * @param fileName 文件名
     * @return 是否为.so文件
     */
    static bool IsSoFile(const std::string& fileName);

private:
    DedupPlan dedupPlan_;
    bool dedupSuccess_;
    std::string errorMessage_;
    std::string reportFilePath_;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_DEDUPLICATOR_H
