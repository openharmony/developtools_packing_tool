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
    public String bundleName = "";
    /**
     * Indicates the vendor of module.
     */
    public String vendor = "";
    /**
     * Indicates the version of module.
     */
    public Version version = new Version();
    /**
     * Indicates the apiVersion of module.
     */
    public ModuleApiVersion apiVersion = new ModuleApiVersion();
    /**
     * Indicates the moduleName of module.
     */
    public String moduleName = "";
    /**
     * Indicates the package name of module.
     */
    public String packageName = "";
    /**
     * Indicates the ability names of module.
     */
    public List<String> abilityNames = new ArrayList<>();
    /**
     * Indicates the distrofilter of module.
     */
    public DistroFilter distroFilter = new DistroFilter();
    /**
     * Indicates the deviceType of module.
     */
    public List<String> deviceType = new ArrayList<>();
    /**
     * Indicates is stage of module.
     */
    public boolean isStageModule = true;
    /**
     * Indicates is entry of module.
     */
    public boolean isEntry = false;
    /**
     * Indicates dependency of module.
     */
    public List<String> dependencies = new ArrayList<>();
    /**
     * Indicates is config.json string or module.json string of module.
     */
    public String profileStr = "";
    /**
     * Indicates is file in profile of module.
     */
    public HashMap<String, String> resourceMap = new HashMap<>();
}
