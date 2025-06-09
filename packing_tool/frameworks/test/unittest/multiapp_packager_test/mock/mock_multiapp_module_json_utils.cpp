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

#include "module_json_utils.h"

#include <algorithm>
#include <filesystem>
#include <iterator>

#include "hap_verify_utils.h"
#include "log.h"
#include "mock_multiapp_module_json_utils.h"
#include "module_json.h"
#include "utils.h"
#include "zip_utils.h"
#include "constants.h"

namespace fs = std::filesystem;

namespace OHOS {

bool MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfo_ = false;
bool MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfo_ = false;
bool MockMultiAppModuleJsonUtils::mockCheckHapsIsValid_ = false;
bool MockMultiAppModuleJsonUtils::mockIsModuleHap_ = false;
bool MockMultiAppModuleJsonUtils::mockCheckHapsIsValidResult_ = false;
bool MockMultiAppModuleJsonUtils::mockIsModuleHapResult_ = false;
bool MockMultiAppModuleJsonUtils::mockCheckAppAtomicServiceCompressedSizeValid_ = false;
bool MockMultiAppModuleJsonUtils::mockCheckAppAtomicServiceCompressedSizeValidResult_ = false;
bool MockMultiAppModuleJsonUtils::mockGetHapVerifyInfosMapfromFileList_ = false;
bool MockMultiAppModuleJsonUtils::mockGetHapVerifyInfosMapfromFileListResult_ = false;
ResultSeries MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeries_;
ResultSeries::const_iterator
    MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeriesIter_ =
    mockGetStageHapVerifyInfoResultSeries_.cbegin();
ResultSeries MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeries_;
ResultSeries::const_iterator
    MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeriesIter_ = mockGetFaHapVerifyInfoResultSeries_.cbegin();

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
    if (MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfo_) {
        const bool result = MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeriesIter_->first;
        hapVerifyInfo = MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeriesIter_->second;

        const auto next = MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeriesIter_ + 1;
        if (next != MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeries_.cend()) {
            MockMultiAppModuleJsonUtils::mockGetStageHapVerifyInfoResultSeriesIter_ = next;
        }

        return result;
    }

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
    hapVerifyInfo.SetResourceMap(resourceMap);
    hapVerifyInfo.SetProfileStr(fileContent);
    hapVerifyInfo.SetStageModule(true);
    hapVerifyInfo.SetFileLength(fileLength);
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
    if (MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfo_) {
        const bool result = MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeriesIter_->first;
        hapVerifyInfo = MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeriesIter_->second;

        const auto next = MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeriesIter_ + 1;
        if (next != MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeries_.cend()) {
            MockMultiAppModuleJsonUtils::mockGetFaHapVerifyInfoResultSeriesIter_ = next;
        }

        return result;
    }

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

bool ModuleJsonUtils::GetHapVerifyInfosMapfromFileList(const std::list<std::string>& fileList,
    std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap)
{
    if (MockMultiAppModuleJsonUtils::mockGetHapVerifyInfosMapfromFileList_) {
        return MockMultiAppModuleJsonUtils::mockGetHapVerifyInfosMapfromFileListResult_;
    }

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

bool ValidateParameters(const std::map<std::string, std::string>& parameterMap)
{
    if (parameterMap.find(Constants::PARAM_MODE) == parameterMap.end() ||
        parameterMap.find(Constants::PARAM_OUT_PATH) == parameterMap.end()) {
        LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: No mode parameters or no output path!");
        return false;
    }
    return true;
}

bool ShouldSkipValidation(const std::string& packMode)
{
    return packMode != Constants::MODE_APP &&
           packMode != Constants::MODE_FAST_APP &&
           packMode != Constants::MODE_MULTIAPP;
}

bool ProcessZipFile(const std::string& outPath,
    std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap,
    std::list<HapVerifyInfo>& hapVerifyInfos)
{
    unzFile zipApp = unzOpen(outPath.c_str());
    if (!zipApp) {
        LOGE("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: unzOpen outPath failed!");
        return false;
    }

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
    return true;
}

bool ShouldSkipHapValidation(const std::list<HapVerifyInfo>& hapVerifyInfos)
{
    if (hapVerifyInfos.empty()) {
        LOGI("ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid: hapVerifyInfos is empty");
        return true;
    }
    std::string bundleType = hapVerifyInfos.front().GetBundleType();
    if (bundleType != Constants::ATOMIC_SERVICE) {
        return true;
    }
    return !hapVerifyInfos.front().IsStageModule();
}

bool ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid(
    std::map<std::string, std::string> parameterMap,
    std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap)
{
    if (MockMultiAppModuleJsonUtils::mockCheckAppAtomicServiceCompressedSizeValid_) {
        return MockMultiAppModuleJsonUtils::mockCheckAppAtomicServiceCompressedSizeValidResult_;
    }

    if (!ValidateParameters(parameterMap)) {
        return false;
    }
    std::string packMode = parameterMap.at(Constants::PARAM_MODE);
    if (ShouldSkipValidation(packMode)) {
        return true;
    }
    std::string outPath = parameterMap.at(Constants::PARAM_OUT_PATH);
    std::list<HapVerifyInfo> hapVerifyInfos;
    if (!ProcessZipFile(outPath, hapVerifyInfoMap, hapVerifyInfos)) {
        return false;
    }
    if (ShouldSkipHapValidation(hapVerifyInfos)) {
        return true;
    }
    return HapVerifyUtils::CheckFileSizeIsValid(hapVerifyInfos);
}

// java : checkHapIsValid
bool ModuleJsonUtils::CheckHapsIsValid(const std::list<std::string>& fileList, const bool& isSharedApp)
{
    if (MockMultiAppModuleJsonUtils::mockCheckHapsIsValid_) {
        return MockMultiAppModuleJsonUtils::mockCheckHapsIsValidResult_;
    }

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
    if (MockMultiAppModuleJsonUtils::mockIsModuleHap_) {
        return MockMultiAppModuleJsonUtils::mockIsModuleHapResult_;
    }

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
