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
#include <sstream>
#include <string>

#include "constants.h"
#define private public
#define protected public
#include "json/json_utils.h"
#include "json/pack_info.h"
#include "package_normalize.h"
#include "hsp_packager.h"
#undef private
#undef protected

using namespace OHOS::AppPackingTool;
using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace {
const std::string BUNDLE_NAME = "dd.dd.dd";
const int32_t NEW_VERSION_CODE = 10086;
const std::string NEW_BUNDLE_NAME = {"new.bundle.name"};
const std::string HSP_LIST = "/data/test/hsp_example";
const std::string OUT_PATH = "/data/test/packagenormalize_packing";
const std::string MODULE_JSON_FILE = "/module.json";
const std::string PACK_INFO_FILE = "/pack.info";
const std::string VERSION_CODE = "1000098";
const std::string HSP_OUT_PATH = "/data/test/hsp_example/library-default-unsigned.hsp";
const std::string HSP_INDEX_PATH = "/data/test/resource/packingtool/test_file/resources.index";
const std::string HSP_PACK_INFO_PATH = "/data/test/resource/packingtool/test_file/pack.info";
const std::string HSP_ETS_PATH = "/data/test/resource/packingtool/test_file/ets";
const std::string HSP_RESOURCES_PATH = "/data/test/resource/packingtool/test_file/resources";
const std::string HSP_JSON_PATH = "/data/test/resource/packingtool/test_file/module.json";
}

class PackageNormalizeTest : public testing::Test {
public:
    PackageNormalizeTest() {}
    virtual ~PackageNormalizeTest() {}

    static void SetUpTestCase();

    static void TearDownTestCase();

    void SetUp();

    void TearDown();
};

void PackageNormalizeTest::SetUpTestCase()
{
    std::string resultReceiver;
    system("mkdir /data/test/hsp_example");
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HSP_OUT_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_FORCE, "true"},
        {OHOS::AppPackingTool::Constants::PARAM_JSON_PATH, HSP_JSON_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_ETS_PATH, HSP_ETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_RESOURCES_PATH, HSP_RESOURCES_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_INDEX_PATH, HSP_INDEX_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_PACK_INFO_PATH, HSP_PACK_INFO_PATH},
    };

    OHOS::AppPackingTool::HspPackager hspPackager(parameterMap, resultReceiver);
    system("mkdir /data/test/resource/packingtool/test_file/ets");
    system("mv /data/test/resource/packingtool/test_file/pack.json "
        "/data/test/resource/packingtool/test_file/pack.info");
    EXPECT_EQ(hspPackager.InitAllowedParam(), 0);
    EXPECT_EQ(hspPackager.PreProcess(), 0);
    EXPECT_EQ(hspPackager.Process(), 0);
    EXPECT_EQ(hspPackager.PostProcess(), 0);
}

void PackageNormalizeTest::TearDownTestCase()
{
    std::string cmd = {"rm -f /data/test/packagenormalize_packing/*"};
    system(cmd.c_str());
}

void PackageNormalizeTest::SetUp() {}

void PackageNormalizeTest::TearDown() {}

/*
 * @tc.name: PackageNormalize_0100
 * @tc.desc: PackageNormalize.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PackageNormalize_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH}
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 0);
    EXPECT_EQ(packageNormalize.Process(), 0);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0100
 * @tc.desc: hsp-list is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0200
 * @tc.desc: hsp-list is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST,
            "/data/test/pnerr.hsp."},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0300
 * @tc.desc: bundle-name is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, "123456"},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0400
 * @tc.desc: version-code is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, "-12345"},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0500
 * @tc.desc: output path is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0600
 * @tc.desc: output path is invalid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, HSP_JSON_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0700
 * @tc.desc: hsp-list is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_ETS_PATH},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0800
 * @tc.desc: bundle-name is missing.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_0900
 * @tc.desc: bundle-name is too long.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_0900, Function | MediumTest | Level1)
{
    std::string bundleName("extremely-long-long-long"); //24
    bundleName.append(bundleName);                      //48
    bundleName.append(bundleName);                      //96
    bundleName.append(bundleName);                      //192

    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, bundleName},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_1000
 * @tc.desc: bundle-name format mismatch.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_1000, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, "!nv@lid.6und1e.n@me"},
        {OHOS::AppPackingTool::Constants::PARAM_VERSION_CODE, VERSION_CODE},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: PreProcess_1100
 * @tc.desc: version-code is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, PreProcess_1100, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    system("mkdir /data/test/packagenormalize_packing");
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);
    EXPECT_EQ(packageNormalize.PreProcess(), 1);
    std::string cmd = {"rm -rf "};
    cmd += OUT_PATH;
    system(cmd.c_str());
}

/*
 * @tc.name: ModifyModuleJson_1200
 * @tc.desc: ModifyModuleJson
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyModuleJson_1200, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_JSON_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string moduleJsonPath = OUT_PATH + MODULE_JSON_FILE;

    EXPECT_TRUE(packageNormalize.ModifyModuleJson(moduleJsonPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromFile(moduleJsonPath));
    Version version;
    EXPECT_TRUE(moduleJson.GetStageVersion(version));
    EXPECT_EQ(version.versionCode, NEW_VERSION_CODE);
    std::string bundleName;
    EXPECT_TRUE(moduleJson.GetBundleName(bundleName));
    EXPECT_STREQ(bundleName.c_str(), NEW_BUNDLE_NAME.c_str());

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyModuleJson_1300
 * @tc.desc: module json file is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyModuleJson_1300, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    EXPECT_FALSE(packageNormalize.ModifyModuleJson(OUT_PATH + MODULE_JSON_FILE, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyModuleJson_1400
 * @tc.desc: bundleName node is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyModuleJson_1400, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_JSON_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string moduleJsonPath = OUT_PATH + MODULE_JSON_FILE;
    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromFile(moduleJsonPath));
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    ASSERT_NE(appObj, nullptr);
    EXPECT_TRUE(appObj->Remove("bundleName"));
    EXPECT_TRUE(JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath));
 
    EXPECT_FALSE(packageNormalize.ModifyModuleJson(moduleJsonPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyModuleJson_1500
 * @tc.desc: versionCode node is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyModuleJson_1500, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_JSON_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string moduleJsonPath = OUT_PATH + MODULE_JSON_FILE;
    ModuleJson moduleJson;
    EXPECT_TRUE(moduleJson.ParseFromFile(moduleJsonPath));
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(moduleJson.GetAppObject(appObj));
    ASSERT_NE(appObj, nullptr);
    EXPECT_TRUE(appObj->Remove("versionCode"));
    EXPECT_TRUE(JsonUtils::StrToFile(moduleJson.ToString(), moduleJsonPath));

    EXPECT_FALSE(packageNormalize.ModifyModuleJson(moduleJsonPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyPackInfo_1600
 * @tc.desc: ModifyPackInfo
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyPackInfo_1600, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_PACK_INFO_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string packInfoPath = OUT_PATH + PACK_INFO_FILE;

    EXPECT_TRUE(packageNormalize.ModifyPackInfo(packInfoPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    PackInfo packInfo;
    EXPECT_TRUE(packInfo.ParseFromFile(packInfoPath));
    PackInfoVersion version;
    EXPECT_TRUE(packInfo.GetVersion(version));
    EXPECT_EQ(version.code, NEW_VERSION_CODE);
    std::string bundleName;
    EXPECT_TRUE(packInfo.GetBundleName(bundleName));
    EXPECT_STREQ(bundleName.c_str(), NEW_BUNDLE_NAME.c_str());

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyPackInfo_1700
 * @tc.desc: module json file is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyPackInfo_1700, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    EXPECT_FALSE(packageNormalize.ModifyPackInfo(OUT_PATH + PACK_INFO_FILE, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyPackInfo_1800
 * @tc.desc: bundleName node is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyPackInfo_1800, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_PACK_INFO_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string packInfoPath = OUT_PATH + PACK_INFO_FILE;
    PackInfo packInfo;
    EXPECT_TRUE(packInfo.ParseFromFile(packInfoPath));
    std::unique_ptr<PtJson> appObj;
    EXPECT_TRUE(packInfo.GetAppObject(appObj));
    ASSERT_NE(appObj, nullptr);
    EXPECT_TRUE(appObj->Remove("bundleName"));
    EXPECT_TRUE(JsonUtils::StrToFile(packInfo.ToString(), packInfoPath));

    EXPECT_FALSE(packageNormalize.ModifyPackInfo(packInfoPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}

/*
 * @tc.name: ModifyPackInfo_1900
 * @tc.desc: versionCode node is missing
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PackageNormalizeTest, ModifyPackInfo_1900, Function | MediumTest | Level1)
{
    std::string resultReceiver;
    std::map<std::string, std::string> parameterMap = {
        {OHOS::AppPackingTool::Constants::PARAM_HSP_LIST, HSP_LIST},
        {OHOS::AppPackingTool::Constants::PARAM_BUNDLE_NAME, BUNDLE_NAME},
        {OHOS::AppPackingTool::Constants::PARAM_OUT_PATH, OUT_PATH},
    };
    OHOS::AppPackingTool::PackageNormalize packageNormalize(parameterMap, resultReceiver);

    std::stringstream cmd;
    cmd << "mkdir " << OUT_PATH;
    system(cmd.str().c_str());

    cmd.str("");
    cmd << "cp " << HSP_PACK_INFO_PATH << " " << OUT_PATH;
    system(cmd.str().c_str());

    std::string packInfoPath = OUT_PATH + PACK_INFO_FILE;
    PackInfo packInfo;
    EXPECT_TRUE(packInfo.ParseFromFile(packInfoPath));
    std::unique_ptr<PtJson> versionObj;
    EXPECT_TRUE(packInfo.GetVersionObject(versionObj));
    ASSERT_NE(versionObj, nullptr);
    EXPECT_TRUE(versionObj->Remove("code"));
    EXPECT_TRUE(JsonUtils::StrToFile(packInfo.ToString(), packInfoPath));

    EXPECT_FALSE(packageNormalize.ModifyPackInfo(packInfoPath, NEW_VERSION_CODE, NEW_BUNDLE_NAME));

    cmd.str("");
    cmd << "rm -rf " << OUT_PATH;
    system(cmd.str().c_str());
}
} // namespace OHOS