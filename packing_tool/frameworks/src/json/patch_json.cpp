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

#include "patch_json.h"

#include <fstream>

#include "error/packing_tool_err_msg.h"
#include "log.h"
#include "utils.h"

using packing_tool::error::PackingToolErrMsg;

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string APP = "app";
const std::string MODULE = "module";
const std::string BUNDLE_NAME = "bundleName";
const std::string VERSION_CODE = "versionCode";
const std::string VERSION_NAME = "versionName";
const std::string PATCH_VERSION_CODE = "patchVersionCode";
const std::string PATCH_VERSION_NAME = "patchVersionName";
const std::string NAME = "name";
const std::string TYPE = "type";
const std::string DEVICE_TYPES = "deviceTypes";
const std::string ORIGINAL_MODULE_HASH = "originalModuleHash";
}

bool PatchJson::ParseFromString(const std::string& jsonString)
{
    Release();
    if (jsonString.length() == 0) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json length is zero!").c_str());
        return false;
    }
    root_ = PtJson::Parse(jsonString);
    return IsValid();
}

bool PatchJson::ParseFromFile(const std::string& jsonFile)
{
    Release();
    std::string realJsonFile;
    if (!Utils::GetRealPath(jsonFile, realJsonFile)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("get real json file failed! jsonFile=") + jsonFile).c_str());
        return false;
    }
    std::ifstream inFile(realJsonFile, std::ios::in);
    if (!inFile.is_open()) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Open json file failed! jsonFile=") + jsonFile + ", realJsonFile=" + realJsonFile).c_str());
        return false;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    root_ = PtJson::Parse(fileContent);
    return IsValid();
}

std::string PatchJson::ToString()
{
    return root_->Stringify();
}

void PatchJson::Release()
{
    if (root_) {
        root_->ReleaseRoot();
        root_ = nullptr;
    }
}

bool PatchJson::IsValid()
{
    return (root_.get() != nullptr);
}

bool PatchJson::GetAppObject(std::unique_ptr<PtJson>& appObj)
{
    if (root_.get() == nullptr) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(APP.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root has no ") + APP + " node!").c_str());
        return false;
    }
    if (root_->GetObject(APP.c_str(), &appObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root get ") + APP + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetModuleObject(std::unique_ptr<PtJson>& moduleObj)
{
    if (root_.get() == nullptr) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!root_->Contains(MODULE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root has no ") + MODULE + " node!").c_str());
        return false;
    }
    if (root_->GetObject(MODULE.c_str(), &moduleObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Json root get ") + MODULE + " node failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetBundleName(std::string& bundleName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetBundleNameByAppObj(appObj, bundleName);
}

bool PatchJson::GetBundleNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& bundleName)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(BUNDLE_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs(
            std::string("App node has no ") + BUNDLE_NAME + " node!").c_str());
        return false;
    }
    if (appObj->GetString(BUNDLE_NAME.c_str(), &bundleName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED.toStringWithArgs(
            std::string("App node get ") + BUNDLE_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetVersionCode(int32_t& versionCode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetVersionCodeByAppObj(appObj, versionCode);
}

bool PatchJson::GetVersionCodeByAppObj(const std::unique_ptr<PtJson>& appObj, int32_t& versionCode)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSION_CODE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSION_CODE + " node!").c_str());
        return false;
    }
    if (appObj->GetInt(VERSION_CODE.c_str(), &versionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSION_CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetVersionName(std::string& versionName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetVersionNameByAppObj(appObj, versionName);
}

bool PatchJson::GetVersionNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& versionName)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(VERSION_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + VERSION_NAME + " node!").c_str());
        return false;
    }
    if (appObj->GetString(VERSION_NAME.c_str(), &versionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + VERSION_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetPatchVersionCode(int32_t& patchVersionCode)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetPatchVersionCodeByAppObj(appObj, patchVersionCode);
}

bool PatchJson::GetPatchVersionCodeByAppObj(const std::unique_ptr<PtJson>& appObj, int32_t& patchVersionCode)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(PATCH_VERSION_CODE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + PATCH_VERSION_CODE + " node!").c_str());
        return false;
    }
    if (appObj->GetInt(PATCH_VERSION_CODE.c_str(), &patchVersionCode) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + PATCH_VERSION_CODE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetPatchVersionName(std::string& patchVersionName)
{
    std::unique_ptr<PtJson> appObj;
    if (!GetAppObject(appObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetAppObject failed!").c_str());
        return false;
    }
    return GetPatchVersionNameByAppObj(appObj, patchVersionName);
}

bool PatchJson::GetPatchVersionNameByAppObj(const std::unique_ptr<PtJson>& appObj, std::string& patchVersionName)
{
    if (!appObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("App node is null!").c_str());
        return false;
    }
    if (!appObj->Contains(PATCH_VERSION_NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node has no ") + PATCH_VERSION_NAME + " node!").c_str());
        return false;
    }
    if (appObj->GetString(PATCH_VERSION_NAME.c_str(), &patchVersionName) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("App node get ") + PATCH_VERSION_NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetName(std::string& name)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_PATCH_MODULE_NAME_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetNameByModuleObj(moduleObj, name);
}

bool PatchJson::GetNameByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::string& name)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_PATCH_MODULE_NAME_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(NAME.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_PATCH_MODULE_NAME_FAILED.toStringWithArgs(
            std::string("Module node has no ") + NAME + " node!").c_str());
        return false;
    }
    if (moduleObj->GetString(NAME.c_str(), &name) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_PATCH_MODULE_NAME_FAILED.toStringWithArgs(
            std::string("Module node get ") + NAME + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetType(std::string& type)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetTypeByModuleObj(moduleObj, type);
}

bool PatchJson::GetTypeByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::string& type)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(TYPE.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + TYPE + " node!").c_str());
        return false;
    }
    if (moduleObj->GetString(TYPE.c_str(), &type) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + TYPE + " failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetDeviceTypes(std::list<std::string>& deviceTypes)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetDeviceTypesByModuleObj(moduleObj, deviceTypes);
}

bool PatchJson::GetDeviceTypesByModuleObj(const std::unique_ptr<PtJson>& moduleObj, std::list<std::string>& deviceTypes)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(DEVICE_TYPES.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + DEVICE_TYPES + " node!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> deviceTypesObj;
    if (moduleObj->GetArray(DEVICE_TYPES.c_str(), &deviceTypesObj) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + DEVICE_TYPES + " array node failed!").c_str());
        return false;
    }
    for (int32_t i = 0; i < deviceTypesObj->GetSize(); i++) {
        deviceTypes.push_back(deviceTypesObj->Get(i)->GetString());
    }
    return true;
}

