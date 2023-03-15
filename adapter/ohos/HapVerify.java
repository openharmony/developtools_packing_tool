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
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * check hap is verify.
 */
class HapVerify {
    private static final String INCLUDE = "include";
    private static final String EXCLUDE = "exclude";
    private static final Log LOG = new Log(HapVerify.class.toString());
    private static final int SERVICE_DEPTH = 2;
    private static final int APPLICATION_DEPTH = 5;
    private static final String EMPTY_STRING = "";
    private static final String ENTRY = "entry";
    private static final String FEATURE = "feature";
    private static final String SHARED_LIBRARY = "shared";
    private static final String HAR = "har";
    private static final String REFERENCE_LINK =
            "https://developer.harmonyos.com/cn/docs/documentation/doc-guides/verification_rule-0000001406748378";
    private static final String ATOMIC_SERVICE = "atomicService";
    private static final String MAIN = "main";
    private static final String NORMAL = "normal";
    private static final long FILE_LENGTH_1M = 1024 * 1024L;
    private static final int ONE = 1;
    private static final int ATOMIC_SERVICE_APP_LIMIT_SIZE = 10;
    private static final int ATOMIC_SERVICE_MODULE_SIZE = 2;

    /**
     * check hap is verify.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard
     */
    public static boolean checkHapIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos == null || hapVerifyInfos.isEmpty()) {
            LOG.error("HapVerify::checkHapIsValid hapVerifyInfos is empty.");
            return false;
        }
        // check app variable is same
        if (!checkAppFieldsIsSame(hapVerifyInfos)) {
            LOG.error("some app variable is different.");
            return false;
        }
        // check moduleName is valid
        if (!checkModuleNameIsValid(hapVerifyInfos)) {
            return false;
        }
        // check package is valid
        if (!checkPackageNameIsValid(hapVerifyInfos)) {
            LOG.error("packageName duplicated.");
            return false;
        }
        // check entry is valid
        if (!checkEntryIsValid(hapVerifyInfos)) {
            return false;
        }
        // check dependency is valid
        if (!checkDependencyIsValid(hapVerifyInfos)) {
            LOG.error("module dependency is invalid.");
            return false;
        }
        // check old atomic service size
        if (!checkOldAtomicServiceSize(hapVerifyInfos)) {
            LOG.error("checkOldAtomicServiceSize failed.");
            return false;
        }
        // check atomic service is valid
        if (!checkAtomicServiceIsValid(hapVerifyInfos)) {
            LOG.error("checkAtomicServiceIsValid failed.");
            return false;
        }
        // check ability is valid
        if (!checkAbilityNameIsValid(hapVerifyInfos)) {
            LOG.info("Ability name is duplicated.");
        }
        // check targetModuleName
        if (!checkTargetModuleNameIsExisted(hapVerifyInfos)) {
            LOG.error("Ability name is duplicated.");
            return false;
        }
        return true;
    }

    /**
     * check whether the app fields in the hap are the same.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if app fields is same
     */
    private static boolean checkAppFieldsIsSame(List<HapVerifyInfo> hapVerifyInfos) {
        if (hapVerifyInfos.isEmpty()) {
            LOG.error("HapVerify::checkAppVariableIsSame failed, hapVerifyInfos is empty.");
            return false;
        }
        VerifyCollection verifyCollection = new VerifyCollection();
        verifyCollection.bundleName = hapVerifyInfos.get(0).getBundleName();
        verifyCollection.vendor = hapVerifyInfos.get(0).getVendor();
        verifyCollection.versionCode = hapVerifyInfos.get(0).getVersion().versionCode;
        verifyCollection.versionName = hapVerifyInfos.get(0).getVersion().versionName;
        verifyCollection.minCompatibleVersionCode = hapVerifyInfos.get(0).getVersion().minCompatibleVersionCode;
        verifyCollection.compatibleApiVersion = hapVerifyInfos.get(0).getApiVersion().getCompatibleApiVersion();
        verifyCollection.targetApiVersion = hapVerifyInfos.get(0).getApiVersion().getTargetApiVersion();
        verifyCollection.releaseType = hapVerifyInfos.get(0).getApiVersion().getReleaseType();
        verifyCollection.setSplit(hapVerifyInfos.get(0).isSplit());
        verifyCollection.setMain(hapVerifyInfos.get(0).getMain());
        verifyCollection.targetBundleName = hapVerifyInfos.get(0).getTargetBundleName();
        verifyCollection.targetPriority = hapVerifyInfos.get(0).getTargetPriority();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (hapVerifyInfo.getBundleName().isEmpty() ||
                    !verifyCollection.bundleName.equals(hapVerifyInfo.getBundleName())) {
                LOG.error("input module bundleName is different.");
                return false;
            }
            if (hapVerifyInfo.getVendor().isEmpty() || !verifyCollection.vendor.equals(hapVerifyInfo.getVendor())) {
                LOG.error("input module vendor is different.");
                return false;
            }
            if (verifyCollection.versionCode != hapVerifyInfo.getVersion().versionCode) {
                LOG.error("input module versionCode is different.");
                return false;
            }
            if (!verifyCollection.versionName.equals(hapVerifyInfo.getVersion().versionName)) {
                LOG.error("input module versionName is different.");
                return false;
            }
            if (verifyCollection.minCompatibleVersionCode != hapVerifyInfo.getVersion().minCompatibleVersionCode) {
                LOG.error("input module minCompatibleVersionCode is different.");
                return false;
            }
            if (verifyCollection.compatibleApiVersion != hapVerifyInfo.getApiVersion().getCompatibleApiVersion()) {
                LOG.error("input module minApiVersion is different.");
                return false;
            }
            if (verifyCollection.targetApiVersion != hapVerifyInfo.getApiVersion().getTargetApiVersion()) {
                LOG.error("input module targetApiVersion is different.");
                return false;
            }
            if (!verifyCollection.releaseType.equals(hapVerifyInfo.getApiVersion().getReleaseType())) {
                LOG.error("input module releaseType is different.");
                return false;
            }
            if (verifyCollection.isSplit() != hapVerifyInfo.isSplit()) {
                LOG.error("input module split is different.");
                return false;
            }
            if (!verifyCollection.getMain().equals(hapVerifyInfo.getMain())) {
                LOG.error("input module main is different.");
                return false;
            }
            if (!verifyCollection.targetBundleName.equals(hapVerifyInfo.getTargetBundleName())) {
                LOG.error("targetBundleName is different.");
                return false;
            }
            if (verifyCollection.targetPriority != hapVerifyInfo.getTargetPriority()) {
                LOG.error("targetPriority is different.");
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
            if (hapVerifyInfos.get(i).getModuleName().isEmpty()) {
                LOG.error("HapVerify::checkModuleNameIsValid should not be empty.");
                throw new BundleException("HapVerify::checkModuleNameIsValid should not be empty.");
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).getModuleName().equals(hapVerifyInfos.get(j).getModuleName()) &&
                    !checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    LOG.error("Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                        hapVerifyInfos.get(j).getModuleName() + ") have the same moduleName, " +
                            "please check deviceType or distroFilter of the module.");
                    LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " has deviceType "
                        + hapVerifyInfos.get(i).getDeviceType() + ".");
                    LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " has deviceType "
                        + hapVerifyInfos.get(j).getDeviceType() + ".");
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(i).getDistroFilter().dump())) {
                        LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " DistroFilter is : "
                                + hapVerifyInfos.get(i).getDistroFilter().dump() + ".");
                    }
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(j).getDistroFilter().dump())) {
                        LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " DistroFilter is "
                                + hapVerifyInfos.get(j).getDistroFilter().dump() + ".");
                    }
                    LOG.error("Solution: Make sure the module name is valid and unique.");
                    LOG.error("Reference: " + REFERENCE_LINK + ".");
                    return false;
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
            if (hapVerifyInfos.get(i).getPackageName().isEmpty()) {
                continue;
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).getPackageName().equals(hapVerifyInfos.get(j).getPackageName()) &&
                        !checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    LOG.error("Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                            hapVerifyInfos.get(j).getModuleName() + ") have the same packageName, " +
                            "please check deviceType or distroFilter of the module.");
                    LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " has deviceType "
                            + hapVerifyInfos.get(i).getDeviceType() + ".");
                    LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " has deviceType "
                            + hapVerifyInfos.get(j).getDeviceType() + ".");
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(i).getDistroFilter().dump())) {
                        LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " DistroFilter is : " +
                                hapVerifyInfos.get(i).getDistroFilter().dump() + ".");
                    }
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(j).getDistroFilter().dump())) {
                        LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " DistroFilter is " +
                                hapVerifyInfos.get(j).getDistroFilter().dump() + ".");
                    }
                    LOG.error("Solution: Make sure package name is valid and unique.");
                    LOG.error("Reference: " + REFERENCE_LINK + ".");
                    return false;
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
            long noDuplicatedCount = hapVerifyInfo.getAbilityNames().stream().distinct().count();
            if (noDuplicatedCount != hapVerifyInfo.getAbilityNames().size()) {
                LOG.warning(
                        hapVerifyInfo.getModuleName() + " ability duplicated, please rename ability name.");
                return false;
            }
        }
        for (int i = 0; i < hapVerifyInfos.size(); ++i) {
            if (hapVerifyInfos.get(i).getAbilityNames().isEmpty()) {
                continue;
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (!Collections.disjoint(hapVerifyInfos.get(i).getAbilityNames(),
                        hapVerifyInfos.get(j).getAbilityNames()) &&
                        !checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    LOG.warning("Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                            hapVerifyInfos.get(j).getModuleName() + ") have the same ability name.");
                    LOG.warning("Module: " + hapVerifyInfos.get(i).getModuleName() + " has ability "
                        + hapVerifyInfos.get(i).getAbilityNames() + ".");
                    LOG.warning( "Module: " + hapVerifyInfos.get(j).getModuleName() + " has ability "
                        + hapVerifyInfos.get(j).getAbilityNames() + ".");
                    LOG.warning("Solution: Make sure ability name is valid and unique.");
                    LOG.warning("Reference: " + REFERENCE_LINK + ".");
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * check targetModuleName is existed.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return true if targetModuleName is erxisted
     * @throws BundleException Throws this exception if the json is not standard.
     */
    private static boolean checkTargetModuleNameIsExisted(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        List<HapVerifyInfo> internalOverlayHap = new ArrayList<>();
        List<HapVerifyInfo> nonOverlayHap = new ArrayList<>();
        List<String> targetModuleList = new ArrayList<>();
        List<String> moduleList = new ArrayList<>();
        for (HapVerifyInfo hapInfo : hapVerifyInfos) {
            if (!hapInfo.getTargetModuleName().isEmpty()) {
                internalOverlayHap.add(hapInfo);
                targetModuleList.add(hapInfo.getTargetModuleName());
                continue;
            }
            nonOverlayHap.add(hapInfo);
            moduleList.add(hapInfo.getModuleName());
        }
        if (internalOverlayHap.isEmpty()) {
            return true;
        }
        if (nonOverlayHap.isEmpty()) {
            LOG.error("target modules are needed to pack with overlay module.");
            return false;
        }
        if (!moduleList.containsAll(targetModuleList)) {
            LOG.error("target modules are needed to pack with overlay module.");
            return false;
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
        List<HapVerifyInfo> featureHapVerifyInfos = new ArrayList<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (ENTRY.equals(hapVerifyInfo.getModuleType())) {
                entryHapVerifyInfos.add(hapVerifyInfo);
            } else if (FEATURE.equals(hapVerifyInfo.getModuleType())) {
                featureHapVerifyInfos.add(hapVerifyInfo);
            } else if (!SHARED_LIBRARY.equals(hapVerifyInfo.getModuleType())) {
                LOG.warning("Input wrong type module.");
            }
        }
        if (hapVerifyInfos.isEmpty()
                || (entryHapVerifyInfos.isEmpty() && !hapVerifyInfos.get(0).isSharedHsp())) {
            LOG.warning("Warning: has no entry module.");
        }

        for (int i = 0; i < entryHapVerifyInfos.size() - 1; ++i) {
            for (int j = i + 1; j < entryHapVerifyInfos.size(); ++j) {
                if (!checkDuplicatedIsValid(entryHapVerifyInfos.get(i), entryHapVerifyInfos.get(j))) {
                    LOG.error("Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                            hapVerifyInfos.get(j).getModuleName() + ") are entry, " +
                            "please check deviceType or distroFilter of the module.");
                    LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " has deviceType "
                            + hapVerifyInfos.get(i).getDeviceType() + ".");
                    LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " has deviceType "
                            + hapVerifyInfos.get(j).getDeviceType() + ".");
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(i).getDistroFilter().dump())) {
                        LOG.error("Module: " + hapVerifyInfos.get(i).getModuleName() + " DistroFilter is : " +
                                hapVerifyInfos.get(i).getDistroFilter().dump() + ".");
                    }
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(j).getDistroFilter().dump())) {
                        LOG.error("Another Module: " + hapVerifyInfos.get(j).getModuleName() + " DistroFilter is " +
                                hapVerifyInfos.get(j).getDistroFilter().dump() + ".");
                    }
                    LOG.error("Solution: Make sure entry name is valid and unique.");
                    LOG.error("Reference: " + REFERENCE_LINK + ".");
                    return false;
                }
            }
        }

        Map<String, List<HapVerifyInfo>> deviceHap = classifyEntry(entryHapVerifyInfos);
        for (HapVerifyInfo hapVerifyInfo : featureHapVerifyInfos) {
            if (!checkFeature(hapVerifyInfo, deviceHap)) {
                LOG.warning(hapVerifyInfo.getModuleName() + " can not be covered by entry.");
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
        if (Collections.disjoint(hapVerifyInfoLeft.getDeviceType(), hapVerifyInfoRight.getDeviceType())) {
            return true;
        }
        // check distroFilter
        if (checkDistroFilterDisjoint(hapVerifyInfoLeft.getDistroFilter(), hapVerifyInfoRight.getDistroFilter())) {
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
        if (distroFilterLeft.apiVersion != null && distroFilterRight.apiVersion != null) {
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
    private static boolean checkPolicyValueDisjoint(String policyLeft, List<String> valueLeft, String policyRight,
                                                    List<String> valueRight) throws BundleException {
        if (valueLeft == null || valueRight == null) {
            LOG.error("HapVerify::checkPolicyValueDisjoint value should not empty.");
            throw new BundleException("HapVerify::checkPolicyValueDisjoint value should not empty.");
        }
        if (EXCLUDE.equals(policyLeft) && INCLUDE.equals(policyRight)) {
            if (valueRight.isEmpty() || valueLeft.containsAll(valueRight)) {
                return true;
            }
        } else if (INCLUDE.equals(policyLeft) && INCLUDE.equals(policyRight)) {
            if (Collections.disjoint(valueLeft, valueRight)) {
                return true;
            }
        } else if (INCLUDE.equals(policyLeft) && EXCLUDE.equals(policyRight)) {
            if (valueLeft.isEmpty() || valueRight.containsAll(valueLeft)) {
                return true;
            }
        } else if (EXCLUDE.equals(policyLeft) && EXCLUDE.equals(policyRight)) {
            return false;
        } else {
            LOG.error("HapVerify::checkPolicyValueDisjoint input policy is invalid.");
            throw new BundleException("HapVerify::checkPolicyValueDisjoint input policy is invalid.");
        }
        return false;
    }

    /**
     * classify entry haps by deviceType.
     *
     * @param entryHapVerifyInfos is the list od entry hapVerifyInfos
     * @return deviceHap that is classfied
     */
    private static Map<String, List<HapVerifyInfo>> classifyEntry(List<HapVerifyInfo> entryHapVerifyInfos) {
        HashMap<String, List<HapVerifyInfo>> deviceHaps = new HashMap<>();
        for (HapVerifyInfo hapVerifyInfo : entryHapVerifyInfos) {
            for (String device : hapVerifyInfo.getDeviceType()) {
                if (deviceHaps.containsKey(device)) {
                    deviceHaps.get(device).add(hapVerifyInfo);
                } else {
                    deviceHaps.put(device, new ArrayList<HapVerifyInfo>());
                    deviceHaps.get(device).add(hapVerifyInfo);
                }
            }
        }
        return deviceHaps;
    }

    /**
     * check feature is valid, deviceType is subset of entry, distroFilter is subset of entry
     *
     * @param featureHap the feature hap will be checked
     * @param deviceHap is the haps that feature matched
     * @return feature is valid
     * @throws BundleException when input distroFilter is invalid
     */
    private static boolean checkFeature(HapVerifyInfo featureHap, Map<String, List<HapVerifyInfo>> deviceHap)
            throws BundleException {
        // check deviceType and distroFilter
        for (String device : featureHap.getDeviceType()) {
            if (!deviceHap.containsKey(device)) {
                LOG.warning("Warning: device " + device + " has feature but has no entry.");
                return false;
            }
            List<HapVerifyInfo> entryHaps = deviceHap.get(device);
            if (!checkFeatureDistroFilter(featureHap, entryHaps)) {
                LOG.warning(featureHap.getModuleName() +
                        "'s distroFilter has not covered by entry.");
                if (!EMPTY_STRING.equals(featureHap.getDistroFilter().dump())) {
                    LOG.warning(featureHap.getModuleName() + " has " +
                            featureHap.getDistroFilter().dump() + ".");
                }
                return false;
            }
        }
        return true;
    }

    /**
     * check feature is valid, deviceType is subset of entry, distroFilter is subset of entry
     *
     * @param featureHap the feature hap will be checked
     * @param entryHaps is the haps that feature matched
     * @return feature is valid
     * @throws BundleException when input policy in invalid
     */
    private static boolean checkFeatureDistroFilter(HapVerifyInfo featureHap, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        if (featureHap.getDistroFilter() == null) {
            if (checkApiVersionCovered(null, entryHaps)
                    && checkScreenShapeCovered(null, entryHaps)
                    && checkScreenWindowCovered(null, entryHaps)
                    && checkScreenDensityCovered(null, entryHaps)
                    && checkCountryCodeCovered(null, entryHaps)) {
                return true;
            } else {
                return false;
            }
        }
        if (!checkApiVersionCovered(featureHap.getDistroFilter().apiVersion, entryHaps)) {
            LOG.warning("HapVerify::checkFeatureDistroFilter failed, apiVersion is not covered.");
            return false;
        }
        if (!checkScreenShapeCovered(featureHap.getDistroFilter().screenShape, entryHaps)) {
            LOG.warning("HapVerify::checkFeatureDistroFilter failed, screenShape is not covered.");
            return false;
        }
        if (!checkScreenWindowCovered(featureHap.getDistroFilter().screenWindow, entryHaps)) {
            LOG.warning("HapVerify::checkFeatureDistroFilter failed, screenWindow is not covered.");
            return false;
        }
        if (!checkScreenDensityCovered(featureHap.getDistroFilter().screenDensity, entryHaps)) {
            LOG.warning("HapVerify::checkFeatureDistroFilter failed, screenDensity is not covered.");
            return false;
        }
        if (!checkCountryCodeCovered(featureHap.getDistroFilter().countryCode, entryHaps)) {
            LOG.warning("HapVerify::checkFeatureDistroFilter failed, countryCode is not covered.");
            return false;
        }
        return true;
    }

    /**
     * check feature apiVersion is subset of entry apiVersion
     *
     * @param apiVersion is the apiVersion of feature hap
     * @param entryHaps is the haps that feature matched
     * @return apiVersion is valid
     * @throws BundleException when input policy is invalid
     */
    private static boolean checkApiVersionCovered(ApiVersion apiVersion, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        List<String> include = null;
        List<String> exclude = null;
        for (HapVerifyInfo hapVerifyInfo : entryHaps) {
            if (hapVerifyInfo.getDistroFilter() == null || hapVerifyInfo.getDistroFilter().apiVersion == null) {
                return true;
            }
            if (hapVerifyInfo.getDistroFilter().apiVersion.policy == null) {
                LOG.error("HapVerify::checkApiVersionCovered input none policy.");
                return false;
            }
            if (INCLUDE.equals(hapVerifyInfo.getDistroFilter().apiVersion.policy)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                // take collection of two include value
                include.addAll(hapVerifyInfo.getDistroFilter().apiVersion.value);
            } else if (EXCLUDE.equals(hapVerifyInfo.getDistroFilter().apiVersion.policy)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                // take intersection of two exclude value
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().apiVersion.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkApiVersionCovered input policy is invalid.");
                throw new BundleException("HapVerify::checkApiVersionCovered input policy is invalid.");
            }
        }
        if (include != null) {
            include = include.stream().distinct().collect(Collectors.toList());
        }
        if (exclude != null) {
            exclude = exclude.stream().distinct().collect(Collectors.toList());
        }
        if (apiVersion == null) {
            return checkEntryPolicyValueCoverAll(include, exclude);
        }
        return checkPolicyValueCovered(apiVersion.policy, apiVersion.value, include, exclude);
    }

    /**
     * check feature screenShape is subset of entry screenShape
     *
     * @param screenShape is the screenShape of feature hap
     * @param entryHaps is the haps that feature matched
     * @return screenShape is valid
     * @throws BundleException when input policy is invalid
     */
    private static boolean checkScreenShapeCovered(ScreenShape screenShape, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        List<String> include = null;
        List<String> exclude = null;
        for (HapVerifyInfo hapVerifyInfo : entryHaps) {
            if (hapVerifyInfo.getDistroFilter() == null || hapVerifyInfo.getDistroFilter().screenShape == null) {
                return true;
            }
            if (hapVerifyInfo.getDistroFilter().screenShape.policy == null) {
                LOG.error("HapVerify::checkScreenShapeCovered input none policy.");
                return false;
            }
            if (INCLUDE.equals(hapVerifyInfo.getDistroFilter().screenShape.policy)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenShape.value);
            } else if (EXCLUDE.equals(hapVerifyInfo.getDistroFilter().screenShape.policy)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenShape.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenShapeCovered input policy is invalid.");
                throw new BundleException("HapVerify::checkScreenShapeCovered input policy is invalid.");
            }
        }
        if (include != null) {
            include = include.stream().distinct().collect(Collectors.toList());
        }
        if (exclude != null) {
            exclude = exclude.stream().distinct().collect(Collectors.toList());
        }
        if (screenShape == null) {
            return checkEntryPolicyValueCoverAll(include, exclude);
        }
        return checkPolicyValueCovered(screenShape.policy, screenShape.value, include, exclude);
    }

    /**
     * check feature screenWindow is subset of entry screenWindow
     *
     * @param screenWindow is the screenWindow of feature hap
     * @param entryHaps is the haps that feature matched
     * @return screenWindow is valid
     */
    private static boolean checkScreenWindowCovered(ScreenWindow screenWindow, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        List<String> include = null;
        List<String> exclude = null;
        for (HapVerifyInfo hapVerifyInfo : entryHaps) {
            if (hapVerifyInfo.getDistroFilter() == null || hapVerifyInfo.getDistroFilter().screenWindow == null) {
                return true;
            }
            if (hapVerifyInfo.getDistroFilter().screenWindow.policy == null) {
                LOG.error("HapVerify::checkScreenWindowCovered input none policy.");
                return false;
            }
            if (INCLUDE.equals(hapVerifyInfo.getDistroFilter().screenWindow.policy)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenWindow.value);
            } else if (EXCLUDE.equals(hapVerifyInfo.getDistroFilter().screenWindow.policy)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenWindow.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenWindowCovered input policy is invalid.");
                throw new BundleException("HapVerify::checkScreenWindowCovered input policy is invalid.");
            }
        }
        if (include != null) {
            include = include.stream().distinct().collect(Collectors.toList());
        }
        if (exclude != null) {
            exclude = exclude.stream().distinct().collect(Collectors.toList());
        }
        if (screenWindow == null) {
            return checkEntryPolicyValueCoverAll(include, exclude);
        }
        return checkPolicyValueCovered(screenWindow.policy, screenWindow.value, include, exclude);
    }

    /**
     * check feature screenDensity is subset of entry screenDensity
     *
     * @param screenDensity is the screenDensity of feature hap
     * @param entryHaps is the haps that feature matched
     * @return screenDensity is valid
     */
    private static boolean checkScreenDensityCovered(ScreenDensity screenDensity, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        List<String> include = null;
        List<String> exclude = null;
        for (HapVerifyInfo hapVerifyInfo : entryHaps) {
            if (hapVerifyInfo.getDistroFilter() == null || hapVerifyInfo.getDistroFilter().screenDensity == null) {
                return true;
            }
            if (hapVerifyInfo.getDistroFilter().screenDensity.policy == null) {
                LOG.error("HapVerify::checkScreenDensityCovered input none policy.");
                return false;
            }
            if (INCLUDE.equals(hapVerifyInfo.getDistroFilter().screenDensity.policy)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenDensity.value);
            } else if (EXCLUDE.equals(hapVerifyInfo.getDistroFilter().screenDensity.policy)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenDensity.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenDensityCovered input policy is invalid.");
                throw new BundleException("HapVerify::checkScreenDensityCovered input policy is invalid.");
            }
        }
        if (include != null) {
            include = include.stream().distinct().collect(Collectors.toList());
        }
        if (exclude != null) {
            exclude = exclude.stream().distinct().collect(Collectors.toList());
        }
        if (screenDensity == null) {
            return checkEntryPolicyValueCoverAll(include, exclude);
        }
        return checkPolicyValueCovered(screenDensity.policy, screenDensity.value, include, exclude);
    }

    /**
     * check feature countryCode is subset of entry countryCode
     *
     * @param countryCode is the countryCode of feature hap
     * @param entryHaps is the haps that feature matched
     * @return countryCode is valid
     */
    private static boolean checkCountryCodeCovered(CountryCode countryCode, List<HapVerifyInfo> entryHaps)
            throws BundleException {
        List<String> include = null;
        List<String> exclude = null;
        for (HapVerifyInfo hapVerifyInfo : entryHaps) {
            if (hapVerifyInfo.getDistroFilter() == null || hapVerifyInfo.getDistroFilter().countryCode == null) {
                return true;
            }
            if (hapVerifyInfo.getDistroFilter().countryCode.policy == null) {
                LOG.error("HapVerify::checkCountryCodeCovered input none policy.");
                return false;
            }
            if (INCLUDE.equals(hapVerifyInfo.getDistroFilter().countryCode.policy)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().countryCode.value);
            } else if (EXCLUDE.equals(hapVerifyInfo.getDistroFilter().countryCode.policy)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().countryCode.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkCountryCodeCovered input policy is invalid.");
                throw new BundleException("HapVerify::checkCountryCodeCovered input policy is invalid.");
            }
        }
        if (include != null) {
            include = include.stream().distinct().collect(Collectors.toList());
        }
        if (exclude != null) {
            exclude = exclude.stream().distinct().collect(Collectors.toList());
        }
        if (countryCode == null) {
            return checkEntryPolicyValueCoverAll(include, exclude);
        }
        return checkPolicyValueCovered(countryCode.policy, countryCode.value, include, exclude);
    }

    /**
     * check entry policy value covered all value
     *
     * @param include is the collection of included value
     * @param exclude is the collection of excluded value
     * @return entry policy value covered all value
     */
    private static boolean checkEntryPolicyValueCoverAll(List<String> include, List<String> exclude) {
        if (include == null) {
            return exclude == null || exclude.isEmpty();
        }
        return exclude != null && include.containsAll(exclude);
    }

    /**
     * check entry policy value covered all value
     *
     * @param include is the collection of included value
     * @param exclude is the collection of excluded value
     * @return entry policy value covered all value
     */
    private static boolean checkPolicyValueCovered(
        String policy, List<String> value, List<String> include, List<String> exclude) {
        if (value == null || policy == null) {
            LOG.error("checkPolicyValueCovered::failed value is null.");
            return false;
        }
        if (EXCLUDE.equals(policy)) {
            return checkCoveredExcludePolicyValue(value, include, exclude);
        } else if (INCLUDE.equals(policy)) {
            return checkCoveredIncludePolicyValue(value, include, exclude);
        } else {
            return false;
        }
    }

    /**
     * check entry covered feature value when feature policy is exclude
     *
     * @param value is the feature value
     * @param include is the included value of entry
     * @param exclude is the excluded value of entry
     * @return entry policy value covered feature value
     */
    private static boolean checkCoveredExcludePolicyValue(
        List<String> value, List<String> include, List<String> exclude) {
        if (include == null) {
            return exclude == null || value.containsAll(exclude);
        }
        if (exclude == null) {
            return false;
        }
        exclude.removeAll(include);
        return value.containsAll(exclude);
    }

    /**
     * check entry covered feature value when feature policy is include
     *
     * @param value is the feature value
     * @param include is the included value of entry
     * @param exclude is the excluded value of entry
     * @return entry policy value covered feature value
     */
    private static boolean checkCoveredIncludePolicyValue(
        List<String> value, List<String> include, List<String> exclude) {
        if (include == null) {
            return exclude == null || Collections.disjoint(exclude, value);
        }
        if (exclude == null) {
            return include.containsAll(value);
        }
        exclude.removeAll(include);
        return Collections.disjoint(exclude, value);
    }

    /**
     * check dependency is valid
     *
     * @param allHapVerifyInfo is all input hap module
     * @return true if dependency is valid
     * @throws BundleException when input hapVerify is invalid
     */
    private static boolean checkDependencyIsValid(List<HapVerifyInfo> allHapVerifyInfo) throws BundleException {
        if (allHapVerifyInfo.isEmpty()) {
            LOG.error("HapVerify::checkDependencyIsValid failed, input none hap.");
            throw new BundleException("HapVerify::checkDependencyIsValid failed, input none hap.");
        }
        boolean isInstallationFree = allHapVerifyInfo.get(0).isInstallationFree();
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            if (isInstallationFree != hapVerifyInfo.isInstallationFree()) {
                LOG.error("installationFree is different in input hap.");
                return false;
            }
        }
        int depth = isInstallationFree ? SERVICE_DEPTH : APPLICATION_DEPTH;
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            List<HapVerifyInfo> dependencyList = new ArrayList<>();
            dependencyList.add(hapVerifyInfo);
            if (!dfsTraverseDependency(hapVerifyInfo, allHapVerifyInfo, dependencyList, depth)) {
                return false;
            }
            dependencyList.remove(dependencyList.size() - 1);
        }
        return true;
    }

    /**
     * DFS traverse dependency, and check dependency list ia valid
     *
     * @param hapVerifyInfo the first node of dependency list
     * @param allHapVerifyInfo is all input hap module
     * @param dependencyList is the current dependency list
     * @param depth is th limit of depth
     * @return true if dependency list is valid
     * @throws BundleException when input hapVerifyInfo is invalid
     */
    private static boolean dfsTraverseDependency(
        HapVerifyInfo hapVerifyInfo, List<HapVerifyInfo> allHapVerifyInfo,
        List<HapVerifyInfo> dependencyList, int depth) throws BundleException {
        // check dependencyList is valid
        if (checkDependencyListCirculate(dependencyList)) {
            return false;
        }
        if (dependencyList.size() > depth + 1) {
            LOG.error("dependency list depth exceed, dependencyList is "
                    + getHapVerifyInfoListNames(dependencyList) + ".");
            return false;
        }
        for (DependencyItem dependency : hapVerifyInfo.getDependencyItemList()) {
            if (!dependency.getBundleName().equals(hapVerifyInfo.getBundleName())) {
                continue;
            }
            if (!checkDependencyInFileList(dependency, allHapVerifyInfo)) {
                LOG.warning("Dependent module " + dependency.getModuleName() + " missing, check the HSP-Path.");
                continue;
            }
            List<HapVerifyInfo> layerDependencyList = getLayerDependency(
                    dependency.getModuleName(), hapVerifyInfo, allHapVerifyInfo);
            for (HapVerifyInfo item : layerDependencyList) {
                if (FEATURE.equals(item.getModuleType()) || ENTRY.equals(item.getModuleType())) {
                    LOG.error("HAP or HSP cannot depend on HAP" + item.getModuleName() + ".");
                    return false;
                }
                dependencyList.add(item);
                if (!dfsTraverseDependency(item, allHapVerifyInfo, dependencyList, depth)) {
                    return false;
                }
                dependencyList.remove(dependencyList.size() - 1);
            }
        }
        return true;
    }

    private static boolean checkDependencyInFileList(
            DependencyItem dependencyItem, List<HapVerifyInfo> allHapVerifyInfo) {
        String moduleName = dependencyItem.getModuleName();
        String bundleName = dependencyItem.getBundleName();
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            if (moduleName.equals(hapVerifyInfo.getModuleName()) && bundleName.equals(hapVerifyInfo.getBundleName())) {
                return true;
            }
        }
        return false;
    }

    /**
     * get one layer dependency module by moduleName
     *
     * @param moduleName is the dependency moduleName of module
     * @param hapVerifyInfo the first node of dependency list
     * @param allHapVerifyInfo is all input hap module
     * @return a layer dependency list
     */
    private static List<HapVerifyInfo> getLayerDependency(
        String moduleName, HapVerifyInfo hapVerifyInfo, List<HapVerifyInfo> allHapVerifyInfo) throws BundleException {
        List<HapVerifyInfo> layerHapVerifyInfoList = new ArrayList<>();
        for (HapVerifyInfo item : allHapVerifyInfo) {
            if (item.getModuleName().equals(moduleName) && checkModuleJoint(hapVerifyInfo, item)) {
                layerHapVerifyInfoList.add(item);
            }
        }
        return layerHapVerifyInfoList;
    }

    /**
     * check two module is joint
     *
     * @param infoLeft is one hapVerifyInfo
     * @param infoRight is another hapVerifyInfo
     * @return true if dependency list is valid
     */
    private static boolean checkModuleJoint(HapVerifyInfo infoLeft, HapVerifyInfo infoRight) throws BundleException {
        return !checkDuplicatedIsValid(infoLeft, infoRight);
    }

    /**
     * check dependency list is circulate
     *
     * @param dependencyList is current dependency list
     * @return true if dependency list is circulate
     */
    private static boolean checkDependencyListCirculate(List<HapVerifyInfo> dependencyList) throws BundleException {
        for (int i = 0; i < dependencyList.size() - 1; ++i) {
            for (int j = i + 1; j < dependencyList.size(); ++j) {
                if (isSameHapVerifyInfo(dependencyList.get(i), dependencyList.get(j))) {
                    LOG.error("circular dependency, dependencyList is "
                            + getHapVerifyInfoListNames(dependencyList) + ".");
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * check two hapVerifyInfo is same.If two module has same moduleName and joint, they are the same hapVerifyInfo
     *
     * @param infoLeft is one hapVerifyInfo
     * @param infoRight is another hapVerifyInfo
     * @return true two hapVerifyInfo is same
     */
    private static boolean isSameHapVerifyInfo(HapVerifyInfo infoLeft, HapVerifyInfo infoRight) throws BundleException {
        if (!infoLeft.getModuleName().equals(infoRight.getModuleName())) {
            return false;
        }
        return checkModuleJoint(infoLeft, infoRight);
    }

    /**
     * get moduleNames from List<HapVerifyInfo>
     *
     * @param hapVerifyInfoList is hapVerifyInfo list
     * @return true two hapVerifyInfo is same
     */
    private static List<String> getHapVerifyInfoListNames(List<HapVerifyInfo> hapVerifyInfoList) {
        List<String> moduleNames = new ArrayList<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            moduleNames.add((hapVerifyInfo.getModuleName()));
        }
        return moduleNames;
    }

    private static boolean checkAtomicServiceModuleSize(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServiceIsValid failed, hapVerifyInfoList is empty.");
            return false;
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<String> dependencies = getModuleDependency(hapVerifyInfo, hapVerifyInfoList);
            List<HapVerifyInfo> dependenciesInfos = new ArrayList<>();
            for (String module : dependencies) {
                HapVerifyInfo info = findAtomicServiceHapVerifyInfo(module, hapVerifyInfoList);
                dependenciesInfos.add(info);
            }
            long fileSize = hapVerifyInfo.getFileLength();
            for (HapVerifyInfo dependency : dependenciesInfos) {
                fileSize += dependency.getFileLength();
            }
            if (fileSize > ATOMIC_SERVICE_MODULE_SIZE * FILE_LENGTH_1M) {
                LOG.error(hapVerifyInfo.getModuleName() +
                    " and its dependencies size is over 2M in atomicService.");
                return false;
            }
        }
        return true;
    }

    private static boolean checkOldAtomicServiceSize(List<HapVerifyInfo> hapVerifyInfoList) {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkOldAtomicServiceSize failed, hapVerifyInfoList is empty.");
            return false;
        }
        String appType = hapVerifyInfoList.get(0).getAppType();
        boolean installationFree = hapVerifyInfoList.get(0).isInstallationFree();
        boolean isFaModule = !hapVerifyInfoList.get(0).isStageModule();
        if (isFaModule || ATOMIC_SERVICE.equals(appType) || !installationFree) {
            return true;
        }
        if (hapVerifyInfoList.size() == ONE &&
                hapVerifyInfoList.get(0).getFileLength() > ATOMIC_SERVICE_APP_LIMIT_SIZE * FILE_LENGTH_1M) {
            LOG.error("final app size is bigger than 10M when installationFree is true.");
            return false;
        }
        boolean split = false;
        for (HapVerifyInfo info : hapVerifyInfoList) {
            if (!info.getModuleType().equals(ENTRY)) {
                split = true;
                break;
            }
        }
        if (split) {
            // check module size
            long finalAppSize = 0L;
            for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
                finalAppSize += hapVerifyInfo.getFileLength();
                if (hapVerifyInfo.getFileLength() > ATOMIC_SERVICE_MODULE_SIZE * FILE_LENGTH_1M) {
                    LOG.error(hapVerifyInfo.getModuleName() + " is bigger than 2M.");
                    return false;
                }
                if (finalAppSize > ATOMIC_SERVICE_APP_LIMIT_SIZE * FILE_LENGTH_1M) {
                    LOG.error("final app size is bigger than 10M.");
                    return false;
                }
                try {
                    boolean result = checkAtomicServiceModuleSize(hapVerifyInfoList);
                    if (!result) {
                        LOG.error("checkAtomicServiceModuleSize failed.");
                        return false;
                    }
                } catch (BundleException e) {
                    LOG.error("checkAtomicServiceModuleSize failed.");
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean checkAtomicServiceIsValid(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServiceIsValid failed, hapVerifyInfoList is empty.");
            return false;
        }
        String appType = hapVerifyInfoList.get(0).getAppType();
        if (!appType.equals(ATOMIC_SERVICE)) {
            return true;
        }
        // check split and module
        if (!checkAtomicServiceSplitWithModule(hapVerifyInfoList)) {
            LOG.error("checkSplitAndModuleNumbers failed.");
            return false;
        }
        // check main is valid
        if (!checkAtomicServiceMainIsValid(hapVerifyInfoList)) {
            LOG.error("checkAtomicServiceMainIsValid failed.");
            return false;
        }
        // check atomic service type
        if (!checkAtomicServiceTypeIsValid(hapVerifyInfoList)) {
            LOG.error("checkAtomicServiceTypeIsValid failed.");
            return false;
        }
        // check preloads is valid
        if (!checkAtomicServicePreloadsIsValid(hapVerifyInfoList)) {
            LOG.error("checkAtomicServicePreloadsIsValid failed.");
            return false;
        }
        // check file size is valid
        if (!checkFileSizeIsValid(hapVerifyInfoList)) {
            LOG.error("checkFileSizeIsValid failed.");
            return false;
        }
        return true;
    }

    private static boolean checkAtomicServiceSplitWithModule(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServiceSplitWithModule failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkAtomicServiceSplitWithModule failed, hapVerifyInfoList is empty.");
        }
        boolean split = hapVerifyInfoList.get(0).isSplit();
        if (!split) {
            for (HapVerifyInfo info : hapVerifyInfoList) {
                if (!ENTRY.equals(info.getModuleType())) {
                    LOG.error("atomicService can only contain entry module when split is false.");
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean checkAtomicServiceMainIsValid(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServiceMainIsValid failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkAtomicServiceMainIsValid failed, hapVerifyInfoList is empty.");
        }
        String main = hapVerifyInfoList.get(0).getMain();
        boolean split = hapVerifyInfoList.get(0).isSplit();
        if (!split) {
            return true;
        }
        if (main.isEmpty()) {
            LOG.error("atomicService must config main when split is true.");
            return false;
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (main.equals(hapVerifyInfo.getModuleName())) {
                if (!hapVerifyInfo.getModuleType().equals(ENTRY)) {
                    LOG.error("main module must be a entry.");
                    return false;
                } else {
                    return true;
                }
            }
        }
        LOG.error("main is not a module in this app.");
        return false;
    }

    private static boolean checkAtomicServiceTypeIsValid(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServiceTypeIsValid failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkAtomicServiceTypeIsValid failed, hapVerifyInfoList is empty.");
        }
        boolean split = hapVerifyInfoList.get(0).isSplit();
        if (!split) {
            return true;
        }
        String main = hapVerifyInfoList.get(0).getMain();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (main.equals(hapVerifyInfo.getModuleName())) {
                hapVerifyInfo.setAtomicServiceType(MAIN);
            } else {
                hapVerifyInfo.setAtomicServiceType(NORMAL);
            }
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (hapVerifyInfo.getAtomicServiceType().equals(MAIN)) {
                if (!hapVerifyInfo.getModuleType().equals(ENTRY)) {
                    LOG.error("main module must be a entry.");
                    return false;
                }
            } else {
                if (hapVerifyInfo.getModuleType().equals(ENTRY)) {
                    LOG.error("entry module must be a main.");
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean checkAtomicServicePreloadsIsValid(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkAtomicServicePreloadsIsValid failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkAtomicServicePreloadsIsValid failed, hapVerifyInfoList is empty.");
        }
        List<String> moduleNames = new ArrayList<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            moduleNames.add(hapVerifyInfo.getModuleName());
        }
        // check preload module is existed and not self
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<PreloadItem> preloadItems = hapVerifyInfo.getPreloadItems();
            for (PreloadItem preloadItem : preloadItems) {
                String moduleName = preloadItem.getModuleName();
                if (!moduleNames.contains(moduleName)) {
                    LOG.error("preloads config a invalid module " + moduleName +
                            " in " + hapVerifyInfo.getModuleName() + ".");
                    return false;
                }
                if (moduleName.equals(hapVerifyInfo.getModuleName())) {
                    LOG.error("can not preload self, " + hapVerifyInfo.getModuleName() + " preload self.");
                    return false;
                }
            }
        }
        // check feature preload is valid
        HashMap<String, String> moduleNameWithType = new HashMap<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            moduleNameWithType.put(hapVerifyInfo.getModuleName(), hapVerifyInfo.getModuleType());
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<PreloadItem> preloadItems = hapVerifyInfo.getPreloadItems();
            for (PreloadItem preloadItem : preloadItems) {
                String moduleName = preloadItem.getModuleName();
                if (moduleNameWithType.get(moduleName).equals(ENTRY)
                        || moduleNameWithType.get(moduleName).equals(HAR)) {
                    LOG.error("feature or shared can not preload entry or har.");
                    return false;
                }
            }
        }

        return true;
    }

    private static boolean checkFileSizeIsValid(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkFileSizeIsValid failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkFileSizeIsValid failed, hapVerifyInfoList is empty.");
        }
        boolean split = hapVerifyInfoList.get(0).isSplit();
        if (split && !checkFileSizeWhenSplit(hapVerifyInfoList)) {
            LOG.error("file size is invalid when split is true.");
            return false;
        }
        if (!split && !checkFileSizeWhenNotSplit(hapVerifyInfoList)) {
            LOG.error("file size is invalid when split is false.");
            return false;
        }
        return true;
    }

    private static boolean checkFileSizeWhenSplit(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error("checkFileSizeWhenSplit failed, hapVerifyInfoList is empty.");
            throw new BundleException("checkFileSizeWhenSplit failed, hapVerifyInfoList is empty.");
        }
        // check single file length
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (hapVerifyInfo.getFileLength() > ATOMIC_SERVICE_MODULE_SIZE * FILE_LENGTH_1M) {
                LOG.error("input module " + hapVerifyInfo.getModuleName() +
                        " file size is bigger than 2M.");
                return false;
            }
        }
        // check final app file length
        long appLength = 0L;
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            appLength += hapVerifyInfo.getFileLength();
            if (appLength > ATOMIC_SERVICE_APP_LIMIT_SIZE * FILE_LENGTH_1M) {
                LOG.error("The sum of all file size is bigger than 10M.");
                return false;
            }
        }
        try {
            boolean result = checkAtomicServiceModuleSize(hapVerifyInfoList);
            if (!result) {
                LOG.error("checkAtomicServiceModuleSize failed.");
                return false;
            }
        } catch (BundleException e) {
            LOG.error("checkAtomicServiceModuleSize failed.");
            return false;
        }
        return true;
    }

    private static List<String> getModuleDependency(HapVerifyInfo hapVerifyInfo,
                                                    List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        List<String> dependencyModules = new ArrayList<>();
        dependencyModules.addAll(hapVerifyInfo.getDependencies());
        List<String> dependencyItems = hapVerifyInfo.getDependencies();
        for (String dependency : dependencyItems) {
            HapVerifyInfo dependencyHapVerifyInfo = findAtomicServiceHapVerifyInfo(dependency, hapVerifyInfoList);
            if (dependencyHapVerifyInfo == null) {
                LOG.error("can not find module by module name.");
                throw new BundleException("can not find module by module name.");
            }
            List<String> childDependencies = getModuleDependency(dependencyHapVerifyInfo, hapVerifyInfoList);
            for (String childDependency : childDependencies) {
                if (!dependencyModules.contains(childDependency)) {
                    dependencyModules.add(childDependency);
                }
            }
        }
        return dependencyModules;
    }

    private static HapVerifyInfo findAtomicServiceHapVerifyInfo(String moduleName,
                                                                List<HapVerifyInfo> hapVerifyInfoList) {
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (hapVerifyInfo.getModuleName().equals(moduleName)) {
                return hapVerifyInfo;
            }
        }
        return null;
    }

    private static boolean checkFileSizeWhenNotSplit(List<HapVerifyInfo> hapVerifyInfoList) {
        if (hapVerifyInfoList.size() != ONE) {
            LOG.error("must input one module when not split atomicService.");
            return false;
        }
        if (hapVerifyInfoList.get(0).getFileLength() > ATOMIC_SERVICE_APP_LIMIT_SIZE * FILE_LENGTH_1M) {
            LOG.error("input module is bigger than 10M when split is false.");
            return false;
        }
        return true;
    }
}
