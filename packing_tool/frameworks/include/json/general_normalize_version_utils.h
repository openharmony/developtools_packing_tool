/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_VERSION_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_VERSION_UTILS_H

#include <list>
#include <string>

#include "general_normalize_version.h"

namespace OHOS {
namespace AppPackingTool {
class GeneralNormalizeVersionUtils {
public:
    GeneralNormalizeVersionUtils() {};
    virtual ~GeneralNormalizeVersionUtils() {};

    GeneralNormalizeVersionUtils(const GeneralNormalizeVersionUtils &) = delete;
    GeneralNormalizeVersionUtils &operator=(const GeneralNormalizeVersionUtils &) = delete;

    static std::string ArrayToString(const std::list<GeneralNormalizeVersion>& generalNormalizeVersions);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_VERSION_UTILS_H
