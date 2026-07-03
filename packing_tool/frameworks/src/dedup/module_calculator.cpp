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

#include "dedup/module_calculator.h"
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"
#include "json/distro_filter.h"
#include <algorithm>

namespace OHOS {
namespace AppPackingTool {

ModuleCalculator::ModuleCalculator() {}
ModuleCalculator::~ModuleCalculator() {}

ModuleConfig ModuleCalculator::ExtractModuleConfig(
    const std::shared_ptr<ModuleJson>& moduleJson, bool stageModel) {
    ModuleConfig config;

    if (!moduleJson) {
        LOG(ERROR) << FormatDedupError("ModuleJson is null");
        return config;
    }
    if (!stageModel) {
        return config;
    }

    try {
        std::unique_ptr<PtJson> moduleObj;
        if (!moduleJson->GetModuleObject(moduleObj) || !moduleObj) {
            return config;
        }
        config.stageModel = true;
        // 获取模块名
        std::string moduleName;
        if (moduleJson->GetStageModuleName(moduleName)) {
            config.moduleName = moduleName;
        }

        // 获取模块类型
        std::string moduleType;
        if (moduleJson->GetStageModuleType(moduleType)) {
            config.moduleType = moduleType;
        }

        // 获取设备类型列表
        std::list<std::string> deviceTypeStrings;
        if (moduleJson->GetStageDeviceTypes(deviceTypeStrings)) {
            for (const auto& deviceTypeStr : deviceTypeStrings) {
                config.deviceTypes.push_back(DeviceCalculator::StringToDeviceType(deviceTypeStr));
            }
            // 移除过早判断，在处理完requiredDeviceFeatures后再判断
        }

        // 获取distributionFilter
        DistroFilter distroFilter;
        std::map<std::string, std::string> resourceMap;
        if (moduleJson->GetStageDistroFilter(distroFilter, resourceMap)) {
            config.distributionFilter = distroFilter.Dump();
        }

        // 获取deliveryWithInstall
        bool deliveryWithInstall = false;
        config.deliveryWithInstallPresent = moduleObj->Contains("deliveryWithInstall");
        if (moduleJson->GetDeliveryWithInstall(deliveryWithInstall)) {
            config.deliveryWithInstall = deliveryWithInstall;
        }

        // 获取compressNativeLibs
        bool compressNativeLibs = false;
        if (moduleJson->GetStageCompressNativeLibs(compressNativeLibs)) {
            config.compressNativeLibs = compressNativeLibs;
        }

        // 获取extractNativeLibs
        bool extractNativeLibs = false;
        if (moduleJson->GetStageExtractNativeLibs(extractNativeLibs)) {
            config.extractNativeLibs = extractNativeLibs;
        }

        bool libIsolation = false;
        if (moduleJson->GetStageLibIsolation(libIsolation)) {
            config.libIsolation = libIsolation;
        }

        bool hasRequiredDeviceFeatures = false;
        if (moduleJson->GetStageHasRequiredDeviceFeatures(hasRequiredDeviceFeatures) &&
            hasRequiredDeviceFeatures) {
            std::list<std::string> requiredDeviceTypes;
            moduleJson->GetStageRequiredDeviceFeatureTypes(requiredDeviceTypes);
            for (const auto& deviceType : requiredDeviceTypes) {
                DeviceType normalized = DeviceCalculator::StringToDeviceType(deviceType);
                config.requireDeviceFeatures.push_back(deviceType);
                if (std::find(config.deviceTypes.begin(), config.deviceTypes.end(), normalized) ==
                    config.deviceTypes.end()) {
                    config.deviceTypes.push_back(normalized);
                }
            }
        }

        // 在处理完所有设备类型（包括从requiredDeviceFeatures添加的）之后判断
        config.deviceTypesConfigured = !config.deviceTypes.empty();

        // 获取compileSdkType
        std::string compileSdkType;
        if (moduleJson->GetStageCompileSdkType(compileSdkType)) {
            config.compileSdkType = compileSdkType;
        }

        // 获取bundleType
        std::string bundleType;
        if (moduleJson->GetStageBundleType(bundleType)) {
            config.bundleType = bundleType;
        }
    } catch (const std::exception& e) {
        LOG(ERROR) << FormatDedupError("Extract module config failed: " + std::string(e.what()));
    }

    return config;
}

bool ModuleCalculator::IsValidForDedup(const ModuleConfig& moduleConfig)
{
    bool supportedType = moduleConfig.bundleType == "app" || moduleConfig.bundleType == "atomicService";
    return moduleConfig.stageModel && !moduleConfig.moduleName.empty() && supportedType &&
        moduleConfig.deviceTypesConfigured && moduleConfig.deliveryWithInstallPresent;
}

bool ModuleCalculator::SupportsDeviceType(const ModuleConfig& moduleConfig, DeviceType deviceType) {
    for (const auto& supportedType : moduleConfig.deviceTypes) {
        if (supportedType == deviceType) {
            return true;
        }
    }
    return false;
}

bool ModuleCalculator::MatchesDistributionFilter(const ModuleConfig& moduleConfig,
                                                  const DeviceInstance& device) {
    // 如果模块的distributionFilter为空，则匹配所有设备
    if (moduleConfig.distributionFilter.empty()) {
        return true;
    }

    // 如果设备的distributionFilter为空，则不匹配有distributionFilter的模块
    if (device.distributionFilter.empty()) {
        return false;
    }

    // 比较distributionFilter
    return moduleConfig.distributionFilter == device.distributionFilter;
}

bool ModuleCalculator::HasEmptyRequireDeviceFeatures(const ModuleConfig& moduleConfig) {
    return moduleConfig.requireDeviceFeatures.empty();
}

bool ModuleCalculator::IsMandatoryModule(const ModuleConfig& moduleConfig, const DeviceInstance& device) {
    // 必然安装模块判断条件：
    // 1. deviceTypes 包含设备
    // 2. deliveryWithInstall = true
    // 3. distributionFilter 为空
    // 4. requireDeviceFeatures 为空

    if (!SupportsDeviceType(moduleConfig, device.type)) {
        return false;
    }

    if (moduleConfig.moduleType == "entry") {
        return MatchesDistributionFilter(moduleConfig, device);
    }
    return moduleConfig.deliveryWithInstall && MatchesDistributionFilter(moduleConfig, device) &&
        HasEmptyRequireDeviceFeatures(moduleConfig);
}

std::map<DeviceInstance, std::vector<std::string>> ModuleCalculator::CalculateMandatoryModules(
    const std::vector<std::shared_ptr<ModuleJson>>& allModules,
    const std::vector<DeviceInstance>& devices) {

    std::map<DeviceInstance, std::vector<std::string>> mandatoryModuleMap;

    if (allModules.empty() || devices.empty()) {
        LOG(WARNING) << "No modules or devices provided for mandatory module calculation";
        return mandatoryModuleMap;
    }

    LOG(DEBUG) << "Calculating mandatory modules for " << devices.size() << " devices from "
              << allModules.size() << " modules";

    // 为每个设备计算必然安装模块集合
    for (const auto& device : devices) {
        std::vector<std::string> mandatoryModules;

        for (const auto& moduleJson : allModules) {
            if (!moduleJson) {
                continue;
            }

            // 提取模块配置
            ModuleConfig config = ExtractModuleConfig(moduleJson);

            // 判断是否为必然安装模块
            if (IsMandatoryModule(config, device)) {
                mandatoryModules.push_back(config.moduleName);
            }
        }

        mandatoryModuleMap[device] = mandatoryModules;
        LOG(DEBUG) << "Device " << DeviceCalculator::DeviceTypeToString(device.type)
                  << " has " << mandatoryModules.size() << " mandatory modules";
    }

    return mandatoryModuleMap;
}

}  // namespace AppPackingTool
}  // namespace OHOS
