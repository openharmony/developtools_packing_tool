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

#include "fast_app_packager.h"

#include <fstream>
#include <string>
#include <sstream>

#include "constants.h"
#include "json/module_json.h"
#include "json/pack_info.h"
#include "json/module_json_utils.h"
#include "log.h"
#include "skill_pack_helper.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
bool ValidateSkillProfilesInArchive(const fs::path &archivePath, const std::set<std::string> &profileNames,
    std::string &failureDetail)
{
    for (const auto &profileName : profileNames) {
        if (profileName.find('/') != std::string::npos || profileName.find('\\') != std::string::npos ||
            profileName == ".." || profileName == ".") {
            failureDetail = "skillProfiles name '" + profileName + "' contains invalid path characters.";
            return false;
        }
        const std::string skillDirPrefix = Constants::SKILLS_DIR + "/" + profileName + "/";
        if (!ZipUtils::IsPathPrefixExistsInZip(archivePath.string(), skillDirPrefix)) {
            failureDetail = "skillProfiles has '" + profileName + "' but no matching skills directory found.";
            return false;
        }
        const std::string skillMarkdown = skillDirPrefix + Constants::SKILL_MD;
        if (!ZipUtils::IsFileNameExistsInZip(archivePath.string(), skillMarkdown)) {
            failureDetail = "skills/" + profileName + "/ must contain SKILL.md file (case-sensitive).";
            return false;
        }
    }
    return true;
}

bool ValidateAppPackSkillProfiles(const fs::path &path, ModuleJson &moduleJson)
{
    std::list<std::map<std::string, std::string>> skillProfiles;
    if (!moduleJson.GetSkillProfiles(skillProfiles) || skillProfiles.empty()) {
        return true;
    }
    std::string bundleType;
    if (moduleJson.GetStageBundleType(bundleType) && SkillPackHelper::IsForbiddenBundleType(bundleType)) {
        LOGE("bundleType '%s' does not support skills in app packing mode.", bundleType.c_str());
        return false;
    }
    std::set<std::string> profileNames;
    SkillPackHelper::CollectProfileNames(skillProfiles, profileNames);
    std::string failedProfile;
    std::string failureDetail;
    if (fs::is_directory(path)) {
        const std::string skillsPath = (path / Constants::SKILLS_DIR).string();
        if (!fs::exists(skillsPath) || !fs::is_directory(skillsPath)) {
            LOGE("skillProfiles is configured but skills/ directory not found in '%s'.", path.string().c_str());
            return false;
        }
        if (!SkillPackHelper::ValidateSkillProfiles(skillsPath, profileNames, failedProfile, failureDetail)) {
            LOGE("%s", failureDetail.c_str());
            return false;
        }
        return true;
    }
    if (!ValidateSkillProfilesInArchive(path, profileNames, failureDetail)) {
        LOGE("%s", failureDetail.c_str());
        return false;
    }
    return true;
}

bool AddFastAppSkillsToZip(const fs::path &skillsRoot, const std::set<std::string> &skillProfileNames,
    ZipWrapper &zipWrapper)
{
    std::string failedProfile;
    return SkillPackHelper::AddConfiguredSkillsToZip(skillsRoot.string(), skillProfileNames,
        [&zipWrapper](const std::string &sourcePath, const std::string &zipPath) {
            return zipWrapper.AddFileOrDirectoryToZip(sourcePath, zipPath) == ZipErrCode::ZIP_ERR_SUCCESS;
        }, failedProfile);
}

bool ParseModuleJsonContent(const std::string &content, ModuleJson &moduleJson)
{
    return !content.empty() && moduleJson.ParseFromString(content);
}
}

FastAppPackager::FastAppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

int32_t FastAppPackager::InitAllowedParam()
{
    allowedParameters_ = {
        {}
    };
    return ERR_OK;
}

