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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HSP_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HSP_PACKAGER_H

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "json/module_json.h"
#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {
class HspPackager : public Packager {
public:
    HspPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~HspPackager() override {}
    bool IsVerifyValidInHspCommonMode();
    bool Compatible(const std::string &paramPath, std::list<std::string> &fileList, const std::string &suffix);
    bool IsVerifyValidInHspMode();
    bool IsHspPathValid();
    bool IsHspPathValid(const std::string &parameterMapKey);
    bool CompressHsp();
    bool CompressHspMode(const std::string &jsonPath);
    bool CompressHspModePartSecond(const std::string &jsonPath);
    bool CompressHspModePartThird(const std::string &jsonPath);
    bool CompressHspModePartFourth();
    bool CompressHspModeMultiple();
    bool AddCommonFileOrDirectoryToZip(const std::string &paramPath, const std::string &targetPath);
    bool CheckAppPlugin();
    bool IsPluginHost();
    bool IsPermissionSupportPlugin(std::unique_ptr<PtJson>& requestPermissionsObj);
    bool CheckPkgContext();
    bool IsExtensionAbility(std::unique_ptr<PtJson>& extensionAbilitiesObj);

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    std::list<std::string> formatedDirList_;
    std::list<std::string> formattedJarPathList_;
    std::list<std::string> formattedTxtPathList_;
    std::list<std::string> formattedSoPathList_;
    
    std::string moduleName_;
    std::string jsonPath_;
    std::list<std::string> deviceTypes_;
    ZipWrapper zipWrapper_;
    ModuleJson moduleJson_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HSP_PACKAGER_H