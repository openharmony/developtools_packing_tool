/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "packing_tool_err_msg.h"

namespace packing_tool {
namespace error {

// Build helpers for macro-friendly error definitions
namespace {
template<typename AppendSolutions>
ErrorMsg BuildPackingToolError(const std::string& typeCode,
    const std::string& errCode,
    const std::string& desc,
    const std::string& cause,
    AppendSolutions&& appendSolutions)
{
    auto builder = ErrorMsg::getPackingToolErrBuilder();
    builder.setTypeCode(typeCode)
        .setErrCode(errCode)
        .setDescription(desc)
        .setCause(cause);
    appendSolutions(builder);
    return builder.build();
}
} // namespace

// Use helper macros to simplify definitions
#define DEFINE_ERROR(name, typeCode, errCode, desc, cause, solutions) \
    const ErrorMsg PackingToolErrMsg::name = \
        BuildPackingToolError((typeCode), (errCode), (desc), (cause), (solutions))

#define WITH_SOLUTIONS(...) \
    [](ErrorMsg::Builder& builder) { \
        __VA_ARGS__ \
    }
#define ADD_SOLUTION(sol) builder.addSolution((sol));

// ==================== Packing tool execution errors (Type: 10) ====================
DEFINE_ERROR(EXECUTE_PACKING_TOOL_FAILED, "10", "001",
    "Execute packing tool failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(COMMAND_PARSER_FAILED, "10", "002",
    "Command parser failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMMAND_VERIFY_FAILED, "10", "003",
    "Command verify failed.", "%s", WITH_SOLUTIONS());

// ==================== Compression validation errors (Type: 11) ====================
DEFINE_ERROR(HAP_MODE_ARGS_INVALID, "11", "001",
    "Parse and check args invalid in hap mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HSP_MODE_ARGS_INVALID, "11", "002",
    "Parse and check args invalid in hsp mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(APP_MODE_ARGS_INVALID, "11", "003",
    "Parse and check args invalid in app mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MULTI_APP_MODE_ARGS_INVALID, "11", "004",
    "Parse and check args invalid in multiApp mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(RES_MODE_ARGS_INVALID, "11", "005",
    "Parse and check args invalid in res mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HQF_MODE_ARGS_INVALID, "11", "006",
    "Parse and check args invalid in hqf mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(APPQF_MODE_ARGS_INVALID, "11", "007",
    "Parse and check args invalid in appqf mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PACKAGE_NORMALIZE_MODE_ARGS_INVALID, "11", "008",
    "Parse and check args invalid in packageNormalize mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(VERSION_NORMALIZE_MODE_ARGS_INVALID, "11", "009",
    "Parse and check args invalid in versionNormalize mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HAR_MODE_ARGS_INVALID, "11", "010",
    "Parse and check args invalid in har mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMMAND_MODE_INVALID, "11", "011",
    "Command verify mode is invalid.", "Input mode is invalid.", WITH_SOLUTIONS());

DEFINE_ERROR(BUNDLE_TYPE_SHARED_INVALID, "11", "012",
    "Check bundleType is shared failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(BUNDLE_TYPE_APPSERVICE_INVALID, "11", "013",
    "Check the bundleType is appService failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HSP_HAS_ABILITIES_FAILED, "11", "014",
    "Check hsp has abilities failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HSP_HAS_EXTENSION_ABILITIES_FAILED, "11", "015",
    "Check hsp has extension abilities failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HAS_HOME_ABILITY_INVALID, "11", "016",
    "Check hsp has entry abilitiy failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HAS_HOME_EXTENSION_ABILITY_INVALID, "11", "017",
    "Check hsp has entry extensionAbility failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(OUT_PATH_INVALID, "11", "018",
    "Invalid output path.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HAP_ADDITION_MODE_ARGS_INVALID, "11", "019",
    "Parse and check args invalid in hapAddition mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HANDLE_HAP_HSP_INPUT_FAILED, "11", "020",
    "Handle hap and hsp input failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(GENERAL_NORMALIZE_MODE_ARGS_INVALID, "11", "021",
    "Parse and check args invalid in generalNormalize mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_ATOMIC_SERVICE_SIZE_LIMIT_FAILED, "11", "022",
    "Parse atomicService size limit failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

// ==================== Compression processing errors (Type: 12) ====================
DEFINE_ERROR(COMPRESS_PROCESS_FAILED, "12", "001",
    "Execute compress process failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(COMPRESS_HAP_FAILED, "12", "002",
    "Compress Stage hap failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(CHECK_STAGE_HAP_FAILED, "12", "003",
    "Verify Stage hap info failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(CHECK_AS_TSAN_ENABLED, "12", "004",
    "Check asanEnabled failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please ensure the parameters are correctly set. Review 'app.json' and adjust "
            "the configuration as needed.")
    ));

DEFINE_ERROR(CHECK_HWASAN_ENABLED_INVALID, "12", "005",
    "Check hwasanEnabled failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please ensure the parameters are correctly set. Review 'app.json' and adjust "
            "the configuration as needed.")
    ));

DEFINE_ERROR(CHECK_ATOMIC_SERVICE_FAILED, "12", "006",
    "Check atomicService failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(CHECK_CONTINUE_BUNDLENAME_INVALID, "12", "007",
    "Check continueBundleName invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Ensure continueBundleName not inclue self.")
    ));

DEFINE_ERROR(CHECK_OVERLAY_CFG_FAILED, "12", "008",
    "Check whether is an overlay hsp failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check 'app.json5' and 'module.json5' file according to the error.")
        ADD_SOLUTION("Check targetBundleName, targetModuleName, and targetPriority in the related module.")
        ADD_SOLUTION("Note: Modules configured with the targetModuleName is overlay feature module.")
        ADD_SOLUTION("Note: Modules configured with the targetBundleName is overlay feature application.")
    ));

DEFINE_ERROR(COMPRESS_PROCESS_EXCEPTION, "12", "009",
    "Process compress exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(HAS_GENERATE_BUILD_HASH, "12", "010",
    "Verify has generate build hash failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(SET_GENERATE_BUILD_HASH, "12", "011",
    "Set generate build hash failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_UBASAN_ENABLED_INVALID, "12", "012",
    "Check ubsanEnabled failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please ensure the parameters are correctly set. Review 'app.json' and adjust "
            "the configuration as needed.")
    ));

DEFINE_ERROR(READ_STAGE_HAP_VERIFY_INFO_FAILED, "12", "013",
    "Read Stage hap verify info exist exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the exception message for more details and modify accordingly.")
    ));

DEFINE_ERROR(COMPRESS_PARALLEL_EXCEPTION, "12", "014",
    "Parallel compress exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message and modify the operation.")
    ));

DEFINE_ERROR(COMPRESS_APP_FAILED, "12", "015",
    "Compress app file failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(INVALID_HAP_FILE, "12", "016",
    "Check input hap or hsp file is invalid.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_SHARED_APP_INVALID, "12", "017",
    "Check shared App invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_BUNDLETYPE_INVALID, "12", "018",
    "Check bundleType is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID, "12", "019",
    "Check app atomicservice compress size failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(READ_FA_HAP_VERIFY_INFO_FAILED, "12", "020",
    "Read FA hap verify info exist exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(COMPRESS_APP_IO_EXCEPTION, "12", "021",
    "IO exception when compress app.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message and modify the operation.")
    ));

DEFINE_ERROR(COMPRESS_HSP_FAILED, "12", "022",
    "Compress Hsp failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(JSON_SPECIAL_PROCESS_FAILED, "12", "023",
    "Json special process exist exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the exception message and modify.")
    ));

DEFINE_ERROR(CHECK_ATOMIC_SERVICE_SIZE_FAILED, "12", "024",
    "Check atomicService size failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related size check error message and reduce related module size.")
    ));

DEFINE_ERROR(COMPRESS_APP_MODE_FORMULTI_PROJECT_FAILED, "12", "025",
    "Compress in app mode for multi project failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(HAP_ADDITION_FAILED, "12", "026",
    "Hap addition failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMPRESS_HAP_ADDITION_FAILED, "12", "027",
    "Compress hap addition failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(DISPOSE_APP_FAILED, "12", "028",
    "Dispose app failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(NO_PACK_INFO, "12", "029",
    "pack.info does not find in hap or app.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMPRESS_PACK_RES_MODE_FAILED, "12", "030",
    "Compress in res mode failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(INVALID_THIRD_LEVEL_DIRECTORY_NAME, "12", "031",
    "Invalid third level directory name.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check it with reference to this example: \n"
            "zh_Hani_CN-vertical-car-mdpi-dark or zh_Hani_CN-vertical-car-mdpi.")
    ));

