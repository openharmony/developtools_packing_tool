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

#include "json/module_json.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <set>
#include <string>

#include "constants.h"
#include "skill_pack_helper.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {

// module.json with app bundleType and skillProfiles
const std::string MODULE_JSON_APP_WITH_SKILLS = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"app\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": false,"
        "\"skillProfiles\": ["
            "{"
                "\"name\": \"my-skill\","
                "\"abilityName\": \"EntryAbility\","
                "\"srcEntries\": [\"../../skills/my-skill/scripts/main.ets\"],"
                "\"permissions\": [\"perm1\"]"
            "}"
        "]"
    "}"
"}";

// module.json with independent skill (bundleType=skill, moduleType=skill)
const std::string MODULE_JSON_INDEPENDENT_SKILL = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"skill\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"skillModule\","
        "\"type\": \"skill\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": false,"
        "\"skillProfiles\": ["
            "{"
                "\"name\": \"my-skill\","
                "\"srcEntries\": [\"../../skills/my-skill/scripts/main.ets\"],"
                "\"permissions\": [\"perm1\"]"
            "}"
        "]"
    "}"
"}";

// module.json with bundleType=skill but moduleType != skill
const std::string MODULE_JSON_SKILL_INVALID_MODULE_TYPE = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"skill\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": false,"
        "\"skillProfiles\": ["
            "{"
                "\"name\": \"my-skill\","
                "\"srcEntries\": [\"../../skills/my-skill/scripts/main.ets\"]"
            "}"
        "]"
    "}"
"}";

// module.json without skillProfiles
const std::string MODULE_JSON_NO_SKILLS = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"app\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": false"
    "}"
"}";

// module.json with multiple skillProfiles
const std::string MODULE_JSON_MULTIPLE_SKILLS = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"app\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": false,"
        "\"skillProfiles\": ["
            "{"
                "\"name\": \"skill-one\","
                "\"abilityName\": \"EntryAbility\""
            "},"
            "{"
                "\"name\": \"skill-two\","
                "\"abilityName\": \"SecondAbility\""
            "},"
            "{"
                "\"name\": \"skill-three\""
            "}"
        "]"
    "}"
"}";

// module.json with skill bundleType and installationFree=true (should fail)
const std::string MODULE_JSON_SKILL_INSTALLATION_FREE_TRUE = "{"
    "\"app\": {"
        "\"bundleName\": \"com.example.test\","
        "\"bundleType\": \"skill\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"1.0.0\""
    "},"
    "\"module\": {"
        "\"name\": \"skillModule\","
        "\"type\": \"skill\","
        "\"deviceTypes\": [\"default\"],"
        "\"installationFree\": true"
    "}"
"}";

class SkillTest : public testing::Test {
public:
    SkillTest() {}
    virtual ~SkillTest() {}
    static void SetUpTestCase() {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

// Test: CheckStageBundleType with bundleType=skill and moduleType=skill
HWTEST_F(SkillTest, CheckStageBundleType_SkillValid, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_INDEPENDENT_SKILL));
    std::string bundleType;
    ASSERT_TRUE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_EQ(bundleType, OHOS::AppPackingTool::Constants::TYPE_SKILL);
    std::string moduleType;
    ASSERT_TRUE(moduleJson.GetStageModuleType(moduleType));
    EXPECT_EQ(moduleType, OHOS::AppPackingTool::Constants::TYPE_SKILL);
}

// Test: bundleType=skill with moduleType != skill should fail in CheckStageBundleType
HWTEST_F(SkillTest, CheckStageBundleType_SkillInvalidModuleType, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_SKILL_INVALID_MODULE_TYPE));
    std::string bundleType;
    // GetStageBundleType calls CheckStageBundleType internally, should fail
    EXPECT_FALSE(moduleJson.GetStageBundleType(bundleType));
}

// Test: GetSkillProfiles parses correctly
HWTEST_F(SkillTest, GetSkillProfiles_ValidApp, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_APP_WITH_SKILLS));
    std::list<std::map<std::string, std::string>> profiles;
    ASSERT_TRUE(moduleJson.GetSkillProfiles(profiles));
    EXPECT_EQ(profiles.size(), 1u);
    if (!profiles.empty()) {
        EXPECT_EQ(profiles.front().at("name"), "my-skill");
    }
}

// Test: GetSkillProfiles with no skillProfiles field
HWTEST_F(SkillTest, GetSkillProfiles_NoSkillProfiles, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_NO_SKILLS));
    std::list<std::map<std::string, std::string>> profiles;
    ASSERT_TRUE(moduleJson.GetSkillProfiles(profiles));
    EXPECT_TRUE(profiles.empty());
}

// Test: GetSkillProfiles parses abilityName field
HWTEST_F(SkillTest, GetSkillProfiles_WithAbilityName, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_APP_WITH_SKILLS));
    std::list<std::map<std::string, std::string>> profiles;
    ASSERT_TRUE(moduleJson.GetSkillProfiles(profiles));
    ASSERT_EQ(profiles.size(), 1u);
    auto it = profiles.begin();
    EXPECT_EQ(it->at("name"), "my-skill");
    EXPECT_EQ(it->at("abilityName"), "EntryAbility");
}

// Test: GetSkillProfiles parses independent skill without abilityName
HWTEST_F(SkillTest, GetSkillProfiles_IndependentSkill, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_INDEPENDENT_SKILL));
    std::list<std::map<std::string, std::string>> profiles;
    ASSERT_TRUE(moduleJson.GetSkillProfiles(profiles));
    ASSERT_EQ(profiles.size(), 1u);
    auto it = profiles.begin();
    EXPECT_EQ(it->at("name"), "my-skill");
    // independent skill profile should not have abilityName
    EXPECT_EQ(it->count("abilityName"), 0u);
}

// Test: GetSkillProfiles with multiple profiles
HWTEST_F(SkillTest, GetSkillProfiles_MultipleProfiles, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_MULTIPLE_SKILLS));
    std::list<std::map<std::string, std::string>> profiles;
    ASSERT_TRUE(moduleJson.GetSkillProfiles(profiles));
    ASSERT_EQ(profiles.size(), 3u);
    auto it = profiles.begin();
    EXPECT_EQ(it->at("name"), "skill-one");
    EXPECT_EQ(it->at("abilityName"), "EntryAbility");
    ++it;
    EXPECT_EQ(it->at("name"), "skill-two");
    EXPECT_EQ(it->at("abilityName"), "SecondAbility");
    ++it;
    EXPECT_EQ(it->at("name"), "skill-three");
    EXPECT_EQ(it->count("abilityName"), 0u);
}

// Test: CheckAtomicServiceInstallationFree with skill and installationFree=false (should pass)
HWTEST_F(SkillTest, CheckAtomicServiceInstallationFree_SkillFalse, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_INDEPENDENT_SKILL));
    EXPECT_TRUE(moduleJson.CheckAtomicServiceInstallationFree());
}

// Test: CheckAtomicServiceInstallationFree with skill and installationFree=true (should pass)
HWTEST_F(SkillTest, CheckAtomicServiceInstallationFree_SkillTruePasses, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_SKILL_INSTALLATION_FREE_TRUE));
    EXPECT_TRUE(moduleJson.CheckAtomicServiceInstallationFree());
}

// Test: CheckAtomicServiceInstallationFree with app and installationFree=false (should pass)
HWTEST_F(SkillTest, CheckAtomicServiceInstallationFree_AppPasses, Function | MediumTest | Level1)
{
    ModuleJson moduleJson;
    ASSERT_TRUE(moduleJson.ParseFromString(MODULE_JSON_APP_WITH_SKILLS));
    EXPECT_TRUE(moduleJson.CheckAtomicServiceInstallationFree());
}

// ============================================================================
// SkillPackHelper tests
// ============================================================================

using SkillPackHelperTest = SkillTest;

// --- IsInvalidProfileName ---

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_Empty, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName(""));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_Dot, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName("."));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_DoubleDot, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName(".."));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_PathTraversal, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName("foo/../../../etc"));
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName("foo\\..\\bar"));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_Slash, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName("a/b"));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_Backslash, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsInvalidProfileName("a\\b"));
}

HWTEST_F(SkillPackHelperTest, IsInvalidProfileName_ValidNames, Function | MediumTest | Level1)
{
    EXPECT_FALSE(SkillPackHelper::IsInvalidProfileName("my-skill"));
    EXPECT_FALSE(SkillPackHelper::IsInvalidProfileName("skill_one"));
    EXPECT_FALSE(SkillPackHelper::IsInvalidProfileName("skill.123"));
    EXPECT_FALSE(SkillPackHelper::IsInvalidProfileName("a"));
}

// --- CollectProfileNames ---

HWTEST_F(SkillPackHelperTest, CollectProfileNames_Empty, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::set<std::string> names;
    ASSERT_TRUE(SkillPackHelper::CollectProfileNames(profiles, names));
    EXPECT_TRUE(names.empty());
}

HWTEST_F(SkillPackHelperTest, CollectProfileNames_SingleProfile, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> profile;
    profile["name"] = "my-skill";
    profiles.push_back(profile);
    std::set<std::string> names;
    ASSERT_TRUE(SkillPackHelper::CollectProfileNames(profiles, names));
    EXPECT_EQ(names.size(), 1u);
    EXPECT_NE(names.find("my-skill"), names.end());
}

HWTEST_F(SkillPackHelperTest, CollectProfileNames_Deduplication, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> p1;
    p1["name"] = "same-name";
    std::map<std::string, std::string> p2;
    p2["name"] = "same-name";
    profiles.push_back(p1);
    profiles.push_back(p2);
    std::set<std::string> names;
    ASSERT_TRUE(SkillPackHelper::CollectProfileNames(profiles, names));
    EXPECT_EQ(names.size(), 1u);
}

HWTEST_F(SkillPackHelperTest, CollectProfileNames_SkipsEmptyName, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> p1;
    p1["name"] = "";
    std::map<std::string, std::string> p2;
    p2["name"] = "valid";
    profiles.push_back(p1);
    profiles.push_back(p2);
    std::set<std::string> names;
    ASSERT_TRUE(SkillPackHelper::CollectProfileNames(profiles, names));
    EXPECT_EQ(names.size(), 1u);
    EXPECT_NE(names.find("valid"), names.end());
}

HWTEST_F(SkillPackHelperTest, CollectProfileNames_NoNameKey, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> profile;
    profile["abilityName"] = "SomeAbility";
    profiles.push_back(profile);
    std::set<std::string> names;
    ASSERT_TRUE(SkillPackHelper::CollectProfileNames(profiles, names));
    EXPECT_TRUE(names.empty());
}

// --- IsForbiddenBundleType ---

HWTEST_F(SkillPackHelperTest, IsForbiddenBundleType_ForbiddenTypes, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::IsForbiddenBundleType("shared"));
    EXPECT_TRUE(SkillPackHelper::IsForbiddenBundleType("appService"));
    EXPECT_TRUE(SkillPackHelper::IsForbiddenBundleType("appPlugin"));
}

HWTEST_F(SkillPackHelperTest, IsForbiddenBundleType_AllowedTypes, Function | MediumTest | Level1)
{
    EXPECT_FALSE(SkillPackHelper::IsForbiddenBundleType("app"));
    EXPECT_FALSE(SkillPackHelper::IsForbiddenBundleType("atomicService"));
    EXPECT_FALSE(SkillPackHelper::IsForbiddenBundleType("skill"));
    EXPECT_FALSE(SkillPackHelper::IsForbiddenBundleType(""));
}

// --- CheckBundleTypeAllowsSkills ---

HWTEST_F(SkillPackHelperTest, CheckBundleTypeAllowsSkills_ForbiddenFails, Function | MediumTest | Level1)
{
    EXPECT_FALSE(SkillPackHelper::CheckBundleTypeAllowsSkills("shared"));
    EXPECT_FALSE(SkillPackHelper::CheckBundleTypeAllowsSkills("appService"));
}

HWTEST_F(SkillPackHelperTest, CheckBundleTypeAllowsSkills_AllowedPasses, Function | MediumTest | Level1)
{
    EXPECT_TRUE(SkillPackHelper::CheckBundleTypeAllowsSkills("app"));
    EXPECT_TRUE(SkillPackHelper::CheckBundleTypeAllowsSkills("skill"));
    EXPECT_TRUE(SkillPackHelper::CheckBundleTypeAllowsSkills("atomicService"));
}

// --- GetSkillsPathFromParam ---

HWTEST_F(SkillPackHelperTest, GetSkillsPathFromParam_Present, Function | MediumTest | Level1)
{
    std::map<std::string, std::string> params;
    params[Constants::PARAM_SKILLS_PATH] = "/path/to/skills";
    std::string skillsPath;
    ASSERT_TRUE(SkillPackHelper::GetSkillsPathFromParam(params, skillsPath));
    EXPECT_EQ(skillsPath, "/path/to/skills");
}

HWTEST_F(SkillPackHelperTest, GetSkillsPathFromParam_Missing, Function | MediumTest | Level1)
{
    std::map<std::string, std::string> params;
    std::string skillsPath = "unchanged";
    ASSERT_TRUE(SkillPackHelper::GetSkillsPathFromParam(params, skillsPath));
    EXPECT_TRUE(skillsPath.empty());
}

HWTEST_F(SkillPackHelperTest, GetSkillsPathFromParam_EmptyValue, Function | MediumTest | Level1)
{
    std::map<std::string, std::string> params;
    params[Constants::PARAM_SKILLS_PATH] = "";
    std::string skillsPath;
    ASSERT_TRUE(SkillPackHelper::GetSkillsPathFromParam(params, skillsPath));
    EXPECT_TRUE(skillsPath.empty());
}

// --- CompressSkillFiles ---

HWTEST_F(SkillPackHelperTest, CompressSkillFiles_EmptyProfiles_ReturnsTrue, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> params;
    bool addFileCalled = false;
    auto addFile = [&addFileCalled](const std::string &, const std::string &) {
        addFileCalled = true;
        return true;
    };
    EXPECT_TRUE(SkillPackHelper::CompressSkillFiles(profiles, params, "app", "/json/path", addFile));
    EXPECT_FALSE(addFileCalled);
}

HWTEST_F(SkillPackHelperTest, CompressSkillFiles_MissingSkillsPath_ReturnsFalse, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> profile;
    profile["name"] = "test-skill";
    profiles.push_back(profile);
    std::map<std::string, std::string> params;
    EXPECT_FALSE(SkillPackHelper::CompressSkillFiles(profiles, params, "app", "/json/path",
        [](const std::string &, const std::string &) { return true; }));
}

HWTEST_F(SkillPackHelperTest, CompressSkillFiles_ForbiddenBundleType_ReturnsFalse, Function | MediumTest | Level1)
{
    std::list<std::map<std::string, std::string>> profiles;
    std::map<std::string, std::string> profile;
    profile["name"] = "test-skill";
    profiles.push_back(profile);
    std::map<std::string, std::string> params;
    params[Constants::PARAM_SKILLS_PATH] = "/tmp/nonexistent_skills";
    EXPECT_FALSE(SkillPackHelper::CompressSkillFiles(profiles, params, "shared", "/json/path",
        [](const std::string &, const std::string &) { return true; }));
}

} // namespace
} // namespace OHOS
