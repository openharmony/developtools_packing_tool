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

#include "res_packager.h"

#include <filesystem>
#include <string>

#include "constants.h"
#include "log.h"

namespace OHOS {
namespace AppPackingTool {
ResPackager::ResPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t ResPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t ResPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    if (!IsVerifyValidInResMode()) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t ResPackager::Process()
{
    if (!CompressPackResMode()) {
        if (fs::exists(outPath_)) {
            fs::remove_all(outPath_);
        }
        LOGE("ResPackager::Process failed.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t ResPackager::PostProcess()
{
    return ERR_OK;
}

bool ResPackager::IsVerifyValidInResMode()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    // check pack-info-path
    if (it != parameterMap_.end()) {
        packInfoPath_ = it->second;
        if (!fs::is_regular_file(packInfoPath_)) {
            LOGE("ResPackager::IsVerifyValidInResMode --pack-info-path is not a file.");
            return false;
        }
        if (fs::path(packInfoPath_).filename().string() != Constants::PACK_INFO) {
            LOGE("ResPackager::IsVerifyValidInResMode --pack-info-path must be pack.info file.");
            return false;
        }
    }
    // check entrycard-path
    it = parameterMap_.find(Constants::PARAM_ENTRYCARD_PATH);
    if (it != parameterMap_.end()) {
        entryCardPath_ = it->second;
        if (!fs::is_directory(entryCardPath_)) {
            LOGE("ResPackager::IsVerifyValidInResMode --entrycard-path is not a directory.");
            return false;
        }
        if (fs::path(entryCardPath_).filename().string() != Constants::ENTRYCARD_NAME) {
            LOGE("ResPackager::IsVerifyValidInResMode the level-1 directory name must be EntryCard, current is %s",
                entryCardPath_.c_str());
            return false;
        }
        if (!CompatibleProcess(entryCardPath_, formattedEntryCardPathList_, Constants::PNG_SUFFIX)) {
            LOGE("ResPackager::IsVerifyValidInResMode --entrycard-path is invalid.");
            return false;
        }
    }
    // check out-path
    it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it != parameterMap_.end()) {
        outPath_ = it->second;
    }
    std::string forceRewrite = "";
    it = parameterMap_.find(Constants::PARAM_FORCE);
    if (it != parameterMap_.end()) {
        forceRewrite = it->second;
    }
    return IsOutPathValid(outPath_, forceRewrite, Constants::RES_SUFFIX);
}

bool ResPackager::CompressPackResMode()
{
    zipWrapper_.Open(outPath_);
    if (!zipWrapper_.IsOpen()) {
        LOGE("ResPackager::Process: zipWrapper Open failed!");
        return false;
    }
    if (zipWrapper_.AddFileOrDirectoryToZip(entryCardPath_, Constants::ENTRYCARD_NAME) !=
        ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("ResPackager::Process: zipWrapper AddFileOrDirectoryToZip failed!");
        return false;
    }
    zipWrapper_.Close();

    return true;
}
} // namespace AppPackingTool
} // namespace OHOS