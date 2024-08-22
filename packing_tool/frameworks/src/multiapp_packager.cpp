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

#include "multiapp_packager.h"

#include <fstream>

#include "constants.h"
#include "json/module_json.h"
#include "json/module_json_utils.h"
#include "json/pack_info_utils.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const int32_t APP_SUFFIX_LENGTH = 4;
}

MultiAppPackager::MultiAppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t MultiAppPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t MultiAppPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    bool ret = IsVerifyValidInMultiAppMode();
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t MultiAppPackager::Process()
{
    bool ret = CompressAppModeForMultiProject();
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t MultiAppPackager::PostProcess()
{
    return ERR_OK;
}

bool MultiAppPackager::IsVerifyValidInMultiAppMode()
{
    std::string hapListStr;
    std::string appListStr;
    if (parameterMap_.find(Constants::PARAM_APP_LIST) != parameterMap_.end()) {
        appListStr = parameterMap_.at(Constants::PARAM_APP_LIST);
    }
    if (parameterMap_.find(Constants::PARAM_HAP_LIST) != parameterMap_.end()) {
        hapListStr = parameterMap_.at(Constants::PARAM_HAP_LIST);
    }

    if (hapListStr.empty() && appListStr.empty()) {
        LOGE("isVerifyValidInMultiAppMode input app-list and hap-list are null.");
        return false;
    }
    if (!appListStr.empty()) {
        if (!CompatibleProcess(appListStr, formattedAppList_, Constants::APP_SUFFIX)) {
            LOGE("isVerifyValidInMultiAppMode app-list is invalid.");
            return false;
        }
    }
    if (!hapListStr.empty()) {
        if (!CompatibleProcess(hapListStr, formattedHapAndHspList_, Constants::HAP_SUFFIX)) {
            LOGE("isVerifyValidInMultiAppMode hap-list is invalid.");
            return false;
        }
    }
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_HSP_LIST);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (!CompatibleProcess(it->second, formattedHapAndHspList_, Constants::HSP_SUFFIX)) {
            LOGE("isVerifyValidInMultiAppMode hsp-list is invalid.");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        std::string outPath = it->second;
        it = parameterMap_.find(Constants::PARAM_FORCE);
        if (it != parameterMap_.end() && !it->second.empty()) {
            if (Utils::IsFileExists(outPath) && it->second == "false") {
                LOGE("isVerifyValidInMultiAppMode out file already existed.");
                return false;
            }
            if (outPath.find('.') == std::string::npos || outPath.substr(outPath.size() - APP_SUFFIX_LENGTH) !=
                Constants::APP_SUFFIX) {
                LOGE("isVerifyValidInMultiAppMode out-path must end with .app.");
                return false;
            }
        }
    }
    return true;
}

bool MultiAppPackager::CopyHapAndHspFromApp(const std::string &appPath, std::list<std::string> &selectedHapsInApp,
                                            std::list<std::string> &selectedHaps, const std::string &tempDir)
{
    fs::path tempPath;
    if (fs::exists(fs::path(tempDir).parent_path().parent_path()) &&
        fs::path(tempDir).parent_path().parent_path() != fs::path("/")) {
        tempPath = fs::path(tempDir).parent_path().parent_path() / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
    } else {
        tempPath = fs::path(tempDir).parent_path() / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
    }
    ZipUtils::Unzip(appPath, tempPath);
    fs::path filePath;
    for (const auto &entry : fs::directory_iterator(tempPath)) {
        if (!Utils::EndsWith(entry.path().filename(), Constants::HAP_SUFFIX) &&
            !Utils::EndsWith(entry.path().filename(), Constants::HSP_SUFFIX)) {
            continue;
        }
        if (std::find(selectedHaps.begin(), selectedHaps.end(), entry.path().filename()) != selectedHaps.end()) {
            LOGE("CopyHapAndHspFromApp file duplicated, file is %s ", entry.path().filename().c_str());
            return false;
        } else {
            filePath = fs::path(tempDir) / entry.path().filename();
            selectedHaps.push_back(filePath.filename());
            selectedHapsInApp.push_back(filePath.filename());
        }
        std::ofstream outputStream(filePath, std::ofstream::binary);
        std::ifstream inputStream(entry, std::ofstream::binary);
        std::vector<char> buf(Constants::BUFFER_SIZE);
        while (inputStream.read(buf.data(), Constants::BUFFER_SIZE) || inputStream.gcount() != 0) {
            outputStream.write(buf.data(), inputStream.gcount());
        }
        outputStream.close();
        inputStream.close();
    }
    if (fs::exists(tempPath)) {
        fs::remove_all(tempPath);
    }
    return true;
}