DEFINE_ERROR(IS_PICTURING_FAILED, "12", "032",
    "Check whether picturingName meets specifications failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(GET_FILE_LIST_FAILED, "12", "033",
    "Get file list failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(GET_MODULE_NAME_FROM_STRING_FAILED, "12", "034",
    "Get module name from line string failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMPRESS_APPQF_FAILED, "12", "035",
    "Compress in appqf mode failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_HQF_INVALID, "12", "036",
    "Input hqf is invalid.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_APP_FIELDS_FAILED, "12", "037",
    "Check hqf has same app fields failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_MODULE_INVALID, "12", "038",
    "Check hqf module is invalid.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(VERSION_NORMALIZE_FAILED, "12", "039",
    "Version normalize failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(VERIFY_MODULE_VERSION_FAILED, "12", "040",
    "Verify module version failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(SHA256_CALCULATION_FAILED, "12", "041",
    "SHA-256 hash calculation failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COMPRESS_FILE_DUPLICATE, "12", "042",
    "Select haps find duplicated hap.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COPY_FILE_FAILED, "12", "043",
    "Copy file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MAKE_DIR_FAILED, "12", "044",
    "Make dir failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(COPY_FILE_SAFELY_FAILED, "12", "045",
    "Copy file safely failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_MODIFY_CONFIG_JSON_FAILED, "12", "046",
    "Parse and modify config.json failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_AND_MODIFY_MODULEJSON_FAILED, "12", "047",
    "Parse and modify module.json failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(WRITE_JSON_FILE_EXPECTION, "12", "048",
    "Write json file exception.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_PACK_INFO_JSON_FAILED, "12", "049",
    "Parse pack.info json file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(IS_PLUGIN_HOST_FAILED, "12", "050",
    "Check is plugin host failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(IS_PERMISSION_SUPPORT_PLUGIN_FAILED, "12", "051",
    "Check is permission support plugin failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_PKG_CONTEXT_FAILED, "12", "052",
    "Check pkg context failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_APP_PLUGIN_FAILED, "12", "053",
    "Check app plugin failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_DEDUPLICATE_HAR_FAILED, "12", "054",
    "Check deduplicateHar field failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_KERNEL_PERMISSION_COMPRESSION_FAILED, "12", "055",
    "Check kernel permission compression failed.", "%s", WITH_SOLUTIONS());

// ==================== Module JSON check errors (Type: 13) ====================
DEFINE_ERROR(PARSE_JSON_OBJECT_EXCEPTION, "13", "001",
    "Parse json object exception.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_JSON_FAILED, "13", "002",
    "Parse json profile failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_MODULE_ATOMIC_SERVICE_FAILED, "13", "003",
    "Check module atomicService invalid.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_STAGE_JSON_FAILED, "13", "004",
    "Failed to parse module.json for the Stage module.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_STAGE_BUNDLE_TYPE_FAILED, "13", "005",
    "Failed to parse module.json and bundleType.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_LEASTONE_ABILITY, "13", "006",
    "Check entry module at least one ability failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Ensure the entry type module contains at least one ability.")
    ));

DEFINE_ERROR(CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED, "13", "007",
    "Check module atomicService and installationFree invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(PARSE_BUNDLE_NAME_FAILED, "13", "008",
    "Get the bundleName from json file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_PROXY_DATA_URI_FAILED, "13", "009",
    "Failed to parse module.json and proxyData object.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_BUNDLETYPE_CONSISTENCY_FAILED, "13", "010",
    "Failed to check the consistency of bundleType.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Make sure the bundleType is consistency for different modules.")
    ));

