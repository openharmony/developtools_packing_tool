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

import java.util.ArrayList;
import java.util.List;

/**
 * collection of members in app fields,
 * those members will be verified when pack app.
 */
class VerifyCollection {
    /**
     * Indicates the bundleName of app.
     */
    public String bundleName = "";
    /**
     * Indicates the type of bundle
     */
    public String bundleType = "app";
    /**
     * Indicates the vendor of app.
     */
    public String vendor = "";
    /**
     * Indicates the versionCode of version.
     */
    public int versionCode = -1;
    /**
     * Indicates the versionName of version.
     */
    public String versionName = "";
    /**
     * Indicates the minCompatibleVersionCode of app.
     */
    public int minCompatibleVersionCode = -1;
    /**
     * Indicates the minApiVersion of app.
     */
    public int compatibleApiVersion = -1;
    /**
     * Indicates the targetApiVersion of app.
     */
    public int targetApiVersion = -1;
    /**
     * Indicates the apiReleaseType of app.
     */
    public String releaseType = "";

    public String targetBundleName = "";

    public int targetPriority = 0;

    /**
     * Indicates the split of atomicService in app.
     */
    private boolean split = true;

    /**
     * Indicates the main of atomicService in app.
     */
    private String main = "";

    /**
     * Indicates whether the app type is Shared
     */
    private boolean sharedApp = false;
    /**
     * Indicates the moduleNames of app.
     */
    List<String> moduleNames = new ArrayList<>();
    /**
     * Indicates the packageNames of app.
     */
    List<String> packageNames = new ArrayList<>();

    public String getMain() {
        return main;
    }

    public void setMain(String main) {
        this.main = main;
    }

    public boolean isSplit() {
        return split;
    }

    public void setSplit(boolean split) {
        this.split = split;
    }

    public boolean isSharedApp() {
        return sharedApp;
    }

    public void setSharedApp(boolean sharedApp) {
        this.sharedApp = sharedApp;
    }
}
