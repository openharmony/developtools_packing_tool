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

#include "hsp_packager.h"

#include <string>

#include "constants.h"
#include "json/json_utils.h"
#include "log.h"

namespace OHOS {
namespace AppPackingTool {
HspPackager::HspPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t HspPackager::InitAllowedParam()
{
    return ERR_OK;
}
int32_t HspPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }

    bool ret = IsVerifyValidInHspCommonMode() && IsVerifyValidInHspMode();
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}
int32_t HspPackager::Process()
{
    if (!CompressHsp()) {
        std::string outPath;
        if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
            outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
        }
        if (fs::exists(outPath)) {
            fs::remove_all(outPath);
        }
        LOGE("Hsp Process failed!");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}
int32_t HspPackager::PostProcess()
{
    if (generateBuildHash_) {
        if (!CompressHsp()) {
            std::string outPath;
            if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
                outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
            }
            if (fs::exists(outPath)) {
                fs::remove_all(outPath);
            }
            LOGE("sencond CompressHsp failed!");
            return ERR_INVALID_VALUE;
        }
    }
    return ERR_OK;
}

bool HspPackager::IsVerifyValidInHspCommonMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        LOGE("HspPackager::isArgsValidInHspMode json-path is empty.");
        return false;
    }
    jsonPath_ = it->second;
    if (!IsPathValid(it->second, true, Constants::MODULE_JSON)) {
        LOGE("HspPackager::isArgsValidInHspMode json-path must be module.json file.");
        return false;
    }
    if (!Compatible(Constants::PARAM_JAR_PATH, formattedJarPathList_, Constants::JAR_SUFFIX) ||
        !Compatible(Constants::PARAM_TXT_PATH, formattedTxtPathList_, Constants::TXT_SUFFIX)) {
        return false;
    }
    if (!IsHspPathValid()) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_DIR_LIST);
    if (it != parameterMap_.end() && !it->second.empty() &&
        !SplitDirList(it->second, formatedDirList_)) {
        LOGE("HspPackager::isArgsValidInHspMode --dir-list is invalid.");
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_PROFILE_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PROFILE_NAME) {
            LOGE("HspPackager::isArgsValidInHspMode profile-path"
                " must be CAPABILITY.profile file.");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end() && !it ->second.empty()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PKG_CONTEXT_JSON) {
            LOGE("HspPackager::isArgsValidInHspMode --pkg-context-path file"
                " must be pkgContextInfo.json file.");
            return false;
        }
    }
    return true;
}

bool HspPackager::IsVerifyValidInHspMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!filePath.empty() && !fs::exists(filePath)) {
            LOGE("HspPackager::IsVerifyValidInHspMode --ets-path is invalid.");
            return false;
        }
    }
    
    std::string outPath = "";
    std::string forceRewrite = "";
    it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it != parameterMap_.end()) {
        outPath = it->second;
    }

    it = parameterMap_.find(Constants::PARAM_FORCE);
    if (it != parameterMap_.end()) {
        forceRewrite = it->second;
    }

    return IsOutPathValid(outPath, forceRewrite, Constants::HSP_SUFFIX);
}

bool HspPackager::Compatible(const std::string &paramPath, std::list<std::string> &fileList,
    const std::string &suffix)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty() && !CompatibleProcess(it->second,
        fileList, suffix)) {
        LOGE("HspPackager::isArgsValidInHapMode %s is invalid.", paramPath.c_str());
        return false;
    }
    return true;
}

bool HspPackager::IsHspPathValid()
{
    if (IsHspPathValid(Constants::PARAM_LIB_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode lib-path is invalid.");
        return false;
    }
    if (IsHspPathValid(Constants::PARAM_RES_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode res-path is invalid.");
        return false;
    }
    if (IsHspPathValid(Constants::PARAM_RESOURCES_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode resources-path is invalid.");
        return false;
    }
    if (IsHspPathValid(Constants::PARAM_ASSETS_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode assets-path is invalid.");
        return false;
    }
    if (IsHspPathValid(Constants::PARAM_AP_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode ap-path is invalid.");
        return false;
    }
    if (IsHspPathValid(Constants::PARAM_AN_PATH)) {
        LOGE("HspPackager::isArgsValidInHspMode an-path is invalid.");
        return false;
    }
    return true;
}

bool HspPackager::IsHspPathValid(const std::string &parameterMapKey)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(parameterMapKey);
    if (it == parameterMap_.end()) {
        return false;
    }
    const std::string path = it->second;
    return (!path.empty() && !IsPathValid(path, false));
}

bool HspPackager::CompressHsp()
{
    if (!SetGenerateBuildHash(jsonPath_, generateBuildHash_, buildHashFinish_)) {
        return false;
    }
    if (!moduleJson_.ParseFromFile(jsonPath_)) {
        LOGE("ParseFromFile failed");
        return false;
    }
    if (JsonUtils::IsModuleJson(jsonPath_)) {
        if (!moduleJson_.CheckStageAsanTsanEnabledValid()) {
            LOGE("CheckStageAsanTsanEnabledValid failed.");
            return false;
        }
        if (!moduleJson_.CheckStageAtomicService()) {
            LOGE("CheckStageAtomicService failed.");
            return false;
        }
        if (!moduleJson_.CheckStageOverlayCfg()) {
            LOGE("checkStageOverlayCfg failed.");
            return false;
        }
        std::string moduleType;
        if (!moduleJson_.GetStageModuleType(moduleType)) {
            LOGW("GetStageModuleType failed.");
        }
        if (moduleType != Constants::TYPE_SHARED) {
            LOGE("module type must be shared.");
            return false;
        }
    }
    if (!CompressHspMode(jsonPath_) || !BuildHash(buildHashFinish_, generateBuildHash_, parameterMap_, jsonPath_)) {
        return false;
    }
    return true;
}

bool HspPackager::CompressHspMode(const std::string &jsonPath)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    std::string outPath;
    if (it != parameterMap_.end()) {
        outPath = it->second;
    }
    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        LOGE("HspPackager::Process: zipWrapper Open failed!");
        return false;
    }
    std::string jsonString = moduleJson_.ToString();
    if (!jsonString.empty()) {
        std::string jsonType;
        if (JsonUtils::IsModuleJson(jsonPath)) {
            jsonType = Constants::MODULE_JSON;
            if (!moduleJson_.GetStageModuleName(moduleName_) || moduleJson_.GetStageDeviceTypes(deviceTypes_)) {
                LOGW("GetStageModuleName or GetStageDeviceTypes failed!");
            }
        } else {
            jsonType = Constants::CONFIG_JSON;
            if (!moduleJson_.GetFaModuleName(moduleName_) || moduleJson_.GetFaDeviceTypes(deviceTypes_)) {
                LOGW("GetStageModuleName or GetStageDeviceTypes failed!");
            }
        }
        if (zipWrapper_.WriteStringToZip(jsonString, jsonType) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper WriteStringToZip failed!");
            return false;
        }
    }
    
    if (!AddCommonFileOrDirectoryToZip(Constants::PARAM_PROFILE_PATH, Constants::PROFILE_NAME)) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_INDEX_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && JsonUtils::IsModuleJson(jsonPath)) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_INDEX) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return CompressHspModePartSecond(jsonPath);
}

