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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACK_INFO_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACK_INFO_H

#include <string>
#include <list>
#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {
struct PackInfoVersion {
    int code = -1;
    std::string name = "";
};

class PackInfo {
public:
    PackInfo() {};
    virtual ~PackInfo() {};

    bool ParseFromString(const std::string& jsonString);
    bool ParseFromFile(const std::string& jsonFile);
    std::string ToString();
    void Release();

    bool IsValid();

    // object funcs
    bool GetSummaryObject(std::unique_ptr<PtJson>& summaryObj);
    bool GetPackagesObject(std::unique_ptr<PtJson>& packagesObj);
    bool GetAppObject(std::unique_ptr<PtJson>& appObj);
    bool GetModulesObject(std::unique_ptr<PtJson>& modulesObj);
    bool GetVersionObject(std::unique_ptr<PtJson>& versionObj);
    bool GetDistroObject(int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj);
    bool GetDistroObjectByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
        int32_t moduleIndex, std::unique_ptr<PtJson>& distroObj);
    bool GetDistroObjectByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
        std::unique_ptr<PtJson>& distroObj);
    bool GetPackageObject(int32_t packageIndex, std::unique_ptr<PtJson>& packageObj);
    bool GetExtensionAbilitiesObj(int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj);
    bool GetExtensionAbilitiesObjByModulesObj(const std::unique_ptr<PtJson>& modulesObj,
        int32_t moduleIndex, std::unique_ptr<PtJson>& extensionAbilitiesObj);
    bool GetExtensionAbilitiesObjByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
        std::unique_ptr<PtJson>& extensionAbilitiesObj);
    bool GetFormsObjByExtensionAbilityObj(const std::unique_ptr<PtJson>& extensionAbilityObj,
        std::unique_ptr<PtJson>& formsObj);

    // funcs
    bool GetBundleName(std::string& bundleName);
    bool GetBundleNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleName);
    bool SetBundleName(const std::string& bundleName);
    bool GetBundleType(std::string& bundleType, const std::string& defaultBundleType);
    bool GetBundleTypeByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleType,
        const std::string& defaultBundleType);
    bool GetVersion(PackInfoVersion& version);
    bool GetVersionByVersionObj(const std::unique_ptr<PtJson>& appObj, PackInfoVersion& version);
    bool SetVersionCode(const int& versionCode);
    bool SetVersionName(const std::string& versionName);
    bool GetModuleNameByDistroObj(const std::unique_ptr<PtJson>& distroObj, std::string& moduleName);
    bool GetNameByPackageObj(const std::unique_ptr<PtJson>& packageObj, std::string& name);
    bool GetNameByFormObj(const std::unique_ptr<PtJson>& formObj, std::string& name);
    bool GetDefaultDimensionByFormObj(const std::unique_ptr<PtJson>& formObj, std::string& defaultDimension);
    bool GetSupportDimensionsByFormObj(const std::unique_ptr<PtJson>& formObj,
        std::list<std::string>& supportDimensions);
    bool GetFormNames(std::list<std::string>& formNames, std::list<std::string>& formFullNames);
    bool GetFormNamesByExtensionAbilitiesObj(const std::unique_ptr<PtJson>& extensionAbilitiesObj,
        std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames);
    bool GetFormNamesByFormsObj(const std::unique_ptr<PtJson>& formsObj,
        std::string moduleName, std::list<std::string>& formNames, std::list<std::string>& formFullNames);

private:
    std::unique_ptr<PtJson> root_ = nullptr;
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACK_INFO_H
