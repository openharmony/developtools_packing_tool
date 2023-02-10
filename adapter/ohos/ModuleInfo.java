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
 * Module ModuleJson info.
 *
 */
class ModuleInfo {
    private final Integer INSTALL_FREE_UNDEFINED = 2;
    private final String DEFAULT = "default";
    private final String HML = "hml";
    /**
     * Indicates the hap name of ModuleInfo.
     */
    public String hapName = "";
    /**
     * Indicates the name of ModuleInfo.
     */
    public String name = "";
    /**
     * Indicates the type of ModuleInfo.
     */
    public String type = "";
    /**
     * Indicates the srcEntrance of ModuleInfo.
     */
    public String srcEntrance = "";
    /**
     * Indicates the description of ModuleInfo.
     */
    public String description = "";

    /**
     * Indicates the descriptions of ModuleInfo, for Multilingual.
     */
    private HashMap<String, String> descriptions = new HashMap<>();

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    /**
     * Indicates the process of ModuleInfo.
     */
    public String process = "";
    /**
     * Indicates the mainElement of ModuleInfo.
     */
    public String mainElement = "";
    /**
     * Indicates the devicesTypes of ModuleInfo.
     */
    public List<String> deviceTypes = new ArrayList<String>();
    /**
     * Indicates the deliveryWithInstall of ModuleInfo.
     */
    public boolean deliveryWithInstall = true;
    /**
     * Indicates the installationFree of ModuleInfo.
     */
    public int installationFree = INSTALL_FREE_UNDEFINED;
    /**
     * Indicates the virtualMachine of ModuleInfo.
     */
    public String virtualMachine = DEFAULT;
    /**
     * Indicates the uiSyntax of ModuleInfo.
     */
    public String uiSyntax = HML;
    /**
     * Indicates the pages of ModuleInfo.
     */
    public List<String> pages = new ArrayList<String>();
    /**
     * Indicates the metadata of ModuleInfo.
     */
    public List<ModuleMetadataInfo> moduleMetadataInfos = new ArrayList<ModuleMetadataInfo>();
    /**
     * Indicates the abilities of ModuleInfo.
     */
    public List<ModuleAbilityInfo> abilities = new ArrayList<ModuleAbilityInfo>();
    /**
     * Indicates the extensionAbilities of ModuleInfo.
     */
    public List<ExtensionAbilityInfo> extensionAbilityInfos = new ArrayList<ExtensionAbilityInfo>();
    /**
     * Indicates the requestPermissions of ModuleInfo.
     */
    public List<ReqPermission> requestPermissions = new ArrayList<ReqPermission>();
    /**
     * Indicates the shortcuts of ModuleInfo.
     */
    public List<ModuleShortcut> moduleShortcuts = new ArrayList<>();
    /**
     * Indicates the distrofilter of ModuleInfo.
     */
    public DistroFilter distroFilter = new DistroFilter();
    /**
     * Indicates the common events of ModuleInfo.
     */
    public List<CommonEvent> commonEvents = new ArrayList<CommonEvent>();
    /**
     * Indicates the common events of ModuleInfo.
     */
    List<AbilityFormInfo> abilityFormInfos = new ArrayList<>();
    /**
     * Indicates the define permissions of ModuleInfo.
     */
    List<DefinePermission> definePermissions = new ArrayList<>();
    /**
     * Indicates the module atomic service of ModuleInfo.
     */
    public ModuleAtomicService moduleAtomicService = new ModuleAtomicService();
}