DEFINE_ERROR(PARSE_PATCH_MODULE_NAME_FAILED, "13", "011",
    "Failed to parse patch module name from patch.json.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_FA_HAP_VERIFY_INFO_FAILED, "13", "012",
    "Failed to parse FA hap verify info from config.json.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_FA_JSON_FAILED, "13", "013",
    "Failed to parse config.json for the FA module.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MERGE_TWO_PACKINFO_FAILED, "13", "014",
    "Merge two pack.info into one pack.info file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(VERIFY_PACKINFO_FAILED, "13", "015",
    "Verify pack.info file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(VERIFY_APP_PACKINFO_FAILED, "13", "016",
    "Verify app in pack.info failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(BUNDLE_TYPE_PACKINFO_INVALID, "13", "017",
    "Verify bundleType in pack.info file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_PACKINFO_FORMS_NAME_FAILED, "13", "018",
    "Parse forms name in pack.info file failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MERGE_PACKINFO_OBJ_FAILED, "13", "019",
    "Failed to merge pack.info objects.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MERGE_PACKINFO_BY_PACKAGE_PAIR_FAILED, "13", "020",
    "Failed to merge two pack.info files into one pack.info file by packagePair.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(MERGE_PACKINFO_OBJ_BY_PACKAGE_PAIR_FAILED, "13", "021",
    "Failed to merge two pack.info json objects into one pack.info json object by packagePair.",
    "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PARSE_PACKINFO_FORMS_FAILED, "13", "022",
    "Failed to parse pack.info forms.", "%s", WITH_SOLUTIONS());

// ==================== File operation errors (Type: 14) ====================
DEFINE_ERROR(FILE_NOT_FOUND, "14", "001",
    "File avaiable not found exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related FileNotFoundException message.")
        ADD_SOLUTION("If the file is in use, close any applications or processes that might be using it.")
    ));

DEFINE_ERROR(CLOSE_ZIP_OUTPUT_STREAM_EXCEPTION, "14", "002",
    "Close zip output stream exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message and modify the operation.")
    ));

DEFINE_ERROR(CLOSE_STREAM_EXCEPTION, "14", "003",
    "IO exception when closing stream.", "",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message for more details and modify accordingly.")
    ));

DEFINE_ERROR(GET_FILE_CONTENT_FAILED, "14", "004",
    "Get file content failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message for more details and modify accordingly.")
    ));

DEFINE_ERROR(FILE_NOT_EXIST, "14", "005",
    "Parse file not exist.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(GET_FILE_SIZE_FAILED, "14", "006",
    "Get file size failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(FILE_IO_EXCEPTION, "14", "007",
    "File IO exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("1. Ensure the file path is correct and the file exists.")
        ADD_SOLUTION("2. Verify you have the necessary permissions to access the file.")
        ADD_SOLUTION("3. Check for possible disk or file system errors.")
        ADD_SOLUTION("4. Review the related IOException message for further insights.")
    ));

DEFINE_ERROR(COMPRESS_FILE_EXCEPTION, "14", "008",
    "Compress file exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the related exception message and modify.")
    ));

DEFINE_ERROR(FILE_DELETE_FAILED, "14", "009",
    "File delete failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
        ADD_SOLUTION("Verify you have the necessary permissions to delete the file.")
    ));

// ==================== IO exceptions (Type: 15) ====================
DEFINE_ERROR(IO_EXCEPTION, "15", "001",
    "IO exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("1. Ensure the file path is correct and the file exists.")
        ADD_SOLUTION("2. Verify you have the necessary permissions to access the file.")
        ADD_SOLUTION("3. Check for possible disk or file system errors.")
        ADD_SOLUTION("4. Review the related error message for further insights.")
    ));

DEFINE_ERROR(NULL_POINTER_EXCEPTION, "15", "002",
    "Null pointer exception.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please review the related exception message for further insights.")
    ));

