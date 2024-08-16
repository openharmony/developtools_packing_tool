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

#include "zip_utils.h"

#include <algorithm>
#include <cstring>
#include <map>
#include <iostream>

#include "log.h"
#include "utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
    const std::string RESOURCE_PATH = "resources/base/profile/";
}

ZipUtils::ZipUtils()
{}

ZipUtils::~ZipUtils()
{}

int32_t ZipUtils::Zip(const std::string& filePath, const std::string& zipFilePath,
    const std::string& zipPath, const ZipLevel& zipLevel, const inint32_tt& append)
{
    ZipWrapper zipWrapper(zipFilePath);
    if (zipWrapper.Open(append) != ZIP_ERR_SUCCESS) {
        LOGE("ZipWrapper Open failed!");
        return ZIP_ERR_FAILURE;
    }
    if (zipLevel != ZipLevel::ZIP_LEVEL_DEFAULT) {
        zipWrapper.SetZipLevel(zipLevel);
    }
    int32_t ret = zipWrapper.AddFileOrDirectoryToZip(filePath, zipPath);
    if (ret != ZIP_ERR_SUCCESS) {
        LOGE("ZipWrapper AddFileOrDirectoryToZip failed!");
    }
    zipWrapper.Close();
    return ret;
}

int32_t ZipUtils::Unzip(const std::string& zipPath, const std::string& filePath)
{
    UnzipWrapper unzipWrapper(zipPath);
    if (unzipWrapper.Open() != ZIP_ERR_SUCCESS) {
        LOGE("UnzipWrapper Open failed!");
        return ZIP_ERR_FAILURE;
    }
    int32_t ret = unzipWrapper.UnzipFile(filePath);
    if (ret != ZIP_ERR_SUCCESS) {
        LOGE("UnzipWrapper UnzipFile failed!");
    }
    unzipWrapper.Close();
    return ret;
}

bool ZipUtils::IsFileExistsInZip(const std::string& zipFilePath, const std::string& filename)
{
    fs::path fsZipFilePath(zipFilePath);
    if (!fs::is_regular_file(fsZipFilePath)) {
        LOGE("Zip file is not a regular file!");
        return false;
    }
    unzFile unzipFile = unzOpen64(zipFilePath.c_str());
    if (unzipFile == nullptr) {
        LOGE("Open zip file failed! zipFilePath=%s", zipFilePath.c_str());
        return false;
    }
    unz_global_info64 unzGlobalInfo;
    if (unzGetGlobalInfo64(unzipFile, &unzGlobalInfo) != UNZ_OK) {
        LOGE("Get zip global info! zipFilePath=%s", zipFilePath.c_str());
        unzClose(unzipFile);
        return false;
    }
    char filePathInZip[MAX_ZIP_BUFFER_SIZE] = {0};
    unz_file_info64 fileInfo;
    int ret = 0;
    bool isExist = false;
    for (size_t i = 0; i < unzGlobalInfo.number_entry; ++i) {
        if (unzGetCurrentFileInfo64(unzipFile, &fileInfo, filePathInZip, MAX_ZIP_BUFFER_SIZE, NULL, 0, NULL, 0) !=
            UNZ_OK) {
            LOGE("Get current file info in zip failed!");
            break;
        }
        std::string strFilePathInZip(filePathInZip);
        if (fileInfo.external_fa != ZIP_FILE_ATTR_DIRECTORY && strFilePathInZip.length() >= filename.length()) {
            std::transform(strFilePathInZip.begin(), strFilePathInZip.end(), strFilePathInZip.begin(), ::tolower);
            if (strFilePathInZip.compare(filename) == 0) {
                isExist = true;
                break;
            }
        }
        ret = unzGoToNextFile(unzipFile);
        if (ret == UNZ_END_OF_LIST_OF_FILE) {
            break;
        } else if (ret != UNZ_OK) {
            LOGE("Go to next file in zip failed!");
            break;
        }
    }
    unzClose(unzipFile);
    return isExist;
}

bool ZipUtils::IsFileNameExistsInZip(const std::string& zipFilePath, const std::string& filename)
{
    fs::path fsZipFilePath(zipFilePath);
    if (!fs::is_regular_file(fsZipFilePath)) {
        LOGE("Zip file is not a regular file!");
        return false;
    }
    unzFile unzipFile = unzOpen64(zipFilePath.c_str());
    if (unzipFile == nullptr) {
        LOGE("Open zip file failed! zipFilePath=%s", zipFilePath.c_str());
        return false;
    }
    unz_global_info64 unzGlobalInfo;
    if (unzGetGlobalInfo64(unzipFile, &unzGlobalInfo) != UNZ_OK) {
        LOGE("Get zip global info! zipFilePath=%s", zipFilePath.c_str());
        unzClose(unzipFile);
        return false;
    }
    char filePathInZip[MAX_ZIP_BUFFER_SIZE] = {0};
    unz_file_info64 fileInfo;
    int ret = 0;
    bool isExist = false;
    for (size_t i = 0; i < unzGlobalInfo.number_entry; ++i) {
        if (unzGetCurrentFileInfo64(unzipFile, &fileInfo, filePathInZip, MAX_ZIP_BUFFER_SIZE, NULL, 0, NULL, 0) !=
            UNZ_OK) {
            LOGE("Get current file info in zip failed!");
            break;
        }
        std::string strFilePathInZip(filePathInZip);
        if (fileInfo.external_fa != ZIP_FILE_ATTR_DIRECTORY && strFilePathInZip.length() >= filename.length()) {
            if (strFilePathInZip.substr(strFilePathInZip.length() - filename.length()).compare(filename) == 0) {
                isExist = true;
                break;
            }
        }
        ret = unzGoToNextFile(unzipFile);
        if (ret == UNZ_END_OF_LIST_OF_FILE) {
            break;
        } else if (ret != UNZ_OK) {
            LOGE("Go to next file in zip failed!");
            break;
        }
    }
    unzClose(unzipFile);
    return isExist;
}

bool ZipUtils::GetFileContentFromZip(const std::string& zipFilePath, const std::string& filename,
    std::string& fileContent)
{
    fs::path fsZipFilePath(zipFilePath);
    if (!fs::is_regular_file(fsZipFilePath)) {
        LOGE("Zip file is not a regular file!");
        return false;
    }
    unzFile unzipFile = unzOpen64(zipFilePath.c_str());
    if (unzipFile == nullptr) {
        LOGE("Open zip file failed! zipFilePath=%s", zipFilePath.c_str());
        return false;
    }
    if (unzLocateFile(unzipFile, filename.c_str(), 0) != UNZ_OK) {
        LOGE("Locate file failed! filename=%s", filename.c_str());
        unzClose(unzipFile);
        return false;
    }
    unz_file_info64 fileInfo;
    char filePathInZip[MAX_ZIP_BUFFER_SIZE] = {0};
    if (unzGetCurrentFileInfo64(unzipFile, &fileInfo, filePathInZip, MAX_ZIP_BUFFER_SIZE, NULL, 0, NULL, 0) !=
        UNZ_OK) {
        LOGE("Get current file info in zip failed! filename=%s", filename.c_str());
        unzClose(unzipFile);
        return false;
    }
    if (unzOpenCurrentFile(unzipFile) != UNZ_OK) {
        LOGE("Open current file in zip failed! filename=%s", filename.c_str());
        unzClose(unzipFile);
        return false;
    }
    char buffer[MAX_ZIP_BUFFER_SIZE];
    int readLen = 0;
    fileContent = "";
    do {
        std::fill_n(buffer, MAX_ZIP_BUFFER_SIZE, '\0');
        readLen = unzReadCurrentFile(unzipFile, buffer, MAX_ZIP_BUFFER_SIZE);
        if (readLen < 0) {
            LOGE("Read current file in zip failed! filename=%s", filename.c_str());
            unzCloseCurrentFile(unzipFile);
            unzClose(unzipFile);
            return false;
        } else if (readLen == 0) {
            break;
        }
        fileContent += std::string(buffer);
    } while (readLen > 0);
    unzCloseCurrentFile(unzipFile);
    unzClose(unzipFile);
    return true;
}

