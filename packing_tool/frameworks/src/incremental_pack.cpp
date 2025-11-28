/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "incremental_pack.h"
#include "log.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string TMEP_EXIST_SRC_DIR = "tempExistSrcDir";
}

std::string IncrementalPack::destExistSrcFilePath_ = "";
std::string IncrementalPack::destTempExistSrcDir_ = "";

bool IncrementalPack::IsIncrementalMode(const std::map<std::string, std::string> &parameterMap)
{
    auto it = parameterMap.find(Constants::PARAM_MODE);
    if (it == parameterMap.end() || it->second.empty()) {
        LOGI("Parse mode empty.");
        return false;
    }
    std::string mode = it->second;

    it = parameterMap.find(Constants::PARAM_EXIST_SRC_PATH);
    if (it == parameterMap.end() || it->second.empty()) {
        LOGI("Parse exist-src-path empty.");
        return false;
    }
    std::string existSrcPathStr = it->second;

    it = parameterMap.find(Constants::PARAM_LIB_PATH_RETAIN);
    if (it == parameterMap.end() || it->second.empty()) {
        LOGI("Parse lib-path-retain empty.");
        return false;
    }
    std::string libPathRetain = it->second;

    return (mode == Constants::MODE_HAP || mode == Constants::MODE_HSP) &&
            (Utils::IsPathValid(existSrcPathStr, true, Constants::HAP_SUFFIX) ||
            Utils::IsPathValid(existSrcPathStr, true, Constants::HSP_SUFFIX)) &&
            libPathRetain == "true";
}

bool IncrementalPack::IncrementalPackProcess(const std::string &paramPath, ZipWrapper &zipWrapper)
{
    unzFile srcZip = unzOpen(IncrementalPack::destExistSrcFilePath_.c_str());
    if (!srcZip) {
        LOGE("Failed to open exist-src-path hap [%s]", IncrementalPack::destExistSrcFilePath_.c_str());
        return false;
    }

    zipFile destZip = zipWrapper.GetZipFile();
    unzGoToFirstFile(srcZip);
    do {
        char fileNameInZip[512] = {0};
        unzGetCurrentFileInfo64(srcZip, nullptr, fileNameInZip, sizeof(fileNameInZip), nullptr, 0, nullptr, 0);
        if (std::string(fileNameInZip).rfind("libs/", 0) == 0) {
            int ret = zipWrapper.AddRawEntryToZip(destZip, srcZip, fileNameInZip);
            if (ret != ZIP_ERR_SUCCESS) {
                LOGE("AddRawEntryToZip failed for [%s]", fileNameInZip);
                unzClose(srcZip);
                return false;
            }
        }
    } while (unzGoToNextFile(srcZip) == UNZ_OK);

    unzClose(srcZip);
    return true;
}

bool IncrementalPack::CopyExistSrcFile(const std::map<std::string, std::string> &parameterMap)
{
    if (!IsIncrementalMode(parameterMap)) {
        return true;
    }
    auto existSrcIt = parameterMap.find(Constants::PARAM_EXIST_SRC_PATH);
    if (existSrcIt == parameterMap.end() || existSrcIt->second.empty()) {
        LOGI("Parse exist-src-path empty.");
        return false;
    }
    std::string realExistSrcPath;
    if (!Utils::GetRealPath(existSrcIt->second, realExistSrcPath)) {
        LOGE("Get real existSrcPath failed! existSrcPath=%s", existSrcIt->second.c_str());
        return false;
    }
    destExistSrcFilePath_ = realExistSrcPath;
    auto outPathIt = parameterMap.find(Constants::PARAM_OUT_PATH);
    if (outPathIt == parameterMap.end() || outPathIt->second.empty()) {
        LOGI("Parse out-path empty.");
        return false;
    }
    std::string realOutPath;
    if (!Utils::GetRealPath(outPathIt->second, realOutPath)) {
        LOGE("Get real outPath failed! outPath=%s", outPathIt->second.c_str());
        return false;
    }
    if (realExistSrcPath == realOutPath) {
        if (!Utils::CopyFileToTempDir(realExistSrcPath,
                                      Constants::TEMP_EXIST_SRC_DIR,
                                      destExistSrcFilePath_,
                                      destTempExistSrcDir_)) {
            LOGI("CopyFileToTempDir failed.");
            return false;
        }
    }
    return true;
}

bool IncrementalPack::DeleteExistSrcTempDir()
{
    return Utils::ForceRemoveDirectory(destTempExistSrcDir_, true);
}
} // namespace AppPackingTool
} // namespace OHOS