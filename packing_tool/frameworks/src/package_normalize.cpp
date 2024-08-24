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

#include "package_normalize.h"

#include "json/json_utils.h"
#include "json/pack_info.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
PackageNormalize::PackageNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t PackageNormalize::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}


int32_t PackageNormalize::PreProcess()
{
    if (!IsOutDirectoryValid()) {
        return ERR_INVALID_VALUE;
    }
    auto it = parameterMap_.find(Constants::PARAM_HSP_LIST);
    if (it == parameterMap_.end()) {
        LOGE("PackageNormalizeMode hsp-list is empty.");
        return ERR_INVALID_VALUE;
    }
    if (!CompatibleProcess(it->second, hspList_, Constants::HSP_SUFFIX)) {
        LOGE("PackageNormalizeMode hsp-list is invalid.");
        return ERR_INVALID_VALUE;
    }
    if (hspList_.empty()) {
        LOGE("PackageNormalizeMode hsp-list is empty.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_BUNDLE_NAME);
    std::regex pattern(Constants::BUNDLE_NAME_PATTERN);
    if (it == parameterMap_.end() || !std::regex_match(it->second, pattern)) {
        LOGE("PackageNormalizeMode bundle-name is invalid.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_VERSION_CODE);
    if (it == parameterMap_.end() || !Utils::IsPositiveInteger(it->second)) {
        LOGE("PackageNormalizeMode version-code is invalid.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

bool PackageNormalize::ModifyModuleJson(const std::string &moduleJsonPath,
    const int32_t &newVersionCode, const std::string &newBundleName)
{
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(moduleJsonPath)) {
        LOGE("Update module.json failed, parse json is null.");
        return false;
    }
    if (!moduleJson.SetBundleName(newBundleName)) {
        LOGE("Parse and modify module.json failed, json file not valid.");
        return false;
    }
    if (!moduleJson.SetStageVersionCode(newVersionCode)) {
        LOGE("Parse and modify module.json failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath)) {
        LOGE("Parse and modify module.json failed, write Json failed.");
        return false;
    }
    return true;
}

bool PackageNormalize::ModifyPackInfo(const std::string &packInfoPath,
    const int32_t &newVersionCode, const std::string &newBundleName)
{
    PackInfo packInfo;
    if (!packInfo.ParseFromFile(packInfoPath)) {
        LOGE("Update packInfo failed, parse json is null.");
        return false;
    }
    if (!packInfo.SetBundleName(newBundleName)) {
        LOGE("Parse and modify packInfo failed, json file not valid.");
        return false;
    }
    if (!packInfo.SetVersionCode(newVersionCode)) {
        LOGE("Parse and modify packInfo failed, json file not valid.");
        return false;
    }
    if (!JsonUtils::StrToFile(packInfo.ToString(), packInfoPath)) {
        LOGE("Parse and modify packinfo failed, write Json failed.");
        return false;
    }
    return true;
}

int32_t PackageNormalize::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    std::string tempPath = outPath + Constants::LINUX_FILE_SEPARATOR + Constants::COMPRESSOR_PACKAGENORMALIZE_TEMP_DIR
        + Utils::GenerateUUID();
    int32_t versionCode = std::stoi(parameterMap_.at(Constants::PARAM_VERSION_CODE));
    std::string bundleName = parameterMap_.at(Constants::PARAM_BUNDLE_NAME);
    for (const std::string &path : hspList_) {
        if (ZipUtils::Unzip(path, tempPath) != ZIP_ERR_SUCCESS) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        std::string moduleJsonPath = tempPath +  Constants::LINUX_FILE_SEPARATOR + Constants::MODULE_JSON;
        std::string packInfoPath = tempPath + Constants::LINUX_FILE_SEPARATOR + Constants::PACK_INFO;
        fs::path entryPath(path);
        std::string filePath = entryPath.filename().string();
        if (!fs::exists(moduleJsonPath) || !fs::is_regular_file(moduleJsonPath) || !fs::exists(packInfoPath) ||
                !fs::is_regular_file(packInfoPath)) {
            LOGE("PackageNormalize failed: hsp not have file module.json or pack.info.");
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }

        if (!ModifyModuleJson(moduleJsonPath, versionCode, bundleName)) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        if (!ModifyPackInfo(packInfoPath, versionCode, bundleName)) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        if (ZipUtils::Zip(tempPath, outPath + Constants::LINUX_FILE_SEPARATOR + filePath) != ZIP_ERR_SUCCESS) {
            Utils::ForceRemoveDirectory(tempPath);
            return ERR_INVALID_VALUE;
        }
        Utils::ForceRemoveDirectory(tempPath);
    }
    return ERR_OK;
}

int32_t PackageNormalize::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS