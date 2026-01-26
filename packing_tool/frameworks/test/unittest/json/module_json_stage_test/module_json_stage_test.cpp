/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include <cstdlib>
#include <numeric>
#include <string>

#include "module_json.h"
#include "pt_json.h"
#include "log.h"

using namespace testing;
using namespace testing::ext;
using namespace std;
using namespace OHOS::AppPackingTool;

namespace OHOS {
namespace {
const std::string MODULE_JSON_TEST_STRING = "{"
    "\"app\": {"
        "\"apiVersion\": {"
        "}"
    "},"
    "\"module\": {"
        "\"requestPermissions\": true,"
        "\"proxyData\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"generateBuildHash\": true"
    "}"
"}";

const std::string MODULE_JSON_TEST_STRING_NOTHING = "{"
"}";

const std::string MODULE_JSON_TEST_STRING_ERROR = "{"
    "\"app\": {"
        "\"bundleType\": \"atomicService\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING = "{"
    "\"app\": {"
        "\"vendor\": \"\","
        "\"versionCode\": \"test\","
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": true,"
        "\"compileSdkVersion\": true,"
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": true,"
        "\"debug\": \"test_bundle_name\","
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": true,"
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": \"test_multiAppMode\","
        "\"tsanEnabled\": \"test_multiAppMode\","
        "\"compressNativeLibs\": \"test_multiAppMode\","
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_BUILDVERSION_TEST_STRING = "{"
    "\"app\": {"
        "\"vendor\": \"\","
        "\"versionCode\": \"test\","
        "\"buildVersion\": \"test_build_version\","
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": true,"
        "\"compileSdkVersion\": true,"
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": true,"
        "\"debug\": \"test_bundle_name\","
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": true,"
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": \"test_multiAppMode\","
        "\"tsanEnabled\": \"test_multiAppMode\","
        "\"compressNativeLibs\": \"test_multiAppMode\","
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"

        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": 1000000,"
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": \"test_compileSdkVersion\","
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": true,"
        "\"type\": true,"
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": true,"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": \"test_compileSdkVersion\","
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": \"test_bundle_name\""
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": \"test_bundle_name\","
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": \"test_version_name\","
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": false,"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_TEST_ONLYAPP = "{"
"\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "}"
"}";

const std::string MODULE_JSON_TEST_NOMODULETYPE = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_STRING_NOMODULENAME = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_STRING_NOPROXY_DATAS = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": false,"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_STRING_NOVENDOR = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": false,"
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";

const std::string MODULE_JSON_STRING = "{"
    "\"app\": {"
        "\"bundleName\": \"test_bundle_name\","
        "\"bundleType\": \"atomicService\","
        "\"vendor\": \"\","
        "\"versionCode\": 1000000,"
        "\"versionName\": \"test_version_name\","
        "\"icon\": \"media:app_icon\","
        "\"label\": \"string:app_name\","
        "\"apiReleaseType\": \"Canary\","
        "\"compileSdkVersion\": \"test_compileSdkVersion\","
        "\"targetAPIVersion\": 10,"
        "\"minAPIVersion\": 10,"
        "\"compileSdkType\": \"OpenHarmony\","
        "\"debug\": true,"
        "\"iconId\": 16777217,"
        "\"labelId\": 16777216,"
        "\"version\": {"
            "\"code\": 666,"
            "\"name\": \"test_version\","
            "\"minCompatibleVersionCode\": 555"
        "},"
        "\"apiVersion\": {"
            "\"compileSdkType\": \"OpenHarmony\","
            "\"compileSdkVersion\": \"test_apiVersion_compileSdkVersion\","
            "\"releaseType\": \"test_apiVersion_release\","
            "\"compatible\": 7,"
            "\"target\": 10"
        "},"
        "\"targetBundleName\": \"test_app_targetBundleName\","
        "\"multiAppMode\": {"
            "\"multiAppModeType\": \"test_multiAppMode\","
            "\"maxCount\": 9"
        "},"
        "\"generateBuildHash\": true,"
        "\"minCompatibleVersionCode\": 99,"
        "\"asanEnabled\": true,"
        "\"tsanEnabled\": false,"
        "\"compressNativeLibs\": true,"
        "\"targetPriority\": 5"
    "},"
    "\"module\": {"
        "\"name\": \"entry\","
        "\"type\": \"entry\","
        "\"description\": \"string:module_desc\","
        "\"mainElement\": \"EntryAbility\","
        "\"deviceTypes\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"deliveryWithInstall\": true,"
        "\"installationFree\": true,"
        "\"pages\": \"profile:main_pages\","
        "\"abilities\": ["
            "{"
                "\"name\": \"EntryAbility\","
                "\"moduleName\": \"test_module_name\","
                "\"srcEntry\": \"./ets/entryability/EntryAbility.ts\","
                "\"description\": \"string:EntryAbility_desc\","
                "\"icon\": \"media:icon\","
                "\"label\": \"string:EntryAbility_label\","
                "\"startWindowIcon\": \"media:icon\","
                "\"startWindowBackground\": \"color:start_window_background\","
                "\"exported\": true,"
                "\"skills\": ["
                    "{"
                        "\"entities\": [\"entity.system.home\"],"
                        "\"actions\": [\"action.system.home\"]"
                    "}"
                "],"
                "\"descriptionId\": 16777218,"
                "\"iconId\": 16777222,"
                "\"labelId\": 16777219,"
                "\"startWindowIconId\": 16777222,"
                "\"startWindowBackgroundId\": 16777221,"
                "\"continueType\":[]"
            "}"
        "],"
        "\"virtualMachine\": \"test_virtualMachine\","
        "\"compileMode\": \"esmodule\","
        "\"dependencies\": ["
            "{"
                "\"bundleName\": \"test_modules_dependency_1\","
                "\"moduleName\": \"entry_1\""
            "},"
            "{"
                "\"bundleName\": \"test_modules_dependency_2\","
                "\"moduleName\": \"entry_1\""
            "}"
        "],"
        "\"descriptionId\": 16777220,"
        "\"distro\": {"
            "\"installationFree\": false,"
            "\"moduleType\": \"entry\","
            "\"moduleName\": \"test_module_name\""
        "},"
        "\"preloads\": ["
            "{"
                "\"name\": \"test_name_1\","
                "\"moduleName\": \"test_module_name_1\""
            "}"
        "],"
        "\"package\": \"test_package\","
        "\"deviceType\": ["
            "\"default\","
            "\"tablet\""
        "],"
        "\"targetModuleName\": \"test_module_targetBundleName\","
        "\"targetPriority\": 6,"
        "\"proxyDatas\": ["
            "{"
                 "\"uri\": \"test_uri1\""
            "},"
            "{"
                 "\"uri\": \"test_uri2\""
            "}"
        "],"
        "\"atomicService\": {"
            "\"preloads\": ["
                "{"
                    "\"atomicServiceObj\": \"test_atomicService\""
                "}"
            "]"
        
