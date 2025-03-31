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

#include "packager.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>

#include "json/json_utils.h"
#include "log.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string EN_US_UTF_8 = "en_US.UTF-8";
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
        LOGE("InitAllowedParam err");
        return ret;
    }

    ret = PreProcess();
    if (ret != ERR_OK) {
        LOGE("PreProcess err");
        return ret;
    }
    ret = Process();
    if (ret != ERR_OK) {
        LOGE("Process err");
        return ret;
    }

    ret = PostProcess();
    if (ret != ERR_OK) {
        LOGE("PostProcess err");
        return ret;
    }
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
        LOGE("Packager::commandVerify forceRewrite is invalid.");
        return false;
    }
    return true;
}

bool Packager::IsPathValid(const std::string &path, const bool &isFile, const std::string suffix)
{
    if (isFile && fs::is_regular_file(path)) {
        std::string name = fs::path(path).filename();
        std::locale englishLocale(EN_US_UTF_8);
        std::transform(name.begin(), name.end(), name.begin(),
                       [&englishLocale](unsigned char c) { return std::tolower(c); });
        if (Utils::EndsWith(name, suffix)) {
            return true;
        }
    }
    return (!isFile) && fs::is_directory(path);
}

bool Packager::SplitDirList(const std::string &dirList, std::list<std::string> &fileList)
{
    std::list<std::string> pathList;
    RemoveDuplicatePath(dirList, pathList);
    for (const std::string &pathItem : pathList) {
        std::string formattedPathItem;
        if (!Utils::GetFormattedPath(pathItem, formattedPathItem)) {
            LOGE("GetFormattedPath failed for %s", pathItem.c_str());
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
                LOGE("GetFormattedPath failed for %s", pathItem.c_str());
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
                LOGE("GetFormattedPath failed for %s", pathItem.c_str());
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

bool Packager::EnsureParentDirectoryExists(const sdt::filesystem::path& filePath)
{
    try {
        const auto parentPath = filePath.parent_path();
        if (!std::filesystem::exists(parentPath)) {
            std::error_code ec;
            if (!std::filesystem::create_directories(parentPath, ec)) {
                LOGE("Packager::Failed to create directory: [%s]", ec.message().c_str());
                return false;
            }
        }
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        LOGE("Packager::Directory creation error: [%s]", e.what());
        return false;
    }
}

bool Packager::IsOutPathValid(const std::string &outPath, const std::string &forceRewrite, const std::string &suffix)
{
    const fs::path filePath(outPath);

    if ("false" == forceRewrite && fs::exists(filePath)) {
        LOGE("Packager::isOutPathValid out file already existed.");
        return false;
    }
	
    if (!EnsureParentDirectoryExists(filePath) {
        return false;
	}

    if (suffix == Constants::HAP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HAP_SUFFIX)) {
            LOGE("Packager::isOutPathValid out-path must end with .hap.");
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::HAR_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HAR_SUFFIX)) {
            LOGE("Packager::isOutPathValid out-path must end with .har.");
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::APP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::APP_SUFFIX)) {
            LOGE("Packager::isOutPathValid out-path must end with .app.");
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::RES_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::RES_SUFFIX)) {
            LOGE("Packager::isOutPathValid out-path must end with .res.");
            return false;
        } else {
            return true;
        }
    } else if (suffix == Constants::HSP_SUFFIX) {
        if (!Utils::EndsWith(filePath.filename().string(), Constants::HSP_SUFFIX)) {
            LOGE("Packager::isOutPathValid out-path must end with .hsp.");
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
        LOGE("Packager::setGenerateBuildHash failed for json file not exist");
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
        LOGE("Packager::setGenerateBuildHash failed for json file not exist");
        return false;
    }

    ModuleJson moduleJsonTemp;
    moduleJsonTemp.ParseFromFile(jsonPath);
    if (!moduleJsonTemp.GetGenerateBuildHash(generateBuildHash)) {
        LOGE("ModuleJson::GetGenerateBuildHash failed");
        return false;
    }

    if (!moduleJsonTemp.RemoveGenerateBuildHash()) {
        LOGE("ModuleJson::RemoveGenerateBuildHash failed");
        return false;
    }
    
    std::string prettyJsonString = moduleJsonTemp.ToString();
    if (prettyJsonString.empty()) {
        LOGE("ModuleJson::ToString failed");
        return false;
    }

    std::string realJsonPath;
    if (!Utils::GetRealPath(jsonPath, realJsonPath)) {
        LOGE("get real json Path failed! jsonPath=%s", jsonPath.c_str());
        return false;
    }
    std::ofstream outFile(realJsonPath);
    if (outFile.is_open()) {
        outFile << prettyJsonString.c_str();
        outFile.close();
    } else {
        LOGE("Failed to open file for writing");
        return false;
    }
    return true;
}

bool Packager::CopyFileToTempDir(std::string &jsonPath)
{
    if (!fs::exists(jsonPath)) {
        LOGE("Packager::copyFileToTempDir failed for json file not found.");
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
        LOGE("out-path not found");
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
        LOGE("ModuleJson::ToString failed");
        return false;
    }

    std::string realJsonPath;
    if (!Utils::GetRealPath(jsonPath, realJsonPath)) {
        LOGE("get real json path failed! jsonFile=%s", jsonPath.c_str());
        return false;
    }
    std::ofstream outFile(realJsonPath);
    if (outFile.is_open()) {
        outFile << prettyJsonString.c_str();
        outFile.close();
    } else {
        LOGE("Failed to open file for writing");
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
        LOGE("get real pack info path failed! packInfoPath=%s", packInfoPath.c_str());
        return;
    }
    std::string destFilePath = unzipPathString + fs::path::preferred_separator + Constants::PACK_INFO;
    std::string realDestFilePath;
    if (!Utils::GetRealPathOfNoneExistFile(destFilePath, realDestFilePath)) {
        LOGE("get real dest file path failed! destFilePath=%s", destFilePath.c_str());
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
        LOGE("Validate out-path is empty");
        return false;
    } else if (!Utils::IsDirectory(it->second)) {
        LOGE("Validate out-path is not a directory.");
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS