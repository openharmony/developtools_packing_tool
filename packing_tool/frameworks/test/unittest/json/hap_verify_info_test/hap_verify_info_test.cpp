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

#include <gtest/gtest.h>
#include <cstdlib>
#include <string>

#define private public
#define protected public
#include "hap_verify_info.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;
namespace OHOS {
class HapVerifyInfoTest : public testing::Test {
public:
    HapVerifyInfoTest() {}
    virtual ~HapVerifyInfoTest() {}
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HapVerifyInfoTest::SetUpTestCase() {}
void HapVerifyInfoTest::TearDownTestCase() {}
void HapVerifyInfoTest::SetUp() {}
void HapVerifyInfoTest::TearDown() {}

/*
 * @tc.name: SetBundleName_0100
 * @tc.desc: SetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetBundleName_0100, Function | MediumTest | Level1)
{
    std::string bundleName = "com.example.bundleName";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleName(bundleName);
    EXPECT_EQ(hapVerifyInfo.GetBundleName(), bundleName);
}

/*
 * @tc.name: SetVendor_0100
 * @tc.desc: SetVendor.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetVendor_0100, Function | MediumTest | Level1)
{
    std::string vendor = "com.example.vendor";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetVendor(vendor);
    EXPECT_EQ(hapVerifyInfo.GetVendor(), vendor);
}

/*
 * @tc.name: SetVersion_0100
 * @tc.desc: SetVersion.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::Version version;
    version.minCompatibleVersionCode = 1;
    version.versionCode = 2;
    version.versionName = "1.0.0";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetVersion(version);
    auto versionOut = hapVerifyInfo.GetVersion();
    EXPECT_EQ(versionOut.minCompatibleVersionCode, version.minCompatibleVersionCode);
    EXPECT_EQ(versionOut.versionCode, version.versionCode);
    EXPECT_EQ(versionOut.versionName, version.versionName);
}

/*
 * @tc.name: SetApiVersion_0100
 * @tc.desc: SetApiVersion.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetApiVersion_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::ModuleApiVersion apiVersion;
    apiVersion.compatibleApiVersion = 1;
    apiVersion.releaseType = "releaseType";
    apiVersion.targetApiVersion = 1;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetApiVersion(apiVersion);
    auto apiVersionOut = hapVerifyInfo.GetApiVersion();
    EXPECT_EQ(apiVersionOut.compatibleApiVersion, apiVersion.compatibleApiVersion);
    EXPECT_EQ(apiVersionOut.releaseType, apiVersion.releaseType);
    EXPECT_EQ(apiVersionOut.targetApiVersion, apiVersion.targetApiVersion);
}

/*
 * @tc.name: SetModuleName_0100
 * @tc.desc: SetModuleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetModuleName_0100, Function | MediumTest | Level1)
{
    std::string moduleName = "com.example.moduleName";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetModuleName(moduleName);
    EXPECT_EQ(hapVerifyInfo.GetModuleName(), moduleName);
}

/*
 * @tc.name: SetPackageName_0100
 * @tc.desc: SetPackageName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetPackageName_0100, Function | MediumTest | Level1)
{
    std::string packageName = "com.example.packageName";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetPackageName(packageName);
    EXPECT_EQ(hapVerifyInfo.GetPackageName(), packageName);
}

/*
 * @tc.name: SetAbilityNames_0100
 * @tc.desc: SetAbilityNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetAbilityNames_0100, Function | MediumTest | Level1)
{
    std::list<std::string> abilityNames = {"abilityName1", "abilityName2", "abilityName3"};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetAbilityNames(abilityNames);
    auto abilityNamesOut = hapVerifyInfo.GetAbilityNames();
    EXPECT_EQ(abilityNamesOut, abilityNames);
}

/*
 * @tc.name: AddAbilityNames_0100
 * @tc.desc: AddAbilityNames.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, AddAbilityNames_0100, Function | MediumTest | Level1)
{
    std::list<std::string> nameList = {"abilityNameInsert"};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.AddAbilityNames(nameList);
    EXPECT_FALSE(hapVerifyInfo.abilityNames_.empty());
    EXPECT_EQ(hapVerifyInfo.abilityNames_.back(), nameList.back());
}

/*
 * @tc.name: SetDistroFilter_0100
 * @tc.desc: SetDistroFilter.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetDistroFilter_0100, Function | MediumTest | Level1)
{
    std::list<std::string> value1 = {"value1"};
    OHOS::AppPackingTool::ApiVersion apiVersion;
    apiVersion.policy = "policy1";
    apiVersion.value = value1;
    std::list<std::string> value2 = {"value2"};
    OHOS::AppPackingTool::ScreenShape screenShape;
    screenShape.policy = "policy2";
    screenShape.value = value2;
    std::list<std::string> value3 = {"value3"};
    OHOS::AppPackingTool::ScreenDensity screenDensity;
    screenDensity.policy = "policy3";
    screenDensity.value = value3;
    std::list<std::string> value4 = {"value4"};
    OHOS::AppPackingTool::ScreenWindow screenWindow;
    screenWindow.policy = "policy4";
    screenWindow.value = value4;
    std::list<std::string> value5 = {"value5"};
    OHOS::AppPackingTool::CountryCode countryCode = {"policy5", value5};
    countryCode.policy = "policy5";
    countryCode.value = value5;
    OHOS::AppPackingTool::DistroFilter distroFilter;
    distroFilter.apiVersion = apiVersion;
    distroFilter.screenShape = screenShape;
    distroFilter.screenDensity = screenDensity;
    distroFilter.screenWindow = screenWindow;
    distroFilter.countryCode = countryCode;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDistroFilter(distroFilter);
    auto distroFilterOut = hapVerifyInfo.GetDistroFilter();
    EXPECT_EQ(distroFilterOut.apiVersion.policy, distroFilter.apiVersion.policy);
    EXPECT_EQ(distroFilterOut.screenShape.policy, distroFilter.screenShape.policy);
    EXPECT_EQ(distroFilterOut.screenDensity.policy, distroFilter.screenDensity.policy);
    EXPECT_EQ(distroFilterOut.screenWindow.policy, distroFilter.screenWindow.policy);
    EXPECT_EQ(distroFilterOut.countryCode.policy, distroFilter.countryCode.policy);
    EXPECT_EQ(distroFilterOut.apiVersion.value, distroFilter.apiVersion.value);
    EXPECT_EQ(distroFilterOut.screenShape.value, distroFilter.screenShape.value);
    EXPECT_EQ(distroFilterOut.screenDensity.value, distroFilter.screenDensity.value);
    EXPECT_EQ(distroFilterOut.screenWindow.value, distroFilter.screenWindow.value);
    EXPECT_EQ(distroFilterOut.countryCode.value, distroFilter.countryCode.value);
}

/*
 * @tc.name: SetDeviceTypes_0100
 * @tc.desc: SetDeviceTypes.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetDeviceTypes_0100, Function | MediumTest | Level1)
{
    std::list<std::string> deviceTypes = {"deviceType1", "deviceType2", "deviceType3"};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDeviceTypes(deviceTypes);
    EXPECT_EQ(hapVerifyInfo.GetDeviceTypes(), deviceTypes);
}

/*
 * @tc.name: SetStageModule_0100
 * @tc.desc: SetStageModule.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetStageModule_0100, Function | MediumTest | Level1)
{
    bool isStageModule = true;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetStageModule(isStageModule);
    EXPECT_EQ(hapVerifyInfo.IsStageModule(), isStageModule);
}

/*
 * @tc.name: SetModuleType_0100
 * @tc.desc: SetModuleType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetModuleType_0100, Function | MediumTest | Level1)
{
    std::string moduleType = "com.example.moduleType";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetModuleType(moduleType);
    EXPECT_EQ(hapVerifyInfo.GetModuleType(), moduleType);
}

/*
 * @tc.name: SetInstallationFree_0100
 * @tc.desc: SetInstallationFree.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetInstallationFree_0100, Function | MediumTest | Level1)
{
    bool isStageModule = true;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetInstallationFree(isStageModule);
    EXPECT_EQ(hapVerifyInfo.IsInstallationFree(), isStageModule);
}

/*
 * @tc.name: SetDependencies_0100
 * @tc.desc: SetDependencies.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetDependencies_0100, Function | MediumTest | Level1)
{
    std::list<std::string> dependencies = {"dependency1", "dependency2", "dependency3"};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDependencies(dependencies);
    EXPECT_EQ(hapVerifyInfo.GetDependencies(), dependencies);
}

/*
 * @tc.name: SetDependencyItemList_0100
 * @tc.desc: SetDependencyItemList.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetDependencyItemList_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DependencyItem dependencyItem1;
    dependencyItem1.bundleName = "bundleName1";
    dependencyItem1.moduleName = "moduleName1";
    dependencyItem1.versionCode = 1;
    OHOS::AppPackingTool::DependencyItem dependencyItem2;
    dependencyItem2.bundleName = "bundleName2";
    dependencyItem2.moduleName = "moduleName2";
    dependencyItem2.versionCode = 1;
    std::list<OHOS::AppPackingTool::DependencyItem> dependencyItemList = {dependencyItem1, dependencyItem2};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDependencyItemList(dependencyItemList);
    auto dependencyItemListOut = hapVerifyInfo.GetDependencyItemList();
    EXPECT_EQ(dependencyItemListOut.front().bundleName, dependencyItemList.front().bundleName);
    EXPECT_EQ(dependencyItemListOut.front().moduleName, dependencyItemList.front().moduleName);
    EXPECT_EQ(dependencyItemListOut.front().versionCode, dependencyItemList.front().versionCode);
    EXPECT_EQ(dependencyItemListOut.back().bundleName, dependencyItemList.back().bundleName);
    EXPECT_EQ(dependencyItemListOut.back().moduleName, dependencyItemList.back().moduleName);
    EXPECT_EQ(dependencyItemListOut.back().versionCode, dependencyItemList.back().versionCode);
}

/*
 * @tc.name: SetProfileStr_0100
 * @tc.desc: SetProfileStr.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetProfileStr_0100, Function | MediumTest | Level1)
{
    std::string profileStr = "com.example.profileStr";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetProfileStr(profileStr);
    EXPECT_EQ(hapVerifyInfo.GetProfileStr(), profileStr);
}

/*
 * @tc.name: SetResourceMap_0100
 * @tc.desc: SetResourceMap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetResourceMap_0100, Function | MediumTest | Level1)
{
    std::map<std::string, std::string> resourceMap = {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetResourceMap(resourceMap);
    EXPECT_EQ(hapVerifyInfo.GetResourceMap(), resourceMap);
}

/*
 * @tc.name: SetBundleType_0100
 * @tc.desc: SetBundleType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetBundleType_0100, Function | MediumTest | Level1)
{
    std::string bundleType = "com.example.bundleType";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetBundleType(bundleType);
    EXPECT_EQ(hapVerifyInfo.GetBundleType(), bundleType);
}

/*
 * @tc.name: SetAtomicServiceType_0100
 * @tc.desc: SetAtomicServiceType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetAtomicServiceType_0100, Function | MediumTest | Level1)
{
    std::string atomicServiceType = "com.example.atomicServiceType";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetAtomicServiceType(atomicServiceType);
    EXPECT_EQ(hapVerifyInfo.GetAtomicServiceType(), atomicServiceType);
}

/*
 * @tc.name: SetPreloadItems_0100
 * @tc.desc: SetPreloadItems.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetPreloadItems_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::PreloadItem preloadItem1;
    preloadItem1.moduleName = "com.example.moduleName1";
    OHOS::AppPackingTool::PreloadItem preloadItem2;
    preloadItem2.moduleName = "com.example.moduleName2";
    std::list<OHOS::AppPackingTool::PreloadItem> preloadItems = {preloadItem1, preloadItem2};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetPreloadItems(preloadItems);
    auto preloadItemsOut = hapVerifyInfo.GetPreloadItems();
    EXPECT_EQ(preloadItemsOut.front().moduleName, preloadItems.front().moduleName);
    EXPECT_EQ(preloadItemsOut.back().moduleName, preloadItems.back().moduleName);
}

/*
 * @tc.name: SetTargetBundleName_0100
 * @tc.desc: SetTargetBundleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetTargetBundleName_0100, Function | MediumTest | Level1)
{
    std::string targetBundleName = "com.example.targetBundleName";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetTargetBundleName(targetBundleName);
    EXPECT_EQ(hapVerifyInfo.GetTargetBundleName(), targetBundleName);
}

/*
 * @tc.name: SetTargetPriority_0100
 * @tc.desc: SetTargetPriority.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetTargetPriority_0100, Function | MediumTest | Level1)
{
    int32_t priority = 1;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetTargetPriority(priority);
    EXPECT_EQ(hapVerifyInfo.GetTargetPriority(), priority);
}

/*
 * @tc.name: SetTargetModuleName_0100
 * @tc.desc: SetTargetModuleName.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetTargetModuleName_0100, Function | MediumTest | Level1)
{
    std::string targetModuleName = "com.example.targetModuleName";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetTargetModuleName(targetModuleName);
    EXPECT_EQ(hapVerifyInfo.GetTargetModuleName(), targetModuleName);
}

/*
 * @tc.name: SetTargetModulePriority_0100
 * @tc.desc: SetTargetModulePriority.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetTargetModulePriority_0100, Function | MediumTest | Level1)
{
    int32_t priority = 1;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetTargetModulePriority(priority);
    EXPECT_EQ(hapVerifyInfo.GetTargetModulePriority(), priority);
}

/*
 * @tc.name: SetFileLength_0100
 * @tc.desc: SetFileLength.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetFileLength_0100, Function | MediumTest | Level1)
{
    int64_t fileLength = 100;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetFileLength(fileLength);
    EXPECT_EQ(hapVerifyInfo.GetFileLength(), fileLength);
}

/*
 * @tc.name: SetEntrySizeLimit_0100
 * @tc.desc: SetEntrySizeLimit.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetEntrySizeLimit_0100, Function | MediumTest | Level1)
{
    int32_t limit = 200;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetEntrySizeLimit(limit);
    EXPECT_EQ(hapVerifyInfo.GetEntrySizeLimit(), limit);
}

/*
 * @tc.name: SetNotEntrySizeLimit_0100
 * @tc.desc: SetNotEntrySizeLimit.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetNotEntrySizeLimit_0100, Function | MediumTest | Level1)
{
    int32_t notEntrySizeLimit = 100;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetNotEntrySizeLimit(notEntrySizeLimit);
    EXPECT_EQ(hapVerifyInfo.GetNotEntrySizeLimit(), notEntrySizeLimit);
}

/*
 * @tc.name: SetSumSizeLimit_0100
 * @tc.desc: SetSumSizeLimit.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetSumSizeLimit_0100, Function | MediumTest | Level1)
{
    int32_t sumSizeLimit = 300;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetSumSizeLimit(sumSizeLimit);
    EXPECT_EQ(hapVerifyInfo.GetSumSizeLimit(), sumSizeLimit);
}

/*
 * @tc.name: SetDebug_0100
 * @tc.desc: SetDebug.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetDebug_0100, Function | MediumTest | Level1)
{
    bool debug = true;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDebug(debug);
    EXPECT_EQ(hapVerifyInfo.IsDebug(), debug);
}

/*
 * @tc.name: SetCompileSdkVersion_0100
 * @tc.desc: SetCompileSdkVersion.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetCompileSdkVersion_0100, Function | MediumTest | Level1)
{
    std::string compileSdkVersion = "com.example.compileSdkVersion";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetCompileSdkVersion(compileSdkVersion);
    EXPECT_EQ(hapVerifyInfo.GetCompileSdkVersion(), compileSdkVersion);
}

/*
 * @tc.name: SetCompileSdkType_0100
 * @tc.desc: SetCompileSdkType.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetCompileSdkType_0100, Function | MediumTest | Level1)
{
    std::string compileSdkType = "com.example.compileSdkType";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetCompileSdkType(compileSdkType);
    EXPECT_EQ(hapVerifyInfo.GetCompileSdkType(), compileSdkType);
}

/*
 * @tc.name: SetProxyDataUris_0100
 * @tc.desc: SetProxyDataUris.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetProxyDataUris_0100, Function | MediumTest | Level1)
{
    std::list<std::string> proxyDataUris = {"proxyDataUri1", "proxyDataUri2", "proxyDataUri3"};
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetProxyDataUris(proxyDataUris);
    EXPECT_EQ(hapVerifyInfo.GetProxyDataUris(), proxyDataUris);
}

/*
 * @tc.name: SetContinueTypeMap_0100
 * @tc.desc: SetContinueTypeMap.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetContinueTypeMap_0100, Function | MediumTest | Level1)
{
    std::map<std::string, std::list<std::string>> continueTypeMap;
    continueTypeMap.insert({"key1", {"value1a", "value1b"}});
    continueTypeMap.insert({"key2", {"value2a", "value2b"}});
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetContinueTypeMap(continueTypeMap);
    EXPECT_EQ(hapVerifyInfo.GetContinueTypeMap(), continueTypeMap);
}

/*
 * @tc.name: SetMultiAppMode_0100
 * @tc.desc: SetMultiAppMode.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, SetMultiAppMode_0100, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::MultiAppMode multiAppMode;
    multiAppMode.multiAppModeType = "multiAppModeType";
    multiAppMode.maxCount = 3;
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetMultiAppMode(multiAppMode);
    auto multiAppModeOut = hapVerifyInfo.GetMultiAppMode();
    EXPECT_EQ(multiAppModeOut.multiAppModeType, multiAppMode.multiAppModeType);
    EXPECT_EQ(multiAppModeOut.maxCount, multiAppMode.maxCount);
}

/*
 * @tc.name: ConvertToDependency_0200
 * @tc.desc: ConvertToDependency.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapVerifyInfoTest, ConvertToDependency_0200, Function | MediumTest | Level1)
{
    OHOS::AppPackingTool::DependencyItem dependencyItem1;
    dependencyItem1.bundleName = "bundleName1";
    dependencyItem1.moduleName = "moduleName1";
    dependencyItem1.versionCode = 1;
    OHOS::AppPackingTool::DependencyItem dependencyItem2;
    dependencyItem2.bundleName = "bundleName2";
    dependencyItem2.moduleName = "moduleName2";
    dependencyItem2.versionCode = 1;
    std::list<OHOS::AppPackingTool::DependencyItem> dependencyItemList = {dependencyItem1, dependencyItem2};
    std::list<std::string> dependencies = {"dependency1", "dependency2"};
    std::string bundleName = "bundleName1";
    OHOS::AppPackingTool::HapVerifyInfo hapVerifyInfo;
    hapVerifyInfo.SetDependencies(dependencies);
    hapVerifyInfo.SetDependencyItemList(dependencyItemList);
    hapVerifyInfo.SetBundleName(bundleName);
    hapVerifyInfo.ConvertToDependency();
    EXPECT_FALSE(hapVerifyInfo.dependencies_.empty());
    EXPECT_TRUE(std::find(hapVerifyInfo.dependencies_.begin(), hapVerifyInfo.dependencies_.end(),
                          dependencyItem1.bundleName) != hapVerifyInfo.dependencies_.end());
}
}