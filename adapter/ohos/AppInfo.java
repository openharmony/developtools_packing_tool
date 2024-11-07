/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

/**
 * App AppInfo info.
 *
 */
public class AppInfo {
    /**
     * Indicates the bundleName of app AppInfo.
     */
    public String bundleName = "";

    /**
     * Indicates the vendor of app AppInfo.
     */
    public String vendor = "";

    /**
     * Indicates the relatedBundleName of app AppInfo.
     */
    public String relatedBundleName = "";

    /**
     * Indicates the versionName of app AppInfo.
     */
    public String versionName = "";

    /**
     * Indicates the versionCode of app AppInfo.
     */
    public String versionCode = "";

    /**
     * Indicates the target Api Version of app AppInfo.
     */
    public int targetApiVersion = 1;

    /**
     * Indicates the compatible Api Version of app AppInfo.
     */
    public int compatibleApiVersion = 1;

    /**
     * Indicates the appName of app AppInfo.
     */
    public String appName = "";

    /**
     * Indicates the appNameEN of app AppInfo.
     */
    public String appNameEN = "";

    /**
     * Indicates the releaseType of app AppInfo.
     */
    public String releaseType = "";

    private String shellVersionCode = "";
    private String shellVersionName = "";
    private boolean multiFrameworkBundle;

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
     * Indicates the description of app AppJson.
     */
    public String description = "";

    /**
     * Indicates the minCompatibleVersionCode of app AppJson.
     */
    public int minCompatibleVersionCode = -1;

    /**
     * Indicates the distributedNotificationEnabled of app AppJson.
     */
    public boolean distributedNotificationEnabled = false;

    /**
     * Indicates the type of bundle.
     */
    private String bundleType = "app";

    /**
     * Indicates the sdk version the app was compiled.
     */
    private String compileSdkVersion = "";

    /**
     * Indicates the sdk type the app was compiled.
     */
    private String compileSdkType = "";

    /**
     * Indicates the targetBundleName of app AppJson.
     */
    private String targetBundleName = "";

    /**
     * Indicates the targetPriority of app AppJson.
     */
    private int targetPriority = 0;

    /**
     * Indicates the labels of app AppJson, for multilingual.
     */
    private HashMap<String, String> labels = new HashMap<>();

    /**
     * Indicates the descriptions of app AppJson, for multilingual.
     */
    private HashMap<String, String> descriptions = new HashMap<>();

    /**
     * Indicates the iconId of app AppJson.
     */
    private int iconId = 0;

    /**
     * Indicates the labelId of app AppJson.
     */
    private int labelId = 0;

    /**
     * Get shell version code.
     *
     * @return shell version code
     */
    public String getShellVersionCode() {
        return shellVersionCode;
    }

    /**
     * Set shell version code.
     *
     * @param shellVersionCode Indicates the shell version name
     */
    public void setShellVersionCode(String shellVersionCode) {
        this.shellVersionCode = shellVersionCode;
    }

    /**
     * Get shell version name.
     *
     * @return shell version name
     */
    public String getShellVersionName() {
        return shellVersionName;
    }

    /**
     * Get bundle type.
     *
     * @return bundle type
     */
    public String getBundleType() {
        return bundleType;
    }

    /**
     * Set shell version name.
     *
     * @param shellVersionName Indicates the shell version name
     */
    public void setShellVersionName(String shellVersionName) {
        this.shellVersionName = shellVersionName;
    }

    /**
     * Whether the app is multi framework bundle.
     *
     * @return true if the app is a multi framework bundle.
     */
    public boolean isMultiFrameworkBundle() {
        return multiFrameworkBundle;
    }

    /**
     * Set multi framework bundle.
     *
     * @param multiFrameworkBundle Indicates the app type
     */
    public void setMultiFrameworkBundle(boolean multiFrameworkBundle) {
        this.multiFrameworkBundle = multiFrameworkBundle;
    }

    /**
     * Set default shell version
     */
    public void setDefaultShellVersion() {
        this.shellVersionCode = versionCode;
        this.shellVersionName = versionName;
    }

    /**
     * Set bundle type
     *
     * @param type bundle type
     */
    public void setBundleType(String type) {
        bundleType = type;
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    public void setLabels(HashMap<String, String> labels) {
        this.labels = labels;
    }

    public HashMap<String, String> getLabels() {
        return labels;
    }

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    public String getCompileSdkVersion() {
        return compileSdkVersion;
    }

    public void setCompileSdkVersion(String compileSdkVersion) {
        this.compileSdkVersion = compileSdkVersion;
    }

    public String getCompileSdkType() {
        return compileSdkType;
    }

    public void setCompileSdkType(String compileSdkType) {
        this.compileSdkType = compileSdkType;
    }

    public String getTargetBundleName() {
        return targetBundleName;
    }

    public void setTargetBundleName(String targetBundleName) {
        this.targetBundleName = targetBundleName;
    }

    public int getTargetPriority() {
        return targetPriority;
    }

    public void setTargetPriority(int targetPriority) {
        this.targetPriority = targetPriority;
    }

    public int getIconId() {
        return iconId;
    }

    public void setIconId(int iconId) {
        this.iconId = iconId;
    }

    public int getLabelId() {
        return labelId;
    }

    public void setLabelId(int labelId) {
        this.labelId = labelId;
    }
}
