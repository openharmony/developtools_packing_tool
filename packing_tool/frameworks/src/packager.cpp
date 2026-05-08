/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include "packager.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>

#include "json/json_utils.h"
#include "log.h"
#include "scan_statdulpicate.h"
#include "utils.h"
#include "zip_utils.h"
#include "error/packing_tool_err_msg.h"
using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {

int32_t Packager::atomicServiceEntrySizeLimit_ = 2048;
int32_t Packager::atomicServiceNonEntrySizeLimit_ = 2048;

namespace {
const std::string EN_US_UTF_8 = "en_US.UTF-8";
const std::string SCAN_RESULT = "scan_result";
}

Packager::Packager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : parameterMap_(parameterMap), resultReceiver_(resultReceiver)
{}

Packager::~Packager()
{}

int32_t Packager::MakePackage()
{
    int32_t ret = ERR_OK;
    ret = InitAllowedParam();
    if (ret != ERR_OK) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_PROCESS_FAILED.toStringWithArgs(
            "InitAllowedParam err").c_str());
        return ret;
    }

    ret = PreProcess();
    if (ret != ERR_OK) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_PROCESS_FAILED.toStringWithArgs(
            "PreProcess err").c_str());
        return ret;
    }
    ret = Process();
    if (ret != ERR_OK) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_PROCESS_FAILED.toStringWithArgs(
            "Process err").c_str());
        return ret;
    }

    ret = PostProcess();
    if (ret != ERR_OK) {
        LOGE("%s", PackingToolErrMsg::COMPRESS_PROCESS_FAILED.toStringWithArgs(
            "PostProcess err").c_str());
        return ret;
    }

    ScanSoFiles();
    return ret;
}

int32_t Packager::PreProcess()
{
    resultReceiver_.append("do PreProcess\n");
    return ERR_OK;
}

int32_t Packager::Process()
{
    resultReceiver_.append("do Process\n");
    return ERR_OK;
}

int32_t Packager::PostProcess()
{
    resultReceiver_.append("do PostProcess\n");
    return ERR_OK;
}

bool Packager::CheckForceFlag()
{
    auto it = parameterMap_.find(Constants::PARAM_FORCE);
    if (it != parameterMap_.end() && it->second != "false" && it->second != "true") {
        LOGE("%s", PackingToolErrMsg::COMMAND_VERIFY_FAILED.toStringWithArgs(
              "If the --force parameter is configured, the value must be either 'true' or 'false'.").c_str());
        return false;
    }
    return true;
}

bool Packager::CheckStatDuplicateFlag()
{
    auto it = parameterMap_.find(Constants::PARAM_STAT_DUPLICATE);
    if (it != parameterMap_.end() && it->second != "false" && it->second != "true") {
        LOGE("%s", PackingToolErrMsg::COMMAND_VERIFY_FAILED.toStringWithArgs(
            "Packager::commandVerify statDuplicate is invalid.").c_str());
        return false;
    }
    return true;
}

bool Packager::IsPathValid(const std::string &path, const bool &isFile, const std::string suffix)
{
    try {
        if (isFile) {
            if (!fs::is_regular_file(path)) {
                return false;
            }
            const std::locale englishLocale(EN_US_UTF_8);
            std::string name = fs::path(path).filename();
            std::transform(name.begin(), name.end(), name.begin(),
                           [&englishLocale](unsigned char c) { return std::tolower(c); });
            return Utils::EndsWith(name, suffix);
        }
        return fs::is_directory(path);
    } catch (...) {
        LOGW("Param parse error");
        return false;
    }
}

bool Packager::IsPathParamValid(const std::string &parameterMapKey, const bool &isFile, const std::string &suffix)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(parameterMapKey);
    if (it == parameterMap_.end()) {
        return true;
    }
    const std::string path = it->second;
    return (path.empty() || IsPathValid(path, isFile, suffix));
}

bool Packager::IsCompressLevelValid()
{
    auto it = parameterMap_.find(Constants::PARAM_COMPRESS_LEVEL);
    if (it != parameterMap_.end() && !it->second.empty()) {
        const std::string& levelStr = it->second;
        bool allDigits = std::all_of(levelStr.begin(), levelStr.end(),
                                     [](unsigned char c) { return std::isdigit(c); });
        if (!allDigits) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("Param parse compress-level [" + levelStr + "] invalid: contains non-digit characters.").c_str()).c_str());
            return false;
        }
        try {
            int level = std::stoi(levelStr);
            if (level < Constants::MIN_COMPRESS_LEVEL || level > Constants::MAX_COMPRESS_LEVEL) {
                LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                    ("Param parse compress-level [" + levelStr + "] invalid: must be between 1 and 9.").c_str()).c_str());
                return false;
            }
        } catch (const std::exception& e) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("Param parse compress-level [" + levelStr + "] invalid: conversion failed.").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool Packager::IsFileMatch(const std::string &path, const std::string &matchFileName)
{
    try {
        if (fs::is_regular_file(path)) {
            std::string name = fs::path(path).filename().string();
            return name == matchFileName;
        }
    } catch (const fs::filesystem_error& e) {
        LOGE("%s", PackingToolErrMsg::COMMAND_VERIFY_FAILED.toStringWithArgs(
            ("Packager::commandVerify fileMatch has error : " + std::string(e.code().message())).c_str()).c_str());
    }
    return false;
}

bool Packager::SplitDirList(const std::string &dirList, std::list<std::string> &fileList)
{
    std::list<std::string> pathList;
    RemoveDuplicatePath(dirList, pathList);
    for (const std::string &pathItem : pathList) {
        std::string formattedPathItem;
        if (!Utils::GetFormattedPath(pathItem, formattedPathItem)) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("GetFormattedPath failed for " + pathItem).c_str()).c_str());
            return false;
        };
        if (!IsPathValid(formattedPathItem, false)) {
            return false;
        }
        fileList.push_back(formattedPathItem);
    }
    return true;
}

void Packager::RemoveDuplicatePath(const std::string &path, std::list<std::string> &pathList)
{
    std::string pathItem;
    std::istringstream pathStream(path);
    while (std::getline(pathStream, pathItem, Constants::COMMA_SPLIT)) {
        pathList.push_back(pathItem);
    }
    std::unordered_set<std::string> uniqueTokens(pathList.begin(), pathList.end());
    pathList.assign(uniqueTokens.begin(), uniqueTokens.end());
}

bool Packager::CompatibleProcess(const std::string &inputPath, std::list<std::string> &fileList,
    const std::string &suffix)
{
    if (IsPathValid(inputPath, false)) {
        for (const auto& fileItem : fs::recursive_directory_iterator(inputPath)) {
            std::string name = fileItem.path().filename();
            std::locale englishLocale(EN_US_UTF_8);
            std::transform(name.begin(), name.end(), name.begin(),
                 [&englishLocale](unsigned char c) { return std::tolower(c); });
            if (Utils::EndsWith(name, suffix)) {
                fileList.push_back(fileItem.path().string());
            }
        }
        return true;
    } else {
        std::string formattedPathItem;
        std::list<std::string> pathList;
        RemoveDuplicatePath(inputPath, pathList);
        for (std::string pathItem : pathList) {
            if (!Utils::GetFormattedPath(pathItem, formattedPathItem)) {
                LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("GetFormattedPath failed for " + pathItem).c_str()).c_str());
                return false;
            };
            if (!IsPathValid(formattedPathItem, true, suffix)) {
                return false;
            }
            fileList.push_back(formattedPathItem);
        }
        return true;
    }
}

bool Packager::CompatibleProcess(const std::string &inputPath, std::list<std::string> &fileList,
    const std::string &suffix, const std::string &extraSuffix)
{
    if (IsPathValid(inputPath, false)) {
        for (const auto& fileItem : fs::recursive_directory_iterator(inputPath)) {
            std::string name = fileItem.path().filename();
            std::locale englishLocale(EN_US_UTF_8);
            std::transform(name.begin(), name.end(), name.begin(),
                 [&englishLocale](unsigned char c) { return std::tolower(c); });
            if (Utils::EndsWith(name, suffix) || Utils::EndsWith(name, extraSuffix)) {
                fileList.push_back(fileItem.path().string());
            }
        }
        return true;
    } else {
        std::string formattedPathItem;
        std::list<std::string> pathList;
        RemoveDuplicatePath(inputPath, pathList);
        for (std::string pathItem : pathList) {
            if (!Utils::GetFormattedPath(pathItem, formattedPathItem)) {
                LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("GetFormattedPath failed for " + pathItem).c_str()).c_str());
                return false;
            };
            if (!IsPathValid(formattedPathItem, true, suffix) &&
                !IsPathValid(formattedPathItem, true, extraSuffix)) {
                return false;
            }
            fileList.push_back(formattedPathItem);
        }
        return true;
    }
}

bool Packager::EnsureParentDirectoryExists(const std::filesystem::path& filePath)
{
    try {
        const auto parentPath = filePath.parent_path();
        if (!std::filesystem::exists(parentPath)) {
            std::error_code ec;
            if (!std::filesystem::create_directories(parentPath, ec)) {
                LOGE("%s", PackingToolErrMsg::MAKE_DIR_FAILED.toStringWithArgs(
                    ("Packager::Failed to create directory: [" + std::string(ec.message()) + "]").c_str()).c_str());
                return false;
            }
        }
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        LOGE("%s", PackingToolErrMsg::MAKE_DIR_FAILED.toStringWithArgs(
            ("Packager::Directory creation error: [" + std::string(e.what()) + "]").c_str()).c_str());
        return false;
    }
}

bool Packager::IsOutPathValid(const std::string &outPath, const std::string &forceRewrite, const std::string &suffix)
{
    const fs::path filePath(outPath);

    if ("false" == forceRewrite && fs::exists(filePath)) {
        LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
            "Packager::isOutPathValid out file already existed.").c_str());
        return false;
    }
	
    if (!EnsureParentDirectoryExists(filePath)) {
        return false;
    }

    if (suffix == Constants::HAP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HAP_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
                "Packager::isOutPathValid out-path must end with .hap.").c_str());
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::HAR_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HAR_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
                "Packager::isOutPathValid out-path must end with .har.").c_str());
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::APP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::APP_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
                "Packager::isOutPathValid out-path must end with .app.").c_str());
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::RES_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::RES_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
                "Packager::isOutPathValid out-path must end with .res.").c_str());
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::HSP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HSP_SUFFIX)) {
            LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
                "Packager::isOutPathValid out-path must end with .hsp.").c_str());
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::MODE_MULTIAPP) {
        return true;
    }
    return false;
}

bool Packager::SetGenerateBuildHash(std::string &jsonPath, bool &generateBuildHash, bool &buildHashFinish)
{
    if (!fs::exists(jsonPath)) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "Packager::setGenerateBuildHash failed for json file not exist").c_str());
        return false;
    }
    ModuleJson moduleJson;
    moduleJson.ParseFromFile(jsonPath);

    if (buildHashFinish || !moduleJson.GetGenerateBuildHash(generateBuildHash)) {
        return true;
    }

    if (!CopyFileToTempDir(jsonPath)) {
        return false;
    }

    if (!fs::exists(jsonPath)) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "Packager::setGenerateBuildHash failed for json file not exist").c_str());
        return false;
    }

    ModuleJson moduleJsonTemp;
    moduleJsonTemp.ParseFromFile(jsonPath);
    if (!moduleJsonTemp.GetGenerateBuildHash(generateBuildHash)) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "ModuleJson::GetGenerateBuildHash failed").c_str());
        return false;
    }

    if (!moduleJsonTemp.RemoveGenerateBuildHash()) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "ModuleJson::RemoveGenerateBuildHash failed").c_str());
        return false;
    }

    std::string prettyJsonString = moduleJsonTemp.ToString();
    if (prettyJsonString.empty()) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "ModuleJson::ToString failed").c_str());
        return false;
    }

    std::string realJsonPath;
    if (!Utils::GetRealPath(jsonPath, realJsonPath)) {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            ("get real json Path failed! jsonPath=" + jsonPath).c_str()).c_str());
        return false;
    }
    std::ofstream outFile(realJsonPath);
    if (outFile.is_open()) {
        outFile << prettyJsonString.c_str();
        outFile.close();
    } else {
        LOGE("%s", PackingToolErrMsg::SET_GENERATE_BUILD_HASH.toStringWithArgs(
            "Failed to open file for writing").c_str());
        return false;
    }
    return true;
}

bool Packager::CopyFileToTempDir(std::string &jsonPath)
{
    if (!fs::exists(jsonPath)) {
        LOGE("%s", PackingToolErrMsg::FILE_NOT_EXIST.toStringWithArgs(
            "Packager::copyFileToTempDir failed for json file not found.").c_str());
        return false;
    }
    fs::path oldFileParent = fs::path(jsonPath).parent_path();
    std::string tempDir = Constants::COMPRESSOR_TEMP_DIR + fs::path::preferred_separator + Utils::GenerateUUID();
    fs::path tempDirFs(tempDir);
    tempDir = oldFileParent.string() + fs::path::preferred_separator + tempDirFs.string();
    fs::create_directories(tempDir);
    fs::path fileName = JsonUtils::IsModuleJson(jsonPath) ? Constants::MODULE_JSON : Constants::CONFIG_JSON;
    std::string tempPath = tempDir + fs::path::preferred_separator + fileName.string();
    if (!Utils::CopyFile(jsonPath, tempPath)) {
        return false;
    }
    jsonPath = tempPath;
    return true;
}

