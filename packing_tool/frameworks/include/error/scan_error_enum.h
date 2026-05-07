#ifndef PACKING_TOOL_SCAN_ERROR_ENUM_H
  #define PACKING_TOOL_SCAN_ERROR_ENUM_H

  #include <string>

  namespace packing_tool {
  namespace error {

  /**
   * @brief 扫描工具错误枚举
   *
   * 使用枚举类定义，每个错误包含code和msg
   */
  enum class ScanErrorEnum {
      SCAN_ENTRANCE_PARSER_ERROR,
      SCAN_ENTRANCE_VERIFY_ERROR,
      SCAN_ENTRANCE_COMPRESS_ERROR,
      SCAN_VERIFY_UTILITY_EMPTY_ERROR,
      SCAN_VERIFY_INPUT_EMPTY_ERROR,
      SCAN_VERIFY_INPUT_INVALID_ERROR,
      SCAN_VERIFY_OUT_PATH_EMPTY_ERROR,
      SCAN_VERIFY_STAT_FILE_SIZE_INVALID_ERROR,
      SCAN_VERIFY_STAT_PARAMETER_EMPTY_ERROR,
      STAT_DUPLICATE_CREATE_FILE_ERROR,
      STAT_DUPLICATE_INPUT_STREAM_ERROR,
      STAT_DUPLICATE_MESSAGE_DIGEST_ERROR,
      SCAN_STAT_FILE_SIZE_CREATE_FILE_PARENT_ERROR,
      SCAN_MKDIRS_ERROR,
      SCAN_NOT_FOUND_ERROR,
      SCAN_REMIND_ERROR,
      SCAN_COMPRESS_ERROR,
      SCAN_DELETE_ERROR,
      SCAN_NO_FILE_ERROR,
      SCAN_UNPACK_ERROR,
      SCAN_UNPACK_STREAM_ERROR,
      SCAN_GET_JS_TEMPLATE_ERROR,
      SCAN_WRITEFILE_ERROR,
      SUFFIX_MKDIRS_ERROR,
      SUFFIX_UNPACK_ERROR,
      SUFFIX_UNPACK_STREAM_ERROR
  };

  /**
   * @brief 获取错误码
   */
  int getScanErrorCode(ScanErrorEnum err);

  /**
   * @brief 获取错误消息
   */
  std::string getScanErrorMsg(ScanErrorEnum err);

  /**
   * @brief 格式化输出
   */
  std::string toString(ScanErrorEnum err);

  /**
   * @brief 格式化输出，带参数
   */
  std::string toStringWithArgs(ScanErrorEnum err, const char* args);

  } // namespace error
  } // namespace packing_tool

  #endif // PACKING_TOOL_SCAN_ERROR_ENUM_H