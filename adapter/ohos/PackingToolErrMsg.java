/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
            .setDescription("Check BundleTypeAppService invalid.")
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
            .setDescription("Check hsp has entry ExtensionAbility failed.")
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
            .setDescription("Compress Stage Hap failed.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_STAGE_HAP_FAILED
     */
    public static final ErrorMsg CHECK_STAGE_HAP_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("003")
            .setDescription("Verify stage hap info failed.")
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
            .setDescription("Read stage hap verify info io exception.")
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
            .addSolution("Please check the related expection message and modify the operation.")
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
            .setDescription("check input hap or hsp file is invalid.")
            .setCause("%s")
            .build();

    /**
     * CHECK_SHARED_APP_INVALID
     */
    public static final ErrorMsg CHECK_SHARED_APP_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("017")
            .setDescription("Check shared App mode invalid.")
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
            .setDescription("Read FA hap verify info io exception.")
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
            .addSolution("Please check the related expection message and modify the operation.")
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
            .addSolution("Please check the expection message and modify.")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_SIZE_FAILED
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_SIZE_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("12")
            .setErrCode("024")
            .setDescription("Atomic service size check failed.")
            .setCause("%s")
            .addSolution("Please check the related size check error message and reduce related module size.")
            .build();

    // module json check error
    /**
     * PARSE_JSON_OBJECT_EXCEPTION
     */
    public static final ErrorMsg PARSE_JSON_OBJECT_EXCEPTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("13")
            .setErrCode("001")
            .setDescription("Parse json Object expection.")
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
            .setDescription("Failed to parse module.json for stage module.")
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
            .setDescription("Check module atomicService installationFree invalid.")
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
            .setDescription("BundleType consistency check failed.")
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
            .setDescription("Failed to parse config.json for FA module.")
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
     * CLOSE_ZIP_OUTPUT_STREAM_EXPECTION
     */
    public static final ErrorMsg CLOSE_ZIP_OUTPUT_STREAM_EXPECTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("002")
            .setDescription("close zip output stream exception.")
            .setCause("%s")
            .addSolution("Please check the related error message and modify the operation.")
            .build();

    /**
     * CLOSE_STREAM_EXPECTION
     */
    public static final ErrorMsg CLOSE_STREAM_EXPECTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("003")
            .setDescription("IO exception when closing stream.")
            .setCause("%s")
            .build();

    /**
     * GET_FILE_CONTENT_FAILED
     */
    public static final ErrorMsg GET_FILE_CONTENT_FAILED = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("14")
            .setErrCode("004")
            .setDescription("Get file content failed.")
            .setCause("%s")
            .addSolution("Please check the related expection message for more details and modify accordingly.")
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

    /*
     * NULL_POINTER_EXPECTION
     */
    public static final ErrorMsg NULL_POINTER_EXPECTION = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("15")
            .setErrCode("002")
            .setDescription("Null pointer exception.")
            .setCause("%s")
            .addSolution("Please review the related error message for further insights.")
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
            .setDescription("Verify hap info is invalid.")
            .setCause("%s")
            .addSolution("Please check the first error message for more details and modify accordingly.")
            .build();

    /**
     * CHECK_ENTRY_INVALID
     */
    public static final ErrorMsg CHECK_ENTRY_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("007")
            .setDescription("Check entry module invalid.")
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
            .addSolution("Please check related error message and modify preloads settings.")
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
            .setDescription("Proxy data uri is not unique.")
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
            .setDescription("The provided list of HapVerifyInfo is empty, unable to process.")
            .setCause("Input list of HapVerifyInfo is empty.")
            .build();

    /**
     * CHECK_ATOMIC_SERVICE_MODULE_SIZE
     */
    public static final ErrorMsg CHECK_ATOMIC_SERVICE_MODULE_SIZE = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("018")
            .setDescription("AtomicService module size check failed.")
            .setCause("%s")
            .addSolution("Please check and reduced related module size.")
            .build();

    /**
     * CHECK_FEATURE_DISTRO_FILTER_INVALID
     */
    public static final ErrorMsg CHECK_FEATURE_DISTRO_FILTER_INVALID = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("019")
            .setDescription("Check feature module distributionFilter is invalid.")
            .setCause("%s")
            .addSolution("Ensure the Entry type module distributionFilter file policy " +
                        "settings is 'exclude' or 'include'.")
            .build();

    /**
     * HAP_VERIFY_INFO_NULL
     */
    public static final ErrorMsg HAP_VERIFY_INFO_EMPTY = ErrorMsg.getPackingToolErrBuilder()
            .setTypeCode("16")
            .setErrCode("020")
            .setDescription("Hap verify infos is empty.")
            .setCause("%s")
            .build();
}
