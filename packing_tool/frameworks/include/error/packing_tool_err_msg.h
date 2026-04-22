 #ifndef PACKING_TOOL_PACKING_TOOL_ERR_MSG_H
  #define PACKING_TOOL_PACKING_TOOL_ERR_MSG_H

  #include "error_msg.h"

  namespace packing_tool {
  namespace error {

  /**
   * @brief 打包工具错误消息定义
   *
   * 使用静态常量定义所有错误类型
   * 错误码格式: [子系统码][类型码][错误码]
   */
  class PackingToolErrMsg {
  public:
      // ==================== 打包工具执行错误 (Type: 10) ====================
      static const ErrorMsg EXECUTE_PACKING_TOOL_FAILED;
      static const ErrorMsg COMMAND_PARSER_FAILED;
      static const ErrorMsg COMMAND_VERIFY_FAILED;

      // ==================== 压缩验证错误 (Type: 11) ====================
      static const ErrorMsg HAP_MODE_ARGS_INVALID;
      static const ErrorMsg HSP_MODE_ARGS_INVALID;
      static const ErrorMsg APP_MODE_ARGS_INVALID;
      static const ErrorMsg MULTI_APP_MODE_ARGS_INVALID;
      static const ErrorMsg RES_MODE_ARGS_INVALID;
      static const ErrorMsg HQF_MODE_ARGS_INVALID;
      static const ErrorMsg APPQF_MODE_ARGS_INVALID;
      static const ErrorMsg PACKAGE_NORMALIZE_MODE_ARGS_INVALID;
      static const ErrorMsg VERSION_NORMALIZE_MODE_ARGS_INVALID;
      static const ErrorMsg HAR_MODE_ARGS_INVALID;
      static const ErrorMsg COMMAND_MODE_INVALID;
      static const ErrorMsg BUNDLE_TYPE_SHARED_INVALID;
      static const ErrorMsg BUNDLE_TYPE_APPSERVICE_INVALID;
      static const ErrorMsg HSP_HAS_ABILITIES_FAILED;
      static const ErrorMsg HSP_HAS_EXTENSION_ABILITIES_FAILED;
      static const ErrorMsg HAS_HOME_ABILITY_INVALID;
      static const ErrorMsg HAS_HOME_EXTENSION_ABILITY_INVALID;
      static const ErrorMsg OUT_PATH_INVALID;
      static const ErrorMsg HAP_ADDITION_MODE_ARGS_INVALID;
      static const ErrorMsg HANDLE_HAP_HSP_INPUT_FAILED;
      static const ErrorMsg GENERAL_NORMALIZE_MODE_ARGS_INVALID;
      static const ErrorMsg PARSE_ATOMIC_SERVICE_SIZE_LIMIT_FAILED;

      // ==================== 压缩处理错误 (Type: 12) ====================
      static const ErrorMsg COMPRESS_PROCESS_FAILED;
      static const ErrorMsg COMPRESS_HAP_FAILED;
      static const ErrorMsg CHECK_STAGE_HAP_FAILED;
      static const ErrorMsg CHECK_AS_TSAN_ENABLED;
      static const ErrorMsg CHECK_HWASAN_ENABLED_INVALID;
      static const ErrorMsg CHECK_ATOMIC_SERVICE_FAILED;
      static const ErrorMsg CHECK_CONTINUE_BUNDLENAME_INVALID;
      static const ErrorMsg CHECK_OVERLAY_CFG_FAILED;
      static const ErrorMsg COMPRESS_PROCESS_EXCEPTION;
      static const ErrorMsg HAS_GENERATE_BUILD_HASH;
      static const ErrorMsg SET_GENERATE_BUILD_HASH;
      static const ErrorMsg CHECK_UBASAN_ENABLED_INVALID;
      static const ErrorMsg READ_STAGE_HAP_VERIFY_INFO_FAILED;
      static const ErrorMsg COMPRESS_PARALLEL_EXCEPTION;
      static const ErrorMsg COMPRESS_APP_FAILED;
      static const ErrorMsg INVALID_HAP_FILE;
      static const ErrorMsg CHECK_SHARED_APP_INVALID;
      static const ErrorMsg CHECK_BUNDLETYPE_INVALID;
      static const ErrorMsg APP_ATOMICSERVICE_COMPRESSED_SIZE_INVALID;
      static const ErrorMsg READ_FA_HAP_VERIFY_INFO_FAILED;
      static const ErrorMsg COMPRESS_APP_IO_EXCEPTION;
      static const ErrorMsg COMPRESS_HSP_FAILED;
      static const ErrorMsg JSON_SPECIAL_PROCESS_FAILED;
      static const ErrorMsg CHECK_ATOMIC_SERVICE_SIZE_FAILED;
      static const ErrorMsg COMPRESS_APP_MODE_FORMULTI_PROJECT_FAILED;
      static const ErrorMsg HAP_ADDITION_FAILED;
      static const ErrorMsg COMPRESS_HAP_ADDITION_FAILED;
      static const ErrorMsg DISPOSE_APP_FAILED;
      static const ErrorMsg NO_PACK_INFO;
      static const ErrorMsg COMPRESS_PACK_RES_MODE_FAILED;
      static const ErrorMsg INVALID_THIRD_LEVEL_DIRECTORY_NAME;
      static const ErrorMsg IS_PICTURING_FAILED;
      static const ErrorMsg GET_FILE_LIST_FAILED;
      static const ErrorMsg GET_MODULE_NAME_FROM_STRING_FAILED;
      static const ErrorMsg COMPRESS_APPQF_FAILED;
      static const ErrorMsg CHECK_HQF_INVALID;
      static const ErrorMsg CHECK_APP_FIELDS_FAILED;
      static const ErrorMsg CHECK_MODULE_INVALID;
      static const ErrorMsg VERSION_NORMALIZE_FAILED;
      static const ErrorMsg VERIFY_MODULE_VERSION_FAILED;
      static const ErrorMsg SHA256_CALCULATION_FAILED;
      static const ErrorMsg COMPRESS_FILE_DUPLICATE;
      static const ErrorMsg COPY_FILE_FAILED;
      static const ErrorMsg MAKE_DIR_FAILED;
      static const ErrorMsg COPY_FILE_SAFELY_FAILED;
      static const ErrorMsg PARSE_MODIFY_CONFIG_JSON_FAILED;
      static const ErrorMsg PARSE_AND_MODIFY_MODULEJSON_FAILED;
      static const ErrorMsg WRITE_JSON_FILE_EXPECTION;
      static const ErrorMsg PARSE_PACK_INFO_JSON_FAILED;
      static const ErrorMsg IS_PLUGIN_HOST_FAILED;
      static const ErrorMsg IS_PERMISSION_SUPPORT_PLUGIN_FAILED;
      static const ErrorMsg CHECK_PKG_CONTEXT_FAILED;
      static const ErrorMsg CHECK_APP_PLUGIN_FAILED;
      static const ErrorMsg CHECK_DEDUPLICATE_HAR_FAILED;
      static const ErrorMsg CHECK_KERNEL_PERMISSION_COMPRESSION_FAILED;

      // ==================== Module JSON检查错误 (Type: 13) ====================
      static const ErrorMsg PARSE_JSON_OBJECT_EXCEPTION;
      static const ErrorMsg PARSE_JSON_FAILED;
      static const ErrorMsg CHECK_MODULE_ATOMIC_SERVICE_FAILED;
      static const ErrorMsg PARSE_STAGE_JSON_FAILED;
      static const ErrorMsg PARSE_STAGE_BUNDLE_TYPE_FAILED;
      static const ErrorMsg CHECK_LEASTONE_ABILITY;
      static const ErrorMsg CHECK_ATOMIC_SERVICE_INSTALLATION_FREE_FAILED;
      static const ErrorMsg PARSE_BUNDLE_NAME_FAILED;
      static const ErrorMsg PARSE_PROXY_DATA_URI_FAILED;
      static const ErrorMsg CHECK_BUNDLETYPE_CONSISTENCY_FAILED;
      static const ErrorMsg PARSE_PATCH_MODULE_NAME_FAILED;
      static const ErrorMsg PARSE_FA_HAP_VERIFY_INFO_FAILED;
      static const ErrorMsg PARSE_FA_JSON_FAILED;
      static const ErrorMsg MERGE_TWO_PACKINFO_FAILED;
      static const ErrorMsg VERIFY_PACKINFO_FAILED;
      static const ErrorMsg VERIFY_APP_PACKINFO_FAILED;
      static const ErrorMsg BUNDLE_TYPE_PACKINFO_INVALID;
      static const ErrorMsg PARSE_PACKINFO_FORMS_NAME_FAILED;
      static const ErrorMsg MERGE_PACKINFO_OBJ_FAILED;
      static const ErrorMsg MERGE_PACKINFO_BY_PACKAGE_PAIR_FAILED;
      static const ErrorMsg MERGE_PACKINFO_OBJ_BY_PACKAGE_PAIR_FAILED;
      static const ErrorMsg PARSE_PACKINFO_FORMS_FAILED;

      // ==================== 文件操作错误 (Type: 14) ====================
      static const ErrorMsg FILE_NOT_FOUND;
      static const ErrorMsg CLOSE_ZIP_OUTPUT_STREAM_EXCEPTION;
      static const ErrorMsg CLOSE_STREAM_EXCEPTION;
      static const ErrorMsg GET_FILE_CONTENT_FAILED;
      static const ErrorMsg FILE_NOT_EXIST;
      static const ErrorMsg GET_FILE_SIZE_FAILED;
      static const ErrorMsg FILE_IO_EXCEPTION;
      static const ErrorMsg COMPRESS_FILE_EXCEPTION;
      static const ErrorMsg FILE_DELETE_FAILED;

      // ==================== IO异常 (Type: 15) ====================
      static const ErrorMsg IO_EXCEPTION;
      static const ErrorMsg NULL_POINTER_EXCEPTION;

      // ==================== HAP验证错误 (Type: 16) ====================
      static const ErrorMsg CHECK_APP_FIELDS_INVALID;
      static const ErrorMsg APP_FIELDS_DIFFERENT_ERROR;
      static const ErrorMsg CHECK_POLICY_DISJOINT_ERROR;
      static const ErrorMsg CHECK_MODULE_NAME_INVALID;
      static const ErrorMsg CHECK_PACKAGE_NAME_INVALID;
      static const ErrorMsg CHECK_HAP_INVALID;
      static const ErrorMsg CHECK_ENTRY_INVALID;
      static const ErrorMsg CHECK_DEPENDENCY_INVALID;
      static const ErrorMsg DEPENDENCY_LIST_INVALID;
      static const ErrorMsg CHECK_ATOMICSERVICE_INVALID;
      static const ErrorMsg ATOMICSERVICE_PRELOADS_INVALID;
      static const ErrorMsg TARGET_MODULE_NAME_NOT_EXIST;
      static const ErrorMsg COMPILE_SDK_TYPE_DIFFERENT;
      static const ErrorMsg PROXY_DATA_URI_NOT_UNIQUE;
      static const ErrorMsg CONTINUE_TYPE_INVALID;
      static const ErrorMsg CHECK_FILE_SIZE_INVALID;
      static const ErrorMsg CHECK_HAP_VERIFY_INFO_LIST_EMPTY;
      static const ErrorMsg CHECK_ATOMIC_SERVICE_MODULE_SIZE;
      static const ErrorMsg CHECK_FEATURE_DISTRO_FILTER_INVALID;

      // ==================== 包规范化错误 (Type: 17) ====================
      static const ErrorMsg PACKAGE_NORMALIZE_FAILED;
      static const ErrorMsg UPDATE_MODULE_JSON_FAILED;
      static const ErrorMsg UPDATE_PACKINFO_FAILED;

      // ==================== 包工具错误 (Type: 18) ====================
      static const ErrorMsg FAST_APP_MODE_ARGS_INVALID;
      static const ErrorMsg CHECK_BUNDLE_TYPE_CONSISTENCY;
      static const ErrorMsg PACK_INFO_INVALID;
      static const ErrorMsg ADD_ARCHIVE_ENTRY_FAILED;
      static const ErrorMsg PACK_MULTI_THREAD_EXCEPTION;
      static const ErrorMsg REPACK_HSP_EXCEPTION;

      // ==================== 增量打包错误 (Type: 19) ====================
      static const ErrorMsg INCREMENTAL_PACK_HAP_EXCEPTION;
      static const ErrorMsg INCREMENTAL_PACK_HSP_EXCEPTION;

      // ==================== SO文件扫描错误 (Type: 20) ====================
      static const ErrorMsg SCAN_SO_FILES_EXCEPTION;

  private:
      PackingToolErrMsg() = delete;  // 禁止实例化
  };

  } // namespace error
  } // namespace packing_tool

  #endif // PACKING_TOOL_PACKING_TOOL_ERR_MSG_H