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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_H

#include "general_normalize_version.h"
#include "json/module_json.h"
#include "packager.h"
#include "unzip_wrapper.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {

class GeneralNormalize : public Packager {
public:
    GeneralNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~GeneralNormalize() override {}

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    bool ModifyModuleJson(const std::string &moduleJsonPath, GeneralNormalizeVersion &generalNormalizeVersion,
        std::string &bundleName, std::string &moduleName);
    bool ModifyConfigJson(const std::string &moduleJsonPath, GeneralNormalizeVersion &generalNormalizeVersion,
        std::string &bundleName, std::string &moduleName);
    bool ModifyPackInfo(const std::string &packInfoPath);
    bool ProcessJsonFiles(const std::string &tempPath, std::list<GeneralNormalizeVersion> &generalNormalizeVersions,
        std::string &bundleName, std::string &moduleName);
    bool CompressDirToHap(const std::string &tempDir, const std::string &modifiedHapPath);
    ZipWrapper zipWrapper_;
    UnzipWrapper unzipWrapper_;
    std::list<std::string> hspOrhapList_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_GENERAL_NORMALIZE_H