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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_UTILS_H

#include "unzip_wrapper.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {
class ZipUtils {
public:
    ZipUtils();
    virtual ~ZipUtils();

    ZipUtils(const ZipUtils &) = delete;
    ZipUtils &operator=(const ZipUtils &) = delete;

    static int32_t Zip(const std::string& filePath, const std::string& zipFilePath,
        const std::string& zipPath = "", const ZipLevel& zipLevel = ZipLevel::ZIP_LEVEL_DEFAULT,
        const int32_t& append = APPEND_STATUS_CREATE);

    static int32_t Unzip(const std::string& zipPath, const std::string& filePath);

    static bool IsFileExistsInZip(const std::string& zipFilePath, const std::string& filename);
    static bool IsFileNameExistsInZip(const std::string& zipFilePath, const std::string& filename);

    static bool GetFileContentFromZip(const std::string& zipFilePath, const std::string& filename,
        std::string& fileContent);

    static bool GetResourceMapFromZip(const std::string& zipFilePath, std::map<std::string, std::string>& resourceMap);

private:
    static bool GetUnzipCurrentFileContent(unzFile& unzipFile, std::string& fileContent);
    static bool AddToResourceMap(unzFile& unzipFile, const std::string& filePathInZip, std::map<std::string,
        std::string>& resourceMap);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_UTILS_H
