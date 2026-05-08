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

#include "app_packager.h"

#include "constants.h"
#include "json/hap_verify_info.h"
#include "json/module_json_utils.h"
#include "json/hap_verify_utils.h"
#include "log.h"
#include "utils.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
bool GetFirstBundleTypeFromPathList(const std::list<std::string> &pathList, std::string &bundleType)
{
    for (const auto &path : pathList) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(path, hapVerifyInfo)) {
            LOGE("Failed to parse package '%s' for bundleType validation.", path.c_str());
            return false;
        }
        bundleType = hapVerifyInfo.GetBundleType();
        return true;
    }
    return false;
}

bool CheckSkillModuleTypes(const std::list<std::string> &hspPathList)
{
    for (const auto &hsp : hspPathList) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(hsp, hapVerifyInfo)) {
            LOGE("Failed to parse HSP '%s' for skill app moduleType validation.", hsp.c_str());
            return false;
        }
        if (hapVerifyInfo.GetModuleType() == Constants::TYPE_SKILL) {
            continue;
        }
        LOGE("HSP moduleType must be skill when bundleType is skill, but got '%s' in %s.",
            hapVerifyInfo.GetModuleType().c_str(), hsp.c_str());
        return false;
    }
    return true;
}

size_t CountInputPathItems(const std::string &pathList)
{
    if (pathList.empty()) {
        return 0;
    }
    size_t count = 0;
    std::string pathItem;
    std::istringstream pathStream(pathList);
    while (std::getline(pathStream, pathItem, Constants::COMMA_SPLIT)) {
        if (!pathItem.empty()) {
            count++;
        }
    }
    return count;
}

bool CheckSkillAppConstraints(const std::string &hapPath, const std::string &hspPath,
    const std::list<std::string> &hspPathList)
{
    if (hspPathList.empty()) {
        return true;
    }
    std::string bundleType;
    if (!GetFirstBundleTypeFromPathList(hspPathList, bundleType)) {
        return false;
    }
    if (bundleType != Constants::TYPE_SKILL) {
        return true;
    }
    if (!hapPath.empty()) {
        LOGE("--hap-path must be empty when bundleType is skill.");
        return false;
    }
    if (CountInputPathItems(hspPath) > 1) {
        LOGE("--hsp-path must contain only 1 HSP when bundleType is skill, but got %zu.",
            CountInputPathItems(hspPath));
        return false;
    }
    return CheckSkillModuleTypes(hspPathList);
}

}

AppPackager::AppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t AppPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t AppPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    if (!CheckStatDuplicateFlag()) {
        return ERR_INVALID_VALUE;
    }
    bool ret = IsVerifyValidInAppMode();
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    if (!Packager::ParseAtomicServiceSizeLimit()) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t AppPackager::Process()
{
    bool ret = CompressAppMode();
    if (!ret) {
        std::string outPath;
        if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
            outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
        }
        if (fs::exists(outPath)) {
            fs::remove_all(outPath);
        }
        LOGE("%s", PackingToolErrMsg::COMPRESS_APP_FAILED.toStringWithArgs("App Process failed.").c_str());
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t AppPackager::PostProcess()
{
    return ERR_OK;
}

bool AppPackager::CheckBundleTypeConsistency(const std::string &hapPath, const std::string &hspPath)
{
    std::string bundleType;
    std::list<std::string> tmpHapPathList;
    std::list<std::string> tmpHspPathList;
    CompatibleProcess(hapPath, tmpHapPathList, Constants::HAP_SUFFIX);
    CompatibleProcess(hspPath, tmpHspPathList, Constants::HSP_SUFFIX);
    if ((!tmpHapPathList.empty() && !ModuleJsonUtils::IsModuleHap(tmpHapPathList.front())) ||
        (!tmpHspPathList.empty() && !ModuleJsonUtils::IsModuleHap(tmpHspPathList.front()))) {
        LOGD("Module is FA");
        return true;
    }
    if (!tmpHapPathList.empty()) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(tmpHapPathList.front(), hapVerifyInfo)) {
            LOGW("GetStageHapVerifyInfo failed, this hap maybe fa module");
            return true;
        }
        bundleType = hapVerifyInfo.GetBundleType();
    } else if (!tmpHspPathList.empty()) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(tmpHspPathList.front(), hapVerifyInfo)) {
            LOGW("GetStageHapVerifyInfo failed, this hap maybe fa module");
            return true;
        }
        bundleType = hapVerifyInfo.GetBundleType();
    }
    for (const auto& hapPath : tmpHapPathList) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(hapPath, hapVerifyInfo)) {
            LOGW("GetStageHapVerifyInfo failed");
            return false;
        }
        if (bundleType != hapVerifyInfo.GetBundleType()) {
            LOGE("%s", PackingToolErrMsg::CHECK_BUNDLETYPE_CONSISTENCY_FAILED.toStringWithArgs(
                "The bundleType is not same for different HAP modules.").c_str());
            return false;
        }
    }
    for (const auto& hspPath : tmpHspPathList) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(hspPath, hapVerifyInfo)) {
            LOGW("GetStageHapVerifyInfo failed");
            return false;
        }
        if (bundleType != hapVerifyInfo.GetBundleType()) {
            LOGE("%s", PackingToolErrMsg::CHECK_BUNDLETYPE_CONSISTENCY_FAILED.toStringWithArgs(
                "The bundleType is not same for different HSP modules.").c_str());
            return false;
        }
    }
    return true;
}
  
bool AppPackager::VerifyIsSharedApp(const std::list<std::string>& hspPath)
{
    HapVerifyInfo hapVerifyInfo;
    if (!ModuleJsonUtils::GetStageHapVerifyInfo(hspPath.front(), hapVerifyInfo)) {
        return false;
    }
    if (hapVerifyInfo.GetBundleType() != Constants::TYPE_SHARED) {
        return false;
    }
    return true;
}

bool AppPackager::IsSharedApp(const std::string &hapPath, const std::string &hspPath)
{
    if (!hapPath.empty()) {
        return false;
    }
    if (hspPath.empty()) {
        return false;
    }
    std::list<std::string> tmpHspPathList;
    if (CompatibleProcess(hspPath, tmpHspPathList, Constants::HSP_SUFFIX)
        && VerifyIsSharedApp(tmpHspPathList)) {
            isSharedApp_ = true;
        return true;
    }
    return false;
}

bool AppPackager::VerifyIsAppService(const std::list<std::string>& modulePathList)
{
    if (modulePathList.empty()) {
        LOGE("%s", PackingToolErrMsg::BUNDLE_TYPE_APPSERVICE_INVALID.toStringWithArgs("Module path list is empty").c_str());
        return false;
    }

    if (!ModuleJsonUtils::IsModuleHap(modulePathList.front())) {
        LOGD("Module is FA");
        return false;
    }

    for (const std::string& modulePath : modulePathList) {
        HapVerifyInfo hapVerifyInfo;
        if (!ModuleJsonUtils::GetStageHapVerifyInfo(modulePath, hapVerifyInfo)) {
            return false;
        }
        if (hapVerifyInfo.GetBundleType() != Constants::BUNDLE_TYPE_APP_SERVICE) {
            return false;
        }
    }
    return true;
}

bool AppPackager::IsAppService(const std::string &hapPath, const std::string &hspPath)
{
    if (!hapPath.empty()) {
        std::list<std::string> tmpHapPathList;
        if (CompatibleProcess(hapPath, tmpHapPathList, Constants::HAP_SUFFIX)
            && VerifyIsAppService(tmpHapPathList)) {
                isAppService_ = true;
            return true;
        }
    }
    if (hspPath.empty()) {
        return false;
    }
    std::list<std::string> tmpHspPathList;
    if (CompatibleProcess(hspPath, tmpHspPathList, Constants::HSP_SUFFIX)
        && VerifyIsAppService(tmpHspPathList)) {
            isAppService_ = true;
        return true;
    }
    return false;
}

bool AppPackager::IsSkillApp(const std::string &hapPath, const std::string &hspPath)
{
    if (!hapPath.empty() || hspPath.empty()) {
        return false;
    }
    std::list<std::string> tmpHspPathList;
    if (!CompatibleProcess(hspPath, tmpHspPathList, Constants::HSP_SUFFIX)) {
        return false;
    }
    std::string bundleType;
    if (!GetFirstBundleTypeFromPathList(tmpHspPathList, bundleType)) {
        return false;
    }
    if (bundleType == Constants::TYPE_SKILL) {
        isSkillApp_ = true;
        return true;
    }
    return false;
}

bool AppPackager::CheckInputModulePath(const std::string &hapPath, const std::string &hspPath)
{
    bool isSharedApp = IsSharedApp(hapPath, hspPath);
    bool isAppService = IsAppService(hapPath, hspPath);
    std::list<std::string> tmpHspPathList;
    bool hasSkillBundleTypeHsp = false;
    if (!hspPath.empty() && CompatibleProcess(hspPath, tmpHspPathList, Constants::HSP_SUFFIX) &&
        !tmpHspPathList.empty()) {
        std::string bundleType;
        if (!GetFirstBundleTypeFromPathList(tmpHspPathList, bundleType)) {
            return false;
        }
        hasSkillBundleTypeHsp = bundleType == Constants::TYPE_SKILL;
    }
    if (!CheckSkillAppConstraints(hapPath, hspPath, tmpHspPathList)) {
        return false;
    }
    bool isSkillAppResult = hasSkillBundleTypeHsp && hapPath.empty();
    if (hapPath.empty() && !isSharedApp && !isAppService && !isSkillAppResult) {
        LOGW("CheckInputModulePath hap-path is empty.");
        return false;
    }
    if (hspPath.empty() && isAppService) {
        LOGW("CheckInputModulePath hsp-path is empty.");
        return false;
    }
    return true;
}

bool AppPackager::GetAndCheckOutPath(std::string &outPath)
{
    if (parameterMap_.find(Constants::PARAM_OUT_PATH) == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input out-path are null.").c_str());
        return false;
    }
    outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    if (outPath.empty()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input out-path are empty.").c_str());
        return false;
    }
    if (outPath.find('.') == std::string::npos ||
        outPath.substr(outPath.size() - Constants::APP_SUFFIX_LENGTH) != Constants::APP_SUFFIX) {
        LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs("out-path must end with .app.").c_str());
        return false;
    }
    return true;
}

bool AppPackager::GetAndCheckHapPathAndHspPath(std::string &hapPath, std::string &hspPath)
{
    if (parameterMap_.find(Constants::PARAM_HAP_PATH) == parameterMap_.end() &&
        parameterMap_.find(Constants::PARAM_HSP_PATH) == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input hap-path or hsp-path are all null.").c_str());
        return false;
    }
    if (parameterMap_.find(Constants::PARAM_HAP_PATH) != parameterMap_.end()) {
        hapPath = parameterMap_.at(Constants::PARAM_HAP_PATH);
    }
    if (parameterMap_.find(Constants::PARAM_HSP_PATH) != parameterMap_.end()) {
        hspPath = parameterMap_.at(Constants::PARAM_HSP_PATH);
    }
    if (hapPath.empty() && hspPath.empty()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input hap-path or hsp-path are all empty.").c_str());
        return false;
    }
    if (!CheckBundleTypeConsistency(hapPath, hspPath)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("bundleType is inconsistent.").c_str());
        return false;
    }
    if (!CheckInputModulePath(hapPath, hspPath)) {
        LOGW("input hap-path or hsp-path is invalid.");
    }
    if (!hapPath.empty() && !CompatibleProcess(hapPath, formattedHapPathList_, Constants::HAP_SUFFIX)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("hap-path is invalid.").c_str());
        return false;
    }
    if (!hspPath.empty() && !CompatibleProcess(hspPath, formattedHspPathList_, Constants::HSP_SUFFIX)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("hsp-path is invalid.").c_str());
        return false;
    }
    return true;
}

bool AppPackager::GetAndCheckPackInfoPath(std::string &packInfoPath)
{
    if (parameterMap_.find(Constants::PARAM_PACK_INFO_PATH) == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input pack-info-path is null.").c_str());
        return false;
    }
    packInfoPath = parameterMap_.at(Constants::PARAM_PACK_INFO_PATH);
    if (packInfoPath.empty()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("input pack-info-path is empty.").c_str());
        return false;
    }
    if (!fs::is_regular_file(packInfoPath) || fs::path(packInfoPath).filename() != Constants::PACK_INFO) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("pack-info-path is invalid.").c_str());
        return false;
    }
    return true;
}

bool AppPackager::CheckSignaturePath()
{
    auto it = parameterMap_.find(Constants::PARAM_SIGNATURE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!fs::is_regular_file(it->second)) {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("signature-path is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::CheckCertificatePath()
{
    auto it = parameterMap_.find(Constants::PARAM_CERTIFICATE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!fs::is_regular_file(it->second)) {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("certificate-path is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::CheckEntrycardPath()
{
    auto it = parameterMap_.find(Constants::PARAM_ENTRYCARD_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!CompatibleProcess(it->second, formattedEntryCardPathList_, Constants::PNG_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("entrycard-path is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::CheckPackResPath()
{
    auto it = parameterMap_.find(Constants::PARAM_PACK_RES_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!IsPathValid(it->second, true, Constants::FILE_PACK_RES)) {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("pack-res-path is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::CheckPacJsonPath()
{
    auto it = parameterMap_.find(Constants::PARAM_PAC_JSON_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!IsFileMatch(it->second, Constants::PAC_JSON)) {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("pac-json-path is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::GetAndCheckReplacePackInfo()
{
    auto it = parameterMap_.find(Constants::PARAM_REPLACE_PACK_INFO);
    if (it != parameterMap_.end()) {
        if (it->second == Constants::TRUE_STRING) {
            isReplacePackInfo_ = true;
            return true;
        } else if (it->second == Constants::FALSE_STRING) {
            isReplacePackInfo_ = false;
            return true;
        } else {
            LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("--replace-pack-info is invalid.").c_str());
            return false;
        }
    }
    isReplacePackInfo_ = true;
    return true;
}

bool AppPackager::IsVerifyValidInAppMode()
{
    std::string hapPath;
    std::string hspPath;
    if (!GetAndCheckHapPathAndHspPath(hapPath, hspPath)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("GetAndCheckHapPathAndHspPath failed!").c_str());
        return false;
    }

    std::string packInfoPath;
    if (!GetAndCheckPackInfoPath(packInfoPath)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("GetAndCheckPackInfoPath failed!").c_str());
        return false;
    }

    if (!CheckSignaturePath() || !CheckCertificatePath() || !CheckEntrycardPath() || !CheckPackResPath()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs(
            "CheckSignaturePath or CheckCertificatePath or CheckEntrycardPath or CheckPackResPath failed!").c_str());
        return false;
    }

    if (!CheckPacJsonPath()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("CheckPacJsonPath failed!").c_str());
        return false;
    }

    std::string outPath;
    if (!GetAndCheckOutPath(outPath)) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("GetAndCheckOutPath failed!").c_str());
        return false;
    }
    std::string force;
    if (parameterMap_.find(Constants::PARAM_FORCE) != parameterMap_.end()) {
        force = parameterMap_.at(Constants::PARAM_FORCE);
    }
    if (!GetAndCheckReplacePackInfo()) {
        LOGE("%s", PackingToolErrMsg::APP_MODE_ARGS_INVALID.toStringWithArgs("GetAndCheckReplacePackInfo failed!").c_str());
        return false;
    }
    return IsOutPathValid(outPath, force, Constants::APP_SUFFIX);
}

bool AppPackager::PrepareDirectoriesAndFiles(const std::string outPath)
{
    fs::path tempPath;
    fs::path hspTempDirPath;
    if (fs::exists(fs::path(outPath).parent_path().parent_path()) &&
        fs::path(outPath).parent_path().parent_path() != fs::path("/")) {
        tempPath = fs::path(outPath).parent_path().parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
            Utils::GenerateUUID());
        hspTempDirPath = fs::path(outPath).parent_path().parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
            Utils::GenerateUUID());
    } else {
        tempPath = fs::path(outPath).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
            Utils::GenerateUUID());
        hspTempDirPath = fs::path(outPath).parent_path() / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
            Utils::GenerateUUID());
    }
    if (!fs::exists(tempPath)) {
        fs::create_directories(tempPath);
    }
    if (!fs::exists(hspTempDirPath)) {
        fs::create_directories(hspTempDirPath);
    }
    if (!CompressHapAndHspFiles(tempPath, hspTempDirPath)) {
        if (fs::exists(tempPath)) {
            fs::remove_all(tempPath);
        }
        if (fs::exists(hspTempDirPath)) {
            fs::remove_all(hspTempDirPath);
        }
        LOGE("%s", PackingToolErrMsg::COMPRESS_APP_FAILED.toStringWithArgs("AppPackager::CompressHapAndHspFiles failed.").c_str());
        return false;
    }
    return true;
}

bool AppPackager::CompressHapAndHspFiles(const fs::path &tempPath, const fs::path &hspTempDirPath)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_HAP_PATH);
    std::list<std::string> fileList;
    if (it != parameterMap_.end()) {
        for (const auto& hapPathItem : formattedHapPathList_) {
            fs::path hapFile = hapPathItem;
            if (!isReplacePackInfo_) {
                fileList.push_back(hapFile.string());
                continue;
            }
            fs::path hapTempPath = tempPath / hapFile.filename();
            fs::path hapUnzipTempPath = tempPath / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
                Utils::GenerateUUID());
            fileList.push_back(hapTempPath.string());
            it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
            if (it != parameterMap_.end()) {
                CompressPackinfoIntoHap(hapPathItem, hapUnzipTempPath, hapTempPath.string(), it->second);
            }
        }
    }
    it = parameterMap_.find(Constants::PARAM_HSP_PATH);
    if (it != parameterMap_.end()) {
        for (const auto& hspPathItem : formattedHspPathList_) {
            fs::path hspFile = hspPathItem;
            if (!isReplacePackInfo_) {
                fileList.push_back(hspFile.string());
                continue;
            }
            fs::path hspTempPath = hspTempDirPath / hspFile.filename();
            fs::path hspUnzipTempPath = tempPath / ((Constants::COMPRESSOR_APP_TEMP_DIR) +
            Utils::GenerateUUID());
            fileList.push_back(hspTempPath.string());
            it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
            if (it != parameterMap_.end()) {
                CompressPackinfoIntoHap(hspPathItem, hspUnzipTempPath, hspTempPath.string(), it->second);
            }
        }
    }
    if (!ModuleJsonUtils::CheckHapsIsValid(fileList, isSharedApp_)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("AppPackager::CheckHapsIsValid verify failed.").c_str());
        return false;
    }
    if (!ModuleJsonUtils::GetHapVerifyInfosMapfromFileList(fileList, hapVerifyInfoMap_)) {
        LOGE("%s", PackingToolErrMsg::CHECK_HAP_INVALID.toStringWithArgs("AppPackager::GetHapVerifyInfosMapfromFileList failed.").c_str());
        return false;
    }
    if (!AddHapListToApp(fileList)) {
        zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
        LOGE("%s", PackingToolErrMsg::COMPRESS_APP_FAILED.toStringWithArgs("AppPackager::AddHapListToApp failed.").c_str());
        return false;
    }
    if (fs::exists(tempPath)) {
        fs::remove_all(tempPath);
    }
    if (fs::exists(hspTempDirPath)) {
        fs::remove_all(hspTempDirPath);
    }
    return true;
}

bool AppPackager::AddHapListToApp(const std::list<std::string> &fileList)
{
    for (const auto& hapPath : fileList) {
        HapVerifyInfo hapVerifyInfo;
        if (ModuleJsonUtils::IsModuleHap(hapPath)) {
            if (!ModuleJsonUtils::GetStageHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGW("GetStageHapVerifyInfo failed! hapPath:%s", hapPath.c_str());
            }
        } else {
            if (!ModuleJsonUtils::GetFaHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGW("GetFaHapVerifyInfo failed! hapPath:%s", hapPath.c_str());
            }
        }
        if (hapVerifyInfo.IsDebug()) {
            zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_0);
        }
        zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_DEFLATED);
        if (zipWrapper_.AddFileOrDirectoryToZip(hapPath, fs::path(hapPath).filename().string()) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
            zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
            zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_STORED);
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "AppPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
            return false;
        }
        zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
        zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_STORED);
    }
    return true;
}

bool AppPackager::CompressOtherFiles()
{
    std::string moduleName;
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_ENTRYCARD_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        std::string entryCardPath = Constants::ENTRYCARD_NAME + Constants::LINUX_FILE_SEPARATOR +
            moduleName + Constants::LINUX_FILE_SEPARATOR +
            Constants::ENTRYCARD_BASE_NAME + Constants::LINUX_FILE_SEPARATOR +
            Constants::ENTRYCARD_SNAPSHOT_NAME + Constants::LINUX_FILE_SEPARATOR;
        for (auto itemFormattedEntryCardPath : formattedEntryCardPathList_) {
            if (zipWrapper_.AddFileOrDirectoryToZip(itemFormattedEntryCardPath, entryCardPath +
                fs::path(itemFormattedEntryCardPath).filename().string()) !=
                ZipErrCode::ZIP_ERR_SUCCESS) {
                return false;
            }
        }
    }
    it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    if (it != parameterMap_.end()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::PACK_INFO) != ZipErrCode::ZIP_ERR_SUCCESS) {
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_SIGNATURE_PATH);
    if (it != parameterMap_.end()) {
        std::string zipPath = Constants::NULL_DIR_NAME;
        zipPath = fs::path(it->second).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_CERTIFICATE_PATH);
    if (it != parameterMap_.end()) {
        std::string zipPath = Constants::NULL_DIR_NAME;
        zipPath = fs::path(it->second).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_PAC_JSON_PATH);
    if (it != parameterMap_.end()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::PAC_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "AppPackager::CompressOtherFiles: zipWrapper pac.json failed!").c_str());
            return false;
        }
    }
    return true;
}

bool AppPackager::CompressAppMode()
{
    std::string outPath;
    if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
        outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    }
    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_APP_FAILED.toStringWithArgs(
            "AppPackager::CompressAppMode: zipWrapper Open failed!").c_str());
        return ERR_INVALID_VALUE;
    }
    if (!PrepareDirectoriesAndFiles(outPath)) {
        return false;
    }
    if (!CompressOtherFiles()) {
        return false;
    }
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_RES_PATH);
    if (it != parameterMap_.end()) {
        std::string zipPath = Constants::NULL_DIR_NAME;
        zipPath = fs::path(it->second).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            return false;
        }
    }
    zipWrapper_.Close();
    if (!ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid(parameterMap_, hapVerifyInfoMap_)) {
        LOGE("%s", PackingToolErrMsg::APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID.toStringWithArgs(
            "AppPackager::CompressAppMode: CheckAppAtomicServiceCompressedSizeValid() failed!").c_str());
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
