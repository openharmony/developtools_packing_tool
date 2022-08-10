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

class HapVerify {
    private static final String INCLUDE = "include";
    private static final String EXCLUDE = "exclude";
    private static final Log LOG = new Log(HapVerify.class.toString());
    private static final int SERVICE_DEPTH = 2;
    private static final int APPLICATION_DEPTH = 5;

    /**
     * check hap is verify.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard
     */
    public static boolean checkHapIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos == null || hapVerifyInfos.isEmpty()) {
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
        // check entry is valid
        if (!checkEntryIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, entry is not valid!");
            return false;
        }
        // check dependency is valid
        if (!checkDependencyIsValid(hapVerifyInfos)) {
            LOG.error("HapVerify::checkHapIsValid failed, dependency is invalid");
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
            LOG.error("HapVerify::checkAppVariableIsSame failed, hapVerifyInfos is empty");
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
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (hapVerifyInfo.getBundleName().isEmpty() ||
                    !verifyCollection.bundleName.equals(hapVerifyInfo.getBundleName())) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, bundleName is invalid!");
                return false;
            }
            if (hapVerifyInfo.getVendor().isEmpty() || !verifyCollection.vendor.equals(hapVerifyInfo.getVendor())) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, vendor is invalid!");
                return false;
            }
            if (verifyCollection.versionCode != hapVerifyInfo.getVersion().versionCode) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, versionCode is different!");
                return false;
            }
            if (!verifyCollection.versionName.equals(hapVerifyInfo.getVersion().versionName)) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, versionName is different!");
                return false;
            }
            if (verifyCollection.minCompatibleVersionCode != hapVerifyInfo.getVersion().minCompatibleVersionCode) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, minCompatibleVersionCode is different!");
                return false;
            }
            if (verifyCollection.compatibleApiVersion != hapVerifyInfo.getApiVersion().getCompatibleApiVersion()) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, minApiVersion is different!");
                return false;
            }
            if (verifyCollection.targetApiVersion != hapVerifyInfo.getApiVersion().getTargetApiVersion()) {
                LOG.error("HapVerify::checkAppVariableIsSame failed, targetApiVersion is different!");
                return false;
            }
            if (!verifyCollection.releaseType.equals(hapVerifyInfo.getApiVersion().getReleaseType())) {
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
            if (hapVerifyInfos.get(i).getModuleName().isEmpty()) {
                LOG.error("HapVerify::checkModuleNameIsValid should not be empty!");
                throw new BundleException("HapVerify::checkModuleNameIsValid should not be empty!");
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).getModuleName().equals(hapVerifyInfos.get(j).getModuleName()) &&
                        !checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    LOG.error("HapVerify::checkModuleNameIsValid " + hapVerifyInfos.get(i).getModuleName() +
                            " " + hapVerifyInfos.get(j).getModuleName() + " duplicated!");
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
                    LOG.error("HapVerify::checkPackageNameIsValid " + hapVerifyInfos.get(i).getPackageName() +
                            " " + hapVerifyInfos.get(j).getPackageName() + " duplicated!");
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
                LOG.error("HapVerify::checkAbilityNameIsValid " +
                        hapVerifyInfo.getModuleName() + " ability duplicated!");
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
                    LOG.error("HapVerify::checkAbilityNameIsValid " + hapVerifyInfos.get(i).getModuleName() +
                            " " + hapVerifyInfos.get(j).getModuleName() + " ability duplicated!");
                    return false;
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
        List<HapVerifyInfo> featureHapVerifyInfos = new ArrayList<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (hapVerifyInfo.isEntry()) {
                entryHapVerifyInfos.add(hapVerifyInfo);
            } else {
                featureHapVerifyInfos.add(hapVerifyInfo);
            }
        }

        for (int i = 0; i < entryHapVerifyInfos.size() - 1; ++i) {
            for (int j = i + 1; j < entryHapVerifyInfos.size(); ++j) {
                if (!checkDuplicatedIsValid(entryHapVerifyInfos.get(i), entryHapVerifyInfos.get(j))) {
                    LOG.error("HapVerify::checkEntryIsValid failed, " + entryHapVerifyInfos.get(i).getModuleName() +
                            " " + entryHapVerifyInfos.get(j).getModuleName() + " entry duplicated!");
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
            LOG.error("HapVerify::checkPolicyValueDisjoint value should not empty!");
            throw new BundleException("HapVerify::checkPolicyValueDisjoint value should not empty!");
        }
        if (policyLeft.equals(EXCLUDE) && policyRight.equals(INCLUDE)) {
            if (valueRight.isEmpty() || valueLeft.containsAll(valueRight)) {
                return true;
            }
        } else if (policyLeft.equals(INCLUDE) && policyRight.equals(INCLUDE)) {
            if (Collections.disjoint(valueLeft, valueRight)) {
                return true;
            }
        } else if (policyLeft.equals(INCLUDE) && policyRight.equals(EXCLUDE)) {
            if (valueLeft.isEmpty() || valueRight.containsAll(valueLeft)) {
                return true;
            }
        } else if (policyLeft.equals(EXCLUDE) && policyRight.equals(EXCLUDE)) {
            return false;
        } else {
            LOG.error("HapVerify::checkPolicyValueDisjoint input policy is invalid!");
            throw new BundleException("HapVerify::checkPolicyValueDisjoint input policy is invalid!");
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
                LOG.error("HapVerify::checkFeature " + device + " has no entry!");
                return false;
            }
            List<HapVerifyInfo> entryHaps = deviceHap.get(device);
            if (!checkFeatureDistroFilter(featureHap, entryHaps)) {
                LOG.error("HapVerify::checkFeature failed, " + featureHap.getModuleName() +
                        " distroFilter has not covered!");
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
            LOG.error("HapVerify::checkFeatureDistroFilter failed, apiVersion is not covered!");
            return false;
        }
        if (!checkScreenShapeCovered(featureHap.getDistroFilter().screenShape, entryHaps)) {
            LOG.error("HapVerify::checkFeatureDistroFilter failed, screenShape is not covered!");
            return false;
        }
        if (!checkScreenWindowCovered(featureHap.getDistroFilter().screenWindow, entryHaps)) {
            LOG.error("HapVerify::checkFeatureDistroFilter failed, screenWindow is not covered!");
            return false;
        }
        if (!checkScreenDensityCovered(featureHap.getDistroFilter().screenDensity, entryHaps)) {
            LOG.error("HapVerify::checkFeatureDistroFilter failed, screenDensity is not covered!");
            return false;
        }
        if (!checkCountryCodeCovered(featureHap.getDistroFilter().countryCode, entryHaps)) {
            LOG.error("HapVerify::checkFeatureDistroFilter failed, countryCode is not covered!");
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
                LOG.error("HapVerify::checkApiVersionCovered input none policy!");
                return false;
            }
            if (hapVerifyInfo.getDistroFilter().apiVersion.policy.equals(INCLUDE)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                // take collection of two include value
                include.addAll(hapVerifyInfo.getDistroFilter().apiVersion.value);
            } else if (hapVerifyInfo.getDistroFilter().apiVersion.policy.equals(EXCLUDE)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                // take intersection of two exclude value
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().apiVersion.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkApiVersionCovered input policy is invalid!");
                throw new BundleException("HapVerify::checkApiVersionCovered input policy is invalid!");
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
                LOG.error("HapVerify::checkScreenShapeCovered input none policy!");
                return false;
            }
            if (hapVerifyInfo.getDistroFilter().screenShape.policy.equals(INCLUDE)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenShape.value);
            } else if (hapVerifyInfo.getDistroFilter().screenShape.policy.equals(EXCLUDE)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenShape.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenShapeCovered input policy is invalid!");
                throw new BundleException("HapVerify::checkScreenShapeCovered input policy is invalid!");
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
                LOG.error("HapVerify::checkScreenWindowCovered input none policy!");
                return false;
            }
            if (hapVerifyInfo.getDistroFilter().screenWindow.policy.equals(INCLUDE)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenWindow.value);
            } else if (hapVerifyInfo.getDistroFilter().screenWindow.policy.equals(EXCLUDE)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenWindow.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenWindowCovered input policy is invalid!");
                throw new BundleException("HapVerify::checkScreenWindowCovered input policy is invalid!");
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
                LOG.error("HapVerify::checkScreenDensityCovered input none policy!");
                return false;
            }
            if (hapVerifyInfo.getDistroFilter().screenDensity.policy.equals(INCLUDE)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().screenDensity.value);
            } else if (hapVerifyInfo.getDistroFilter().screenDensity.policy.equals(EXCLUDE)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().screenDensity.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkScreenDensityCovered input policy is invalid!");
                throw new BundleException("HapVerify::checkScreenDensityCovered input policy is invalid!");
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
                LOG.error("HapVerify::checkCountryCodeCovered input none policy!");
                return false;
            }
            if (hapVerifyInfo.getDistroFilter().countryCode.policy.equals(INCLUDE)) {
                if (include == null) {
                    include = new ArrayList<>();
                }
                include.addAll(hapVerifyInfo.getDistroFilter().countryCode.value);
            } else if (hapVerifyInfo.getDistroFilter().countryCode.policy.equals(EXCLUDE)) {
                if (exclude == null) {
                    exclude = new ArrayList<>();
                }
                exclude = Stream.of(exclude, hapVerifyInfo.getDistroFilter().countryCode.value).
                        flatMap(Collection::stream).distinct().collect(Collectors.toList());
            } else {
                LOG.error("HapVerify::checkCountryCodeCovered input policy is invalid!");
                throw new BundleException("HapVerify::checkCountryCodeCovered input policy is invalid!");
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
    private static boolean checkPolicyValueCovered(String policy, List<String> value,
                                                   List<String> include, List<String> exclude) {
        if (value == null || policy == null) {
            LOG.error("checkPolicyValueCovered::failed value is null!");
            return false;
        }
        if (policy.equals(EXCLUDE)) {
            return checkCoveredExcludePolicyValue(value, include, exclude);
        } else if (policy.equals(INCLUDE)) {
            return checkCoveredIncludePolicyValue(value, include, exclude);
        }
        return false;
    }

    /**
     * check entry covered feature value when feature policy is exclude
     *
     * @param value is the feature value
     * @param include is the included value of entry
     * @param exclude is the excluded value of entry
     * @return entry policy value covered feature value
     */
    private static boolean checkCoveredExcludePolicyValue(List<String> value, List<String> include,
                                                          List<String> exclude) {
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
    private static boolean checkCoveredIncludePolicyValue(List<String> value, List<String> include,
                                                          List<String> exclude) {
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
            LOG.error("HapVerify::checkDependencyIsValid failed, input none hap!");
            throw new BundleException("HapVerify::checkDependencyIsValid failed, input none hap!");
        }
        boolean installationFree = allHapVerifyInfo.get(0).isInstallationFree();
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            if (installationFree != hapVerifyInfo.isInstallationFree()) {
                LOG.error("HapVerify::checkDependencyIsValid installationFree is different in input hap!");
                return false;
            }
        }
        int depth = installationFree ? SERVICE_DEPTH : APPLICATION_DEPTH;
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
    private static boolean dfsTraverseDependency(HapVerifyInfo hapVerifyInfo, List<HapVerifyInfo> allHapVerifyInfo,
                                                 List<HapVerifyInfo> dependencyList, int depth) throws BundleException {
        // check dependencyList is valid
        if (checkDependencyListCirculate(dependencyList)) {
            return false;
        }
        if (dependencyList.size() > depth + 1) {
            LOG.error("HapVerify::DFSTraverseDependency depth exceed, dependencyList is "
                    +  getHapVerifyInfoListNames(dependencyList));
            return false;
        }
        for (String dependency : hapVerifyInfo.getDependencies()) {
            List<HapVerifyInfo> layerDependencyList = getLayerDependency(dependency, hapVerifyInfo, allHapVerifyInfo);
            for (HapVerifyInfo item : layerDependencyList) {
                dependencyList.add(item);
                if (!dfsTraverseDependency(item, allHapVerifyInfo, dependencyList, depth)) {
                    return false;
                }
                dependencyList.remove(dependencyList.size() - 1);
            }
        }
        return true;
    }

    /**
     * get one layer dependency module by moduleName
     *
     * @param moduleName is the dependency moduleName of module
     * @param hapVerifyInfo the first node of dependency list
     * @param allHapVerifyInfo is all input hap module
     * @return a layer dependency list
     */
    private static List<HapVerifyInfo> getLayerDependency(String moduleName, HapVerifyInfo hapVerifyInfo,
                                                          List<HapVerifyInfo> allHapVerifyInfo) throws BundleException {
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
                    LOG.error("HapVerify::checkDependencyListIsValid circular dependency, dependencyList is "
                            + getHapVerifyInfoListNames(dependencyList));
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
}