std::string MultiAppPackager::GetJsonInZips(const std::string &filePath, const std::string &jsonName)
{
    std::string jsonStr;
    if (ZipUtils::IsFileExistsInZip(filePath, jsonName)) {
        if (ZipUtils::GetFileContentFromZip(filePath, jsonName, jsonStr)) {
            jsonStr.erase(std::remove_if(jsonStr.begin(), jsonStr.end(),
                [](char c) { return c == '\r' || c == '\n' || c == '\t'; }),
                jsonStr.end());
        }
    }
    return jsonStr;
}

std::string MultiAppPackager::ReadModuleNameFromHap(const std::string &hapPath)
{
    std::string moduleName;
    fs::path hapFile(hapPath);
    ModuleJson module;
    if (IsModuleHap(hapFile)) {
        std::string jsonString = GetJsonInZips(hapFile, Constants::MODULE_JSON);
        module.ParseFromString(jsonString);
        module.GetStageModuleName(moduleName);
    } else {
        std::string jsonString = GetJsonInZips(hapFile, Constants::CONFIG_JSON);
        module.ParseFromString(jsonString);
        module.GetFaModuleName(moduleName);
    }
    return moduleName;
}

std::string MultiAppPackager::SelectHapInApp(const std::string &appPath, std::list<std::string> &selectedHaps,
                                             const std::string &tempDir, std::string &finalAppPackInfo)
{
    std::list<std::string> selectedHapsInApp;
    CopyHapAndHspFromApp(appPath, selectedHapsInApp, selectedHaps, tempDir);
    std::string packInfoStr = GetJsonInZips(appPath, Constants::PACK_INFO);
    if (packInfoStr.empty()) {
        LOGE("MultiAppPackager:SelectHapInApp failed, app has no pack.info.");
    }
    if (finalAppPackInfo.empty()) {
        finalAppPackInfo = packInfoStr;
        return finalAppPackInfo;
    }

    std::map<std::string, std::string> packagePair;
    for (const auto &hapName : selectedHapsInApp) {
        packagePair[hapName] = ReadModuleNameFromHap(tempDir + "/" + hapName);
    }
    std::string packInfoJsonStr;
    if (!PackInfoUtils::MergeTwoPackInfosByPackagePair(finalAppPackInfo, packInfoStr, packagePair, packInfoJsonStr)) {
        LOGE("PackInfoUtils::MergeTwoPackInfosByPackagePair failed.");
    }
    return packInfoJsonStr;
}

std::string MultiAppPackager::DisposeApp(std::list<std::string> &selectedHaps, const std::string &tempDir)
{
    std::string finalAppPackInfo;
    if (formattedAppList_.empty()) {
        return finalAppPackInfo;
    }
    for (const auto &appPath : formattedAppList_) {
        finalAppPackInfo = SelectHapInApp(appPath, selectedHaps, tempDir, finalAppPackInfo);
    }
    return finalAppPackInfo;
}

std::string MultiAppPackager::DisposeHapAndHsp(std::list<std::string> &selectedHaps,
                                               const std::string &tempDir, std::string finalPackInfoStr)
{
    if (formattedHapAndHspList_.empty()) {
        LOGE("MultiAppPackager::DisposeHapAndHsp hapList is empty.");
        return finalPackInfoStr;
    }
    for (const auto &hapPath : formattedHapAndHspList_) {
        fs::path hapPathFile(hapPath);
        if (std::find(selectedHaps.begin(), selectedHaps.end(), hapPathFile.filename()) != selectedHaps.end()) {
            LOGE("DisposeHapAndHsp file duplicated, file is %s", hapPathFile.filename().c_str());
        }
        fs::path hapFile(hapPath);
        selectedHaps.push_back(hapFile.filename());
        std::string dstDirString = tempDir + "/" + static_cast<std::string>(hapFile.filename());
        Utils::CopyFile(hapPath, dstDirString);
        std::string packInfo = GetJsonInZips(hapFile, Constants::PACK_INFO);
        if (packInfo.empty()) {
            LOGE("MultiAppPackager::DisposeHapAndHsp failed, hap has no pack.info.");
        }
        if (finalPackInfoStr.empty()) {
            finalPackInfoStr = packInfo;
        } else {
            std::string packInfoJsonStr;
            if (!PackInfoUtils::MergeTwoPackInfos(finalPackInfoStr, packInfo, packInfoJsonStr)) {
                LOGE("PackInfoUtils::MergeTwoPackInfos failed.");
            }
            finalPackInfoStr = packInfoJsonStr;
        }
    }
    return finalPackInfoStr;
}

