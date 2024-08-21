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

#include "hqf_packager.h"

#include "constants.h"
#include "contrib/minizip/zip.h"
#include "json/json_utils.h"
#include "json/module_json.h"
#include "log.h"
#include "nlohmann/json.hpp"
#include "packager.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {}

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
        LOGE("Output file is empty.");
        return ERR_INVALID_VALUE;
    } else if (!Utils::EndsWith(it->second, Constants::HQF_SUFFIX)) {
        LOGE("Input out file must end with .hqf.");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_FORCE);
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    fs::path outPath(parameterMap_.at(Constants::PARAM_OUT_PATH));
    if (it->second != "true") {
        if (fs::exists(outPath) && fs::is_regular_file(outPath)) {
            LOGE("%s already exist and can not overrite", outPath.c_str());
            return ERR_INVALID_VALUE;
        }
    }

    if (!fs::exists(outPath.parent_path())) {
        fs::create_directories(outPath.parent_path());
    }
    
    it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it == parameterMap_.end()) {
        LOGE("Must input patch.json file when pack hqf file.");
        return ERR_INVALID_VALUE;
    } else {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_JSON_PATH), true, Constants::JSON_SUFFIX)) {
            LOGE("Input patch.json is invalid when pack hqf file");
            return ERR_INVALID_VALUE;
        }
    }
    it = parameterMap_.find(Constants::PARAM_LIB_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_LIB_PATH), false)) {
            LOGE("Input lib path is invalid when pack hqf file");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_RESOURCES_PATH), false)) {
            LOGE("Input resources path is invalid when pack hqf file");
            return ERR_INVALID_VALUE;
        }
    }

    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end()) {
        if (!IsPathValid(parameterMap_.at(Constants::PARAM_ETS_PATH), false)) {
            LOGE("Must input valid ets path when pack hqf file.");
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
        if (zipWrapper_.WriteStringToZip(moduleJson.ToString(), Constants::PATCH_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("ZipWrapper WriteStringToZip failed!");
            return ERR_INVALID_VALUE;
        }
    } else {
        LOGE("patch.json is invalid!");
        return ERR_INVALID_VALUE;
    }
    it = parameterMap_.find(Constants::PARAM_LIB_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::LIB_PATH) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("zipWrapper AddFileOrDirectoryToZip failed!");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_PATH) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("zipWrapper AddFileOrDirectoryToZip failed!");
        return ERR_INVALID_VALUE;
    }

    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::ETS_PATH) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("zipWrapper AddFileOrDirectoryToZip failed!");
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