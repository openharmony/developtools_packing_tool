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

/**
 * ModuleJson info.
 *
 */
class ModuleProfileInfo {
    /**
     * Indicates the app of ModuleProfileInfo.
     */
    public ModuleAppInfo moduleAppInfo = new ModuleAppInfo();
    /**
     * Indicates the module of ModuleProfileInfo.
     */
    public ModuleInfo moduleInfo = new ModuleInfo();
    /**
     * Indicates the hapName of ModuleProfileInfo.
     */
    public String hapName = "";
}