void MultiAppPackager::WritePackInfo(const std::string &filePath, const std::string &packInfoStr)
{
    std::ofstream fwriter(filePath);
    if (!fwriter) {
        LOGE("MultiAppPackager:WritePackInfo failed. Unable to open file: %s", filePath.c_str());
        return;
    }
    fwriter << packInfoStr;
    if (fwriter.fail()) {
        LOGE("MultiAppPackager:WritePackInfo failed. Error writing to file: %s", filePath.c_str());
        return;
    }
    fwriter.close();
}

bool MultiAppPackager::PrepareFilesForCompression(std::list<std::string> &fileList, fs::path &tempHapDirPath,
    fs::path &tempSelectedHapDirPath, std::string &finalPackInfoStr, std::string &finalPackInfoPath)
{
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        LOGE("MultiAppPackager::CompressAppModeForMultiProject: zipWrapper Open failed!");
        return false;
    }
    if (fs::exists(fs::path(outPath).parent_path().parent_path()) &&
        fs::path(outPath).parent_path().parent_path() != fs::path("/")) {
        tempHapDirPath =  fs::path(outPath).parent_path().parent_path() / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
        tempSelectedHapDirPath = fs::path(outPath).parent_path().parent_path() /
            ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) + Utils::GenerateUUID());
    } else {
        tempHapDirPath =  fs::path(outPath).parent_path() / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
        tempSelectedHapDirPath = fs::path(outPath).parent_path() / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
    }
    if (!fs::exists(tempHapDirPath)) {
        fs::create_directories(tempHapDirPath);
    }
    if (!fs::exists(tempSelectedHapDirPath)) {
        fs::create_directories(tempSelectedHapDirPath);
    }
    std::list<std::string> selectedHaps;
    finalPackInfoStr = DisposeApp(selectedHaps, tempSelectedHapDirPath);
    finalPackInfoStr = DisposeHapAndHsp(selectedHaps, tempSelectedHapDirPath, finalPackInfoStr);
    finalPackInfoPath = tempSelectedHapDirPath.string() + "/" + Constants::PACK_INFO;
    WritePackInfo(finalPackInfoPath, finalPackInfoStr);
    for (const auto &selectedHapName : selectedHaps) {
        std::string hapPathItem = tempSelectedHapDirPath.string() + "/" + selectedHapName;
        fs::path hapFile(hapPathItem);
        std::string hapTempPath = tempHapDirPath.string() + "/" + hapFile.filename().string();
        fs::path hapUnzipTempPath = tempHapDirPath / ((Constants::COMPRESSOR_MULTIAPP_TEMP_DIR) +
            Utils::GenerateUUID());
        fileList.push_back(hapTempPath);
        CompressPackinfoIntoHap(hapPathItem, hapUnzipTempPath, hapTempPath, finalPackInfoPath);
    }
    return true;
}

bool MultiAppPackager::CompressAppModeForMultiProject()
{
    std::list<std::string> fileList;
    fs::path tempHapDirPath;
    fs::path tempSelectedHapDirPath;
    std::string finalPackInfoStr;
    std::string finalPackInfoPath;
    if(!PrepareFilesForCompression(fileList, tempHapDirPath, tempSelectedHapDirPath, finalPackInfoStr,
        finalPackInfoPath)) {
        return false;
    }
    if (!ModuleJsonUtils::CheckHapsIsValid(fileList, false)) {
        LOGE("here are somehaps with different version code or duplicated moduleName or packageName.");
        return false;
    }
    for (const auto &hapPath : fileList) {
        std::string zipPath = fs::path(hapPath).filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(hapPath, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            return false;
        }
    }
    if (zipWrapper_.AddFileOrDirectoryToZip(finalPackInfoPath, Constants::PACK_INFO) != ZipErrCode::ZIP_ERR_SUCCESS) {
        return false;
    }
    zipWrapper_.Close();
    if (fs::exists(tempHapDirPath)) {
        fs::remove_all(tempHapDirPath);
    }
    if (fs::exists(tempSelectedHapDirPath)) {
        fs::remove_all(tempSelectedHapDirPath);
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS