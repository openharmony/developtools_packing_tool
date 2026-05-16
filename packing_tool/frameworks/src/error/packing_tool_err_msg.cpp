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
#include <initializer_list>

namespace packing_tool {
namespace error {

// Build helpers for error message construction
namespace {
ErrorMsg BuildPackingToolError(const char* typeCode,
    const char* errCode,
    const char* desc,
    const char* cause,
    std::initializer_list<const char*> solutions = {})
{
    auto builder = ErrorMsg::getPackingToolErrBuilder();
    builder.setTypeCode(typeCode)
        .setErrCode(errCode)
        .setDescription(desc)
        .setCause(cause);
    for (const auto* solution : solutions) {
        builder.addSolution(solution);
    }
    return builder.build();
}
} // namespace

// ==================== Packing tool execution errors (Type: 10) ====================
const ErrorMsg PackingToolErrMsg::EXECUTE_PACKING_TOOL_FAILED =
    BuildPackingToolError("10", "001",
        "Execute packing tool failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::COMMAND_PARSER_FAILED =
    BuildPackingToolError("10", "002",
        "Command parser failed.", "%s");

const ErrorMsg PackingToolErrMsg::COMMAND_VERIFY_FAILED =
    BuildPackingToolError("10", "003",
        "Command verify failed.", "%s");

// ==================== Compression validation errors (Type: 11) ====================
const ErrorMsg PackingToolErrMsg::HAP_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "001",
        "Parse and check args invalid in hap mode.", "%s");

const ErrorMsg PackingToolErrMsg::HSP_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "002",
        "Parse and check args invalid in hsp mode.", "%s");

const ErrorMsg PackingToolErrMsg::APP_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "003",
        "Parse and check args invalid in app mode.", "%s");

const ErrorMsg PackingToolErrMsg::MULTI_APP_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "004",
        "Parse and check args invalid in multiApp mode.", "%s");

const ErrorMsg PackingToolErrMsg::RES_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "005",
        "Parse and check args invalid in res mode.", "%s");

const ErrorMsg PackingToolErrMsg::HQF_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "006",
        "Parse and check args invalid in hqf mode.", "%s");

const ErrorMsg PackingToolErrMsg::APPQF_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "007",
        "Parse and check args invalid in appqf mode.", "%s");

const ErrorMsg PackingToolErrMsg::PACKAGE_NORMALIZE_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "008",
        "Parse and check args invalid in packageNormalize mode.", "%s");

const ErrorMsg PackingToolErrMsg::VERSION_NORMALIZE_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "009",
        "Parse and check args invalid in versionNormalize mode.", "%s");

const ErrorMsg PackingToolErrMsg::HAR_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "010",
        "Parse and check args invalid in har mode.", "%s");

const ErrorMsg PackingToolErrMsg::COMMAND_MODE_INVALID =
    BuildPackingToolError("11", "011",
        "Command verify mode is invalid.", "Input mode is invalid.");

const ErrorMsg PackingToolErrMsg::BUNDLE_TYPE_SHARED_INVALID =
    BuildPackingToolError("11", "012",
        "Check bundleType is shared failed.", "%s");

const ErrorMsg PackingToolErrMsg::BUNDLE_TYPE_APPSERVICE_INVALID =
    BuildPackingToolError("11", "013",
        "Check the bundleType is appService failed.", "%s");

const ErrorMsg PackingToolErrMsg::HSP_HAS_ABILITIES_FAILED =
    BuildPackingToolError("11", "014",
        "Check hsp has abilities failed.", "%s");

const ErrorMsg PackingToolErrMsg::HSP_HAS_EXTENSION_ABILITIES_FAILED =
    BuildPackingToolError("11", "015",
        "Check hsp has extension abilities failed.", "%s");

const ErrorMsg PackingToolErrMsg::HAS_HOME_ABILITY_INVALID =
    BuildPackingToolError("11", "016",
        "Check hsp has entry abilitiy failed.", "%s");

const ErrorMsg PackingToolErrMsg::HAS_HOME_EXTENSION_ABILITY_INVALID =
    BuildPackingToolError("11", "017",
        "Check hsp has entry extensionAbility failed.", "%s");

const ErrorMsg PackingToolErrMsg::OUT_PATH_INVALID =
    BuildPackingToolError("11", "018",
        "Invalid output path.", "%s");

const ErrorMsg PackingToolErrMsg::HAP_ADDITION_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "019",
        "Parse and check args invalid in hapAddition mode.", "%s");

