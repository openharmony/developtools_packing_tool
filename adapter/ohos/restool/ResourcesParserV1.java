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

package ohos.restool;

import ohos.BundleException;
import ohos.Log;
import ohos.ResourceIndexResult;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Optional;

/**
 * Resources Parser.
 *
 * @since 2021-03-11
 */
public class ResourcesParserV1 implements ResourcesParser {
    /**
     * Parses resources default id.
     */
    public static final int RESOURCE_DEFAULT_ID = -1;

    private static final Log LOG = new Log(ResourcesParserV1.class.toString());
    private static final int VERSION_BYTE_LENGTH = 128;
    private static final int TAG_BYTE_LENGTH = 4;
    private static final String LEFT_BRACKET = "<";
    private static final String RIGHT_BRACKET = ">";
    private static final String VERTICAL = "vertical";
    private static final String HORIZONTAL = "horizontal";
    private static final String DARK = "dark";
    private static final String LIGHT = "light";
    private static final String MCC = "mcc";
    private static final String MNC = "mnc";
    private static final String CONFIG_CONJUNCTION = "-";
    private static final String MCC_CONJUNCTION = "_";
    private static final String BASE = "base";
    private static final int CHAR_LENGTH = 1;
    private static final String EMPTY_STRING = "";

    private enum ResType {
        VALUES(0, "Values"),
        ANIMATOR(1, "Animator"),
        DRAWABLE(2, "Drawable"),
        LAYOUT(3, "Layout"),
        MENU(4, "Menu"),
        MIPMAP(5, "Mipmap"),
        RAW(6, "Raw"),
        XML(7, "Xml"),
        INTEGER(8, "Integer"),
        STRING(9, "String"),
        STR_ARRAY(10, "StrArray"),
        INT_ARRAY(11, "IntArray"),
        BOOLEAN(12, "Boolean"),
        DIMEN(13, "Dimen"),
        COLOR(14, "Color"),
        ID(15, "Id"),
        THEME(16, "Theme"),
        PLURALS(17, "Plurals"),
        FLOAT(18, "Float"),
        MEDIA(19, "Media"),
        PROF(20, "Prof"),
        SVG(21, "Svg"),
        PATTERN(22, "Pattern");

        private final int index;
        private final String type;

        private ResType(int index, String type) {
            this.index = index;
            this.type = type;
        }

