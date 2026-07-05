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
#include <algorithm>
#include <memory>
#include "dedup/module_calculator.h"
#include "dedup/device_calculator.h"
#include "json/module_json.h"

using testing::ext::TestSize;

// 测试辅助函数：创建模拟模块
std::shared_ptr<OHOS::AppPackingTool::ModuleJson> CreateMockModule(
    const std::string& moduleName,
    const std::string& moduleType,
    const std::vector<std::string>& deviceTypes,
    bool deliveryWithInstall = true,
    const std::string& distributionFilter = "",
    const std::string& compileSdkType = "OpenHarmony")
{
    (void)distributionFilter;
    std::string jsonStr = R"({"app":{"bundleName":"com.example.app","bundleType":"app")";
    if (!compileSdkType.empty()) {
        jsonStr += R"(,"compileSdkType":")" + compileSdkType + "\"";
    }
    jsonStr += R"(},"module":{"name":")" + moduleName +
        R"(","type":")" + moduleType + R"(","deviceTypes":[)";

    // 添加设备类型
    for (size_t i = 0; i < deviceTypes.size(); ++i) {
        jsonStr += "\"" + deviceTypes[i] + "\"";
        if (i < deviceTypes.size() - 1) {
            jsonStr += ",";
        }
    }

    jsonStr += R"(],"deliveryWithInstall":)";
    jsonStr += (deliveryWithInstall ? "true" : "false");
    jsonStr += "}}";

    auto moduleJson = std::make_shared<OHOS::AppPackingTool::ModuleJson>();
    if (moduleJson->ParseFromString(jsonStr)) {
        return moduleJson;
    }

    return nullptr;
}

class ModuleCalculatorTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::ModuleCalculator calculator_;
};

void ModuleCalculatorTest::SetUp() {}
void ModuleCalculatorTest::TearDown() {}

// 测试1：支持指定设备类型的模块
HWTEST_F(ModuleCalculatorTest, SupportsDeviceType_ValidDevice, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.deviceTypes = {
        OHOS::AppPackingTool::DeviceType::PHONE,
        OHOS::AppPackingTool::DeviceType::TABLET
    };

    EXPECT_TRUE(calculator_.SupportsDeviceType(config, OHOS::AppPackingTool::DeviceType::PHONE));
    EXPECT_TRUE(calculator_.SupportsDeviceType(config, OHOS::AppPackingTool::DeviceType::TABLET));
    EXPECT_FALSE(calculator_.SupportsDeviceType(config, OHOS::AppPackingTool::DeviceType::TV));
}

// 测试2：distributionFilter匹配
HWTEST_F(ModuleCalculatorTest, MatchesDistributionFilter_ValidMatch, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.distributionFilter = "filter1";

    OHOS::AppPackingTool::DeviceInstance device1;
    device1.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device1.distributionFilter = "filter1";

    OHOS::AppPackingTool::DeviceInstance device2;
    device2.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device2.distributionFilter = "filter2";

    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::MatchesDistributionFilter(config, device1));
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::MatchesDistributionFilter(config, device2));
}

// 测试3：空distributionFilter匹配所有设备
HWTEST_F(ModuleCalculatorTest, MatchesDistributionFilter_EmptyFilter, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.distributionFilter = "";

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "some_filter";

    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::MatchesDistributionFilter(config, device));
}

