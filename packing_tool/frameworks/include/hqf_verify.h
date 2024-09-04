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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQFVERIFY_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQFVERIFY_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "json/hqf_info.h"

namespace OHOS {
namespace AppPackingTool {

class HQFVerify {
public:
    static bool CheckHQFIsValid(const std::vector<HqfInfo> &hqfInfos);

private:
    static bool CheckAppFields(const std::vector<HqfInfo> &hqfInfos);
    static bool CheckModuleIsDuplicated(const HqfInfo& hqfVerifyInfoLeft, const HqfInfo& hqfVerifyInfoRight);
    static bool CheckModuleIsValid(const std::vector<HqfInfo> &hqfInfos);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQFVERIFY_H
