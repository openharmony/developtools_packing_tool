/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import static ohos.Uncompress.getResourceFromHap;


/**
 * bundle uncompress.
 * step1: parse arguments
 * step2: verity arguments
 * step3: uncompress arguments
 *
 */
public class UncompressEntrance {
    /**
     * Parses and returns the hap list that supports the device type.
     */
    public static final String PARSE_MODE_HAPLIST = "hap-list";

    /**
     * Parses and returns the information about the hap.
     */
    public static final String PARSE_MODE_HAPINFO = "hap-info";

    /**
     * Parses and returns the informations about the hap list that supports the device type and the haps in the app.
     */
    public static final String PARSE_MODE_ALL = "all";

    /**
     * Device type of default.
     */
    public static final String DEVICE_TYPE_DEFAULT = "default";

    /**
     * Device type of phone.
     */
    public static final String DEVICE_TYPE_PHONE = "phone";

    /**
     * Device type of tablet.
     */
    public static final String DEVICE_TYPE_TABLET = "tablet";

    /**
     * Device type of tv.
     */
    public static final String DEVICE_TYPE_TV = "tv";

    /**
     * Device type of car.
     */
    public static final String DEVICE_TYPE_CAR = "car";

    /**
     * Device type of smartWatch.
     */
    public static final String DEVICE_TYPE_SMARTWATCH = "smartWatch";

    /**
     * Device type of fitnessWatch.
     */
    public static final String DEVICE_TYPE_FITNESSWATCH = "fitnessWatch";

    /**
     * Device type of fitnessBand.
     */
    public static final String DEVICE_TYPE_FITNESSBAND = "fitnessBand";

    private static final String APPQF_SUFFIX = ".appqf";
    private static final String APP_SUFFIX = ".app";
    private static final String HAP_SUFFIX = ".hap";
    private static final String HSP_SUFFIX = ".hsp";

    private static final int EXIT_STATUS_NORMAL = 0;
    private static final int EXIT_STATUS_EXCEPTION = 1;
    private static final Log LOG = new Log(UncompressEntrance.class.toString());

    /**
     * Indicates the parseMode for parseApp interface.
     */
    public enum ParseAppMode {
        ALL(0, "all"),
        HAP_LIST(1, "hap-list"),
        HAP_INFO(2, "hap-info");

        ParseAppMode(int index, String type) {
            this.index = index;
            this.type = type;
        }

        public static String getType(int index) {
            for (UncompressEntrance.ParseAppMode resType : UncompressEntrance.ParseAppMode.values()) {
                if (resType.getIndex() == index) {
                    return resType.type;
                }
            }
            return "";
        }

        public int getIndex() {
            return index;
        }
        public String getType() {
            return type;
        }
        private final int index;
        private final String type;
    }

    /**
     * Unpack the app.
     *
     * @param appPath Indicates the app path.
     * @param outPath Indicates the out path.
     * @param deviceType Indicates the device type supported by the haps.If this parameter is null, all the haps will
     *                   be unpacked.
     * @param unpackApk Indicates whether to decompress the apk file in the hap.The default value is {@code false},
     *                  not unpack the apk file.
     * @return Return the unpack result.
     */
    public static boolean unpack(String appPath, String outPath, String deviceType, boolean unpackApk) {
        if (appPath == null || appPath.isEmpty()) {
            LOG.error("UncompressEntrance::unpack appPath is invalid!");
            return false;
        }

        if (outPath == null || outPath.isEmpty()) {
            LOG.error("UncompressEntrance::unpack outPath is invalid!");
            return false;
        }

        Utility utility = new Utility();
        utility.setMode(Utility.MODE_APP);
        utility.setAppPath(appPath);
        utility.setDeviceType(deviceType == null ? "" : deviceType);
        utility.setOutPath(outPath);
        utility.setUnpackApk(String.valueOf(unpackApk));
        utility.setForceRewrite("true");

        if (!UncompressVerify.commandVerify(utility)) {
            LOG.error("CompressEntrance::unpack verity failed");
            return false;
        }

        if (!Uncompress.unpackageProcess(utility)) {
            LOG.error("UncompressEntrance::unpackageProcess failed");
            return false;
        }

        return true;
    }