// 测试4：必然安装模块判断 - 满足所有条件
HWTEST_F(ModuleCalculatorTest, IsMandatoryModule_AllConditionsMet, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "feature_module";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = true;
    config.distributionFilter = "";
    config.requireDeviceFeatures = {};

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "";

    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

// 测试5：必然安装模块判断 - deliveryWithInstall为false
HWTEST_F(ModuleCalculatorTest, IsMandatoryModule_DeliveryWithInstallFalse, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "feature_module";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = false; // 不满足条件
    config.distributionFilter = "";
    config.requireDeviceFeatures = {};

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "";

    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

HWTEST_F(ModuleCalculatorTest, IsMandatoryEntry_IgnoresDeliveryWithInstall, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "entry";
    config.moduleType = "entry";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = false;
    config.requireDeviceFeatures = {"phone"};

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;

    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

// 测试6：必然安装模块判断 - distributionFilter不为空
HWTEST_F(ModuleCalculatorTest, IsMandatoryModule_NonEmptyDistributionFilter, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "feature_module";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = true;
    config.distributionFilter = "some_filter"; // 不满足条件
    config.requireDeviceFeatures = {};

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "";

    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

HWTEST_F(ModuleCalculatorTest, IsMandatoryModule_MatchingDistributionFilter, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "feature_module";
    config.moduleType = "feature";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = true;
    config.distributionFilter = "some_filter";

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "some_filter";

    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

// 测试7：必然安装模块判断 - requireDeviceFeatures不为空
HWTEST_F(ModuleCalculatorTest, IsMandatoryModule_NonEmptyRequireDeviceFeatures, TestSize.Level0) {
    OHOS::AppPackingTool::ModuleConfig config;
    config.moduleName = "feature_module";
    config.deviceTypes = {OHOS::AppPackingTool::DeviceType::PHONE};
    config.deliveryWithInstall = true;
    config.distributionFilter = "";
    config.requireDeviceFeatures = {"feature1"}; // 不满足条件

    OHOS::AppPackingTool::DeviceInstance device;
    device.type = OHOS::AppPackingTool::DeviceType::PHONE;
    device.distributionFilter = "";

    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsMandatoryModule(config, device));
}

// 测试8：计算必然安装模块集合
HWTEST_F(ModuleCalculatorTest, CalculateMandatoryModules_BasicCase, TestSize.Level0) {
    // 创建3个模块
    auto module1 = CreateMockModule("entry", "entry", {"phone"}, true, "");
    auto module2 = CreateMockModule("feature1", "feature", {"phone"}, true, "");
    auto module3 = CreateMockModule("feature2", "feature", {"phone"}, false, ""); // deliveryWithInstall=false

    ASSERT_NE(module1, nullptr);
    ASSERT_NE(module2, nullptr);
    ASSERT_NE(module3, nullptr);

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> modules;
    modules.push_back(module1);
    modules.push_back(module2);
    modules.push_back(module3);

    // 创建phone设备
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    std::vector<OHOS::AppPackingTool::DeviceInstance> devices;
    devices.push_back(phoneDevice);

    // 计算必然安装模块集合
    auto mandatoryMap = calculator_.CalculateMandatoryModules(modules, devices);

    EXPECT_EQ(mandatoryMap.size(), 1);
    EXPECT_EQ(mandatoryMap[phoneDevice].size(), 2); // entry和feature1应该是必然安装的

    // 验证模块名称
    auto& modulesForPhone = mandatoryMap[phoneDevice];
    EXPECT_TRUE(std::find(modulesForPhone.begin(), modulesForPhone.end(), "entry") != modulesForPhone.end());
    EXPECT_TRUE(std::find(modulesForPhone.begin(), modulesForPhone.end(), "feature1") != modulesForPhone.end());
    EXPECT_FALSE(std::find(modulesForPhone.begin(), modulesForPhone.end(), "feature2") != modulesForPhone.end());
}

// 测试9：多设备场景
HWTEST_F(ModuleCalculatorTest, CalculateMandatoryModules_MultipleDevices, TestSize.Level0) {
    // 创建支持不同设备的模块
    auto module1 = CreateMockModule("entry_phone", "entry", {"phone"}, true, "");
    auto module2 = CreateMockModule("entry_tablet", "entry", {"tablet"}, true, "");
    auto module3 = CreateMockModule("feature_common", "feature", {"phone", "tablet"}, true, "");

    ASSERT_NE(module1, nullptr);
    ASSERT_NE(module2, nullptr);
    ASSERT_NE(module3, nullptr);

    std::vector<std::shared_ptr<OHOS::AppPackingTool::ModuleJson>> modules;
    modules.push_back(module1);
    modules.push_back(module2);
    modules.push_back(module3);

    // 创建phone和tablet设备
    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    phoneDevice.distributionFilter = "";

    OHOS::AppPackingTool::DeviceInstance tabletDevice;
    tabletDevice.type = OHOS::AppPackingTool::DeviceType::TABLET;
    tabletDevice.distributionFilter = "";

    std::vector<OHOS::AppPackingTool::DeviceInstance> devices;
    devices.push_back(phoneDevice);
    devices.push_back(tabletDevice);

    // 计算必然安装模块集合
    auto mandatoryMap = calculator_.CalculateMandatoryModules(modules, devices);

    EXPECT_EQ(mandatoryMap.size(), 2);

    // phone设备应该有entry_phone和feature_common
    EXPECT_EQ(mandatoryMap[phoneDevice].size(), 2);

    // tablet设备应该有entry_tablet和feature_common
    EXPECT_EQ(mandatoryMap[tabletDevice].size(), 2);
}

HWTEST_F(ModuleCalculatorTest, CalculateMandatoryModules_DefaultDoesNotMatchTablet, TestSize.Level0) {
    auto defaultEntry = CreateMockModule("entry", "entry", {"default"}, true, "");
    auto tabletEntry = CreateMockModule("tablet_entry", "entry", {"tablet"}, true, "");
    ASSERT_NE(defaultEntry, nullptr);
    ASSERT_NE(tabletEntry, nullptr);

    OHOS::AppPackingTool::DeviceInstance phoneDevice;
    phoneDevice.type = OHOS::AppPackingTool::DeviceType::PHONE;
    OHOS::AppPackingTool::DeviceInstance tabletDevice;
    tabletDevice.type = OHOS::AppPackingTool::DeviceType::TABLET;

    auto mandatoryMap = calculator_.CalculateMandatoryModules(
        {defaultEntry, tabletEntry}, {phoneDevice, tabletDevice});

    ASSERT_EQ(mandatoryMap[phoneDevice].size(), 1);
    EXPECT_EQ(mandatoryMap[phoneDevice][0], "entry");
    ASSERT_EQ(mandatoryMap[tabletDevice].size(), 1);
    EXPECT_EQ(mandatoryMap[tabletDevice][0], "tablet_entry");
}

HWTEST_F(ModuleCalculatorTest, IsValidForDedup_RequiredFields, TestSize.Level0) {
    auto validModule = CreateMockModule("entry", "entry", {"phone"}, false);
    ASSERT_NE(validModule, nullptr);
    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(validModule);
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));

    config.moduleName.clear();
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.moduleName = "entry";
    config.moduleType = "har";
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.moduleType = "skill";
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.moduleType = "entry";
    config.bundleType = "shared";
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.bundleType = "skill";
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.bundleType = "appService";
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.bundleType = "atomicService";
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.bundleType = "app";
    config.deviceTypesConfigured = false;
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    config.deviceTypesConfigured = true;
    config.deliveryWithInstallPresent = false;
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
}

HWTEST_F(ModuleCalculatorTest, BundleType_MissingDefaultsToApp, TestSize.Level0)
{
    const std::string jsonStr = R"({
        "app": {
            "bundleName": "com.example.app",
            "compileSdkType": "OpenHarmony"
        },
        "module": {
            "name": "entry",
            "type": "entry",
            "deviceTypes": ["phone"],
            "deliveryWithInstall": true
        }
    })";
    auto module = std::make_shared<OHOS::AppPackingTool::ModuleJson>();
    ASSERT_TRUE(module->ParseFromString(jsonStr));

    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(module);
    EXPECT_EQ(config.bundleType, "app");
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
}

