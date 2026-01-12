/*
 * Copyright (c) 2026-2026 Huawei Device Co., Ltd.
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

package ohos.validator;

import ohos.BundleException;
import ohos.Constants;
import ohos.Log;
import ohos.Utility;
import ohos.PackingToolErrMsg;

import java.io.File;

/**
 * Class used to check the packaging parameters and verify the package attributes
 * when packaging an HAP.
 *
 * @since 2026-01-10
 */
public class HapValidator extends AbstractPackValidator {
    private static final Log LOG = new Log(HapValidator.class.toString());

    /**
     * Verifying the packaging parameters when packaging a HAP.
     *
     * @param utility packing parameters
     * @return The result of verify
     * @throws BundleException packing exception.
     */
    @Override
    public boolean isVerifyValid(Utility utility) throws BundleException {
        if (!isValidPkgSdkInfoFile(utility)) {
            String errMsg = "--pkg-sdk-info-path value is invalid.";
            LOG.error(PackingToolErrMsg.HAP_MODE_ARGS_INVALID.toString(errMsg));
            return false;
        }
        return true;
    }

    /**
     * Indicates whether the "--pkg-sdk-info-path" parameter is valid.
     *
     * @param utility - packing parameters
     * @return true if "--pkg-sdk-info-path" param exists and the file name is pkgSdkInfo.json,
     *              or the "--pkg-sdk-info-path" param is empty,
     *              or has no "--pkg-sdk-info-path" param
     *         false other situations
     */
    private static boolean isValidPkgSdkInfoFile(Utility utility) {
        if (!utility.getPkgSdkInfoJsonPath().isEmpty()) {
            File filePkgSdkInfoJson = new File(utility.getPkgSdkInfoJsonPath());
            return filePkgSdkInfoJson.isFile() && Constants.FILE_PKG_SDK_INFO_JSON.equals(filePkgSdkInfoJson.getName());
        }
        return true;
    }
}
