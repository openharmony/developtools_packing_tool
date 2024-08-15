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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PATCH_JSON_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PATCH_JSON_H

#include <string>
#include <list>
#include "pt_json.h"
#include "hqf_info.h"

namespace OHOS {
namespace AppPackingTool {

class PatchJson {
public:
    PatchJson() {};
    virtual ~PatchJson() {};

    bool ParseFromString(const std::string& jsonString);
    bool ParseFromFile(const std::string& jsonFile);
    std::string ToString();
    void Release();

    bool IsValid();

    // // object funcs
    bool GetAppObject(std::unique_ptr<PtJson>& appObj);
    bool GetModuleObject(std::unique_ptr<PtJson>& moduleObj);
 
    // app funcs
    bool GetBundleName(std::string& bundleName);
    bool GetBundleNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleName);
    bool GetVersionCode(int32_t& versionCode);
    bool GetVersionCodeByAppObj(const std::unique_ptr<PtJson>& appObj, int32_t& versionCode);
    bool GetVersionName(std::string& versionName);
    bool GetVersionNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& versionName);
    bool GetPatchVersionCode(int32_t& patchVersionCode);
    bool GetPatchVersionCodeByAppObj(const std::unique_ptr<PtJson>& appObj, int32_t& patchVersionCode);
    bool GetPatchVersionName(std::string& patchVersionName);
    bool GetPatchVersionNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& patchVersionName);

    // module funcs
    bool GetName(std::string& name);
    bool GetNameByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::string& name);
    bool GetType(std::string& type);
    bool GetTypeByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::string& type);
    bool GetDeviceTypes(std::list<std::string>& deviceTypes);
    bool GetDeviceTypesByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes);
    bool GetOriginalModuleHash(std::string& originalModuleHash);
    bool GetOriginalModuleHashByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::string& originalModuleHash);

    bool GetHqfInfo(HqfInfo& hqfInfo);

private:
    std::unique_ptr<PtJson> root_ = nullptr;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PATCH_JSON_H
