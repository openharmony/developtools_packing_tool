/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_JSON_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_JSON_UTILS_H

#include <memory>
#include <string>

#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {
class JsonUtils {
public:
    JsonUtils() {};
    virtual ~JsonUtils() {};

    JsonUtils(const JsonUtils &) = delete;
    JsonUtils &operator=(const JsonUtils &) = delete;

    static bool IsModuleJson(const std::string& filePath);
    static bool IsConfigJson(const std::string& filePath);
    static bool IsPatchJson(const std::string& filePath);

    static std::unique_ptr<PtJson> JsonFromFile(const std::string& filePath);
    static bool JsonToFile(const std::unique_ptr<PtJson>& json, const std::string& filePath);
    static bool StrToFile(const std::string& str, const std::string& filePath);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_JSON_UTILS_H
