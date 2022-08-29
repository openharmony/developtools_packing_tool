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
import java.util.Collections;
import java.util.List;

class HQFVerify {
    private static final Log LOG = new Log(HQFVerify.class.toString());

    /**
     * check hqf is valid.
     *
     * @param hqfVerifyInfos is the collection of hqf infos
     * @return the result
     */
    public static boolean checkHQFIsValid(List<HQFVerifyInfo> hqfVerifyInfos) {
        // check app fields
        if (hqfVerifyInfos.isEmpty()) {
            LOG.error("Error: input hqf file is empty!");
            return false;
        }
        if (!checkAppFields(hqfVerifyInfos)) {
            LOG.error("Error: input hqf file has different fields in app!");
            return false;
        }
        if (!checkModuleIsValid(hqfVerifyInfos)) {
            LOG.error("Error: input hqf file moduleName is invalid!");
            return false;
        }
        return true;
    }

    /**
     * check hqf has same app fields.
     *
     * @param hqfVerifyInfos is the collection of hqf infos
     * @return the result
     */
    private static boolean checkAppFields(List<HQFVerifyInfo> hqfVerifyInfos) {
        String bundleName = hqfVerifyInfos.get(0).getBundleName();
        int versionCode = hqfVerifyInfos.get(0).getVersionCode();
        String versionName = hqfVerifyInfos.get(0).getVersionName();
        int patchVersionCode = hqfVerifyInfos.get(0).getPatchVersionCode();
        String patchVersionName = hqfVerifyInfos.get(0).getPatchVersionName();
        for (HQFVerifyInfo hqfVerifyInfo : hqfVerifyInfos) {
            if (bundleName == null || !bundleName.equals(hqfVerifyInfo.getBundleName())) {
                LOG.error("Error: input hqf file has different bundleName!");
                return false;
            }
            if (versionCode != hqfVerifyInfo.getVersionCode()) {
                LOG.error("Error: input hqf file has different versionCode!");
                return false;
            }
            if (versionName == null || !versionName.equals(hqfVerifyInfo.getVersionName())) {
                LOG.error("Error: input hqf file has different versionName!");
                return false;
            }
            if (patchVersionCode != hqfVerifyInfo.getPatchVersionCode()) {
                LOG.error("Error: input hqf file has different patchVersionCode!");
                return false;
            }
            if (patchVersionName == null || !patchVersionName.equals(hqfVerifyInfo.getPatchVersionName())) {
                LOG.error("Error: input hqf file has different patchVersionName!");
                return false;
            }
        }
        return true;
    }

    /**
     * check hqf module is valid.
     *
     * @param hqfVerifyInfos is the collection of hqf infos
     * @return the result
     */
    private static boolean checkModuleIsValid(List<HQFVerifyInfo> hqfVerifyInfos) {
        for (int i = 0; i < hqfVerifyInfos.size(); ++i) {
            for (int j = i + 1; j < hqfVerifyInfos.size(); ++j) {
                if (checkModuleIsDuplicated(hqfVerifyInfos.get(i), hqfVerifyInfos.get(j))) {
                    LOG.error("Error: input hqf file moduleName duplicated!");
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * check module name duplicated.
     *
     * @param hqfVerifyInfoLeft is one HQFVerifyInfo
     * @param hqfVerifyInfoRight is another HQFVerifyInfo
     * @return the result
     */
    private static boolean checkModuleIsDuplicated(HQFVerifyInfo hqfVerifyInfoLeft, HQFVerifyInfo hqfVerifyInfoRight) {
        if (!hqfVerifyInfoLeft.getModuleName().equals(hqfVerifyInfoRight.getModuleName())) {
            return false;
        }
        if (Collections.disjoint(hqfVerifyInfoLeft.getDeviceTypes(), hqfVerifyInfoRight.getDeviceTypes())) {
            return false;
        }
        return true;
    }
}
