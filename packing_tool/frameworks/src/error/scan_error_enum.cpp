#include "scan_error_enum.h"

  namespace packing_tool {
  namespace error {

  namespace {
      struct ScanErrorInfo {
          int code;
          const char* msg;
      };

      // 错误信息映射表
      const ScanErrorInfo SCAN_ERROR_MAP[] = {
          {9132000, "ScanEntrance main exit, parser failed."},
          {9132001, "ScanEntrance main exit, verify failed."},
          {9133002, "ScanEntrance main exit, compress failed."},
          {9132100, "ScanVerify commandVerify utility is null."},
          {9132101, "ScanVerify commandVerify input is null."},
          {9132102, "ScanVerify commandVerify input is invalid!"},
          {9132103, "ScanVerify commandVerify outPath is null."},
          {9132104, "ScanVerify commandVerify statFileSize is invalid! Must be integer in [0, 4294967295]"},
          {9132105, "ScanVerify commandVerify stat parameter is null."},
          {9131300, "statDuplicate create target file parent directory failed."},
          {9134301, "md5HashCode InputStream failed IOException "},
          {9134302, "md5HashCode MessageDigest failed NoSuchAlgorithmException "},
          {9131400, "ScanStatFileSize statFileSize create target file parent directory failed."},
          {9131200, "scanProcess create out file parent directory failed."},
          {9131201, "scanProcess file not found exception."},
          {9133202, "scanProcess exception "},
          {9133203, "scanProcess compress failed."},
          {9131204, "scanProcess delete dest file failed."},
          {9131205, "no file in this file path."},
          {9131206, "unpack hap failed IOException "},
          {9131207, "unpack hap FileOutputStream failed IOException "},
          {9131209, "getJsTemplate failed IOException "},
          {9131209, "writeFile failed IOException "},
          {9131500, "statSuffix create target file parent directory failed."},
          {9131501, "unpack hap failed IOException "},
          {9131502, "unpack hap FileOutputStream failed IOException "}
      };
  }

  int getScanErrorCode(ScanErrorEnum err) {
      return SCAN_ERROR_MAP[static_cast<int>(err)].code;
  }

  std::string getScanErrorMsg(ScanErrorEnum err) {
      return SCAN_ERROR_MAP[static_cast<int>(err)].msg;
  }

  std::string toString(ScanErrorEnum err) {
      return "code:" + std::to_string(getScanErrorCode(err)) +
             " error:" + getScanErrorMsg(err);
  }

  } // namespace error
  } // namespace packing_tool