const ErrorMsg PackingToolErrMsg::HANDLE_HAP_HSP_INPUT_FAILED =
    BuildPackingToolError("11", "020",
        "Handle hap and hsp input failed.", "%s");

const ErrorMsg PackingToolErrMsg::GENERAL_NORMALIZE_MODE_ARGS_INVALID =
    BuildPackingToolError("11", "021",
        "Parse and check args invalid in generalNormalize mode.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_ATOMIC_SERVICE_SIZE_LIMIT_FAILED =
    BuildPackingToolError("11", "022",
        "Parse atomicService size limit failed.", "%s",
        {
            "%s",
        });

// ==================== Compression processing errors (Type: 12) ====================
const ErrorMsg PackingToolErrMsg::COMPRESS_PROCESS_FAILED =
    BuildPackingToolError("12", "001",
        "Execute compress process failed.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_HAP_FAILED =
    BuildPackingToolError("12", "002",
        "Compress Stage hap failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_STAGE_HAP_FAILED =
    BuildPackingToolError("12", "003",
        "Verify Stage hap info failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_AS_TSAN_ENABLED =
    BuildPackingToolError("12", "004",
        "Check asanEnabled failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_HWASAN_ENABLED_INVALID =
    BuildPackingToolError("12", "005",
        "Check hwasanEnabled failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_ATOMIC_SERVICE_FAILED =
    BuildPackingToolError("12", "006",
        "Check atomicService failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_CONTINUE_BUNDLENAME_INVALID =
    BuildPackingToolError("12", "007",
        "Check continueBundleName invalid.", "%s",
        {
            "Ensure continueBundleName not inclue self.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_OVERLAY_CFG_FAILED =
    BuildPackingToolError("12", "008",
        "Check whether is an overlay hsp failed.", "%s",
        {
            "Please check 'app.json5' and 'module.json5' file according to the error.",
            "Check targetBundleName, targetModuleName, and targetPriority in the related module.",
            "Note: Modules configured with the targetModuleName is overlay feature module.",
            "Note: Modules configured with the targetBundleName is overlay feature application.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_PROCESS_EXCEPTION =
    BuildPackingToolError("12", "009",
        "Process compress exception.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::HAS_GENERATE_BUILD_HASH =
    BuildPackingToolError("12", "010",
        "Verify has generate build hash failed.", "%s");

const ErrorMsg PackingToolErrMsg::SET_GENERATE_BUILD_HASH =
    BuildPackingToolError("12", "011",
        "Set generate build hash failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_UBASAN_ENABLED_INVALID =
    BuildPackingToolError("12", "012",
        "Check ubsanEnabled failed.", "%s");

const ErrorMsg PackingToolErrMsg::READ_STAGE_HAP_VERIFY_INFO_FAILED =
    BuildPackingToolError("12", "013",
        "Read Stage hap verify info exist exception.", "%s",
        {
            "Please check the exception message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_PARALLEL_EXCEPTION =
    BuildPackingToolError("12", "014",
        "Parallel compress exception.", "%s",
        {
            "Please check the related exception message and modify the operation.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_APP_FAILED =
    BuildPackingToolError("12", "015",
        "Compress app file failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::INVALID_HAP_FILE =
    BuildPackingToolError("12", "016",
        "Check input hap or hsp file is invalid.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_SHARED_APP_INVALID =
    BuildPackingToolError("12", "017",
        "Check shared App invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_BUNDLETYPE_INVALID =
    BuildPackingToolError("12", "018",
        "Check bundleType is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID =
    BuildPackingToolError("12", "019",
        "Check app atomicservice compress size failed.", "%s");

const ErrorMsg PackingToolErrMsg::READ_FA_HAP_VERIFY_INFO_FAILED =
    BuildPackingToolError("12", "020",
        "Read FA hap verify info exist exception.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_APP_IO_EXCEPTION =
    BuildPackingToolError("12", "021",
        "IO exception when compress app.", "%s",
        {
            "Please check the related exception message and modify the operation.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_HSP_FAILED =
    BuildPackingToolError("12", "022",
        "Compress Hsp failed.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::JSON_SPECIAL_PROCESS_FAILED =
    BuildPackingToolError("12", "023",
        "Json special process exist exception.", "%s",
        {
            "Please check the exception message and modify.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_ATOMIC_SERVICE_SIZE_FAILED =
    BuildPackingToolError("12", "024",
        "Check atomicService size failed.", "%s",
        {
            "Please check the related size check error message and reduce related module size.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_APP_MODE_FORMULTI_PROJECT_FAILED =
    BuildPackingToolError("12", "025",
        "Compress in app mode for multi project failed.", "%s");

const ErrorMsg PackingToolErrMsg::HAP_ADDITION_FAILED =
    BuildPackingToolError("12", "026",
        "Hap addition failed.", "%s");

const ErrorMsg PackingToolErrMsg::COMPRESS_HAP_ADDITION_FAILED =
    BuildPackingToolError("12", "027",
        "Compress hap addition failed.", "%s");

const ErrorMsg PackingToolErrMsg::DISPOSE_APP_FAILED =
    BuildPackingToolError("12", "028",
        "Dispose app failed.", "%s");

const ErrorMsg PackingToolErrMsg::NO_PACK_INFO =
    BuildPackingToolError("12", "029",
        "pack.info does not find in hap or app.", "%s");

const ErrorMsg PackingToolErrMsg::COMPRESS_PACK_RES_MODE_FAILED =
    BuildPackingToolError("12", "030",
        "Compress in res mode failed.", "%s");

const ErrorMsg PackingToolErrMsg::INVALID_THIRD_LEVEL_DIRECTORY_NAME =
    BuildPackingToolError("12", "031",
        "Invalid third level directory name.", "%s");

const ErrorMsg PackingToolErrMsg::IS_PICTURING_FAILED =
    BuildPackingToolError("12", "032",
        "Check whether picturingName meets specifications failed.", "%s");

const ErrorMsg PackingToolErrMsg::GET_FILE_LIST_FAILED =
    BuildPackingToolError("12", "033",
        "Get file list failed.", "%s");

const ErrorMsg PackingToolErrMsg::GET_MODULE_NAME_FROM_STRING_FAILED =
    BuildPackingToolError("12", "034",
        "Get module name from line string failed.", "%s");

const ErrorMsg PackingToolErrMsg::COMPRESS_APPQF_FAILED =
    BuildPackingToolError("12", "035",
        "Compress in appqf mode failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_HQF_INVALID =
    BuildPackingToolError("12", "036",
        "Input hqf is invalid.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_APP_FIELDS_FAILED =
    BuildPackingToolError("12", "037",
        "Check hqf has same app fields failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_MODULE_INVALID =
    BuildPackingToolError("12", "038",
        "Check hqf module is invalid.", "%s");

const ErrorMsg PackingToolErrMsg::VERSION_NORMALIZE_FAILED =
    BuildPackingToolError("12", "039",
        "Version normalize failed.", "%s");

const ErrorMsg PackingToolErrMsg::VERIFY_MODULE_VERSION_FAILED =
    BuildPackingToolError("12", "040",
        "Verify module version failed.", "%s");

const ErrorMsg PackingToolErrMsg::SHA256_CALCULATION_FAILED =
    BuildPackingToolError("12", "041",
        "SHA-256 hash calculation failed.", "%s");

const ErrorMsg PackingToolErrMsg::COMPRESS_FILE_DUPLICATE =
    BuildPackingToolError("12", "042",
        "Select haps find duplicated hap.", "%s");

const ErrorMsg PackingToolErrMsg::COPY_FILE_FAILED =
    BuildPackingToolError("12", "043",
        "Copy file failed.", "%s");

const ErrorMsg PackingToolErrMsg::MAKE_DIR_FAILED =
    BuildPackingToolError("12", "044",
        "Make dir failed.", "%s");

const ErrorMsg PackingToolErrMsg::COPY_FILE_SAFELY_FAILED =
    BuildPackingToolError("12", "045",
        "Copy file safely failed.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_MODIFY_CONFIG_JSON_FAILED =
    BuildPackingToolError("12", "046",
        "Parse and modify config.json failed.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_AND_MODIFY_MODULEJSON_FAILED =
    BuildPackingToolError("12", "047",
        "Parse and modify module.json failed.", "%s");

const ErrorMsg PackingToolErrMsg::WRITE_JSON_FILE_EXPECTION =
    BuildPackingToolError("12", "048",
        "Write json file exception.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_PACK_INFO_JSON_FAILED =
    BuildPackingToolError("12", "049",
        "Parse pack.info json file failed.", "%s");

const ErrorMsg PackingToolErrMsg::IS_PLUGIN_HOST_FAILED =
    BuildPackingToolError("12", "050",
        "Check is plugin host failed.", "%s");

const ErrorMsg PackingToolErrMsg::IS_PERMISSION_SUPPORT_PLUGIN_FAILED =
    BuildPackingToolError("12", "051",
        "Check is permission support plugin failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_PKG_CONTEXT_FAILED =
    BuildPackingToolError("12", "052",
        "Check pkg context failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_APP_PLUGIN_FAILED =
    BuildPackingToolError("12", "053",
        "Check app plugin failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_DEDUPLICATE_HAR_FAILED =
    BuildPackingToolError("12", "054",
        "Check deduplicateHar field failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_KERNEL_PERMISSION_COMPRESSION_FAILED =
    BuildPackingToolError("12", "055",
        "Check kernel permission compression failed.", "%s");

// ==================== Module JSON check errors (Type: 13) ====================
const ErrorMsg PackingToolErrMsg::PARSE_JSON_OBJECT_EXCEPTION =
    BuildPackingToolError("13", "001",
        "Parse json object exception.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_JSON_FAILED =
    BuildPackingToolError("13", "002",
        "Parse json profile failed.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_MODULE_ATOMIC_SERVICE_FAILED =
    BuildPackingToolError("13", "003",
        "Check module atomicService invalid.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_STAGE_JSON_FAILED =
    BuildPackingToolError("13", "004",
        "Failed to parse module.json for the Stage module.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_STAGE_BUNDLE_TYPE_FAILED =
    BuildPackingToolError("13", "005",
        "Failed to parse module.json and bundleType.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_LEASTONE_ABILITY =
    BuildPackingToolError("13", "006",
        "Check entry module at least one ability failed.", "%s",
        {
            "Ensure the entry type module contains at least one ability.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED =
    BuildPackingToolError("13", "007",
        "Check module atomicService and installationFree invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::PARSE_BUNDLE_NAME_FAILED =
    BuildPackingToolError("13", "008",
        "Get the bundleName from json file failed.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_PROXY_DATA_URI_FAILED =
    BuildPackingToolError("13", "009",
        "Failed to parse module.json and proxyData object.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_BUNDLETYPE_CONSISTENCY_FAILED =
    BuildPackingToolError("13", "010",
        "Failed to check the consistency of bundleType.", "%s",
        {
            "Make sure the bundleType is consistency for different modules.",
        });

const ErrorMsg PackingToolErrMsg::PARSE_PATCH_MODULE_NAME_FAILED =
    BuildPackingToolError("13", "011",
        "Failed to parse patch module name from patch.json.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_FA_HAP_VERIFY_INFO_FAILED =
    BuildPackingToolError("13", "012",
        "Failed to parse FA hap verify info from config.json.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_FA_JSON_FAILED =
    BuildPackingToolError("13", "013",
        "Failed to parse config.json for the FA module.", "%s");

const ErrorMsg PackingToolErrMsg::MERGE_TWO_PACKINFO_FAILED =
    BuildPackingToolError("13", "014",
        "Merge two pack.info into one pack.info file failed.", "%s");

const ErrorMsg PackingToolErrMsg::VERIFY_PACKINFO_FAILED =
    BuildPackingToolError("13", "015",
        "Verify pack.info file failed.", "%s");

const ErrorMsg PackingToolErrMsg::VERIFY_APP_PACKINFO_FAILED =
    BuildPackingToolError("13", "016",
        "Verify app in pack.info failed.", "%s");

const ErrorMsg PackingToolErrMsg::BUNDLE_TYPE_PACKINFO_INVALID =
    BuildPackingToolError("13", "017",
        "Verify bundleType in pack.info file failed.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_PACKINFO_FORMS_NAME_FAILED =
    BuildPackingToolError("13", "018",
        "Parse forms name in pack.info file failed.", "%s");

const ErrorMsg PackingToolErrMsg::MERGE_PACKINFO_OBJ_FAILED =
    BuildPackingToolError("13", "019",
        "Failed to merge pack.info objects.", "%s");

const ErrorMsg PackingToolErrMsg::MERGE_PACKINFO_BY_PACKAGE_PAIR_FAILED =
    BuildPackingToolError("13", "020",
        "Failed to merge two pack.info files into one pack.info file by packagePair.", "%s");

const ErrorMsg PackingToolErrMsg::MERGE_PACKINFO_OBJ_BY_PACKAGE_PAIR_FAILED =
    BuildPackingToolError("13", "021",
        "Failed to merge two pack.info json objects into one pack.info json object by packagePair.", "%s");

const ErrorMsg PackingToolErrMsg::PARSE_PACKINFO_FORMS_FAILED =
    BuildPackingToolError("13", "022",
        "Failed to parse pack.info forms.", "%s");

// ==================== File operation errors (Type: 14) ====================
const ErrorMsg PackingToolErrMsg::FILE_NOT_FOUND =
    BuildPackingToolError("14", "001",
        "File avaiable not found exception.", "%s",
        {
            "Please check the related FileNotFoundException message.",
            "If the file is in use, close any applications or processes that might be using it.",
        });

const ErrorMsg PackingToolErrMsg::CLOSE_ZIP_OUTPUT_STREAM_EXCEPTION =
    BuildPackingToolError("14", "002",
        "Close zip output stream exception.", "%s",
        {
            "Please check the related exception message and modify the operation.",
        });

const ErrorMsg PackingToolErrMsg::CLOSE_STREAM_EXCEPTION =
    BuildPackingToolError("14", "003",
        "IO exception when closing stream.", "",
        {
            "Please check the related exception message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::GET_FILE_CONTENT_FAILED =
    BuildPackingToolError("14", "004",
        "Get file content failed.", "%s",
        {
            "Please check the related exception message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::FILE_NOT_EXIST =
    BuildPackingToolError("14", "005",
        "Parse file not exist.", "%s");

const ErrorMsg PackingToolErrMsg::GET_FILE_SIZE_FAILED =
    BuildPackingToolError("14", "006",
        "Get file size failed.", "%s");

const ErrorMsg PackingToolErrMsg::FILE_IO_EXCEPTION =
    BuildPackingToolError("14", "007",
        "File IO exception.", "%s",
        {
            "1. Ensure the file path is correct and the file exists.",
            "2. Verify you have the necessary permissions to access the file.",
            "3. Check for possible disk or file system errors.",
            "4. Review the related IOException message for further insights.",
        });

const ErrorMsg PackingToolErrMsg::COMPRESS_FILE_EXCEPTION =
    BuildPackingToolError("14", "008",
        "Compress file exception.", "%s",
        {
            "Please check the related exception message and modify.",
        });

const ErrorMsg PackingToolErrMsg::FILE_DELETE_FAILED =
    BuildPackingToolError("14", "009",
        "File delete failed.", "%s",
        {
            "%s",
            "Verify you have the necessary permissions to delete the file.",
        });

// ==================== IO exceptions (Type: 15) ====================
const ErrorMsg PackingToolErrMsg::IO_EXCEPTION =
    BuildPackingToolError("15", "001",
        "IO exception.", "%s",
        {
            "1. Ensure the file path is correct and the file exists.",
            "2. Verify you have the necessary permissions to access the file.",
            "3. Check for possible disk or file system errors.",
            "4. Review the related error message for further insights.",
        });

const ErrorMsg PackingToolErrMsg::NULL_POINTER_EXCEPTION =
    BuildPackingToolError("15", "002",
        "Null pointer exception.", "%s",
        {
            "Please review the related exception message for further insights.",
        });

// ==================== HAP validation errors (Type: 16) ====================
const ErrorMsg PackingToolErrMsg::CHECK_APP_FIELDS_INVALID =
    BuildPackingToolError("16", "001",
        "App fields is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::APP_FIELDS_DIFFERENT_ERROR =
    BuildPackingToolError("16", "002",
        "Some app variable is different.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_POLICY_DISJOINT_ERROR =
    BuildPackingToolError("16", "003",
        "Check two distroFilter policy disjoint invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_MODULE_NAME_INVALID =
    BuildPackingToolError("16", "004",
        "Check module name is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_PACKAGE_NAME_INVALID =
    BuildPackingToolError("16", "005",
        "Check packageName invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_HAP_INVALID =
    BuildPackingToolError("16", "006",
        "Check hap info is invalid.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_ENTRY_INVALID =
    BuildPackingToolError("16", "007",
        "Check entry module is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_DEPENDENCY_INVALID =
    BuildPackingToolError("16", "008",
        "Check dependency is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::DEPENDENCY_LIST_INVALID =
    BuildPackingToolError("16", "009",
        "Check dependency list is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_ATOMICSERVICE_INVALID =
    BuildPackingToolError("16", "010",
        "Check atomicservice is invalid.", "%s",
        {
            "Please check the first error message for more details and modify accordingly.",
        });

const ErrorMsg PackingToolErrMsg::ATOMICSERVICE_PRELOADS_INVALID =
    BuildPackingToolError("16", "011",
        "Atomicservice preloads is invalid.", "%s",
        {
            "Please check related preload message and modify preloads settings.",
        });

const ErrorMsg PackingToolErrMsg::TARGET_MODULE_NAME_NOT_EXIST =
    BuildPackingToolError("16", "012",
        "Target moduleName is not exist.", "%s",
        {
            "Make sure pack with vaild and existing target module.",
            "Create missing modules or check target moduleName whether valid.",
        });

const ErrorMsg PackingToolErrMsg::COMPILE_SDK_TYPE_DIFFERENT =
    BuildPackingToolError("16", "013",
        "CompileSdkType is different.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::PROXY_DATA_URI_NOT_UNIQUE =
    BuildPackingToolError("16", "014",
        "The values of uri in proxyData of module.json are not unique.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CONTINUE_TYPE_INVALID =
    BuildPackingToolError("16", "015",
        "Check continueType is invalid.", "%s",
        {
            "%s",
        });

const ErrorMsg PackingToolErrMsg::CHECK_FILE_SIZE_INVALID =
    BuildPackingToolError("16", "016",
        "Check file size failed.", "%s",
        {
            "Please check and reduced related module size.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_HAP_VERIFY_INFO_LIST_EMPTY =
    BuildPackingToolError("16", "017",
        "The provided list of HapVerifyInfos is empty.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_ATOMIC_SERVICE_MODULE_SIZE =
    BuildPackingToolError("16", "018",
        "Check the atomicService module size failed.", "%s",
        {
            "Please check and reduced related module size.",
        });

const ErrorMsg PackingToolErrMsg::CHECK_FEATURE_DISTRO_FILTER_INVALID =
    BuildPackingToolError("16", "019",
        "Check the entry module distributionFilter is invalid.", "%s",
        {
            "Ensure the Entry type module distributionFilter file policy settings is 'exclude' or 'include'.",
        });

// ==================== Package normalization errors (Type: 17) ====================
const ErrorMsg PackingToolErrMsg::PACKAGE_NORMALIZE_FAILED =
    BuildPackingToolError("17", "001",
        "Normalize HSP bundleName and versionCode failed.", "%s");

const ErrorMsg PackingToolErrMsg::UPDATE_MODULE_JSON_FAILED =
    BuildPackingToolError("17", "002",
        "Update module.json failed.", "%s");

const ErrorMsg PackingToolErrMsg::UPDATE_PACKINFO_FAILED =
    BuildPackingToolError("17", "003",
        "Update pack.info failed.", "%s");

// ==================== Packaging tool errors (Type: 18) ====================
const ErrorMsg PackingToolErrMsg::FAST_APP_MODE_ARGS_INVALID =
    BuildPackingToolError("18", "001",
        "Parse and check args invalid in fast app mode.", "%s");

const ErrorMsg PackingToolErrMsg::CHECK_BUNDLE_TYPE_CONSISTENCY =
    BuildPackingToolError("18", "002",
        "Check bundleType consistency failed.", "%s");

const ErrorMsg PackingToolErrMsg::PACK_INFO_INVALID =
    BuildPackingToolError("18", "003",
        "Pack.info is invalid.", "%s");

const ErrorMsg PackingToolErrMsg::ADD_ARCHIVE_ENTRY_FAILED =
    BuildPackingToolError("18", "004",
        "Add archive entry failed.", "%s");

const ErrorMsg PackingToolErrMsg::PACK_MULTI_THREAD_EXCEPTION =
    BuildPackingToolError("18", "005",
        "Packing with multiple threads exception.", "%s");

const ErrorMsg PackingToolErrMsg::REPACK_HSP_EXCEPTION =
    BuildPackingToolError("18", "006",
        "Repack hsp exception.", "%s");

// ==================== Incremental packaging errors (Type: 19) ====================
const ErrorMsg PackingToolErrMsg::INCREMENTAL_PACK_HAP_EXCEPTION =
    BuildPackingToolError("19", "001",
        "Incremental pack hap exception.", "%s");

const ErrorMsg PackingToolErrMsg::INCREMENTAL_PACK_HSP_EXCEPTION =
    BuildPackingToolError("19", "002",
        "Incremental pack hsp exception.", "%s");

// ==================== SO file scanning errors (Type: 20) ====================
const ErrorMsg PackingToolErrMsg::SCAN_SO_FILES_EXCEPTION =
    BuildPackingToolError("20", "001",
        "Scan so files exception.", "%s");

} // namespace error
} // namespace packing_tool
