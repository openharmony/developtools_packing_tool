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

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * check hap is verify.
 */
class HapVerify {
    private static final String INCLUDE = "include";
    private static final String EXCLUDE = "exclude";
    private static final Log LOG = new Log(HapVerify.class.toString());
    private static final String EMPTY_STRING = "";
    private static final String ENTRY = "entry";
    private static final String FEATURE = "feature";
    private static final String SHARED_LIBRARY = "shared";
    private static final String HAR = "har";
    private static final String REFERENCE_LINK = "FAQ";
    private static final String ATOMIC_SERVICE = "atomicService";
    private static final String TYPE_SHARED = "shared";
    private static final long FILE_LENGTH_1M = 1024 * 1024L;
    private static final double FILE_SIZE_OFFSET_DOUBLE = 0.01d;
    private static final int FILE_SIZE_DECIMAL_PRECISION = 2;
    private static final String HAP_SUFFIX = ".hap";
    private static final String HSP_SUFFIX = ".hsp";

    /**
     * check hap is verify.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard
     */
    public static boolean checkHapIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos == null || hapVerifyInfos.isEmpty()) {
            String errMsg = "Hap verify infos is null or empty.";
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString(errMsg));
            return false;
        }
        // check app variable is same
        if (!checkAppFieldsIsSame(hapVerifyInfos)) {
            String errMsg = "Some app variable is different.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        // check moduleName is valid
        if (!checkModuleNameIsValid(hapVerifyInfos)) {
            return false;
        }
        // check package is valid
        if (!checkPackageNameIsValid(hapVerifyInfos)) {
            String errMsg = "Check packageName duplicated.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        // check entry is valid
        if (!checkEntryIsValid(hapVerifyInfos)) {
            return false;
        }
        // check dependency is valid
        if (!checkDependencyIsValid(hapVerifyInfos)) {
            String errMsg = "Check module dependency is invalid.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        // check atomic service is valid
        if (!checkAtomicServiceIsValid(hapVerifyInfos)) {
            String errMsg = "Check AtomicService valid failed.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        // check ability is valid
        if (!checkAbilityNameIsValid(hapVerifyInfos)) {
            LOG.info("Ability name is duplicated.");
        }
        // check targetModuleName
        if (!checkTargetModuleNameIsExisted(hapVerifyInfos)) {
            String errMsg = "Target module is not found.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        if (!checkCompileSdkIsValid(hapVerifyInfos)) {
            String errMsg = "Compile sdk config is not same.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        if (!checkProxyDataUriIsUnique(hapVerifyInfos)) {
            String errMsg = "uris in proxy data are not unique.";
            LOG.error(PackingToolErrMsg.CHECK_HAP_INVALID.toString(errMsg));
            return false;
        }
        if (!checkContinueTypeIsValid(hapVerifyInfos)) {
            return false;
        }
        return true;
    }

    private static boolean checkContinueTypeIsValid(List<HapVerifyInfo> hapVerifyInfos) {
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (!checkContinueTypeIsValid(hapVerifyInfo)) {
                return false;
            }
        }
        for (int i = 0; i < hapVerifyInfos.size(); i++) {
            for (int j = i + 1; j < hapVerifyInfos.size(); j++) {
                if (!checkContinueTypeIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean checkContinueTypeIsValid(HapVerifyInfo hapVerifyInfo) {
        List<String> abilityNames = hapVerifyInfo.getAbilityNames();
        if (abilityNames.size() < 2) {
            return true;
        }
        for (int i = 0; i < abilityNames.size(); i++) {
            List<String> typeList = hapVerifyInfo.getContinueTypeMap().get(abilityNames.get(i));
            if (typeList == null) {
                continue;
            }
            for (int j = i + 1; j < abilityNames.size(); j++) {
                List<String> typeList2 = hapVerifyInfo.getContinueTypeMap().get(abilityNames.get(j));
                if (typeList2 == null) {
                    continue;
                }
                if (!Collections.disjoint(typeList, typeList2)) {
                    String cause = "Module(" + hapVerifyInfo.getModuleName() + "), Ability(" +
                            abilityNames.get(i) + ") and Ability(" +
                            abilityNames.get(j) + ") have same continueType.\n";
                    cause += "Ability(" + abilityNames.get(i) + ") have continueType: " + typeList + ", ";
                    cause += "Another Ability(" + abilityNames.get(j) + ") have continueType: " + typeList2 + ".";
                    String solution = "Please ensure that the continueType for different abilities do not overlap.";
                    LOG.error(PackingToolErrMsg.CONTINUE_TYPE_INVALID.toString(cause, solution));
                    return false;
                }
            }
        }
        return true;
    }

    private static boolean checkContinueTypeIsValid(HapVerifyInfo hapVerifyInfo, HapVerifyInfo hapVerifyInfo2) {
        if (Collections.disjoint(hapVerifyInfo.getDeviceType(), hapVerifyInfo2.getDeviceType())) {
            return true;
        }
        List<String> typeList = hapVerifyInfo.getContinueTypeMap().values().stream()
                .flatMap(Collection::stream).collect(Collectors.toList());
        List<String> typeList2 = hapVerifyInfo2.getContinueTypeMap().values().stream()
                .flatMap(Collection::stream).collect(Collectors.toList());
        if (!Collections.disjoint(typeList, typeList2)) {
            String cause = "Conflict detected between modules due to overlapping deviceType and continueType:\n" +
                    "- Module(" + hapVerifyInfo.getModuleName() + ") with deviceType: " +
                    hapVerifyInfo.getDeviceType() + " and continueType: " + typeList + "\n" +
                    "- Module(" + hapVerifyInfo2.getModuleName() + ") with deviceType: " +
                    hapVerifyInfo2.getDeviceType() + " and continueType: " + typeList2;

            String solution = "Ensure that the continueType fields in these modules are different. " +
                    "Update either (" + hapVerifyInfo.getModuleName() + ") or (" + hapVerifyInfo2.getModuleName() +
                    ") to avoid continueType or deviceType overlap.";

            LOG.error(PackingToolErrMsg.CONTINUE_TYPE_INVALID.toString(cause, solution));
            return false;
        }
        return true;
    }

    /**
     * check inter-app hsp is valid.
     *
     * @param hapVerifyInfos is the collection of hap infos
     * @return the result
     * @throws BundleException Throws this exception if the json is not standard
     */
    public static boolean checkSharedApppIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos == null || hapVerifyInfos.isEmpty()) {
            String cause = "Hap verify infos is null or empty";
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString(cause));
            return false;
        }
        String moduleName = hapVerifyInfos.get(0).getModuleName();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (!moduleName.equals(hapVerifyInfo.getModuleName())) {
                String cause = "module name is different.";
                String solution = "Ensure that all module have the same module name.";
                LOG.error(PackingToolErrMsg.CHECK_SHARED_APP_INVALID.toString(cause, solution));
                return false;
            }
            if (!hapVerifyInfo.getDependencyItemList().isEmpty()) {
                String cause = "Shared app can not depend on other modules.";
                String solution =
                        "Remove dependencies settings in 'module.json5' and ensure module does not contain dependencies.";
                LOG.error(PackingToolErrMsg.CHECK_SHARED_APP_INVALID.toString(cause, solution));
                return false;
            }
            if (!TYPE_SHARED.equals(hapVerifyInfo.getModuleType())) {
                String cause = "Module type is not shared app.";
                String solution = "Ensure module type is 'shared' for all module.";
                LOG.error(PackingToolErrMsg.CHECK_SHARED_APP_INVALID.toString(cause, solution));
                return false;
            }
        }
        for (int i = 0; i < hapVerifyInfos.size(); i++) {
            for (int j = i + 1; j < hapVerifyInfos.size(); j++) {
                if (!checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    String cause = "There are duplicated modules in the packing file.";
                    String solution = "Ensure that there are no duplicated modules.";
                    LOG.error(PackingToolErrMsg.CHECK_SHARED_APP_INVALID.toString(cause, solution));
                    return false;
                }
            }
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
            String cause = "Hap verify infos is empty.";
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString(cause));
            return false;
        }
        HapVerifyInfo verifyInfo = hapVerifyInfos.get(0);
        Optional<HapVerifyInfo> optional = hapVerifyInfos.stream()
                .filter(hapVerifyInfo -> !hapVerifyInfo.getModuleType().equals(TYPE_SHARED))
                .findFirst();
        if (optional.isPresent()) {
            verifyInfo = optional.get();
        }
        List<HapVerifyInfo> hapList = new ArrayList<>();
        int hspMinCompatibleVersionCodeMax = -1;
        int hspTargetApiVersionMax = -1;
        VerifyCollection verifyCollection = new VerifyCollection();
        verifyCollection.bundleName = verifyInfo.getBundleName();
        verifyCollection.setBundleType(verifyInfo.getBundleType());
        verifyCollection.vendor = verifyInfo.getVendor();
        verifyCollection.versionCode = verifyInfo.getVersion().versionCode;
        verifyCollection.versionName = verifyInfo.getVersion().versionName;
        verifyCollection.compatibleApiVersion = verifyInfo.getApiVersion().getCompatibleApiVersion();
        verifyCollection.releaseType = verifyInfo.getApiVersion().getReleaseType();
        verifyCollection.targetBundleName = verifyInfo.getTargetBundleName();
        verifyCollection.targetPriority = verifyInfo.getTargetPriority();
        verifyCollection.debug = verifyInfo.isDebug();
        verifyCollection.setModuleName(verifyInfo.getModuleName());
        verifyCollection.setModuleType(verifyInfo.getModuleType());
        verifyCollection.setMultiAppMode(verifyInfo.getMultiAppMode());

        List<String> assetAccessGroups = verifyInfo.getAssetAccessGroups();
        String moduleName = verifyInfo.getModuleName();
        Optional<HapVerifyInfo> entryOptional = hapVerifyInfos.stream()
                .filter(hapVerifyInfo -> ENTRY.equals(hapVerifyInfo.getModuleType()))
                .findFirst();
        if (entryOptional.isPresent()) {
            moduleName = entryOptional.get().getModuleName();
            assetAccessGroups = entryOptional.get().getAssetAccessGroups();
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            if (!appFieldsIsSame(verifyCollection, hapVerifyInfo)) {
                LOG.warning("Module: (" + verifyCollection.getModuleName() + ") and Module: (" +
                        hapVerifyInfo.getModuleName() + ") has different values.");
                return false;
            }
            if (!appAssetAccessGroupsIsSame(assetAccessGroups, hapVerifyInfo)) {
                LOG.warning("Module: (" + moduleName + ") and Module: (" +
                        hapVerifyInfo.getModuleName() + ") has different values.");
            }
            if (hapVerifyInfo.getFileType() == HAP_SUFFIX) {
                hapList.add(hapVerifyInfo);
            } else if (hapVerifyInfo.getFileType() == HSP_SUFFIX) {
                if (hapVerifyInfo.getVersion().minCompatibleVersionCode > hspMinCompatibleVersionCodeMax) {
                    hspMinCompatibleVersionCodeMax = hapVerifyInfo.getVersion().minCompatibleVersionCode;
                }
                if (hapVerifyInfo.getApiVersion().getTargetApiVersion() > hspTargetApiVersionMax) {
                    hspTargetApiVersionMax = hapVerifyInfo.getApiVersion().getTargetApiVersion();
                }
            }
        }
        if (!appFieldsIsValid(hapList, hspMinCompatibleVersionCodeMax, hspTargetApiVersionMax)) {
            return false;
        }
        return true;
    }

    private static boolean appFieldsIsValid(List<HapVerifyInfo> hapVerifyInfos, int minCompatibleVersionCode,
        int targetApiVersion) {
        if (hapVerifyInfos.isEmpty()) {
            LOG.warning("Hap verify infos is empty");
            return true;
        }
        HapVerifyInfo hap = hapVerifyInfos.get(0);
        for (HapVerifyInfo hapInfo : hapVerifyInfos) {
            if (hap.getVersion().minCompatibleVersionCode != hapInfo.getVersion().minCompatibleVersionCode ||
                hap.getApiVersion().getTargetApiVersion() != hapInfo.getApiVersion().getTargetApiVersion()) {
                String errMsg = "Hap minCompatibleVersionCode or targetApiVersion different.";
                String solution =
                        "Ensure that the values of 'minCompatibleVersionCode' and 'targetApiVersion' in the module.json file of each HAP module are the same.";
                LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(errMsg, solution));
                return false;
            }
        }
        if (hap.getVersion().minCompatibleVersionCode < minCompatibleVersionCode ||
            hap.getApiVersion().getTargetApiVersion() < targetApiVersion) {
            String cause = "The values of minCompatibleVersionCode or targetApiVersion in the module.json file of the HAP " +
                    "module are smaller than those of the HSP module.";
            String solution = "Ensure that the values of aminCompatibleVersionCode and targetApiVersion in the module.json file " +
                    "of the HAP module are greater than or equal to those of the HSP module.";
            LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(cause, solution));
            return false;
        }
        return true;
    }

    private static boolean appFieldsIsSame(VerifyCollection verifyCollection, HapVerifyInfo hapVerifyInfo) {
        if (hapVerifyInfo.getBundleName().isEmpty() ||
                !verifyCollection.bundleName.equals(hapVerifyInfo.getBundleName())) {
            String errMsg = "Input module bundleName is different.";
            String solution = "Check if the bundleName is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (!verifyCollection.getBundleType().equals(hapVerifyInfo.getBundleType())) {
            String errMsg = "Input module bundleType is different.";
            String solution = "Check if the bundleType is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (verifyCollection.versionCode != hapVerifyInfo.getVersion().versionCode) {
            String errMsg = "Input module versionCode is different.";
            String solution = "Check if the versionCode is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (verifyCollection.compatibleApiVersion != hapVerifyInfo.getApiVersion().getCompatibleApiVersion()) {
            String errMsg = "Input module minApiVersion is different.";
            String solution = "Check if the minApiVersion is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
        }
        if (!verifyCollection.releaseType.equals(hapVerifyInfo.getApiVersion().getReleaseType())) {
            if (verifyCollection.getModuleType().equals(TYPE_SHARED) ||
                    hapVerifyInfo.getModuleType().equals(TYPE_SHARED)) {
                LOG.warning("Module: (" + verifyCollection.getModuleName() + ") and Module: (" +
                        hapVerifyInfo.getModuleName() + ") has different releaseType.");
            } else {
                String errMsg = "Input module releaseType is different.";
                String solution = "Check if the releaseType is the same in different modules.";
                LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
                return false;
            }
        }
        if (!verifyCollection.targetBundleName.equals(hapVerifyInfo.getTargetBundleName())) {
            String errMsg = "Input targetBundleName is different.";
            String solution = "Check if the targetBundleName is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (verifyCollection.targetPriority != hapVerifyInfo.getTargetPriority()) {
            String errMsg = "Input targetPriority is different.";
            String solution = "Check if the targetPriority is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (verifyCollection.debug != hapVerifyInfo.isDebug()) {
            String errMsg = "The debug parameter values are different.";
            String solution = "Check if the debug setting is the same in different modules.";
            LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
            return false;
        }
        if (isEntryOrFeature(verifyCollection.getModuleType()) && isEntryOrFeature(hapVerifyInfo.getModuleType())) {
            if (!verifyCollection.getMultiAppMode().equals(hapVerifyInfo.getMultiAppMode())) {
                String errMsg = "The multiAppMode parameter values are different.";
                String solution = "Check if the multiAppMode is the same in different modules.";
                LOG.error(PackingToolErrMsg.APP_FIELDS_DIFFERENT_ERROR.toString(errMsg, solution));
                return false;
            }
        }
        return true;
    }

    private static boolean appAssetAccessGroupsIsSame(List<String> assetAccessGroups, HapVerifyInfo hapVerifyInfo) {
        if (!new HashSet<>(assetAccessGroups).
                equals(new HashSet<>(hapVerifyInfo.getAssetAccessGroups()))){
            LOG.warning("input module assetAccessGroups is different.");
            return false;
        }
        return true;
    }

    private static boolean isEntryOrFeature(String moduleType) {
        return ENTRY.equals(moduleType) || FEATURE.equals(moduleType);
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
                String cause = "The module name in the HAP infos is empty.";
                String solution = "Ensure that each HAP file contains a valid module name field before verification.";
                LOG.error(PackingToolErrMsg.CHECK_MODULE_NAME_INVALID.toString(cause, solution));
                throw new BundleException("Check moduleName is valid should not be empty.");
            }
            for (int j = i + 1; j < hapVerifyInfos.size(); ++j) {
                if (hapVerifyInfos.get(i).getModuleName().equals(hapVerifyInfos.get(j).getModuleName()) &&
                    !checkDuplicatedIsValid(hapVerifyInfos.get(i), hapVerifyInfos.get(j))) {
                    String cause = "Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                            hapVerifyInfos.get(j).getModuleName() + ") have the same moduleName, " +
                            "please check deviceType or distroFilter of the module.\n" + "Module: " +
                            hapVerifyInfos.get(i).getModuleName() + " has deviceType " + hapVerifyInfos.get(i).getDeviceType() +
                            ".\n" + "Another Module: " + hapVerifyInfos.get(j).getModuleName() + " has deviceType " +
                            hapVerifyInfos.get(j).getDeviceType() + ".";
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(i).getDistroFilter().dump())) {
                        cause += "\n" + "Module: " + hapVerifyInfos.get(i).getModuleName() + " DistroFilter is : " +
                                hapVerifyInfos.get(i).getDistroFilter().dump() + ".";
                    }
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(j).getDistroFilter().dump())) {
                        cause += "\n" +"Another Module: " + hapVerifyInfos.get(j).getModuleName() + " DistroFilter is " +
                                hapVerifyInfos.get(j).getDistroFilter().dump() + ".";
                    }
                    String solution = "Make sure the module name is valid and unique.\n" +
                            "Reference: " + REFERENCE_LINK + ".";
                    LOG.error(PackingToolErrMsg.CHECK_MODULE_NAME_INVALID.toString(cause, solution));
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
                    String cause = "Module: (" + hapVerifyInfos.get(i).getModuleName() + ") and Module: (" +
                        hapVerifyInfos.get(j).getModuleName() + ") have the same packageName, " +
                        "please check deviceType or distroFilter of the module.\n" + "Module: " +
                        hapVerifyInfos.get(i).getModuleName() + " has deviceType " + hapVerifyInfos.get(i).getDeviceType() +
                        ".\n" + "Another Module: " + hapVerifyInfos.get(j).getModuleName() + " has deviceType " +
                        hapVerifyInfos.get(j).getDeviceType() + ".";
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(i).getDistroFilter().dump())) {
                        cause += "\n" + "Module: " + hapVerifyInfos.get(i).getModuleName() + " DistroFilter is : " +
                                hapVerifyInfos.get(i).getDistroFilter().dump() + ".";
                    }
                    if (!EMPTY_STRING.equals(hapVerifyInfos.get(j).getDistroFilter().dump())) {
                        cause += "\n" + "Another Module: " + hapVerifyInfos.get(j).getModuleName() + " DistroFilter is " +
                                hapVerifyInfos.get(j).getDistroFilter().dump() + ".";
                    }
                    String solution = "Make sure package name is valid and unique.\n" +
                            "Reference: " + REFERENCE_LINK + ".";
                    LOG.error(PackingToolErrMsg.CHECK_PACKAGE_NAME_INVALID.toString(cause, solution));
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
                    LOG.warning("Module: " + hapVerifyInfos.get(j).getModuleName() + " has ability "
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
            if (!hapInfo.getTargetBundleName().isEmpty()) {
                return true;
            }
            if (!hapInfo.getTargetModuleName().isEmpty()) {
                internalOverlayHap.add(hapInfo);
                targetModuleList.add(hapInfo.getTargetModuleName());
                continue;
            }
            nonOverlayHap.add(hapInfo);
            if (!SHARED_LIBRARY.equals(hapInfo.getModuleType())) {
                moduleList.add(hapInfo.getModuleName());
            }
        }
        if (internalOverlayHap.isEmpty()) {
            return true;
        }
        if (nonOverlayHap.isEmpty()) {
            LOG.error(PackingToolErrMsg.TARGET_MODULE_NAME_NOT_EXIST.toString(
                    "Target hap modules are needed to pack with overlay hap module."));
            return false;
        }
        if (!moduleList.containsAll(targetModuleList)) {
            List<String> missingModules = new ArrayList<>(targetModuleList);
            missingModules.removeAll(moduleList);
            LOG.error(PackingToolErrMsg.TARGET_MODULE_NAME_NOT_EXIST.toString(
                    "The following target overlay hap modules are missing: " + missingModules));
            return false;
        }


        return true;
    }

    private static boolean checkCompileSdkIsValid(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos.isEmpty()) {
            String cause = "Hap verify infos is empty";
            String solution = "Ensure the App contains at least one module before proceeding.";
            LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(cause, solution));
            return false;
        }
        String compileSdkType = hapVerifyInfos.get(0).getCompileSdkType();
        for (HapVerifyInfo info : hapVerifyInfos) {
            if (!compileSdkType.equals(info.getCompileSdkType())) {
                String cause = "CompileSdkType is not the same for all modules.";
                String solution = "Ensure that all modules has same compileSdkType.";
                LOG.error(PackingToolErrMsg.COMPILE_SDK_TYPE_DIFFERENT.toString(cause, solution));
                return false;
            }
        }
        return true;
    }

    private static boolean checkProxyDataUriIsUnique(List<HapVerifyInfo> hapVerifyInfos) throws BundleException {
        if (hapVerifyInfos.isEmpty()) {
            String cause = "Hap verify infos is empty";
            String solution = "Ensure the App contains at least one module before proceeding.";
            LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(cause, solution));
            return false;
        }
        Set<String> uriSet = new HashSet<>();
        for (HapVerifyInfo info : hapVerifyInfos) {
            for (String uri : info.getProxyDataUris()) {
                if (uriSet.contains(uri)) {
                    String moduleName = info.getModuleName();
                    String cause = "Module(" + moduleName +  ") and uri(" + uri + ") in proxyData settings is duplicated.";
                    String solution = "Ensure that the uri in proxyData is unique across different modules.";
                    LOG.error(PackingToolErrMsg.PROXY_DATA_URI_NOT_UNIQUE.toString(cause, solution));
                    return false;
                } else {
                    uriSet.add(uri);
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
            if (ENTRY.equals(hapVerifyInfo.getModuleType())) {
                entryHapVerifyInfos.add(hapVerifyInfo);
            } else if (FEATURE.equals(hapVerifyInfo.getModuleType())) {
                featureHapVerifyInfos.add(hapVerifyInfo);
            } else if (!SHARED_LIBRARY.equals(hapVerifyInfo.getModuleType())) {
                LOG.warning("Input wrong type module.");
            }
        }
        if (hapVerifyInfos.isEmpty()
                || (entryHapVerifyInfos.isEmpty() && (!SHARED_LIBRARY.equals(hapVerifyInfos.get(0).getBundleType())))) {
            LOG.warning("Warning: has no entry module.");
        }

        for (int i = 0; i < entryHapVerifyInfos.size() - 1; ++i) {
            for (int j = i + 1; j < entryHapVerifyInfos.size(); ++j) {
                if (!checkDuplicatedIsValid(entryHapVerifyInfos.get(i), entryHapVerifyInfos.get(j))) {
                    String cause = "Module(" + entryHapVerifyInfos.get(i).getModuleName() + ") and Module(" +
                            entryHapVerifyInfos.get(j).getModuleName() + ") are entry, " +
                            "please check deviceType or distroFilter of the module.\n" + "Module: " +
                            entryHapVerifyInfos.get(i).getModuleName() + " has deviceType " +
                            entryHapVerifyInfos.get(i).getDeviceType() + ".\n" + "Another Module: " +
                            entryHapVerifyInfos.get(j).getModuleName() + " has deviceType " +
                            entryHapVerifyInfos.get(j).getDeviceType() + ".";

                    if (!EMPTY_STRING.equals(entryHapVerifyInfos.get(i).getDistroFilter().dump())) {
                        cause += "\n" + "Module: " + entryHapVerifyInfos.get(i).getModuleName() + " DistroFilter is : " +
                                entryHapVerifyInfos.get(i).getDistroFilter().dump() + ".";
                    }
                    if (!EMPTY_STRING.equals(entryHapVerifyInfos.get(j).getDistroFilter().dump())) {
                        cause += "\n" + "Another Module: " + entryHapVerifyInfos.get(j).getModuleName() +
                                " DistroFilter is " + entryHapVerifyInfos.get(j).getDistroFilter().dump() + ".";
                    }

                    String solution = 
                            "Make sure the entry module is valid and unique, and the HAP uniqueness check logic passes.\n" +
                            "Reference: " + REFERENCE_LINK + ".";
                    LOG.error(PackingToolErrMsg.CHECK_ENTRY_INVALID.toString(cause, solution));
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
        if (checkDistroFilterDisjoint(hapVerifyInfoLeft.getDistroFilter(), hapVerifyInfoRight.getDistroFilter(),
                                      hapVerifyInfoLeft.getModuleName(), hapVerifyInfoRight.getModuleName())) {
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
    private static boolean checkDistroFilterDisjoint(DistroFilter distroFilterLeft, DistroFilter distroFilterRight,
                                                    String moduleNameLeft, String moduleNameRight)
            throws BundleException {
        if (distroFilterLeft == null || distroFilterRight == null) {
            return false;
        }
        if (distroFilterLeft.apiVersion != null && distroFilterRight.apiVersion != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.apiVersion.policy, distroFilterLeft.apiVersion.value,
                    distroFilterRight.apiVersion.policy, distroFilterRight.apiVersion.value, moduleNameLeft, moduleNameRight)) {
                return true;
            }
        }
        if (distroFilterLeft.screenShape != null && distroFilterRight.screenShape != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenShape.policy, distroFilterLeft.screenShape.value,
                    distroFilterRight.screenShape.policy, distroFilterRight.screenShape.value, moduleNameLeft, moduleNameRight)) {
                return true;
            }
        }
        if (distroFilterLeft.screenDensity != null && distroFilterRight.screenDensity != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenDensity.policy, distroFilterLeft.screenDensity.value,
                    distroFilterRight.screenDensity.policy, distroFilterRight.screenDensity.value, moduleNameLeft, moduleNameRight)) {
                return true;
            }
        }
        if (distroFilterLeft.screenWindow != null && distroFilterRight.screenWindow != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.screenWindow.policy, distroFilterLeft.screenWindow.value,
                    distroFilterRight.screenWindow.policy, distroFilterRight.screenWindow.value, moduleNameLeft, moduleNameRight)) {
                return true;
            }
        }
        if (distroFilterLeft.countryCode != null && distroFilterRight.countryCode != null) {
            if (checkPolicyValueDisjoint(distroFilterLeft.countryCode.policy, distroFilterLeft.countryCode.value,
                    distroFilterRight.countryCode.policy, distroFilterRight.countryCode.value, moduleNameLeft, moduleNameRight)) {
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
                                                    List<String> valueRight, String moduleNameLeft, String ModuleNameRight)
                                                    throws BundleException {
        if (valueLeft == null || valueRight == null) {
            String errMsg = "The variable 'value' in the distributionFilter setting is empty.";
            String solution = "Ensure that all distributionFilter file and filter settings has 'value' setting.";
            solution += "Module " + moduleNameLeft + " and " + ModuleNameRight +  " can be checked in priority.";
            LOG.error(PackingToolErrMsg.CHECK_POLICY_DISJOINT_ERROR.toString(errMsg, solution));
            throw new BundleException("Check policy value disjoint value should not empty.");
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
            String errMsg = "Check distributionFilter 'policy' setting is invalid.";
            String solution = "Ensure all distributionFilter file and filter settings 'policy' value must 'include' or 'exclude'.\n";
            solution += "Module " + moduleNameLeft + " and " + ModuleNameRight +  " can be checked in priority.";
            LOG.error(PackingToolErrMsg.CHECK_POLICY_DISJOINT_ERROR.toString(errMsg, solution));
            throw new BundleException("Check policy value disjoint input policy is invalid.");
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
        Map<String, List<HapVerifyInfo>> deviceHaps = new HashMap<>();
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Entry module(" + hapVerifyInfo.getModuleName() + ") policy is null."));
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Entry module(" + hapVerifyInfo.getModuleName() + ") policy '" + hapVerifyInfo.getDistroFilter().apiVersion.policy +
                    "' is invalid."));
                throw new BundleException("Check ApiVersion covered input policy is invalid.");
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenShape is subset of entry module screenShape failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy is null."));
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenShape is subset of entry module screenShape failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy '" + hapVerifyInfo.getDistroFilter().screenShape.policy +
                    "' is invalid."));
                throw new BundleException("Check ScreenShape covered input policy is invalid.");
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenWindow is subset of entry module screenWindow failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy is null."));
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenWindow is subset of entry module screenWindow failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy '" + hapVerifyInfo.getDistroFilter().screenWindow.policy +
                    "' is invalid."));
                throw new BundleException("Check ScreenWindow covered input policy is invalid.");
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenDensity is subset of entry module screenDensity failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy is null."));
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module screenDensity is subset of entry module screenDensity failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy '" + hapVerifyInfo.getDistroFilter().screenDensity.policy +
                    "' is invalid."));
                throw new BundleException("Check ScreenDensity covered input policy is invalid.");
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module countryCode is subset of entry module countryCode failed, Entry module(" +
                    hapVerifyInfo.getModuleName() + ") policy is null."));
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
                LOG.error(PackingToolErrMsg.CHECK_FEATURE_DISTRO_FILTER_INVALID.toString(
                    "Check feature module countryCode is subset of entry module countryCode failed, Entry module " + 
                    hapVerifyInfo.getModuleName() + ") policy '" + hapVerifyInfo.getDistroFilter().countryCode.policy +
                    "' is invalid."));
                throw new BundleException("Check CountryCode covered input policy is invalid.");
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
            LOG.warning("checkPolicyValueCovered::failed covered value or policy is null.");
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
            String cause = "No module included.";
            String solution = "Ensure the App contains at least one module before proceeding.";
            LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(cause, solution));
            throw new BundleException("HapVerify::checkDependencyIsValid failed, input none hap.");
        }
        boolean isInstallationFree = allHapVerifyInfo.get(0).isInstallationFree();
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            if (isInstallationFree != hapVerifyInfo.isInstallationFree()) {
                String cause = "The installationFree value is different in input hap.";
                String solution = "Ensure that the installationFree field is same for all hap.";
                LOG.error(PackingToolErrMsg.CHECK_DEPENDENCY_INVALID.toString(cause ,solution));
                return false;
            }
        }
        for (HapVerifyInfo hapVerifyInfo : allHapVerifyInfo) {
            List<HapVerifyInfo> dependencyList = new ArrayList<>();
            dependencyList.add(hapVerifyInfo);
            if (!dfsTraverseDependency(hapVerifyInfo, allHapVerifyInfo, dependencyList)) {
                return false;
            }
            dependencyList.remove(dependencyList.size() - 1);
        }
        return true;
    }

    /**
     * DFS traverse dependency, and check dependency list is valid
     *
     * @param hapVerifyInfo the first node of dependency list
     * @param allHapVerifyInfo is all input hap module
     * @param dependencyList is the current dependency list
     * @return true if dependency list is valid
     * @throws BundleException when input hapVerifyInfo is invalid
     */
    private static boolean dfsTraverseDependency(
        HapVerifyInfo hapVerifyInfo, List<HapVerifyInfo> allHapVerifyInfo,
        List<HapVerifyInfo> dependencyList) throws BundleException {
        // check dependencyList is valid
        if (checkDependencyListCirculate(dependencyList)) {
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
                    String cause = "The dependeny module(" + item.getModuleName() + ") type is feature or entry.";
                    String solution = "Remove module dependencies on module (" + item.getModuleName() +
                            ") to ensure the dependency list is valid.";
                    LOG.error(PackingToolErrMsg.DEPENDENCY_LIST_INVALID.toString(cause, solution));
                    return false;
                }
                dependencyList.add(item);
                if (!dfsTraverseDependency(item, allHapVerifyInfo, dependencyList)) {
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
                    String cause = "Circular dependency, dependencyList is " +
                            getHapVerifyInfoListNames(dependencyList) + ".";
                    String solution = "Please check the dependecy against the module name in the list.\n";
                    solution += "Remove circulate dependency module to ensure the dependency list is valid.";
                    LOG.error(PackingToolErrMsg.DEPENDENCY_LIST_INVALID.toString(cause, solution));
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
            LOG.error(PackingToolErrMsg.CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toString());
            return false;
        }
        int entryLimit = hapVerifyInfoList.get(0).getEntrySizeLimit();
        int notEntryLimit = hapVerifyInfoList.get(0).getNotEntrySizeLimit();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<String> dependencies = getModuleDependency(hapVerifyInfo, hapVerifyInfoList);
            List<HapVerifyInfo> dependenciesInfos = new ArrayList<>();
            for (String module : dependencies) {
                HapVerifyInfo info = findAtomicServiceHapVerifyInfo(module, hapVerifyInfoList);
                dependenciesInfos.add(info);
            }
            long fileSize = hapVerifyInfo.getFileLength();
            for (HapVerifyInfo dependency : dependenciesInfos) {
                if (dependency == null) {
                    continue;
                }
                fileSize += dependency.getFileLength();
            }
            if (hapVerifyInfo.getModuleType().equals(ENTRY) && (fileSize >= entryLimit * FILE_LENGTH_1M)) {
                String errMsg = "module " + hapVerifyInfo.getModuleName() + " and it's dependencies size sum is " +
                        getCeilFileSize(fileSize, entryLimit) + "MB, which is overlarge than " + entryLimit + "MB.";
                LOG.error(PackingToolErrMsg.CHECK_ATOMIC_SERVICE_MODULE_SIZE.toString(errMsg));
                return false;
            }
            if (!hapVerifyInfo.getModuleType().equals(ENTRY) && (fileSize >= notEntryLimit * FILE_LENGTH_1M)) {
                String errMsg = "module " + hapVerifyInfo.getModuleName() + " and it's dependencies size sum is " +
                        getCeilFileSize(fileSize, notEntryLimit) + "MB, which is overlarge than " + notEntryLimit + "MB.";
                LOG.error(PackingToolErrMsg.CHECK_ATOMIC_SERVICE_MODULE_SIZE.toString(errMsg));
                return false;
            }
        }
        return true;
    }

    private static double getCeilFileSize(long fileSize, int sizeLimit) {
        double threshold = Double.valueOf(sizeLimit) + FILE_SIZE_OFFSET_DOUBLE;
        double size = new BigDecimal((float) fileSize
                / FILE_LENGTH_1M).setScale(FILE_SIZE_DECIMAL_PRECISION, BigDecimal.ROUND_HALF_UP).doubleValue();
        if (size < threshold && size >= sizeLimit) {
            size = threshold;
        }
        return size;
    }

    private static Map<String, List<HapVerifyInfo>> getDeviceHapVerifyInfoMap(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error(PackingToolErrMsg.CHECK_HAP_VERIFY_INFO_LIST_EMPTY.toString());
            throw new BundleException("getDeviceHapVerifyInfoMap failed, hapVerifyInfoList is empty.");
        }
        Map<String, List<HapVerifyInfo>> deviceInfoMap = new HashMap<String, List<HapVerifyInfo>>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<String> deviceTypes = hapVerifyInfo.getDeviceType();
            for (String device : deviceTypes) {
                if (!deviceInfoMap.containsKey(device)) {
                    List<HapVerifyInfo> infos = new ArrayList<>();
                    infos.add(hapVerifyInfo);
                    deviceInfoMap.put(device, infos);
                } else {
                    deviceInfoMap.get(device).add(hapVerifyInfo);
                }
            }
        }
        return deviceInfoMap;
    }

    private static boolean checkAtomicServiceIsValid(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            String cause = "No module included.";
            String solution = "Ensure the App contains at least one module before proceeding.";
            LOG.error(PackingToolErrMsg.CHECK_APP_FIELDS_INVALID.toString(cause, solution));
            return false;
        }
        String bundleType = hapVerifyInfoList.get(0).getBundleType();
        if (!bundleType.equals(ATOMIC_SERVICE)) {
            return true;
        }
        boolean isStage = hapVerifyInfoList.get(0).isStageModule();
        if (!isStage) {
            return true;
        }
        // check preloads is valid
        Map<String, List<HapVerifyInfo>> deviceInfoMap = getDeviceHapVerifyInfoMap(hapVerifyInfoList);
        for (String device : deviceInfoMap.keySet()) {
            List<HapVerifyInfo> hapVerifyInfos = deviceInfoMap.get(device);
            if (!checkAtomicServicePreloadsIsValid(hapVerifyInfos)) {
                LOG.error(PackingToolErrMsg.CHECK_ATOMICSERVICE_INVALID.toString(
                        "Check whether AtomicService preloads are valid failed on device "+ device + "."));
                return false;
            }
        }

        return true;
    }

    private static boolean checkAtomicServiceSumLimit(List<HapVerifyInfo>hapVerifyInfos) {
        int sumLimit = hapVerifyInfos.get(0).getSumSizeLimit();
        if (!hapVerifyInfos.get(0).isStageModule()) {
            return true;
        }
        long fileSize = 0L;
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfos) {
            fileSize += hapVerifyInfo.getFileLength();
            if (fileSize >= sumLimit * FILE_LENGTH_1M) {
                LOG.error("The total file size is " + getCeilFileSize(fileSize, sumLimit) +
                        "MB, greater than " + sumLimit + "MB.");
                return false;
            }
        }
        return true;
    }

    private static boolean checkAtomicServicePreloadsIsValid(List<HapVerifyInfo> hapVerifyInfoList)
            throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            String cause = "Hap verify infos is empty.";
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString(cause));
            throw new BundleException("checkAtomicServicePreloadsIsValid failed, hapVerifyInfoList is empty.");
        }
        List<String> moduleNames = new ArrayList<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            moduleNames.add(hapVerifyInfo.getModuleName());
        }
        // check preload module is existed and not self
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<String> preloadModuleName = new ArrayList<>();
            List<PreloadItem> preloadItems = hapVerifyInfo.getPreloadItems();
            for (PreloadItem preloadItem : preloadItems) {
                String moduleName = preloadItem.getModuleName();
                if (preloadModuleName.contains(moduleName)) {
                    LOG.error(PackingToolErrMsg.ATOMICSERVICE_PRELOADS_INVALID.toString("Preloads a duplicate module, "
                        + moduleName + " cannot on module " + hapVerifyInfo.getModuleName() + "."));
                    return false;
                }
                preloadModuleName.add(moduleName);
                if (!moduleNames.contains(moduleName)) {
                    LOG.error(PackingToolErrMsg.ATOMICSERVICE_PRELOADS_INVALID.toString("Preloads a not exist module, "
                        + moduleName + " cannot on module " + hapVerifyInfo.getModuleName() + "."));
                    return false;
                }
                if (moduleName.equals(hapVerifyInfo.getModuleName())) {
                    LOG.error(PackingToolErrMsg.ATOMICSERVICE_PRELOADS_INVALID.toString("Can not preload self, module "
                        + hapVerifyInfo.getModuleName() + " cannot preload self."));
                    return false;
                }
            }
        }
        // check feature preload is valid
        Map<String, String> moduleNameWithType = new HashMap<>();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            moduleNameWithType.put(hapVerifyInfo.getModuleName(), hapVerifyInfo.getModuleType());
        }
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            List<PreloadItem> preloadItems = hapVerifyInfo.getPreloadItems();
            for (PreloadItem preloadItem : preloadItems) {
                String moduleName = preloadItem.getModuleName();
                if (moduleNameWithType.get(moduleName).equals(ENTRY)
                        || moduleNameWithType.get(moduleName).equals(HAR)) {
                    LOG.error(PackingToolErrMsg.ATOMICSERVICE_PRELOADS_INVALID.toString(
                        "feature or shared can not preload entry or har, module(" + hapVerifyInfo.getModuleName() +
                        ") cannot preloads module(" + moduleNameWithType.get(moduleName) + ")."));
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * check file size is valid from List<HapVerifyInfo>
     *
     * @param hapVerifyInfoList is hapVerifyInfo list
     * @return true file size is under limit
     */
    public static boolean checkFileSizeIsValid(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString("Hap verify infos is empty."));
        }
        if (!checkFileSize(hapVerifyInfoList)) {
            return false;
        }
        return true;
    }

    private static boolean checkFileSize(List<HapVerifyInfo> hapVerifyInfoList) throws BundleException {
        if (hapVerifyInfoList.isEmpty()) {
            LOG.error(PackingToolErrMsg.HAP_VERIFY_INFO_EMPTY.toString("Hap verify infos is empty."));
            throw new BundleException("checkFileSizeWhenSplit failed, hapVerifyInfoList is empty.");
        }
        // check single file length
        int entryLimit = hapVerifyInfoList.get(0).getEntrySizeLimit();
        int notEntryLimit = hapVerifyInfoList.get(0).getNotEntrySizeLimit();
        for (HapVerifyInfo hapVerifyInfo : hapVerifyInfoList) {
            if (hapVerifyInfo.getModuleType().equals(ENTRY) &&
                    (hapVerifyInfo.getFileLength() >= entryLimit * FILE_LENGTH_1M)) {
                String errMsg = "module " + hapVerifyInfo.getModuleName() + "'s size is " +
                        getCeilFileSize(hapVerifyInfo.getFileLength(), entryLimit) +
                        "MB, which is overlarge than " + entryLimit + "MB.";
                LOG.error(PackingToolErrMsg.CHECK_FILE_SIZE_INVALID.toString(errMsg));
                return false;
            }
            if (!hapVerifyInfo.getModuleType().equals(ENTRY) &&
                    (hapVerifyInfo.getFileLength() >= notEntryLimit * FILE_LENGTH_1M)) {
                    String errMsg = "module " + hapVerifyInfo.getModuleName() + "'s size is " +
                            getCeilFileSize(hapVerifyInfo.getFileLength(), notEntryLimit) +
                            "MB, which is overlarge than " + notEntryLimit + "MB.";
                LOG.error(PackingToolErrMsg.CHECK_FILE_SIZE_INVALID.toString(errMsg));
                return false;
            }
        }

        Map<String, List<HapVerifyInfo>> deviceInfoMap = getDeviceHapVerifyInfoMap(hapVerifyInfoList);
        for (String device : deviceInfoMap.keySet()) {
            List<HapVerifyInfo>hapVerifyInfoList1 = deviceInfoMap.get(device);
            if (!checkAtomicServiceModuleSize(hapVerifyInfoList1)) {
                String errMsg = "Check AtomicService module size failed on device " + device + ".";
                LOG.error(PackingToolErrMsg.CHECK_FILE_SIZE_INVALID.toString(errMsg));
                return false;
            }
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
                continue;
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
}
