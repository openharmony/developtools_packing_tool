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
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.HashMap;
import java.util.stream.Collectors;

public class HapVerify {
    private static final String INCLUDE = "include";
    private static final String EXCLUDE = "exclude";
    private static final Log LOG = new Log(ModuleJsonUtil.class.toString());

    /**
     * check hap is verify.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @throws BundleException Throws this exception if the json is not standard.
     * @return the result
     */
    public static boolean checkHapIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos.isEmpty()) {
            LOG.error("HapVerify::checkHapIsValid hapVerifyInfos is empty!");
            return false;
        }
        // check app variable is same
        if (!checkAppFieldsIsSame(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, some app variable is different!");
            return false;
        }
        // check moduleName is valid
        if (!checkModuleNameIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, moduleName duplicated!");
            return false;
        }
        // check package is valid
        if (!checkPackageNameIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, packageName duplicated!");
            return false;
        }
        // check abilityName is valid
        if (!checkAbilityNameIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, ability duplicated!");
            return false;
        }
        // check entry is valid
        if (!checkEntryIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, entry is not valid!");
            return false;
        }
        return true;
    }

    /**
     * check whether the app fields in the hap are the same.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @throws BundleException Throws this exception if the json is not standard.
     * @return true if app fields is same
     */
    private static boolean checkAppFieldsIsSame(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos.isEmpty()) {
            LOG.error("HapVerify::checkAppVariableIsSame failed, hapVerifyInfos is empty");
            return false;
        }
        VerifyCollection verifyCollection = new VerifyCollection();
        verifyCollection.bundleName = hapVerifyInfos.get(0).bundleName;
        verifyCollection.vendor = hapVerifyInfos.get(0).vendor;
        verifyCollection.versionCode = hapVerifyInfos.get(0).version.versionCode;
        verifyCollection.versionName = hapVerifyInfos.get(0).version.versionName;
        verifyCollection.minCompatibleVersionCode = hapVerifyInfos.get(0).version.minCompatibleVersionCode;
        verifyCollection.compatibleApiVersion = hapVerifyInfos.get(0).apiVersion.compatibleApiVersion;
        verifyCollection.targetApiVersion = hapVerifyInfos.get(0).apiVersion.targetApiVersion;
        verifyCollection.releaseType = hapVerifyInfos.get(0).apiVersion.releaseType;
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            // check bundleName
            if (hapVerifyInfo.bundleName.isEmpty()) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, bundleName should not be empty!");
                throw new BundleException("HapVerify::checkAppVariableIsSame failed, bundleName should not be empty!");
            }
            if (!verifyCollection.bundleName.equals(hapVerifyInfo.bundleName)) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, bundleName is different!");
                return false;
            }
            // check vendor
            if (hapVerifyInfo.vendor.isEmpty()) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, vendor should not be empty!");
                throw new BundleException("HapVerify::checkAppVariableIsSame failed, vendor should not be empty!");
            }
            if (!verifyCollection.vendor.equals(hapVerifyInfo.vendor)) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, vendor is different!");
                return false;
            }
            // check version
            if (verifyCollection.versionCode != hapVerifyInfo.version.versionCode) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, versionCode is different!");
                return false;
            }
            if (!verifyCollection.versionName.equals(hapVerifyInfo.version.versionName)) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, versionName is different!");
                return false;
            }
            if (verifyCollection.minCompatibleVersionCode != hapVerifyInfo.version.minCompatibleVersionCode) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, minCompatibleVersionCode is different!");
                return false;
            }
            // check apiVersion
            if (verifyCollection.compatibleApiVersion != hapVerifyInfo.apiVersion.compatibleApiVersion) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, minApiVersion is different!");
                return false;
            }
            if (verifyCollection.targetApiVersion != hapVerifyInfo.apiVersion.targetApiVersion) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, targetApiVersion is different!");
                return false;
            }
            if (!verifyCollection.releaseType.equals(hapVerifyInfo.apiVersion.releaseType)) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, releaseType is different!");
                return false;
            }
        }
        return true;
    }

    /**
     * check moduleName is valid.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if moduleName is valid
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkModuleNameIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        for (int i = 0; i < hapVerifyInfos.size() - 1; ++i) {
            if (hapVerifyInfos.get(i).moduleName.isEmpty()) {
                LOG.error("HapVerify::checkModuleNameIsValid should not be empty!");
                throw new BundleException("HapVerify::checkModuleNameIsValid should not be empty!");
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).moduleName.equals(hapVerifyInfos.get(j).moduleName)) {
                    if (!checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                        LOG.error("HapVerify::checkModuleNameIsValid " + hapVerifyInfos.get(i).moduleName +
                                " " + hapVerifyInfos.get(j).moduleName + " duplicated!");
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * check packageName is valid.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if moduleName is valid
     * @throws BundleException Throws this exception if the json is not standard
     */
    private static boolean checkPackageNameIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        for (int i = 0; i < hapVerifyInfos.size() - 1; ++i) {
            if (hapVerifyInfos.get(i).packageName.isEmpty()) {
                continue;
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).packageName.equals(hapVerifyInfos.get(j).packageName)) {
                    if (!checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                        LOG.error("HapVerify::checkPackageNameIsValid " + hapVerifyInfos.get(i).packageName +
                                " " + hapVerifyInfos.get(j).packageName + " duplicated!");
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * check abilityName is valid.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if abilityName is valid
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkAbilityNameIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            long noDuplicatedCount = hapVerifyInfo.abilityNames.stream().distinct().count();
            if (noDuplicatedCount != hapVerifyInfo.abilityNames.size()) {
                LOG.error("HapVerify::checkAbilityNameIsValid " +
                        hapVerifyInfo.moduleName + " ability duplicated!");
                return false;
            }
        }
        for (int i = 0; i < hapVerifyInfos.size(); ++i) {
            if (hapVerifyInfos.get(i).abilityNames.isEmpty()) {
                continue;
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (!Collections.disjoint(hapVerifyInfos.get(i).abilityNames, hapVerifyInfos.get(j).abilityNames)) {
                    if (!checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                        LOG.error("HapVerify::checkAbilityNameIsValid " + hapVerifyInfos.get(i).moduleName +
                                " " + hapVerifyInfos.get(j).moduleName + " ability duplicated!");
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * check entry is valid.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if entry is valid
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkEntryIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        List<HapVerifyInfo> entryHapVerifyInfos = new ArrayList<>();
        HashMap<String, Integer> deviceTypeMap = new HashMap<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (hapVerifyInfo.isEntry) {
                entryHapVerifyInfos.add(hapVerifyInfo);
                for (String device : hapVerifyInfo.deviceType) {
                    if (deviceTypeMap.containsKey(device)) {
                        deviceTypeMap.replace(device, deviceTypeMap.get(device) + 1);
                    } else {
                        deviceTypeMap.put(device, 1);
                    }
                }
            } else {
                for (String device : hapVerifyInfo.deviceType) {
                    if (!deviceTypeMap.containsKey(device)) {
                        deviceTypeMap.put(device, 0);
                    }
                }
            }
        }
        if (entryHapVerifyInfos.isEmpty()) {
            LOG.error("HapVerify::checkEntryIsValid failed, no entry module!");
            return false;
        }
        for (String device : deviceTypeMap.keySet()) {
            if (deviceTypeMap.get(device) == 0) {
                LOG.error("HapVerify::checkEntryIsValid failed, " + device + " has no entry!");
                return false;
            }
        }
        for (int i = 0; i < entryHapVerifyInfos.size() - 1; ++i) {
            for (int j = i + 1; j < entryHapVerifyInfos.size(); ++j) {
                if (!checkDuplicatedIsValid(entryHapVerifyInfos.get(i), entryHapVerifyInfos.get(j))) {
                    LOG.error("HapVerify::checkEntryIsValid failed, " + entryHapVerifyInfos.get(i).moduleName +
                            " " + entryHapVerifyInfos.get(j).moduleName + " entry duplicated!");
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * check if name duplicated, name is valid.
     *
     * @param hapVerifyInfoLeft is one hapVerifyInfo
     * @param hapVerifyInfoRight is another hapVerifyInfo that name is duplicated with hapVerifyInfoLeft
     * @return true if moduleName is valid
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkDuplicatedIsValid(HapVerifyInfo hapVerifyInfoLeft, HapVerifyInfo hapVerifyInfoRight)
            throws BundleException {
        // check deviceType
        if (Collections.disjoint(hapVerifyInfoLeft.deviceType, hapVerifyInfoRight.deviceType)) {
            return true;
        }
        // check distroFilter
        if (checkDistroFilterDisjoint(hapVerifyInfoLeft.distroFilter, hapVerifyInfoRight.distroFilter)) {
            return true;
        }
        return false;
    }

    /**
     * check two distroFilter is disjoint.
     *
     * @param distroFilterLeft is one distroFilter
     * @param distroFilterRight is another distroFilter will be checked
     * @throws BundleException Throws this exception if the json is not standard.
     * @return true if two distroFilter is disjoint
     */
    private static boolean checkDistroFilterDisjoint(DistroFilter distroFilterLeft, DistroFilter distroFilterRight)
            throws BundleException {
        if (distroFilterLeft == null || distroFilterRight == null) {
            return false;
        }
        if (distroFilterLeft.apiVersion != null && distroFilterRight.apiVersion !=  null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.apiVersion.policy, distroFilterLeft.apiVersion.value,
                    distroFilterRight.apiVersion.policy, distroFilterRight.apiVersion.value)) {
                return true;
            }
        }
        if (distroFilterLeft.screenShape != null && distroFilterRight.screenShape != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenShape.policy, distroFilterLeft.screenShape.value,
                    distroFilterRight.screenShape.policy, distroFilterRight.screenShape.value)) {
                return true;
            }
        }
        if (distroFilterLeft.screenDensity != null && distroFilterRight.screenDensity != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenDensity.policy, distroFilterLeft.screenDensity.value,
                    distroFilterRight.screenDensity.policy, distroFilterRight.screenDensity.value)) {
                return true;
            }
        }
        if (distroFilterLeft.screenWindow != null && distroFilterRight.screenWindow != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenWindow.policy, distroFilterLeft.screenWindow.value,
                    distroFilterRight.screenWindow.policy, distroFilterRight.screenWindow.value)) {
                return true;
            }
        }
        if (distroFilterLeft.countryCode != null && distroFilterRight.countryCode != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.countryCode.policy, distroFilterLeft.countryCode.value,
                    distroFilterRight.countryCode.policy, distroFilterRight.countryCode.value)) {
                return true;
            }
        }
        LOG.error("HapVerify::checkDistroFilterDisjoint two distroFilter duplicated!");
        return false;
    }

    /**
     * check two distroFilter variable is disjoint.
     *
     * @param policyLeft is one distroFilter variable policy
     * @param valueLeft is one distroFilter variable value
     * @param policyRight is another distroFilter variable policy
     * @param valueRight is another distroFilter variable value
     * @return true if two variable is disjoint
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkPolicyValueDisjoint(String policyLeft, List<String> valueLeft,
                                       String policyRight, List<String> valueRight) throws BundleException {
        if (valueLeft == null || valueLeft.isEmpty() || valueRight == null || valueRight.isEmpty()) {
            LOG.error("HapVerify::policyValueDisjoint value should not empty!");
            throw new BundleException("HapVerify::policyValueDisjoint value should not empty!");
        }
        if (policyLeft.equals(EXCLUDE) && policyRight.equals(INCLUDE)) {
            if (valueLeft.containsAll(valueRight)) {
                return true;
            }
        } else if (policyLeft.equals(INCLUDE) && policyRight.equals(INCLUDE)) {
            if (Collections.disjoint(valueLeft, valueRight)) {
                return true;
            }
        } else if (policyLeft.equals(INCLUDE) && policyRight.equals(EXCLUDE)) {
            if (valueRight.containsAll(valueLeft)) {
                return true;
            }
        } else if (policyLeft.equals(EXCLUDE) && policyRight.equals(EXCLUDE)) {
            return false;
        }
        return false;
    }
}
