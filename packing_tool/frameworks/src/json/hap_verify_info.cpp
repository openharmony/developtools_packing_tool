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

#include "hap_verify_info.h"

#include <iostream>

namespace OHOS {
namespace AppPackingTool {
const std::string& HapVerifyInfo::GetBundleName() const
{
    return bundleName_;
}

void HapVerifyInfo::SetBundleName(const std::string& bundleName)
{
    bundleName_ = bundleName;
}

const std::string& HapVerifyInfo::GetVendor() const
{
    return vendor_;
}

void HapVerifyInfo::SetVendor(const std::string& vendor)
{
    vendor_ = vendor;
}

const Version& HapVerifyInfo::GetVersion() const
{
    return version_;
}

void HapVerifyInfo::SetVersion(const Version& version)
{
    version_ = version;
}

const ModuleApiVersion& HapVerifyInfo::GetApiVersion() const
{
    return apiVersion_;
}

void HapVerifyInfo::SetApiVersion(const ModuleApiVersion& apiVersion)
{
    apiVersion_ = apiVersion;
}

const std::string& HapVerifyInfo::GetModuleName() const
{
    return moduleName_;
}

void HapVerifyInfo::SetModuleName(const std::string& moduleName)
{
    moduleName_ = moduleName;
}

const std::string& HapVerifyInfo::GetPackageName() const
{
    return packageName_;
}

void HapVerifyInfo::SetPackageName(const std::string& packageName)
{
    packageName_ = packageName;
}

const std::list<std::string>& HapVerifyInfo::GetAbilityNames() const
{
    return abilityNames_;
}

void HapVerifyInfo::SetAbilityNames(const std::list<std::string>& abilityNames)
{
    abilityNames_ = abilityNames;
}

void HapVerifyInfo::AddAbilityNames(const std::list<std::string>& nameList)
{
    abilityNames_.insert(abilityNames_.end(), nameList.begin(), nameList.end());
}

const DistroFilter& HapVerifyInfo::GetDistroFilter() const
{
    return distroFilter_;
}

void HapVerifyInfo::SetDistroFilter(const DistroFilter& distroFilter)
{
    distroFilter_ = distroFilter;
}

const std::list<std::string>& HapVerifyInfo::GetDeviceTypes() const
{
    return deviceTypes_;
}

void HapVerifyInfo::SetDeviceTypes(const std::list<std::string>& deviceTypes)
{
    deviceTypes_ = deviceTypes;
}

const bool& HapVerifyInfo::IsStageModule() const
{
    return isStageModule_;
}

void HapVerifyInfo::SetStageModule(const bool& isStageModule)
{
    isStageModule_ = isStageModule;
}

const std::string& HapVerifyInfo::GetModuleType() const
{
    return moduleType_;
}

void HapVerifyInfo::SetModuleType(const std::string& moduleType)
{
    moduleType_ = moduleType;
}

const bool& HapVerifyInfo::IsInstallationFree() const
{
    return isInstallationFree_;
}

void HapVerifyInfo::SetInstallationFree(const bool& isInstallationFree)
{
    isInstallationFree_ = isInstallationFree;
}

const std::list<std::string>& HapVerifyInfo::GetDependencies() const
{
    return dependencies_;
}

void HapVerifyInfo::SetDependencies(const std::list<std::string>& dependencies)
{
    dependencies_ = dependencies;
}

const std::list<DependencyItem>& HapVerifyInfo::GetDependencyItemList() const
{
    return dependencyItemList_;
}

void HapVerifyInfo::SetDependencyItemList(const std::list<DependencyItem>& dependencyItemList)
{
    dependencyItemList_ = dependencyItemList;
    ConvertToDependency();
}

const std::string& HapVerifyInfo::GetProfileStr() const
{
    return profileStr_;
}

void HapVerifyInfo::SetProfileStr(const std::string& profileStr)
{
    profileStr_ = profileStr;
}

const std::map<std::string, std::string>& HapVerifyInfo::GetResourceMap() const
{
    return resourceMap_;
}

void HapVerifyInfo::SetResourceMap(const std::map<std::string, std::string>& resourceMap)
{
    resourceMap_ = resourceMap;
}

const std::string& HapVerifyInfo::GetBundleType() const
{
    return bundleType_;
}

void HapVerifyInfo::SetBundleType(const std::string& bundleType)
{
    bundleType_ = bundleType;
}

const std::string& HapVerifyInfo::GetAtomicServiceType() const
{
    return atomicServiceType_;
}

void HapVerifyInfo::SetAtomicServiceType(const std::string& atomicServiceType)
{
    atomicServiceType_ = atomicServiceType;
}

const std::list<PreloadItem>& HapVerifyInfo::GetPreloadItems() const
{
    return preloadItems_;
}

void HapVerifyInfo::SetPreloadItems(const std::list<PreloadItem>& preloadItems)
{
    preloadItems_ = preloadItems;
}

const std::string& HapVerifyInfo::GetTargetBundleName() const
{
    return targetBundleName_;
}

void HapVerifyInfo::SetTargetBundleName(const std::string& targetBundleName)
{
    targetBundleName_ = targetBundleName;
}

const int32_t& HapVerifyInfo::GetTargetPriority() const
{
    return targetPriority_;
}

void HapVerifyInfo::SetTargetPriority(const int32_t& priority)
{
    targetPriority_ = priority;
}

const std::string& HapVerifyInfo::GetTargetModuleName() const
{
    return targetModuleName_;
}

void HapVerifyInfo::SetTargetModuleName(const std::string& targetModuleName)
{
    targetModuleName_ = targetModuleName;
}

const int32_t& HapVerifyInfo::GetTargetModulePriority() const
{
    return targetModulePriority_;
}

void HapVerifyInfo::SetTargetModulePriority(const int32_t& priority)
{
    targetModulePriority_ = priority;
}

const int64_t& HapVerifyInfo::GetFileLength() const
{
    return fileLength_;
}

void HapVerifyInfo::SetFileLength(const int64_t& fileLength)
{
    fileLength_ = fileLength;
}

const int32_t& HapVerifyInfo::GetEntrySizeLimit() const
{
    return entrySizeLimit_;
}

void HapVerifyInfo::SetEntrySizeLimit(const int32_t& limit)
{
    entrySizeLimit_ = limit;
}

const int32_t& HapVerifyInfo::GetNotEntrySizeLimit() const
{
    return notEntrySizeLimit_;
}

void HapVerifyInfo::SetNotEntrySizeLimit(const int32_t& notEntrySizeLimit)
{
    notEntrySizeLimit_ = notEntrySizeLimit;
}

const int32_t& HapVerifyInfo::GetSumSizeLimit() const
{
    return sumSizeLimit_;
}

void HapVerifyInfo::SetSumSizeLimit(const int32_t& sumSizeLimit)
{
    sumSizeLimit_ = sumSizeLimit;
}

const bool& HapVerifyInfo::IsDebug() const
{
    return debug_;
}

void HapVerifyInfo::SetDebug(const bool& debug)
{
    debug_ = debug;
}

const std::string& HapVerifyInfo::GetCompileSdkVersion() const
{
    return compileSdkVersion_;
}

void HapVerifyInfo::SetCompileSdkVersion(const std::string& compileSdkVersion)
{
    compileSdkVersion_ = compileSdkVersion;
}

const std::string& HapVerifyInfo::GetCompileSdkType() const
{
    return compileSdkType_;
}

void HapVerifyInfo::SetCompileSdkType(const std::string& compileSdkType)
{
    compileSdkType_ = compileSdkType;
}

const std::list<std::string>& HapVerifyInfo::GetProxyDataUris() const
{
    return proxyDataUris_;
}

void HapVerifyInfo::SetProxyDataUris(const std::list<std::string>& proxyDataUris)
{
    proxyDataUris_ = proxyDataUris;
}

const std::map<std::string, std::list<std::string>>& HapVerifyInfo::GetContinueTypeMap() const
{
    return continueTypeMap_;
}

void HapVerifyInfo::SetContinueTypeMap(const std::map<std::string, std::list<std::string>>& continueTypeMap)
{
    continueTypeMap_ = continueTypeMap;
}

const MultiAppMode& HapVerifyInfo::GetMultiAppMode() const
{
    return multiAppMode_;
}

void HapVerifyInfo::SetMultiAppMode(const MultiAppMode& multiAppMode)
{
    multiAppMode_ = multiAppMode;
}

const std::list<std::string>& HapVerifyInfo::GetAssetAccessGroups() const
{
    return assetAccessGroups_;
}

void HapVerifyInfo::SetAssetAccessGroups(const std::list<std::string>& assetAccessGroups)
{
    assetAccessGroups_ = assetAccessGroups;
}

void HapVerifyInfo::ConvertToDependency()
{
    for (DependencyItem item : dependencyItemList_) {
        if (!item.bundleName.empty() && item.bundleName.compare(bundleName_) == 0) {
            dependencies_.emplace_back(item.bundleName);
        }
    }
}

const std::string& HapVerifyInfo::GetFileType() const
{
    return fileType_;
}

void HapVerifyInfo::SetFileType(const std::string& fileType)
{
    fileType_ = fileType;
}
} // namespace AppPackingTool
} // namespace OHOS
