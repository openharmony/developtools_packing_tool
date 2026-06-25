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

#include <gtest/gtest/gtest.h>
#include <memory>
#include "dedup/device_calculator.h"
#include "json/module_json.h"

// 测试辅助函数：创建模拟的entry模块
std::shared_ptr<OHOS::AppPackingTool::ModuleJson> CreateMockEntryModule(
    const std::vector<std::string>& deviceTypes,
    const std::string& distributionFilter = "") {

    // 创建包含deviceTypes的module.json字符串
    std::string jsonStr = R"({
        "app": {
            "bundleName": "com.example.app"
        },
        "module": {
            "name": "entry",
            "type": "entry",
            "deviceTypes": [)";

    // 添加设备类型
    for (size_t i = 0; i < deviceTypes.size(); ++i) {
        jsonStr += "\"" + deviceTypes[i] + "\"";
        if (i < deviceTypes.size() - 1) {
            jsonStr += ",";
        }
    }

    jsonStr += R"(]
        }
    })";

    auto moduleJson = std::make_shared<OHOS::AppPackingTool::ModuleJson>();
    if (moduleJson->ParseFromString(jsonStr)) {
        return moduleJson;
    }

    return nullptr;
}

class DeviceCalculatorTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::DeviceCalculator calculator_;
};

void DeviceCalculatorTest::SetUp() {}
void DeviceCalculatorTest::TearDown() {}

// 测试1：设备类型字符串转枚举
HWTEST_F(DeviceCalculatorTest, StringToDeviceType_ValidInput, TestSize.Level0) {
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("default"),
              OHOS::AppPackingTool::DeviceType::PHONE);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("phone"),
              OHOS::AppPackingTool::DeviceType::PHONE);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("tablet"),
              OHOS::AppPackingTool::DeviceType::TABLET);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("2in1"),
              OHOS::AppPackingTool::DeviceType::TWOINONE);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("wearable"),
              OHOS::AppPackingTool::DeviceType::WEARABLE);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("tv"),
              OHOS::AppPackingTool::DeviceType::TV);
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::StringToDeviceType("car"),
              OHOS::AppPackingTool::DeviceType::CAR);
}

// 测试2：设备类型枚举转字符串
HWTEST_F(DeviceCalculatorTest, DeviceTypeToString_ValidInput, TestSize.Level0) {
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::PHONE), "phone");
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::TABLET), "tablet");
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::TWOINONE), "2in1");
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::WEARABLE), "wearable");
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::TV), "tv");
    EXPECT_EQ(OHOS::AppPackingTool::DeviceCalculator::DeviceTypeToString(
                  OHOS::AppPackingTool::DeviceType::CAR), "car");
}

// 测试3：单个entry模块计算设备集合
HWTEST_F(DeviceCalculatorTest, CalculateDevices_SingleModule, TestSize.Level0) {
    // 创建支持phone和tablet的entry模块
    auto entryModule = CreateMockEntryModule({"phone", "tablet"});
    ASSERT_NE(entryModule, nullptr);

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> entryModules;
    entryModules.push_back(entryModule);

    auto devices = calculator_.CalculateDevices(entryModules);

    EXPECT_EQ(devices.size(), 2); // 应该有2个设备实例
    EXPECT_EQ(devices[0].type, OHOS::AppPackingTool::DeviceType::PHONE);
    EXPECT_EQ(devices[1].type, OHOS::AppPackingTool::DeviceType::TABLET);
    EXPECT_TRUE(devices[0].distributionFilter.empty());
    EXPECT_TRUE(devices[1].distributionFilter.empty());
}

// 测试4：多个entry模块计算设备集合（合并）
HWTEST_F(DeviceCalculatorTest, CalculateDevices_MultipleModules, TestSize.Level0) {
    // 创建两个entry模块，分别支持不同的设备
    auto entryModule1 = CreateMockEntryModule({"phone"});
    auto entryModule2 = CreateMockEntryModule({"tablet", "tv"});

    ASSERT_NE(entryModule1, nullptr);
    ASSERT_NE(entryModule2, nullptr);

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> entryModules;
    entryModules.push_back(entryModule1);
    entryModules.push_back(entryModule2);

    auto devices = calculator_.CalculateDevices(entryModules);

    EXPECT_EQ(devices.size(), 3); // 应该有3个不同的设备实例
}

// 测试5：空entry模块列表
HWTEST_F(DeviceCalculatorTest, CalculateDevices_EmptyModules, TestSize.Level0) {
    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> entryModules;
    auto devices = calculator_.CalculateDevices(entryModules);

    EXPECT_EQ(devices.size(), 0); // 应该返回空列表
}

// 测试6：设备实例去重
HWTEST_F(DeviceCalculatorTest, CalculateDevices_DuplicateRemoval, TestSize.Level0) {
    // 创建两个entry模块，都支持相同的设备
    auto entryModule1 = CreateMockEntryModule({"phone"});
    auto entryModule2 = CreateMockEntryModule({"phone"});

    ASSERT_NE(entryModule1, nullptr);
    ASSERT_NE(entryModule2, nullptr);

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> entryModules;
    entryModules.push_back(entryModule1);
    entryModules.push_back(entryModule2);

    auto devices = calculator_.CalculateDevices(entryModules);

    EXPECT_EQ(devices.size(), 1); // 应该去重，只有1个设备实例
    EXPECT_EQ(devices[0].type, OHOS::AppPackingTool::DeviceType::PHONE);
}

// 测试7：默认设备类型
HWTEST_F(DeviceCalculatorTest, CalculateDevices_DefaultDeviceTypes, TestSize.Level0) {
    // 创建一个没有指定deviceTypes的模块（模拟空列表）
    std::string jsonStr = R"({
        "app": {
            "bundleName": "com.example.app"
        },
        "module": {
            "name": "entry",
            "type": "entry"
        }
    })";

    auto moduleJson = std::make_shared<OHOS::AppPackingTool::ModuleJson>();
    ASSERT_TRUE(moduleJson->ParseFromString(jsonStr));

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> entryModules;
    entryModules.push_back(moduleJson);

    auto devices = calculator_.CalculateDevices(entryModules);

    // 应该默认支持所有6种设备类型
    EXPECT_EQ(devices.size(), 6);
}

HWTEST_F(DeviceCalculatorTest, CalculateDevices_DefaultMeansPhone, TestSize.Level0) {
    auto defaultEntry = CreateMockEntryModule({"default"});
    auto tabletEntry = CreateMockEntryModule({"tablet"});
    ASSERT_NE(defaultEntry, nullptr);
    ASSERT_NE(tabletEntry, nullptr);

    auto devices = calculator_.CalculateDevices({defaultEntry, tabletEntry});

    ASSERT_EQ(devices.size(), 2);
    EXPECT_EQ(devices[0].type, OHOS::AppPackingTool::DeviceType::PHONE);
    EXPECT_EQ(devices[1].type, OHOS::AppPackingTool::DeviceType::TABLET);
}
