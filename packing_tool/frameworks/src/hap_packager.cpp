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

#include "hap_packager.h"

#include <string>

#include "constants.h"
#include "json/json_utils.h"
#include "log.h"

namespace OHOS {
namespace AppPackingTool {
HapPackager::HapPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t HapPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t HapPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }

    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    std::string jsonPath;
    if (it != parameterMap_.end()) {
        jsonPath = it->second;
    }
    it = parameterMap_.find(Constants::PARAM_BIN_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && jsonPath.empty()) {
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

        return IsOutPathValid(outPath, forceRewrite, Constants::HAP_SUFFIX);
    } else {
        bool ret = IsVerifyValidInHapCommonMode() && IsVerifyValidInHapMode();
        if (!ret) {
            return ERR_INVALID_VALUE;
        }
    }
    return ERR_OK;
}

int32_t HapPackager::Process()
{
    if (!CompressHap()) {
        LOGE("Hap DoPackage failed.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t HapPackager::PostProcess()
{
    if (generateBuildHash_) {
        if (!CompressHap()) {
            LOGE("sencond CompressHap failed.");
            return ERR_INVALID_VALUE;
        }
    }
    return ERR_OK;
}

bool HapPackager::IsVerifyValidInHapCommonMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        LOGE("HapPackager::commandPathVerify json-path is empty.");
        return false;
    }
    jsonPath_ = it->second;
    if (!IsPathValid(it->second, true, Constants::CONFIG_JSON)
        && !IsPathValid(it->second, true, Constants::MODULE_JSON)) {
        LOGE("HapPackager::isArgsValidInHarMode json-path must be"
            " config.json or module.json file.");
        return false;
    }
    if (!IsValidRpcid() || !IsValidPackInfo()) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_PROFILE_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PROFILE_NAME) {
            LOGE("HapPackager::isArgsValidInHapMode profile-path"
                " must be CAPABILITY.profile file.");
            return false;
        }
    }
    if (!Compatible(Constants::PARAM_ABC_PATH, formattedAbcPathList_, Constants::ABC_SUFFIX)) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_DIR_LIST);
    if (it != parameterMap_.end() && !SplitDirList(it->second, formatedDirList_)) {
        LOGE("HapPackager::isArgsValidInHapMode --dir-list is invalid.");
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PKG_CONTEXT_JSON) {
            LOGE("HapPackager::isArgsValidInHapMode --pkg-context-path"
                " file must be pkgContextInfo.json file.");
            return false;
        }
    }
    return true;
}

bool HapPackager::Compatible(const std::string &paramPath, std::list<std::string> &fileList,
    const std::string &suffix)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty() && !CompatibleProcess(it->second,
        fileList, suffix)) {
        LOGE("HapPackager::isArgsValidInHapMode %s is invalid.", paramPath.c_str());
        return false;
    }
    return true;
}

bool HapPackager::IsVerifyValidInHapMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_INDEX_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::RESOURCES_INDEX) {
            LOGE("HapPackager::isArgsValidInHapMode index-path must be resources.index file.");
            return false;
        }
    }
    
    if (!Compatible(Constants::PARAM_MAPLE_SO_PATH, formattedSoPathList_, Constants::SO_SUFFIX) ||
        !Compatible(Constants::PARAM_ABILITY_SO_PATH, formattedAbilitySoPathList_, Constants::SO_SUFFIX) ||
        !Compatible(Constants::PARAM_JAR_PATH, formattedJarPathList_, Constants::JAR_SUFFIX) ||
        !Compatible(Constants::PARAM_TXT_PATH, formattedTxtPathList_, Constants::TXT_SUFFIX)) {
        return false;
    }
    
    if (!IsHapPathValid()) {
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!filePath.empty() && !fs::exists(filePath)) {
            LOGE("HapPackager::IsVerifyValidInHapMode --ets-path is invalid.");
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

    return IsOutPathValid(outPath, forceRewrite, Constants::HAP_SUFFIX);
}


bool HapPackager::IsValidRpcid()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_RPCID_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath)) {
            LOGE("HapPackager::isValidRpcid rpcid-path is not a file.");
            return false;
        }
        if (fs::path(filePath).filename().string() != Constants::RPCID_SC) {
            LOGE("HapPackager::isValidRpcid rpcid-path must be rpcid.sc file.");
            return false;
        }
    }
    return true;
}

bool HapPackager::IsValidPackInfo()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath)) {
            LOGE("HapPackager::isValidPackInfo --pack-info-path is not a file.");
            return false;
        }
        if (fs::path(filePath).filename().string() != Constants::PACK_INFO) {
            LOGE("HapPackager::isValidPackInfo --pack-info-path must be pack.info file.");
            return false;
        }
    }
    return true;
}

bool HapPackager::IsHapPathValid()
{
    if (IsHapPathValid(Constants::PARAM_MAPLE_SO_DIR)) {
        LOGE("HapPackager::isArgsValidInHapMode maple-so-dir is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_LIB_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode lib-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_HNP_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode hnp-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_RES_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode res-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_RESOURCES_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode resources-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_ASSETS_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode assets-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_SHAREDLIBS_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode shared-libs-path is invalid.");
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_AN_PATH)) {
        LOGE("HapPackager::isArgsValidInHapMode an-path is invalid.");
        return false;
    }
    return true;
}

bool HapPackager::IsHapPathValid(const std::string &parameterMapKey)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(parameterMapKey);
    if (it == parameterMap_.end()) {
        return false;
    }
    const std::string path = it->second;
    return (!path.empty() && !IsPathValid(path, false));
}

bool HapPackager::CompressHap()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_BIN_PATH);
    if (jsonPath_.empty() && it != parameterMap_.end() && !it->second.empty()) {
        CompressHapMode();
        return true;
    }
    if (!SetGenerateBuildHash(jsonPath_, generateBuildHash_, buildHashFinish_)) {
        return false;
    }
    if (!moduleJson_.ParseFromFile(jsonPath_)) {
        LOGE("ParseFromFile failed");
        return false;
    }
    if (JsonUtils::IsModuleJson(jsonPath_)) {
        if (!CheckStageHap(jsonPath_)) {
            LOGE("CheckStageHap failed.");
            return false;
        }
        std::string moduleType;
        if (!moduleJson_.GetStageModuleType(moduleType)) {
            LOGW("GetStageModuleType failed");
        }
        if (Constants::TYPE_SHARED == moduleType) {
            LOGW("Compress mode is hap, but module type is shared.");
        }

        std::string bundleType;
        if (!moduleJson_.GetStageBundleType(bundleType)) {
            LOGW("GetStageBundleType failed");
        }
        if (Constants::TYPE_SHARED == bundleType) {
            LOGW("warning:Compress mode is hap, but app type is shared.");
        }
        if (!CompressHapModeForModule(jsonPath_) || !BuildHash(buildHashFinish_, generateBuildHash_,
            parameterMap_, jsonPath_)) {
            return false;
        }
    } else {
        if (!CompressHapMode() || !BuildHash(buildHashFinish_, generateBuildHash_, parameterMap_, jsonPath_)) {
            return false;
        }
    }
    return true;
}

bool HapPackager::CheckStageHap(const std::string &jsonPath)
{
    if (!moduleJson_.CheckStageAsanTsanEnabledValid()) {
        LOGE("CheckStageAsanTsanEnabledValid failed.");
        return false;
    }

    if (!moduleJson_.CheckStageAtomicService()) {
        LOGE("CheckStageAtomicService failed.");
        return false;
    }

    return true;
}