bool ZipUtils::GetUnzipCurrentFileContent(unzFile& unzipFile, std::string& fileContent)
{
    if (unzipFile == nullptr) {
        return false;
    }
    if (unzOpenCurrentFile(unzipFile) != UNZ_OK) {
        LOGE("Open current file in zip failed!");
        return false;
    }
    char buffer[MAX_ZIP_BUFFER_SIZE];
    int readLen = 0;
    fileContent = "";
    do {
        std::fill_n(buffer, MAX_ZIP_BUFFER_SIZE, '\0');
        readLen = unzReadCurrentFile(unzipFile, buffer, MAX_ZIP_BUFFER_SIZE);
        if (readLen < 0) {
            LOGE("Read current file in zip failed!");
            unzCloseCurrentFile(unzipFile);
            return false;
        } else if (readLen == 0) {
            break;
        }
        fileContent += std::string(buffer);
    } while (readLen > 0);
    unzCloseCurrentFile(unzipFile);
    return true;
}

bool ZipUtils::AddToResourceMap(unzFile& unzipFile, const std::string& filePathInZip,
    std::map<std::string, std::string>& resourceMap)
{
    if (unzipFile == nullptr) {
        LOGE("zip file not open!");
        return false;
    }
    if (filePathInZip.find(RESOURCE_PATH) != std::string::npos) {
        std::string fileName = Utils::ReplaceAll(filePathInZip, RESOURCE_PATH, "");
        std::string fileContent;
        if (!GetUnzipCurrentFileContent(unzipFile, fileContent)) {
            LOGE("Get current file content failed! filename=%s", filePathInZip.c_str());
            return false;
        } else {
            resourceMap.emplace(fileName, fileContent);
        }
    }
    return true;
}

bool ZipUtils::GetResourceMapFromZip(const std::string& zipFilePath, std::map<std::string, std::string>& resourceMap)
{
    fs::path fsZipFilePath(zipFilePath);
    if (!fs::is_regular_file(fsZipFilePath)) {
        LOGE("Zip file is not a regular file!");
        return false;
    }
    unzFile unzipFile = unzOpen64(zipFilePath.c_str());
    if (unzipFile == nullptr) {
        LOGE("Open zip file failed! zipFilePath=%s", zipFilePath.c_str());
        return false;
    }
    unz_global_info64 unzGlobalInfo;
    if (unzGetGlobalInfo64(unzipFile, &unzGlobalInfo) != UNZ_OK) {
        LOGE("Get zip global info! zipFilePath=%s", zipFilePath.c_str());
        unzClose(unzipFile);
        return false;
    }
    char filePathInZip[MAX_ZIP_BUFFER_SIZE] = {0};
    unz_file_info64 fileInfo;
    int ret = 0;
    for (size_t i = 0; i < unzGlobalInfo.number_entry; ++i) {
        if (unzGetCurrentFileInfo64(unzipFile, &fileInfo, filePathInZip, MAX_ZIP_BUFFER_SIZE, NULL, 0, NULL, 0) !=
            UNZ_OK) {
            LOGE("Get current file info in zip failed!");
            break;
        }
        std::string strFilePathInZip(filePathInZip);
        if (fileInfo.external_fa != ZIP_FILE_ATTR_DIRECTORY && strFilePathInZip.length() > RESOURCE_PATH.length()) {
            AddToResourceMap(unzipFile, strFilePathInZip, resourceMap);
        }
        ret = unzGoToNextFile(unzipFile);
        if (ret == UNZ_END_OF_LIST_OF_FILE) {
            break;
        } else if (ret != UNZ_OK) {
            LOGE("Go to next file in zip failed!");
            break;
        }
    }
    unzClose(unzipFile);
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
