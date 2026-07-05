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

ModuleCalculator::ModuleCalculator()
{}
ModuleCalculator::~ModuleCalculator()
{}

ModuleConfig ModuleCalculator::ExtractModuleConfig(
    const std::shared_ptr<ModuleJson>& moduleJson, bool stageModel)
{
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
        // Get module name
        std::string moduleName;
        if (moduleJson->GetStageModuleName(moduleName)) {
            config.moduleName = moduleName;
        }

        // Get module type
        std::string moduleType;
        if (moduleJson->GetStageModuleType(moduleType)) {
            config.moduleType = moduleType;
        }

        // Get device type list
        std::list<std::string> deviceTypeStrings;
        if (moduleJson->GetStageDeviceTypes(deviceTypeStrings)) {
            for (const auto& deviceTypeStr : deviceTypeStrings) {
                config.deviceTypes.push_back(DeviceCalculator::StringToDeviceType(deviceTypeStr));
            }
            // Remove premature judgment, judge after processing requiredDeviceFeatures
        }

        // Get distributionFilter
        DistroFilter distroFilter;
        std::map<std::string, std::string> resourceMap;
        if (moduleJson->GetStageDistroFilter(distroFilter, resourceMap)) {
            config.distributionFilter = distroFilter.Dump();
        }

        // Get deliveryWithInstall
        bool deliveryWithInstall = false;
        config.deliveryWithInstallPresent = moduleObj->Contains("deliveryWithInstall");
        if (moduleJson->GetDeliveryWithInstall(deliveryWithInstall)) {
            config.deliveryWithInstall = deliveryWithInstall;
        }

        // Get compressNativeLibs
        bool compressNativeLibs = false;
        if (moduleJson->GetStageCompressNativeLibs(compressNativeLibs)) {
            config.compressNativeLibs = compressNativeLibs;
        }

        // Get extractNativeLibs
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

        // Judge after processing all device types (including those added from requiredDeviceFeatures)
        config.deviceTypesConfigured = !config.deviceTypes.empty();

        // Get compileSdkType
        std::string compileSdkType;
        if (moduleJson->GetStageCompileSdkType(compileSdkType)) {
            config.compileSdkType = compileSdkType;
        }

        // Get bundleType
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

bool ModuleCalculator::SupportsDeviceType(const ModuleConfig& moduleConfig, DeviceType deviceType)
{
    for (const auto& supportedType : moduleConfig.deviceTypes) {
        if (supportedType == deviceType) {
            return true;
        }
    }
    return false;
}

bool ModuleCalculator::MatchesDistributionFilter(const ModuleConfig& moduleConfig,
                                                 const DeviceInstance& device)
{
    // If module's distributionFilter is empty, match all devices
    if (moduleConfig.distributionFilter.empty()) {
        return true;
    }

    // If device's distributionFilter is empty, don't match modules with distributionFilter
    if (device.distributionFilter.empty()) {
        return false;
    }

    // Compare distributionFilter
    return moduleConfig.distributionFilter == device.distributionFilter;
}

bool ModuleCalculator::HasEmptyRequireDeviceFeatures(const ModuleConfig& moduleConfig)
{
    return moduleConfig.requireDeviceFeatures.empty();
}

bool ModuleCalculator::IsMandatoryModule(const ModuleConfig& moduleConfig,
                                         const DeviceInstance& device)
{
    // Mandatory module judgment conditions:
    // 1. deviceTypes contains device
    // 2. deliveryWithInstall = true
    // 3. distributionFilter is empty
    // 4. requireDeviceFeatures is empty

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
    const std::vector<DeviceInstance>& devices)
{
    std::map<DeviceInstance, std::vector<std::string>> mandatoryModuleMap;

    if (allModules.empty() || devices.empty()) {
        LOG(WARNING) << "No modules or devices provided for mandatory module calculation";
        return mandatoryModuleMap;
    }

    LOG(DEBUG) << "Calculating mandatory modules for " << devices.size() << " devices from "
              << allModules.size() << " modules";

    // Calculate mandatory module set for each device
    for (const auto& device : devices) {
        std::vector<std::string> mandatoryModules;

        for (const auto& moduleJson : allModules) {
            if (!moduleJson) {
                continue;
            }

            // Extract module configuration
            ModuleConfig config = ExtractModuleConfig(moduleJson);
            // Check if module is mandatory
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
