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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQF_INFO_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQF_INFO_H

#include <string>
#include <list>
#include <map>

namespace OHOS {
namespace AppPackingTool {

class HqfInfo {
public:
    HqfInfo() {};
    virtual ~HqfInfo() {};

    const std::string& GetBundleName() const
    {
        return bundleName_;
    }

    void SetBundleName(const std::string& bundleName)
    {
        bundleName_ = bundleName;
    }

    const int32_t& GetVersionCode() const
    {
        return versionCode_;
    }

    void SetVersionCode(const int32_t& versionCode)
    {
        versionCode_ = versionCode;
    }

    const std::string& GetVersionName() const
    {
        return versionName_;
    }

    void SetVersionName(const std::string& versionName)
    {
        versionName_ = versionName;
    }

    const int32_t& GetPatchVersionCode() const
    {
        return patchVersionCode_;
    }

    void SetPatchVersionCode(const int32_t& patchVersionCode)
    {
        patchVersionCode_ = patchVersionCode;
    }

    const std::string& GetPatchVersionName() const
    {
        return patchVersionName_;
    }

    void SetPatchVersionName(const std::string& patchVersionName)
    {
        patchVersionName_ = patchVersionName;
    }

    const std::string& GetModuleName() const
    {
        return moduleName_;
    }

    void SetModuleName(const std::string& moduleName)
    {
        moduleName_ = moduleName;
    }

    const std::string& GetType() const
    {
        return type_;
    }

    void SetType(const std::string& type)
    {
        type_ = type;
    }

    const std::list<std::string>& GetDeviceTypes() const
    {
        return deviceTypes_;
    }

    void SetDeviceTypes(const std::list<std::string>& deviceTypes)
    {
        deviceTypes_ = deviceTypes;
    }

    const std::string& GetOriginalModuleHash() const
    {
        return originalModuleHash_;
    }

    void SetOriginalModuleHash(const std::string& originalModuleHash)
    {
        originalModuleHash_ = originalModuleHash;
    }

private:
    std::string bundleName_ = "";
    int32_t versionCode_ = -1;
    std::string versionName_ = "";
    int32_t patchVersionCode_ = -1;
    std::string patchVersionName_ = "";
    std::string moduleName_ = "";
    std::string type_ = "";
    std::list<std::string> deviceTypes_;
    std::string originalModuleHash_ = "";
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HQF_INFO_H