int32_t FastAppPackager::PreProcess()
{
    if (!CheckForceFlag()) {
        return ERR_INVALID_VALUE;
    }
    if (!CheckStatDuplicateFlag()) {
        return ERR_INVALID_VALUE;
    }
    bool ret = IsVerifyValidInFastAppMode();
    if (!ret) {
        return ERR_INVALID_VALUE;
    }
    if (!Packager::ParseAtomicServiceSizeLimit()) {
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t FastAppPackager::Process()
{
    bool ret = CompressFastAppMode();
    if (!ret) {
        std::string outPath;
        if (parameterMap_.find(Constants::PARAM_OUT_PATH) != parameterMap_.end()) {
            outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
        }
        if (fs::exists(outPath)) {
            fs::remove_all(outPath);
        }
        LOGE("Fast App process failed.");
        return ERR_INVALID_VALUE;
    }
    return ERR_OK;
}

int32_t FastAppPackager::PostProcess()
{
    return ERR_OK;
}

bool FastAppPackager::IsVerifyValidInFastAppMode()
{
    if (!IsVerifyValid()) {
        return false;
    }

    if (!FormatPath()) {
        return false;
    }

    if (!ModuleJsonAndPackInfoExists(formattedHapPathList_, formattedHspPathList_)) {
        LOGE("hap-path or hsp-path is invalid.");
        return false;
    }
    
    if (!CheckBundleTypeConsistency(formattedHapPathList_, formattedHspPathList_)) {
        LOGE("bundleType is inconsistent.");
        return false;
    }

    if (!CheckSkillRules(formattedHapPathList_, formattedHspPathList_)) {
        return false;
    }
    
    std::string packInfoPath;
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        packInfoPath = it->second;
    }
    if (!IsPackInfoValid(fs::path(packInfoPath), formattedHapPathList_, formattedHspPathList_)) {
        LOGE("pack.info is invalid.");
        return false;
    }
    
    std::string outPath = "";
    std::string forceRewrite = "";
    it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it != parameterMap_.end()) {
        outPath = it->second;
    }

    it = parameterMap_.find(Constants::PARAM_FORCE);
    if (it != parameterMap_.end()) {
        forceRewrite = it->second;
    }

    return IsOutPathValid(outPath, forceRewrite, Constants::APP_SUFFIX);
}

bool FastAppPackager::FormatPath()
{
    std::map<std::string, std::string>::const_iterator itHap = parameterMap_.find(Constants::PARAM_HAP_PATH);
    if (itHap != parameterMap_.end() && !itHap->second.empty() &&
        (!IsFormatPathValid(itHap->second, formattedHapPathList_) || !IsHapPathValid(formattedHapPathList_))) {
        LOGE("Fast App packager hap-path is invalid.");
        return false;
    }

    std::map<std::string, std::string>::const_iterator itHsp = parameterMap_.find(Constants::PARAM_HSP_PATH);
    if (itHsp != parameterMap_.end() && !itHsp->second.empty() && (!IsFormatPathValid(itHsp->second,
        formattedHspPathList_) || !IsHspPathValid(formattedHspPathList_, Constants::HSP_SUFFIX))) {
        LOGE("Fast App packager hsp-path is invalid.");
        return false;
    }

    if ((itHap == parameterMap_.end() || itHap->second.empty()) &&
        (itHsp == parameterMap_.end() || itHsp->second.empty())) {
        LOGE("Fast App packager hap-path and hsp-path are empty.");
        return false;
    }
    return true;
}

bool FastAppPackager::IsVerifyValid()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        LOGE("Fast App packager pack-info-path is empty.");
        return false;
    }

    packInfoPath_ = it->second;
    if (!IsPathValid(packInfoPath_, true, Constants::PACK_INFO)) {
        LOGE("Fast App packager pack-info-path is invalid.");
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_SIGNATURE_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && !IsPathValid(it->second, true)) {
        LOGE("Fast App packager signature-path is invalid.");
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_CERTIFICATE_PATH);
    if (it != parameterMap_.end() && !it->second.empty() && !IsPathValid(it->second, true)) {
        LOGE("Fast App packager certificate-path is invalid.");
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_PACK_RES_PATH);
    if (it != parameterMap_.end() && !it->second.empty() &&
        !IsPathValid(it->second, true, Constants::FILE_PACK_RES)) {
        LOGE("Fast App packager pack-res-path is invalid.");
        return false;
    }
    
    it = parameterMap_.find(Constants::PARAM_ENTRYCARD_PATH);
    if (it != parameterMap_.end() && !it->second.empty() &&
        !CompatibleProcess(it->second, formattedEntryCardPathList_, Constants::PNG_SUFFIX)) {
        LOGE("Fast App packager entrycard-path is invalid.");
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_PAC_JSON_PATH);
    if (it != parameterMap_.end() && !it->second.empty() &&
        !IsFileMatch(it->second, Constants::PAC_JSON)) {
        LOGE("Fast App packager pac-json-path is invalid.");
        return false;
    }

    it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it == parameterMap_.end() || it->second.empty()) {
        LOGE("Fast App packager out-path is empty.");
        return false;
    }
    return true;
}

