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
#include "utils.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

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

zipFile ZipWrapper::GetZipFile() const
{
    return zipFile_;
}

int32_t ZipWrapper::Open(std::string& zipPath, int32_t append)
{
    zipFilePath_ = zipPath;
    return Open(append);
}

int32_t ZipWrapper::Open(int32_t append)
{
    if (zipFile_ != nullptr) {
        // LOGE("zip file handle has open");
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zip file handle has open").c_str());
        return ZIP_ERR_SUCCESS;
    }
    zipFile_ = zipOpen64(zipFilePath_.c_str(), append);
    if (zipFile_ == nullptr) {
        // LOGE("zip file handle open failed");
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zip file handle open failed").c_str());
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

int32_t ZipWrapper::AddFileOrDirectoryToZip(const std::string &filePath,
                                            const std::string &zipPath,
                                            const bool isCompress,
                                            const ZipLevel &zipLevel)
{
    return AddFileOrDirectoryToZip(fs::path(filePath), fs::path(zipPath), isCompress, zipLevel);
}

int32_t ZipWrapper::AddFileOrDirectoryToZip(const fs::path &fsFilePath,
                                            const fs::path &fsZipPath,
                                            const bool isCompress,
                                            const ZipLevel &zipLevel)
{
    int32_t ret = ZIP_ERR_SUCCESS;
    if (fs::is_directory(fsFilePath)) {
        LOGD("[%s] is directory to [%s]", fsFilePath.string().c_str(), fsZipPath.string().c_str());
        for (const auto &entry : fs::directory_iterator(fsFilePath)) {
            fs::path fsZipFullPath = fsZipPath / entry.path().filename();
            ret = AddFileOrDirectoryToZip(entry.path(), fsZipFullPath, isCompress, zipLevel);
            if (ret != ZIP_ERR_SUCCESS) {
                // LOGE("AddFileOrDirectoryToZip failed![%s]", fsFilePath.string().c_str());
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    ("AddFileOrDirectoryToZip failed![" + fsFilePath.string() + "]").c_str()).c_str());
                return ret;
            }
        }
    } else if (fs::is_regular_file(fsFilePath)) {
        ret = AddFileToZip(fsFilePath, fsZipPath, isCompress, zipLevel);
        if (ret != ZIP_ERR_SUCCESS) {
            // LOGE("AddFileToZip failed![%s]", fsFilePath.string().c_str());
            LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                ("AddFileToZip failed![" + fsFilePath.string() + "]").c_str()).c_str());
            return ret;
        }
    }
    return ZIP_ERR_SUCCESS;
}

int32_t ZipWrapper::WriteStringToZip(const std::string &content, const std::string& zipPath)
{
    if (!IsOpen()) {
        // LOGE("zip file is not open");
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zip file is not open").c_str());
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DEFAULT;
    int ret = zipOpenNewFileInZip64(zipFile_, zipPath.c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, 0, static_cast<int>(zipLevel_), 1);
    if (ret != ZIP_OK) {
        // LOGE("open file in zip failed![ret=%d][zipPath=%s]", ret, zipPath.c_str());
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            ("open file in zip failed![ret=" + std::to_string(ret) + "][zipPath=" + zipPath + "]").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }
    ret = zipWriteInFileInZip(zipFile_, content.data(), content.length());
    if (ret < 0) {
        // LOGE("write file in zip failed![errno=%d][ret=%d]", errno, ret);
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            ("write file in zip failed![errno=" + std::to_string(errno) + "][ret=" + std::to_string(ret) + "]").c_str()).c_str());
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
    LOGD("Add [%s] into zip", fsZipDirPath.string().c_str());
    if (!IsOpen()) {
        // LOGE("zip file is not open");
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zip file is not open").c_str());
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DIRECTORY;
    int ret = zipOpenNewFileInZip64(zipFile_, fsZipDirPath.string().c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, 0, static_cast<int>(zipLevel_), 1);
    if (ret != ZIP_OK) {
        // LOGE("open file in zip failed![ret=%d][zipPath=%s]", ret, fsZipPath.string().c_str());
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            ("open file in zip failed![ret=" + std::to_string(ret) + "][zipPath=" + fsZipPath.string() + "]").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }
    zipCloseFileInZip(zipFile_);
    return ZIP_ERR_SUCCESS;
}

int32_t ZipWrapper::AddFileToZip(const std::string &filePath,
                                 const std::string &zipPath,
                                 const bool isCompress,
                                 const ZipLevel &zipLevel)
{
    return AddFileToZip(fs::path(filePath), fs::path(zipPath), isCompress, zipLevel);
}

int32_t ZipWrapper::AddFileToZip(const fs::path &fsFilePath,
                                 const fs::path &fsZipPath,
                                 const bool isCompress,
                                 const ZipLevel &zipLevel)
{
    if (!IsOpen()) {
        // LOGE("zip file is not open");
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            "zip file is not open").c_str());
        return ZIP_ERR_FAILURE;
    }
    if (!fs::is_regular_file(fsFilePath)) {
        LOGE("%s", PackingToolErrMsg::FILE_NOT_EXIST.toStringWithArgs(
            ("filePath is not regular file![filePath=" + fsFilePath.string() + "]").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }
    std::string realFilePath;
    if (!Utils::GetRealPath(fsFilePath.string(), realFilePath)) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("get real file path failed![filePath=" + fsFilePath.string() + "]").c_str()).c_str());
        return false;
    }
    std::ifstream file(realFilePath, std::ios::binary);
    if (!file.is_open()) {
        LOGE("%s", PackingToolErrMsg::FILE_IO_EXCEPTION.toStringWithArgs(
            ("open file failed![filePath=" + fsFilePath.string() + "][realFilePath=" + realFilePath + "]").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }
    zipFileInfo_.external_fa = ZIP_FILE_ATTR_DEFAULT;
    if (isCompress) {
        zipMethod_ = ZipMethod::ZIP_METHOD_DEFLATED;
    } else {
        zipMethod_ = ZipMethod::ZIP_METHOD_STORED;
    }
    int ret = zipOpenNewFileInZip64(zipFile_, fsZipPath.string().c_str(), &zipFileInfo_, nullptr, 0,
        nullptr, 0, nullptr, static_cast<int>(zipMethod_), static_cast<int>(zipLevel), 1);
    if (ret != ZIP_OK) {
        // LOGE("open file in zip failed![ret=%d][fsZipPath=%s]", ret, fsZipPath.string().c_str());
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            ("open file in zip failed![ret=" + std::to_string(ret) + "][fsZipPath=" + fsZipPath.string() + "]").c_str()).c_str());
        file.close();
        return ZIP_ERR_FAILURE;
    }
    int32_t result = ZIP_ERR_SUCCESS;
    if (fs::file_size(realFilePath) > 0) {
        result = ZIP_ERR_SUCCESS;
        char buffer[MAX_ZIP_BUFFER_SIZE] = {0};
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            if (file.fail() && !file.eof()) {
                // LOGE("read file error!");
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    "read file error!").c_str());
                break;
            }
            auto bytesRead = file.gcount();
            if (bytesRead < 0) {
                // LOGE("read file bytes error![filePath=%s][bytesRead=%u]", fsFilePath.c_str(), bytesRead);
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    ("read file bytes error![filePath=" + fsFilePath.string() + "][bytesRead=" + std::to_string(bytesRead) + "]").c_str()).c_str());
                result = ZIP_ERR_FAILURE;
                break;
            }
            if (bytesRead == 0) {
                break;
            }
            if (zipWriteInFileInZip(zipFile_, buffer, bytesRead) < 0) {
                // LOGE("write file in zip failed!");
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    "write file in zip failed!").c_str());
                result = ZIP_ERR_FAILURE;
                break;
            }
        }
    }
    zipCloseFileInZip(zipFile_);
    file.close();
    return result;
}