        "},"
        "\"metadata\": ["
            "{"
                "\"name\": \"test_metadata\","
                "\"value\": \"test_value\","
                "\"resource\": \"test_resource\""
            "}"
        "],"
        "\"extensionAbilities\": ["
            "{"
                "\"name\": \"test_extension_abilities\""
            "}"
        "]"
    "},"
    "\"deviceConfig\": {"
        "\"default\": {\"debug\": true}"
    "}"
"}";
}

class ModuleJsonStageTest : public testing::Test {
public:
    ModuleJsonStageTest() {}
    virtual ~ModuleJsonStageTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void ModuleJsonStageTest::SetUpTestCase() {}

void ModuleJsonStageTest::TearDownTestCase() {}

void ModuleJsonStageTest::SetUp() {}

void ModuleJsonStageTest::TearDown() {}

/*
 * @tc.name: SetStageVersionCode
 * @tc.desc: test set stage version code
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageVersionCode_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    int32_t versionCode = 99;
    EXPECT_FALSE(moduleJson.SetStageVersionCode(versionCode));
}

/*
 * @tc.name: SetStageVersionName
 * @tc.desc: test set stage version name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageVersionName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string versionName = "1.0.0";
    EXPECT_FALSE(moduleJson.SetStageVersionName(versionName));
}

/*
 * @tc.name: GetStageVersionByAppObj
 * @tc.desc: test get stage version by obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    moduleJson.GetAppObject(appObj);
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersionByAppObj(appObj, version));
}

/*
 * @tc.name: GetStageVersionByAppObj
 * @tc.desc: test get stage version by obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageVersionByAppObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    moduleJson.GetAppObject(appObj);
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersionByAppObj(appObj, version));
}

/*
 * @tc.name: GetStageVersionByAppObj
 * @tc.desc: test get stage version by obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageVersionByAppObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    moduleJson.GetAppObject(appObj);
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersionByAppObj(appObj, version));
}

/*
 * @tc.name: GetStageVersionByAppObj
 * @tc.desc: test get stage version by obj
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageVersionByAppObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    moduleJson.GetAppObject(appObj);
    Version version;
    EXPECT_FALSE(moduleJson.GetStageVersionByAppObj(appObj, version));
}

/*
 * @tc.name: GetStageInstallationFree
 * @tc.desc: test get stage installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageInstallationFree_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    bool installationFree = true;
    EXPECT_FALSE(moduleJson.GetStageInstallationFree(installationFree));
    EXPECT_EQ(installationFree, true);
}

/*
 * @tc.name: GetStageInstallationFreeByModuleObj
 * @tc.desc: test get stage installation free
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageInstallationFreeByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    bool installationFree = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_FALSE(moduleJson.GetStageInstallationFreeByModuleObj(moduleObj, installationFree));
    EXPECT_EQ(installationFree, false);
}

/*
 * @tc.name: GetStageModuleApiVersion
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleApiVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    ModuleApiVersion moduleApiVersion;
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersion(moduleApiVersion));
}

/*
 * @tc.name: GetStageModuleApiVersionByAppObj
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleApiVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion));
}

/*
 * @tc.name: GetStageModuleApiVersionByAppObj
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleApiVersionByAppObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion));
}

/*
 * @tc.name: GetStageModuleApiVersionByAppObj
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleApiVersionByAppObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion));
}

/*
 * @tc.name: GetStageModuleApiVersionByAppObj
 * @tc.desc: test get stage module api version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleApiVersionByAppObj_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    ModuleApiVersion moduleApiVersion;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageModuleApiVersionByAppObj(appObj, moduleApiVersion));
}

/*
 * @tc.name: GetStageModuleName
 * @tc.desc: test get stage module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string stageModuleName = "";
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageModuleName(stageModuleName));
}

/*
 * @tc.name: GetStageModuleNameByModuleObj
 * @tc.desc: test get stage module name
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleNameByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string stageModuleName = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_FALSE(moduleJson.GetStageModuleNameByModuleObj(moduleObj, stageModuleName));
}

/*
 * @tc.name: GetStageCompileSdkTypeByAppObj
 * @tc.desc: test get stage compile sdk type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageCompileSdkTypeByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string compileSdkType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageCompileSdkTypeByAppObj(appObj, compileSdkType));
}

/*
 * @tc.name: GetStageCompileSdkVersionByAppObj
 * @tc.desc: test get stage compile sdk version
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageCompileSdkVersionByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string compileSdkVersion = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageCompileSdkVersionByAppObj(appObj, compileSdkVersion));
}

/*
 * @tc.name: GetStageDebugByAppObj
 * @tc.desc: test get stage debug
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDebugByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool debug = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageDebugByAppObj(appObj, debug));
    EXPECT_EQ(debug, true);
}

/*
 * @tc.name: GetStageEntry
 * @tc.desc: test get stage entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageEntry_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.GetStageEntry(deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageEntry
 * @tc.desc: test get stage entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageEntry_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageEntry(deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageEntry
 * @tc.desc: test get stage entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageEntry_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageEntry(deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageEntry
 * @tc.desc: test get stage entry
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageEntry_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageEntry(deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageDeviceTypes
 * @tc.desc: test get stage device types
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDeviceTypes_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageDeviceTypes(deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageDeviceTypesByModuleObj
 * @tc.desc: test get stage device types
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDeviceTypesByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageDeviceTypesByModuleObj
 * @tc.desc: test get stage device types
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDeviceTypesByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageDeviceTypesByModuleObj(moduleObj, deviceTypes));
    EXPECT_EQ(deviceTypes.size(), 0);
}

/*
 * @tc.name: GetStageDistroFilter
 * @tc.desc: test get stage distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDistroFilter_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    DistroFilter distroFilter;
    map<std::string, std::string> resourceMap;
    EXPECT_FALSE(moduleJson.GetStageDistroFilter(distroFilter, resourceMap));
}

/*
 * @tc.name: GetStageDistroFilterByModuleObj
 * @tc.desc: test get stage distro filter
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageDistroFilterByModuleObj_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    map<std::string, std::string> resourceMap;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    resourceMap.insert((make_pair("test_resource.json", str)));
    DistroFilter distroFilter;
    EXPECT_FALSE(moduleJson.GetStageDistroFilterByModuleObj(moduleObj, resourceMap, distroFilter));
}

/*
 * @tc.name: GetStageModuleType
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string moduleType = "";
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageModuleType(moduleType));
    EXPECT_EQ(moduleType.size(), 0);
}

/*
 * @tc.name: GetStageModuleTypeByModuleObj
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleTypeByModuleObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string moduleType = "";
    EXPECT_FALSE(moduleJson.GetStageModuleTypeByModuleObj(moduleObj, moduleType));
}

/*
 * @tc.name: GetStageModuleTypeByModuleObj
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleTypeByModuleObj_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string moduleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONNAME_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_FALSE(moduleJson.GetStageModuleTypeByModuleObj(moduleObj, moduleType));
}

/*
 * @tc.name: GetStageModuleTypeByModuleObj
 * @tc.desc: test get stage module type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageModuleTypeByModuleObj_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> moduleObj = nullptr;
    std::string moduleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetModuleObject(moduleObj));
    EXPECT_TRUE(moduleJson.GetStageModuleTypeByModuleObj(moduleObj, moduleType));
    EXPECT_STREQ(moduleType.c_str(), "");
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageBundleType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "");
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageBundleType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_ERROR));
    EXPECT_FALSE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "");
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageBundleType_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "");
}

/*
 * @tc.name: GetStageAsanEnabled
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageAsanEnabled_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool asanEnabled = true;
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageAsanEnabled(asanEnabled));
    EXPECT_EQ(asanEnabled, true);
}

/*
 * @tc.name: GetStageAsanEnabledByAppObj
 * @tc.desc: test get stage asan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageAsanEnabledByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool asanEnabled = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageAsanEnabledByAppObj(appObj, asanEnabled));
    EXPECT_EQ(asanEnabled, true);
}

/*
 * @tc.name: GetStageTsanEnabled
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageTsanEnabled_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool tsanEnabled = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.GetStageTsanEnabled(tsanEnabled));
    EXPECT_EQ(tsanEnabled, true);
}

/*
 * @tc.name: GetStageTsanEnabledByAppObj
 * @tc.desc: test get stage tsan enabled
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageTsanEnabledByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool tsanEnabled = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageTsanEnabledByAppObj(appObj, tsanEnabled));
    EXPECT_EQ(tsanEnabled, true);
}

/*
 * @tc.name: GetStageCompressNativeLibs
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageCompressNativeLibs_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool compressNativeLibs = true;
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageCompressNativeLibs(compressNativeLibs));
    EXPECT_EQ(compressNativeLibs, true);
}

/*
 * @tc.name: GetStageCompressNativeLibsByAppObj
 * @tc.desc: test get stage compress native libs
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageCompressNativeLibsByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    bool compressNativeLibs = true;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    EXPECT_FALSE(moduleJson.GetStageCompressNativeLibsByAppObj(appObj, compressNativeLibs));
}

/*
 * @tc.name: GetStageApiReleaseType
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageApiReleaseType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "";
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageApiReleaseType(apiReleaseType));
    EXPECT_STREQ(apiReleaseType.c_str(), "");
    EXPECT_EQ(apiReleaseType.size(), 0);
}

/*
 * @tc.name: GetStageApiReleaseTypeByAppObj
 * @tc.desc: test get stage api release type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageApiReleaseTypeByAppObj_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::unique_ptr<PtJson> appObj = nullptr;
    std::string apiReleaseType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageApiReleaseTypeByAppObj(appObj, apiReleaseType));
    EXPECT_STREQ(apiReleaseType.c_str(), "");
    EXPECT_EQ(apiReleaseType.size(), 0);
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0600, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0700, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_MINIVERSIONCODE_TEST_STRING));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0900, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING_NOMODULENAME));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_1000, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING_NOPROXY_DATAS));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageHapVerifyInfo
 * @tc.desc: test get stage hap verify info
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageHapVerifyInfo_0800, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    string str = "{"
                 "\"distributionFilter\": {"
                 "}"
                 "}";
    HapVerifyInfo hapVerifyInfo;
    map<std::string, std::string> resourceMap;
    resourceMap.insert((make_pair("test_resource.json", str)));
    hapVerifyInfo.SetResourceMap(resourceMap);
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING_NOVENDOR));
    EXPECT_FALSE(moduleJson.GetStageHapVerifyInfo(hapVerifyInfo));
}

/*
 * @tc.name: GetStageBundleType
 * @tc.desc: test get stage bundle type
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, GetStageBundleType_0500, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.GetStageBundleType(bundleType));
    EXPECT_STREQ(bundleType.c_str(), "atomicService");
}

/*
 * @tc.name: SetStageBundleName_0100
 * @tc.desc: SetStageBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleName_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageBundleName(bundleType));
}

/*
 * @tc.name: SetStageBundleName_0200
 * @tc.desc: SetStageBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleName_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageBundleName(bundleType));
}

/*
 * @tc.name: SetStageBundleName_0300
 * @tc.desc: SetStageBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleName_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageBundleName(bundleType));
}

/*
 * @tc.name: SetStageBundleName_0400
 * @tc.desc: SetStageBundleName
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleName_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "app";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageBundleName(bundleType));
}

/*
 * @tc.name: SetStageMinCompatibleVersionCode_0100
 * @tc.desc: SetStageMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinCompatibleVersionCode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetStageMinCompatibleVersionCode_0200
 * @tc.desc: SetStageMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinCompatibleVersionCode_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetStageMinCompatibleVersionCode_0300
 * @tc.desc: SetStageMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinCompatibleVersionCode_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetStageMinCompatibleVersionCode_0400
 * @tc.desc: SetStageMinCompatibleVersionCode
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinCompatibleVersionCode_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minCompatibleVersionCode = 99;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinCompatibleVersionCode(minCompatibleVersionCode));
}

/*
 * @tc.name: SetStageMinAPIVersion_0100
 * @tc.desc: SetStageMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinAPIVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetStageMinAPIVersion_0200
 * @tc.desc: SetStageMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinAPIVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetStageMinAPIVersion_0300
 * @tc.desc: SetStageMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinAPIVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetStageMinAPIVersion_0400
 * @tc.desc: SetStageMinAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageMinAPIVersion_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t minAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(minAPIVersion));
}

/*
 * @tc.name: SetStageTargetAPIVersion_0100
 * @tc.desc: SetStageTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageTargetAPIVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetStageTargetAPIVersion_0200
 * @tc.desc: SetStageTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageTargetAPIVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(targetAPIVersion));
}


/*
 * @tc.name: SetStageTargetAPIVersion_0300
 * @tc.desc: SetStageTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageTargetAPIVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = -1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetStageTargetAPIVersion_0400
 * @tc.desc: SetStageTargetAPIVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageTargetAPIVersion_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    int32_t targetAPIVersion = 1;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageMinAPIVersion(targetAPIVersion));
}

/*
 * @tc.name: SetStageApiReleaseType_0100
 * @tc.desc: SetStageApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageApiReleaseType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetStageApiReleaseType_0200
 * @tc.desc: SetStageApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageApiReleaseType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetStageApiReleaseType_0300
 * @tc.desc: SetStageApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageApiReleaseType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetStageApiReleaseType_0400
 * @tc.desc: SetStageApiReleaseType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageApiReleaseType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string apiReleaseType = "Canary";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageApiReleaseType(apiReleaseType));
}

/*
 * @tc.name: SetStageBundleType_0100
 * @tc.desc: SetStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleType_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageBundleType(bundleType));
}

/*
 * @tc.name: SetStageBundleType_0200
 * @tc.desc: SetStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleType_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageBundleType(bundleType));
}

/*
 * @tc.name: SetStageBundleType_0300
 * @tc.desc: SetStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleType_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageBundleType(bundleType));
}

/*
 * @tc.name: SetStageBundleType_0400
 * @tc.desc: SetStageBundleType
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageBundleType_0400, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::string bundleType = "test_bundle_type";
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_ONLYAPP));
    EXPECT_TRUE(moduleJson.SetStageBundleType(bundleType));
}

/*
 * @tc.name: SetStageInstallationFree_0100
 * @tc.desc: SetStageInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageInstallationFree_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageInstallationFree(installationFree));
}

/*
 * @tc.name: SetStageInstallationFree_0200
 * @tc.desc: SetStageInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageInstallationFree_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageInstallationFree(installationFree));
}

/*
 * @tc.name: SetStageInstallationFree_0300
 * @tc.desc: SetStageInstallationFree
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageInstallationFree_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool installationFree = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_TRUE(moduleJson.SetStageInstallationFree(installationFree));
}

/*
 * @tc.name: SetStageDeliveryWithInstall_0100
 * @tc.desc: SetStageDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeliveryWithInstall_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetStageDeliveryWithInstall_0200
 * @tc.desc: SetStageDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeliveryWithInstall_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetStageDeliveryWithInstall_0300
 * @tc.desc: SetStageDeliveryWithInstall
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeliveryWithInstall_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    bool deliveryWithInstall = false;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_NOMODULETYPE));
    EXPECT_TRUE(moduleJson.SetStageDeliveryWithInstall(deliveryWithInstall));
}

/*
 * @tc.name: SetStageDeviceTypes_0100
 * @tc.desc: SetStageDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeviceTypes_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING_NOTHING));
    EXPECT_FALSE(moduleJson.SetStageDeviceTypes(deviceTypes));
}

/*
 * @tc.name: SetStageDeviceTypes_0200
 * @tc.desc: SetStageDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeviceTypes_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    EXPECT_TRUE(moduleJson.SetStageDeviceTypes(deviceTypes));
}

/*
 * @tc.name: SetStageDeviceTypes_0300
 * @tc.desc: SetStageDeviceTypes
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetStageDeviceTypes_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    std::list<std::string> deviceTypes = {"default", "tablet"};
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_STRING));
    EXPECT_TRUE(moduleJson.SetStageDeviceTypes(deviceTypes));
}

/*
 * @tc.name: SetBuildVersion_0100
 * @tc.desc: test set stage buildVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetBuildVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_TEST_STRING));
    std::string buildVersion = "1.0.0";
    EXPECT_FALSE(moduleJson.SetBuildVersion(buildVersion));
}

/*
 * @tc.name: SetBuildVersion_0200
 * @tc.desc: test set stage buildVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetBuildVersion_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_VERSIONBYAPPOBJ_VERSIONCODE_TEST_STRING));
    std::string buildVersion = "1.0.0";
    EXPECT_FALSE(moduleJson.SetBuildVersion(buildVersion));
}

/*
 * @tc.name: SetBuildVersion_0300
 * @tc.desc: test set stage buildVersion
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ModuleJsonStageTest, SetBuildVersion_0300, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleJson moduleJson;
    moduleJson.Release();
    EXPECT_TRUE(moduleJson.ParseFromString(MODULE_JSON_BUILDVERSION_TEST_STRING));
    std::string buildVersion = "1.0.0";
    EXPECT_TRUE(moduleJson.SetBuildVersion(buildVersion));
}
}