/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SCAN_STAT_DUPLICATE_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SCAN_STAT_DUPLICATE_H

#include "packager.h"

namespace OHOS {
namespace AppPackingTool {

struct ParamModel {
    std::vector<std::string> files;
    std::string md5;
    long long size = 0;
};

class ScanStatDuplicate {
public:
    ScanStatDuplicate();
    ~ScanStatDuplicate() = default;
    bool ScanSoFiles(const std::string &outPath);

private:
    std::string GetCurrentTime();
    std::string GetHtmlRow(const std::string& key, const long long& value);
    std::string GetHtmlRow(const std::string& key, const std::string& value);
    std::string GetHtmlRowResultClass(const std::string& key, const std::string& value);
    std::string GetHtmlRowResult(const std::string& key, const std::string& value, const std::string& trClass,
        const std::string& tdClassKey, const std::string& tdClassValue);
    std::string GetHtmlRowResult(const std::string& key, const std::vector<std::string>& value);
    std::string GetResultHtml(const std::vector<ParamModel> models);
    std::vector<ParamModel> GetDuplicateResList(const std::vector<std::string> &fileList);
    std::string StatDuplicate(const std::vector<std::string> fileList, std::string &jsonList);
    std::vector<std::string> GetAllInputFileList(const std::string &inputApp, const std::string &unZipPath);
    std::string SplitPath(const std::string &path, const std::string &packageName);
    bool WriteFile(const std::string &filePath, const std::string &data);

private:
    std::string taskDesc_;
    std::string startTime_;
    std::string stopTime_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SCAN_STAT_DUPLICATE_H