bool FastAppPackager::IsFormatPathValid(const std::string &inputPath, std::list<std::string> &formattedPathList)
{
    std::set<std::string> formatPathSet;
    std::istringstream iss(inputPath);
    std::string path;

    while (std::getline(iss, path, Constants::COMMA_SPLIT)) {
        try {
            fs::path realpath = fs::canonical(path);
            if (fs::exists(realpath)) {
                formatPathSet.insert(realpath.string());
            } else {
                LOGE("Fast App packager path does not exist: %s", realpath.string().c_str());
                return false;
            }
        } catch (const std::exception& ex) {
            LOGE("Fast App packager format path failed: %s", ex.what());
            return false;
        }
    }

    formattedPathList.assign(formatPathSet.begin(), formatPathSet.end());
    return true;
}

bool FastAppPackager::IsHapPathValid(const std::list<std::string> &formatPathList)
{
    for (const auto& path : formatPathList) {
        if (!fs::is_directory(path)) {
            return false;
        }
    }
    return true;
}

bool FastAppPackager::IsHspPathValid(const std::list<std::string> &formatPathList, const std::string &hspSuffix)
{
    for (const auto& path : formatPathList) {
        if (!fs::is_directory(path) && !IsPathValid(path, true, hspSuffix)) {
            return false;
        }
    }
    return true;
}

bool FastAppPackager::ModuleJsonAndPackInfoExists(const std::list<std::string> &hapPathList,
    const std::list<std::string> &hspPathList)
{
    for (const auto& hapPath : hapPathList) {
        fs::path path = fs::path(hapPath);
        if (!fs::exists(path / fs::path(Constants::MODULE_JSON))) {
            LOGE("not found module.json in path: %s", path.string().c_str());
            return false;
        }
        if (!fs::exists(path / fs::path(Constants::PACK_INFO))) {
            LOGE("not found pack.info in path: %s", path.string().c_str());
            return false;
        }
    }
    for (const auto& hspPath : hspPathList) {
        fs::path path = fs::path(hspPath);
        if (fs::is_directory(path)) {
            if (!fs::exists(path / fs::path(Constants::MODULE_JSON))) {
                LOGE("not found module.json in path: %s", path.string().c_str());
                return false;
            }
            if (!fs::exists(path / fs::path(Constants::PACK_INFO))) {
                LOGE("not found pack.info in path: %s", path.string().c_str());
                return false;
            }
        }
    }
    return true;
}

bool FastAppPackager::CheckBundleTypeConsistency(const std::list<std::string> &hapPathList,
    const std::list<std::string> &hspPathList)
{
    std::string bundleType;
    if (!hapPathList.empty()) {
        bundleType = GetBundleTypeFromPath(hapPathList.front());
    } else if (!hspPathList.empty()) {
        bundleType = GetBundleTypeFromPath(hspPathList.front());
    }
    if (bundleType.empty()) {
        return false;
    }
    for (const auto& hapPath : hapPathList) {
        if (GetBundleTypeFromPath(hapPath) != bundleType) {
            LOGE("bundleType is not same");
            return false;
        }
    }
    for (const auto& hspPath : hspPathList) {
        if (GetBundleTypeFromPath(hspPath) != bundleType) {
            LOGE("bundleType is not same");
            return false;
        }
    }
    if (bundleType == Constants::TYPE_SHARED) {
        isSharedApp_ = true;
    }
    return true;
}

std::string FastAppPackager::GetBundleTypeFromPath(const std::string &filePath)
{
    fs::path path(filePath);
    if (!fs::exists(path)) {
        LOGE("getBundleTypeFromPath path not exists: %s", path.string().c_str());
        return "";
    }
    std::string content = GetModuleJsonContentFromPath(path);
    if (content.empty()) {
        return "";
    }
    return GetBundleTypeFromModuleJson(content);
}

std::string FastAppPackager::GetModuleJsonContentFromPath(const fs::path &path)
{
    if (fs::is_regular_file(path)) {
        std::string filename = path.filename().string();
        if (filename == Constants::MODULE_JSON) {
            return ReadFileToString(path);
        } else if (Utils::EndsWith(filename, Constants::HSP_SUFFIX)) {
            std::string fileContent;
            if (!ZipUtils::GetFileContentFromZip(path, Constants::MODULE_JSON, fileContent)) {
                LOGE("GetModuleJsonContentFromPath err: %s", path.string().c_str());
                return "";
            }
            return fileContent;
        }
    } else {
        return ReadFileToString(path / fs::path(Constants::MODULE_JSON));
    }
    return "";
}

std::string FastAppPackager::ReadFileToString(const fs::path &path)
{
    std::string realPath;
    if (!Utils::GetRealPath(path.string(), realPath)) {
        LOGE("get real path failed! path=%s", path.string().c_str());
        return "";
    }
    std::ifstream file(realPath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
  
    std::stringstream buffer;
    buffer << file.rdbuf();
  
    return buffer.str();
}

bool FastAppPackager::CheckSkillRules(const std::list<std::string> &hapPathList,
    const std::list<std::string> &hspPathList)
{
    for (const auto &hapPath : hapPathList) {
        if (!CheckSkillRulesForPath(hapPath, true)) {
            return false;
        }
    }
    for (const auto &hspPath : hspPathList) {
        if (!CheckSkillRulesForPath(hspPath, false)) {
            return false;
        }
    }
    std::string bundleType;
    bool hasSkillBundleType = false;
    if (!hspPathList.empty()) {
        bundleType = GetBundleTypeFromPath(hspPathList.front());
    }
    hasSkillBundleType = bundleType == Constants::TYPE_SKILL;
    return CheckSkillBundleTypeConstraints(hapPathList, hspPathList, hasSkillBundleType);
}

bool FastAppPackager::CheckSkillRulesForPath(const std::string &pathValue, bool isHapPath)
{
    const fs::path path(pathValue);
    if (!fs::is_directory(path) && !fs::is_regular_file(path)) {
        return true;
    }

    ModuleJson moduleJson;
    if (!ParseModuleJsonContent(GetModuleJsonContentFromPath(path), moduleJson)) {
        LOGE("Failed to parse module.json from path '%s' for Fast App skill validation.", pathValue.c_str());
        return false;
    }

    std::string moduleType;
    moduleJson.GetStageModuleType(moduleType);
    if (isHapPath && moduleType == Constants::TYPE_SKILL) {
        LOGE("moduleType 'skill' is not allowed in --hap-path, use --hsp-path instead.");
        return false;
    }
    return ValidateAppPackSkillProfiles(path, moduleJson);
}

bool FastAppPackager::CheckSkillBundleTypeConstraints(const std::list<std::string> &hapPathList,
    const std::list<std::string> &hspPathList, bool hasSkillBundleType)
{
    if (!hasSkillBundleType) {
        return true;
    }
    if (!hapPathList.empty()) {
        LOGE("--hap-path must be empty when bundleType is skill.");
        return false;
    }

    int hspCount = static_cast<int>(hspPathList.size());
    if (hspCount > 1) {
        LOGE("--hsp-path must contain only 1 HSP when bundleType is skill, but got %d.", hspCount);
        return false;
    }

    for (const auto &hspPath : hspPathList) {
        if (!IsSkillHspModule(hspPath)) {
            return false;
        }
    }
    return true;
}

bool FastAppPackager::IsSkillHspModule(const std::string &pathValue)
{
    const fs::path path(pathValue);
    if (!fs::is_directory(path) && !fs::is_regular_file(path)) {
        return true;
    }

    ModuleJson moduleJson;
    if (!ParseModuleJsonContent(GetModuleJsonContentFromPath(path), moduleJson)) {
        LOGE("Failed to parse module.json from path '%s' for Fast App skill moduleType validation.",
            pathValue.c_str());
        return false;
    }

    std::string moduleType;
    moduleJson.GetStageModuleType(moduleType);
    if (moduleType == Constants::TYPE_SKILL) {
        return true;
    }

    LOGE("HSP moduleType must be 'skill' when bundleType is skill, but got '%s'.", moduleType.c_str());
    return false;
}

std::string FastAppPackager::GetBundleTypeFromModuleJson(const std::string &moduleJsonContent)
{
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromString(moduleJsonContent)) {
        LOGE("GetBundleTypeFromModuleJson ParseFromString err.");
        return "";
    }
    
    std::string bundleType;
    if (!moduleJson.GetStageBundleType(bundleType)) {
        LOGE("GetBundleTypeFromModuleJson GetStageBundleType err.");
        return "";
    }
    return bundleType != "" ? bundleType : Constants::MODE_APP;
}

bool FastAppPackager::IsPackInfoValid(const fs::path &packInfo, const std::list<std::string> &hapPathList,
    const std::list<std::string> &hspPathList)
{
    std::list<std::string> allPackages;
    if (!GetPackageNameFromPath(packInfo, allPackages)) {
        LOGE("GetPackageNameFromPath for packInfo err: %s", packInfo.string().c_str());
        return false;
    }
    std::set<std::string> allPackageSet(allPackages.begin(), allPackages.end());
    if (allPackages.size() > allPackageSet.size()) {
        LOGE("package name is redundant in app pack.info: %s", packInfo.string().c_str());
        return false;
    }
    if (allPackages.empty()) {
        LOGE("app pack.info format err: %s", packInfo.string().c_str());
        return false;
    }
    std::set<std::string> packages;
    if (!IsPackInfoPathListValid(hapPathList, packages, allPackageSet) ||
        !IsPackInfoPathListValid(hspPathList, packages, allPackageSet)) {
        return false;
    }

    if (allPackageSet != packages) {
        LOGE("package name not same between module and app pack.info.");
        return false;
    }
    return true;
}