bool HspPackager::CompressHspModePartSecond(const std::string &jsonPath)
{
    std::map<std::string, std::string> paramFileMap = {
        {Constants::PARAM_LIB_PATH, Constants::LIB_PATH},
        {Constants::PARAM_AN_PATH, Constants::AN_PATH},
        {Constants::PARAM_AP_PATH, Constants::AP_PATH},
        {Constants::PARAM_RPCID_PATH, Constants::RPCID_SC},
        {Constants::PARAM_ASSETS_PATH, Constants::ASSETS_PATH}
    };
    for (auto& item : paramFileMap) {
        if (!AddCommonFileOrDirectoryToZip(item.first, item.second)) {
            return false;
        }
    }

    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_FILE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && JsonUtils::IsModuleJson(jsonPath)) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_PATH) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return CompressHspModePartThird(jsonPath);
}

bool HspPackager::CompressHspModePartThird(const std::string &jsonPath)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_RES_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && !moduleName_.empty()) {
        std::string resPath = Constants::ASSETS_PATH + Constants::LINUX_FILE_SEPARATOR + moduleName_ +
            Constants::LINUX_FILE_SEPARATOR + Constants::RESOURCES_PATH;
        std::string deviceType;
        if (!deviceTypes_.empty()) {
            deviceType = deviceTypes_.front();
        }
        if (Constants::DEVICE_TYPE_FITNESSWATCH == deviceType ||
            Constants::DEVICE_TYPE_FITNESSWATCH_NEW == deviceType) {
            resPath = Constants::RES_PATH;
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, resPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_JS_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && JsonUtils::IsModuleJson(jsonPath)) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::JS_PATH) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && JsonUtils::IsModuleJson(jsonPath)) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::ETS_PATH) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return CompressHspModePartFourth();
}

bool HspPackager::CompressHspModePartFourth()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_BIN_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    if (!AddCommonFileOrDirectoryToZip(Constants::PARAM_PACK_INFO_PATH, Constants::PACK_INFO)) {
        return false;
    }
    if (!formatedDirList_.empty()) {
        for (const auto& dirPath : formatedDirList_) {
            std::string baseDir = fs::path(dirPath).filename().string();
            if (zipWrapper_.AddFileOrDirectoryToZip(dirPath, baseDir) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
        }
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        ModuleJson moduleJson;
        if (!moduleJson.ParseFromFile(it->second)) {
            LOGE("HspPackager::Process: moduleJson Read failed!");
            return false;
        }
        std::string jsonString = moduleJson.ToString();
        if (!jsonString.empty()) {
            if (zipWrapper_.WriteStringToZip(jsonString, Constants::PKG_CONTEXT_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("HspPackager::Process: zipWrapper WriteStringToZip failed!");
                return false;
            }
        } else {
            LOGE("HspPackager::Process: jsonFile error!");
            return false;
        }
    }
    return CompressHspModeMultiple();
}

bool HspPackager::CompressHspModeMultiple()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_MAPLE_SO_DIR);
    if (it != parameterMap_.end() && formattedSoPathList_.size() == 0 && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::SO_DIR) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    for (auto jarPathItem : formattedJarPathList_) {
        std::string zipPath = fs::path(jarPathItem).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(jarPathItem, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    for (auto txtPathItem : formattedTxtPathList_) {
        std::string zipPath = fs::path(txtPathItem).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(txtPathItem, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_SHAREDLIBS_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::SHARED_LIBS_DIR) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    zipWrapper_.Close();
    return true;
}

bool HspPackager::AddCommonFileOrDirectoryToZip(const std::string &paramPath, const std::string &targetPath)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, targetPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HspPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS