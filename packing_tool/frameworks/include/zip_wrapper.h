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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_WRAPPER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_WRAPPER_H

#include <filesystem>
#include <string>

#include "zip_constants.h"

namespace OHOS {
namespace AppPackingTool {
class ZipWrapper {
public:
    ZipWrapper();
    explicit ZipWrapper(std::string zipPath);
    virtual ~ZipWrapper();

    ZipWrapper(const ZipWrapper &) = delete;
    ZipWrapper &operator=(const ZipWrapper &) = delete;

    zipFile GetZipFile() const;

    int32_t Open(std::string& zipPath, int32_t append = APPEND_STATUS_CREATE);
    int32_t Open(int32_t append = APPEND_STATUS_CREATE);
    void Close();

    void SetZipFilePath(std::string path)
    {
        zipFilePath_ = path;
    }

    void SetZipLevel(ZipLevel zipLevel)
    {
        zipLevel_ = zipLevel;
    }

    void SetZipMethod(ZipMethod zipMethod)
    {
        zipMethod_ = zipMethod;
    }

    const zip_fileinfo& ZipFileInfo() const
    {
        return zipFileInfo_;
    }

    bool IsOpen() const
    {
        return (zipFile_ != nullptr);
    }

    int32_t AddFileOrDirectoryToZip(const std::string &filePath,
                                    const std::string &zipPath,
                                    const bool isCompress = false,
                                    const ZipLevel &zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT);
    int32_t AddFileOrDirectoryToZip(const fs::path &fsFilePath,
                                    const fs::path &fsZipPath,
                                    const bool isCompress = false,
                                    const ZipLevel &zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT);

    int32_t WriteStringToZip(const std::string &content, const std::string &zipPath);
    int32_t AddRawEntryToZip(zipFile destZip, unzFile srcZip, const std::string &entryName);
    ZipLevel StringToZipLevel(const std::string& levelStr);
    
protected:
    int32_t AddEmptyDirToZip(const std::string &zipPath);
    int32_t AddEmptyDirToZip(const fs::path &fsZipPath);
    int32_t AddFileToZip(const std::string &filePath,
                         const std::string &zipPath,
                         const bool isCompress = false,
                         const ZipLevel &zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT);
    int32_t AddFileToZip(const fs::path &fsFilePath,
                         const fs::path &fsZipPath,
                         const bool isCompress = false,
                         const ZipLevel &zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT);

private:
    zipFile zipFile_ = nullptr;
    zip_fileinfo zipFileInfo_ = {};
    std::string zipFilePath_;
    ZipLevel zipLevel_ = ZipLevel::ZIP_LEVEL_DEFAULT;
    ZipMethod zipMethod_ = ZipMethod::ZIP_METHOD_STORED;
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_WRAPPER_H
