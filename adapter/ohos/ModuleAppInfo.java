/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

package ohos;

import java.util.HashMap;
import java.util.Map;

/**
 * ModuleJson AppJson info.
 *
 */
class ModuleAppInfo {
    private final Integer DEFAULT_VERSION_CODE = -1;
    private final String RELEASE = "Release";
    private final String UNSPECIFIED = "unspecified";
    /**
     * Indicates the bundleName of app AppJson.
     */
    public String bundleName = "";
    /**
     * Indicates the debug of app AppJson.
     */
    public boolean debug = false;
    /**
     * Indicates the icon of app AppJson.
     */
    public String icon = "";
    /**
     * Indicates the label of app AppJson.
     */
    public String label = "";
    /**
     * Indicates the labels of app AppJson, for multilingual.
     */
    public HashMap<String, String> labels;
    /**
     * Indicates the description of app AppJson.
     */
    public String description = "";
    /**
     * Indicates the descriptions of app AppJson, for multilingual.
     */
    public HashMap<String, String> descriptions;
    /**
     * Indicates the vendor of app AppJson.
     */
    public String vendor = "";
    /**
     * Indicates the versionCode of app AppJson.
     */
    public int versionCode = DEFAULT_VERSION_CODE;
    /**
     * Indicates the versionName of app AppJson.
     */
    public String versionName = "";
    /**
     * Indicates the minCompatibleVersionCode of app AppJson.
     */
    public int minCompatibleVersionCode = DEFAULT_VERSION_CODE;
    /**
     * Indicates the minAPIVersion of app AppJson.
     */
    public int minAPIVersion = DEFAULT_VERSION_CODE;
    /**
     * Indicates the targetAPIVersion of app AppJson.
     */
    public int targetAPIVersion = DEFAULT_VERSION_CODE;
    /**
     * Indicates the apiReleaseType of app AppJson.
     */
    public String apiReleaseType = RELEASE;
    /**
     * Indicates the distributedNotificationEnabled of app AppJson.
     */
    public boolean distributedNotificationEnabled = false;
    /**
     * Indicates the entityType of app AppJson.
     */
    public String entityType = UNSPECIFIED;
    /**
     * Indicates the appName of app AppJson.
     */
    public String appName = "";
    /**
     * Indicates the appNameEn of app AppJson.
     */
    public String appNameEN = "";
    /**
     * Indicates the deviceType of app AppJson.
     */
    public Map<String, ModuleDeviceType> deviceTypes = new HashMap<>();
}
