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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_PACKAGER_H

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "json/module_json.h"
#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {
class HapPackager : public Packager {
public:
    HapPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~HapPackager() override {}
    bool IsVerifyValidInHapCommonMode();
    bool Compatible(const std::string &paramPath, std::list<std::string> &fileList, const std::string &suffix);
    bool IsVerifyValidInHapMode();
    bool IsValidRpcid();
    bool IsValidPackInfo();
    bool IsHapPathValid();
    bool IsHapPathValid(const std::string &parameterMapKey);
    bool CompressHap();
    bool CheckStageHap(const std::string &jsonPath);
    bool AddFileListToZip(const std::list<std::string> &pathList_);
    bool OpenZipWrapper();
    bool AddCommonFileOrDirectoryToZip(const std::string &paramPath, const std::string &targetPath);
    bool AddParamFileToZip();
    bool AddResFileAndDirLsitToZip();
    bool AddIndexToZipForFaMaode();
    bool AddPkgAndBinFileToZipForStageMaode();
    bool CompressHapModeForModule(const std::string &jsonPath);
    bool CompressHapMode();
    bool CompressHapModeMultiple();

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    std::list<std::string> formattedSoPathList_;
    std::list<std::string> formatedDirList_;
    std::list<std::string> formattedAbcPathList_;
    std::list<std::string> formattedAbilitySoPathList_;
    std::list<std::string> formattedJarPathList_;
    std::list<std::string> formattedTxtPathList_;

    std::string moduleName_;
    std::string jsonPath_;
    std::list<std::string> deviceTypes_;
    ZipWrapper zipWrapper_;
    ModuleJson moduleJson_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_PACKAGER_H