bool FastAppPackager::IsPackInfoPathListValid(const std::list<std::string> &pathList, std::set<std::string> &packages,
    const std::set<std::string> &allPackageSet)
{
    for (const auto& path : pathList) {
        std::list<std::string> list;
        if (!GetPackageNameFromPath(fs::path(path), list)) {
            LOGE("GetPackageNameFromPath for packInfo err");
            return false;
        }
        if (list.size() != 1) {
            LOGE("module pack.info format err: %s", path.c_str());
            return false;
        }
        std::string packageName = list.front();
        if (allPackageSet.find(packageName) == allPackageSet.end()) {
            LOGE("module pack.info name not exist in app pack.info name list: %s", path.c_str());
            return false;
        }
        if (packages.find(packageName) != packages.end()) {
            LOGE("package name is redundant in %s", path.c_str());
            return false;
        }
        packages.insert(packageName);
    }
    return true;
}

bool FastAppPackager::GetPackageNameFromPath(const fs::path &path, std::list<std::string> &packageNameList)
{
    if (!fs::exists(path)) {
        LOGE("GetPackageNameFromPath path not exists: %s", path.string().c_str());
        return false;
    }
    if (fs::is_regular_file(path)) {
        std::string filename = path.filename().string();
        if (Utils::EndsWith(filename, Constants::HSP_SUFFIX)) {
            // .hsp: return filename
            packageNameList.push_back(filename.substr(0, filename.find(Constants::HSP_SUFFIX)));
            return true;
        }
    }
    std::string content = GetPackInfoContentFromPath(path);
    if (content.empty()) {
        return false;
    }
    
    PackInfo packInfo;
    if (!packInfo.ParseFromString(content)) {
        LOGE("GetPackageNameFromPath ParseFromString err.");
        return false;
    }
    return packInfo.GetPackageNames(packageNameList);
}

std::string FastAppPackager::GetPackInfoContentFromPath(const fs::path &path)
{
    if (fs::is_regular_file(path)) {
        std::string filename = path.filename().string();
        if (filename == Constants::PACK_INFO) {
            return ReadFileToString(path);
        } else if (Utils::EndsWith(filename, Constants::HSP_SUFFIX)) {
            std::string fileContent;
            if (!ZipUtils::GetFileContentFromZip(path, Constants::PACK_INFO, fileContent)) {
                LOGE("GetPackInfoContentFromPath err: %s", path.string().c_str());
                return "";
            }
            return fileContent;
        }
    } else {
        return ReadFileToString(path / fs::path(Constants::PACK_INFO));
    }
    return "";
}

bool FastAppPackager::CompressFastAppMode()
{
    fs::path tmpDir;
    fs::path appOutPath;
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        appOutPath = fs::path(it->second);
    }
    
    try {
        tmpDir = appOutPath.parent_path() / fs::path(Constants::COMPRESSOR_FAST_APP_TEMP_DIR + Utils::GenerateUUID());
        if (!fs::create_directories(tmpDir)) {
            LOGE("Fast App packager create temporary directory failed: %s", tmpDir.string().c_str());
            return false;
        }

        std::list<std::string> fileList;
        if (!Pack(tmpDir, fileList)) {
            if (fs::exists(tmpDir)) {
                fs::remove_all(tmpDir);
            }
            return false;
        }

        if (!CheckHapAndPackFastApp(fileList, tmpDir)) {
            if (fs::exists(tmpDir)) {
                fs::remove_all(tmpDir);
            }
            return false;
        }
    } catch (const std::exception& ex) {
        LOGE("Fast App packager compress failed: %s", ex.what());
        if (fs::exists(tmpDir)) {
            fs::remove_all(tmpDir);
        }
        return false;
    }
    if (fs::exists(tmpDir)) {
        fs::remove_all(tmpDir);
    }
    if (!ModuleJsonUtils::CheckAppAtomicServiceCompressedSizeValid(parameterMap_, hapVerifyInfoMap_)) {
        LOGE("Fast App packager atomic service size validation failed.");
        return false;
    }
    return true;
}

bool FastAppPackager::CheckHapAndPackFastApp(std::list<std::string> &fileList, const fs::path &tmpDir)
{
    if (!ModuleJsonUtils::CheckHapsIsValid(fileList, isSharedApp_)) {
            LOGE("Fast App packager HAP validation failed, check version, apiVersion, moduleName, packageName.");
            return false;
    }
    if (!ModuleJsonUtils::GetHapVerifyInfosMapfromFileList(fileList, hapVerifyInfoMap_)) {
        LOGE("Fast App packager failed to build HAP verify info map.");
        return false;
    }
    if (!PackFastApp(fileList)) {
        LOGE("Fast App packager pack process failed.");
        return false;
    }
    return true;
}

bool FastAppPackager::Pack(const std::string &tmpDir, std::list<std::string> &fileList)
{
    fs::path appPackInfo = fs::path(packInfoPath_);
        for (const auto& hapPath : formattedHapPathList_) {
            fs::path path = fs::path(hapPath);
            fs::path hap;
            if (!Pack(path, appPackInfo, tmpDir, hap)) {
                if (fs::exists(tmpDir)) {
                    fs::remove_all(tmpDir);
                }
                return false;
            }
            if (!hap.empty()) {
                fileList.push_back(hap.string());
            }
        }
        for (const auto& hspPath : formattedHspPathList_) {
            fs::path path = fs::path(hspPath);
            fs::path hsp;
            if (!Pack(path, appPackInfo, tmpDir, hsp)) {
                if (fs::exists(tmpDir)) {
                    fs::remove_all(tmpDir);
                }
                return false;
            }
            if (!hsp.empty()) {
                fileList.push_back(hsp.string());
            }
        }
    return true;
}

bool FastAppPackager::Pack(const fs::path &inputPath, const fs::path &appPackInfo, const fs::path &outPath,
    fs::path &path)
{
    if (!fs::exists(inputPath)) {
        LOGE("pack err, input path not exists.");
        return false;
    }
    if (!fs::exists(appPackInfo)) {
        LOGE("pack err, app pack.info not exists.");
        return false;
    }
    if (fs::is_directory(inputPath)) {
        return PackDir(inputPath, appPackInfo, outPath, path);
    } else if (fs::is_regular_file(inputPath) && Utils::EndsWith(inputPath.filename().string(),
        Constants::HSP_SUFFIX)) {
        return RepackHsp(inputPath, appPackInfo, outPath, path);
    }

    LOGE("pack err, not support: %s", inputPath.string().c_str());
    return false;
}

bool FastAppPackager::PackDir(const fs::path &inputPath, const fs::path &appPackInfo,
    const fs::path &outPath, fs::path &path)
{
    std::list<std::string> packageNames;
    if (!GetPackageNameFromPath(inputPath / fs::path(Constants::PACK_INFO), packageNames)) {
        LOGE("pack err, pack.info format err");
        return false;
    }
    if (packageNames.size() != 1) {
        LOGE("pack err, pack.info format err");
        return false;
    }
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(inputPath / fs::path(Constants::MODULE_JSON))) {
        LOGE("pack err, module.json format err");
        return false;
    }
    std::string pkgName = packageNames.front();
    std::string moudleType;
    moduleJson.GetStageModuleType(moudleType);
    moduleJson.GetStageCompressNativeLibs(compressNativeLibs_);
    moduleJson.GetGenerateBuildHash(isGenerateBuildHash_);
    // Extract skillProfiles names for filtering during compression
    skillProfileNames_.clear();
    std::list<std::map<std::string, std::string>> skillProfiles;
    if (moduleJson.GetSkillProfiles(skillProfiles)) {
        for (const auto& profile : skillProfiles) {
            auto nameIt = profile.find("name");
            if (nameIt != profile.end()) {
                skillProfileNames_.insert(nameIt->second);
            }
        }
    }
    std::string suffix = (moudleType == Constants::TYPE_SHARED || moudleType == Constants::TYPE_SKILL)
        ? Constants::HSP_SUFFIX : Constants::HAP_SUFFIX;

    fs::path outHap = outPath / fs::path(pkgName);
    outHap += fs::path(suffix);
    return packSingleThread(inputPath, appPackInfo, outHap, path);
}

bool FastAppPackager::RepackHsp(const fs::path &inputPath, const fs::path &appPackInfo,
    const fs::path &outPath, fs::path &path)
{
    fs::path outHsp = outPath / inputPath.filename();
    std::string outHspStr = outHsp.string();
    zipWrapper_.Open(outHspStr);
    if (!zipWrapper_.IsOpen()) {
        LOGE("Fast App repack HSP open zip failed.");
        return false;
    }

    std::string zipPath = Constants::NULL_DIR_NAME;
    if (!fs::is_directory(appPackInfo)) {
        zipPath = (appPackInfo).filename().string();
    }
    if (zipWrapper_.AddFileOrDirectoryToZip(appPackInfo.string(), zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("Fast App repack HSP add file to zip failed.");
        return false;
    }
    
    std::string uZipTempPath = "uzip_fast_app_temp_";
    fs::path unzipPathString = inputPath.parent_path() / fs::path(uZipTempPath + Utils::GenerateUUID());
    if (!fs::create_directories(unzipPathString)) {
        LOGE("Can't create directory to path %s", unzipPathString.string().c_str());
        return false;
    }
    std::string uzipHsp = UzipHspAndRemovePackInfo(inputPath.string(), unzipPathString.string());
    if (zipWrapper_.AddFileOrDirectoryToZip(uzipHsp, Constants::NULL_DIR_NAME) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("Fast App repack HSP add file to zip failed.");
        if (fs::exists(uzipHsp)) {
            fs::remove_all(uzipHsp);
        }
        return false;
    }
    fs::remove_all(uzipHsp);
    zipWrapper_.Close();
    path = outHspStr;
    return true;
}

bool FastAppPackager::PackFastApp(const std::list<std::string> &fileList)
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_OUT_PATH);
    std::string outPath;
    if (it != parameterMap_.end()) {
        outPath = it->second;
    }
    zipWrapper_.Open(outPath);
    if (!zipWrapper_.IsOpen()) {
        LOGE("Fast App pack open zip failed.");
        return false;
    }

    if (zipWrapper_.AddFileOrDirectoryToZip(packInfoPath_, Constants::PACK_INFO) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("Fast App pack write string to zip failed.");
        return false;
    }

    if (!AddHapListToApp(fileList)) {
        zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
        LOGE("Fast App add HAP list to app failed.");
        return false;
    }
    
    std::string moduleName;
    it = parameterMap_.find(Constants::PARAM_ENTRYCARD_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        std::string entryCardPath = Constants::ENTRYCARD_NAME + Constants::LINUX_FILE_SEPARATOR +
            moduleName + Constants::LINUX_FILE_SEPARATOR +
            Constants::ENTRYCARD_BASE_NAME + Constants::LINUX_FILE_SEPARATOR +
            Constants::ENTRYCARD_SNAPSHOT_NAME + Constants::LINUX_FILE_SEPARATOR;
        for (auto itemFormattedEntryCardPath : formattedEntryCardPathList_) {
            if (zipWrapper_.AddFileOrDirectoryToZip(itemFormattedEntryCardPath, entryCardPath +
                fs::path(itemFormattedEntryCardPath).filename().string()) !=
                ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("Fast App process add file to zip failed.");
                return false;
            }
        }
    }
    it = parameterMap_.find(Constants::PARAM_PACK_RES_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::FILE_PACK_RES) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App process add file to zip failed.");
            return false;
        }
    }
    it = parameterMap_.find(Constants::PARAM_PAC_JSON_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, Constants::PAC_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App pack add pac.json failed.");
            return false;
        }
    }
    return AddSignatureAndCertificateToApp();
}

bool FastAppPackager::AddHapListToApp(const std::list<std::string> &fileList)
{
    for (const auto& hapPath : fileList) {
        HapVerifyInfo hapVerifyInfo;
        if (ModuleJsonUtils::IsModuleHap(hapPath)) {
            if (!ModuleJsonUtils::GetStageHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGW("GetStageHapVerifyInfo failed! hapPath:%s", hapPath.c_str());
            }
        } else {
            if (!ModuleJsonUtils::GetFaHapVerifyInfo(hapPath, hapVerifyInfo)) {
                LOGW("GetFaHapVerifyInfo failed! hapPath:%s", hapPath.c_str());
            }
        }
        if (hapVerifyInfo.IsDebug()) {
            zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_0);
        }
        zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_DEFLATED);
        if (zipWrapper_.AddFileOrDirectoryToZip(hapPath, fs::path(hapPath).filename().string()) !=
            ZipErrCode::ZIP_ERR_SUCCESS) {
            zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
            zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_STORED);
            LOGE("Fast App process add file to zip failed.");
            return false;
        }
        zipWrapper_.SetZipLevel(ZipLevel::ZIP_LEVEL_DEFAULT);
        zipWrapper_.SetZipMethod(ZipMethod::ZIP_METHOD_STORED);
    }
    return true;
}

