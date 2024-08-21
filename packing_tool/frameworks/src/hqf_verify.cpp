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
    bool HQFVerify::checkHQFIsValid(const std::vector<HqfInfo>& HqfInfos)
    {
        if (HqfInfos.empty()) {
            LOGE("Error: input hqf file is empty!");
            return false;
        }
        if (!checkAppFields(HqfInfos)) {
            LOGE("Error: input hqf file has different fields in app!");
            return false;
        }
        if (!checkModuleIsValid(HqfInfos)) {
            LOGE("Error: input hqf file moduleName is invalid!");
            return false;
        }
        return true;
    }

    bool HQFVerify::checkAppFields(const std::vector<HqfInfo>& HqfInfos)
    {
        const std::string& bundleName = HqfInfos[0].GetBundleName();
        int versionCode = HqfInfos[0].GetVersionCode();
        const std::string& versionName = HqfInfos[0].GetVersionName();
        int patchVersionCode = HqfInfos[0].GetPatchVersionCode();
        const std::string& patchVersionName = HqfInfos[0].GetPatchVersionName();

        for (const auto& info : HqfInfos) {
            if (bundleName.empty() || bundleName != info.GetBundleName()) {
                LOGE("Error: input hqf file has different bundleName!");
                return false;
            }
            if (versionCode != info.GetVersionCode()) {
                LOGE("Error: input hqf file has different versionCode!");
                return false;
            }
            if (versionName.empty() || versionName != info.GetVersionName()) {
                LOGE("Error: input hqf file has different versionName!");
                return false;
            }
            if (patchVersionCode != info.GetPatchVersionCode()) {
                LOGE("Error: input hqf file has different patchVersionCode!");
                return false;
            }
            if (patchVersionName.empty() || patchVersionName != info.GetPatchVersionName()) {
                LOGE("Error: input hqf file has different patchVersionName!");
                return false;
            }
        }
        return true;
    }

    bool HQFVerify::checkModuleIsValid(const std::vector<HqfInfo>& HqfInfos)
    {
        std::unordered_set<std::string> moduleNames;
        std::list<std::string> deviceTypes_;
        for (const auto& info : HqfInfos) {
            if (!moduleNames.insert(info.GetModuleName()).second) {
                LOGE("Error: input hqf file has overlapping moduleNames!, %s", info.GetModuleName().c_str());
                return false;
            }
            const auto& deviceTypes = info.GetDeviceTypes();
            if (!Utils::CheckDisjoint(deviceTypes_, deviceTypes)) {
                LOGE("Error: input hqf file has overlapping device types!");
                return false;
            }
            deviceTypes_.insert(deviceTypes_.end(), deviceTypes.begin(), deviceTypes.end());
        }
        return true;
    }
}  // namespace AppPackingTool
}  // namespace OHOS

