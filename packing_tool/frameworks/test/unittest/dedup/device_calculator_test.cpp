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

#include <gtest/gtest.h>
#include "dedup/device_calculator.h"
#include "dedup/module_calculator.h"

using testing::ext::TestSize;

namespace {
OHOS::AppPackingTool::ModuleConfig CreateEntryConfig(
    const std::vector<OHOS::AppPackingTool::DeviceType>& deviceTypes,
    const std::string& distributionFilter = "")
{
    OHOS::AppPackingTool::ModuleConfig config;
    config.deviceTypes = deviceTypes;
    config.distributionFilter = distributionFilter;
    return config;
}
}

class DeviceCalculatorTest : public testing::Test {
protected:
    OHOS::AppPackingTool::DeviceCalculator calculator_;
};

HWTEST_F(DeviceCalculatorTest, StringToDeviceType_ValidInput, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    EXPECT_EQ(calculator_.StringToDeviceType("default"), DeviceType::PHONE);
    EXPECT_EQ(calculator_.StringToDeviceType("phone"), DeviceType::PHONE);
    EXPECT_EQ(calculator_.StringToDeviceType("tablet"), DeviceType::TABLET);
    EXPECT_EQ(calculator_.StringToDeviceType("2in1"), DeviceType::TWOINONE);
    EXPECT_EQ(calculator_.StringToDeviceType("wearable"), DeviceType::WEARABLE);
    EXPECT_EQ(calculator_.StringToDeviceType("tv"), DeviceType::TV);
    EXPECT_EQ(calculator_.StringToDeviceType("car"), DeviceType::CAR);
}

HWTEST_F(DeviceCalculatorTest, DeviceTypeToString_ValidInput, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::PHONE), "phone");
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::TABLET), "tablet");
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::TWOINONE), "2in1");
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::WEARABLE), "wearable");
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::TV), "tv");
    EXPECT_EQ(calculator_.DeviceTypeToString(DeviceType::CAR), "car");
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_SingleModule, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    auto devices = calculator_.CalculateDevices({CreateEntryConfig({DeviceType::PHONE, DeviceType::TABLET})});

    ASSERT_EQ(devices.size(), 2);
    EXPECT_EQ(devices[0].type, DeviceType::PHONE);
    EXPECT_EQ(devices[1].type, DeviceType::TABLET);
    EXPECT_TRUE(devices[0].distributionFilter.empty());
    EXPECT_TRUE(devices[1].distributionFilter.empty());
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_MultipleModules, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    auto devices = calculator_.CalculateDevices({
        CreateEntryConfig({DeviceType::PHONE}),
        CreateEntryConfig({DeviceType::TABLET, DeviceType::TV})
    });

    EXPECT_EQ(devices.size(), 3);
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_EmptyModules, TestSize.Level0)
{
    EXPECT_TRUE(calculator_.CalculateDevices({}).empty());
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_DuplicateRemoval, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    auto devices = calculator_.CalculateDevices({
        CreateEntryConfig({DeviceType::PHONE}), CreateEntryConfig({DeviceType::PHONE})
    });

    ASSERT_EQ(devices.size(), 1);
    EXPECT_EQ(devices[0].type, DeviceType::PHONE);
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_MissingDeviceTypes, TestSize.Level0)
{
    EXPECT_TRUE(calculator_.CalculateDevices({CreateEntryConfig({})}).empty());
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_PreservesDistributionFilter, TestSize.Level0)
{
    using OHOS::AppPackingTool::DeviceType;
    auto devices = calculator_.CalculateDevices({
        CreateEntryConfig({DeviceType::PHONE}, "xldpi"),
        CreateEntryConfig({DeviceType::PHONE}, "xxldpi")
    });

    ASSERT_EQ(devices.size(), 2);
    EXPECT_EQ(devices[0].distributionFilter, "xldpi");
    EXPECT_EQ(devices[1].distributionFilter, "xxldpi");
}
