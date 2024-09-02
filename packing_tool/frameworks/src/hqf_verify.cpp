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

#include "hqf_verify.h"

#include "log.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
bool HQFVerify::CheckHQFIsValid(const std::vector<HqfInfo>& hqfInfos)
{
    if (hqfInfos.empty()) {
        LOGE("Error: input hqf file is empty!");
        return false;
    }
    if (!CheckAppFields(hqfInfos)) {
        LOGE("Error: input hqf file has different fields in app!");
        return false;
    }
    if (!CheckModuleIsValid(hqfInfos)) {
        LOGE("Error: input hqf file moduleName is invalid!");
        return false;
    }
    return true;
}

bool HQFVerify::CheckAppFields(const std::vector<HqfInfo>& hqfInfos)
{
    const std::string& bundleName = hqfInfos[0].GetBundleName();
    int32_t versionCode = hqfInfos[0].GetVersionCode();
    const std::string& versionName = hqfInfos[0].GetVersionName();
    int32_t patchVersionCode = hqfInfos[0].GetPatchVersionCode();
    const std::string& patchVersionName = hqfInfos[0].GetPatchVersionName();

    for (const auto& info : hqfInfos) {
        if (bundleName.empty() || bundleName != info.GetBundleName()) {
            LOGE("Input hqf file has different bundleName!");
            return false;
        }
        if (versionCode != info.GetVersionCode()) {
            LOGE("Input hqf file has different versionCode!");
            return false;
        }
        if (versionName.empty() || versionName != info.GetVersionName()) {
            LOGE("Input hqf file has different versionName!");
            return false;
        }
        if (patchVersionCode != info.GetPatchVersionCode()) {
            LOGE("Input hqf file has different patchVersionCode!");
            return false;
        }
        if (patchVersionName.empty() || patchVersionName != info.GetPatchVersionName()) {
            LOGE("Input hqf file has different patchVersionName!");
            return false;
        }
    }
    return true;
}

bool HQFVerify::CheckModuleIsDuplicated(const HqfInfo& hqfVerifyInfoLeft, const HqfInfo& hqfVerifyInfoRight)
{
    if (hqfVerifyInfoLeft.GetModuleName().compare(hqfVerifyInfoRight.GetModuleName()) != 0) {
        return false;
    }
    if (Utils::CheckDisjoint(hqfVerifyInfoLeft.GetDeviceTypes(), hqfVerifyInfoRight.GetDeviceTypes())) {
        return false;
    }
    return true;
}

bool HQFVerify::CheckModuleIsValid(const std::vector<HqfInfo>& hqfInfos)
{
    for (int i = 0; i < hqfInfos.size(); ++i) {
        for (int j = i + 1; j < hqfInfos.size(); ++j) {
            if (CheckModuleIsDuplicated(hqfInfos[i], hqfInfos[j])) {
                LOGE("input hqf file moduleName duplicated.");
                return false;
            }
        }
    }
    return true;
}
}  // namespace AppPackingTool
}  // namespace OHOS