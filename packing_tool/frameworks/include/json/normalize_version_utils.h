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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_NORMALIZE_VERSION_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_NORMALIZE_VERSION_UTILS_H

#include <string>
#include <list>
#include "normalize_version.h"

namespace OHOS {
namespace AppPackingTool {
class NormalizeVersionUtils {
public:
    NormalizeVersionUtils() {};
    virtual ~NormalizeVersionUtils() {};

    NormalizeVersionUtils(const NormalizeVersionUtils &) = delete;
    NormalizeVersionUtils &operator=(const NormalizeVersionUtils &) = delete;

    static std::string ToString(const NormalizeVersion& normalizeVersion);
    static std::string ArrayToString(const std::list<NormalizeVersion>& normalizeVersions);
};
}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_NORMALIZE_VERSION_UTILS_H
