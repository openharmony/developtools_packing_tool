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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>

#include "constants.h"
#include "contrib/minizip/zip.h"
#include "nlohmann/json.hpp"
#include "packager.h"

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
    return ERR_OK;
}

int32_t HapPackager::Process()
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        std::cout << "HapPackager::Process: zipWrapper Open failed!" << std::endl;
        return ERR_INVALID_VALUE;
    }
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_JSON_PATH);
    if (it != parameterMap_.end()) {
        if (ParseJsonFile(moduleJson, it->second)) {
            if (zipWrapper_.WriteStringToZip(moduleJson.dump(), Constants::MODULE_JSON) !=
                ZipErrCode::ZIP_ERR_SUCCESS) {
                std::cout << "HapPackager::Process: zipWrapper WriteStringToZip failed!" << std::endl;
                return ERR_INVALID_VALUE;
            }
        }
    }
    it = parameterMap_.find(Constants::PARAM_LIB_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::LIB_PATH);
    }
    it = parameterMap_.find(Constants::PARAM_RESOURCES_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_PATH);
    }
    it = parameterMap_.find(Constants::PARAM_INDEX_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RESOURCES_INDEX);
    }
    it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::PACK_INFO);
    }
    it = parameterMap_.find(Constants::PARAM_ETS_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::ETS_PATH);
    }
    it = parameterMap_.find(Constants::PARAM_RPCID_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::RPCID_SC);
    }
    it = parameterMap_.find(Constants::PARAM_PKG_CONTEXT_PATH);
    if (it != parameterMap_.end()) {
        zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::PKG_CONTEXT_JSON);
    }
    zipWrapper_.Close();
    return ERR_OK;
}

int32_t HapPackager::PostProcess()
{
    return ERR_OK;
}
} // namespace AppPackingTool
} // namespace OHOS