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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_PACKAGER_H

#include <map>
#include <string>

#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {

class AppPackager : public Packager {
public:
    AppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~AppPackager() override {}
    
    bool CheckBundleTypeConsistency(const std::string &hapPath, const std::string &hspPath);
    bool VerifyIsSharedApp(const std::list<std::string> &hspPath);
    bool IsSharedApp(const std::string &hapPath, const std::string &hspPath);
    bool VerifyIsAppService(const std::list<std::string> &modulePathList);
    bool IsAppService(const std::string &hapPath, const std::string &hspPath);
    bool CheckInputModulePath(const std::string &hapPath, const std::string &hspPath);
    bool IsVerifyValidInAppMode();
    bool PrepareDirectoriesAndFiles(const std::string outPath);
    bool CompressHapAndHspFiles(const fs::path &tempPath, const fs::path &hspTempDirPath);
    bool AddHapListToApp(const std::list<std::string> &fileList);
    bool CompressOtherFiles();
    bool CompressAppMode();
    bool GetAndCheckOutPath(std::string &outPath);
    bool GetAndCheckHapPathAndHspPath(std::string &hapPath, std::string &hspPath);
    bool GetAndCheckPackInfoPath(std::string &packInfoPath);
    bool CheckSignaturePath();
    bool CheckCertificatePath();
    bool CheckEntrycardPath();
    bool CheckPackResPath();
    bool CheckPacJsonPath();

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    ZipWrapper zipWrapper_;
    std::list<std::string> formattedHapPathList_;
    std::list<std::string> formattedHspPathList_;
    std::list<std::string> formattedEntryCardPathList_;
    bool isSharedApp_ = false;
    bool isAppService_ = false;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_PACKAGER_H