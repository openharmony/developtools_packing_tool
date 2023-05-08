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
 * DefPermission info.
 * @deprecated
 */
public class DefPermission {
    /**
     * Indicates the name of DefPermission.
     */
    public String name = "";

    /**
     * Indicates the grantMode of DefPermission.
     */
    public String grantMode = "";

    /**
     * Indicates the group of DefPermission.
     */
    public String group = "";

    /**
     * Indicates the label of DefPermission.
     */
    public String label = "";

    /**
     * Indicates the description of DefPermission.
     */
    public String description = "";

    /**
     * Indicates the availableScope of DefPermission.
     */
    public List<String> availableScope = new ArrayList<>();

    private HashMap<String, String> labels = new HashMap<>();

    private HashMap<String, String> descriptions = new HashMap<>();

    public HashMap<String, String> getDescriptions() {
        return descriptions;
    }

    public void setDescriptions(HashMap<String, String> descriptions) {
        this.descriptions = descriptions;
    }

    public HashMap<String, String> getLabels() {
        return labels;
    }

    public void setLabels(HashMap<String, String> labels) {
        this.labels = labels;
    }
}