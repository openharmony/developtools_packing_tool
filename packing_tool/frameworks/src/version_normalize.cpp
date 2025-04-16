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

#include "version_normalize.h"

#include "hap_packager.h"
#include "hqf_packager.h"
#include "hsp_packager.h"
#include "json/json_utils.h"
#include "json/normalize_version_utils.h"
#include "json/pack_info.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
VersionNormalize::VersionNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t VersionNormalize::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t VersionNormalize::PreProcess()
{
    if (!IsOutDirectoryValid()) {
        return ERR_INVALID_VALUE;
    }
    auto it = parameterMap_.find(Constants::PARAM_INPUT_LIST);
    if (it == parameterMap_.end()) {
        LOGE("Input input-list is empty.");
        return ERR_INVALID_VALUE;
    }
    if (!CompatibleProcess(it->second, hspOrhapList_, Constants::HAP_SUFFIX, Constants::HSP_SUFFIX)) {
            LOGE("Input input-list is invalid.");
            return ERR_INVALID_VALUE;
        }
    if (hspOrhapList_.size() == 0) {
        LOGE("Input input-list is empty.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    std::regex pattern(Constants::VERSION_NAME_PATTERN);
    if (it == parameterMap_.end()) {
        LOGE("Input version-name is empty.");
        return ERR_INVALID_VALUE;
    } else if (!std::regex_match(it->second, pattern)) {
        LOGE("Input version-name is not valid.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it == parameterMap_.end()) {
        LOGE("Input version-code is empty.");
        return ERR_INVALID_VALUE;
    } else if (!Utils::IsPositiveInteger(it->second)) {
        LOGE("Input version-code is invalid.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

bool VersionNormalize::VerifyModuleVersion(const NormalizeVersion &normalizeVersion, const int32_t &newVersionCode,
    const std::string &newVersionName)
{
    if (normalizeVersion.originVersionCode > newVersionCode) {
        LOGE("VersionNormalize failed, input version code less than module %s version code.",
            normalizeVersion.moduleName.c_str());
        return false;
    } else if (normalizeVersion.originVersionCode == newVersionCode) {
        LOGW("VersionNormalize warning: module %s version code not changed.", normalizeVersion.moduleName.c_str());
    }
    if (normalizeVersion.originVersionName == newVersionName) {
        LOGW("VersionNormalize warning: module %s version name not changed.", normalizeVersion.moduleName.c_str());
    }
    return true;
}

bool VersionNormalize::ModifyModuleJson(const std::string &moduleJsonPath, NormalizeVersion &normalizeVersion,
    const int32_t &newVersionCode, const std::string &newVersionName)
{
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(moduleJsonPath)) {
        LOGE("Parse and modify module.json failed, parse module.json is null.");
        return false;
    }
    if (!moduleJson.GetNormalizeVersion(normalizeVersion, true)) {
        LOGE("Parse and modify module.json failed, write Json failed.");
        return false;
    }
    if (!moduleJson.SetVersionCodeAndName(newVersionCode, newVersionName, true)) {
        LOGE("Parse and modify module.json failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath)) {
        LOGE("Parse and modify module.json failed, write Json failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::ModifyConfigJson(const std::string &configJsonPath, NormalizeVersion &normalizeVersion,
    const int32_t &newVersionCode, const std::string &newVersionName)
{
    ModuleJson configJson;
    if (!configJson.ParseFromFile(configJsonPath)) {
        LOGE("Parse and modify config.json failed, parse json is null.");
        return false;
    }
    if (!configJson.GetNormalizeVersion(normalizeVersion, false)) {
        LOGE("Parse and modify config.json failed, json file not valid.");
        return false;
    }
    if (!configJson.SetVersionCodeAndName(newVersionCode, newVersionName, false)) {
        LOGE("Parse and modify config.json failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(configJson.ToString(), configJsonPath)) {
        LOGE("Parse and modify config.json failed, writeJson failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::ModifyPackInfo(const std::string &packInfoPath, const int32_t &newVersionCode,
    const std::string &newVersionName)
{
    PackInfo packInfo;
    if (!packInfo.ParseFromFile(packInfoPath)) {
        LOGW("Parse and modify packInfo failed, json format invalid.");
        return false;
    }
    if (!packInfo.SetVersionCode(newVersionCode)) {
        LOGW("Parse and modify packInfo failed, version code invalid.");
        return false;
    }
    if (!packInfo.SetVersionName(newVersionName)) {
        LOGW("Parse and modify packInfo failed, version name invalid.");
        return false;
    }

    if (!JsonUtils::StrToFile(packInfo.ToString(), packInfoPath)) {
        LOGW("Parse and modify packInfo failed, writeJson failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::CompressDirToHap(const std::string &sourceDir, const std::string &zipFilePath)
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


bool VersionNormalize::ProcessJsonFiles(const std::string &tempPath, std::list<NormalizeVersion> &normalizeVersionList,
    const int32_t &versionCode, const std::string &versionName)
{
    NormalizeVersion normalizeVersion;
    std::string moduleJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::MODULE_JSON;
    std::string configJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::CONFIG_JSON;
    std::string packInfoPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::PACK_INFO;

    bool isModuleFileExists = fs::exists(moduleJsonPath);
    bool isConfigFileExists = fs::exists(configJsonPath);
    bool isPackInfoFileExists = fs::exists(packInfoPath);
    if (isModuleFileExists == isConfigFileExists) {
        LOGE("VersionNormalize failed, invalid hap structure.");
        return false;
    }

    bool modifyJsonSuccess = false;
    if (isModuleFileExists) {
        modifyJsonSuccess = ModifyModuleJson(moduleJsonPath, normalizeVersion, versionCode, versionName);
    } else {
        modifyJsonSuccess = ModifyConfigJson(configJsonPath, normalizeVersion, versionCode, versionName);
    }

    if (!modifyJsonSuccess) {
        normalizeVersionList.push_back(normalizeVersion);
        return false;
    }

    if (isPackInfoFileExists) {
        if (!ModifyPackInfo(packInfoPath, versionCode, versionName)) {
            normalizeVersionList.push_back(normalizeVersion);
            return false;
        }
    }

    if (!VerifyModuleVersion(normalizeVersion, versionCode, versionName)) {
        return false;
    }
    normalizeVersionList.push_back(normalizeVersion);
    return true;
}

int32_t VersionNormalize::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    std::string tempPath = outPath + Constants::LINUX_FILE_SEPARATOR + Constants::COMPRESSOR_VERSIONNORMALIZE_TEMP_DIR
        + Utils::GenerateUUID();
    int32_t versionCode = 0;
    auto it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it != parameterMap_.end()) {
        try {
            versionCode = std::stoi(it->second);
        } catch (const std::exception& e) {
            LOGE("Exception: %s", e.what());
            return ERR_INVALID_VALUE;
        }
    } else {
        LOGE("Parameter not found: %s", Constants::PARAM_VERSION_CODE.c_str());
        return ERR_INVALID_VALUE;
    }
    std::string versionName = parameterMap_.at(Constants::PARAM_VERSION_NAME);
    std::list<NormalizeVersion> normalizeVersionList;

    for (const std::string& path : hspOrhapList_) {
        ZipUtils::Unzip(path, tempPath);
        if (!ProcessJsonFiles(tempPath, normalizeVersionList, versionCode, versionName)) {
            Utils::ForceRemoveDirectory(tempPath);
            continue;
        }
        if (!CompressDirToHap(tempPath,
            outPath + Constants::LINUX_FILE_SEPARATOR + fs::path(path).filename().string())) {
            Utils::ForceRemoveDirectory(tempPath);
            continue;
        }
        Utils::ForceRemoveDirectory(tempPath);
    }

    if (!JsonUtils::StrToFile(NormalizeVersionUtils::ArrayToString(normalizeVersionList), outPath +
        Constants::LINUX_FILE_SEPARATOR + Constants::VERSION_RECORD)) {
        LOGE("WriteVersionRecord failed.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t VersionNormalize::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS