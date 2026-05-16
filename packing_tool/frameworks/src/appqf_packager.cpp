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

#include "appqf_packager.h"

#include "json/json_utils.h"
#include "json/patch_json.h"
#include "hqf_verify.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
APPQFPackager::APPQFPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t APPQFPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

bool APPQFPackager::CheckHqfList(const std::list<std::string>& hqfList)
{
    if (hqfList_.size() == 0) {
        LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs("--hqf-list is empty.").c_str());
        return false;
    }

    std::vector<HqfInfo> hqfInfos;
    for (const std::string& path : hqfList_) {
        fs::path hqfPath(path);
        if (!fs::exists(hqfPath) || !fs::is_regular_file(hqfPath)) {
            LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
                ("Input file " + hqfPath.string() + " not valid").c_str()).c_str());
            return false;
        }
        std::string content;
        PatchJson patchJson;
        HqfInfo hqfInfo;
        if (!ZipUtils::GetFileContentFromZip(hqfPath, Constants::PATCH_JSON, content)) {
            LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
                ("Get input file " + hqfPath.string() + " failed").c_str()).c_str());
            return false;
        }
        
        if (!patchJson.ParseFromString(content)) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("Parse " + hqfPath.string() + " patch.json failed").c_str()).c_str());
            return false;
        }

        if (!patchJson.GetHqfInfo(hqfInfo)) {
            LOGE("%s", PackingToolErrMsg::CHECK_HQF_INVALID.toStringWithArgs(
                ("Get " + hqfPath.string() + " HqfInfo failed").c_str()).c_str());
            return false;
        }
        hqfInfos.push_back(hqfInfo);
    }
    if (!HQFVerify::CheckHQFIsValid(hqfInfos)) {
        return false;
    }
    return true;
}

int32_t APPQFPackager::PreProcess()
{
    auto it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs("--out-path is empty.").c_str());
        return ERR_INVALID_VALUE;
    } else {
        std::string outputPath = it->second;
        std::transform(outputPath.begin(), outputPath.end(), outputPath.begin(), ::tolower);
        if (!Utils::EndsWith(outputPath, Constants::APPQF_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs(
                "--out-path must end with .appqf.").c_str());
            return ERR_INVALID_VALUE;
        }
    }

    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    it = parameterMap_.find(Constants::PARAM_FORCE);
    fs::path outPath(parameterMap_.at(Constants::PARAM_OUT_PATH));
    if (it == parameterMap_.end() || it->second == "false") {
        if (fs::exists(outPath) && fs::is_regular_file(outPath)) {
            LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs(
                "--out-path file already exist, but --force is not 'true'.").c_str());
            return ERR_INVALID_VALUE;
        }
    }
    if (!fs::exists(outPath.parent_path())) {
        fs::create_directories(outPath.parent_path());
    }
    it = parameterMap_.find(Constants::PARAM_HQF_LIST);
    if (it == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs("--hqf-list is empty.").c_str());
        return ERR_INVALID_VALUE;
    }
    if (!CompatibleProcess(it->second, hqfList_, Constants::HQF_SUFFIX)) {
        LOGE("%s", PackingToolErrMsg::APPQF_MODE_ARGS_INVALID.toStringWithArgs("--hqf-list is invalid.").c_str());
        return ERR_INVALID_VALUE;
    }

    if (!CheckHqfList(hqfList_)) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t APPQFPackager::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    zipWrapper_.Open(outPath);
    for (const std::string& path : hqfList_) {
        fs::path entryPath(path);
        std::string filePath = entryPath.filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(path, filePath) !=  ZipErrCode::ZIP_ERR_SUCCESS) {
            return ERR_INVALID_VALUE;
        }
    }
    zipWrapper_.Close();
    return ERR_OK;
}

int32_t APPQFPackager::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS
