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
 * scanEntrance info
 *
 * @since 2023/11/23
 */

public class ScanEntrance {
    private static final int EXIT_STATUS_NORMAL = 0;
    private static final int EXIT_STATUS_EXCEPTION = 1;
    private static final Log LOG = new Log(ScanEntrance.class.toString());


    /**
     * scan tool main function.
     *
     * @param args command line
     */

    public static void main(String[] args) {
        Utility utility = new Utility();

        if (!CommandParser.commandParser(utility, args)) {
            LOG.error("ScanEntrance::main exit, parser failed");
            ShowHelp.scanHelp();
            System.exit(EXIT_STATUS_EXCEPTION);
        }

        if (!ScanVerify.commandVerify(utility)) {
            LOG.error("ScanEntrance::main exit, verify failed");
            ShowHelp.scanHelp();
            System.exit(EXIT_STATUS_EXCEPTION);
        }

        Scan scan = new Scan();
        if (!scan.scanProcess(utility)) {
            LOG.error("ScanEntrance::main exit, compress failed");
            ShowHelp.scanHelp();
            System.exit(EXIT_STATUS_EXCEPTION);
        }

        System.exit(EXIT_STATUS_NORMAL);
    }
}