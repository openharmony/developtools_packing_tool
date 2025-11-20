/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

/**
 * PackingToolErrMsg
 *
 * @since 2025/01/21
 */
public class PackingToolErrMsg {

    // packing tool error
    /**
     * EXECUTE_PACKING_TOOL_FAILED
     */
    public static final ErrorMsg EXECUTE_PACKING_TOOL_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("10")
            .setErrCode("001")
            .setDescription("Execute packing tool failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * COMMAND_PARSER_FAILED
     */
    public static final ErrorMsg COMMAND_PARSER_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("10")
            .setErrCode("002")
            .setDescription("Command parser failed.")
            .setCause("%s")
            .build();

    /**
     * COMMAND_VERIFY_FAILED
     */
    public static final ErrorMsg COMMAND_VERIFY_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("10")
            .setErrCode("003")
            .setDescription("Command verify failed.")
            .setCause("%s")
            .build();

    // compress verify error
    /**
     * HAP_MODE_ARGS_INVALID
     */
    public static final ErrorMsg HAP_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("001")
            .setDescription("Parse and check args invalid in hap mode.")
            .setCause("%s")
            .build();

    /**
     * HSP_MODE_ARGS_INVALID
     */
    public static final ErrorMsg HSP_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("002")
            .setDescription("Parse and check args invalid in hsp mode.")
            .setCause("%s")
            .build();

    /**
     * APP_MODE_ARGS_INVALID
     */
    public static final ErrorMsg APP_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("003")
            .setDescription("Parse and check args invalid in app mode.")
            .setCause("%s")
            .build();

    /**
     * MULTI_APP_MODE_ARGS_INVALID
     */
    public static final ErrorMsg MULTI_APP_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("004")
            .setDescription("Parse and check args invalid in multiApp mode.")
            .setCause("%s")
            .build();

    /**
     * RES_MODE_ARGS_INVALID
     */
    public static final ErrorMsg RES_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("005")
            .setDescription("Parse and check args invalid in res mode.")
            .setCause("%s")
            .build();

    /**
     * HQF_MODE_ARGS_INVALID
     */
    public static final ErrorMsg HQF_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("006")
            .setDescription("Parse and check args invalid in hqf mode.")
            .setCause("%s")
            .build();

    /**
     * APPQF_MODE_ARGS_INVALID
     */
    public static final ErrorMsg APPQF_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("007")
            .setDescription("Parse and check args invalid in appqf mode.")
            .setCause("%s")
            .build();

    /**
     * PACKAGE_NORMALIZE_MODE_ARGS_INVALID
     */
    public static final ErrorMsg PACKAGE_NORMALIZE_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("008")
            .setDescription("Parse and check args invalid in packageNormalize mode.")
            .setCause("%s")
            .build();

    /**
     * VERSION_NORMALIZE_MODE_ARGS_INVALID
     */
    public static final ErrorMsg VERSION_NORMALIZE_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("009")
            .setDescription("Parse and check args invalid in versionNormalize mode.")
            .setCause("%s")
            .build();

    /**
     * HAR_MODE_ARGS_INVALID
     */
    public static final ErrorMsg HAR_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("010")
            .setDescription("Parse and check args invalid in har mode.")
            .setCause("%s")
            .build();

    /**
     * COMMAND_MODE_INVALID
     */
    public static final ErrorMsg COMMAND_MODE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("011")
            .setDescription("Command verify mode is invalid.")
            .setCause("Input mode is invalid.")
            .build();

    /**
     * BUNDLE_TYPE_SHARED_INVALID
     */
    public static final ErrorMsg BUNDLE_TYPE_SHARED_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("012")
            .setDescription("Check bundleType is shared failed.")
            .setCause("%s")
            .build();

    /**
     * BUNDLE_TYPE_APPSERVICE_INVALID
     */
    public static final ErrorMsg BUNDLE_TYPE_APPSERVICE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("013")
            .setDescription("Check the bundleType is appService failed.")
            .setCause("%s")
            .build();

    /**
     * HSP_HAS_ABILITIES_FAILED
     */
    public static final ErrorMsg HSP_HAS_ABILITIES_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("014")
            .setDescription("Check hsp has abilities failed.")
            .setCause("%s")
            .build();

    /**
     * HSP_HAS_EXTENSION_ABILITIES_FAILED
     */
    public static final ErrorMsg HSP_HAS_EXTENSION_ABILITIES_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("015")
            .setDescription("Check hsp has extension abilities failed.")
            .setCause("%s")
            .build();

    /**
     * HAS_HOME_ABILITY_INVALID
     */
    public static final ErrorMsg HAS_HOME_ABILITY_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("016")
            .setDescription("Check hsp has entry abilitiy failed.")
            .setCause("%s")
            .build();

    /**
     * HAS_HOME_EXTENSION_ABILITY_INVALID
     */
    public static final ErrorMsg HAS_HOME_EXTENSION_ABILITY_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("017")
            .setDescription("Check hsp has entry extensionAbility failed.")
            .setCause("%s")
            .build();

    /**
     * OUT_PATH_INVALID
     */
    public static final ErrorMsg OUT_PATH_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("018")
            .setDescription("Invalid output path.")
            .setCause("%s")
            .build();

    /**
     * HAP_ADDITION_MODE_ARGS_INVALID
     */
    public static final ErrorMsg HAP_ADDITION_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("019")
            .setDescription("Parse and check args invalid in hapAddition mode.")
            .setCause("%s")
            .build();

    /**
     * HANDLE_HAP_HSP_INPUT_FAILED
     */
    public static final ErrorMsg HANDLE_HAP_HSP_INPUT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("020")
            .setDescription("Handle hap and hsp input failed.")
            .setCause("%s")
            .build();

    /**
     * GENERAL_NORMALIZE_MODE_ARGS_INVALID
     */
    public static final ErrorMsg GENERAL_NORMALIZE_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("021")
            .setDescription("Parse and check args invalid in generalNormalize mode.")
            .setCause("%s")
            .build();

    /**
     * PARSE_ATOMIC_SERVICE_SIZE_LIMIT_FAILED
     */
    public static final ErrorMsg PARSE_ATOMIC_SERVICE_SIZE_LIMIT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("11")
            .setErrCode("022")
            .setDescription("Parse atomicService size limit failed.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    // compress process error
    /**
     * COMPRESS_PROCESS_FAILED
     */
    public static final ErrorMsg COMPRESS_PROCESS_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("001")
            .setDescription("Execute compress process failed.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * COMPRESS_HAP_FAILED
     */
    public static final ErrorMsg COMPRESS_HAP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("002")
            .setDescription("Compress Stage hap failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_STAGE_HAP_FAILED
     */
    public static final ErrorMsg CHECK_STAGE_HAP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("003")
            .setDescription("Verify Stage hap info failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_AS_TSAN_ENABLED
     */
    public static final ErrorMsg CHECK_AS_TSAN_ENABLED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("004")
            .setDescription("Check asanEnabled failed.")
            .setCause("%s")
            .addSolution("Please ensure the parameters are correctly set. " +
                        "Review 'app.json' and adjust the configuration as needed.")
            .build();

    /**
     * CHECK_HWASAN_ENABLED_INVALID
     */
    public static final ErrorMsg CHECK_HWASAN_ENABLED_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("005")
            .setDescription("Check hwasanEnabled failed.")
            .setCause("%s")
            .addSolution("Please ensure the parameters are correctly set. " +
                        "Review 'app.json' and adjust the configuration as needed.")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_FAILED
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("006")
            .setDescription("Check atomicService failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_CONTINUE_BUNDLENAME_INVALID
     */
    public static final ErrorMsg CHECK_CONTINUE_BUNDLENAME_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("007")
            .setDescription("Check continueBundleName invalid.")
            .setCause("%s")
            .addSolution("Ensure continueBundleName not inclue self.")
            .build();

    /**
     * CHECK_OVERLAY_CFG_FAILED
     */
    public static final ErrorMsg CHECK_OVERLAY_CFG_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("008")
            .setDescription("Check whether is an overlay hsp failed.")
            .setCause("%s")
            .addSolution("Please check 'app.json5' and 'module.json5' file according to the error.")
            .addSolution("Check targetBundleName, targetModuleName, and targetPriority in the related module.")
            .addSolution("Note: Modules configured with the targetModuleName is overlay feature module.")
            .addSolution("Note: Modules configured with the targetBundleName is overlay feature application.")
            .build();

    /**
     * COMPRESS_PROCESS_EXCEPTION
     */
    public static final ErrorMsg COMPRESS_PROCESS_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("009")
            .setDescription("Process compress exception.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * HAS_GENERATE_BUILD_HASH
     */
    public static final ErrorMsg HAS_GENERATE_BUILD_HASH = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("010")
            .setDescription("Verify has generate build hash failed.")
            .setCause("%s")
            .build();

    /**
     * SET_GENERATE_BUILD_HASH
     */
    public static final ErrorMsg SET_GENERATE_BUILD_HASH = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("011")
            .setDescription("Set generate build hash failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_UBASAN_ENABLED_INVALID
     */
    public static final ErrorMsg CHECK_UBASAN_ENABLED_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("012")
            .setDescription("Check ubsanEnabled failed.")
            .setCause("%s")
            .addSolution("Please ensure the parameters are correctly set. " +
                        "Review 'app.json' and adjust the configuration as needed.")
            .build();

    /**
     * READ_STAGE_HAP_VERIFY_INFO_FAILED
     */
    public static final ErrorMsg READ_STAGE_HAP_VERIFY_INFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("013")
            .setDescription("Read Stage hap verify info exist exception.")
            .setCause("%s")
            .addSolution("Please check the exception message for more details and modify accordingly.")
            .build();

    /**
     * COMPRESS_PARALLEL_EXCEPTION
     */
    public static final ErrorMsg COMPRESS_PARALLEL_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("014")
            .setDescription("Parallel compress exception.")
            .setCause("%s")
            .addSolution("Please check the related exception message and modify the operation.")
            .build();

    /**
     * COMPRESS_APP_FAILED
     */
    public static final ErrorMsg COMPRESS_APP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("015")
            .setDescription("Compress app file failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * INVALID_HAP_FILE
     */
    public static final ErrorMsg INVALID_HAP_FILE = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("016")
            .setDescription("Check input hap or hsp file is invalid.")
            .setCause("%s")
            .build();

    /**
     * CHECK_SHARED_APP_INVALID
     */
    public static final ErrorMsg CHECK_SHARED_APP_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("017")
            .setDescription("Check shared App invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_BUNDLETYPE_INVALID
     */
    public static final ErrorMsg CHECK_BUNDLETYPE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("018")
            .setDescription("Check bundleType is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID
     */
    public static final ErrorMsg APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("019")
            .setDescription("Check app atomicservice compress size failed.")
            .setCause("%s")
            .build();

    /**
     * READ_FA_HAP_VERIFY_INFO_FAILED
     */
    public static final ErrorMsg READ_FA_HAP_VERIFY_INFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("020")
            .setDescription("Read FA hap verify info exist exception.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * COMPRESS_APP_IO_EXCEPTION
     */
    public static final ErrorMsg COMPRESS_APP_IO_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("021")
            .setDescription("IO exception when compress app.")
            .setCause("%s")
            .addSolution("Please check the related exception message and modify the operation.")
            .build();

    /**
     * COMPRESS_HSP_FAILED
     */
    public static final ErrorMsg COMPRESS_HSP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("022")
            .setDescription("Compress Hsp failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * JSON_SPECIAL_PROCESS_FAILED
     */
    public static final ErrorMsg JSON_SPECIAL_PROCESS_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("023")
            .setDescription("Json special process exist exception.")
            .setCause("%s")
            .addSolution("Please check the exception message and modify.")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_SIZE_FAILED
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_SIZE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("024")
            .setDescription("Check atomicService size failed.")
            .setCause("%s")
            .addSolution("Please check the related size check error message and reduce related module size.")
            .build();

    /**
     * COMPRESS_APP_MODE_FORMULTI_PROJECT_FAILED
     */
    public static final ErrorMsg COMPRESS_APP_MODE_FORMULTI_PROJECT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("025")
            .setDescription("Compress in app mode for multi project failed.")
            .setCause("%s")
            .build();

    /**
     * HAP_ADDITION_FAILED
     */
    public static final ErrorMsg HAP_ADDITION_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("026")
            .setDescription("Hap addition failed.")
            .setCause("%s")
            .build();

    /**
     * COMPRESS_HAP_ADDITION_FAILED
     */
    public static final ErrorMsg COMPRESS_HAP_ADDITION_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("027")
            .setDescription("Compress hap addition failed.")
            .setCause("%s")
            .build();

    /**
     * DISPOSE_APP_FAILED
     */
    public static final ErrorMsg DISPOSE_APP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("028")
            .setDescription("Dispose app failed.")
            .setCause("%s")
            .build();

    /**
     * NO_PACK_INFO
     */
    public static final ErrorMsg NO_PACK_INFO = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("029")
            .setDescription("pack.info does not find in hap or app.")
            .setCause("%s")
            .build();

    /**
     * COMPRESS_PACK_RES_MODE_FAILED
     */
    public static final ErrorMsg COMPRESS_PACK_RES_MODE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("030")
            .setDescription("Compress in res mode failed.")
            .setCause("%s")
            .build();

    /**
     * INVALID_THIRD_LEVEL_DIRECTORY_NAME
     */
    public static final ErrorMsg INVALID_THIRD_LEVEL_DIRECTORY_NAME = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("031")
            .setDescription("Invalid third level directory name.")
            .setCause("%s")
            .addSolution("Please check it with reference to this example: \n" +
                    "zh_Hani_CN-vertical-car-mdpi-dark or zh_Hani_CN-vertical-car-mdpi.")
            .build();

    /**
     * IS_PICTURING_FAILED
     */
    public static final ErrorMsg IS_PICTURING_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("032")
            .setDescription("Check whether picturingName meets specifications failed.")
            .setCause("%s")
            .build();

    /**
     * GET_FILE_LIST_FAILED
     */
    public static final ErrorMsg GET_FILE_LIST_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("033")
            .setDescription("Get file list failed.")
            .setCause("%s")
            .build();

    /**
     * GET_MODULE_NAME_FROM_STRING_FAILED
     */
    public static final ErrorMsg GET_MODULE_NAME_FROM_STRING_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("034")
            .setDescription("Get module name from line string failed.")
            .setCause("%s")
            .build();

    /**
     * COMPRESS_APPQF_FAILED
     */
    public static final ErrorMsg COMPRESS_APPQF_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("035")
            .setDescription("Compress in appqf mode failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_HQF_INVALID
     */
    public static final ErrorMsg CHECK_HQF_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("036")
            .setDescription("Input hqf is invalid.")
            .setCause("%s")
            .build();

    /**
     * CHECK_APP_FIELDS_FAILED
     */
    public static final ErrorMsg CHECK_APP_FIELDS_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("037")
            .setDescription("Check hqf has same app fields failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_MODULE_INVALID
     */
    public static final ErrorMsg CHECK_MODULE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("038")
            .setDescription("Check hqf module is invalid.")
            .setCause("%s")
            .build();

    /**
     * VERSION_NORMALIZE_FAILED
     */
    public static final ErrorMsg VERSION_NORMALIZE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("039")
            .setDescription("Version normalize failed.")
            .setCause("%s")
            .build();

    /**
     * VERIFY_MODULE_VERSION_FAILED
     */
    public static final ErrorMsg VERIFY_MODULE_VERSION_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("040")
            .setDescription("Verify module version failed.")
            .setCause("%s")
            .build();

    /**
     * SHA256_CALCULATION_FAILED
     */
    public static final ErrorMsg SHA256_CALCULATION_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("041")
            .setDescription("SHA-256 hash calculation failed.")
            .setCause("%s")
            .build();

    /**
     * COMPRESS_FILE_DUPLICATE
     */
    public static final ErrorMsg COMPRESS_FILE_DUPLICATE = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("042")
            .setDescription("Select haps find duplicated hap.")
            .setCause("%s")
            .build();

    /**
     * COPY_FILE_FAILED
     */
    public static final ErrorMsg COPY_FILE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("043")
            .setDescription("Copy file failed.")
            .setCause("%s")
            .build();

    /**
     * MAKE_DIR_FAILED
     */
    public static final ErrorMsg MAKE_DIR_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("044")
            .setDescription("Make dir failed.")
            .setCause("%s")
            .build();

    /**
     * COPY_FILE_SAFELY_FAILED
     */
    public static final ErrorMsg COPY_FILE_SAFELY_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("045")
            .setDescription("Copy file safely failed.")
            .setCause("%s")
            .build();

    /**
     * PARSE_MODIFY_CONFIG_JSON_FAILED
     */
    public static final ErrorMsg PARSE_MODIFY_CONFIG_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("046")
            .setDescription("Parse and modify config.json failed.")
            .setCause("%s")
            .build();

    /**
     * PARSE_AND_MODIFY_MODULEJSON_FAILED
     */
    public static final ErrorMsg PARSE_AND_MODIFY_MODULEJSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("047")
            .setDescription("Parse and modify module.json failed.")
            .setCause("%s")
            .build();

    /**
     * WRITE_JSON_FILE_EXPECTION
     */
    public static final ErrorMsg WRITE_JSON_FILE_EXPECTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("048")
            .setDescription("Write json file exception.")
            .setCause("%s")
            .build();

    /**
     * PARSE_PACK_INFO_JSON_FAILED
     */
    public static final ErrorMsg PARSE_PACK_INFO_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("049")
            .setDescription("Parse pack.info json file failed.")
            .setCause("%s")
            .build();

    /**
     * IS_PLUGIN_HOST_FAILED
     */
    public static final ErrorMsg IS_PLUGIN_HOST_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("050")
            .setDescription("Check is plugin host failed.")
            .setCause("%s")
            .build();

    /**
     * IS_PERMISSION_SUPPORT_PLUGIN_FAILED
     */
    public static final ErrorMsg IS_PERMISSION_SUPPORT_PLUGIN_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("051")
            .setDescription("Check is permission support plugin failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_PKG_CONTEXT_FAILED
     */
    public static final ErrorMsg CHECK_PKG_CONTEXT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("052")
            .setDescription("Check pkg context failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_APP_PLUGIN_FAILED
     */
    public static final ErrorMsg CHECK_APP_PLUGIN_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("053")
            .setDescription("Check app plugin failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_HSP_DEDUPLICATE_HAR_FAILED
     */
    public static final ErrorMsg CHECK_DEDUPLICATE_HAR_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("054")
            .setDescription("Check deduplicateHar field failed.")
            .setCause("%s")
            .build();

    // module json check error
    /**
     * PARSE_JSON_OBJECT_EXCEPTION
     */
    public static final ErrorMsg PARSE_JSON_OBJECT_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("001")
            .setDescription("Parse json object exception.")
            .setCause("%s")
            .build();

    /**
     * PARSE_JSON_FAILED
     */
    public static final ErrorMsg PARSE_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("002")
            .setDescription("Parse json profile failed.")
            .setCause("%s")
            .build();

    /**
     * CHECK_MODULE_ATOMIC_SERVICE_FAILED
     */
    public static final ErrorMsg CHECK_MODULE_ATOMIC_SERVICE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("003")
            .setDescription("Check module atomicService invalid.")
            .setCause("%s")
            .build();

    /**
     * PARSE_STAGE_JSON_FAILED
     */
    public static final ErrorMsg PARSE_STAGE_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("004")
            .setDescription("Failed to parse module.json for the Stage module.")
            .setCause("%s")
            .build();

    /**
     * PARSE_STAGE_BUNDLE_TYPE_FAILED
     */
    public static final ErrorMsg PARSE_STAGE_BUNDLE_TYPE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("005")
            .setDescription("Failed to parse module.json and bundleType.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_LEASTONE_ABILITY
     */
    public static final ErrorMsg CHECK_LEASTONE_ABILITY = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("006")
            .setDescription("Check entry module at least one ability failed.")
            .setCause("%s")
            .addSolution("Ensure the entry type module contains at least one ability.")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("007")
            .setDescription("Check module atomicService and installationFree invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * PARSE_BUNDLE_NAME_FAILED
     */
    public static final ErrorMsg PARSE_BUNDLE_NAME_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("008")
            .setDescription("Get the bundleName from json file failed.")
            .setCause("%s")
            .build();

    /**
     * PARSE_PROXY_DATA_URI_FAILED
     */
    public static final ErrorMsg PARSE_PROXY_DATA_URI_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("009")
            .setDescription("Failed to parse module.json and proxyData object.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_BUNDLETYPE_CONSISTENCY_FAILED
     */
    public static final ErrorMsg CHECK_BUNDLETYPE_CONSISTENCY_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("010")
            .setDescription("Failed to check the consistency of bundleType.")
            .setCause("%s")
            .addSolution("Make sure the bundleType is consistency for different modules.")
            .build();

    /**
     * PARSE_PATCH_MODULE_NAME_FAILED
     */
    public static final ErrorMsg PARSE_PATCH_MODULE_NAME_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("011")
            .setDescription("Failed to parse patch module name from patch.json.")
            .setCause("%s")
            .build();

    /**
     * PARSE_FA_HAP_VERIFY_INFO_FAILED
     */
    public static final ErrorMsg PARSE_FA_HAP_VERIFY_INFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("012")
            .setDescription("Failed to parse FA hap verify info from config.json.")
            .setCause("%s")
            .build();

    /**
     * PARSE_FA_JSON_FAILED
     */
    public static final ErrorMsg PARSE_FA_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("013")
            .setDescription("Failed to parse config.json for the FA module.")
            .setCause("%s")
            .build();

    /**
     * MERGE_TWO_PACKINFO_FAILED
     */
    public static final ErrorMsg MERGE_TWO_PACKINFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("014")
            .setDescription("Merge two pack.info into one pack.info file failed.")
            .setCause("%s")
            .build();

    /**
     * VERIFY_PACKINFO_FAILED
     */
    public static final ErrorMsg VERIFY_PACKINFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("015")
            .setDescription("Verify pack.info file failed.")
            .setCause("%s")
            .build();

    /**
     * VERIFY_APP_PACKINFO_FAILED
     */
    public static final ErrorMsg VERIFY_APP_PACKINFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("016")
            .setDescription("Verify app in pack.info failed.")
            .setCause("%s")
            .build();

    /**
     * BUNDLE_TYPE_PACKINFO_INVALID
     */
    public static final ErrorMsg BUNDLE_TYPE_PACKINFO_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("017")
            .setDescription("Verify bundleType in pack.info file failed.")
            .setCause("%s")
            .build();

    /**
     * PARSE_PACKINFO_FORMS_NAME_FAILED
     */
    public static final ErrorMsg PARSE_PACKINFO_FORMS_NAME_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("018")
            .setDescription("Parse forms name in pack.info file failed.")
            .setCause("%s")
            .build();

    /**
     * MERGE_PACKINFO_OBJ_FAILED
     */
    public static final ErrorMsg MERGE_PACKINFO_OBJ_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("019")
            .setDescription("Failed to merge pack.info objects.")
            .setCause("%s")
            .build();

    /**
     * MERGE_PACKINFO_BY_PACKAGE_PAIR_FAILED
     */
    public static final ErrorMsg MERGE_PACKINFO_BY_PACKAGE_PAIR_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("020")
            .setDescription("Failed to merge two pack.info files into one pack.info file by packagePair.")
            .setCause("%s")
            .build();

    /**
     * MERGE_PACKINFO_OBJ_BY_PACKAGE_PAIR_FAILED
     */
    public static final ErrorMsg MERGE_PACKINFO_OBJ_BY_PACKAGE_PAIR_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("021")
            .setDescription(
                    "Failed to merge two pack.info json objects into one pack.info json object by packagePair.")
            .setCause("%s")
            .build();

    /**
     * PARSE_PACKINFO_FORMS_FAILED
     */
    public static final ErrorMsg PARSE_PACKINFO_FORMS_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("022")
            .setDescription("Failed to parse pack.info forms.")
            .setCause("%s")
            .build();

    // file operator error
    /**
     * FILE_NOT_FOUND
     */
    public static final ErrorMsg FILE_NOT_FOUND = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("001")
            .setDescription("File avaiable not found exception.")
            .setCause("%s")
            .addSolution("Please check the related FileNotFoundException message.")
            .addSolution("If the file is in use, close any applications or processes that might be using it.")
            .build();

    /**
     * CLOSE_ZIP_OUTPUT_STREAM_EXCEPTION
     */
    public static final ErrorMsg CLOSE_ZIP_OUTPUT_STREAM_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("002")
            .setDescription("Close zip output stream exception.")
            .setCause("%s")
            .addSolution("Please check the related exception message and modify the operation.")
            .build();

    /**
     * CLOSE_STREAM_EXCEPTION
     */
    public static final ErrorMsg CLOSE_STREAM_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("003")
            .setDescription("IO exception when closing stream.")
            .addSolution("Please check the related exception message for more details and modify accordingly.")
            .build();

    /**
     * GET_FILE_CONTENT_FAILED
     */
    public static final ErrorMsg GET_FILE_CONTENT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("004")
            .setDescription("Get file content failed.")
            .setCause("%s")
            .addSolution("Please check the related exception message for more details and modify accordingly.")
            .build();

    /**
     * FILE_NOT_EXIST
     */
    public static final ErrorMsg FILE_NOT_EXIST = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("005")
            .setDescription("Parse file not exist.")
            .setCause("%s")
            .build();

    /**
     * GET_FILE_SIZE_FAILED
     */
    public static final ErrorMsg GET_FILE_SIZE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("006")
            .setDescription("Get file size failed.")
            .setCause("%s")
            .build();

    /**
     * FILE_IO_EXCEPTION
     */
    public static final ErrorMsg FILE_IO_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("007")
            .setDescription("File IO exception.")
            .setCause("%s")
            .addSolution("1. Ensure the file path is correct and the file exists.")
            .addSolution("2. Verify you have the necessary permissions to access the file.")
            .addSolution("3. Check for possible disk or file system errors.")
            .addSolution("4. Review the related IOException message for further insights.")
            .build();

    /**
     * COMPRESS_FILE_EXCEPTION
     */
    public static final ErrorMsg COMPRESS_FILE_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("008")
            .setDescription("Compress file exception.")
            .setCause("%s")
            .addSolution("Please check the related exception message and modify.")
            .build();

    /**
     * FILE_DELETE_FAILED
     */
    public static final ErrorMsg FILE_DELETE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("009")
            .setDescription("File delete failed.")
            .setCause("%s")
            .addSolution("%s")
            .addSolution("Verify you have the necessary permissions to delete the file.")
            .build();

    // io exception
    /**
     * IO_EXCEPTION
     */
    public static final ErrorMsg IO_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("15")
            .setErrCode("001")
            .setDescription("IO exception.")
            .setCause("%s")
            .addSolution("1. Ensure the file path is correct and the file exists.")
            .addSolution("2. Verify you have the necessary permissions to access the file.")
            .addSolution("3. Check for possible disk or file system errors.")
            .addSolution("4. Review the related error message for further insights.")
            .build();

    /**
     * NULL_POINTER_EXCEPTION
     */
    public static final ErrorMsg NULL_POINTER_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("15")
            .setErrCode("002")
            .setDescription("Null pointer exception.")
            .setCause("%s")
            .addSolution("Please review the related exception message for further insights.")
            .build();

    // hap verify error
    /**
     * CHECK_APP_FIELDS_INVALID
     */
    public static final ErrorMsg CHECK_APP_FIELDS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("001")
            .setDescription("App fields is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * APP_FIELDS_DIFFERENT_ERROR
     */
    public static final ErrorMsg APP_FIELDS_DIFFERENT_ERROR = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("002")
            .setDescription("Some app variable is different.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_POLICY_DISJOINT_ERROR
     */
    public static final ErrorMsg CHECK_POLICY_DISJOINT_ERROR = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("003")
            .setDescription("Check two distroFilter policy disjoint invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_MODULE_NAME_INVALID
     */
    public static final ErrorMsg CHECK_MODULE_NAME_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("004")
            .setDescription("Check module name is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_PACKAGE_NAME_INVALID
     */
    public static final ErrorMsg CHECK_PACKAGE_NAME_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("005")
            .setDescription("Check packageName invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_HAP_INVALID
     */
    public static final ErrorMsg CHECK_HAP_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("006")
            .setDescription("Check hap info is invalid.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_ENTRY_INVALID
     */
    public static final ErrorMsg CHECK_ENTRY_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("007")
            .setDescription("Check entry module is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_DEPENDENCY_INVALID
     */
    public static final ErrorMsg CHECK_DEPENDENCY_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("008")
            .setDescription("Check dependency is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * DEPENDENCY_LIST_INVALID
     */
    public static final ErrorMsg DEPENDENCY_LIST_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("009")
            .setDescription("Check dependency list is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_ATOMICSERVICE_INVALID
     */
    public static final ErrorMsg CHECK_ATOMICSERVICE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("010")
            .setDescription("Check atomicservice is invalid.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * ATOMICSERVICE_PRELOADS_INVALID
     */
    public static final ErrorMsg ATOMICSERVICE_PRELOADS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("011")
            .setDescription("Atomicservice preloads is invalid.")
            .setCause("%s")
            .addSolution("Please check related preload message and modify preloads settings.")
            .build();

    /**
     * TARGET_MODULE_NAME_NOT_EXIST
     */
    public static final ErrorMsg TARGET_MODULE_NAME_NOT_EXIST = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("012")
            .setDescription("Target moduleName is not exist.")
            .setCause("%s")
            .addSolution("Make sure pack with vaild and existing target module.")
            .addSolution("Create missing modules or check target moduleName whether valid.")
            .build();

    /**
     * COMPILE_SDK_TYPE_DIFFERENT
     */
    public static final ErrorMsg COMPILE_SDK_TYPE_DIFFERENT = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("013")
            .setDescription("CompileSdkType is different.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * PROXY_DATA_URI_NOT_UNIQUE
     */
    public static final ErrorMsg PROXY_DATA_URI_NOT_UNIQUE = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("014")
            .setDescription("The values of uri in proxyData of module.json are not unique.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CONTINUE_TYPE_INVALID
     */
    public static final ErrorMsg CONTINUE_TYPE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("015")
            .setDescription("Check continueType is invalid.")
            .setCause("%s")
            .addSolution("%s")
            .build();

    /**
     * CHECK_FILE_SIZE_INVALID
     */
    public static final ErrorMsg CHECK_FILE_SIZE_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("016")
            .setDescription("Check file size failed.")
            .setCause("%s")
            .addSolution("Please check and reduced related module size.")
            .build();

    /**
     * CHECK_HAP_VERIFY_INFO_LIST_EMPTY
     */
    public static final ErrorMsg CHECK_HAP_VERIFY_INFO_LIST_EMPTY = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("017")
            .setDescription("The provided list of HapVerifyInfos is empty.")
            .setCause("%s")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_MODULE_SIZE
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_MODULE_SIZE = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("018")
            .setDescription("Check the atomicService module size failed.")
            .setCause("%s")
            .addSolution("Please check and reduced related module size.")
            .build();

    /**
     * CHECK_FEATURE_DISTRO_FILTER_INVALID
     */
    public static final ErrorMsg CHECK_FEATURE_DISTRO_FILTER_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("019")
            .setDescription("Check the entry module distributionFilter is invalid.")
            .setCause("%s")
            .addSolution("Ensure the Entry type module distributionFilter file policy " +
                        "settings is 'exclude' or 'include'.")
            .build();

    // package normalize error
    /**
     * PACKAGE_NORMALIZE_FAILED
     */
    public static final ErrorMsg PACKAGE_NORMALIZE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("17")
            .setErrCode("001")
            .setDescription("Normalize HSP bundleName and versionCode failed.")
            .setCause("%s")
            .build();

    /**
     * UPDATE_MODULE_JSON_FAILED
     */
    public static final ErrorMsg UPDATE_MODULE_JSON_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("17")
            .setErrCode("002")
            .setDescription("Update module.json failed.")
            .setCause("%s")
            .build();

    /**
     * UPDATE_PACKINFO_FAILED
     */
    public static final ErrorMsg UPDATE_PACKINFO_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("17")
            .setErrCode("003")
            .setDescription("Update pack.info failed.")
            .setCause("%s")
            .build();

    // package util error
    /**
     * FAST_APP_MODE_ARGS_INVALID
     */
    public static final ErrorMsg FAST_APP_MODE_ARGS_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("001")
            .setDescription("Parse and check args invalid in fast app mode.")
            .setCause("%s")
            .build();

    /**
     * CHECK_BUNDLE_TYPE_CONSISTENCY
     */
    public static final ErrorMsg CHECK_BUNDLE_TYPE_CONSISTENCY = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("002")
            .setDescription("Check bundleType consistency failed.")
            .setCause("%s")
            .build();

    /**
     * PACK_INFO_INVALID
     */
    public static final ErrorMsg PACK_INFO_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("003")
            .setDescription("Pack.info is invalid.")
            .setCause("%s")
            .build();

    /**
     * ADD_ARCHIVE_ENTRY_FAILED
     */
    public static final ErrorMsg ADD_ARCHIVE_ENTRY_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("004")
            .setDescription("Add archive entry failed.")
            .setCause("%s")
            .build();

    /**
     * PACK_MULTI_THREAD_EXCEPTION
     */
    public static final ErrorMsg PACK_MULTI_THREAD_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("005")
            .setDescription("Packing with multiple threads exception.")
            .setCause("%s")
            .build();

    /**
     * REPACK_HSP_EXCEPTION
     */
    public static final ErrorMsg REPACK_HSP_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("18")
            .setErrCode("006")
            .setDescription("Repack hsp exception.")
            .setCause("%s")
            .build();

    /**
     * INCREMENTAL_PACK_HAP_EXCEPTION
     */
    public static final ErrorMsg INCREMENTAL_PACK_HAP_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("19")
            .setErrCode("001")
            .setDescription("Incremental pack hap exception.")
            .setCause("%s")
            .build();

    /**
     * INCREMENTAL_PACK_HSP_EXCEPTION
     */
    public static final ErrorMsg INCREMENTAL_PACK_HSP_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("19")
            .setErrCode("002")
            .setDescription("Incremental pack hsp exception.")
            .setCause("%s")
            .build();

    /**
     * SCAN_SO_FILES_EXCEPTION
     */
    public static final ErrorMsg SCAN_SO_FILES_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("20")
            .setErrCode("001")
            .setDescription("Scan so files exception.")
            .setCause("%s")
            .build();
}
