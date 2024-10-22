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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_RES_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_RES_PACKAGER_H

#include <list>
#include <map>
#include <string>

#include "json/pack_info.h"
#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {
class ResPackager : public Packager {
public:
    ResPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~ResPackager() override {}
    bool IsVerifyValidInResMode();
    bool CompressPackResMode();
protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;
private:
    std::string packInfoPath_;
    std::string entryCardPath_;
    std::string outPath_;
    std::list<std::string> formattedEntryCardPathList_;
    PackInfo packInfo_;
    ZipWrapper zipWrapper_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_RES_PACKAGER_H