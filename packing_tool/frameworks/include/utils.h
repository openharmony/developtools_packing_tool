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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UTILS_H

#include <filesystem>
#include <list>
#include <openssl/sha.h>
#include <set>
#include <string>

namespace fs = std::filesystem;

namespace OHOS {
namespace AppPackingTool {
class Utils {
public:
    Utils() {};
    virtual ~Utils() {};

    Utils(const Utils &) = delete;
    Utils &operator=(const Utils &) = delete;

    // Container
    static std::string ListToString(const std::list<std::string>& lst);
    static void CopyListToSet(const std::list<std::string>& lst, std::set<std::string>& st);
    static bool CheckDisjoint(const std::list<std::string>& list1, const std::list<std::string>& list2);
    static bool CheckContainsAll(const std::list<std::string>& list1, const std::list<std::string>& list2);
    static bool CheckListContain(const std::list<std::string>& lst, const std::string& value);
    static long GetListDistinctCount(const std::list<std::string>& lst);

    // String
    static bool IsPositiveInteger(const std::string& str, int min = 0, int max = INT32_MAX);
    static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
    static bool EndsWith(const std::string& str, const std::string& suffix);

    // Algorithm
    static std::string GetSha256Str(const std::string &str);
    static std::string GetSha256File(const std::string &filePath);
    static std::string GetSha256Folder(const std::string &filePath);
    static std::string GenerateUUID();

    // File
    static int64_t GetFileLength(const std::string filePath);
    static std::string GetFileContent(const std::string filePath);
    static bool IsFileExists(const std::string& file);
    static bool IsFile(const std::string& file);
    static bool IsDirectory(const std::string& dir);
    static bool RemoveFile(const std::string& file);
    static bool RemoveDirectory(const std::string& dir);
    static std::string GetFilePathByDir(const std::string& dir, const std::string& fileName);
    static bool ForceCreateDirectory(const std::string& dir);
    static bool ForceRemoveDirectory(const std::string& dir, bool isDeleteSelf = true);
    static double GetCeilFileSize(long fileSize, int sizeLimit);
    static bool CheckFileName(const std::string& filePath, const std::string& fileName);
    static bool CheckFileSuffix(const std::string& filePath, const std::string& suffix);
    static bool CopyFile(const std::string& srcFile, const std::string& dstFile);
    static bool GetFormattedPath(const std::string& path, std::string& formattedPath);
    static bool GetRealPath(const std::string& path, std::string& realPath);
    static bool GetRealPathOfNoneExistFile(const std::string& path, std::string& realPath);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_UTILS_H
