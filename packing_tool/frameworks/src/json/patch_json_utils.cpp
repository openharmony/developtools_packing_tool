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

#include "patch_json_utils.h"

#include <fstream>

#include "log.h"

namespace OHOS {
namespace AppPackingTool {
bool PatchJsonUtils::ParsePatchByJsonFilePath(const std::string& patchJsonFilePath, HqfInfo& hqfInfo)
{
    PatchJson patchJson;
    if (!patchJson.ParseFromFile(patchJsonFilePath)) {
        return false;
    }
    if (!patchJson.GetHqfInfo(hqfInfo)) {
        return false;
    }
    return true;
}

bool PatchJsonUtils::ParsePatchByJsonStr(const std::string& patchJsonStr, HqfInfo& hqfInfo)
{
    PatchJson patchJson;
    if (!patchJson.ParseFromString(patchJsonStr)) {
        return false;
    }
    if (!patchJson.GetHqfInfo(hqfInfo)) {
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
