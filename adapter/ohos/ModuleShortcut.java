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
 * module Shortcut info.
 *
 */
class ModuleShortcut {
    /**
     * Indicates the shortcutId of module Shortcut.
     */
    private String shortcutId = "";

    /**
     * Indicates the label of module Shortcut.
     */
    private String label = "";

    /**
     * Indicates the icon of module Shortcut.
     */
    private String icon = "";

    /**
     * Indicates the intents of module Shortcut.
     */
    private List<Want> wants = new ArrayList<Want>();

    public String getShortcutId() {
        return shortcutId;
    }

    public void setShortcutId(String shortcutId) {
        this.shortcutId = shortcutId;
    }

    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String icon) {
        this.icon = icon;
    }

    public List<Want> getWants() {
        return wants;
    }

    public void setWants(List<Want> wants) {
        this.wants = wants;
    }
}