    /**
     * Unpack the hap.
     *
     * @param hapPath Indicates the hap path.
     * @param outPath Indicates the out path.
     * @param unpackApk Indicates whether to decompress the apk file in the hap.The default value is {@code false},
     *                  not unpack the apk file.
     * @return Return the unpack result.
     */
    public static boolean unpackHap(String hapPath, String outPath, boolean unpackApk) {
        if (hapPath == null || hapPath.isEmpty()) {
            LOG.error("UncompressEntrance::unpackHap hapPath is invalid!");
            return false;
        }

        if (outPath == null || outPath.isEmpty()) {
            LOG.error("UncompressEntrance::unpackHap outPath is invalid!");
            return false;
        }

        Utility utility = new Utility();
        utility.setMode(Utility.MODE_HAP);
        utility.setHapPath(hapPath);
        utility.setDeviceType("");
        utility.setOutPath(outPath);
        utility.setUnpackApk(String.valueOf(unpackApk));
        utility.setForceRewrite("true");

        if (!UncompressVerify.commandVerify(utility)) {
            LOG.error("CompressEntrance::unpackHap verity failed");
            return false;
        }

        if (!Uncompress.unpackageProcess(utility)) {
            LOG.error("UncompressEntrance::unpackageProcess failed");
            return false;
        }

        return true;
    }


