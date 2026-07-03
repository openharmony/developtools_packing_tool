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
#include <filesystem>
#include <fstream>
#include <memory>
#include "json/pack_info.h"

class PackInfoParserTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    OHOS::AppPackingTool::PackInfo packInfo_;
};

void PackInfoParserTest::SetUp() {}
void PackInfoParserTest::TearDown() {}

// 测试1：解析包含deduplicateSo字段的pack.info（true）
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_WithTrueValue, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app", "deduplicateSo": true}, "modules": []},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = false;
    EXPECT_TRUE(packInfo_.GetDeduplicateSo(deduplicateSo));
    EXPECT_TRUE(deduplicateSo);
}

// 测试2：解析包含deduplicateSo字段的pack.info（false）
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_WithFalseValue, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app", "deduplicateSo": false}, "modules": []},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = true;
    EXPECT_TRUE(packInfo_.GetDeduplicateSo(deduplicateSo));
    EXPECT_FALSE(deduplicateSo);
}

// 测试3：解析不包含deduplicateSo字段的pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_WithoutField, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app"}, "modules": []},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = false;
    EXPECT_FALSE(packInfo_.GetDeduplicateSo(deduplicateSo));
}

// 测试4：使用默认值解析不包含deduplicateSo字段的pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSoWithDefault_WithoutField, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app"}, "modules": []},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = true; // 初始值
    EXPECT_TRUE(packInfo_.GetDeduplicateSoWithDefault(deduplicateSo, false));
    EXPECT_FALSE(deduplicateSo); // 应该使用默认值false
}

// 测试5：使用默认值解析包含deduplicateSo字段的pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSoWithDefault_WithField, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app", "deduplicateSo": true}, "modules": []},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = false;
    EXPECT_TRUE(packInfo_.GetDeduplicateSoWithDefault(deduplicateSo, false));
    EXPECT_TRUE(deduplicateSo); // 应该使用实际值true
}

// 测试6：解析复杂格式的pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_ComplexPackInfo, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {
            "app": {
                "bundleName": "com.example.app",
                "deduplicateSo": true,
                "version": {"code": 1, "name": "1.0.0"}
            },
            "modules": []
        },
        "packages": [{"moduleType": "entry", "name": "entry", "deliveryWithInstall": true}]
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = false;
    EXPECT_TRUE(packInfo_.GetDeduplicateSo(deduplicateSo));
    EXPECT_TRUE(deduplicateSo);
}

// 测试7：向后兼容性 - 旧版本pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_LegacyPackInfo, TestSize.Level0) {
    // 模拟旧版本pack.info（不包含deduplicateSo字段）
    std::string jsonStr = R"({
        "summary": {
            "app": {
                "bundleName": "com.example.app",
                "version": {"code": 1000000, "name": "1.0.0"}
            },
            "modules": []
        },
        "packages": [{"moduleType": "entry", "name": "entry"}]
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));

    bool deduplicateSo = true;
    bool result = packInfo_.GetDeduplicateSo(deduplicateSo);

    // 应该返回false，表示字段不存在
    EXPECT_FALSE(result);

    // 使用默认值应该成功
    EXPECT_TRUE(packInfo_.GetDeduplicateSoWithDefault(deduplicateSo, false));
    EXPECT_FALSE(deduplicateSo);
}

// 测试8：从文件解析pack.info
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_FromFile, TestSize.Level0) {
    // 创建临时pack.info文件
    std::string tempFilePath = "temp_pack_info.json";
    std::string jsonStr = R"({
        "summary": {"app": {"bundleName": "com.example.app", "deduplicateSo": true}, "modules": []},
        "packages": []
    })";

    // 写入临时文件
    std::ofstream outFile(tempFilePath);
    outFile << jsonStr;
    outFile.close();

    // 从文件解析
    EXPECT_TRUE(packInfo_.ParseFromFile(tempFilePath));

    bool deduplicateSo = false;
    EXPECT_TRUE(packInfo_.GetDeduplicateSo(deduplicateSo));
    EXPECT_TRUE(deduplicateSo);

    // 清理临时文件
    std::filesystem::remove(tempFilePath);
}

// deduplicateSo is parsed from summary.app.
HWTEST_F(PackInfoParserTest, GetDeduplicateSo_FromSummaryApp, TestSize.Level0) {
    std::string jsonStr = R"({
        "summary": {
            "app": {
                "bundleName": "com.example.app",
                "deduplicateSo": true
            }
        },
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));
    bool deduplicateSo = false;
    EXPECT_TRUE(packInfo_.GetDeduplicateSo(deduplicateSo));
    EXPECT_TRUE(deduplicateSo);
}

HWTEST_F(PackInfoParserTest, GetDeduplicateSo_RootValueIgnored, TestSize.Level0) {
    std::string jsonStr = R"({
        "deduplicateSo": true,
        "summary": {"app": {"bundleName": "com.example.app"}},
        "packages": []
    })";

    ASSERT_TRUE(packInfo_.ParseFromString(jsonStr));
    bool deduplicateSo = false;
    EXPECT_FALSE(packInfo_.GetDeduplicateSo(deduplicateSo));
}