bool PatchJson::GetOriginalModuleHash(std::string& originalModuleHash)
{
    std::unique_ptr<PtJson> moduleObj;
    if (!GetModuleObject(moduleObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("GetModuleObject failed!").c_str());
        return false;
    }
    return GetOriginalModuleHashByModuleObj(moduleObj, originalModuleHash);
}

bool PatchJson::GetOriginalModuleHashByModuleObj(const std::unique_ptr<PtJson>& moduleObj,
    std::string& originalModuleHash)
{
    if (!moduleObj) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Module node is null!").c_str());
        return false;
    }
    if (!moduleObj->Contains(ORIGINAL_MODULE_HASH.c_str())) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node has no ") + ORIGINAL_MODULE_HASH + " node!").c_str());
        return false;
    }
    if (moduleObj->GetString(ORIGINAL_MODULE_HASH.c_str(), &originalModuleHash) != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            std::string("Module node get ") + ORIGINAL_MODULE_HASH + " array node failed!").c_str());
        return false;
    }
    return true;
}

bool PatchJson::GetHqfInfo(HqfInfo& hqfInfo)
{
    std::unique_ptr<PtJson> appObj;
    std::unique_ptr<PtJson> moduleObj;
    if (!GetAppObject(appObj) || !GetModuleObject(moduleObj)) {
        return false;
    }
    std::string bundleName = "";
    int32_t versionCode = -1;
    std::string versionName = "";
    int32_t patchVersionCode = -1;
    std::string patchVersionName = "";
    std::string moduleName = "";
    std::string type = "";
    std::list<std::string> deviceTypes;
    std::string originalModuleHash = "";
    if (!GetBundleNameByAppObj(appObj, bundleName) ||
        !GetVersionCodeByAppObj(appObj, versionCode) ||
        !GetVersionNameByAppObj(appObj, versionName) ||
        !GetPatchVersionCodeByAppObj(appObj, patchVersionCode) ||
        !GetPatchVersionNameByAppObj(appObj, patchVersionName) ||
        !GetNameByModuleObj(moduleObj, moduleName) ||
        !GetTypeByModuleObj(moduleObj, type) ||
        !GetDeviceTypesByModuleObj(moduleObj, deviceTypes) ||
        !GetOriginalModuleHashByModuleObj(moduleObj, originalModuleHash)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Get nodes failed!").c_str());
        return false;
    }
    hqfInfo.SetBundleName(bundleName);
    hqfInfo.SetVersionCode(versionCode);
    hqfInfo.SetVersionName(versionName);
    hqfInfo.SetPatchVersionCode(patchVersionCode);
    hqfInfo.SetPatchVersionName(patchVersionName);
    hqfInfo.SetModuleName(moduleName);
    hqfInfo.SetType(type);
    hqfInfo.SetDeviceTypes(deviceTypes);
    hqfInfo.SetOriginalModuleHash(originalModuleHash);
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
