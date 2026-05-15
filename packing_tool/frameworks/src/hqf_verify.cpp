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
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
bool HQFVerify::CheckHQFIsValid(const std::vector<HqfInfo>& hqfInfos)
{
    if (hqfInfos.empty()) {
        LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
            "Input hqf file is empty.").c_str());
        return false;
    }
    if (!CheckAppFields(hqfInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
            "Input hqf file has different fields in app.").c_str());
        return false;
    }
    if (!CheckModuleIsValid(hqfInfos)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
            "Input hqf file moduleName is invalid.").c_str());
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
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_FAILED.toStringWithArgs(
                "Input hqf file has different bundleName.").c_str());
            return false;
        }
        if (versionCode != info.GetVersionCode()) {
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_FAILED.toStringWithArgs(
                "Input hqf file has different versionCode.").c_str());
            return false;
        }
        if (versionName.empty() || versionName != info.GetVersionName()) {
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_FAILED.toStringWithArgs(
                "Input hqf file has different versionName.").c_str());
            return false;
        }
        if (patchVersionCode != info.GetPatchVersionCode()) {
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_FAILED.toStringWithArgs(
                "Input hqf file has different patchVersionCode.").c_str());
            return false;
        }
        if (patchVersionName.empty() || patchVersionName != info.GetPatchVersionName()) {
            LOGE("%s", PackingToolErrMsg::CHECK_APP_FIELDS_FAILED.toStringWithArgs(
                "Input hqf file has different patchVersionName.").c_str());
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
    for (size_t i = 0; i < hqfInfos.size(); ++i) {
        for (size_t j = i + 1; j < hqfInfos.size(); ++j) {
            if (CheckModuleIsDuplicated(hqfInfos[i], hqfInfos[j])) {
                LOGE("%s", PackingToolErrMsg::CHECK_MODULE_INVALID.toStringWithArgs(
                    "Input hqf file moduleName is duplicated.").c_str());
                return false;
            }
        }
    }
    return true;
}
}  // namespace AppPackingTool
}  // namespace OHOS
