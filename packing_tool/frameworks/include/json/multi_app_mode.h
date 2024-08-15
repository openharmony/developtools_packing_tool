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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTI_APP_MODE_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTI_APP_MODE_H

#include <string>

namespace OHOS {
namespace AppPackingTool {
// java : java has two override functions: equals() and hashCode()
struct MultiAppMode {
    std::string multiAppModeType = "";
    int maxCount = 0;

    bool operator == (const MultiAppMode& multiAppMode) const
    {
        return ((multiAppModeType == multiAppMode.multiAppModeType) && (maxCount == multiAppMode.maxCount));
    }

    bool operator != (const MultiAppMode& multiAppMode) const
    {
        return !((multiAppModeType == multiAppMode.multiAppModeType) && (maxCount == multiAppMode.maxCount));
    }
};
}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTI_APP_MODE_H
