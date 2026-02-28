/*
 * Copyright (c) 2025-2026 Huawei Device Co., Ltd.
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

#include "general_normalize_version_utils.h"
#include "general_normalize_version.h"

#include "log.h"
#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string DEVICE_TYPES = "deviceTypes";
const std::string BUNDLE_NAME = "bundleName";
const std::string VERSION_CODE = "versionCode";
const std::string VERSION_NAME = "versionName";
const std::string MIN_COMPATIBLE_VERSION_CODE = "minCompatibleVersionCode";
const std::string MIN_API_VERSION = "minAPIVersion";
const std::string TARGET_API_VERSION = "targetAPIVersion";
const std::string API_RELEASE_TYPE = "apiReleaseType";
const std::string BUNDLE_TYPE = "bundleType";
const std::string INSTALLATION_FREE = "installationFree";
const std::string DELIVERY_WITH_INSTALL = "deliveryWithInstall";
const std::string MODULE_NAME = "moduleName";
}

std::string GeneralNormalizeVersionUtils::ArrayToString(const std::list<GeneralNormalizeVersion>& generalNormalizeVersions)
{
    LOGD("ArrayToString begin");
    std::unique_ptr<PtJson> versionsJson = PtJson::CreateArray();
    for (auto& generalNormalizeVersion : generalNormalizeVersions) {
        std::unique_ptr<PtJson> ptJson = PtJson::CreateObject();
        if (generalNormalizeVersion.moduleName != "") {
            if (!ptJson->Add(MODULE_NAME.c_str(), generalNormalizeVersion.moduleName.c_str())) {
                LOGE("add moduleName failed");
                return "";
            }
        }
        if (!generalNormalizeVersion.originDeviceTypes.empty()) {
            if (!ptJson->Add(DEVICE_TYPES.c_str(), generalNormalizeVersion.originDeviceTypes)) {
                LOGE("add originDeviceTypes failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originBundleName != "") {
            if (!ptJson->Add(BUNDLE_NAME.c_str(), generalNormalizeVersion.originBundleName.c_str())) {
                LOGE("add originBundleName failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originVersionCode != -1) {
            if (!ptJson->Add(VERSION_CODE.c_str(), generalNormalizeVersion.originVersionCode)) {
                LOGE("add originVersionCode failed");
                return "";
            }
        } 
        if (generalNormalizeVersion.originVersionName != "") {
            if (!ptJson->Add(VERSION_NAME.c_str(), generalNormalizeVersion.originVersionName.c_str())) {
                LOGE("add originVersionName failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originMinCompatibleVersionCode != -1) {
            if (!ptJson->Add(MIN_COMPATIBLE_VERSION_CODE.c_str(),
                generalNormalizeVersion.originMinCompatibleVersionCode)) {
                    LOGE("add originMinCompatibleVersionCode failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originMinAPIVersion != -1) {
            if (!ptJson->Add(MIN_API_VERSION.c_str(), generalNormalizeVersion.originMinAPIVersion)) {
                LOGE("add originMinAPIVersion failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originTargetAPIVersion != -1) {
            if (!ptJson->Add(TARGET_API_VERSION.c_str(), generalNormalizeVersion.originTargetAPIVersion)) {
                LOGE("add originTargetAPIVersion failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originApiReleaseType != "") {
            if (!ptJson->Add(API_RELEASE_TYPE.c_str(), generalNormalizeVersion.originApiReleaseType.c_str())) {
                LOGE("add originApiReleaseType failed");
                return "";
            }
        }
        if (generalNormalizeVersion.originBundleType != "") {
            if (!ptJson->Add(BUNDLE_TYPE.c_str(), generalNormalizeVersion.originBundleType.c_str())) {
                LOGE("add originBundleType failed");
                return "";
            }
        }
        if (generalNormalizeVersion.modifyInstallationFree == true) {
            if (!ptJson->Add(INSTALLATION_FREE.c_str(), generalNormalizeVersion.originInstallationFree)) {
                LOGE("add originInstallationFree failed");
                return "";
            }
        }
        if (generalNormalizeVersion.modifyDeliveryWithInstall == true) {
            if (!ptJson->Add(DELIVERY_WITH_INSTALL.c_str(), generalNormalizeVersion.originDeliveryWithInstall)) {
                LOGE("add originDeliveryWithInstall failed");
                return "";
            }
        }
        versionsJson->Push(ptJson);
    }
    return versionsJson->Stringify();
}
} // namespace AppPackingTool
} // namespace OHOS
