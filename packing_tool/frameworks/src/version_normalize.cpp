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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>

#include "version_normalize.h"

#include "constants.h"
#include "contrib/minizip/unzip.h"
#include "contrib/minizip/zip.h"
#include "hap_packager.h"
#include "hqf_packager.h"
#include "hsp_packager.h"
#include "json/json_utils.h"
#include "json/normalize_version_utils.h"
#include "json/pack_info.h"
#include "log.h"
#include "nlohmann/json.hpp"
#include "packager.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {}

VersionNormalize::VersionNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int VersionNormalize::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int VersionNormalize::PreProcess()
{
    if (!IsOutDirectoryValid()) {
        return ERR_INVALID_VALUE;
    }
    auto it = parameterMap_.find(Constants::PARAM_INPUT_LIST);
    if (it == parameterMap_.end()) {
        LOGE("CompressVerify::validate VersionNormalizeMode input-list is empty.");
        return ERR_INVALID_VALUE;
    }
    if (!CompatibleProcess(it->second, hspOrhapList_, Constants::HAP_SUFFIX, Constants::HSP_SUFFIX)) {
            LOGE("CompressVerify::validatePackageNormalizeMode input-list is invalid.");
            return ERR_INVALID_VALUE;
        }
    if (hspOrhapList_.size() == 0) {
        LOGE("CompressVerify::validate VersionNormalizeMode input-list is empty.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_NAME);
    std::regex pattern(Constants::VERSION_NAME_PATTERN);
    if (it == parameterMap_.end()) {
        LOGE("CompressVerify::validate VersionNormalizeMode version-name is empty.");
        return ERR_INVALID_VALUE;
    } else if (!std::regex_match(it->second, pattern)) {
        LOGE("CompressVerify::validate VersionNormalizeMode version-name is not valid.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it == parameterMap_.end()) {
        LOGE("validate VersionNormalizeMode version-code is empty.");
        return ERR_INVALID_VALUE;
    } else if (!Utils::IsPositiveInteger(it->second)) {
        LOGE("validate VersionNormalizeMode version-code is invalid.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

bool VersionNormalize::VerifyModuleVersion(const NormalizeVersion &normalizeVersion, const int newVersionCode,
    const std::string newVersionName)
{
    if (normalizeVersion.originVersionCode > newVersionCode) {
        LOGE("versionNormalize failed, module %s version code less than input version code",
            normalizeVersion.moduleName.c_str());
        return false;
    } else if (normalizeVersion.originVersionCode == newVersionCode) {
        LOGW("versionNormalize warning: module %s version code not changed", normalizeVersion.moduleName.c_str());
    }
    if (normalizeVersion.originVersionName == newVersionName) {
        LOGW("versionNormalize warning: module %s version name not changed", normalizeVersion.moduleName.c_str());
    }
    return true;
}

bool VersionNormalize::ModifyModuleJson(const std::string &moduleJsonPath, NormalizeVersion &normalizeVersion,
    const int newVersionCode, const std::string newVersionName)
{
    LOGI("start modify moduleJson");
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(moduleJsonPath)) {
        LOGE("parseAndModifyModuleJson failed, parse module.json is null.");
        return false;
    }
    if (!moduleJson.GetNormalizeVersion(normalizeVersion, true)) {
        LOGE("parseAndModifyModuleJson failed, write Json failed.");
        return false;
    }
    if (!moduleJson.SetVersionCodeAndName(newVersionCode, newVersionName, true)) {
        LOGE("parseAndModifyModuleJson failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath)) {
        LOGE("parseAndModifyModuleJson failed,  writeJson failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::ModifyConfigJson(const std::string &configJsonPath, NormalizeVersion &normalizeVersion,
    const int newVersionCode, const std::string newVersionName)
{
    ModuleJson configJson;
    if (!configJson.ParseFromFile(configJsonPath)) {
        LOGE("parseAndModifyConfigJson failed, parse json is null.");
        return false;
    }
    if (!configJson.GetNormalizeVersion(normalizeVersion, false)) {
        LOGE("parseAndModifyConfigJson failed, json file not valid");
        return false;
    }
    if (!configJson.SetVersionCodeAndName(newVersionCode, newVersionName, false)) {
        LOGE("parseAndModifyConfigJson failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(configJson.ToString(), configJsonPath)) {
        LOGE("parseAndModifyConfigJson failed,  writeJson failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::ModifyPackInfo(const std::string &packInfoPath, const int newVersionCode,
    const std::string newVersionName)
{
    PackInfo packInfo;
    if (!packInfo.ParseFromFile(packInfoPath)) {
        LOGW("parseAndModifyPackInfo failed, json format invalid.");
        return false;
    }
    if (!packInfo.SetVersionCode(newVersionCode)) {
        LOGW("parseAndModifyPackInfo failed, version invalid.");
        return false;
    }
    if (!packInfo.SetVersionName(newVersionName)) {
        LOGW("parseAndModifyPackInfo failed, version invalid.");
        return false;
    }

    if (!JsonUtils::StrToFile(packInfo.ToString(), packInfoPath)) {
        LOGW("parseAndModifyPackInfo failed, writeJson failed.");
        return false;
    }
    return true;
}

bool VersionNormalize::compressDirToHap(const std::string& sourceDir, const std::string& zipFilePath)
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
    if (packager->PreProcess() != ERR_OK || packager->Process() != ERR_OK) {
        return false;
    }
    return true;
}


bool VersionNormalize::ProcessJsonFiles(const std::string& tempPath, NormalizeVersion& normalizeVersion,
    int versionCode, const std::string& versionName)
{
    std::string moduleJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::MODULE_JSON;
    std::string configJsonPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::CONFIG_JSON;
    std::string packInfoPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::PACK_INFO;

    bool isModuleFileExists = fs::exists(moduleJsonPath);
    bool isConfigFileExists = fs::exists(configJsonPath);
    bool isPackInfoFileExists = fs::exists(packInfoPath);
    if ((isModuleFileExists && isConfigFileExists) || (!isModuleFileExists && !isConfigFileExists)) {
        LOGE("versionNormalize failed, invalid hap structure.");
        return false;
    }

    if (isModuleFileExists) {
        if (!ModifyModuleJson(moduleJsonPath, normalizeVersion, versionCode, versionName))
            return false;
    } else {
        if (!ModifyConfigJson(configJsonPath, normalizeVersion, versionCode, versionName))
            return false;
    }

    if (isPackInfoFileExists) {
        if (!ModifyPackInfo(packInfoPath, versionCode, versionName)) {
            return false;
        }
    }

    if (!VerifyModuleVersion(normalizeVersion, versionCode, versionName)) {
        LOGE("versionNormalize failed, version code less than input version code");
        return false;
    }
    return true;
}

int VersionNormalize::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    std::string tempPath = outPath + Constants::LINUX_FILE_SEPARATOR + Constants::COMPRESSOR_TEMP_DIR;
    int versionCode = std::stoi(parameterMap_.at(Constants::PARAM_VERSION_CODE));
    std::string versionName = parameterMap_.at(Constants::PARAM_VERSION_NAME);
    std::list<NormalizeVersion> normalizeVersionList;

    for (const std::string& path : hspOrhapList_) {
        ZipUtils::Unzip(path, tempPath);
        NormalizeVersion normalizeVersion;
        if (!ProcessJsonFiles(tempPath, normalizeVersion, versionCode, versionName)) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        normalizeVersionList.push_back(normalizeVersion);
        if (!compressDirToHap(tempPath,
            outPath + Constants::LINUX_FILE_SEPARATOR + fs::path(path).filename().string())) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        Utils::ForceRemoveDirectory(tempPath);
    }

    if (!normalizeVersionList.empty() &&
        !JsonUtils::StrToFile(NormalizeVersionUtils::ArrayToString(normalizeVersionList),
                              outPath + Constants::LINUX_FILE_SEPARATOR + Constants::VERSION_RECORD)) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int VersionNormalize::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS