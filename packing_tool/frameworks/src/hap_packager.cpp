/*
 * Copyright (c) 2024-2026 Huawei Device Co., Ltd.
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
#include "skill_pack_helper.h"
#include "utils.h"
#include "incremental_pack.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string NAME = "name";
const std::string REQUEST_PERMISSIONS = "requestPermissions";
const std::string PERMISSION_SUPPORT_PLUGIN = "ohos.permission.kernel.SUPPORT_PLUGIN";
const std::string COMPRESS_NATIVE_LIBS = "compressNativeLibs";
const bool DEFAULT_COMPRESS_NATIVE_LIBS = false;
const bool DEFAULT_EXTRACT_NATIVE_LIBS = true;

}
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
        std::string outPath;
        if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
            outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
        }
        if (fs::exists(outPath)) {
            fs::remove_all(outPath);
        }
        LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs("Compress hap failed.").c_str());
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t HapPackager::PostProcess()
{
    if (generateBuildHash_) {
        if (!CompressHap()) {
            std::string outPath;
            if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
                outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
            }
            if (fs::exists(outPath)) {
                fs::remove_all(outPath);
            }
            LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs("Compress hap failed.").c_str());
            return ERR_INVALID_VALUE;
        }
    }
    return ERR_OK;
}

bool HapPackager::IsVerifyValidInHapCommonMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--json-path is empty.").c_str());
        return false;
    }
    jsonPath_ = it->second;
    if (!IsPathValid(it->second, true, Constants::CONFIG_JSON)
        && !IsPathValid(it->second, true, Constants::MODULE_JSON)) {
           
      LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
      "--json-path must be the config.json file or module.json file.").c_str());
        return false;
    }
    if (!IsValidRpcid() || !IsValidPackInfo()) {
        return false;
    }
    if (!IsPathParamValid(Constants::PARAM_EXIST_SRC_PATH, true, Constants::HAP_SUFFIX)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "The value of --exist-src-path must be a file with the .hap suffix.").c_str());
        return false;
    }
    if (!CheckLibPathRetainParam()) {
        return false;
    }
    if (!IsCompressLevelValid()) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_PROFILE_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PROFILE_NAME) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--profile-path must be the CAPABILITY.profile file.").c_str());
            return false;
        }
    }
    if (!Compatible(Constants::PARAM_ABC_PATH, formattedAbcPathList_, Constants::ABC_SUFFIX)) {
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_DIR_LIST);
    if (it != parameterMap_.end() && !SplitDirList(it->second, formatedDirList_)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--dir-list is invalid.").c_str());
        return false;
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PKG_CONTEXT_JSON) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--pkg-context-path file must be the pkgContextInfo.json file.").c_str());
            return false;
        }
    }
    if (!CheckPkgSdkInfoParam()) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--pkg-sdk-info-path value is invalid.").c_str());
        return false;
    }
    return true;
}

bool HapPackager::Compatible(const std::string &paramPath, std::list<std::string> &fileList,
    const std::string &suffix)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty() && !CompatibleProcess(it->second,
        fileList, suffix)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            (paramPath + " is invalid.").c_str()).c_str());
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
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--index-path must be the resources.index file.").c_str());
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
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--ets-path is invalid.").c_str());
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
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--rpcid-path is not a file.").c_str());
            return false;
        }
        if (fs::path(filePath).filename().string() != Constants::RPCID_SC) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--rpcid-path must be the rpcid.sc file.").c_str());
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
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--pack-info-path is not a file.").c_str());
            return false;
        }
        if (fs::path(filePath).filename().string() != Constants::PACK_INFO) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--pack-info-path must be the pack.info file.").c_str());
            return false;
        }
    }
    return true;
}

bool HapPackager::IsHapPathValid()
{
    if (IsHapPathValid(Constants::PARAM_MAPLE_SO_DIR)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--maple-so-dir is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_LIB_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--lib-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_HNP_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--hnp-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_RES_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--res-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_RESOURCES_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--resources-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_ASSETS_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--assets-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_SHAREDLIBS_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--shared-libs-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_AN_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--an-path is invalid.").c_str());
        return false;
    }
    if (IsHapPathValid(Constants::PARAM_SKILLS_PATH)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--skills-path is invalid.").c_str());
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
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "Failed to parse --json-path.").c_str());
        return false;
    }
    if (JsonUtils::IsModuleJson(jsonPath_)) {
        if (!CheckStageHap(jsonPath_)) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
                "CheckStageHap failed.").c_str());
            return false;
        }
        std::string moduleType;
        if (!moduleJson_.GetStageModuleType(moduleType)) {
            LOGW("GetStageModuleType failed");
        }
        if (moduleType == Constants::TYPE_SKILL) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "moduleType 'skill' is not allowed in HAP mode, use HSP mode instead.").c_str());
            return false;
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
        moduleJson_.GetStageCompressNativeLibs(compressNativeLibs_);
        
        // Check kernel permission compression validation
        if (!CheckKernelPermissionCompression()) {
            return false;
        }
        
        if (!CompressHapModeForModule(jsonPath_) || !BuildHash(buildHashFinish_, generateBuildHash_,
            parameterMap_, jsonPath_)) {
            return false;
        }
        if (Constants::TYPE_APP_PLUGIN == bundleType) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
                "hap can not plugin.").c_str());
            return false;
        } else {
            if (!IsPluginHost()) {
                LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
                    "plugin package cannot be the host.").c_str());
                return false;
            }
        }
    } else {
        if (!CompressHapMode() || !BuildHash(buildHashFinish_, generateBuildHash_, parameterMap_, jsonPath_)) {
            return false;
        }
    }
    return true;
}

bool HapPackager::IsPluginHost()
{
    std::unique_ptr<PtJson> moduleObj;
    if (!moduleJson_.GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::CHECK_APP_PLUGIN_FAILED.toStringWithArgs(
            "GetModuleObject failed!").c_str());
        return false;
    }
    if (moduleObj->Contains(REQUEST_PERMISSIONS.c_str())) {
        std::unique_ptr<PtJson> requestPermissionsObj;
        if (moduleObj->GetArray(REQUEST_PERMISSIONS.c_str(), &requestPermissionsObj) != Result::SUCCESS) {
            LOGW("Module node get %s array node failed!", REQUEST_PERMISSIONS.c_str());
            return true;
        }
        if (IsPermissionSupportPlugin(requestPermissionsObj)) {
            return CheckPkgContext();
        }
    }
    return true;
}

bool HapPackager::IsPermissionSupportPlugin(std::unique_ptr<PtJson>& requestPermissionsObj)
{
    if (requestPermissionsObj == nullptr) {
        LOGE("%s", PackingToolErrMsg::CHECK_APP_PLUGIN_FAILED.toStringWithArgs(
            "requestPermissionsObj nullptr!").c_str());
        return false;
    }
    for (int32_t i = 0; i < requestPermissionsObj->GetSize(); i++) {
        std::unique_ptr<PtJson> requestPermissionObj = requestPermissionsObj->Get(i);
        if (requestPermissionObj->Contains(NAME.c_str())) {
            std::string requestPermissionName;
            if (requestPermissionObj->GetString(NAME.c_str(), &requestPermissionName) != Result::SUCCESS) {
                LOGW("get %s failed!", NAME.c_str());
                continue;
            }
            if (requestPermissionName == PERMISSION_SUPPORT_PLUGIN) {
                return true;
            }
        }
    }
    return false;
}

bool HapPackager::CheckPkgContext()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end()) {
        const std::string filePath = it->second;
        if (!fs::is_regular_file(filePath) ||
            fs::path(filePath).filename().string() != Constants::PKG_CONTEXT_JSON) {
            LOGE("%s", PackingToolErrMsg::CHECK_PKG_CONTEXT_FAILED.toStringWithArgs(
                "host must include pkgContextInfo.json").c_str());
            return false;
        }
        return true;
    }
    LOGE("%s", PackingToolErrMsg::CHECK_PKG_CONTEXT_FAILED.toStringWithArgs(
        "no have pkgContextInfo.json.").c_str());
    return false;
}

bool HapPackager::CheckStageHap(const std::string &jsonPath)
{
    if (!moduleJson_.CheckStageAsanTsanEnabledValid()) {
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            "CheckStageAsanTsanEnabledValid failed.").c_str());
        return false;
    }

    if (!moduleJson_.CheckStageAtomicService()) {
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            "CheckStageAtomicService failed.").c_str());
        return false;
    }

    if (!moduleJson_.CheckQuerySchemes()) {
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            "CheckQuerySchemes failed.").c_str());
        return false;
    }

    if (!moduleJson_.CheckDeduplicateHar()) {
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            "CheckDeduplicateHar failed.").c_str());
        return false;
    }

    return true;
}

bool HapPackager::CompressHapModeForModule(const std::string &jsonPath)
{
    if (!moduleJson_.GetStageModuleName(moduleName_)) {
        LOGE("%s", PackingToolErrMsg::PARSE_AND_MODIFY_MODULEJSON_FAILED.toStringWithArgs(
            "HapPackager::Process: GetStageModuleName failed!").c_str());
    }
    if (!moduleJson_.GetStageDeviceTypes(deviceTypes_)) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
            "HapPackager::Process: GetStageDeviceTypes failed!").c_str());
    }
    IncrementalPack::CopyExistSrcFile(parameterMap_);
    if (!OpenZipWrapper()) {
        return false;
    }

    std::string jsonString = moduleJson_.ToString();
    if (!jsonString.empty()) {
        if (zipWrapper_.WriteStringToZip(jsonString, Constants::MODULE_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper WriteStringToZip failed!").c_str());
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
        {Constants::PARAM_PACK_INFO_PATH, Constants::PACK_INFO},
        {Constants::PARAM_PKG_SDK_INFO_PATH, Constants::PKG_SDK_INFO_JSON}
    };
    for (auto& item : paramFileMap) {
        if (!AddCommonFileOrDirectoryToZip(item.first, item.second)) {
            return false;
        }
    }
    IncrementalPack::DeleteExistSrcTempDir();
    if (!CompressSkillsDirectory()) {
        return false;
    }
    if (!AddParamFileToZip() || !AddResFileAndDirLsitToZip() || !AddPkgAndBinFileToZipForStageMaode()) {
        return false;
    }
    return CompressHapModeMultiple();
}

bool HapPackager::CompressHapMode()
{
    if (!moduleJson_.GetFaModuleName(moduleName_)) {
        LOGE("%s", PackingToolErrMsg::PARSE_AND_MODIFY_MODULEJSON_FAILED.toStringWithArgs(
            "HapPackager::Process: GetFaModuleName failed!").c_str());
    }
    if (!moduleJson_.GetFaDeviceTypes(deviceTypes_)) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
            "HapPackager::Process: GetFaDeviceTypes failed!").c_str());
    }

    if (!OpenZipWrapper()) {
        return false;
    }

    std::string jsonString = moduleJson_.ToString();
    if (!jsonString.empty()) {
        if (zipWrapper_.WriteStringToZip(jsonString, Constants::CONFIG_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper WriteStringToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
            return false;
        }
    }

    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_MAPLE_SO_DIR);
    if (it != parameterMap_.end() && formattedSoPathList_.size() == 0 && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::SO_DIR) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
        LOGE("%s", PackingToolErrMsg::COMPRESS_HAP_FAILED.toStringWithArgs(
            "HapPackager::Process: zipWrapper Open failed!").c_str());
        return false;
    }
    return true;
}

bool HapPackager::AddCommonFileOrDirectoryToZip(const std::string &paramPath, const std::string &targetPath)
{
    bool isCompress = (paramPath == Constants::PARAM_LIB_PATH && compressNativeLibs_);
    ZipLevel zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT;
    if (isCompress) {
        zipLevel = ZipLevel::ZIP_LEVEL_1;
        auto it = parameterMap_.find(Constants::PARAM_COMPRESS_LEVEL);
        if (it != parameterMap_.end() && !it->second.empty()) {
            zipLevel = zipWrapper_.StringToZipLevel(it->second);
        }
    }

    if (paramPath == Constants::PARAM_LIB_PATH &&
        IncrementalPack::IsIncrementalMode(parameterMap_)) {
        return IncrementalPack::IncrementalPackProcess(paramPath, zipWrapper_);
    }

    auto it = parameterMap_.find(paramPath);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second,
                                                targetPath,
                                                isCompress,
                                                zipLevel) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
            return false;
        }
    }

    if (!formatedDirList_.empty()) {
        for (const auto& dirPath : formatedDirList_) {
            std::string baseDir = fs::path(dirPath).filename().string();
            if (zipWrapper_.AddFileOrDirectoryToZip(dirPath, baseDir) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
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
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "HapPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!").c_str());
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        ModuleJson moduleJson;
        if (!moduleJson.ParseFromFile(it->second)) {
            LOGE("%s", PackingToolErrMsg::CHECK_PKG_CONTEXT_FAILED.toStringWithArgs(
                "HapPackager::Process: moduleJson Read failed!").c_str());
            return false;
        }
        std::string jsonString = moduleJson.ToString();
        if (!jsonString.empty()) {
            if (zipWrapper_.WriteStringToZip(jsonString, Constants::PKG_CONTEXT_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    "HapPackager::Process: zipWrapper WriteStringToZip failed!").c_str());
                return false;
            }
        } else {
            LOGE("%s", PackingToolErrMsg::CHECK_PKG_CONTEXT_FAILED.toStringWithArgs(
                "HapPackager::Process: jsonFile error!").c_str());
            return false;
        }
    }
    return true;
}

bool HapPackager::CheckLibPathRetainParam()
{
    auto it = parameterMap_.find(Constants::PARAM_LIB_PATH_RETAIN);
    if (it != parameterMap_.end() && it->second != "false" && it->second != "true") {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "--lib-path-retain parameter value must be either 'true' or 'false'.").c_str());
        return false;
    }
    return true;
}

bool HapPackager::CheckPkgSdkInfoParam()
{
    auto it = parameterMap_.find(Constants::PARAM_PKG_SDK_INFO_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!IsFileMatch(it->second, Constants::PKG_SDK_INFO_JSON)) {
            LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
                "--pkg-sdk-info-path value is invalid.").c_str());
            return false;
        }
    }
    return true;
}

bool HapPackager::CheckKernelPermissionCompression()
{
    // Only check for module.json files (not config.json for FA mode)
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        return true;  // No json path, skip validation
    }

    std::string jsonPath = it->second;
    if (!IsPathValid(jsonPath, true, Constants::MODULE_JSON)) {
        return true;  // Not a module.json file, skip validation
    }

    // Parse module.json to check for kernel permission
    if (!moduleJson_.ParseFromFile(jsonPath)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            ("Failed to parse module.json: " + jsonPath).c_str()).c_str());
        return false;
    }

    // Check if executableBinaries exists (has kernel permission)
    bool hasKernelPermission = moduleJson_.HasExecutableBinaries();
    if (!hasKernelPermission) {
        return true;  // No kernel permission, no need to check
    }

    // Get compressNativeLibs and extractNativeLibs values
    bool compressNativeLibs = DEFAULT_COMPRESS_NATIVE_LIBS;
    bool extractNativeLibs = DEFAULT_EXTRACT_NATIVE_LIBS;

    if (!moduleJson_.GetStageCompressNativeLibs(compressNativeLibs)) {
        LOGW("Failed to get compressNativeLibs, using default value: false");
        compressNativeLibs = DEFAULT_COMPRESS_NATIVE_LIBS;
    }

    if (!moduleJson_.GetStageExtractNativeLibs(extractNativeLibs)) {
        LOGW("Failed to get extractNativeLibs, using default value: true");
        extractNativeLibs = DEFAULT_EXTRACT_NATIVE_LIBS;
    }

    // Validate: if has kernel permission, at least one of compress/extract must be true
    if (!compressNativeLibs && !extractNativeLibs) {
        LOGE("%s", PackingToolErrMsg::CHECK_STAGE_HAP_FAILED.toStringWithArgs(
            "When executableBinaryPaths is configured in module.json, "
            "at least one of compressNativeLibs or extractNativeLibs must be true.").c_str());
        return false;
    }

    return true;
}

bool HapPackager::CompressSkillsDirectory()
{
    std::list<std::map<std::string, std::string>> skillProfiles;
    if (!moduleJson_.GetSkillProfiles(skillProfiles)) {
        LOGE("%s", PackingToolErrMsg::HAP_MODE_ARGS_INVALID.toStringWithArgs(
            "Failed to get skillProfiles from module.json.").c_str());
        return false;
    }
    std::string bundleType;
    moduleJson_.GetStageBundleType(bundleType);
    return SkillPackHelper::CompressSkillFiles(skillProfiles, parameterMap_, bundleType, jsonPath_,
        [this](const std::string &sourcePath, const std::string &zipPath) {
            return zipWrapper_.AddFileOrDirectoryToZip(sourcePath, zipPath) == ZipErrCode::ZIP_ERR_SUCCESS;
        });
}
} // namespace AppPackingTool
} // namespace OHOS
