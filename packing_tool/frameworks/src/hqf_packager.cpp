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

#include "hqf_packager.h"

#include "json/json_utils.h"
#include "json/module_json.h"
#include "log.h"
#include "utils.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
HqfPackager::HqfPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t HqfPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t HqfPackager::PreProcess()
{
    auto it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs("--out-path is empty.").c_str());
        return ERR_INVALID_VALUE;
    } else if (!Utils::EndsWith(it->second, Constants::HQF_SUFFIX)) {
        LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs(
            "--out-path file must end with .hqf.").c_str());
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_FORCE);
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    fs::path outPath(parameterMap_.at(Constants::PARAM_OUT_PATH));
    if (it->second != "true") {
        if (fs::exists(outPath) && fs::is_regular_file(outPath)) {
            LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs(
                "--out-path file already exist, but --force is not 'true'.").c_str());
            return ERR_INVALID_VALUE;
        }
    }

    if (!fs::exists(outPath.parent_path())) {
        fs::create_directories(outPath.parent_path());
    }
    
    it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs(
            "--json-path is empty, must input patch.json file when pack hqf file.").c_str());
        return ERR_INVALID_VALUE;
    } else {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_JSON_PATH), true, Constants::JSON_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs("--json-path is invalid.").c_str());
            return ERR_INVALID_VALUE;
        }
    }
    it = parameterMap_.find(Constants::PARAM_LIB_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_LIB_PATH), false)) {
            LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs("--lib-path is invalid.").c_str());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_RESOURCES_PATH), false)) {
            LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs(
                "--resources-path is invalid.").c_str());
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_ETS_PATH), false)) {
            LOGE("%s", PackingToolErrMsg::HQF_MODE_ARGS_INVALID.toStringWithArgs("--ets-path is invalid.").c_str());
            return ERR_INVALID_VALUE;
        }
    }
    return ERR_OK;
}

int32_t HqfPackager::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    zipWrapper_.Open(outPath);
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    ModuleJson moduleJson;
    if (moduleJson.ParseFromFile(it->second)) {
        if (zipWrapper_.WriteStringToZip(moduleJson.ToString(), Constants::PATCH_JSON) !=
                ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                "ZipWrapper WriteStringToZip failed!").c_str());
            return ERR_INVALID_VALUE;
        }
    } else {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Input patch.json is invalid.").c_str());
        return ERR_INVALID_VALUE;
    }
    it = parameterMap_.find(Constants::PARAM_LIB_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::LIB_PATH) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zipWrapper AddFileOrDirectoryToZip failed!").c_str());
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_PATH) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zipWrapper AddFileOrDirectoryToZip failed!").c_str());
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::ETS_PATH) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zipWrapper AddFileOrDirectoryToZip failed!").c_str());
        return ERR_INVALID_VALUE;
    }
    zipWrapper_.Close();
    return ERR_OK;
}

int32_t HqfPackager::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS
