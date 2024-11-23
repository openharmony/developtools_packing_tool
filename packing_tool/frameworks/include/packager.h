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
#include <list>
#include <map>
#include <regex>
#include <string>
#include <vector>

#include "constants.h"
#include "json/module_json.h"

namespace fs = std::filesystem;

namespace OHOS {
namespace AppPackingTool {

enum ErrCode {
    ERR_OK = 0,
    ERR_INVALID_VALUE,
};

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

    int32_t MakePackage();

    virtual int32_t InitAllowedParam() = 0;
    virtual int32_t PreProcess() = 0;
    virtual int32_t Process() = 0;
    virtual int32_t PostProcess() = 0;

protected:
    const std::map<std::string, std::string> &parameterMap_;
    std::string &resultReceiver_;
    std::vector<Parameter> allowedParameters_;
    bool buildHashFinish_ = false;
    bool generateBuildHash_ = false;
    
    bool CheckForceFlag();
    bool IsPathValid(const std::string &path, const bool &isFile, const std::string suffix = "");
    bool SplitDirList(const std::string &dirList, std::list<std::string> &fileList);
    void RemoveDuplicatePath(const std::string &path, std::list<std::string> &pathList);
    bool CompatibleProcess(const std::string &inputPath, std::list<std::string> &fileList, const std::string &suffix);
    bool CompatibleProcess(const std::string &inputPath, std::list<std::string> &fileList,
        const std::string &suffix, const std::string &extraSuffix);
    bool IsOutPathValid(const std::string &outPath, const std::string &forceRewrite, const std::string &suffix);
    bool SetGenerateBuildHash(std::string &jsonPath, bool &generateBuildHash, bool &buildHashFinish);
    bool CopyFileToTempDir(std::string &jsonPath);
    bool BuildHash(bool &buildHashFinish, const bool &generateBuildHash,
        const std::map<std::string, std::string> &parameterMap, const std::string &jsonPath);
    bool PutBuildHash(const std::string &jsonPath, const std::string &hash, bool &buildHashFinish);
    bool IsOutDirectoryValid();
    bool IsModuleHap(const std::string& hapPath);
    void CompressPackinfoIntoHap(const std::string& hapPathItem, const std::string& unzipPathString,
        const std::string& outPathString, const std::string& packInfoPath);
};

}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGER_H