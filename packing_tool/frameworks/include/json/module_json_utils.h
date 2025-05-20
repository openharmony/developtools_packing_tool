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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_UTILS_H

#include <string>

#include "hap_verify_info.h"

namespace OHOS {
namespace AppPackingTool {
class ModuleJsonUtils {
public:
    ModuleJsonUtils() {};
    virtual ~ModuleJsonUtils() {};

    ModuleJsonUtils(const ModuleJsonUtils &) = delete;
    ModuleJsonUtils &operator=(const ModuleJsonUtils &) = delete;

    static bool GetStageHapVerifyInfo(const std::string& hapFilePath, HapVerifyInfo& hapVerifyInfo);
    static bool GetFaHapVerifyInfo(const std::string& hapFilePath, HapVerifyInfo& hapVerifyInfo);
    static bool CheckHapsIsValid(const std::list<std::string>& fileList, const bool& isSharedApp);
    static bool IsModuleHap(const std::string hapFilePath);
    static bool GetHapVerifyInfosMapfromFileList(const std::list<std::string>& fileList,
        std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap);
    static bool CheckAppAtomicServiceCompressedSizeValid(std::map<std::string,
        std::string> parameterMap, std::map<std::string, std::shared_ptr<HapVerifyInfo>>& hapVerifyInfoMap);

private:
    static bool CheckSharedAppIsValid(const std::list<HapVerifyInfo>& hapVerifyInfos, bool& isOverlay);
    static void setAtomicServiceFileSizeLimit(std::list<HapVerifyInfo>& hapVerifyInfos);
    static bool GetHapVerifyInfosfromFileList(const std::list<std::string>& fileList,
        std::list<HapVerifyInfo>& hapVerifyInfos);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_UTILS_H
