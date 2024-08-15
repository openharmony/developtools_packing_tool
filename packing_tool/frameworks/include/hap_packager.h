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
#include <map>
#include <string>

#include "packager.h"
#include "zip_wrapper.h"
namespace OHOS {
namespace AppPackingTool {

class HapPackager : public Packager {
public:
    HapPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~HapPackager() override {}

protected:
    int InitAllowedParam() override;
    int PreProcess() override;
    int Process() override;
    int PostProcess() override;

private:
    ZipWrapper zipWrapper_;
};

}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_HAP_PACKAGER_H