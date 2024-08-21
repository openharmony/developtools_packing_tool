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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTIAPP_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTIAPP_PACKAGER_H

#include <map>
#include <string>

#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {

class MultiAppPackager : public Packager {
public:
    MultiAppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~MultiAppPackager() override {}
    bool IsVerifyValidInMultiAppMode();
    bool CompressAppModeForMultiProject();
    std::string GetJsonInZips(const std::string &filePath, const std::string &jsonName);
    std::string ReadModuleNameFromHap(const std::string &hapPath);
    std::string SelectHapInApp(const std::string &appPath, std::list<std::string> &selectedHaps,
        const std::string &tempDir, std::string &finalAppPackInfo);
    std::string DisposeApp(std::list<std::string> &selectedHaps, const std::string &tempDir);
    std::string DisposeHapAndHsp(std::list<std::string> &selectedHaps,
        const std::string &tempDir, std::string finalPackInfoStr);
    bool CopyHapAndHspFromApp(const std::string &appPath, std::list<std::string> &selectedHapsInApp,
        std::list<std::string> &selectedHaps, const std::string &tempDir);
    void WritePackInfo(const std::string &filePath, const std::string &packInfoStr);

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    ZipWrapper zipWrapper_;
    std::list<std::string> formattedAppList_;
    std::list<std::string> formattedHapAndHspList_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MULTIAPP_PACKAGER_H