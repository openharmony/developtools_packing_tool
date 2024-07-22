/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

public class ModuleJsonInfo {
    private String moduleType = "";
    private String moduleName = "";
    private boolean compressNativeLibs = false;
    private boolean generateBuildHash = false;

    public String getModuleType() {
        return moduleType;
    }

    public void setModuleType(String moduleType) {
        this.moduleType = moduleType;
    }

    public String getModuleName() {
        return moduleName;
    }

    public void setModuleName(String moduleName) {
        this.moduleName = moduleName;
    }

    public boolean isCompressNativeLibs() {
        return compressNativeLibs;
    }

    public void setCompressNativeLibs(boolean compressNativeLibs) {
        this.compressNativeLibs = compressNativeLibs;
    }

    public boolean isGenerateBuildHash() {
        return generateBuildHash;
    }

    public void setGenerateBuildHash(boolean generateBuildHash) {
        this.generateBuildHash = generateBuildHash;
    }

    public boolean isShared() {
        return moduleType != null && moduleType.equals(Constants.TYPE_SHARED);
    }
}
