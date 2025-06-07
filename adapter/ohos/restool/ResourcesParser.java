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

import ohos.BundleException;
import ohos.ResourceIndexResult;

import java.util.HashMap;
import java.util.List;

/**
 * Resources Parser Interface.
 *
 * @since 2025-06-06
 */
public interface ResourcesParser {
    /**
     * Get resource value by resource id.
     *
     * @param resourceId resource id
     * @param data       resource index data
     * @return the resource value
     * @throws BundleException IOException.
     */
    String getResourceById(int resourceId, byte[] data) throws BundleException;

    /**
     * Get base resource value by resource id.
     *
     * @param resourceId resource id
     * @param data resource index data
     * @return the resource value
     * @throws BundleException IOException.
     */
    String getBaseResourceById(int resourceId, byte[] data) throws BundleException;

    /**
     * Read all config item.
     *
     * @param data config byte buffer
     * @return the item info.
     */
    List<ResourceIndexResult> getAllDataItem(byte[] data);

    /**
     * Read resource map by id.
     *
     * @param resId The resource ID to query
     * @param data config byte buffer
     * @return the resource map of id.
     */
    HashMap<String, String> getResourceMapById(int resId, byte[] data);

    /**
     * Gets the resource string value by ID
     *
     * @param resId The resource ID to query
     * @param data The resource data buffer
     * @return The string value of the resource
     */
    String getResourceStringById(int resId, byte[] data);
}
