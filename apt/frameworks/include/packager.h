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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGER_H

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "constants.h"
#include "contrib/minizip/zip.h"
#include "contrib/minizip/unzip.h"
#include "errors.h"
#include "nlohmann/json.hpp"

namespace fs = std::filesystem;

namespace OHOS {
namespace AppPackingTool {

struct Parameter {
    std::string paramName;
    bool required;
    Constants::PARAM_TYPE paramType;
    std::string paramRegrex;
    std::string zipEntry;
};

class Packager {
public:
    Packager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    virtual ~Packager();

    std::string MakePackage();

    virtual ErrCode InitAllowedParam() = 0;
    virtual ErrCode PreProcess() = 0;
    virtual ErrCode Process() = 0;
    virtual ErrCode PostProcess() = 0;

protected:
    const std::map<std::string, std::string> &parameterMap_;
    std::string &resultReceiver_;
    std::vector<Parameter> allowedParameters_;
    nlohmann::json moduleJson;
    nlohmann::json packInfo;
    
    void AddFileToZip(zipFile zf, const fs::path &filePath, const fs::path &zipPath, zip_fileinfo &zipfi);
    void WriteStringToZip(zipFile zf, const std::string &content, const fs::path &zipPath, zip_fileinfo &zipfi);
    bool ParseJsonFile(nlohmann::json &jsonObject, std::string filePath);
    bool CheckFileValid(const std::string &filePath, const std::string &filename);
    bool endWith(const std::string &str, const std::string &suffix);
};

}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGER_H