bool HapPackager::CompressHapModeForModule(const std::string &jsonPath)
{
    if (!moduleJson_.GetStageModuleName(moduleName_)) {
        LOGE("HapPackager::Process: GetStageModuleName failed!");
    }
    if (!moduleJson_.GetStageDeviceTypes(deviceTypes_)) {
        LOGE("HapPackager::Process: GetStageDeviceTypes failed!");
    }
    if (!OpenZipWrapper()) {
        return false;
    }

    std::string jsonString = moduleJson_.ToString();
    if (!jsonString.empty()) {
        if (zipWrapper_.WriteStringToZip(jsonString, Constants::MODULE_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper WriteStringToZip failed!");
            return false;
        }
    }
    
    std::map<std::string, std::string> paramFileMap = {
        {Constants::PARAM_PROFILE_PATH, Constants::PROFILE_NAME},
        {Constants::PARAM_INDEX_PATH, Constants::RESOURCES_INDEX},
        {Constants::PARAM_LIB_PATH, Constants::LIB_PATH},
        {Constants::PARAM_AN_PATH, Constants::AN_PATH},
        {Constants::PARAM_AP_PATH, Constants::AP_PATH},
        {Constants::PARAM_RESOURCES_PATH, Constants::RESOURCES_PATH},
        {Constants::PARAM_JS_PATH, Constants::JS_PATH},
        {Constants::PARAM_ETS_PATH, Constants::ETS_PATH},
        {Constants::PARAM_HNP_PATH, Constants::HNP_PATH},
        {Constants::PARAM_RPCID_PATH, Constants::RPCID_SC},
        {Constants::PARAM_ASSETS_PATH, Constants::ASSETS_PATH},
        {Constants::PARAM_PACK_INFO_PATH, Constants::PACK_INFO}
    };
    for (auto& item : paramFileMap) {
        if (!AddCommonFileOrDirectoryToZip(item.first, item.second)) {
            return false;
        }
    }

    if (!AddParamFileToZip() || !AddResFileAndDirLsitToZip() || !AddPkgAndBinFileToZipForStageMaode()) {
        return false;
    }
    return CompressHapModeMultiple();
}

bool HapPackager::CompressHapMode()
{
    if (!moduleJson_.GetFaModuleName(moduleName_)) {
        LOGE("HapPackager::Process: GetFaModuleName failed!");
    }
    if (!moduleJson_.GetFaDeviceTypes(deviceTypes_)) {
        LOGE("HapPackager::Process: GetFaDeviceTypes failed!");
    }

    if (!OpenZipWrapper()) {
        return false;
    }

    std::string jsonString = moduleJson_.ToString();
    if (!jsonString.empty()) {
        if (zipWrapper_.WriteStringToZip(jsonString, Constants::CONFIG_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper WriteStringToZip failed!");
            return false;
        }
    }
    
    std::map<std::string, std::string> paramFileMap = {
        {Constants::PARAM_PROFILE_PATH, Constants::PROFILE_NAME},
        {Constants::PARAM_LIB_PATH, Constants::LIB_PATH},
        {Constants::PARAM_RPCID_PATH, Constants::RPCID_SC},
        {Constants::PARAM_PACK_INFO_PATH, Constants::PACK_INFO},
        {Constants::PARAM_ASSETS_PATH, Constants::ASSETS_PATH}
    };
    for (auto& item : paramFileMap) {
        if (!AddCommonFileOrDirectoryToZip(item.first, item.second)) {
            return false;
        }
    }

    if (!AddIndexToZipForFaMaode() || !AddParamFileToZip() || !AddResFileAndDirLsitToZip()) {
        return false;
    }

    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && !moduleName_.empty()) {
        std::string resourcesPath = Constants::ASSETS_PATH + Constants::LINUX_FILE_SEPARATOR + moduleName_ +
            Constants::LINUX_FILE_SEPARATOR + Constants::RESOURCES_PATH;
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, resourcesPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return CompressHapModeMultiple();
}

bool HapPackager::CompressHapModeMultiple()
{
    for (std::string soPathItem : formattedSoPathList_) {
        std::string zipPath = Constants::SO_ARM64_DIR + fs::path(soPathItem).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(soPathItem, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_MAPLE_SO_DIR);
    if (it != parameterMap_.end() && formattedSoPathList_.size() == 0 && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::SO_DIR) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    
    for (auto& pathList : {formattedAbilitySoPathList_, formattedAbcPathList_, formattedJarPathList_,
        formattedTxtPathList_}) {
        if (!AddFileListToZip(pathList)) {
            return false;
        }
    }

    if (!AddCommonFileOrDirectoryToZip(Constants::PARAM_SHAREDLIBS_PATH, Constants::SHARED_LIBS_DIR)) {
        return false;
    }
    zipWrapper_.Close();
    return true;
}

bool HapPackager::AddFileListToZip(const std::list<std::string> &pathList_)
{
    for (auto Item : pathList_) {
        std::string zipPath = fs::path(Item).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(Item, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return true;
}

bool HapPackager::OpenZipWrapper()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    std::string outPath;
    if (it != parameterMap_.end()) {
        outPath = it->second;
    }

    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        LOGE("HapPackager::Process: zipWrapper Open failed!");
        return false;
    }
    return true;
}

bool HapPackager::AddCommonFileOrDirectoryToZip(const std::string &paramPath, const std::string &targetPath)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, targetPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return true;
}

bool HapPackager::AddParamFileToZip()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_FILE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return true;
}

bool HapPackager::AddResFileAndDirLsitToZip()
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
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }

    if (!formatedDirList_.empty()) {
        for (const auto& dirPath : formatedDirList_) {
            std::string baseDir = fs::path(dirPath).filename().string();
            if (zipWrapper_.AddFileOrDirectoryToZip(dirPath, baseDir) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
                return false;
            }
        }
    }
    return true;
}

bool HapPackager::AddIndexToZipForFaMaode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_INDEX_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && !moduleName_.empty()) {
        std::string assetsPath = Constants::ASSETS_PATH + Constants::LINUX_FILE_SEPARATOR + moduleName_ +
            Constants::LINUX_FILE_SEPARATOR + fs::path(it->second).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, assetsPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    return true;
}

bool HapPackager::AddPkgAndBinFileToZipForStageMaode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_BIN_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        ModuleJson moduleJson;
        if (!moduleJson.ParseFromFile(it->second)) {
            LOGE("HapPackager::Process: moduleJson Read failed!");
            return false;
        }
        std::string jsonString = moduleJson.ToString();
        if (!jsonString.empty()) {
            if (zipWrapper_.WriteStringToZip(jsonString, Constants::PKG_CONTEXT_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("HapPackager::Process: zipWrapper WriteStringToZip failed!");
                return false;
            }
        } else {
            LOGE("HapPackager::Process: jsonFile error!");
            return false;
        }
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS