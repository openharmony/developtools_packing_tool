/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "general_normalize.h"

#include "hap_packager.h"
#include "hqf_packager.h"
#include "hsp_packager.h"
#include "json/json_utils.h"
#include "json/general_normalize_version_utils.h"
#include "json/module_json.h"
#include "json/pack_info.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string BUNDLE_TYPE = "bundleType";
const std::string BUNDLE_NAME = "bundleName";
const std::string VERSION_CODE = "versionCode";
const std::string VERSION_NAME = "versionName";
const std::string MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
const std::string MIN_API_VERSION = "minAPIVersion";
const std::string TARGET_API_VERSION = "targetAPIVersion";
const std::string API_RELEASE_TYPE = "apiReleaseType";
const std::string DEVICE_TYPES = "deviceTypes";
const std::string INSTALLATION_FREE = "installationFree";
const std::string DELIVERY_WITH_INSTALL = "deliveryWithInstall";
const std::string MODULE_NAME = "moduleName";
const std::vector<std::string> DEVICE_TYPE_LIST = {"default", "tablet", "tv", "wearable", "car", "2in1", "phone"};
}
GeneralNormalize::GeneralNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t GeneralNormalize::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t GeneralNormalize::PreProcess()
{
    auto it = parameterMap_.find(Constants::PARAM_INPUT_LIST);
    if (it == parameterMap_.end()) {
        LOGE("--input-list is empty.");
        return ERR_INVALID_VALUE;
    }
    if (!CompatibleProcess(it->second, hspOrhapList_, Constants::HAP_SUFFIX, Constants::HSP_SUFFIX)) {
        LOGE("--input-list is invalid.");
        return ERR_INVALID_VALUE;
    }
    if (hspOrhapList_.size() == 0) {
        LOGE("--input-list is empty.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    std::regex pattern(Constants::VERSION_NAME_PATTERN);
    if (it != parameterMap_.end()) {
        if (!std::regex_match(it->second, pattern) || it->second.size() > Constants::MAX_VERSION_NAME_LENGTH) {
            LOGE("--version-name is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it != parameterMap_.end()) {
        if (!Utils::IsPositiveInteger(it->second)) {
            LOGE("--version-code is invalid.");
            return ERR_INVALID_VALUE;
        }
        try {
            int32_t versionCode = std::stoi(it->second);
            if (versionCode > Constants::MAX_VERSION_CODE) {
                LOGE("--version-code is invalid.");
                return ERR_INVALID_VALUE;
            }
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUILD_VERSION);
    std::regex buildPattern(Constants::BUILD_VERSION_PATTERN);
    if (it != parameterMap_.end()) {
        if (!std::regex_match(it->second, buildPattern) || it->second.size() > Constants::BUILD_VERSION_MAX_LENGTH) {
            LOGE("--build-version is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DEVICE_TYPES);
    if (it != parameterMap_.end()) {
        std::list<std::string> deviceTypeList;
        if (!Utils::StringToArray(it->second, deviceTypeList)) {
            LOGE("--device-types is invalid.");
            return ERR_INVALID_VALUE;
        }
        for (auto& item : deviceTypeList) {
            if ((std::find(DEVICE_TYPE_LIST.begin(), DEVICE_TYPE_LIST.end(), item) == DEVICE_TYPE_LIST.end())) {
                LOGE("--device-types is invalid.");
                return ERR_INVALID_VALUE;
            }
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_NAME);
    std::regex bundleNamePattern(Constants::BUNDLE_NAME_PATTERN);
    if (it != parameterMap_.end()) {
        if (!std::regex_match(it->second, bundleNamePattern) || it->second.length() < Constants::BUNDLE_NAME_LEN_MIN ||
            it->second.length() > Constants::BUNDLE_NAME_LEN_MAX) {
            LOGE("--bundle-name is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE);
    if (it != parameterMap_.end()) {
        if (!Utils::IsPositiveInteger(it->second)) {
            LOGE("--min-compatible-version-code is invalid.");
            return ERR_INVALID_VALUE;
        }
        try {
            int32_t minCompatibleVersionCode = std::stoi(it->second);
            if (minCompatibleVersionCode > Constants::MAX_VERSION_CODE) {
                LOGE("--min-compatible-version-code is invalid.");
                return ERR_INVALID_VALUE;
            }
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_API_VERSION);
    if (it != parameterMap_.end()) {
        if (!Utils::IsPositiveInteger(it->second)) {
            LOGE("--min-api-version is invalid.");
            return ERR_INVALID_VALUE;
        }
        try {
            int32_t minApiVersion = std::stoi(it->second);
            if (minApiVersion > Constants::MAX_VERSION_CODE) {
                LOGE("--min-api-version is invalid.");
                return ERR_INVALID_VALUE;
            }
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_TARGET_API_VERSION);
    if (it != parameterMap_.end()) {
        if (!Utils::IsPositiveInteger(it->second)) {
            LOGE("--target-api-version is invalid.");
            return ERR_INVALID_VALUE;
        }
        try {
            int32_t targetApiVersion = std::stoi(it->second);
            if (targetApiVersion > Constants::MAX_VERSION_CODE) {
                LOGE("--target-api-version is invalid.");
                return ERR_INVALID_VALUE;
            }
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_API_RELEASE_TYPE);
    std::regex releaseTypePattern(Constants::API_RELEASE_TYPE_PATTERN);
    if (it != parameterMap_.end()) {
        if (!std::regex_match(it->second, releaseTypePattern)) {
            LOGE("--api-release-type is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_TYPE);
    if (it != parameterMap_.end()) {
        if ((std::find(Constants::BUNDLE_TYPE_LIST.begin(), Constants::BUNDLE_TYPE_LIST.end(), it->second) ==
            Constants::BUNDLE_TYPE_LIST.end())) {
            LOGE("--bundle-type is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_INSTALLATION_FREE);
    if (it != parameterMap_.end()) {
        if (it->second != "true" && it->second != "false") {
            LOGE("--installation-free is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DELIVERY_WITH_INSTALL);
    if (it != parameterMap_.end()) {
        if (it->second != "true" && it->second != "false") {
            LOGE("--delivery-with-install is invalid.");
            return ERR_INVALID_VALUE;
        }
    }

    if (!IsOutDirectoryValid()) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

bool GeneralNormalize::ModifyModuleJson(const std::string &moduleJsonPath,
    GeneralNormalizeVersion &generalNormalizeVersion, std::string &bundleName, std::string &moduleName)
{
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(moduleJsonPath)) {
        LOGE("Parse and modify module.json failed, parse module.json is null.");
        return false;
    }
    auto it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it != parameterMap_.end()) {
        Version version;
        moduleJson.GetStageVersion(version);
        generalNormalizeVersion.originVersionCode = version.versionCode;
        int32_t versionCode = 0;
        try {
            versionCode = std::stoi(parameterMap_.at(Constants::PARAM_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!moduleJson.SetVersionCode(versionCode, true)) {
            LOGE("SetVersionCode failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUILD_VERSION);
    if (it != parameterMap_.end()) {
        std::string buildVersion = parameterMap_.at(Constants::PARAM_BUILD_VERSION);
        Version version;
        moduleJson.GetStageVersion(version);
        generalNormalizeVersion.originBuildVersion = version.buildVersion;
        if (!moduleJson.SetBuildVersion(buildVersion)) {
            LOGE("SetBuildVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    if (it != parameterMap_.end()) {
        std::string versionName = parameterMap_.at(Constants::PARAM_VERSION_NAME);
        Version version;
        moduleJson.GetStageVersion(version);
        generalNormalizeVersion.originVersionName = version.versionName;
        if (!moduleJson.SetVersionName(versionName, true)) {
            LOGE("SetVersionName failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_NAME);
    if (it != parameterMap_.end()) {
        std::string targetBundleName = parameterMap_.at(Constants::PARAM_BUNDLE_NAME);
        std::string originBundleName;
        moduleJson.GetBundleName(originBundleName);
        generalNormalizeVersion.originBundleName = originBundleName;
        if (!moduleJson.SetBundleName(targetBundleName, true)) {
            LOGE("SetBundleName failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE);
    if (it != parameterMap_.end()) {
        Version version;
        moduleJson.GetStageVersion(version);
        generalNormalizeVersion.originMinCompatibleVersionCode = version.minCompatibleVersionCode;
        int32_t minCompatibleVersionCode = 0;
        try {
            minCompatibleVersionCode = std::stoi(parameterMap_.at(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!moduleJson.SetMinCompatibleVersionCode(minCompatibleVersionCode, true)) {
            LOGE("SetMinCompatibleVersionCode failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_API_VERSION);
    if (it != parameterMap_.end()) {
        int32_t originMinAPIVersion = -1;
        moduleJson.GetMinApiVersion(originMinAPIVersion);
        generalNormalizeVersion.originMinAPIVersion = originMinAPIVersion;
        int32_t minAPIVersion = 0;
        try {
            minAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_MIN_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!moduleJson.SetMinAPIVersion(minAPIVersion, true)) {
            LOGE("SetMinAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_TARGET_API_VERSION);
    if (it != parameterMap_.end()) {
        int32_t originTargetAPIVersion = -1;
        moduleJson.GetTargetApiVersion(originTargetAPIVersion);
        generalNormalizeVersion.originTargetAPIVersion = originTargetAPIVersion;
        int32_t targetAPIVersion = 0;
        try {
            targetAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_TARGET_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!moduleJson.SetTargetAPIVersion(targetAPIVersion, true)) {
            LOGE("SetTargetAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_API_RELEASE_TYPE);
    if (it != parameterMap_.end()) {
        std::string apiReleaseType = parameterMap_.at(Constants::PARAM_API_RELEASE_TYPE);
        std::string originApiReleaseType;
        moduleJson.GetStageApiReleaseType(originApiReleaseType);
        generalNormalizeVersion.originApiReleaseType = originApiReleaseType;
        if (!moduleJson.SetApiReleaseType(apiReleaseType, true)) {
            LOGE("SetApiReleaseType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_TYPE);
    if (it != parameterMap_.end()) {
        std::string bundleType = parameterMap_.at(Constants::PARAM_BUNDLE_TYPE);
        std::string originBundleType;
        moduleJson.GetStageBundleType(originBundleType);
        generalNormalizeVersion.originBundleType = originBundleType;
        if (!moduleJson.SetBundleType(bundleType, true)) {
            LOGE("SetBundleType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_INSTALLATION_FREE);
    if (it != parameterMap_.end()) {
        bool installationFree = Utils::StringToBool(parameterMap_.at(Constants::PARAM_INSTALLATION_FREE));
        bool originInstallationFree;
        moduleJson.GetStageInstallationFree(originInstallationFree);
        generalNormalizeVersion.originInstallationFree = originInstallationFree;
        generalNormalizeVersion.modifyInstallationFree = true;
        if (!moduleJson.SetInstallationFree(installationFree, true)) {
            LOGE("SetInstallationFree failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DELIVERY_WITH_INSTALL);
    if (it != parameterMap_.end()) {
        bool deliveryWithInstall = Utils::StringToBool(parameterMap_.at(Constants::PARAM_DELIVERY_WITH_INSTALL));
        bool originDeliveryWithInstall;
        moduleJson.GetDeliveryWithInstall(originDeliveryWithInstall);
        generalNormalizeVersion.originDeliveryWithInstall = originDeliveryWithInstall;
        generalNormalizeVersion.modifyDeliveryWithInstall = true;
        if (!moduleJson.SetDeliveryWithInstall(deliveryWithInstall, true)) {
            LOGE("SetDeliveryWithInstall failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DEVICE_TYPES);
    if (it != parameterMap_.end()) {
        std::list<std::string> deviceTypes;
        Utils::StringToArray(parameterMap_.at(Constants::PARAM_DEVICE_TYPES), deviceTypes);
        std::list<std::string> originDeviceTypes;
        moduleJson.GetStageDeviceTypes(originDeviceTypes);
        generalNormalizeVersion.originDeviceTypes = originDeviceTypes;
        if (!moduleJson.SetDeviceTypes(deviceTypes, true)) {
            LOGE("SetDeviceTypes failed");
            return false;
        }
    }

    moduleJson.GetBundleName(bundleName);
    moduleJson.GetModuleName(moduleName);
    generalNormalizeVersion.moduleName = moduleName;
    if (!JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath)) {
        LOGE("Parse and modify module.json failed, write Json failed.");
        return false;
    }
    return true;
}

bool GeneralNormalize::ModifyConfigJson(const std::string &configJsonPath,
    GeneralNormalizeVersion &generalNormalizeVersion, std::string &bundleName, std::string &moduleName)
{
    ModuleJson configJson;
    if (!configJson.ParseFromFile(configJsonPath)) {
        LOGE("Parse and modify config.json failed, parse json is null.");
        return false;
    }
    auto it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it != parameterMap_.end()) {
        Version version;
        configJson.GetFaVersion(version);
        generalNormalizeVersion.originVersionCode = version.versionCode;
        int32_t versionCode = 0;
        try {
            versionCode = std::stoi(parameterMap_.at(Constants::PARAM_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!configJson.SetVersionCode(versionCode, false)) {
            LOGE("SetVersionCode failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    if (it != parameterMap_.end()) {
        Version version;
        configJson.GetFaVersion(version);
        generalNormalizeVersion.originVersionName = version.versionName;
        std::string versionName = parameterMap_.at(Constants::PARAM_VERSION_NAME);
        if (!configJson.SetVersionName(versionName, false)) {
            LOGE("SetVersionName failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_NAME);
    if (it != parameterMap_.end()) {
        std::string originBundleName;
        configJson.GetBundleName(originBundleName);
        generalNormalizeVersion.originBundleName = originBundleName;
        std::string targetBundleName = parameterMap_.at(Constants::PARAM_BUNDLE_NAME);
        if (!configJson.SetBundleName(targetBundleName, false)) {
            LOGE("SetBundleName failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE);
    if (it != parameterMap_.end()) {
        Version version;
        configJson.GetFaVersion(version);
        generalNormalizeVersion.originMinCompatibleVersionCode = version.minCompatibleVersionCode;
        int32_t minCompatibleVersionCode = 0;
        try {
            minCompatibleVersionCode = std::stoi(parameterMap_.at(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!configJson.SetMinCompatibleVersionCode(minCompatibleVersionCode, false)) {
            LOGE("SetMinCompatibleVersionCode failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_API_VERSION);
    if (it != parameterMap_.end()) {
        ModuleApiVersion moduleApiVersion;
        configJson.GetFaModuleApiVersion(moduleApiVersion);
        generalNormalizeVersion.originMinAPIVersion = moduleApiVersion.compatibleApiVersion;
        int32_t minAPIVersion = 0;
        try {
            minAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_MIN_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!configJson.SetMinAPIVersion(minAPIVersion, false)) {
            LOGE("SetMinAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_TARGET_API_VERSION);
    if (it != parameterMap_.end()) {
        ModuleApiVersion moduleApiVersion;
        configJson.GetFaModuleApiVersion(moduleApiVersion);
        generalNormalizeVersion.originTargetAPIVersion = moduleApiVersion.targetApiVersion;
        int32_t targetAPIVersion = 0;
        try {
            targetAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_TARGET_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!configJson.SetTargetAPIVersion(targetAPIVersion, false)) {
            LOGE("SetTargetAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_API_RELEASE_TYPE);
    if (it != parameterMap_.end()) {
        ModuleApiVersion moduleApiVersion;
        configJson.GetFaModuleApiVersion(moduleApiVersion);
        generalNormalizeVersion.originApiReleaseType = moduleApiVersion.releaseType;
        std::string apiReleaseType = parameterMap_.at(Constants::PARAM_API_RELEASE_TYPE);
        if (!configJson.SetApiReleaseType(apiReleaseType, false)) {
            LOGE("SetApiReleaseType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_TYPE);
    if (it != parameterMap_.end()) {
        std::string originBundleType;
        configJson.GetFaBundleType(originBundleType);
        generalNormalizeVersion.originBundleType = originBundleType;
        std::string bundleType = parameterMap_.at(Constants::PARAM_BUNDLE_TYPE);
        if (!configJson.SetBundleType(bundleType, false)) {
            LOGE("SetBundleType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_INSTALLATION_FREE);
    if (it != parameterMap_.end()) {
        bool originInstallationFree;
        configJson.GetFaInstallationFree(originInstallationFree);
        generalNormalizeVersion.originInstallationFree = originInstallationFree;
        generalNormalizeVersion.modifyInstallationFree = true;
        bool installationFree = Utils::StringToBool(parameterMap_.at(Constants::PARAM_INSTALLATION_FREE));
        if (!configJson.SetInstallationFree(installationFree, false)) {
            LOGE("SetInstallationFree failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DELIVERY_WITH_INSTALL);
    if (it != parameterMap_.end()) {
        bool originDeliveryWithInstall;
        configJson.GetFaDeliveryWithInstall(originDeliveryWithInstall);
        generalNormalizeVersion.originDeliveryWithInstall = originDeliveryWithInstall;
        generalNormalizeVersion.modifyDeliveryWithInstall = true;
        bool deliveryWithInstall = Utils::StringToBool(parameterMap_.at(Constants::PARAM_DELIVERY_WITH_INSTALL));
        if (!configJson.SetDeliveryWithInstall(deliveryWithInstall, false)) {
            LOGE("SetDeliveryWithInstall failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DEVICE_TYPES);
    if (it != parameterMap_.end()) {
        std::list<std::string> originDeviceTypes;
        configJson.GetFaDeviceTypes(originDeviceTypes);
        generalNormalizeVersion.originDeviceTypes = originDeviceTypes;
        std::list<std::string> deviceTypes;
        Utils::StringToArray(parameterMap_.at(Constants::PARAM_DEVICE_TYPES), deviceTypes);
        if (!configJson.SetDeviceTypes(deviceTypes, false)) {
            LOGE("SetDeviceTypes failed");
            return false;
        }
    }

    configJson.GetBundleName(bundleName);
    configJson.GetFaModuleName(moduleName);
    generalNormalizeVersion.moduleName = moduleName;
    if (!JsonUtils::StrToFile(configJson.ToString(), configJsonPath)) {
        LOGE("Parse and modify config.json failed, writeJson failed.");
        return false;
    }
    return true;
}

bool GeneralNormalize::ModifyPackInfo(const std::string &packInfoPath)
{
    PackInfo packInfo;
    if (!packInfo.ParseFromFile(packInfoPath)) {
        LOGW("Parse and modify packInfo failed, json format invalid.");
        return false;
    }

    auto it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it != parameterMap_.end()) {
        int32_t versionCode = 0;
        try {
            versionCode = std::stoi(parameterMap_.at(Constants::PARAM_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!packInfo.SetVersionCode(versionCode)) {
            LOGW("SetVersionCode packInfo failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUILD_VERSION);
    if (it != parameterMap_.end()) {
        std::string buildVersion = parameterMap_.at(Constants::PARAM_BUILD_VERSION);
        if (!packInfo.SetBuildVersion(buildVersion)) {
            LOGW("SetBuildVersion packInfo failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    if (it != parameterMap_.end()) {
        std::string versionName = parameterMap_.at(Constants::PARAM_VERSION_NAME);
        if (!packInfo.SetVersionName(versionName)) {
            LOGW("SetVersionName packInfo failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_NAME);
    if (it != parameterMap_.end()) {
        std::string bundleName = parameterMap_.at(Constants::PARAM_BUNDLE_NAME);
        if (!packInfo.SetBundleName(bundleName)) {
            LOGW("SetBundleName failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE);
    if (it != parameterMap_.end()) {
        int32_t minCompatibleVersionCode = 0;
        try {
            minCompatibleVersionCode = std::stoi(parameterMap_.at(Constants::PARAM_MIN_COMPATIBLE_VERSION_CODE));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!packInfo.SetMinCompatibleVersionCode(minCompatibleVersionCode)) {
            LOGW("SetMinCompatibleVersionCode failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_MIN_API_VERSION);
    if (it != parameterMap_.end()) {
        int32_t minAPIVersion = 0;
        try {
            minAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_MIN_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!packInfo.SetMinAPIVersion(minAPIVersion)) {
            LOGW("SetMinAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_TARGET_API_VERSION);
    if (it != parameterMap_.end()) {
        int32_t targetAPIVersion = 0;
        try {
            targetAPIVersion = std::stoi(parameterMap_.at(Constants::PARAM_TARGET_API_VERSION));
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
        if (!packInfo.SetTargetAPIVersion(targetAPIVersion)) {
            LOGW("SetTargetAPIVersion failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_API_RELEASE_TYPE);
    if (it != parameterMap_.end()) {
        std::string apiReleaseType = parameterMap_.at(Constants::PARAM_API_RELEASE_TYPE);
        if (!packInfo.SetApiReleaseType(apiReleaseType)) {
            LOGW("SetApiReleaseType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_TYPE);
    if (it != parameterMap_.end()) {
        std::string bundleType = parameterMap_.at(Constants::PARAM_BUNDLE_TYPE);
        if (!packInfo.SetBundleType(bundleType)) {
            LOGW("SetBundleType failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_INSTALLATION_FREE);
    if (it != parameterMap_.end()) {
        bool installationFree = Utils::StringToBool(parameterMap_.at(Constants::PARAM_INSTALLATION_FREE));
        if (!packInfo.SetInstallationFree(installationFree)) {
            LOGW("SetInstallationFree failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DELIVERY_WITH_INSTALL);
    if (it != parameterMap_.end()) {
        bool deliveryWithInstall = Utils::StringToBool(parameterMap_.at(Constants::PARAM_DELIVERY_WITH_INSTALL));
        if (!packInfo.SetDeliveryWithInstall(deliveryWithInstall)) {
            LOGW("SetDeliveryWithInstall failed");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_DEVICE_TYPES);
    if (it != parameterMap_.end()) {
        std::list<std::string> deviceTypes;
        Utils::StringToArray(parameterMap_.at(Constants::PARAM_DEVICE_TYPES), deviceTypes);
        if (!packInfo.SetDeviceTypes(deviceTypes)) {
            LOGW("SetDeviceTypes failed");
            return false;
        }
    }

    if (!JsonUtils::StrToFile(packInfo.ToString(), packInfoPath)) {
        LOGW("Parse and modify packInfo failed, writeJson failed.");
        return false;
    }
    return true;
}

bool GeneralNormalize::CompressDirToHap(const std::string &sourceDir, const std::string &zipFilePath)
{
    std::map<std::string, std::string> parameterMap;
    std::string resultReceiver = "";
    parameterMap[Constants::PARAM_OUT_PATH] = zipFilePath;
    std::unique_ptr<Packager> packager = nullptr;
    for (const auto& entry : fs::directory_iterator(fs::path(sourceDir))) {
        std::string fileName = entry.path().filename().string();
        std::string filePath = entry.path().string();
        if (fileName == Constants::ETS_PATH) {
            parameterMap[Constants::PARAM_ETS_PATH] = filePath;
        } else if (fileName == Constants::HNP_PATH) {
            parameterMap[Constants::PARAM_HNP_PATH] = filePath;
        } else if (fileName == Constants::LIB_PATH) {
            parameterMap[Constants::PARAM_LIB_PATH] = filePath;
        } else if (fileName == Constants::AN_PATH) {
            parameterMap[Constants::PARAM_AN_PATH] = filePath;
        } else if (fileName == Constants::AP_PATH) {
            parameterMap[Constants::PARAM_AP_PATH] = filePath;
        } else if (fileName == Constants::RESOURCES_PATH) {
            parameterMap[Constants::PARAM_RESOURCES_PATH] = filePath;
        } else if (fileName == Constants::JS_PATH) {
            parameterMap[Constants::PARAM_JS_PATH] = filePath;
        } else if (fileName == Constants::ASSETS_PATH) {
            parameterMap[Constants::PARAM_ASSETS_PATH] = filePath;
        } else if (fileName == Constants::SO_DIR) {
            parameterMap[Constants::PARAM_MAPLE_SO_DIR] = filePath;
        } else if (fileName == Constants::SHARED_LIBS_DIR) {
            parameterMap[Constants::PARAM_SHAREDLIBS_PATH] = filePath;
        } else if (fileName == Constants::CONFIG_JSON) {
            parameterMap[Constants::PARAM_JSON_PATH] = filePath;
        } else if (fileName == Constants::MODULE_JSON) {
            parameterMap[Constants::PARAM_JSON_PATH] = filePath;
        } else if (fileName == Constants::RESOURCES_INDEX) {
            parameterMap[Constants::PARAM_INDEX_PATH] = filePath;
        } else if (fileName == Constants::PACK_INFO) {
            parameterMap[Constants::PARAM_PACK_INFO_PATH] = filePath;
        } else if (fileName == Constants::RPCID_SC) {
            parameterMap[Constants::PARAM_RPCID_PATH] = filePath;
        } else if (fileName == Constants::PKG_CONTEXT_JSON) {
            parameterMap[Constants::PARAM_PKG_CONTEXT_PATH] = filePath;
        }
    }
    if (Utils::EndsWith(zipFilePath, Constants::HAP_SUFFIX)) {
        packager = std::make_unique<HapPackager>(parameterMap, resultReceiver);
    } else if (Utils::EndsWith(zipFilePath, Constants::HSP_SUFFIX)) {
        packager = std::make_unique<HspPackager>(parameterMap, resultReceiver);
    }

    if (packager == nullptr || packager->PreProcess() != ERR_OK || packager->Process() != ERR_OK) {
        return false;
    }
    return true;
}


bool GeneralNormalize::ProcessJsonFiles(const std::string &tempPath,
    std::list<GeneralNormalizeVersion> &generalNormalizeVersions, std::string &bundleName, std::string &moduleName)
{
    GeneralNormalizeVersion generalNormalizeVersion;
    std::string moduleJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::MODULE_JSON;
    std::string configJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::CONFIG_JSON;
    std::string packInfoPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::PACK_INFO;

    bool isModuleFileExists = fs::exists(moduleJsonPath);
    bool isConfigFileExists = fs::exists(configJsonPath);
    bool isPackInfoFileExists = fs::exists(packInfoPath);
    if (isModuleFileExists == isConfigFileExists) {
        LOGE("GeneralNormalize failed, invalid hap structure.");
        return false;
    }

    bool modifyJsonSuccess = false;
    if (isModuleFileExists) {
        modifyJsonSuccess = ModifyModuleJson(moduleJsonPath, generalNormalizeVersion, bundleName, moduleName);
    } else {
        modifyJsonSuccess = ModifyConfigJson(configJsonPath, generalNormalizeVersion, bundleName, moduleName);
    }

    if (!modifyJsonSuccess) {
        return false;
    }

    if (isPackInfoFileExists) {
        if (!ModifyPackInfo(packInfoPath)) {
            return false;
        }
    }
    generalNormalizeVersions.push_back(generalNormalizeVersion);
    return true;
}

int32_t GeneralNormalize::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    std::string tempPath = outPath + Constants::LINUX_FILE_SEPARATOR + Constants::COMPRESSOR_GENERALNORMALIZE_TEMP_DIR
        + Utils::GenerateUUID();
    std::list<GeneralNormalizeVersion> generalNormalizeVersionList;
    bool isSuccess = true;
    std::string bundleName;
    std::string moduleName;
    for (const std::string& path : hspOrhapList_) {
        ZipUtils::Unzip(path, tempPath);
        if (!ProcessJsonFiles(tempPath, generalNormalizeVersionList, bundleName, moduleName)) {
            Utils::ForceRemoveDirectory(tempPath);
            isSuccess = false;
            break;
        }
        if (!CompressDirToHap(tempPath,
            outPath + Constants::LINUX_FILE_SEPARATOR + fs::path(path).filename().string())) {
            Utils::ForceRemoveDirectory(tempPath);
            isSuccess = false;
            break;
        }
        Utils::ForceRemoveDirectory(tempPath);
    }
    if (!isSuccess) {
        LOGE("GeneralNormalize failed, bundleName: %s, moduleName: %s", bundleName.c_str(), moduleName.c_str());
        Utils::RemoveAllFilesInDirectory(outPath);
        return ERR_INVALID_VALUE;
    }
    if (!JsonUtils::StrToFile(GeneralNormalizeVersionUtils::ArrayToString(generalNormalizeVersionList), outPath +
        Constants::LINUX_FILE_SEPARATOR + Constants::GENERAL_RECORD)) {
        LOGE("WriteVersionRecord failed.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t GeneralNormalize::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS