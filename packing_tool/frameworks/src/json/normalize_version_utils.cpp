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

#include "pt_json.h"
#include "normalize_version_utils.h"
#include "log.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
    const std::string ORIGIN_VERSION_CODE = "originVersionCode";
    const std::string ORIGIN_VERSION_NAME = "originVersionName";
    const std::string MODULE_NAME = "moduleName";
}

std::string NormalizeVersionUtils::ToString(const NormalizeVersion& normalizeVersion)
{
    std::unique_ptr<PtJson> ptJson = PtJson::CreateObject();
    if (!ptJson->Add(MODULE_NAME.c_str(), normalizeVersion.moduleName.c_str())) {
        return "";
    }
    if (!ptJson->Add(ORIGIN_VERSION_CODE.c_str(), normalizeVersion.originVersionCode)) {
        return "";
    }
    if (!ptJson->Add(ORIGIN_VERSION_NAME.c_str(), normalizeVersion.originVersionName.c_str())) {
        return "";
    }
    return ptJson->Stringify();
}

std::string NormalizeVersionUtils::ArrayToString(const std::list<NormalizeVersion>& normalizeVersions)
{
    std::unique_ptr<PtJson> versionsJson = PtJson::CreateArray();
    for (auto& normalizeVersion : normalizeVersions) {
        std::unique_ptr<PtJson> versionJson = PtJson::CreateObject();
        if (!versionJson->Add(MODULE_NAME.c_str(), normalizeVersion.moduleName.c_str())) {
            return "";
        }
        if (!versionJson->Add(ORIGIN_VERSION_CODE.c_str(), normalizeVersion.originVersionCode)) {
            return "";
        }
        if (!versionJson->Add(ORIGIN_VERSION_NAME.c_str(), normalizeVersion.originVersionName.c_str())) {
            return "";
        }
        versionsJson->Push(versionJson);
    }
    return versionsJson->Stringify();
}
} // namespace AppPackingTool
} // namespace OHOS