bool FastAppPackager::AddSignatureAndCertificateToApp()
{
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_CERTIFICATE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App process add file to zip failed.");
            return false;
        }
    }

    it = parameterMap_.find(Constants::PARAM_SIGNATURE_PATH);
    if (it != parameterMap_.end() && !it->second.empty()) {
        fs::path filePath = fs::path(it->second);
        std::string zipPath = Constants::NULL_DIR_NAME;
        if (!fs::is_directory(filePath)) {
            zipPath = (filePath).filename().string();
        }
        if (zipWrapper_.AddFileOrDirectoryToZip(it->second, zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App process add file to zip failed.");
            return false;
        }
    }

    zipWrapper_.Close();
    return true;
}

bool FastAppPackager::packMultiThread(const fs::path &inputPath, const fs::path &appPackInfo,
    const fs::path &outHap, fs::path &path)
{
    return true;
}

bool FastAppPackager::packSingleThread(const fs::path &inputPath, const fs::path &appPackInfo,
    const fs::path &outHap, fs::path &path)
{
    if (!fs::exists(inputPath) || !fs::is_directory(inputPath)) {
        LOGE("Directory does not exist or is not a directory");
        return false;
    }
    
    std::string pathStr = outHap.string();
    zipWrapper_.Open(pathStr);
    if (!zipWrapper_.IsOpen()) {
        LOGE("Fast App single-thread pack open zip failed: %s", pathStr.c_str());
        return false;
    }

    std::string zipPath = Constants::NULL_DIR_NAME;
    if (!fs::is_directory(appPackInfo)) {
        zipPath = (appPackInfo).filename().string();
    }
    if (zipWrapper_.AddFileOrDirectoryToZip(appPackInfo.string(), zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
        LOGE("Fast App process add file to zip failed.");
        return false;
    }

    if (isGenerateBuildHash_) {
        std::string jsonString;
        GenBuildHash(inputPath, jsonString);
        if (zipWrapper_.WriteStringToZip(jsonString, Constants::MODULE_JSON) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App single-thread pack write string to zip failed.");
            return false;
        }
    }
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (!AddOtherFileToZip(entry.path())) {
            return false;
        }
    }
    zipWrapper_.Close();
    path = pathStr;
    isGenerateBuildHash_ = false;
    return true;
}

bool FastAppPackager::AddOtherFileToZip(const fs::path &entry)
{
    if (fs::is_regular_file(entry) && entry.filename().string() != Constants::PACK_INFO) {
        if (isGenerateBuildHash_ && entry.filename().string() == Constants::MODULE_JSON) {
            return true;
        }
        std::string zipPath = entry.filename().string();
        if (zipWrapper_.AddFileOrDirectoryToZip(entry.string(), zipPath) != ZipErrCode::ZIP_ERR_SUCCESS) {
            LOGE("Fast App process add file to zip failed.");
            return false;
        }
    } else if (fs::is_directory(entry)) {
        if (entry.filename() == Constants::SKILLS_DIR) {
            // No skillProfiles configured → skip skills/ entirely
            if (skillProfileNames_.empty()) {
                return true;
            }
            if (!AddFastAppSkillsToZip(entry, skillProfileNames_, zipWrapper_)) {
                LOGE("Fast App add skills directory to zip failed.");
                return false;
            }
        } else if (entry.filename() == Constants::LIB_PATH) {
            if (zipWrapper_.AddFileOrDirectoryToZip(entry.string(), Constants::LIB_PATH) !=
                ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("Fast App process add file to zip failed.");
                return false;
            }
        } else {
            if (zipWrapper_.AddFileOrDirectoryToZip(entry.string(), entry.filename().string() +
                Constants::NULL_DIR_NAME) != ZipErrCode::ZIP_ERR_SUCCESS) {
                LOGE("Fast App process add file to zip failed.");
                return false;
            }
        }
    }
    return true;
}

void FastAppPackager::GenBuildHash(const fs::path &inputPath, std::string &jsonString)
{
    std::string hash = Utils::GetSha256Folder(inputPath);
    if (hash.empty()) {
        return;
    }
    ModuleJson moduleJson;
    if (!moduleJson.ParseFromFile(inputPath / fs::path(Constants::MODULE_JSON))) {
        LOGE("pack err, module.json format err");
        return;
    }
    moduleJson.SetBuildHash(hash);
    jsonString = moduleJson.ToString();
}

std::string FastAppPackager::UzipHspAndRemovePackInfo(const std::string& hspPath, const std::string& unzipPathString)
{
    ZipUtils::Unzip(hspPath, unzipPathString);
    for (const auto& entry : fs::directory_iterator(unzipPathString)) {
        if (entry.path().filename() == Constants::PACK_INFO) {
            fs::remove(entry.path());
            continue;
        }
    }
    return unzipPathString;
}
} // namespace AppPackingTool
} // namespace OHOS
