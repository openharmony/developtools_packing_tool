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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_MODULE_CALCULATOR_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_MODULE_CALCULATOR_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "json/module_json.h"
#include "dedup/device_calculator.h"

namespace OHOS {
namespace AppPackingTool {

// 模块配置摘要
struct ModuleConfig {
    std::string moduleName;
    std::string moduleType; // "entry", "feature", "shared"
    std::string bundleType; // "app", "atomicService"
    bool stageModel;
    std::vector<DeviceType> deviceTypes;
    bool deviceTypesConfigured;
    std::string distributionFilter;
    bool deliveryWithInstall;
    bool deliveryWithInstallPresent;
    std::vector<std::string> requireDeviceFeatures;
    bool compressNativeLibs;
    bool extractNativeLibs;
    bool libIsolation;
    std::string compileSdkType; // "HarmonyOS" 或 "OpenHarmony"

    ModuleConfig() : stageModel(false), deviceTypesConfigured(false), deliveryWithInstall(false),
                     deliveryWithInstallPresent(false),
                     compressNativeLibs(false),
                     extractNativeLibs(false), libIsolation(false) {}
};

// 模块集合计算器
class ModuleCalculator {
public:
    ModuleCalculator();
    ~ModuleCalculator();

    /**
     * @brief 计算每个设备的必然安装模块集合A(i)
     * @param allModules 所有模块列表（包括entry、feature、shared）
     * @param devices 设备实例列表
     * @return 映射：设备实例 -> 该设备的必然安装模块列表
     */
    std::map<DeviceInstance, std::vector<std::string>> CalculateMandatoryModules(
        const std::vector<std::shared_ptr<ModuleJson>>& allModules,
        const std::vector<DeviceInstance>& devices);

    /**
     * @brief 判断模块是否为必然安装模块
     * @param moduleConfig 模块配置
     * @param device 设备实例
     * @return 是否为必然安装模块
     */
    static bool IsMandatoryModule(const ModuleConfig& moduleConfig, const DeviceInstance& device);

    /**
     * @brief 从ModuleJson提取模块配置
     * @param moduleJson ModuleJson对象
     * @return 模块配置
     */
    static ModuleConfig ExtractModuleConfig(
        const std::shared_ptr<ModuleJson>& moduleJson, bool stageModel = true);

    static bool IsValidForDedup(const ModuleConfig& moduleConfig);

    /**
     * @brief 检查设备的deviceTypes是否包含指定设备类型
     * @param moduleConfig 模块配置
     * @param deviceType 设备类型
     * @return 是否包含
     */
    static bool SupportsDeviceType(const ModuleConfig& moduleConfig, DeviceType deviceType);

    /**
     * @brief 检查模块的distributionFilter是否匹配设备的distributionFilter
     * @param moduleConfig 模块配置
     * @param device 设备实例
     * @return 是否匹配
     */
    static bool MatchesDistributionFilter(const ModuleConfig& moduleConfig, const DeviceInstance& device);

private:
    /**
     * @brief 检查requireDeviceFeatures是否为空
     * @param moduleConfig 模块配置
     * @return 是否为空
     */
    static bool HasEmptyRequireDeviceFeatures(const ModuleConfig& moduleConfig);
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_MODULE_CALCULATOR_H
