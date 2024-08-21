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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGE_NORMALIZE_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGE_NORMALIZE_H
#include "packager.h"
#include "unzip_wrapper.h"
#include "zip_wrapper.h"
#include "json/module_json.h"
#include "json/normalize_version.h"
namespace OHOS {
namespace AppPackingTool {
class PackageNormalize : public Packager {
public:
    PackageNormalize(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~PackageNormalize() override {}
protected:
    int InitAllowedParam() override;
    int PreProcess() override;
    int Process() override;
    int PostProcess() override;
private:
    bool ModifyModuleJson(const std::string &moduleJsonPath,
        const int newVersionCode, const std::string newBundleName);
    bool ModifyPackInfo(const std::string &moduleJsonPath,
        const int newVersionCode, const std::string newBundleName);
    ZipWrapper zipWrapper_;
    UnzipWrapper unzipWrapper_;
    std::list<std::string> hspList_;
};

}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACKAGE_NORMALIZE_H