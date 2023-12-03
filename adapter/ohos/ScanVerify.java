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

import java.math.BigDecimal;
import java.util.regex.Pattern;

/**
 * scanVerify info
 *
 * @since 2023/11/23
 */

public class ScanVerify {

    static final String HAP = ".hap";
    static final String HSP = ".hsp";
    static final String APP = ".app";
    private static final String EMPTY_STRING = "";
    private static final Pattern PATTERN = Pattern.compile("[0-9]*");
    private static final Long MAX_VALUE = 4294967295L;

    private static final Log LOG = new Log(ScanVerify.class.toString());

    /**
     * if args valid.
     *
     * @param utility common data
     * @return commandVerify if command valid.
     */
    public static boolean commandVerify(Utility utility) {
        if (utility == null) {
            LOG.error("ScanVerify::commandVerify utility is null.");
            return false;
        }

        if (utility.getInput().isEmpty()) {
            LOG.error("ScanVerify::commandVerify input is null.");
            return false;
        }

        if (!(utility.getInput().endsWith(HAP) || utility.getInput().endsWith(HSP)
                || utility.getInput().endsWith(APP))) {
            LOG.error("ScanVerify::commandVerify input is invalid!");
            return false;
        }

        if (utility.getOutPath().isEmpty()) {
            LOG.error("ScanVerify::commandVerify outPath is null.");
            return false;
        }

        if (!utility.getStatFileSize().isEmpty()) {
            if (!PATTERN.matcher(utility.getStatFileSize()).matches()) {
                LOG.error("ScanVerify::commandVerify statFileSize is invalid!"
                        + " Must be integer in [0, 4294967295]");
                return false;
            }
            if (new BigDecimal(utility.getStatFileSize()).compareTo(BigDecimal.ZERO) < 0
                    || new BigDecimal(utility.getStatFileSize()).compareTo(new BigDecimal(MAX_VALUE)) > 0) {
                LOG.error("ScanVerify::commandVerify statFileSize is invalid!"
                        + " Must be integer in [0, 4294967295]");
                return false;
            }
        }

        if (!utility.getStatSuffix() && !utility.getStatDuplicate()
                && EMPTY_STRING.equals(utility.getStatFileSize())) {
            LOG.error("ScanVerify::commandVerify stat parameter is null.");
        }

        return true;
    }
}
