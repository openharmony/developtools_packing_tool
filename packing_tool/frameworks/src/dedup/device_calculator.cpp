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
#include "dedup/module_calculator.h"

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

std::vector<DeviceInstance> DeviceCalculator::CalculateDevices(
    const std::vector<ModuleConfig>& entryModules)
{
    std::vector<DeviceInstance> allDevices;

    if (entryModules.empty()) {
        LOG(WARNING) << "No entry modules provided for device calculation";
        return allDevices;
    }
    LOG(DEBUG) << "Calculating devices from " << entryModules.size() << " entry modules";

    // Traverse all entry modules, collect device instances
    for (const auto& entryModule : entryModules) {
        for (DeviceType deviceType : entryModule.deviceTypes) {
            DeviceInstance device = {deviceType, entryModule.distributionFilter};
            if (std::find(allDevices.begin(), allDevices.end(), device) == allDevices.end()) {
                allDevices.push_back(device);
            }
        }
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
