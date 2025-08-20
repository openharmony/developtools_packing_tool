/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_H

#include <filesystem>
#include <list>
#include <memory>
#include <string>

#include "hap_verify_info.h"
#include "module_api_version.h"
#include "module_metadata_info.h"
#include "normalize_version.h"
#include "pt_json.h"
#include "version.h"

namespace OHOS {
namespace AppPackingTool {
class ModuleJson {
public:
    ModuleJson() {};
    virtual ~ModuleJson() {};

    bool ParseFromString(const std::string& jsonString);
    bool ParseFromFile(const std::string& jsonFile);
    std::string ToString();
    void Release();

    bool IsValid();

    // object funcs
    bool GetAppObject(std::unique_ptr<PtJson>& appObj);
    bool GetDeviceConfigObject(std::unique_ptr<PtJson>& deviceConfigObj);
    bool GetVersionObject(std::unique_ptr<PtJson>& versionObj);
    bool GetModuleObject(std::unique_ptr<PtJson>& moduleObj);
    bool GetDistroObject(std::unique_ptr<PtJson>& distroObj);
    bool GetDistroObjectByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::unique_ptr<PtJson>& distroObj);
    bool GetApiVersionObject(std::unique_ptr<PtJson>& apiVersionObj);

    // stage funcs, module.json
    bool SetStageBundleName(const std::string& bundleName);
    bool SetStageMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode);
    bool SetStageMinAPIVersion(const int32_t& minAPIVersion);
    bool SetStageTargetAPIVersion(const int32_t& targetAPIVersion);
    bool SetStageApiReleaseType(const std::string& apiReleaseType);
    bool SetStageBundleType(const std::string& bundleType);
    bool SetStageInstallationFree(const bool& installationFree);
    bool SetStageDeliveryWithInstall(const bool& deliveryWithInstall);
    bool SetStageDeviceTypes(const std::list<std::string>& deviceType);

