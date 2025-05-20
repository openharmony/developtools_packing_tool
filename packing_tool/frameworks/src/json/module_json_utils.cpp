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

#include "module_json_utils.h"

#include <algorithm>
#include <filesystem>
#include <iterator>

#include "hap_verify_utils.h"
#include "log.h"
#include "module_json.h"
#include "utils.h"
#include "zip_utils.h"
#include "constants.h"

namespace fs = std::filesystem;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string MODULE_JSON = "module.json";
const std::string CONFIG_JSON = "config.json";
const std::string HAP_SUFFIX = ".hap";
const std::string HSP_SUFFIX = ".hsp";
const int32_t SHARED_APP_HSP_LIMIT = 1;
const std::string TYPE_SHARED = "shared";
const std::string INCLUDE = "include";
const std::string EXCLUDE = "exclude";
const std::string ATOMIC_SERVICE = "atomicService";
static int32_t g_entryModuleSizeLimit = 2;
static int32_t g_notEntryModuleSizeLimit = 2;
static int32_t g_sumModuleSizeLimit = 10;
}

// java : parseStageHapVerifyInfo
bool ModuleJsonUtils::GetStageHapVerifyInfo(const std::string& hapFilePath, HapVerifyInfo& hapVerifyInfo)
{
    std::string fileContent;
    std::map<std::string, std::string> resourceMap;
    int64_t fileLength = Utils::GetFileLength(hapFilePath);
    if (fileLength < 0) {
        LOGE("Get hap file length failed! hapFile=%s", hapFilePath.c_str());
        return false;
    }
    if (!ZipUtils::GetFileContentFromZip(hapFilePath, MODULE_JSON, fileContent)) {
        LOGE("Get module.json content from hap file failed! hapFile=%s", hapFilePath.c_str());
        return false;
    }
    if (!ZipUtils::GetResourceMapFromZip(hapFilePath, resourceMap)) {
        LOGE("Get resouce map from hap file failed! hapFile=%s", hapFilePath.c_str());
        return false;
    }
    std::string fileType = "";
    fs::path fsHapPath(hapFilePath);
    std::string fileName = fsHapPath.filename().string();
    std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
    if (Utils::EndsWith(fileName, HAP_SUFFIX)) {
        fileType = HAP_SUFFIX;
    } else if (Utils::EndsWith(fileName, HSP_SUFFIX)) {
        fileType = HSP_SUFFIX;
    }
    hapVerifyInfo.SetResourceMap(resourceMap);
    hapVerifyInfo.SetProfileStr(fileContent);
    hapVerifyInfo.SetStageModule(true);
    hapVerifyInfo.SetFileLength(fileLength);
    hapVerifyInfo.SetFileType(fileType);
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromString(fileContent)) {
        LOGE("Parse json string failed!");
        return false;
    }
    if (!moduleJson.GetStageHapVerifyInfo(hapVerifyInfo)) {
        LOGE("Get stage hap verify info failed!");
        return false;
    }
    return true;
}

bool ModuleJsonUtils::GetFaHapVerifyInfo(const std::string& hapFilePath, HapVerifyInfo& hapVerifyInfo)
{
    std::string fileContent;
    int64_t fileLength = Utils::GetFileLength(hapFilePath);
    if (fileLength < 0) {
        LOGE("Get hap file length failed! hapFile=%s", hapFilePath.c_str());
        return false;
    }
    if (!ZipUtils::GetFileContentFromZip(hapFilePath, CONFIG_JSON, fileContent)) {
        LOGE("Get module.json content from hap file failed! hapFile=%s", hapFilePath.c_str());
        return false;
    }
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromString(fileContent)) {
        LOGE("Parse json string failed!");
        return false;
    }
    if (!moduleJson.GetFaHapVerifyInfo(hapVerifyInfo)) {
        LOGE("Get FA hap verify info failed!");
        return false;
    }
    hapVerifyInfo.SetProfileStr(fileContent);
    hapVerifyInfo.SetStageModule(false);
    hapVerifyInfo.SetFileLength(fileLength);
    return true;
}

// java : Compressor::checkSharedAppIsValid / HapVerify::checkSharedApppIsValid
bool ModuleJsonUtils::CheckSharedAppIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos, bool& isOverlay)
{
    if (hapVerifyInfos.empty()) {
        LOGE("hapVerifyInfos is empty");
        return false;
    }
    if (hapVerifyInfos.size() > SHARED_APP_HSP_LIMIT) {
        LOGE("hapVerifyInfos size is over than %d", SHARED_APP_HSP_LIMIT);
        return false;
    }
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (!hapVerifyInfo.GetTargetBundleName().empty()) {
            isOverlay = true;
            return true;
        }
    }
    return HapVerifyUtils::CheckSharedAppIsValid(hapVerifyInfos);
}

bool ModuleJsonUtils::GetHapVerifyInfosfromFileList(const std::list<std::string>& fileList,
    std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapPath : fileList) {
        if (hapPath.empty()) {
            LOGE("Hap file path is empty!");
            return false;
        }
        fs::path fsHapPath(hapPath);
        std::string fileName = fsHapPath.filename().string();
        if (fileName.empty()) {
            LOGE("Hap file name is empty!");
            return false;
        }
        std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
        if (!Utils::EndsWith(fileName, HAP_SUFFIX) && !Utils::EndsWith(fileName, HSP_SUFFIX)) {
            LOGE("Hap file is not a hap or hsp file!");
            return false;
        }
        HapVerifyInfo hapVerifyInfo;
        if (IsModuleHap(hapPath)) {
            if (!GetStageHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGE("GetStageHapVerifyInfo failed!");
                return false;
            }
        } else {
            if (!GetFaHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGE("GetFaHapVerifyInfo failed!");
                return false;
            }
        }
        hapVerifyInfos.push_back(hapVerifyInfo);
    }
    return true;
}

bool ModuleJsonUtils::GetHapVerifyInfosMapfromFileList(const std::list<std::string>& fileList, std::map<std::string,
    std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap)
{
    for (auto& hapPath : fileList) {
        if (hapPath.empty()) {
            LOGE("Hap file path is empty!");
            return false;
        }
        fs::path fsHapPath(hapPath);
        std::string fileName = fsHapPath.filename().string();
        if (fileName.empty()) {
            LOGE("Hap file name is empty!");
            return false;
        }
        std::transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
        if (!Utils::EndsWith(fileName, HAP_SUFFIX) && !Utils::EndsWith(fileName, HSP_SUFFIX)) {
            LOGE("Hap file is not a hap or hsp file!");
            return false;
        }
        auto hapVerifyInfo = std::make_shared<HapVerifyInfo>();
        if (IsModuleHap(hapPath)) {
            if (!GetStageHapVerifyInfo(hapPath, *hapVerifyInfo)) {
                LOGE("GetStageHapVerifyInfo failed");
                return false;
            }
        } else {
            if (!GetFaHapVerifyInfo(hapPath, *hapVerifyInfo)) {
                LOGE("GetFaHapVerifyInfo failed");
                return false;
            }
        }
        hapVerifyInfoMap.emplace(fileName, hapVerifyInfo);
    }
    return true;
}

