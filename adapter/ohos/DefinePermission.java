/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

/**
 * DefinePermission info.
 * @deprecated
 */
public class DefinePermission {
    /**
     * Indicates the name of DefPermission.
     */
    public String name = "";

    /**
     * Indicates the grantMode of DefPermission.
     */
    public String grantMode = "";

    /**
     * Indicates the availableLevel of DefPermission.
     */
    public String availableLevel = "";

    /**
     * Indicates the provisionEnable of ModuleDefinePermissions.
     */
    public boolean provisionEnable  = true;

    /**
     * Indicates the distributedSceneEnable of ModuleDefinePermissions.
     */
    public boolean distributedSceneEnable = false;

    /**
     * Indicates the label of DefPermission.
     */
    public String label = "";

    /**
     * Indicates the description of DefPermission.
     */
    public String description = "";
}
