/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SKILL_PACK_HELPER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SKILL_PACK_HELPER_H

#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <string>

#include "constants.h"

namespace OHOS {
namespace AppPackingTool {
namespace SkillPackHelper {
namespace fs = std::filesystem;

using AddSkillFileFunc = std::function<bool(const std::string &, const std::string &)>;

bool IsInvalidProfileName(const std::string &profileName);

bool CollectProfileNames(const std::list<std::map<std::string, std::string>> &skillProfiles,
    std::set<std::string> &profileNames);

bool IsForbiddenBundleType(const std::string &bundleType);

bool GetSkillsPathFromParam(const std::map<std::string, std::string> &parameterMap, std::string &skillsPath);

bool ValidateSkillsPath(const std::map<std::string, std::string> &parameterMap, std::string &skillsPath);

bool CheckBundleTypeAllowsSkills(const std::string &bundleType);

bool CompressSkillFiles(const std::list<std::map<std::string, std::string>> &skillProfiles,
    const std::map<std::string, std::string> &parameterMap, const std::string &bundleType,
    const std::string &jsonPath, const AddSkillFileFunc &addFile);

bool HasSkillMarkdown(const fs::path &skillDir);

bool ValidateSkillProfiles(const std::string &skillsPath, const std::set<std::string> &profileNames,
    std::string &failedProfile, std::string &failureDetail);

bool AddSkillDirectoryToZip(const fs::path &skillDir, const std::string &profileName, const AddSkillFileFunc &addFile);

bool AddConfiguredSkillsToZip(const std::string &skillsPath, const std::set<std::string> &profileNames,
    const AddSkillFileFunc &addFile, std::string &failedProfile);
} // namespace SkillPackHelper
} // namespace AppPackingTool
} // namespace OHOS

#endif // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SKILL_PACK_HELPER_H
