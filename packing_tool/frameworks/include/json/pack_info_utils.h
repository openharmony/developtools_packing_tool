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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACK_INFO_UTILS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_PACK_INFO_UTILS_H

#include <map>
#include <string>

#include "pack_info.h"
#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {

class PackInfoUtils {
public:
    PackInfoUtils() {};
    virtual ~PackInfoUtils() {};

    PackInfoUtils(const PackInfoUtils &) = delete;
    PackInfoUtils &operator=(const PackInfoUtils &) = delete;

    static bool MergeTwoPackInfos(const std::string& srcPackInfoJsonStr1, const std::string& srcPackInfoJsonStr2,
        std::string& dstPackInfoJsonStr);
    static bool MergeTwoPackInfosByPackagePair(const std::string& srcPackInfoJsonStr1,
        const std::string& srcPackInfoJsonStr2, const std::map<std::string, std::string>& packagesMap,
        std::string& dstPackInfoJsonStr);

private:
    static bool MergeTwoPackInfos(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2);
    static bool MergeTwoPackInfosByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
        const std::string& packageName, const std::string& moduleName);
    static bool FindAndMergeModulesByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
        const std::string& moduleName);
    static bool FindAndMergePackagesByPackagePair(PackInfo& srcPackInfo1, PackInfo& srcPackInfo2,
        const std::string& packageName);
    static bool VerifyPackInfos(const PackInfo& finalPackInfo, const PackInfo& srcPackInfo);
    static bool CheckBundleNameInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2);
    static bool CheckBundleTypeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2);
    static bool CheckVersionCodeInPackInfo(const PackInfo& packInfo1, const PackInfo& packInfo2);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_MODULE_JSON_UTILS_H
