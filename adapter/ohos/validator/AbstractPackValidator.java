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
import ohos.Utility;

/**
 * Abstract class for checking package configurations during packaging.
 *
 * @since 2026-01-10
 */
public abstract class AbstractPackValidator {
    /**
     * Validates the packaging inputs using a fixed validation workflow.
     *
     * <p>This is the <b>only public entry point</b> for pack validation.
     * The validation process is fixed and must not be overridden by subclasses.
     *
     * <p>Validation flow:
     * <ol>
     *   <li>Perform common validation checks</li>
     *   <li>Perform mode-specific validation via {@link #isVerifyValid(Utility)}</li>
     * </ol>
     *
     * <p>Subclasses should implement {@link #isVerifyValid(Utility)} to provide
     * mode-specific validation logic, instead of overriding this method.
     *
     * @param utility packaging context and parameters
     * @return {@code true} if validation passes; {@code false} otherwise
     */
    public final boolean validate(Utility utility) throws BundleException {
        // 公共校验待添加
        return isVerifyValid(utility);
    }

    /**
     * Verifying the packaging parameter value.
     *
     * @param utility packing param
     * @return The result of verify
     * @throws BundleException packing exception.
     */
    protected abstract boolean isVerifyValid(Utility utility) throws BundleException;
}
