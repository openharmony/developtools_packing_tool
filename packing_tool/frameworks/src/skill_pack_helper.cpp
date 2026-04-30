/*
 * Copyright (c) 2024-2026 Huawei Device Co., Ltd.
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

#include "skill_pack_helper.h"

#include "log.h"

namespace OHOS {
namespace AppPackingTool {
namespace SkillPackHelper {
namespace {
std::string BuildSkillZipBase(const std::string &profileName)
{
    return Constants::SKILLS_DIR + Constants::LINUX_FILE_SEPARATOR + profileName +
        Constants::LINUX_FILE_SEPARATOR;
}

void NormalizeZipPath(std::string &zipPath)
{
#ifdef _WIN32
    for (char &ch : zipPath) {
        if (ch == '\\') {
            ch = '/';
        }
    }
#else
    (void)zipPath;
#endif
}

bool AddRegularSkillFile(const fs::path &filePath, const fs::path &skillDir, const std::string &zipBase,
    const AddSkillFileFunc &addFile)
{
    std::error_code ec;
    std::string relativePath = fs::relative(filePath, skillDir, ec).string();
    if (ec) {
        return false;
    }
    std::string zipPath = zipBase + relativePath;
    NormalizeZipPath(zipPath);
    return addFile(filePath.string(), zipPath);
}

bool AddSkillChildDirectoryToZip(const fs::path &childPath, const fs::path &skillDir, const std::string &zipBase,
    const AddSkillFileFunc &addFile)
{
    std::error_code ec;
    fs::recursive_directory_iterator end;
    for (fs::recursive_directory_iterator it(childPath, ec); !ec && it != end; it.increment(ec)) {
        const fs::directory_entry &contentEntry = *it;
        if (!contentEntry.is_regular_file(ec) || ec) {
            continue;
        }
        if (!AddRegularSkillFile(contentEntry.path(), skillDir, zipBase, addFile)) {
            return false;
        }
    }
    return !ec;
}

bool AddSkillEntryToZip(const fs::directory_entry &child, const fs::path &skillDir, const std::string &zipBase,
    const AddSkillFileFunc &addFile)
{
    std::error_code ec;
    const fs::path childPath = child.path();
    if (child.is_directory(ec) && !ec) {
        if (childPath.filename().string() == Constants::SCRIPTS_DIR) {
            return true;
        }
        return AddSkillChildDirectoryToZip(childPath, skillDir, zipBase, addFile);
    }
    if (child.is_regular_file(ec) && !ec) {
        return AddRegularSkillFile(childPath, skillDir, zipBase, addFile);
    }
    return !ec;
}
} // namespace

bool IsInvalidProfileName(const std::string &profileName)
{
    return profileName.empty() || profileName == "." || profileName == ".." ||
        profileName.find("..") != std::string::npos ||
        profileName.find('/') != std::string::npos ||
        profileName.find('\\') != std::string::npos;
}

bool CollectProfileNames(const std::list<std::map<std::string, std::string>> &skillProfiles,
    std::set<std::string> &profileNames)
{
    for (const auto &profile : skillProfiles) {
        auto nameIt = profile.find("name");
        if (nameIt != profile.end() && !nameIt->second.empty()) {
            profileNames.insert(nameIt->second);
        }
    }
    return true;
}

bool IsForbiddenBundleType(const std::string &bundleType)
{
    return bundleType == Constants::TYPE_SHARED || bundleType == Constants::BUNDLE_TYPE_APP_SERVICE ||
        bundleType == Constants::TYPE_APP_PLUGIN;
}

bool GetSkillsPathFromParam(const std::map<std::string, std::string> &parameterMap, std::string &skillsPath)
{
    skillsPath.clear();
    const auto skillsPathIt = parameterMap.find(Constants::PARAM_SKILLS_PATH);
    if (skillsPathIt != parameterMap.end() && !skillsPathIt->second.empty()) {
        skillsPath = skillsPathIt->second;
    }
    return true;
}

bool ValidateSkillsPath(const std::map<std::string, std::string> &parameterMap, std::string &skillsPath)
{
    if (!GetSkillsPathFromParam(parameterMap, skillsPath)) {
        return false;
    }
    if (skillsPath.empty()) {
        LOGE("--skills-path is required when skillProfiles is configured in module.json.");
        return false;
    }
    if (!fs::exists(skillsPath) || !fs::is_directory(skillsPath)) {
        LOGE("skills-path does not exist or is not a directory.");
        return false;
    }
    return true;
}

bool CheckBundleTypeAllowsSkills(const std::string &bundleType)
{
    if (IsForbiddenBundleType(bundleType)) {
        LOGE("skillProfiles is not allowed when bundleType is %s.", bundleType.c_str());
        return false;
    }
    return true;
}

bool CompressSkillFiles(const std::list<std::map<std::string, std::string>> &skillProfiles,
    const std::map<std::string, std::string> &parameterMap, const std::string &bundleType,
    const std::string &jsonPath, const AddSkillFileFunc &addFile)
{
    std::set<std::string> profileNames;
    if (!CollectProfileNames(skillProfiles, profileNames)) {
        return false;
    }
    if (profileNames.empty()) {
        return true;
    }
    std::string skillsPath;
    if (!ValidateSkillsPath(parameterMap, skillsPath)) {
        return false;
    }
    if (!CheckBundleTypeAllowsSkills(bundleType)) {
        return false;
    }
    std::string failedProfile;
    std::string failureDetail;
    if (!ValidateSkillProfiles(skillsPath, profileNames, failedProfile, failureDetail)) {
        if (failureDetail.find("skillProfiles has '") == 0) {
            failureDetail += " Checked skillProfiles declared in --json-path: " + jsonPath + ".";
        }
        LOGE("%s", failureDetail.c_str());
        return false;
    }
    if (!AddConfiguredSkillsToZip(skillsPath, profileNames, addFile, failedProfile)) {
        LOGE("Failed to add skill files to zip for profile: %s", failedProfile.c_str());
        return false;
    }
    return true;
}

bool HasSkillMarkdown(const fs::path &skillDir)
{
    std::error_code ec;
    fs::directory_iterator end;
    for (fs::directory_iterator it(skillDir, ec); !ec && it != end; it.increment(ec)) {
        const fs::directory_entry &entry = *it;
        if (entry.is_regular_file(ec) && !ec &&
            entry.path().filename().string() == Constants::SKILL_MD) {
            return true;
        }
    }
    return false;
}

bool ValidateSkillProfiles(const std::string &skillsPath, const std::set<std::string> &profileNames,
    std::string &failedProfile, std::string &failureDetail)
{
    for (const std::string &profileName : profileNames) {
        if (IsInvalidProfileName(profileName)) {
            failedProfile = profileName;
            failureDetail = "skillProfiles name '" + profileName + "' contains invalid path characters.";
            return false;
        }
        const fs::path skillDir = fs::path(skillsPath) / profileName;
        if (!fs::exists(skillDir) || !fs::is_directory(skillDir)) {
            failedProfile = profileName;
            failureDetail = "skillProfiles has '" + profileName +
                "' but no matching skills directory found under --skills-path.";
            return false;
        }
        if (!HasSkillMarkdown(skillDir)) {
            failedProfile = profileName;
            failureDetail = "skills/" + profileName + "/ must contain SKILL.md file (case-sensitive).";
            return false;
        }
    }
    return true;
}

bool AddSkillDirectoryToZip(const fs::path &skillDir, const std::string &profileName, const AddSkillFileFunc &addFile)
{
    const std::string zipBase = BuildSkillZipBase(profileName);
    std::error_code ec;
    fs::directory_iterator end;
    for (fs::directory_iterator it(skillDir, ec); !ec && it != end; it.increment(ec)) {
        if (!AddSkillEntryToZip(*it, skillDir, zipBase, addFile)) {
            return false;
        }
    }
    return !ec;
}

bool AddConfiguredSkillsToZip(const std::string &skillsPath, const std::set<std::string> &profileNames,
    const AddSkillFileFunc &addFile, std::string &failedProfile)
{
    for (const std::string &profileName : profileNames) {
        const fs::path skillDir = fs::path(skillsPath) / profileName;
        if (!AddSkillDirectoryToZip(skillDir, profileName, addFile)) {
            failedProfile = profileName;
            return false;
        }
    }
    return true;
}
} // namespace SkillPackHelper
} // namespace AppPackingTool
} // namespace OHOS
