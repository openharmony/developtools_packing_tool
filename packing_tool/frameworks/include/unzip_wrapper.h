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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UNZIP_WRAPPER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UNZIP_WRAPPER_H

#include <filesystem>

#include "zip_constants.h"

namespace OHOS {
namespace AppPackingTool {
class UnzipWrapper {
public:
    UnzipWrapper();
    explicit UnzipWrapper(std::string path);
    virtual ~UnzipWrapper();

    UnzipWrapper(const UnzipWrapper &) = delete;
    UnzipWrapper &operator=(const UnzipWrapper &) = delete;

    int32_t Open(std::string& unzPath);
    int32_t Open();
    void Close();
    int32_t UnzipFile(std::string filePath);

    bool IsOpen() const
    {
        return (unzFile_ != nullptr);
    }

protected:
    std::string ExtractFile(const std::string filePath);

private:
    unzFile unzFile_ = nullptr;
    unz_global_info64 unzGlobalInfo_ = { 0, 0 };
    std::string unzFilePath_;
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UNZIP_WRAPPER_H
