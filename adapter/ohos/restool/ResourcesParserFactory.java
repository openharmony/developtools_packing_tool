/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

package ohos.restool;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

/**
 * ResourcesParserFactory
 *
 * @since 2025-06-06
 */
public class ResourcesParserFactory {
    private static final int VERSION_BYTE_LENGTH = 128;
    private static final String RESOURCE_PROTOCOL_VERSION_TAG = "RestoolV2";

    /**
     * createParser.
     *
     * @param data resource index data
     * @return ResourcesParser
     */
    public static ResourcesParser createParser(byte[] data) {
        if (isV2Protocol(data)) {
            return new ResourcesParserV2(data);
        }
        return new ResourcesParserV1();
    }

    /**
     * Is V2 protocol.
     *
     * @param data resource index data
     * @return is V2
     */
    private static boolean isV2Protocol(byte[] data) {
        ByteBuffer byteBuf = ByteBuffer.wrap(data);
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        byteBuf.get(version);
        String versionStr = new String(version, StandardCharsets.UTF_8);
        return versionStr.contains(RESOURCE_PROTOCOL_VERSION_TAG);
    }
}
