/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
 * Hap info.
 *
 */
public class HapInfo {
    /**
     * Indicates hap is stage or FA.
     */
    public AppModel appModel = AppModel.FA;

    /**
     * Indicates the package of HapInfo.
     */
    public String packageStr = "";

    /**
     * Indicates the name of HapInfo.
     */
    public String name = "";

    /**
     * Indicates the description of HapInfo.
     */
    public String description = "";

    /**
     * Indicates the supportedModes of HapInfo.
     */
    public List<String> supportedModes = new ArrayList<String>();

    /**
     * Indicates the abilities of HapInfo.
     */
    public List<AbilityInfo> abilities = new ArrayList<AbilityInfo>();

    /**
     * Indicates the defPermissions of HapInfo.
     * @Deprecated
     */
    public List<DefPermission> defPermissions = new ArrayList<DefPermission>();

    /**
     * Indicates the definePermissions of HapInfo.
     * @Deprecated
     */
    public List<DefinePermission> definePermissions = new ArrayList<>();

    /**
     * Indicates the defPermissionsGroups of HapInfo.
     * @Deprecated
     */
    public List<DefPermissionGroup> defPermissionsGroups = new ArrayList<DefPermissionGroup>();

    /**
     * Indicates the distro of HapInfo.
     */
    public Distro distro = null;

    /**
     * Indicates the reqCapabilities of HapInfo.
     */
    public List<String> reqCapabilities = new ArrayList<String>();

    /**
     * Indicates the deviceType of HapInfo.
     */
    public List<String> deviceType = new ArrayList<String>();

    /**
     * Indicates the metaData of HapInfo.
     */
    public MetaData metaData = new MetaData();

    /**
     * Indicates the dependency config.
     */
    public List<DependencyItem> dependencies = new ArrayList<>();

    /**
     * Indicates proxyData config.
     */
    public List<ProxyDataItem> proxyData = new ArrayList<>();

    /**
     * Indicates the HapInfo is Js app.
     */
    public boolean isJs = false;

    /**
     * Indicates the reqPermissions of HapInfo.
     */
    public List<ReqPermission> reqPermissions = new ArrayList<ReqPermission>();

    /**
     * Indicates the commonEvents of HapInfo.
     */
    public List<CommonEvent> commonEvents = new ArrayList<CommonEvent>();

    /**
     * Indicates the shortcuts of HapInfo.
     */
    public List<Shortcut> shortcuts = new ArrayList<Shortcut>();

    /**
     * Indicates the DistroFilter of HapInfo
     */
    public DistroFilter distroFilter = new DistroFilter();

    // stage module character
    /**
     * Indicates the srcEntrance of ModuleInfo.
     */
    public String srcEntrance = "";

    /**
     * Indicates the process of ModuleInfo.
     */
    public String process = "";

    /**
     * Indicates the mainElement of ModuleInfo.
     */
    public String mainElement = "";

    /**
     * Indicates the uiSyntax of ModuleInfo.
     */
    public String uiSyntax = "hml";

    /**
     * Indicates the pages of ModuleInfo.
     */
    public List<String> pages = new ArrayList<>();

    /**
     * Indicates the extensionAbilityInfo of ModuleInfo.
     */
    public  List<ExtensionAbilityInfo> extensionAbilityInfos = new ArrayList<>();

    /**
     * Indicates the module atomic service of ModuleInfo.
     */
    public ModuleAtomicService moduleAtomicService = new ModuleAtomicService();

    /**
     * Indicates the form of module ModuleJson.
     */
    public List<AbilityFormInfo> formInfos = new ArrayList<>();

    /**
     * Indicates the compressed size of hap.
     */
    public long compressedSize = 0;

    /**
     * Indicates the original size of hap.
     */
    public long originalSize = 0;

    /**
     * Indicates the descriptions of HapInfo.
     */
    private HashMap<String, String> descriptions = new HashMap<>();

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    /**
     * get the customize Data value defined in this module.
     */
    public String getCustomizeDataValue(String customizeDataName) {
        for (CustomizeData data : metaData.customizeDatas) {
            if (customizeDataName.equals(data.name)) {
                return data.value;
            }
        }
        return "";
    }
}
