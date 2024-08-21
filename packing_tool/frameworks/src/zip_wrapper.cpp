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

#include "zip_wrapper.h"

#include <fstream>
#include <iostream>

#include "log.h"

namespace OHOS {
namespace AppPackingTool {
ZipWrapper::ZipWrapper() : zipFile_(nullptr)
{}

ZipWrapper::ZipWrapper(std::string zipPath) : zipFile_(nullptr), zipFilePath_(zipPath)
{}

ZipWrapper::~ZipWrapper()
{
    Close();
}

int32_t ZipWrapper::Open(std::string& zipPath, int32_t append)
{
    zipFilePath_ = zipPath;
    return Open(append);
}

int32_t ZipWrapper::Open(int32_t append)
{
    if (zipFile_ != nullptr) {
        LOGE("zip file handle has open");
        return ZIP_ERR_SUCCESS;
    }
    zipFile_ = zipOpen64(zipFilePath_.c_str(), append);
    if (zipFile_ == nullptr) {
        LOGE("zip file handle open failed");
        return ZIP_ERR_FAILURE;
    }
    return ZIP_ERR_SUCCESS;
}

void ZipWrapper::Close()
{
    if (zipFile_ != nullptr) {
        zipClose(zipFile_, nullptr);
        zipFile_ = nullptr;
    }
}

int32_t ZipWrapper::AddFileOrDirectoryToZip(const std::string &filePath, const std::string &zipPath)
{
    return AddFileOrDirectoryToZip(fs::path(filePath), fs::path(zipPath));
}

int32_t ZipWrapper::AddFileOrDirectoryToZip(const fs::path &fsFilePath, const fs::path &fsZipPath)
{
    int32_t ret = ZIP_ERR_SUCCESS;
    if (fs::is_directory(fsFilePath)) {
        LOGI("[%s] is directory to [%s]", fsFilePath.string().c_str(), fsZipPath.string().c_str());
        int32_t count = 0;
        for (const auto &entry : fs::directory_iterator(fsFilePath)) {
            fs::path fsZipFullPath = fsZipPath / entry.path().filename();
            LOGI("Add File %s from %s", entry.path().string().c_str(), fsZipFullPath.string().c_str());
            ret = AddFileOrDirectoryToZip(entry.path(), fsZipFullPath);
            if (ret != ZIP_ERR_SUCCESS) {
                LOGE("AddFileOrDirectoryToZip failed![%s]", fsFilePath.string().c_str());
                return ret;
            }
            count++;
        }
        if (count == 0) {
            ret = AddEmptyDirToZip(fsZipPath);
            if (ret != ZIP_ERR_SUCCESS) {
                LOGE("AddDirToZip failed![%s]", fsFilePath.string().c_str());
                return ret;
            }
        }
    } else if (fs::is_regular_file(fsFilePath)) {
        ret = AddFileToZip(fsFilePath, fsZipPath);
        if (ret != ZIP_ERR_SUCCESS) {
            LOGE("AddFileToZip failed![%s]", fsFilePath.string().c_str());
            return ret;
        }
    }
    return ZIP_ERR_SUCCESS;
}

int32_t ZipWrapper::WriteStringToZip(const std::string &content, const std::string& zipPath)
{
    if (!IsOpen()) {
        LOGE("zip file is not open");
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DEFAULT;
    int ret = zipOpenNewFileInZip64(zipFile_, zipPath.c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, 0, static_cast<int>(zipLevel_), 1);
    if (ret != ZIP_OK) {
        LOGE("open file in zip failed![ret=%d][zipPath=%s]", ret, zipPath.c_str());
        return ZIP_ERR_FAILURE;
    }
    ret = zipWriteInFileInZip(zipFile_, content.data(), content.length());
    if (ret < 0) {
        LOGE("write file in zip failed![errno=%d][ret=%d]", errno, ret);
        ret = ZIP_ERR_FAILURE;
    }
    zipCloseFileInZip(zipFile_);
    return ZIP_ERR_SUCCESS;
}

int32_t ZipWrapper::AddEmptyDirToZip(const std::string &zipPath)
{
    return AddEmptyDirToZip(fs::path(zipPath));
}

int32_t ZipWrapper::AddEmptyDirToZip(const fs::path &fsZipPath)
{
    fs::path fsZipDirPath = fs::path(fsZipPath.string() + "/");
    LOGI("Add [%s] into zip", fsZipDirPath.string().c_str());
    if (!IsOpen()) {
        LOGE("zip file is not open");
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DIRECTORY;
    int ret = zipOpenNewFileInZip64(zipFile_, fsZipDirPath.string().c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, 0, static_cast<int>(zipLevel_), 1);
    if (ret != ZIP_OK) {
        LOGE("open file in zip failed![ret=%d][zipPath=%s]", ret, fsZipPath.string().c_str());
        return ZIP_ERR_FAILURE;
    }
    zipCloseFileInZip(zipFile_);
    return ZIP_ERR_SUCCESS;
}

int32_t ZipWrapper::AddFileToZip(const std::string &filePath, const std::string &zipPath)
{
    return AddFileToZip(fs::path(filePath), fs::path(zipPath));
}

int32_t ZipWrapper::AddFileToZip(const fs::path &fsFilePath, const fs::path &fsZipPath)
{
    if (!IsOpen()) {
        LOGE("zip file is not open");
        return ZIP_ERR_FAILURE;
    }
    if (!fs::is_regular_file(fsFilePath)) {
        LOGE("filePath is not regular file![filePath=%s]", fsFilePath.string().c_str());
        return ZIP_ERR_FAILURE;
    }
    std::ifstream file(fsFilePath.string(), std::ios::binary);
    if (!file.is_open()) {
        LOGE("open file failed![filePath=%s]", fsFilePath.string().c_str());
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DEFAULT;
    int ret = zipOpenNewFileInZip64(zipFile_, fsZipPath.string().c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, 0, static_cast<int>(zipLevel_), 1);
    if (ret != ZIP_OK) {
        LOGE("open file in zip failed![ret=%d][fsZipPath=%s]", ret, fsZipPath.string().c_str());
        file.close();
        return ZIP_ERR_FAILURE;
    }
    int32_t result = ZIP_ERR_SUCCESS;
    if (fs::file_size(fsFilePath) > 0) {
        result = ZIP_ERR_SUCCESS;
        char buffer[MAX_ZIP_BUFFER_SIZE];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            if (file.fail() && !file.eof()) {
                LOGE("read file error!");
                break;
            }
            auto bytesRead = file.gcount();
            if (bytesRead <= 0) {
                LOGE("read file bytes error![filePath=%s][bytesRead=%u]", fsFilePath.c_str(), bytesRead);
                result = ZIP_ERR_FAILURE;
                break;
            }
            if (zipWriteInFileInZip(zipFile_, buffer, bytesRead) < 0) {
                LOGE("write file in zip failed!");
                result = ZIP_ERR_FAILURE;
                break;
            }
        }
    }
    zipCloseFileInZip(zipFile_);
    file.close();
    return result;
}
} // namespace AppPackingTool
} // namespace OHOS
