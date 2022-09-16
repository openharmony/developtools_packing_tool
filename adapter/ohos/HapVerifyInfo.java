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

/**
 * collection of HQF verify info.
 */
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
    private String moduleType = "";

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

    /**
     * get bundle name form HapVerifyInfo.
     */
    public String getBundleName() {
        return bundleName;
    }

    /**
     * set bundle name for HapVerifyInfo.
     */
    public void setBundleName(String bundleName) {
        this.bundleName = bundleName;
    }

    /**
     * get vendor form HapVerifyInfo.
     */
    public String getVendor() {
        return vendor;
    }

    /**
     * set vendor for HapVerifyInfo.
     */
    public void setVendor(String vendor) {
        this.vendor = vendor;
    }

    /**
     * get version from HapVerifyInfo.
     */
    public Version getVersion() {
        return version;
    }

    /**
     * set version for HapVerifyInfo.
     */
    public void setVersion(Version version) {
        this.version = version;
    }

    /**
     * get apiVersion from HapVerifyInfo.
     */
    public ModuleApiVersion getApiVersion() {
        return apiVersion;
    }

    /**
     * set apiVersion for HapVerifyInfo.
     */
    public void setApiVersion(ModuleApiVersion apiVersion) {
        this.apiVersion = apiVersion;
    }

    /**
     * get module name from HapVerifyInfo.
     */
    public String getModuleName() {
        return moduleName;
    }

    /**
     * set module name for HapVerifyInfo.
     */
    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    /**
     * get package name from HapVerifyInfo.
     */
    public String getPackageName() {
        return packageName;
    }

    /**
     * set package name for HapVerifyInfo.
     */
    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    /**
     * get ability names from HapVerifyInfo.
     */
    public List<String> getAbilityNames() {
        return abilityNames;
    }

    /**
     * set abilityNames for HapVerifyInfo.
     */
    public void setAbilityNames(List<String> abilityNames) {
        this.abilityNames = abilityNames;
    }

    /**
     * add abilityNames for HapVerifyInfo.
     */
    public void addAbilityNames(List<String> nameList) {
        this.abilityNames.addAll(nameList);
    }

    /**
     * get distroFilter from HapVerifyInfo.
     */
    public DistroFilter getDistroFilter() {
        return distroFilter;
    }

    /**
     * set distroFilter for HapVerifyInfo.
     */
    public void setDistroFilter(DistroFilter distroFilter) {
        this.distroFilter = distroFilter;
    }

    /**
     * get deviceType from HapVerifyInfo.
     */
    public List<String> getDeviceType() {
        return deviceType;
    }

    /**
     * set deviceType for HapVerifyInfo.
     */
    public void setDeviceType(List<String> deviceType) {
        this.deviceType = deviceType;
    }

    /**
     * get isStageModule from HapVerifyInfo.
     */
    public boolean isStageModule() {
        return isStageModule;
    }

    /**
     * set isStageModule for HapVerifyInfo.
     */
    public void setStageModule(boolean isStageModule) {
        this.isStageModule = isStageModule;
    }

    /**
     * get moduleType from HapVerifyInfo.
     */
    public String getModuleType() {
        return moduleType;
    }

    /**
     * set is module for HapVerifyInfo.
     */
    public void setModuleType(String moduleType) {
        this.moduleType = moduleType;
    }

    /**
     * get isInstallationFree form HapVerifyInfo.
     */
    public boolean isInstallationFree() {
        return isInstallationFree;
    }

    /**
     * set isInstallationFree for HapVerifyInfo.
     */
    public void setInstallationFree(boolean isInstallationFree) {
        this.isInstallationFree = isInstallationFree;
    }

    /**
     * get dependency form HapVerifyInfo.
     */
    public List<String> getDependencies() {
        return dependencies;
    }

    /**
     * set dependency for HapVerifyInfo.
     */
    public void setDependencies(List<String> dependencies) {
        this.dependencies = dependencies;
    }

    /**
     * get json file string form HapVerifyInfo.
     */
    public String getProfileStr() {
        return profileStr;
    }

    /**
     * set json file string for HapVerifyInfo.
     */
    public void setProfileStr(String profileStr) {
        this.profileStr = profileStr;
    }

    /**
     * get resource map form HapVerifyInfo.
     */
    public HashMap<String, String> getResourceMap() {
        return resourceMap;
    }

    /**
     * set resource map for HapVerifyInfo.
     */
    public void setResourceMap(HashMap<String, String> resourceMap) {
        this.resourceMap = resourceMap;
    }
}
