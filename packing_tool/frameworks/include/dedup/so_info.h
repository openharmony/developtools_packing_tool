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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_INFO_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_INFO_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdint>

namespace OHOS {
namespace AppPackingTool {

// SO文件信息
struct SoInfo {
    std::string relativePath;  // 解压后的路径（如"libs/armeabi-v7a/libexample.so"）
    std::string md5;          // MD5值
    std::string sourceModule; // 来源模块名
    int64_t fileSize;         // 文件大小（字节）

    bool operator==(const SoInfo& other) const {
        return relativePath == other.relativePath && md5 == other.md5;
    }

    bool operator<(const SoInfo& other) const {
        if (relativePath != other.relativePath) {
            return relativePath < other.relativePath;
        }
        return md5 < other.md5;
    }
};

// 重复SO组（模块内相对路径和MD5均相同的SO集合）
struct DuplicateSoGroup {
    std::string relativePath;         // 模块内相对路径
    std::string md5;                  // MD5值
    std::vector<SoInfo> soList;       // 包含的SO列表

    // 获取该重复SO组涉及的所有模块名
    std::set<std::string> GetInvolvedModules() const {
        std::set<std::string> modules;
        for (const auto& so : soList) {
            modules.insert(so.sourceModule);
        }
        return modules;
    }
};

// 去重方案
struct DedupPlan {
    // 保留的SO（按模块名 -> SO路径列表）
    std::map<std::string, std::vector<std::string>> keptSoMap;

    // 移除的SO（按模块名 -> SO路径列表）
    std::map<std::string, std::vector<std::string>> removedSoMap;

    // 总共节省的大小（字节）
    int64_t totalSavedSize;

    DedupPlan() : totalSavedSize(0) {}

    // 添加保留的SO
    void AddKeptSo(const std::string& moduleName, const std::string& soPath) {
        keptSoMap[moduleName].push_back(soPath);
    }

    // 添加移除的SO
    void AddRemovedSo(const std::string& moduleName, const std::string& soPath, int64_t fileSize) {
        removedSoMap[moduleName].push_back(soPath);
        totalSavedSize += fileSize;
    }
};

// 去重策略类型
enum class DedupStrategy {
    EXACT,   // 精确算法（整数规划）
    GREEDY   // 贪心算法
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_SO_INFO_H