bool Packager::BuildHash(bool &buildHashFinish, const bool &generateBuildHash,
    const std::map<std::string, std::string> &parameterMap, const std::string &jsonPath)
{
    if (buildHashFinish || !generateBuildHash) {
        return true;
    }
    std::map<std::string, std::string>::const_iterator it = parameterMap.find(Constants::PARAM_OUT_PATH);
    if (it == parameterMap.end()) {
        LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
            "out-path not found").c_str());
        return false;
    }
    std::string filePath = it->second;
    std::string hash = Utils::GetSha256File(filePath);
    return PutBuildHash(jsonPath, hash, buildHashFinish);
}

bool Packager::PutBuildHash(const std::string &jsonPath, const std::string &hash, bool &buildHashFinish)
{
    if (buildHashFinish) {
        return true;
    }

    ModuleJson moduleJson;
    moduleJson.ParseFromFile(jsonPath);
    moduleJson.SetBuildHash(hash);
    std::string prettyJsonString = moduleJson.ToString();
    if (prettyJsonString.empty()) {
        LOGE("%s", PackingToolErrMsg::WRITE_JSON_FILE_EXPECTION.toStringWithArgs(
            "ModuleJson::ToString failed").c_str());
        return false;
    }

    std::string realJsonPath;
    if (!Utils::GetRealPath(jsonPath, realJsonPath)) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("get real json path failed! jsonFile=" + jsonPath).c_str()).c_str());
        return false;
    }
    std::ofstream outFile(realJsonPath);
    if (outFile.is_open()) {
        outFile << prettyJsonString.c_str();
        outFile.close();
    } else {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            "Failed to open file for writing").c_str());
        return false;
    }

    buildHashFinish = true;
    return true;
}

bool Packager::IsModuleHap(const std::string& hapPath)
{
    if (!Utils::EndsWith(hapPath, Constants::HAP_SUFFIX)) {
        return false;
    }
    if (!ZipUtils::IsFileExistsInZip(hapPath, Constants::MODULE_JSON)) {
        return false;
    }
    return true;
}

void Packager::CompressPackinfoIntoHap(const std::string& hapPathItem, const std::string& unzipPathString,
    const std::string& outPathString, const std::string& packInfoPath)
{
    if (!fs::exists(fs::path(unzipPathString))) {
        fs::create_directory(fs::path(unzipPathString));
    }
    ZipUtils::Unzip(hapPathItem, unzipPathString);
    for (const auto& entry : fs::directory_iterator(unzipPathString)) {
        if (entry.path().filename() == Constants::PACK_INFO) {
            fs::remove(entry.path());
        }
    }

    std::string realPackInfoPath;
    if (!Utils::GetRealPath(packInfoPath, realPackInfoPath)) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("get real pack info path failed! packInfoPath=" + packInfoPath).c_str()).c_str());
        return;
    }
    std::string destFilePath = unzipPathString + fs::path::preferred_separator + Constants::PACK_INFO;
    std::string realDestFilePath;
    if (!Utils::GetRealPathOfNoneExistFile(destFilePath, realDestFilePath)) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("get real dest file path failed! destFilePath=" + destFilePath).c_str()).c_str());
        return;
    }
    std::ifstream packInfoFile(realPackInfoPath, std::ios::binary);
    std::ofstream destFile(realDestFilePath, std::ios::binary | std::ios::trunc);
    destFile << packInfoFile.rdbuf();
    destFile.close();
    packInfoFile.close();
    ZipUtils::Zip(unzipPathString, outPathString);
    if (fs::exists(fs::path(unzipPathString))) {
        fs::remove_all(fs::path(unzipPathString));
    }
}

bool Packager::IsOutDirectoryValid()
{
    auto it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
            "--out-path is empty.").c_str());
        return false;
    } else if (!Utils::IsDirectory(it->second)) {
        LOGE("%s", PackingToolErrMsg::OUT_PATH_INVALID.toStringWithArgs(
            "--out-path is not a directory.").c_str());
        return false;
    }
    return true;
}

bool Packager::ParseAtomicServiceSizeLimit()
{
    bool parseEntry = ParseAtomicServiceEntrySizeLimitParameter();
    bool parseNonEntry = ParseAtomicServiceNonEntrySizeLimitParameter();
    return parseEntry && parseNonEntry;
}

bool Packager::ParseAtomicServiceEntrySizeLimitParameter()
{
    auto it = parameterMap_.find(Constants::PARAM_ATOMIC_SERVICE_ENTRY_SIZE_LIMIT);
    int32_t entrySizeLimit = Constants::ATOMIC_SERVICE_ENTRY_SIZE_LIMIT_DEFAULT;
    if (it != parameterMap_.end()) {
        try {
            entrySizeLimit = std::stoi(it->second);
        } catch (const std::exception& e) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                "ParseAtomicServiceEntrySizeLimitParameter failed, "
                "input --atomic-service-entry-size-limit value invalid.").c_str());
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("Exception: " + std::string(e.what())).c_str()).c_str());
            return false;
        }
        if (entrySizeLimit < 0 || entrySizeLimit > Constants::ATOMIC_SERVICE_TOTAL_SIZE_LIMIT_MAX) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                "ParseAtomicServiceEntrySizeLimitParameter failed, "
                "input --atomic-service-entry-size-limit value out of range [0,4194304].").c_str());
            return false;
        }
    }
    Packager::setAtomicServiceEntrySizeLimit(entrySizeLimit);
    return true;
}

bool Packager::ParseAtomicServiceNonEntrySizeLimitParameter()
{
    auto it = parameterMap_.find(Constants::PARAM_ATOMIC_SERVICE_NON_ENTRY_SIZE_LIMIT);
    int32_t nonEntrySizeLimit = Constants::ATOMIC_SERVICE_NON_ENTRY_SIZE_LIMIT_DEFAULT;
    if (it != parameterMap_.end()) {
        try {
            nonEntrySizeLimit = std::stoi(it->second);
        } catch (const std::exception& e) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                "ParseAtomicServiceEntrySizeLimitParameter failed, "
                "input --atomic-service-non-entry-size-limit value invalid.").c_str());
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                ("Exception: " + std::string(e.what())).c_str()).c_str());
            return false;
        }
        if (nonEntrySizeLimit < 0 || nonEntrySizeLimit > Constants::ATOMIC_SERVICE_TOTAL_SIZE_LIMIT_MAX) {
            LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
                "ParseAtomicServiceNonEntrySizeLimitParameter failed, "
                "input --atomic-service-non-entry-size-limit value out of range [0,4194304].").c_str());
            return false;
        }
    }
    Packager::setAtomicServiceNonEntrySizeLimit(nonEntrySizeLimit);
    return true;
}

int32_t Packager::getAtomicServiceEntrySizeLimit()
{
    return atomicServiceEntrySizeLimit_;
}

void Packager::setAtomicServiceEntrySizeLimit(int32_t atomicServiceEntrySizeLimit)
{
    atomicServiceEntrySizeLimit_ = atomicServiceEntrySizeLimit;
}

int32_t Packager::getAtomicServiceNonEntrySizeLimit()
{
    return atomicServiceNonEntrySizeLimit_;
}

void Packager::setAtomicServiceNonEntrySizeLimit(int32_t atomicServiceNonEntrySizeLimit)
{
    atomicServiceNonEntrySizeLimit_ = atomicServiceNonEntrySizeLimit;
}

void Packager::ScanSoFiles()
{
    if (parameterMap_.find(Constants::PARAM_STAT_DUPLICATE) == parameterMap_.end() ||
        parameterMap_.at(Constants::PARAM_STAT_DUPLICATE) == "false") {
        return;
    }

    if (parameterMap_.find(Constants::PARAM_MODE) == parameterMap_.end() ||
        parameterMap_.find(Constants::PARAM_OUT_PATH) == parameterMap_.end()) {
        LOGE("%s", PackingToolErrMsg::COMMAND_PARSER_FAILED.toStringWithArgs(
            "No mode parameters or no output path!").c_str());
        return;
    }
    std::string mode = parameterMap_.at(Constants::PARAM_MODE);
    if (mode != Constants::MODE_APP
        && mode != Constants::MODE_FAST_APP
        && mode != Constants::MODE_MULTIAPP) {
        return;
    }

    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    if (fs::exists(outPath)) {
        ScanStatDuplicate scanStatDuplicate;
        fs::path filePath(outPath);
        std::string reportPath = filePath.parent_path().string() + Constants::LINUX_FILE_SEPARATOR + SCAN_RESULT;
        if (!scanStatDuplicate.ScanSoFiles(outPath)) {
            LOGE("%s", PackingToolErrMsg::SCAN_SO_FILES_EXCEPTION.toStringWithArgs(
                ("Scan so files in app exist Exception: make scan report failed! App path = " + outPath).c_str()).c_str());
            fs::remove_all(reportPath);
        } else {
            std::string realFilePath;
            if (!Utils::GetRealPath(reportPath, realFilePath)) {
                LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
                ("get real report path failed! Report path = " + reportPath).c_str()).c_str());
            } else {
                LOGW("Scanning for duplicate SO files has completed successfully. The scan report is located at = %s",
                    realFilePath.c_str());
            }
        }
    }
    return;
}
} // namespace AppPackingTool
} // namespace OHOS