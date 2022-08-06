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
import java.util.HashMap;
import java.util.List;

class HapVerifyInfo {
    /**
     * Indicates the bundleName of module.
     */
    private String bundleName = "";

    /**
     * Indicates the vendor of module.
     */
    private String vendor = "";

    /**
     * Indicates the version of module.
     */
    private Version version = new Version();

    /**
     * Indicates the apiVersion of module.
     */
    private ModuleApiVersion apiVersion = new ModuleApiVersion();

    /**
     * Indicates the moduleName of module.
     */
    private String moduleName = "";

    /**
     * Indicates the package name of module.
     */
    private String packageName = "";

    /**
     * Indicates the ability names of module.
     */
    private List<String> abilityNames = new ArrayList<>();

    /**
     * Indicates the distrofilter of module.
     */
    private DistroFilter distroFilter = new DistroFilter();

    /**
     * Indicates the deviceType of module.
     */
    private List<String> deviceType = new ArrayList<>();

    /**
     * Indicates is stage of module.
     */
    private boolean isStageModule = true;

    /**
     * Indicates is entry of module.
     */
    private boolean isEntry = false;

    /**
     * Indicates is installationFree of module.
     */
    private boolean isInstallationFree = false;

    /**
     * Indicates dependency of module.
     */
    private List<String> dependencies = new ArrayList<>();

    /**
     * Indicates is config.json string or module.json string of module.
     */
    private String profileStr = "";

    /**
     * Indicates is file in profile of module.
     */
    private HashMap<String, String> resourceMap = new HashMap<>();

    public String getBundleName() {
        return bundleName;
    }

    public void setBundleName(String bundleName) {
        this.bundleName = bundleName;
    }

    public String getVendor() {
        return vendor;
    }

    public void setVendor(String vendor) {
        this.vendor = vendor;
    }

    public Version getVersion() {
        return version;
    }

    public void setVersion(Version version) {
        this.version = version;
    }

    public ModuleApiVersion getApiVersion() {
        return apiVersion;
    }

    public void setApiVersion(ModuleApiVersion apiVersion) {
        this.apiVersion = apiVersion;
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public List<String> getAbilityNames() {
        return abilityNames;
    }

    public void setAbilityNames(List<String> abilityNames) {
        this.abilityNames = abilityNames;
    }

    public void addAbilityNames(List<String> nameList) {
        this.abilityNames.addAll(nameList);
    }

    public DistroFilter getDistroFilter() {
        return distroFilter;
    }

    public void setDistroFilter(DistroFilter distroFilter) {
        this.distroFilter = distroFilter;
    }

    public List<String> getDeviceType() {
        return deviceType;
    }

    public void setDeviceType(List<String> deviceType) {
        this.deviceType = deviceType;
    }

    public boolean isStageModule() {
        return isStageModule;
    }

    public void setStageModule(boolean stageModule) {
        isStageModule = stageModule;
    }

    public boolean isEntry() {
        return isEntry;
    }

    public void setEntry(boolean entry) {
        isEntry = entry;
    }

    public boolean isInstallationFree() {
        return isInstallationFree;
    }

    public void setInstallationFree(boolean installationFree) {
        this.isInstallationFree = installationFree;
    }

    public List<String> getDependencies() {
        return dependencies;
    }

    public void setDependencies(List<String> dependencies) {
        this.dependencies = dependencies;
    }

    public String getProfileStr() {
        return profileStr;
    }

    public void setProfileStr(String profileStr) {
        this.profileStr = profileStr;
    }

    public HashMap<String, String> getResourceMap() {
        return resourceMap;
    }

    public void setResourceMap(HashMap<String, String> resourceMap) {
        this.resourceMap = resourceMap;
    }
}
