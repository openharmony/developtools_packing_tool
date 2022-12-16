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
 * ModuleJson extensionAbility info.
 *
 */
public class ExtensionAbilityInfo {
    /**
     * Indicates the name of module ModuleJson.
     */
    public String name = "";
    /**
     * Indicates the srcEntrance of module ModuleJson.
     */
    public String srcEntrance = "";
    /**
     * Indicates the icon of module ModuleJson.
     */
    public String icon = "";
    /**
     * Indicates the label of module ModuleJson.
     */
    public String label = "";
    /**
     * Indicates the labels of module ModuleJson, .
     */
    public HashMap<String, String> labels = new HashMap<>();
    /**
     * Indicates the description of module ModuleJson.
     */
    public String description = "";
    /**
     * Indicates the descriptions of module ModuleJson.
     */
    public HashMap<String, String> descriptions = new HashMap<>();
    /**
     * Indicates the type of module ModuleJson.
     */
    public String type = "";
    /**
     * Indicates the permissions of module ModuleJson.
     */
    public List<String> permissions = new ArrayList<>();
    /**
     * Indicates the readPermission of module ModuleJson.
     */
    public String readPermission = "";
    /**
     * Indicates the writePermission of module ModuleJson.
     */
    public String writePermission = "";
    /**
     * Indicates the visible of module ModuleJson.
     */
    public boolean visible = false;
    /**
     * Indicates the skill of module ModuleJson.
     */
    public List<SkillInfo> skills = new ArrayList<>();

    List<ModuleMetadataInfo> metadataInfos = new ArrayList<>();

    /**
     * Indicates the metadata of module ModuleJson.
     */
    public MetaData metadata = new MetaData();
    /**
     * Indicates the uri of module ModuleJson.
     */
    public String uri = "";
}
