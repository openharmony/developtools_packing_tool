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

/**
 * DistroFilter info
 *
 */
public class DistroFilter {
    /**
     * Indicates the DistroFilter info ApiVersion
     */
    public ApiVersion apiVersion = null;

    /**
     * Indicates the DistroFilter info ScreenShape
     */
    public ScreenShape screenShape = null;

    /**
     * Indicates the DistroFilter info ScreenDensity
     */
    public ScreenDensity screenDensity = null;

    /**
     * Indicates the DistroFilter info ScreenWindow
     */
    public ScreenWindow screenWindow = null;

    /**
     * Indicates the DistroFilter info CountryCode
     */
    public CountryCode countryCode = null;
    
    /**
     * dump distroFilter message.
     *
     * @return distroFilter dump string
     */
    public String dump() {
        String dumpStr = "";
        if (apiVersion == null && screenShape == null && screenDensity == null
            && screenWindow == null && countryCode == null) {
            return dumpStr;
        }
        dumpStr = "distroFilter:";
        if (apiVersion != null) {
            String apiVersionStr = "apiVersion: policy is " + apiVersion.policy + ", value is " + apiVersion.value;
            dumpStr += " " + apiVersionStr;
        }
        if (screenShape != null) {
            String screenShapeStr = "screenShape: policy is " + screenShape.policy + ", value is " + screenShape.value;
            dumpStr += " " + screenShapeStr;
        }
        if (screenDensity != null) {
            String screenDensityStr = "screenDensity: policy is " + screenDensity.policy + ", value is "
                + screenDensity.value;
            dumpStr += " " + screenDensityStr;
        }
        if (screenWindow != null) {
            String screenWindowStr = "screenWindow: policy is " + screenWindow.policy + ", value is "
                + screenWindow.value;
            dumpStr += " " + screenWindowStr;
        }
        if (countryCode != null) {
            String countryCodeStr = "countryCode: policy is " + countryCode.policy + ", value is " + countryCode.value;
            dumpStr += " " + countryCodeStr;
        }
        return dumpStr;
    }
}