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
import java.util.Map;

/**
 * Resources Parser Interface.
 *
 * @since 2025-06-06
 */
public interface ResourcesParser {
    int RESOURCE_REFERENCE_MAX_DEPTH = 20;
    int RESOURCE_DEFAULT_ID = -1;
    String RESOURCE_REFERENCE_PREFIX = "$";
    String RESOURCE_REFERENCE_SEPARATOR = ":";
    String BASE_CONFIG = "base";

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

    /**
     * Get final resource value by resource id.
     *
     * @param resId The resource ID to query
     * @param data The resource data buffer
     * @return the final resource value
     */
    default String getFinalResourceStringById(int resId, byte[] data) {
        return getFinalResourceValue(getResourceStringById(resId, data), data);
    }

    /**
     * Get final resource map by resource id.
     *
     * @param resId The resource ID to query
     * @param data The resource data buffer
     * @return the final resource map of id
     */
    default HashMap<String, String> getFinalResourceMapById(int resId, byte[] data) {
        HashMap<String, String> resourceMap = getResourceMapById(resId, data);
        for (Map.Entry<String, String> entry : resourceMap.entrySet()) {
            entry.setValue(getFinalResourceValue(entry.getValue(), data, entry.getKey()));
        }
        return resourceMap;
    }

    /**
     * Get final resource value by base resource id.
     *
     * @param resourceId resource id
     * @param data resource index data
     * @return the final resource value
     * @throws BundleException IOException.
     */
    default String getFinalBaseResourceById(int resourceId, byte[] data) throws BundleException {
        return getFinalResourceValue(getBaseResourceById(resourceId, data), data, BASE_CONFIG);
    }

    /**
     * Get final resource value by resource id.
     *
     * @param resourceId resource id
     * @param data resource index data
     * @return the final resource value
     * @throws BundleException IOException.
     */
    default String getFinalResourceById(int resourceId, byte[] data) throws BundleException {
        return getFinalResourceValue(getResourceById(resourceId, data), data);
    }

    /**
     * Recursively parse resource references like "$string:16777216".
     *
     * @param value resource value
     * @param data resource index data
     * @return the final resource value
     */
    default String getFinalResourceValue(String value, byte[] data) {
        return getFinalResourceValue(value, data, null);
    }

    /**
     * Recursively parse resource references like "$string:16777216".
     *
     * @param value resource value
     * @param data resource index data
     * @param configClass resource config class
     * @return the final resource value
     */
    default String getFinalResourceValue(String value, byte[] data, String configClass) {
        String result = value;
        for (int depth = 0; depth < RESOURCE_REFERENCE_MAX_DEPTH; depth++) {
            int resourceId = parseResourceReferenceId(result);
            if (resourceId == RESOURCE_DEFAULT_ID) {
                break;
            }
            String nestedValue = getResourceStringById(resourceId, data, configClass);
            if (nestedValue == null || nestedValue.isEmpty()) {
                break;
            }
            result = nestedValue;
        }
        return result;
    }

    /**
     * Gets the resource string value by ID and config class.
     *
     * @param resId The resource ID to query
     * @param data The resource data buffer
     * @param configClass resource config class
     * @return The string value of the resource
     */
    default String getResourceStringById(int resId, byte[] data, String configClass) {
        if (configClass == null || configClass.isEmpty()) {
            return getResourceStringById(resId, data);
        }
        List<ResourceIndexResult> resources = getAllDataItem(data);
        for (ResourceIndexResult indexResult : resources) {
            if (indexResult.id == resId && configClass.equals(indexResult.configClass)) {
                return indexResult.value;
            }
        }
        try {
            return getBaseResourceById(resId, data);
        } catch (BundleException exception) {
            return "";
        }
    }

    /**
     * Parse resource id from resource reference.
     *
     * @param value resource value
     * @return resource id
     */
    default int parseResourceReferenceId(String value) {
        if (value == null || !value.startsWith(RESOURCE_REFERENCE_PREFIX)) {
            return RESOURCE_DEFAULT_ID;
        }
        int separatorIndex = value.indexOf(RESOURCE_REFERENCE_SEPARATOR);
        if (separatorIndex < 0 || separatorIndex == value.length() - 1) {
            return RESOURCE_DEFAULT_ID;
        }
        String resourceId = value.substring(separatorIndex + 1);
        for (int i = 0; i < resourceId.length(); i++) {
            if (!Character.isDigit(resourceId.charAt(i))) {
                return RESOURCE_DEFAULT_ID;
            }
        }
        try {
            return Integer.parseUnsignedInt(resourceId);
        } catch (NumberFormatException exception) {
            return RESOURCE_DEFAULT_ID;
        }
    }
}