bool ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid(std::map<std::string, std::string> parameterMap,
    std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap)
{
    std::string packMode;
    std::string outPath;
    if (parameterMap.find(Constants::PARAM_MODE) == parameterMap.end() ||
        parameterMap.find(Constants::PARAM_OUT_PATH) == parameterMap.end()) {
        LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: No mode parameters or no output path!");
        return false;
    }
    packMode = parameterMap.at(Constants::PARAM_MODE);
    if (packMode != Constants::MODE_APP
        && packMode != Constants::MODE_FAST_APP
        && packMode != Constants::MODE_MULTIAPP) {
        return true;
    }
    outPath = parameterMap.at(Constants::PARAM_OUT_PATH);
    unzFile zipApp = unzOpen(outPath.c_str());
    if (!zipApp) {
        LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: unzOpen outPath failed!");
        return false;
    }
    std::list<HapVerifyInfo> hapVerifyInfos;
    if (unzGoToFirstFile(zipApp) != UNZ_OK) {
        unzClose(zipApp);
        LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: unzGoToFirstFile outPath failed!");
        return false;
    }
    do {
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(zipApp, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
            unzClose(zipApp);
            LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: failed to get file info (phase 1)");
            return false;
        }
        std::vector<char> fileNameBuffer(fileInfo.size_filename + 1);
        if (unzGetCurrentFileInfo(zipApp, &fileInfo, fileNameBuffer.data(), fileNameBuffer.size(),
            nullptr, 0, nullptr, 0) != UNZ_OK) {
            unzClose(zipApp);
            LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: failed to get file info (phase 2)");
            return false;
        }
        std::string fileName(fileNameBuffer.data());
        auto it = hapVerifyInfoMap.find(fileName);
        if (it == hapVerifyInfoMap.end() || it->second == nullptr) {
            continue;
        }
        it->second->SetFileLength(fileInfo.compressed_size);
        hapVerifyInfos.push_back(*(it->second));
    } while (unzGoToNextFile(zipApp) == UNZ_OK);
    unzClose(zipApp);
    if (hapVerifyInfos.empty()) {
        LOGI("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: hapVerifyInfos is empty");
        return true;
    }
    std::string bundleType = hapVerifyInfos.front().GetBundleType();
    if (bundleType != Constants::ATOMIC_SERVICE) {
        return true;
    }
    bool isStage = hapVerifyInfos.front().IsStageModule();
    if (!isStage) {
        return true;
    }
    return HapVerifyUtils::CheckFileSizeIsValid(hapVerifyInfos);
}

// java : checkHapIsValid
bool ModuleJsonUtils::CheckHapsIsValid(const std::list<std::string>& fileList, const bool& isSharedApp)
{
    std::list<HapVerifyInfo> hapVerifyInfos;
    if (!GetHapVerifyInfosfromFileList(fileList, hapVerifyInfos)) {
        LOGE("GetHapVerifyInfosfromFileList failed!");
        return false;
    }
    if (isSharedApp) {
        bool isOverlay = false;
        if (!CheckSharedAppIsValid(hapVerifyInfos, isOverlay)) {
            LOGE("CheckSharedAppIsValid failed!");
            return false;
        }
        if (!isOverlay) {
            return true;
        }
    } else {
        for (auto& hapVerifyInfo : hapVerifyInfos) {
            if (hapVerifyInfo.GetBundleType().compare(TYPE_SHARED) == 0) {
                LOGE("bundle type cannot be %s when app is not shared", TYPE_SHARED.c_str());
                return false;
            }
        }
    }
    setAtomicServiceFileSizeLimit(hapVerifyInfos);
    if (!HapVerifyUtils::CheckHapIsValid(hapVerifyInfos)) {
        LOGE("CheckHapIsValid failed!");
        return false;
    }
    return true;
}

bool ModuleJsonUtils::IsModuleHap(const std::string hapFilePath)
{
    return ZipUtils::IsFileExistsInZip(hapFilePath, MODULE_JSON);
}

void ModuleJsonUtils::setAtomicServiceFileSizeLimit(std::list<HapVerifyInfo>& hapVerifyInfos)
{
    for (auto& hapVerifyInfo : hapVerifyInfos) {
        if (hapVerifyInfo.GetBundleType().compare(ATOMIC_SERVICE) == 0) {
            hapVerifyInfo.SetEntrySizeLimit(g_entryModuleSizeLimit);
            hapVerifyInfo.SetNotEntrySizeLimit(g_notEntryModuleSizeLimit);
            hapVerifyInfo.SetSumSizeLimit(g_sumModuleSizeLimit);
        }
    }
}
} // namespace AppPackingTool
} // namespace OHOS
