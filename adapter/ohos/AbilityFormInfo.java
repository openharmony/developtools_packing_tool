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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Ability form info.
 *
 */
public class AbilityFormInfo {

    /**
     * Module window info.
     *
     */
    public static class ModuleWindowInfo {
        /**
         * Indicates the designWidth of ModuleUsedScene.
         */
        public int designWidth = 0;
        /**
         * Indicates the autoDesignWidth of ModuleUsedScene.
         */
        public boolean autoDesignWidth = false;
    }

    /**
     * Indicates the name of ability form.
     */
    public String name = "";

    /**
     * Indicates the type of ability form.
     */
    public String type = "";

    /**
     * Indicates whether or not this form is allowed to update periodically.
     */
    public boolean updateEnabled = false;

    /**
     * Indicates the scheduledUpdateTime of ability form.
     */
    public String scheduledUpdateTime = "";

    /**
     * Indicates the update duration, unit: 30 mins.
     */
    public int updateDuration = 1;

    /**
     * Indicates the supportDimensions of ability form.
     */
    public List<String> supportDimensions = new ArrayList<String>();

    /**
     * Indicates the defaultDimension of ability form.
     */
    public String defaultDimension = "";

    /**
     * Indicates the metaData of ability form.
     */
    public MetaData metaData = new MetaData();

    // stage module attributes
    /**
     * Indicates the description of ability form.
     */
    public String description = "";

    /**
     * Indicates the descriptions of ability form,for Multilingual.
     */
    private HashMap<String, String> descriptions = new HashMap<>();

    /**
     * Indicates the src of ability form the attribute is corresponding to the UI code of the form.
     */
    public String src = "";

    /**
     * Indicates the window of ability form.
     */
    public ModuleWindowInfo windowInfo = new ModuleWindowInfo();

    /**
     * Indicates the isDefault of ability form.
     */
    public boolean isDefault = false;

    /**
     * Indicates the colorMode of ability form.
     */
    public String colorMode = "auto";

    /**
     * Indicates the formConfigAbility of ability form, it is the ability name for card.
     */
    public String formConfigAbility = "";

    /**
     * Indicates the formVisibleNotify of ability form.
     */
    public boolean formVisibleNotify = false;

    /**
     * Indicates the providerAbility of ability form.
     */
    public String providerAbility = "";

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    /**
     * get the customize Data value defined in this ability form.
     */
    public String getCustomizeDataValue(String customizeDataName) {
        for (CustomizeData data : metaData.customizeDatas) {
            if (customizeDataName.equals(data.name)) {
                return data.value;
            }
        }
        return "";
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }
}