// ==================== HAP validation errors (Type: 16) ====================
DEFINE_ERROR(CHECK_APP_FIELDS_INVALID, "16", "001",
    "App fields is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(APP_FIELDS_DIFFERENT_ERROR, "16", "002",
    "Some app variable is different.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_POLICY_DISJOINT_ERROR, "16", "003",
    "Check two distroFilter policy disjoint invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_MODULE_NAME_INVALID, "16", "004",
    "Check module name is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_PACKAGE_NAME_INVALID, "16", "005",
    "Check packageName invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_HAP_INVALID, "16", "006",
    "Check hap info is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(CHECK_ENTRY_INVALID, "16", "007",
    "Check entry module is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_DEPENDENCY_INVALID, "16", "008",
    "Check dependency is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(DEPENDENCY_LIST_INVALID, "16", "009",
    "Check dependency list is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_ATOMICSERVICE_INVALID, "16", "010",
    "Check atomicservice is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check the first error message for more details and modify accordingly.")
    ));

DEFINE_ERROR(ATOMICSERVICE_PRELOADS_INVALID, "16", "011",
    "Atomicservice preloads is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check related preload message and modify preloads settings.")
    ));

DEFINE_ERROR(TARGET_MODULE_NAME_NOT_EXIST, "16", "012",
    "Target moduleName is not exist.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Make sure pack with vaild and existing target module.")
        ADD_SOLUTION("Create missing modules or check target moduleName whether valid.")
    ));

DEFINE_ERROR(COMPILE_SDK_TYPE_DIFFERENT, "16", "013",
    "CompileSdkType is different.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(PROXY_DATA_URI_NOT_UNIQUE, "16", "014",
    "The values of uri in proxyData of module.json are not unique.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CONTINUE_TYPE_INVALID, "16", "015",
    "Check continueType is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("%s")
    ));

DEFINE_ERROR(CHECK_FILE_SIZE_INVALID, "16", "016",
    "Check file size failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check and reduced related module size.")
    ));

DEFINE_ERROR(CHECK_HAP_VERIFY_INFO_LIST_EMPTY, "16", "017",
    "The provided list of HapVerifyInfos is empty.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_ATOMIC_SERVICE_MODULE_SIZE, "16", "018",
    "Check the atomicService module size failed.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Please check and reduced related module size.")
    ));

DEFINE_ERROR(CHECK_FEATURE_DISTRO_FILTER_INVALID, "16", "019",
    "Check the entry module distributionFilter is invalid.", "%s",
    WITH_SOLUTIONS(
        ADD_SOLUTION("Ensure the Entry type module distributionFilter file policy settings is 'exclude' or 'include'.")
    ));

// ==================== Package normalization errors (Type: 17) ====================
DEFINE_ERROR(PACKAGE_NORMALIZE_FAILED, "17", "001",
    "Normalize HSP bundleName and versionCode failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(UPDATE_MODULE_JSON_FAILED, "17", "002",
    "Update module.json failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(UPDATE_PACKINFO_FAILED, "17", "003",
    "Update pack.info failed.", "%s", WITH_SOLUTIONS());

// ==================== Packaging tool errors (Type: 18) ====================
DEFINE_ERROR(FAST_APP_MODE_ARGS_INVALID, "18", "001",
    "Parse and check args invalid in fast app mode.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(CHECK_BUNDLE_TYPE_CONSISTENCY, "18", "002",
    "Check bundleType consistency failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PACK_INFO_INVALID, "18", "003",
    "Pack.info is invalid.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(ADD_ARCHIVE_ENTRY_FAILED, "18", "004",
    "Add archive entry failed.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(PACK_MULTI_THREAD_EXCEPTION, "18", "005",
    "Packing with multiple threads exception.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(REPACK_HSP_EXCEPTION, "18", "006",
    "Repack hsp exception.", "%s", WITH_SOLUTIONS());

// ==================== Incremental packaging errors (Type: 19) ====================
DEFINE_ERROR(INCREMENTAL_PACK_HAP_EXCEPTION, "19", "001",
    "Incremental pack hap exception.", "%s", WITH_SOLUTIONS());

DEFINE_ERROR(INCREMENTAL_PACK_HSP_EXCEPTION, "19", "002",
    "Incremental pack hsp exception.", "%s", WITH_SOLUTIONS());

// ==================== SO file scanning errors (Type: 20) ====================
DEFINE_ERROR(SCAN_SO_FILES_EXCEPTION, "20", "001",
    "Scan so files exception.", "%s", WITH_SOLUTIONS());

#undef DEFINE_ERROR
#undef ADD_SOLUTION

} // namespace error
} // namespace packing_tool
