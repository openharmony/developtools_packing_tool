/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 *
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
 * scanErrorEnum info
 *
 * @since 2023/12/4
 */

public enum ScanErrorEnum {
    SCAN_ENTRANCE_PARSER_ERROR(9132000, "ScanEntrance main exit, parser failed."),
    SCAN_ENTRANCE_VERIFY_ERROR(9132001, "ScanEntrance main exit, verify failed."),
    SCAN_ENTRANCE_COMPRESS_ERROR(9133002, "ScanEntrance main exit, compress failed."),
    SCAN_VERIFY_UTILITY_EMPTY_ERROR(9132100, "ScanVerify commandVerify utility is null."),
    SCAN_VERIFY_INPUT_EMPTY_ERROR(9132101, "ScanVerify commandVerify input is null."),
    SCAN_VERIFY_INPUT_INVALID_ERROR(9132102, "ScanVerify commandVerify input is invalid!"),
    SCAN_VERIFY_OUT_PATH_EMPTY_ERROR(9132103, "ScanVerify commandVerify outPath is null."),
    SCAN_VERIFY_STAT_FILE_SIZE_INVALID_ERROR(9132104,
            "ScanVerify commandVerify statFileSize is invalid! Must be integer in [0, 4294967295]"),
    SCAN_VERIFY_STAT_PARAMETER_EMPTY_ERROR(9132105, "ScanVerify commandVerify stat parameter is null. " +
            "At least one of the parameters: --stat-duplicate true | --stat-suffix true | --stat-file-size 0"),
    STAT_DUPLICATE_CREATE_FILE_ERROR(9131300, "statDuplicate create target file parent directory failed."),
    STAT_DUPLICATE_INPUT_STREAM_ERROR(9134301, "md5HashCode InputStream failed IOException "),
    STAT_DUPLICATE_MESSAGE_DIGEST_ERROR(9134302,
            "md5HashCode MessageDigest failed NoSuchAlgorithmException "),
    SCAN_STAT_FILE_SIZE_CREATE_FILE_PARENT_ERROR(9131400,
            "ScanStatFileSize statFileSize create target file parent directory failed."),
    SCAN_MKDIRS_ERROR(9131200, "scanProcess create out file parent directory failed."),
    SCAN_NOT_FOUND_ERROR(9131201, "scanProcess file not found exception."),
    SCAN_REMIND_ERROR(9133202, "scanProcess exception "),
    SCAN_COMPRESS_ERROR(9133203, "scanProcess compress failed."),
    SCAN_DELETE_ERROR(9131204, "scanProcess delete dest file failed."),
    SCAN_NO_FILE_ERROR(9131205, "no file in this file path."),
    SCAN_UNPACK_ERROR(9131206, "unpack hap failed IOException "),
    SCAN_UNPACK_STREAM_ERROR(9131207, "unpack hap FileOutputStream failed IOException "),
    SCAN_GET_JS_TEMPLATE_ERROR(9131209, "getJsTemplate failed IOException "),
    SCAN_WRITEFILE_ERROR(9131209, "writeFile failed IOException "),
    SUFFIX_MKDIRS_ERROR(9131500, "statSuffix create target file parent directory failed."),
    SUFFIX_UNPACK_ERROR(9131501, "unpack hap failed IOException "),
    SUFFIX_UNPACK_STREAM_ERROR(9131502, "unpack hap FileOutputStream failed IOException ");

    /**
     * error codes of scan tool.
     */
    public final int code;

    /**
     * error messages from scan tool.
     */
    public final String msg;

    ScanErrorEnum(int code, String msg) {
        this.code = code;
        this.msg = msg;
    }

    /**
     * scan tool error string
     *
     * @return error message
     */
    public String toString() {
        return "code:" + this.code + " error:" + this.msg;
    }
}
