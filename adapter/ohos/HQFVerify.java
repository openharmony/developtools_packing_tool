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

import java.util.Collections;
import java.util.List;

/**
 * Collections of method for HQF verify.
 *
 */
class HQFVerify {
    private static final Log LOG = new Log(HQFVerify.class.toString());

    /**
     * check hqf is valid.
     *
     * @param hqfVerifyInfos is the collection of hqf infos
     * @return the result
     */
    public static boolean checkHQFIsValid(List<HQFInfo> hqfVerifyInfos) {
        // check app fields
        if (hqfVerifyInfos.isEmpty()) {
            LOG.error(PackingToolErrMsg.CHECK_HQF_INVALID.toString("Input hqf file is empty."));
            return false;
        }
        if (!checkAppFields(hqfVerifyInfos)) {
            LOG.error(PackingToolErrMsg.CHECK_HQF_INVALID.toString("Input hqf file has different fields in app."));
            return false;
        }
        if (!checkModuleIsValid(hqfVerifyInfos)) {
            LOG.error(PackingToolErrMsg.CHECK_HQF_INVALID.toString("Input hqf file moduleName is invalid."));
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
    private static boolean checkAppFields(List<HQFInfo> hqfVerifyInfos) {
        String bundleName = hqfVerifyInfos.get(0).getBundleName();
        int versionCode = hqfVerifyInfos.get(0).getVersionCode();
        String versionName = hqfVerifyInfos.get(0).getVersionName();
        int patchVersionCode = hqfVerifyInfos.get(0).getPatchVersionCode();
        String patchVersionName = hqfVerifyInfos.get(0).getPatchVersionName();
        for (HQFInfo hqfVerifyInfo : hqfVerifyInfos) {
            if (bundleName == null || !bundleName.equals(hqfVerifyInfo.getBundleName())) {
                String errMsg = "Input hqf file has different bundleName.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_FAILED.toString(errMsg));
                return false;
            }
            if (versionCode != hqfVerifyInfo.getVersionCode()) {
                String errMsg = "Input hqf file has different versionCode.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_FAILED.toString(errMsg));
                return false;
            }
            if (versionName == null || !versionName.equals(hqfVerifyInfo.getVersionName())) {
                String errMsg = "Input hqf file has different versionName.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_FAILED.toString(errMsg));
                return false;
            }
            if (patchVersionCode != hqfVerifyInfo.getPatchVersionCode()) {
                String errMsg = "Input hqf file has different patchVersionCode.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_FAILED.toString(errMsg));
                return false;
            }
            if (patchVersionName == null || !patchVersionName.equals(hqfVerifyInfo.getPatchVersionName())) {
                String errMsg = "Input hqf file has different patchVersionName.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_FAILED.toString(errMsg));
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
    private static boolean checkModuleIsValid(List<HQFInfo> hqfVerifyInfos) {
        for (int i = 0; i < hqfVerifyInfos.size(); ++i) {
            for (int j = i + 1; j < hqfVerifyInfos.size(); ++j) {
                if (checkModuleIsDuplicated(hqfVerifyInfos.get(i), hqfVerifyInfos.get(j))) {
                    String errMsg = "Input hqf file moduleName is duplicated.";
                    LOG.error(PackingToolErrMsg.CHECK_MODULE_INVALID.toString(errMsg));
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
    private static boolean checkModuleIsDuplicated(HQFInfo hqfVerifyInfoLeft, HQFInfo hqfVerifyInfoRight) {
        if (!hqfVerifyInfoLeft.getModuleName().equals(hqfVerifyInfoRight.getModuleName())) {
            return false;
        }
        if (Collections.disjoint(hqfVerifyInfoLeft.getDeviceTypes(), hqfVerifyInfoRight.getDeviceTypes())) {
            return false;
        }
        return true;
    }
}
