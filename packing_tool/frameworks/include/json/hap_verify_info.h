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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_INFO_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_INFO_H

#include <string>
#include <list>
#include <map>
#include "version.h"
#include "module_api_version.h"
#include "distro_filter.h"
#include "dependency_item.h"
#include "preload_item.h"
#include "multi_app_mode.h"

namespace OHOS {
namespace AppPackingTool {
class HapVerifyInfo {
public:
    HapVerifyInfo() {};
    virtual ~HapVerifyInfo() {};

    const std::string& GetBundleName() const;
    void SetBundleName(const std::string& bundleName);
    const std::string& GetVendor() const;
    void SetVendor(const std::string& vendor);
    const Version& GetVersion() const;
    void SetVersion(const Version& version);
    const ModuleApiVersion& GetApiVersion() const;
    void SetApiVersion(const ModuleApiVersion& apiVersion);
    const std::string& GetModuleName() const;
    void SetModuleName(const std::string& moduleName);
    const std::string& GetPackageName() const;
    void SetPackageName(const std::string& packageName);
    const std::list<std::string>& GetAbilityNames() const;
    void SetAbilityNames(const std::list<std::string>& abilityNames);
    void AddAbilityNames(const std::list<std::string>& nameList);
    const DistroFilter& GetDistroFilter() const;
    void SetDistroFilter(const DistroFilter& distroFilter);
    const std::list<std::string>& GetDeviceTypes() const;
    void SetDeviceTypes(const std::list<std::string>& deviceTypes);
    const bool& IsStageModule() const;
    void SetStageModule(const bool& isStageModule);
    const std::string& GetModuleType() const;
    void SetModuleType(const std::string& moduleType);
    const bool& IsInstallationFree() const;
    void SetInstallationFree(const bool& isInstallationFree);
    const std::list<std::string>& GetDependencies() const;
    void SetDependencies(const std::list<std::string>& dependencies);
    const std::list<DependencyItem>& GetDependencyItemList() const;
    void SetDependencyItemList(const std::list<DependencyItem>& dependencyItemList);
    const std::string& GetProfileStr() const;
    void SetProfileStr(const std::string& profileStr);
    const std::map<std::string, std::string>& GetResourceMap() const;
    void SetResourceMap(const std::map<std::string, std::string>& resourceMap);
    const std::string& GetBundleType() const;
    void SetBundleType(const std::string& bundleType);
    const std::string& GetAtomicServiceType() const;
    void SetAtomicServiceType(const std::string& atomicServiceType);
    const std::list<PreloadItem>& GetPreloadItems() const;
    void SetPreloadItems(const std::list<PreloadItem>& preloadItems);
    const std::string& GetTargetBundleName() const;
    void SetTargetBundleName(const std::string& targetBundleName);
    const int& GetTargetPriority() const;
    void SetTargetPriority(const int& priority);
    const std::string& GetTargetModuleName() const;
    void SetTargetModuleName(const std::string& targetModuleName);
    const int& GetTargetModulePriority() const;
    void SetTargetModulePriority(const int& priority);
    const int64_t& GetFileLength() const;
    void SetFileLength(const int64_t& fileLength);
    const int& GetEntrySizeLimit() const;
    void SetEntrySizeLimit(const int& limit);
    const int& GetNotEntrySizeLimit() const;
    void SetNotEntrySizeLimit(const int& notEntrySizeLimit);
    const int& GetSumSizeLimit() const;
    void SetSumSizeLimit(const int& sumSizeLimit);
    const bool& IsDebug() const;
    void SetDebug(const bool& debug);
    const std::string& GetCompileSdkVersion() const;
    void SetCompileSdkVersion(const std::string& compileSdkVersion);
    const std::string& GetCompileSdkType() const;
    void SetCompileSdkType(const std::string& compileSdkType);
    const std::list<std::string>& GetProxyDataUris() const;
    void SetProxyDataUris(const std::list<std::string>& proxyDataUris);
    const std::map<std::string, std::list<std::string>>& GetContinueTypeMap() const;
    void SetContinueTypeMap(const std::map<std::string, std::list<std::string>>& continueTypeMap);
    const MultiAppMode& GetMultiAppMode() const;
    void SetMultiAppMode(const MultiAppMode& multiAppMode);

private:
    void ConvertToDependency();

private:
    std::string bundleName_ = "";
    std::string vendor_ = "";
    Version version_;
    ModuleApiVersion apiVersion_;
    std::string moduleName_ = "";
    std::string packageName_ = "";
    std::list<std::string> abilityNames_;
    DistroFilter distroFilter_;
    std::list<std::string> deviceTypes_;
    bool isStageModule_ = true;
    std::string moduleType_ = "";
    std::string atomicServiceType_ = "";
    bool isInstallationFree_ = false;
    std::list<std::string> dependencies_;
    std::list<DependencyItem> dependencyItemList_;
    // json file content string
    std::string profileStr_ = "";
    std::map<std::string, std::string> resourceMap_;
    std::string bundleType_ = "app";
    std::string targetBundleName_ = "";
    int targetPriority_ = 0;
    std::string targetModuleName_ = "";
    int targetModulePriority_ = 0;
    std::list<PreloadItem> preloadItems_;
    // json file size
    int64_t fileLength_ = 0L;
    int entrySizeLimit_ = 2;
    int notEntrySizeLimit_ = 2;
    int sumSizeLimit_ = 10;
    bool debug_ = false;
    std::string compileSdkVersion_ = "";
    std::string compileSdkType_ = "";
    std::list<std::string> proxyDataUris_;
    std::map<std::string, std::list<std::string>> continueTypeMap_;
    MultiAppMode multiAppMode_;
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_VERIFY_INFO_H