int32_t ZipWrapper::AddRawEntryToZip(zipFile destZip, unzFile srcZip, const std::string &entryName)
{
    if (unzLocateFile(srcZip, entryName.c_str(), 0) != UNZ_OK) {
        // LOGE("Entry [%s] not found in source zip!", entryName.c_str());
        LOGE("%s", PackingToolErrMsg::IO_EXCEPTION.toStringWithArgs(
            ("Entry [" + entryName + "] not found in source zip!").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }

    unz_file_info64 fileInfo;
    if (unzGetCurrentFileInfo64(srcZip, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK) {
        // LOGE("Get file info failed for [%s]", entryName.c_str());
        LOGE("%s", PackingToolErrMsg::IO_EXCEPTION.toStringWithArgs(
            ("Get file info failed for [" + entryName + "]").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }

    if (unzOpenCurrentFile2(srcZip, nullptr, nullptr, 1) != UNZ_OK) {
        // LOGE("Cannot open entry [%s] in source zip!", entryName.c_str());
        LOGE("%s", PackingToolErrMsg::IO_EXCEPTION.toStringWithArgs(
            ("Cannot open entry [" + entryName + "] in source zip!").c_str()).c_str());
        return ZIP_ERR_FAILURE;
    }

    zip_fileinfo zfi = {};
    int openRet = zipOpenNewFileInZip2_64(destZip, entryName.c_str(), &zfi, nullptr, 0,
        nullptr, 0, nullptr, fileInfo.compression_method, 0, 1, 1);
    if (openRet != ZIP_OK) {
        // LOGE("zipOpenNewFileInZip2_64 failed for [%s]", entryName.c_str());
        LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
            ("zipOpenNewFileInZip2_64 failed for [" + entryName + "]").c_str()).c_str());
        unzCloseCurrentFile(srcZip);
        return ZIP_ERR_FAILURE;
    }

    const size_t BUF_SIZE = 8192;
    std::vector<char> buffer(BUF_SIZE);
    int bytesRead = 0;
    do {
        bytesRead = unzReadCurrentFile(srcZip, buffer.data(), buffer.size());
        if (bytesRead < 0) {
            // LOGE("read raw data error for [%s]", entryName.c_str());
            LOGE("%s", PackingToolErrMsg::IO_EXCEPTION.toStringWithArgs(
                ("read raw data error for [" + entryName + "]").c_str()).c_str());
            break;
        }
        if (bytesRead > 0) {
            if (zipWriteInFileInZip(destZip, buffer.data(), bytesRead) < 0) {
                // LOGE("write raw data failed for [%s]", entryName.c_str());
                LOGE("%s", PackingToolErrMsg::COMPRESS_FILE_EXCEPTION.toStringWithArgs(
                    ("write raw data failed for [" + entryName + "]").c_str()).c_str());
                break;
            }
        }
    } while (bytesRead > 0);

    zipCloseFileInZipRaw64(destZip, fileInfo.uncompressed_size, fileInfo.crc);
    unzCloseCurrentFile(srcZip);
    return ZIP_ERR_SUCCESS;
}

ZipLevel ZipWrapper::StringToZipLevel(const std::string& levelStr)
{
    if (levelStr.empty()) return ZipLevel::ZIP_LEVEL_1;

    char levelChar = levelStr[0];
    if (levelChar >= '1' && levelChar <= '9') {
        return static_cast<ZipLevel>(levelChar - '0');
    }
    return ZipLevel::ZIP_LEVEL_1;
}
} // namespace AppPackingTool
} // namespace OHOS
