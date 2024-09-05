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

#include "unzip_wrapper.h"

#include <fstream>
#include <iostream>

#include "log.h"

namespace OHOS {
namespace AppPackingTool {
UnzipWrapper::UnzipWrapper()
{}

UnzipWrapper::UnzipWrapper(std::string path) : unzFilePath_(path)
{}

UnzipWrapper::~UnzipWrapper()
{
    Close();
}

int32_t UnzipWrapper::Open(std::string& unzPath)
{
    unzFilePath_ = unzPath;
    return Open();
}

int32_t UnzipWrapper::Open()
{
    if (unzFile_ != nullptr) {
        LOGE("unzip file handle has open");
        return ZIP_ERR_SUCCESS;
    }
    unzFile_ = unzOpen64(unzFilePath_.c_str());
    if (unzFile_ == nullptr) {
        LOGE("unzip file handle open failed");
        return ZIP_ERR_FAILURE;
    }
    return ZIP_ERR_SUCCESS;
}

void UnzipWrapper::Close()
{
    if (unzFile_ != nullptr) {
        unzClose(unzFile_);
        unzFile_ = nullptr;
    }
}

std::string UnzipWrapper::ExtractFile(const std::string filePath)
{
    char filename[MAX_ZIP_BUFFER_SIZE] = {0};
    unz_file_info64 fileInfo;
    if (unzGetCurrentFileInfo64(unzFile_, &fileInfo, filename, MAX_ZIP_BUFFER_SIZE, NULL, 0, NULL, 0) != UNZ_OK) {
        LOGE("get current file info in zip failed!");
        return "";
    }
    fs::path fsUnzipPath(filename);
    fs::path fsFilePath(filePath);
    fs::path fsFullFilePath = fsFilePath / filename;
    if (fileInfo.external_fa == ZIP_FILE_ATTR_DIRECTORY ||
        (fsUnzipPath.string().rfind('/') == fsUnzipPath.string().length() - 1)) {
        if (!fs::exists(fsFullFilePath)) {
            LOGD("fsFullFilePath not exist, create: %s", fsFullFilePath.string().c_str());
            fs::create_directories(fsFullFilePath.string());
        }
        return fsFullFilePath.string();
    }
    if (!fs::exists(fsFullFilePath.parent_path())) {
        LOGD("fsFullFilePath not exists, create : %s", fsFullFilePath.parent_path().string().c_str());
        fs::create_directories(fsFullFilePath.parent_path().string());
    }
    if (unzOpenCurrentFile(unzFile_) != UNZ_OK) {
        LOGE("open current file in zip failed![filename=%s]", filename);
        return "";
    }
    std::fstream file;
    file.open(fsFullFilePath.string(), std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        LOGE("open file failed![fsFullFilePath=%s]", fsFullFilePath.string().c_str());
        return "";
    }
    char fileData[MAX_ZIP_BUFFER_SIZE] = {0};
    int32_t bytesRead;
    do {
        bytesRead = unzReadCurrentFile(unzFile_, (voidp)fileData, MAX_ZIP_BUFFER_SIZE);
        if (bytesRead < 0) {
            LOGE("Read current file in zip failed!!");
            file.close();
            return "";
        }
        file.write(fileData, bytesRead);
    } while (bytesRead > 0);
    file.close();
    unzCloseCurrentFile(unzFile_);
    return fsFullFilePath.string();
}

int32_t UnzipWrapper::UnzipFile(std::string filePath)
{
    LOGD("Unzip file[%s] to [%s]", unzFilePath_.c_str(), filePath.c_str());
    if (unzFile_ == nullptr) {
        LOGE("zip file not open");
        return ZIP_ERR_FAILURE;
    }
    if (unzGetGlobalInfo64(unzFile_, &unzGlobalInfo_) != UNZ_OK) {
        LOGE("Get global info failed!");
        return ZIP_ERR_FAILURE;
    }
    int ret = UNZ_OK;
    for (size_t i = 0; i < unzGlobalInfo_.number_entry; ++i) {
        std::string f = ExtractFile(filePath);
        if (f.empty()) {
            LOGE("Extract file failed!");
            return ZIP_ERR_FAILURE;
        }
        ret = unzGoToNextFile(unzFile_);
        if (ret == UNZ_END_OF_LIST_OF_FILE) {
            break;
        } else if (ret != UNZ_OK) {
            LOGE("Go to next file in zip failed!");
            return ZIP_ERR_FAILURE;
        }
    }
    return ZIP_ERR_SUCCESS;
}
} // namespace AppPackingTool
} // namespace OHOS
