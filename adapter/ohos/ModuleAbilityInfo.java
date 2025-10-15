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
 * ModuleJson ability info in module
 *
 */
class ModuleAbilityInfo {
    /**
     * Indicates the name of module ModuleJson.
     */
    public String name = "";

    /**
     * Indicates the srcEntrance of module ModuleJson.
     */
    public String srcEntrance = "";

    /**
     * Indicates the launchType of module ModuleJson.
     */
    public String launchType = "standard";

    /**
     * Indicates the description of module ModuleJson.
     */
    public String description = "";

    /**
     * Indicates the icon of module ModuleJson.
     */
    public String icon = "";

    /**
     * Indicates the label of module ModuleJson.
     */
    public String label = "";

    /**
     * Indicates the base label of module ModuleJson.
     */
    public String baseLabel = "";

    /**
     * Indicates the permissions of module ModuleJson.
     */
    public List<String> permissions = new ArrayList<String>();

    /**
     * Indicates the metadata of module ModuleJson.
     */
    public List<ModuleMetadataInfo> metadata = new ArrayList<ModuleMetadataInfo>();

    /**
     * Indicates the visible of module ModuleJson.
     */
    public boolean visible = false;

    /**
     * Indicates the continuable of module ModuleJson.
     */
    public boolean continuable = false;

    /**
     * Indicates the skills of module ModuleJson.
     */
    public List<SkillInfo> skills = new ArrayList<SkillInfo>();

    /**
     * Indicates the backgroundModes of module ModuleJson.
     */
    public List<String> backgroundModes = new ArrayList<String>();

    /**
     * Indicates the descriptions of module ModuleJson, for Multilingual.
     */
    private HashMap<String, String> descriptions = new HashMap<>();

    /**
     * Indicates the labels of module ModuleJson, for Multilingual.
     */
    private HashMap<String, String> labels = new HashMap<>();

    /**
     * Indicates the iconId of module ModuleJson.
     */
    private int iconId = 0;

    /**
     * Indicates the labelId of module ModuleJson.
     */
    private int labelId = 0;

    public void setLabels(HashMap<String, String> labels) {
        this.labels = labels;
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    public HashMap<String, String> getLabels() {
        return labels;
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
