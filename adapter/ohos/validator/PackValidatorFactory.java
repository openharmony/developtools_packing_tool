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

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.function.Supplier;

/**
 * PackValidatorFactory.
 *
 * @since 2026-01-10
 */
public final class PackValidatorFactory {

    private static final Map<String, Supplier<AbstractPackValidator>> REGISTRY = new HashMap<>();

    static {
        REGISTRY.put("hap", HapValidator::new);
        REGISTRY.put("hsp", HspValidator::new);
    }

    private PackValidatorFactory() {
    }

    public static AbstractPackValidator getValidator(String packType) throws BundleException {
        if (packType == null || packType.isEmpty()) {
            throw new BundleException("Pack type must not be null or empty");
        }
        Supplier<AbstractPackValidator> supplier = REGISTRY.get(packType.toLowerCase(Locale.ROOT));
        if (supplier == null) {
            throw new BundleException("Unsupported pack type: " + packType);
        }
        return supplier.get();
    }
}