        public static String getType(int index) {
            for (ResType resType : ResType.values()) {
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
    }

    private enum DeviceType {
        PHONE(0, "phone"),
        TABLET(1, "tablet"),
        CAR(2, "car"),
        PC(3, "pc"),
        TV(4, "tv"),
        SPEAKER(5, "speaker"),
        WEARABLE(6, "wearable"),
        GLASSES(7, "glasses"),
        HEADSET(8, "headset");

        private final int index;
        private final String type;
        private DeviceType(int index, String type) {
            this.index = index;
            this.type = type;
        }

        public static String getType(int index) {
            for (DeviceType deviceType : DeviceType.values()) {
                if (deviceType.getIndex() == index) {
                    return deviceType.type;
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
    }

    private enum Resolution {
        NODPI(-2, "nodpi"),
        ANYDPI(-1, "anydpi"),
        SDPI(120, "sdpi"),
        MDPI(160, "mdpi"),
        TVDPI(213, "tvdpi"),
        LDPI(240, "ldpi"),
        XLDPI(320, "xldpi"),
        XXLDPI(480, "xxldpi"),
        XXXLDPI(640, "xxxldpi");

        private final int index;
        private final String type;
        private Resolution(int index, String type) {
            this.index = index;
            this.type = type;
        }
        public static String getType(int index) {
            for (Resolution resolution : Resolution.values()) {
                if (resolution.getIndex() == index) {
                    return resolution.type;
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
    }

    private enum ConfigType {
        LANGUAGE,
        REGION,
        RESOLUTION,
        DIRECTION,
        DEVICE_TYPE,
        SCRIPT,
        LIGHT_MODE,
        MCC,
        MNC
    }

    /**
     * Key Param.
     */
    static class KeyParam {
        int keyType;
        int value;
    }

    /**
     * Config Index.
     */
    static class ConfigIndex {
        String tag;
        int offset;
        int keyCount;
        KeyParam[] params;
    }

    /**
     * Data Item.
     */
    static class DataItem {
        int size;
        int type;
        int id;
        String value;
        String name;
    }

    /**
     * Get resource value by resource id.
     *
     * @param resourceId resource id
     * @param data resource index data
     * @return the resourceId value
     * @throws BundleException IOException.
     */
    @Override
    public String getResourceById(int resourceId, byte[] data) throws BundleException {
        String resourceIdValue = "";
        if (data == null || data.length <= 0 || resourceId == RESOURCE_DEFAULT_ID) {
            LOG.error("ResourcesParser::getIconPath data byte or ResourceId is null");
            return resourceIdValue;
        }

        List<String> result = getResource(resourceId, data);
        if (result != null && result.size() > 0 && result.get(0) != null && !EMPTY_STRING.equals(result.get(0))) {
            resourceIdValue = result.get(0).substring(0, result.get(0).length() - 1);
        }
        return resourceIdValue;
    }

    /**
     * Get base resource value by resource id.
     *
     * @param resourceId resource id
     * @param data resource index data
     * @return the resource value
     * @throws BundleException IOException.
     */
    @Override
    public String getBaseResourceById(int resourceId, byte[] data) throws BundleException {
        String resourceIdValue = "";
        if (data == null || data.length <= 0 || resourceId == RESOURCE_DEFAULT_ID) {
            LOG.error("ResourcesParser::getBaseResourceById data byte or ResourceId is null");
            return resourceIdValue;
        }
        resourceIdValue = getBaseResource(resourceId, data);
        if (resourceIdValue != null && !EMPTY_STRING.equals(resourceIdValue)) {
            resourceIdValue = resourceIdValue.substring(0, resourceIdValue.length() - 1);
        }
        return resourceIdValue;
    }

    /**
     * Get base resource.
     *
     * @param resId resource id
     * @param data resource index data array
     * @return the resource value
     * @throws BundleException IOException.
     */
    static String getBaseResource(int resId, byte[] data) {
        ByteBuffer byteBuf = ByteBuffer.wrap(data);
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        byteBuf.get(version);
        byteBuf.getInt();
        int configCount = byteBuf.getInt();
        Optional<ConfigIndex> optionalConfigIndex = loadBaseConfig(byteBuf, configCount);
        if (!optionalConfigIndex.isPresent()) {
            LOG.error("ResourcesParser::getBaseResource configIndex is null");
            return "";
        }
        return readBaseItem(resId, optionalConfigIndex.get(), byteBuf);
    }

    /**
     * Load index config.
     *
     * @param bufBuf config byte buffer
     * @param count config count
     * @return the base config index
     * @throws BundleException IOException.
     */
    static Optional<ConfigIndex> loadBaseConfig(ByteBuffer bufBuf, int count) {
        for (int i = 0; i < count; i++) {
            ConfigIndex cfg = new ConfigIndex();
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            bufBuf.get(tag);
            cfg.tag = new String(tag, StandardCharsets.UTF_8);
            cfg.offset = bufBuf.getInt();
            cfg.keyCount = bufBuf.getInt();
            cfg.params = new KeyParam[cfg.keyCount];
            for (int j = 0; j < cfg.keyCount; j++) {
                cfg.params[j] = new KeyParam();
                cfg.params[j].keyType = bufBuf.getInt();
                cfg.params[j].value = bufBuf.getInt();
            }
            if (cfg.keyCount == 0) {
                return Optional.of(cfg);
            }
        }
        return Optional.empty();
    }

    /**
     * Read base config item.
     *
     * @param resId resource id
     * @param configIndex the config index
     * @param buf config byte buffer
     * @return the base item
     * @throws BundleException IOException.
     */
    static String readBaseItem(int resId, ConfigIndex configIndex, ByteBuffer buf) {
        buf.rewind();
        buf.position(configIndex.offset);
        byte[] tag = new byte[TAG_BYTE_LENGTH];
        buf.get(tag);
        int count = buf.getInt();
        for (int i = 0; i < count; i++) {
            int id = buf.getInt();
            int offset = buf.getInt();
            if (id == resId) {
                buf.rewind();
                buf.position(offset);
                DataItem item = readItem(buf);
                return item.value;
            }
        }
        return "";
    }

    /**
     * Get Icon resource.
     *
     * @param resId resource id
     * @param data resource index data array
     * @return the result
     * @throws BundleException IOException.
     */
    static List<String> getResource(int resId, byte[] data) {
        ByteBuffer byteBuf = ByteBuffer.wrap(data);
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        byteBuf.get(version);
        byteBuf.getInt();
        int configCount = byteBuf.getInt();
        List<ConfigIndex> cfg = loadConfig(byteBuf, configCount);
        return readAllItem(resId, cfg, byteBuf);
    }

    /**
     * Load index config.
     *
     * @param bufBuf config byte buffer
     * @param count config count
     * @return the config list
     * @throws BundleException IOException.
     */
    static List<ConfigIndex> loadConfig(ByteBuffer bufBuf, int count) {
        List<ConfigIndex> configList = new ArrayList<>(count);
        for (int i = 0; i < count; i++) {
            ConfigIndex cfg = new ConfigIndex();
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            bufBuf.get(tag);
            cfg.tag = new String(tag, StandardCharsets.UTF_8);
            cfg.offset = bufBuf.getInt();
            cfg.keyCount = bufBuf.getInt();
            cfg.params = new KeyParam[cfg.keyCount];
            for (int j = 0; j < cfg.keyCount; j++) {
                cfg.params[j] = new KeyParam();
                cfg.params[j].keyType = bufBuf.getInt();
                cfg.params[j].value = bufBuf.getInt();
            }

            configList.add(cfg);
        }
        return configList;
    }

    /**
     * Read all config item.
     *
     * @param resId resource id
     * @param configs the config list
     * @param buf config byte buffer
     * @return the item list
     * @throws BundleException IOException.
     */
    static List<String> readAllItem(int resId, List<ConfigIndex> configs, ByteBuffer buf) {
        List<String> result = new ArrayList<>();
        for (ConfigIndex index : configs) {
            buf.rewind();
            buf.position(index.offset);
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            buf.get(tag);
            int count = buf.getInt();
            for (int i = 0; i < count; i++) {
                int id = buf.getInt();
                int offset = buf.getInt();
                if (id == resId) {
                    buf.rewind();
                    buf.position(offset);
                    DataItem item = readItem(buf);
                    result.add(item.value);
                    break;
                }
            }
        }
        return result;
    }

    /**
     * Read the config item.
     *
     * @param buf config byte buffer
     * @return the item info
     */
    static DataItem readItem(ByteBuffer buf) {
        DataItem item = new DataItem();
        item.size = buf.getInt();
        item.type = buf.getInt();
        item.id = buf.getInt();
        int len = buf.getShort() & 0xFFFF;
        byte[] value = new byte[len];
        buf.get(value);
        item.value = new String(value, StandardCharsets.UTF_8);
        len = buf.getShort() & 0xFFFF;
        byte[] name = new byte[len];
        buf.get(name);
        item.name = new String(name, StandardCharsets.UTF_8);
        return item;
    }

    /**
     * Read all config item.
     *
     * @param data config byte buffer
     * @return the item info.
     */
    @Override
    public List<ResourceIndexResult> getAllDataItem(byte[] data) {
        ByteBuffer byteBuf = ByteBuffer.wrap(data);
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        byteBuf.get(version);
        byteBuf.getInt();
        int configCount = byteBuf.getInt();
        List<ConfigIndex> cfg = loadConfig(byteBuf, configCount);
        return readDataAllItem(cfg, byteBuf);
    }

    /**
     * Read resource map by id.
     *
     * @param resId resource id
     * @param data config byte buffer
     * @return the resource map of id.
     */
    @Override
    public HashMap<String, String> getResourceMapById(int resId, byte[] data) {
        List<ResourceIndexResult> resources = getAllDataItem(data);
        HashMap<String, String> resourceMap = new HashMap<>();
        for (ResourceIndexResult indexResult : resources) {
            if (indexResult.id == resId) {
                resourceMap.put(indexResult.configClass, indexResult.value);
            }
        }
        return resourceMap;
    }

    /**
     * Read resource by id.
     *
     * @param resId resource id
     * @param data config byte buffer
     * @return the resource.
     */
    @Override
    public String getResourceStringById(int resId, byte[] data) {
        List<ResourceIndexResult> resources = getAllDataItem(data);
        for (ResourceIndexResult indexResult : resources) {
            if (indexResult.id == resId) {
                return indexResult.value;
            }
        }
        return "";
    }

    /**
     * Read all config item.
     *
     * @param configs the config list
     * @param buf config byte buffer
     * @return the item list
     */
    static List<ResourceIndexResult> readDataAllItem(List<ConfigIndex> configs, ByteBuffer buf) {
        List<ResourceIndexResult> resourceIndexResults = new ArrayList<>();
        for (ConfigIndex index : configs) {
            String configClass = convertConfigIndexToString(index);
            buf.rewind();
            buf.position(index.offset);
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            buf.get(tag);
            int count = buf.getInt();
            int position = buf.position();
            for (int i = 0; i < count; ++i) {
                buf.position(position);
                buf.getInt();
                int offset = buf.getInt();
                position = buf.position();
                buf.rewind();
                buf.position(offset);
                DataItem item = readItem(buf);
                resourceIndexResults.add(parseDataItems(item, configClass));
            }
        }
        return resourceIndexResults;
    }

    /**
     * convert DataItems to ResourceIndexResult.
     *
     *  @param item Indicates the DataItem.
     *  @param configClass config info.
     *  @return the final ResourceIndexResult
     */
    static ResourceIndexResult parseDataItems(DataItem item, String configClass) {
        ResourceIndexResult resourceIndexResult = new ResourceIndexResult();
        resourceIndexResult.configClass = configClass;
        if (item != null) {
            resourceIndexResult.type = ResType.getType(item.type);
            resourceIndexResult.id = item.id;
            resourceIndexResult.name = item.name.substring(0, item.name.length() - 1);
            if (requireBytesConversion(item.type)) {
                byte[] bytes =
                        item.value.substring(0, item.value.length() - 1).getBytes(StandardCharsets.UTF_8);
                resourceIndexResult.value = convertBytesToString(bytes);
            } else {
                resourceIndexResult.value = item.value.substring(0, item.value.length() - 1);
            }
        }
        return resourceIndexResult;
    }

    private static boolean requireBytesConversion(int resType) {
        return resType == ResType.STR_ARRAY.getIndex()
                || resType == ResType.INT_ARRAY.getIndex()
                || resType == ResType.THEME.getIndex()
                || resType == ResType.PLURALS.getIndex()
                || resType == ResType.PATTERN.getIndex();
    }

    /**
     * convert bytes to string.
     *
     * @param data Indicates the bytes of data.
     * @return the final string
     */
    static String convertBytesToString(byte[] data) {
        StringBuilder result = new StringBuilder();
        ByteBuffer byteBuf = ByteBuffer.wrap(data);
        byteBuf.order(ByteOrder.LITTLE_ENDIAN);
        while (byteBuf.hasRemaining()) {
            result.append(LEFT_BRACKET);
            int len = byteBuf.getShort();
            if (len <= 0) {
                LOG.info("len less than 0, dismiss");
                result.append(RIGHT_BRACKET);
                break;
            }
            byte[] value = new byte[len + CHAR_LENGTH];
            byteBuf.get(value);
            String item = new String(value, StandardCharsets.UTF_8);
            result.append(item, 0, item.length() - 1);
            result.append(RIGHT_BRACKET);
        }
        return result.toString();
    }

    /**
     * convert config to string.
     *
     * @param configIndex Indicates the configIndex.
     *  @return the final string
     */
    static String convertConfigIndexToString(ConfigIndex configIndex) {
        StringBuilder configClass = new StringBuilder();
        int lastKeyType = -1;
        for (int i = 0; i < configIndex.keyCount; ++i) {
            KeyParam param = configIndex.params[i];
            if (param.keyType == ConfigType.LANGUAGE.ordinal()
                    || param.keyType == ConfigType.REGION.ordinal()
                    || param.keyType == ConfigType.SCRIPT.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(parseAscii(param.value));
                } else {
                    if (lastKeyType == ConfigType.LANGUAGE.ordinal() ||
                            lastKeyType == ConfigType.REGION.ordinal() || lastKeyType == ConfigType.SCRIPT.ordinal()) {
                        configClass.append(MCC_CONJUNCTION).append(parseAscii(param.value));
                    } else {
                        configClass.append(CONFIG_CONJUNCTION).append(parseAscii(param.value));
                    }
                }
                lastKeyType = param.keyType;
            } else if (param.keyType == ConfigType.RESOLUTION.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(Resolution.getType(param.value));
                } else {
                    configClass.append(CONFIG_CONJUNCTION).append(Resolution.getType(param.value));
                }
            } else if (param.keyType == ConfigType.DIRECTION.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(param.value == 0 ? VERTICAL : HORIZONTAL);
                } else {
                    configClass.append(CONFIG_CONJUNCTION).append(param.value == 0 ? VERTICAL : HORIZONTAL);
                }
            } else if (param.keyType == ConfigType.DEVICE_TYPE.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(DeviceType.getType(param.value));
                } else {
                    configClass.append(CONFIG_CONJUNCTION).append(DeviceType.getType(param.value));
                }
            } else if (param.keyType == ConfigType.LIGHT_MODE.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(param.value == 0 ? DARK : LIGHT);
                } else {
                    configClass.append(CONFIG_CONJUNCTION).append(param.value == 0 ? DARK : LIGHT);
                }
            } else if (param.keyType == ConfigType.MCC.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(MCC).append(String.valueOf(param.value));
                } else {
                    configClass.append(MCC_CONJUNCTION).append(MCC).append(String.valueOf(param.value));
                }
            } else if (param.keyType == ConfigType.MNC.ordinal()) {
                if (EMPTY_STRING.equals(configClass.toString())) {
                    configClass.append(MNC).append(fillUpZero(String.valueOf(param.value), 3));
                } else {
                    configClass.append(MCC_CONJUNCTION).append(MNC).append(fillUpZero(String.valueOf(param.value), 3));
                }
            }
        }
        if (EMPTY_STRING.equals(configClass.toString())) {
            configClass = new StringBuilder(BASE);
        }
        return configClass.toString();
    }

    /**
     * convert integer to string.
     *
     * @param value Indicates the Integer.
     *  @return the final string
     */
    private static String parseAscii(Integer value) {
        StringBuilder result = new StringBuilder();
        while (value > 0) {
            result.insert(0, (char) (value & 0xFF));
            value = value >> 8;
        }
        return result.toString();
    }

    /**
     * fillup zero to string.
     * @param inputString Indicates the string should to be filled.
     * @param length Indicates the final length of String.
     * @return the final string
     */
    private static String fillUpZero(String inputString, int length) {
        if (inputString.length() >= length) {
            return inputString;
        }
        StringBuilder result = new StringBuilder();
        while (result.length() < length - inputString.length()) {
            result.append('0');
        }
        result.append(inputString);
        return result.toString();
    }
}

