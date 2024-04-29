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
 * collection of uriInfo members
 */
public class UriInfo {
    /**
     * Indicates the schema of ModuleUriInfo.
     */
    public String scheme = "";
    /**
     * Indicates the host of ModuleUriInfo.
     */
    public String host = "";
    /**
     * Indicates the port of ModuleUriInfo.
     */
    public String port = "";
    /**
     * Indicates the pathStartWith of ModuleUriInfo.
     */
    public String pathStartWith = "";
    /**
     * Indicates the pathRegex of ModuleUriInfo.
     */
    public String pathRegex = "";
    /**
     * Indicates the path of ModuleUriInfo.
     */
    public String path = "";
    /**
     * Indicates the type of ModuleUriInfo.
     */
    public String type = "";
    /**
     * Indicates the uniform file type supported by the ability.
     */
    private String utd = "";
    /**
     * Indicates the maximum number of files that can be received.
     */
    private Integer maxFileSupported = 0;
    /**
     * Indicates the uri feature.
     */
    private String linkFeature = "";

    /**
     * get the uniform file type supported by the ability.
     */
    public String getUtd() {
        return utd;
    }

    /**
     * set the uniform file type supported by the ability.
     */
    public void setUtd(String utd) {
        this.utd = utd;
    }

    /**
     * get the maximum number of files that can be received.
     */
    public Integer getMaxFileSupported() {
        return maxFileSupported;
    }

    /**
     * set the maximum number of files that can be received.
     */
    public void setMaxFileSupported(Integer maxFileSupported) {
        this.maxFileSupported = maxFileSupported;
    }

    /**
     * get the uri feature.
     */
    public String getLinkFeature() {
        return linkFeature;
    }

    /**
     * set the uri feature.
     */
    public void setLinkFeature(String linkFeature) {
        this.linkFeature = linkFeature;
    }
}