    /**
     * Parse the app.
     *
     * @param appPath Indicates the app path.
     * @param parseMode Indicates the parse mode, which can be {@link #PARSE_MODE_HAPLIST}, {@link #PARSE_MODE_HAPINFO},
     *                  {@link #PARSE_MODE_ALL}.
     * @param deviceType Indicates the device type supported by the haps, This parameter is required
     *                   when {@code #parseMode} is {@link #PARSE_MODE_HAPLIST}.
     * @param hapName Indicates the hap name, This parameter is required when {@code #parseMode}
     *                is {@link #PARSE_MODE_HAPINFO}.
     * @param outPath Indicates the out path to unpack the files.
     * @return Return the uncomperss result of parseApp
     * @deprecated
     */
    public static UncompressResult parseApp(String appPath, String parseMode, String deviceType, String hapName,
                                            String outPath) {
        UncompressResult compressResult = new UncompressResult();

        Utility utility = new Utility();
        utility.setAppPath(appPath);
        utility.setParseMode(parseMode);
        utility.setDeviceType(deviceType == null ? "" : deviceType);
        utility.setHapName(hapName == null ? "" : hapName);

        if (!UncompressVerify.isPathValid(utility.getAppPath(), true, APP_SUFFIX)) {
            LOG.error("UncompressEntrance::parseApp must input a app file!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }
        if (!UncompressVerify.isParseAppModeValid(utility.getParseMode(), utility.getHapName())) {
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }

        compressResult = Uncompress.uncompressAppByPath(utility);

        return compressResult;
    }

    /**
     * Parse the app.
     *
     * @param input Indicates the InputStream about the app package.
     * @param parseMode Indicates the parse mode, which can be {@link #PARSE_MODE_HAPLIST}, {@link #PARSE_MODE_HAPINFO},
     *                  {@link #PARSE_MODE_ALL}.
     * @param deviceType Indicates the device type supported by the haps, This parameter is required
     *                   when {@code #parseMode} is {@link #PARSE_MODE_HAPLIST}.
     * @param hapName Indicates the hap name, This parameter is required when {@code #parseMode}
     *                is {@link #PARSE_MODE_HAPINFO}.
     * @param outPath Indicates the out path to unzip temp files.
     * @return Return the uncomperss result of parseApp
     * @deprecated
     */
    public static UncompressResult parseApp(InputStream input, String parseMode, String deviceType, String hapName,
                                            String outPath) {
        UncompressResult compressResult = new UncompressResult();

        if (input == null) {
            LOG.error("UncompressEntrance::parseApp input is null!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp input is null");
            return compressResult;
        }

        Utility utility = new Utility();
        utility.setMode(Utility.MODE_APP);
        utility.setParseMode(parseMode);
        utility.setDeviceType(deviceType == null ? "" : deviceType);
        utility.setHapName(hapName == null ? "" : hapName);

        if (!UncompressVerify.isParseAppModeValid(utility.getParseMode(), utility.getHapName())) {
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }

        compressResult = Uncompress.uncompressAppByInput(utility, input);

        return compressResult;
    }

    /**
     * Parse the app.
     *
     * @param appPath Indicates the path about the app package.
     * @param parseAppMode Indicates the parse mode.
     * @param hapName Indicates the hap name, This parameter is required when {@code #parseMode}
     *                is {@link #PARSE_MODE_HAPINFO}.
     * @return Return the uncomperss result of parseApp
     */
    public static UncompressResult parseApp(String appPath, ParseAppMode parseAppMode, String hapName) {
        UncompressResult compressResult = new UncompressResult();
        Utility utility = new Utility();
        utility.setAppPath(appPath);
        utility.setParseMode(parseAppMode.getType());
        utility.setDeviceType("");
        utility.setHapName(hapName);
        if (!UncompressVerify.isPathValid(utility.getAppPath(), true, APP_SUFFIX)) {
            LOG.error("UncompressEntrance::parseApp must input a app file!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }
        if (!UncompressVerify.isParseAppModeValid(utility.getParseMode(), utility.getHapName())) {
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }
        compressResult = Uncompress.uncompressAppByPath(utility);

        return compressResult;
    }

    /**
     * Parse the app.
     *
     * @param input Indicates the input stream about the app package.
     * @param parseAppMode Indicates the parse mode.
     * @param hapName Indicates the hap name, This parameter is required when {@code #parseMode}
     *                is {@link #PARSE_MODE_HAPINFO}.
     * @return Return the uncomperss result of parseApp
     */
    public static UncompressResult parseApp(InputStream input, ParseAppMode parseAppMode, String hapName) {
        UncompressResult compressResult = new UncompressResult();
        if (input == null) {
            LOG.error("UncompressEntrance::parseApp input is null!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp input is null");
            return compressResult;
        }
        Utility utility = new Utility();
        utility.setParseMode(parseAppMode.getType());
        utility.setDeviceType("");
        utility.setHapName(hapName);
        if (!UncompressVerify.isParseAppModeValid(utility.getParseMode(), utility.getHapName())) {
            compressResult.setResult(false);
            compressResult.setMessage("ParseApp verify failed");
            return compressResult;
        }
        compressResult = Uncompress.uncompressAppByInput(utility, input);
        return compressResult;
    }

    /**
     * Parse the hap.
     *
     * @param hapPath Indicates the hap path.
     * @return Return the uncomperss result of parseHap
     */
    public static UncompressResult parseHap(String hapPath) {
        UncompressResult compressResult = new UncompressResult();

        Utility utility = new Utility();
        utility.setHapPath(hapPath);
        if (!UncompressVerify.isPathValid(utility.getHapPath(), true, HAP_SUFFIX) &&
                !UncompressVerify.isPathValid(utility.getHapPath(), true, HSP_SUFFIX)) {
            LOG.error("UncompressEntrance::parseHap must input a hap file!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseHap hapPath is invalid");
        }

        compressResult = Uncompress.uncompressHap(utility);

        return compressResult;
    }

    /**
     * Parse the hap.
     *
     * @param input Indicates the InputStream about the app package.
     * @return Return the uncomperss result of parseHap
     */
    public static UncompressResult parseHap(InputStream input) {
        UncompressResult compressResult = new UncompressResult();

        if (input == null) {
            LOG.error("UncompressEntrance::parseHap input is null!");
            compressResult.setResult(false);
            compressResult.setMessage("ParseHap input is null");
            return compressResult;
        }

        Utility utility = new Utility();
        compressResult = Uncompress.uncompressHapByInput(utility, input);
        return compressResult;
    }

    /**
     * Parse the hap resource.
     *
     * @param hapPath Indicates the hap path.
     * @return Return the List<ResourceIndexResult> result of parseHap
     */
    public static List<ResourceIndexResult> parseResource(String hapPath) throws BundleException, IOException {
        return getResourceFromHap(hapPath);
    }

    /**
     * Parse the appqf file.
     *
     * @param appqfPath Indicates the hap path.
     * @return Return the List<ResourceIndexResult> result of parseHap
     */
    public static APPQFResult parseAPPQF(String appqfPath) {
        APPQFResult result = new APPQFResult();
        if (!appqfPath.endsWith(APPQF_SUFFIX)) {
            LOG.error("UncompressEntrance::parseAPPQF Error, input wrong type APPQF file!");
            result.setSuccess(false);
        }
        try {
            result.setHqfInfoList(Uncompress.parseAPPQFFile(appqfPath));
            result.setSuccess(true);
        } catch (BundleException e) {
            LOG.error("UncompressEntrance::parseAPPQF failed, read patch.json in APPQF file failed!");
            result.setSuccess(false);
        }
        return result;
    }

    /**
     * uncompress tool main function.
     *
     * @param args command line
     */
    public static void main(String[] args) {
        parseHap("D:\\Project\\MyApplication0926\\entry\\build\\default\\outputs\\default\\entry-default-unsigned.hap");
//        Utility utility = new Utility();
//
//        if (!CommandParser.commandParser(utility, args)) {
//            LOG.error("UncompressEntrance::main exit, parser failed");
//            ShowHelp.uncompressHelp();
//            System.exit(EXIT_STATUS_EXCEPTION);
//        }
//
//        if (!UncompressVerify.commandVerify(utility)) {
//            LOG.error("UncompressEntrance::main exit, verify failed");
//            ShowHelp.uncompressHelp();
//            System.exit(EXIT_STATUS_EXCEPTION);
//        }
//
//        if (!Uncompress.unpackageProcess(utility)) {
//            LOG.error("UncompressEntrance::main exit, uncompress failed");
//            ShowHelp.uncompressHelp();
//            System.exit(EXIT_STATUS_EXCEPTION);
//        }
//
//        System.exit(EXIT_STATUS_NORMAL);
    }
}