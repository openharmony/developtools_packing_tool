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

public class HQFVerifyInfo {
    private String bundleName = "";

    private int versionCode = -1;

    private String versionName = "";

    private int patchVersionCode = -1;

    private String patchVersionName = "";

    private String moduleName = "";

    private String type = "";

    private List<String> deviceTypes = new ArrayList<>();

    private String originalModuleHash = "";

    public String getBundleName() {
        return bundleName;
    }

    public void setBundleName(String bundleName) {
        this.bundleName = bundleName;
    }

    public int getVersionCode() {
        return versionCode;
    }

    public void setVersionCode(int versionCode) {
        this.versionCode = versionCode;
    }

    public String getVersionName() {
        return versionName;
    }

    public void setVersionName(String versionName) {
        this.versionName = versionName;
    }

    public int getPatchVersionCode() {
        return patchVersionCode;
    }

    public void setPatchVersionCode(int patchVersionCode) {
        this.patchVersionCode = patchVersionCode;
    }

    public String getPatchVersionName() {
        return patchVersionName;
    }

    public void setPatchVersionName(String patchVersionName) {
        this.patchVersionName = patchVersionName;
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public List<String> getDeviceTypes() {
        return deviceTypes;
    }

    public void setDeviceTypes(List<String> deviceTypes) {
        this.deviceTypes = deviceTypes;
    }

    public String getOriginalModuleHash() {
        return originalModuleHash;
    }

    public void setOriginalModuleHash(String originalModuleHash) {
        this.originalModuleHash = originalModuleHash;
    }
}