    // fa funcs, module.json
    bool SetFaBundleName(const std::string& bundleName);
    bool SetFaMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode);
    bool SetFaMinAPIVersion(const int32_t& minAPIVersion);
    bool SetFaTargetAPIVersion(const int32_t& targetAPIVersion);
    bool SetFaApiReleaseType(const std::string& apiReleaseType);
    bool SetFaBundleType(const std::string& bundleType);
    bool SetFaInstallationFree(const bool& installationFree);
    bool SetFaDeliveryWithInstall(const bool& deliveryWithInstall);
    bool SetFaDeviceTypes(const std::list<std::string>& deviceType);

    bool GetStageVersion(Version& version);
    bool GetStageVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version);
    bool SetStageVersionCode(const int32_t& versionCode);
    bool SetStageVersionName(const std::string& versionName);
    bool GetStageModuleApiVersion(ModuleApiVersion& moduleApiVersion);
    bool GetStageModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion);
    bool GetStageModuleName(std::string& moduleName);
    bool GetStageModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleName);
    bool GetStageEntry(std::list<std::string>& deviceTypes);
    bool GetStageDeviceTypes(std::list<std::string>& deviceTypes);
    bool GetStageDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes);
    bool GetStageHapVerifyInfo(HapVerifyInfo& hapVerifyInfo);
    bool GetStageDistroFilter(DistroFilter& distroFilter,
        const std::map<std::string, std::string>& resourceMap);
    bool GetStageDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj,
        const std::map<std::string, std::string>& resourceMap, DistroFilter& distroFilter);
    bool GetStageInstallationFree(bool& installationFree);
    bool GetStageInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree);
    bool GetStageBundleType(std::string& bundleType);
    bool GetStageCompileSdkType(std::string& compileSdkType);
    bool GetStageCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType);
    bool GetStageCompileSdkVersion(std::string& compileSdkVersion);
    bool GetStageCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion);
    bool GetStageModuleType(std::string& moduleType);
    bool GetStageModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType);
    bool GetStageDebug(bool& debug);
    bool GetStageDebugByAppObj(std::unique_ptr<PtJson>& appObj, bool& debug);
    bool GetStageAsanEnabled(bool& asanEnabled);
    bool GetStageAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled);
    bool GetStageTsanEnabled(bool& tsanEnabled);
    bool GetStageTsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& tsanEnabled);
    bool GetStageApiReleaseType(std::string& apiReleaseType);
    bool GetStageApiReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& apiReleaseType);
    bool GetStageCompressNativeLibs(bool& compressNativeLibs);
    bool GetStageCompressNativeLibsByAppObj(std::unique_ptr<PtJson>& appObj, bool& compressNativeLibs);
    bool GetAbilityContinueTypeMap(std::map<std::string, std::list<std::string>>& abilityContinueTypeMap);
    bool GetAbilityContinueTypeMapByModuleObj(std::unique_ptr<PtJson>& moduleObj,
        std::map<std::string, std::list<std::string>>& abilityContinueTypeMap);
    bool GetExtensionAbilityNames(std::list<std::string>& extensionAbilityNames);
    bool GetExtensionAbilityNamesByModuleObj(std::unique_ptr<PtJson>& moduleObj,
        std::list<std::string>& extensionAbilityNames);
    bool GetModuleMetadatas(std::list<ModuleMetadataInfo>& moduleMetadataInfos,
        const std::map<std::string, std::string>& resourceMap);
    bool GetModuleMetadatasByModuleObj(std::unique_ptr<PtJson>& moduleObj,
        const std::map<std::string, std::string>& resourceMap, std::list<ModuleMetadataInfo>& moduleMetadataInfos);
    bool ParseModuleMetadatasToDistroFilter(const std::list<ModuleMetadataInfo>& moduleMetadataInfos,
        DistroFilter& distroFilter);

    bool IsExistedStageRequestPermissions();
    bool IsExistedStageModuleTargetPriority();
    bool IsExistedStageAppTargetPriority();

    // fa funcs config.json
    bool GetFaVersion(Version& version);
    bool GetFaVersionByAppObj(std::unique_ptr<PtJson>& appObj, Version& version);
    bool GetFaVersionByVersionObj(std::unique_ptr<PtJson>& versionObj, Version& version);
    bool SetFaVersionCode(const int32_t& versionCode);
    bool SetFaVersionName(const std::string& versionName);
    bool GetFaInstallationFree(bool& installationFree);
    bool GetFaInstallationFreeByModuleObj(std::unique_ptr<PtJson>& moduleObj, bool& installationFree);
    bool GetFaInstallationFreeByDistroObj(std::unique_ptr<PtJson>& distroObj, bool& installationFree);
    bool GetFaBundleType(std::string& bundleType);
    bool GetFaModuleApiVersion(ModuleApiVersion& moduleApiVersion);
    bool GetFaModuleApiVersionByAppObj(std::unique_ptr<PtJson>& appObj, ModuleApiVersion& moduleApiVersion);
    bool GetFaModuleApiVersionByApiVersionObj(std::unique_ptr<PtJson>& apiVersionObj,
        ModuleApiVersion& moduleApiVersion);
    bool GetFaModuleName(std::string& faModuleName);
    bool GetFaModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& faModuleName);
    bool GetFaModuleNameByDistroObj(std::unique_ptr<PtJson>& distroObj, std::string& faModuleName);
    bool GetFaPackageStr(std::string& packageStr);
    bool GetFaPackageStrByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& packageStr);
    bool GetFaEntry(std::list<std::string>& deviceTypes);
    bool GetFaDeviceTypes(std::list<std::string>& deviceTypes);
    bool GetFaDeviceTypesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes);
    bool GetFaHapVerifyInfo(HapVerifyInfo& hapVerifyInfo);
    bool GetFaDistroFilter(DistroFilter& distroFilter);
    bool GetFaDistroFilterByModuleObj(std::unique_ptr<PtJson>& moduleObj, DistroFilter& distroFilter);
    bool GetFaCompileSdkType(std::string& compileSdkType);
    bool GetFaCompileSdkTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkType);
    bool GetFaCompileSdkVersion(std::string& compileSdkVersion);
    bool GetFaCompileSdkVersionByAppObj(std::unique_ptr<PtJson>& appObj, std::string& compileSdkVersion);
    bool GetFaModuleType(std::string& moduleType);
    bool GetFaModuleTypeByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleType);
    bool GetFaDebug(bool& debug);
    bool GetFaDebugByDeviceConfigObj(std::unique_ptr<PtJson>& deviceConfigObj, bool& debug);
    bool GetFaAsanEnabled(bool& asanEnabled);
    bool GetFaAsanEnabledByAppObj(std::unique_ptr<PtJson>& appObj, bool& asanEnabled);
    bool GetFaReleaseType(std::string& releaseType);
    bool GetFaReleaseTypeByAppObj(std::unique_ptr<PtJson>& appObj, std::string& releaseType);
    bool GetFaDeliveryWithInstall(bool& deliveryWithInstall);

    // common funcs
    bool GetBundleName(std::string& bundleName);
    bool GetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& bundleName);
    bool SetBundleName(const std::string& bundleName);
    bool GetModuleName(std::string& moduleName);
    bool GetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& moduleName);
    bool GetVendor(std::string& vendor);
    bool GetVendorByAppObj(std::unique_ptr<PtJson>& appObj, std::string& vendor);
    bool GetTargetBundleName(std::string& targetBundleName);
    bool GetTargetBundleNameByAppObj(std::unique_ptr<PtJson>& appObj, std::string& targetBundleName);
    bool GetTargetModuleName(std::string& targetModuleName);
    bool GetTargetModuleNameByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::string& targetModuleName);
    bool GetTargetPriority(int32_t& targetPriority);
    bool GetTargetPriorityByAppObj(std::unique_ptr<PtJson>& appObj, int32_t& targetPriority);
    bool GetTargetModulePriority(int32_t& targetModulePriority);
    bool GetTargetModulePriorityByModuleObj(std::unique_ptr<PtJson>& moduleObj, int32_t& targetModulePriority);

    // config.json or module.json
    bool GetAbilityNames(std::list<std::string>& abilityNames);
    bool GetAbilityNamesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& abilityNames);
    // unknown
    bool GetDependencyItems(std::list<DependencyItem>& dependencyItems, const std::string& defaultBundleName);
    bool GetDependencyItemsByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<DependencyItem>& dependencyItems,
        const std::string& defaultBundleName);
    bool GetAtomicServicePreloads(std::list<PreloadItem>& preloadItems);
    bool GetAtomicServicePreloadsByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<PreloadItem>& preloadItems);
    bool GetProxyDataUris(std::list<std::string>& proxyDataUris);
    bool GetProxyDataUrisByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& proxyDataUris);
    bool GetProxyDataUrisByProxyDatasObj(std::unique_ptr<PtJson>& proxyDatasObj,
        std::list<std::string>& proxyDataUris);

    bool GetAssetAccessGroups(std::list<std::string>& assetAccessGroups);
    bool GetAssetAccessGroupsByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& assetAccessGroups);

    bool GetMultiAppMode(MultiAppMode& multiAppMode);
    bool GetMultiAppModeByAppObj(std::unique_ptr<PtJson>& appObj, MultiAppMode& multiAppMode);
    bool GetMinApiVersion(int32_t& minAPIVersion);
    bool GetTargetApiVersion(int32_t& targetAPIVersion);
    bool GetDeliveryWithInstall(bool& deliveryWithInstall);
    bool GetQuerySchemes(std::list<std::string>& querySchemes);
    bool GetQuerySchemesByModuleObj(std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& querySchemes);
    bool GetQuerySchemesByArray(std::unique_ptr<PtJson>& querySchemesObj, std::list<std::string>& querySchemes);

    bool IsModuleAtomicServiceValid();
    bool CheckEntryInAtomicService();
    bool CheckAtomicServiceInstallationFree();
    bool CheckStageAsanTsanEnabledValid();
    bool CheckStageAtomicService();
    bool CheckQuerySchemes();
    bool CheckStageOverlayCfg();

    bool GetGenerateBuildHash(bool& generateBuildHash);
    bool RemoveGenerateBuildHash();
    bool RemoveGenerateBuildHashFromAppObj();
    bool RemoveGenerateBuildHashFromModuleObj();

    bool GetNormalizeVersion(NormalizeVersion& normalizeVersion, const bool& isStage = true);
    bool SetVersionCodeAndName(const int32_t& versionCode, const std::string& versionName, const bool& isStage = true);

    bool SetBuildHash(const std::string& buildHash);

    // json function for hqf
    bool GetPatchModuleName(std::string& patchModuleName);

    bool SetBundleName(const std::string& bundleName, const bool& isStage);
    bool SetMinCompatibleVersionCode(const int32_t& minCompatibleVersionCode, const bool& isStage);
    bool SetMinAPIVersion(const int32_t& minAPIVersion, const bool& isStage);
    bool SetTargetAPIVersion(const int32_t& targetAPIVersion, const bool& isStage);
    bool SetApiReleaseType(const std::string& apiReleaseType, const bool& isStage);
    bool SetBundleType(const std::string& bundleType, const bool& isStage);
    bool SetInstallationFree(const bool& installationFree, const bool& isStage);
    bool SetDeliveryWithInstall(const bool& deliveryWithInstall, const bool& isStage);
    bool SetVersionCode(const int32_t& versionCode, const bool& isStage);
    bool SetVersionName(const std::string& versionName, const bool& isStage);
    bool SetDeviceTypes(const std::list<std::string>& deviceTypes, const bool& isStage);