HWTEST_F(ModuleCalculatorTest, FaModel_IsExcludedFromDedup, TestSize.Level0)
{
    auto module = CreateMockModule("entry", "entry", {"phone"}, true);
    ASSERT_NE(module, nullptr);

    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(module, false);
    EXPECT_FALSE(config.stageModel);
    EXPECT_FALSE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
}

// 测试10：compileSdkType为OpenHarmony时允许去重
HWTEST_F(ModuleCalculatorTest, CompileSdkType_OpenHarmony_Allowed, TestSize.Level0) {
    auto module = CreateMockModule("entry", "entry", {"phone"}, true, "", "OpenHarmony");
    ASSERT_NE(module, nullptr);

    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(module);
    EXPECT_EQ(config.compileSdkType, "OpenHarmony");
    // compileSdkType为OpenHarmony不影响去重资格验证
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
}

// 测试11：compileSdkType为OpenHarmony时模块不参与去重（在so_deduplicator中过滤）
HWTEST_F(ModuleCalculatorTest, CompileSdkType_OpenHarmony_Extracted, TestSize.Level0) {
    auto module = CreateMockModule("entry", "entry", {"phone"}, true, "", "OpenHarmony");
    ASSERT_NE(module, nullptr);

    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(module);
    EXPECT_EQ(config.compileSdkType, "OpenHarmony");
    // compileSdkType为OpenHarmony时，ExtractModuleConfig仍返回有效配置
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
    // 但在so_deduplicator的FilterDedupEligibleModules中会被过滤掉
}

// 测试12：compileSdkType为空时默认允许去重
HWTEST_F(ModuleCalculatorTest, CompileSdkType_Empty_Allowed, TestSize.Level0) {
    auto module = CreateMockModule("entry", "entry", {"phone"}, true, "", "");
    ASSERT_NE(module, nullptr);

    auto config = OHOS::AppPackingTool::ModuleCalculator::ExtractModuleConfig(module);
    EXPECT_EQ(config.compileSdkType, "");
    // 空compileSdkType不影响去重资格验证
    EXPECT_TRUE(OHOS::AppPackingTool::ModuleCalculator::IsValidForDedup(config));
}
