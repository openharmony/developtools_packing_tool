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

#include "dedup/device_calculator.h"
#include <algorithm>
#include "dedup/dedup_log.h"
#include "dedup/dedup_error.h"
#include "json/distro_filter.h"

namespace OHOS {
namespace AppPackingTool {

DeviceCalculator::DeviceCalculator()
{}
DeviceCalculator::~DeviceCalculator()
{}

DeviceType DeviceCalculator::StringToDeviceType(const std::string& deviceTypeStr)
{
    if (deviceTypeStr == "phone") {
        return DeviceType::PHONE;
    } else if (deviceTypeStr == "tablet") {
        return DeviceType::TABLET;
    } else if (deviceTypeStr == "2in1") {
        return DeviceType::TWOINONE;
    } else if (deviceTypeStr == "wearable") {
        return DeviceType::WEARABLE;
    } else if (deviceTypeStr == "tv") {
        return DeviceType::TV;
    } else if (deviceTypeStr == "car") {
        return DeviceType::CAR;
    } else {
        // Default to phone
        return DeviceType::PHONE;
    }
}

std::string DeviceCalculator::DeviceTypeToString(DeviceType deviceType)
{
    switch (deviceType) {
        case DeviceType::PHONE:
            return "phone";
        case DeviceType::TABLET:
            return "tablet";
        case DeviceType::TWOINONE:
            return "2in1";
        case DeviceType::WEARABLE:
            return "wearable";
        case DeviceType::TV:
            return "tv";
        case DeviceType::CAR:
            return "car";
        default:
            return "phone";
    }
}

std::vector<DeviceInstance> DeviceCalculator::ExtractDevicesFromModule(
    const std::shared_ptr<ModuleJson>& moduleJson)
{
    std::vector<DeviceInstance> devices;

    if (!moduleJson) {
        return devices;
    }

    try {
        // Get device type list
        std::list<std::string> deviceTypeStrings;
        bool isStageModel = moduleJson->GetStageDeviceTypes(deviceTypeStrings);
        if (!isStageModel) {
            return devices;
        }
        std::list<std::string> requiredDeviceTypes;
        if (moduleJson->GetStageRequiredDeviceFeatureTypes(requiredDeviceTypes)) {
            deviceTypeStrings.insert(deviceTypeStrings.end(), requiredDeviceTypes.begin(), requiredDeviceTypes.end());
        }

        // Get distributionFilter
        DistroFilter distroFilter;
        std::map<std::string, std::string> resourceMap; // Empty resource mapping
        bool hasDistroFilter = moduleJson->GetStageDistroFilter(distroFilter, resourceMap);

        // Create device instance for each device type
        for (const auto& deviceTypeStr : deviceTypeStrings) {
            DeviceInstance device;
            device.type = StringToDeviceType(deviceTypeStr);

            // If has distributionFilter, add to device instance
            if (hasDistroFilter && !distroFilter.IsEmpty()) {
                device.distributionFilter = distroFilter.Dump();
            } else {
                device.distributionFilter = ""; // No distributionFilter
            }

            devices.push_back(device);
        }
    } catch (const std::exception& e) {
        LOG(ERROR) << FormatDedupError("Extract devices from module failed: " + std::string(e.what()));
    }

    return devices;
}

std::vector<DeviceInstance> DeviceCalculator::MergeDevices(
    const std::vector<DeviceInstance>& devices1,
    const std::vector<DeviceInstance>& devices2)
{
    std::vector<DeviceInstance> merged = devices1;

    for (const auto& device : devices2) {
        // 检查是否已存在
        bool found = false;
        for (const auto& existing : merged) {
            if (existing == device) {
                found = true;
                break;
            }
        }

        if (!found) {
            merged.push_back(device);
        }
    }

    return merged;
}

std::vector<DeviceInstance> DeviceCalculator::CalculateDevices(
    const std::vector<std::shared_ptr<ModuleJson>>& entryModules)
{
    std::vector<DeviceInstance> allDevices;

    if (entryModules.empty()) {
        LOG(WARNING) << "No entry modules provided for device calculation";
        return allDevices;
    }

    LOG(DEBUG) << "Calculating devices from " << entryModules.size() << " entry modules";

    // Traverse all entry modules, collect device instances
    for (const auto& entryModule : entryModules) {
        if (!entryModule) {
            continue;
        }

        std::vector<DeviceInstance> moduleDevices = ExtractDevicesFromModule(entryModule);

        if (moduleDevices.empty()) {
            LOG(WARNING) << "No devices found in entry module";
            continue;
        }

        // Merge device instances
        allDevices = MergeDevices(allDevices, moduleDevices);
    }

    LOG(DEBUG) << "Calculated " << allDevices.size() << " unique device instances";

    // Output device list for debugging
    for (const auto& device : allDevices) {
        std::string deviceStr = DeviceTypeToString(device.type);
        if (!device.distributionFilter.empty()) {
            deviceStr += " (distributionFilter: " + device.distributionFilter + ")";
        }
        LOG(DEBUG) << "  - " << deviceStr;
    }

    return allDevices;
}
}  // namespace AppPackingTool
}  // namespace OHOS