protected:
    bool CheckStageBundleType(const std::string& moduleName, const std::string& moduleType,
        const std::string& bundleType, const bool& installationFree);
    bool GetAbilityNameByAbilityObj(std::unique_ptr<PtJson>& abilityObj, std::string& abilityName);
    bool GetContinueTypesByAbilityObj(std::unique_ptr<PtJson>& abilityObj, std::list<std::string>& continueTypes);
    bool GetDependencyBundleNameByDependencyItemObj(std::unique_ptr<PtJson>& dependencyItemObj,
        std::string& bundleName, const std::string& defaultBundleName);
    bool GetDependencyModuleNameByDependencyItemObj(std::unique_ptr<PtJson>& dependencyItemObj,
        std::string& moduleName);
    bool GetModuleMetadataInfoByModuleMetadataInfoObj(std::unique_ptr<PtJson>& moduleMetadataInfoObj,
        const std::map<std::string, std::string>& resourceMap, ModuleMetadataInfo& moduleMetadataInfo);
    bool GetExtensionAbilityNamesByExtensionAbilityObj(std::unique_ptr<PtJson>& extensionAbilitiesObj,
        std::list<std::string>& extensionAbilityNames);
    bool GetAbilityNamesByAbilitiesObj(std::unique_ptr<PtJson>& abilitiesObj, std::list<std::string>& abilityNames);
    bool SetStageHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo);
    bool SetStageHapVerifyInfoByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo);
    bool SetStageHapVerifyInfoExtByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo);
    bool SetFaHapVerifyInfoByAppObj(std::unique_ptr<PtJson>& appObj, HapVerifyInfo& hapVerifyInfo);
    bool SetFaHapVerifyInfoByModuleObj(std::unique_ptr<PtJson>& moduleObj, HapVerifyInfo& hapVerifyInfo);

private:
    std::unique_ptr<PtJson> root_ = nullptr;
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_H
