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
 * Collection of members in ModuleApiVersion.
 */
class ModuleApiVersion {
    /**
     * Indicates the compatibleApiVersion of app.
     */
    private int compatibleApiVersion = -1;

    /**
     * Indicates the compatibleApiVersion of app.
     */
    private int targetApiVersion = -1;

    /**
     * Indicates the releaseType of app.
     */
    private String releaseType = "";

    public String getReleaseType() {
        return releaseType;
    }

    public void setReleaseType(String releaseType) {
        this.releaseType = releaseType;
    }

    public int getCompatibleApiVersion() {
        return compatibleApiVersion;
    }

    public void setCompatibleApiVersion(int compatibleApiVersion) {
        this.compatibleApiVersion = compatibleApiVersion;
    }

    public int getTargetApiVersion() {
        return targetApiVersion;
    }

    public void setTargetApiVersion(int targetApiVersion) {
        this.targetApiVersion = targetApiVersion;
    }
}
