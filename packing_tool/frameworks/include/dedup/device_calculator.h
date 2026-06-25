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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEVICE_CALCULATOR_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEVICE_CALCULATOR_H

#include <string>
#include <vector>
#include <memory>
#include "json/module_json.h"

namespace OHOS {
namespace AppPackingTool {

// 设备类型枚举
enum class DeviceType {
    PHONE,
    TABLET,
    TWOINONE,    // 2in1
    WEARABLE,
    TV,
    CAR
};

// 设备实例结构（考虑distributionFilter）
struct DeviceInstance {
    DeviceType type;
    std::string distributionFilter; // 可选的distributionFilter标识

    bool operator==(const DeviceInstance& other) const {
        return type == other.type && distributionFilter == other.distributionFilter;
    }

    bool operator<(const DeviceInstance& other) const {
        if (type != other.type) {
            return type < other.type;
        }
        return distributionFilter < other.distributionFilter;
    }
};

// 设备集合计算器
class DeviceCalculator {
public:
    DeviceCalculator();
    ~DeviceCalculator();

    /**
     * @brief 计算设备集合
     * @param entryModules entry模块列表
     * @return 设备实例列表
     */
    std::vector<DeviceInstance> CalculateDevices(const std::vector<std::shared_ptr<ModuleJson>>& entryModules);

    /**
     * @brief 将设备类型字符串转换为枚举
     * @param deviceTypeStr 设备类型字符串（如"phone", "tablet"等）
     * @return 设备类型枚举
     */
    static DeviceType StringToDeviceType(const std::string& deviceTypeStr);

    /**
     * @brief 将设备类型枚举转换为字符串
     * @param deviceType 设备类型枚举
     * @return 设备类型字符串
     */
    static std::string DeviceTypeToString(DeviceType deviceType);

private:
    /**
     * @brief 从单个entry模块提取设备实例
     * @param moduleJson entry模块的ModuleJson对象
     * @return 该模块支持的设备实例列表
     */
    std::vector<DeviceInstance> ExtractDevicesFromModule(const std::shared_ptr<ModuleJson>& moduleJson);

    /**
     * @brief 合并设备实例列表，去重
     * @param devices1 设备实例列表1
     * @param devices2 设备实例列表2
     * @return 合并后的设备实例列表
     */
    std::vector<DeviceInstance> MergeDevices(const std::vector<DeviceInstance>& devices1,
                                            const std::vector<DeviceInstance>& devices2);
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEVICE_CALCULATOR_H
