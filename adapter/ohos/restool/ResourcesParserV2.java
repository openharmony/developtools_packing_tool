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
import ohos.Log;
import ohos.ResourceIndexResult;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

/**
 * Resources Parser V2.
 *
 * @since 2025-06-06
 */
public class ResourcesParserV2 implements ResourcesParser {
    /**
     * Parses resources default id.
     */
    public static final int RESOURCE_DEFAULT_ID = -1;

    private static final Log LOG = new Log(ResourcesParserV2.class.toString());
    private static final int VERSION_BYTE_LENGTH = 128;
    private static final int TAG_BYTE_LENGTH = 4;
    private static final int TYPE_BYTE_LENGTH = 4;
    private static final int VALUE_BYTE_LENGTH = 4;
    private static final int IDSS_TYPE_BYTE_LENGTH = 4;
    private static final int DATA_OFFSET_LENGTH = 4;
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

    private static int idssOffset;
    private static Map<String, Map<String, KeysItemV2>> keysMap = new HashMap<>();
    private static Map<Integer, Map<Integer, IdssItemV2>> idssMap = new HashMap<>();
    private static Map<Integer, Map<Integer, DataItemV2>> dataMap = new HashMap<>();

    /**
     * ResourcesParserV2
     *
     * @param data resource index data
     */
    public ResourcesParserV2(byte[] data) {
        parseZone(data);
    }

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
        BOOLEAN_TYPE(12, "Boolean"),
        DIMEN(13, "Dimen"),
        COLOR(14, "Color"),
        ID(15, "Id"),
        THEME(16, "Theme"),
        PLURALS(17, "Plurals"),
        FLOAT_TYPE(18, "Float"),
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
        private static final Map<Integer, DeviceType> INDEX_MAP;

        static {
            INDEX_MAP = new HashMap<>();
            for (DeviceType deviceType : values()) {
                INDEX_MAP.put(deviceType.index, deviceType);
            }
        }

        private DeviceType(int index, String type) {
            this.index = index;
            this.type = type;
        }

        /**
         * get index
         *
         * @return index of DeviceType
         */
        public int getIndex() {
            return index;
        }

        /**
         * get type
         *
         * @return type of DeviceType
         */
        public String getType() {
            return type;
        }

        /**
         * get type by index
         *
         * @param index index of DeviceType
         * @return type of DeviceType
         */
        public static String getTypeByIndex(int index) {
            DeviceType deviceType = INDEX_MAP.get(index);
            return deviceType != null ? deviceType.type : "";
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
        private static final Map<Integer, Resolution> INDEX_MAP;

        static {
            INDEX_MAP = new HashMap<>();
            for (Resolution resolution : values()) {
                INDEX_MAP.put(resolution.index, resolution);
            }
        }

        private Resolution(int index, String type) {
            this.index = index;
            this.type = type;
        }

        /**
         * get index
         *
         * @return index of Resolution
         */
        public int getIndex() {
            return index;
        }

        /**
         * get type
         *
         * @return type of Resolution
         */
        public String getType() {
            return type;
        }

        /**
         * get type by index
         *
         * @param index index of Resolution
         * @return type of Resolution
         */
        public static String getTypeByIndex(int index) {
            Resolution resolution = INDEX_MAP.get(index);
            return resolution != null ? resolution.type : "";
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
    static class KeyParamV2 {
        int keyType;
        int value;
    }

    /**
     * Config Index.
     */
    static class ConfigIndexV2 {
        String tag;
        int resCfgId;
        int keyCount;
        KeyParamV2[] params;
    }

    /**
     * KEYS item.
     */
    static class KeysItemV2 {
        String tag;
        int resCfgId;
        String type;
        String value;
    }

    /**
     * IDSS item.
     */
    static class IdssItemV2 {
        int type;
        int resId;
        int offset;
        int nameLen;
        String name;
    }

    /**
     * DATA item.
     */
    static class DataItemV2 {
        int type;
        int resId;
        int resCfgId;
        String name;
        String value;
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
            LOG.error("ResourcesParserV2::getIconPath data byte or ResourceId is null");
            return resourceIdValue;
        }

        List<String> result = getResource(resourceId);
        if (result != null && result.size() > 0 && result.get(0) != null && !EMPTY_STRING.equals(result.get(0))) {
            resourceIdValue = result.get(0);
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
            LOG.error("ResourcesParserV2::getBaseResourceById data byte or ResourceId is null");
            return resourceIdValue;
        }
        resourceIdValue = getBaseResource(resourceId, data);
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
        byteBuf.getInt(); // length
        int configCount = byteBuf.getInt();
        byteBuf.getInt(); // dataBrokeOffset
        Optional<ConfigIndexV2> optionalConfigIndex = loadBaseConfig(byteBuf, configCount);
        if (!optionalConfigIndex.isPresent()) {
            LOG.error("ResourcesParserV2::getBaseResource configIndex is null");
            return "";
        }
        return readBaseItem(resId, optionalConfigIndex.get());
    }

    /**
     * Load index config.
     *
     * @param bufBuf config byte buffer
     * @param count config count
     * @return the base config index
     * @throws BundleException IOException.
     */
    static Optional<ConfigIndexV2> loadBaseConfig(ByteBuffer bufBuf, int count) {
        for (int i = 0; i < count; i++) {
            ConfigIndexV2 cfg = new ConfigIndexV2();
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            bufBuf.get(tag);
            cfg.tag = new String(tag, StandardCharsets.UTF_8);
            cfg.resCfgId = bufBuf.getInt();
            cfg.keyCount = bufBuf.getInt();
            cfg.params = new KeyParamV2[cfg.keyCount];
            for (int j = 0; j < cfg.keyCount; j++) {
                cfg.params[j] = new KeyParamV2();
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
     * @return the base item
     * @throws BundleException IOException.
     */
    static String readBaseItem(int resId, ConfigIndexV2 configIndex) {
        Map<Integer, DataItemV2> map = dataMap.get(resId);
        return map.get(configIndex.resCfgId).value;
    }

    /**
     * Get Icon resource.
     *
     * @param resId resource id
     * @return the result
     * @throws BundleException IOException.
     */
    static List<String> getResource(int resId) {
        return readAllItem(resId);
    }

    /**
     * Load index config.
     *
     * @param bufBuf config byte buffer
     * @param count config count
     * @return the config list
     * @throws BundleException IOException.
     */
    static List<ConfigIndexV2> loadConfig(ByteBuffer bufBuf, int count) {
        List<ConfigIndexV2> configList = new ArrayList<>(count);
        for (int i = 0; i < count; i++) {
            ConfigIndexV2 cfg = new ConfigIndexV2();
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            bufBuf.get(tag);
            cfg.tag = new String(tag, StandardCharsets.UTF_8);
            cfg.resCfgId = bufBuf.getInt();
            cfg.keyCount = bufBuf.getInt();
            cfg.params = new KeyParamV2[cfg.keyCount];
            for (int j = 0; j < cfg.keyCount; j++) {
                cfg.params[j] = new KeyParamV2();
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
     * @return the item list
     * @throws BundleException IOException.
     */
    static List<String> readAllItem(int resId) {
        List<String> result = new ArrayList<>();
        Map<Integer, DataItemV2> map = dataMap.get(resId);
        for (DataItemV2 dataItemV2 : map.values()) {
            result.add(dataItemV2.value);
        }
        return result;
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
        int keyCount = byteBuf.getInt();
        byteBuf.getInt(); // dataBrokeOffset
        List<ConfigIndexV2> cfg = loadConfig(byteBuf, keyCount);
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
     * @return resource
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
    static List<ResourceIndexResult> readDataAllItem(List<ConfigIndexV2> configs, ByteBuffer buf) {
        List<ResourceIndexResult> resourceIndexResults = new ArrayList<>();
        for (ConfigIndexV2 index : configs) {
            String configClass = convertConfigIndexToString(index);
            dataMap.values()
                    .forEach(
                            integerDataItemV2Map ->
                                    integerDataItemV2Map
                                            .values()
                                            .forEach(
                                                    dataItemV2 -> {
                                                        if (dataItemV2.resCfgId == index.resCfgId) {
                                                            resourceIndexResults.add(
                                                                    parseDataItems(dataItemV2, configClass));
                                                        }
                                                    }));
        }
        return resourceIndexResults;
    }

    /**
     * convert DataItems to ResourceIndexResult.
     *
     * @param item Indicates the DataItem.
     * @param configClass config info
     * @return the final ResourceIndexResult
     */
    static ResourceIndexResult parseDataItems(DataItemV2 item, String configClass) {
        ResourceIndexResult resourceIndexResult = new ResourceIndexResult();
        resourceIndexResult.configClass = configClass;
        if (item != null) {
            resourceIndexResult.type = ResType.getType(item.type);
            resourceIndexResult.id = item.resId;
            resourceIndexResult.name = item.name;
            if (requireBytesConversion(item.type)) {
                byte[] bytes = item.value.getBytes(StandardCharsets.UTF_8);
                resourceIndexResult.value = convertBytesToString(bytes);
            } else {
                resourceIndexResult.value = item.value;
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
            result.append(item, 0, item.length());
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
    static String convertConfigIndexToString(ConfigIndexV2 configIndex) {
        if (configIndex.keyCount == 0) {
            return BASE;
        }

        StringBuilder configClass = new StringBuilder();
        ConfigType lastKeyType = null;

        for (int i = 0; i < configIndex.keyCount; ++i) {
            KeyParamV2 param = configIndex.params[i];
            ConfigType currentType = ConfigType.values()[param.keyType];
            Optional<String> valuePart = processParamValue(param, currentType);

            if (valuePart.isPresent()) {
                appendValue(configClass, lastKeyType, currentType, valuePart.get());
                lastKeyType = currentType;
            }
        }

        return configClass.length() == 0 ? BASE : configClass.toString();
    }

    private static Optional<String> processParamValue(KeyParamV2 param, ConfigType type) {
        switch (type) {
            case LANGUAGE:
            case REGION:
            case SCRIPT:
                return Optional.ofNullable(parseAscii(param.value));
            case RESOLUTION:
                return Optional.ofNullable(Resolution.getTypeByIndex(param.value));
            case DIRECTION:
                return Optional.ofNullable(param.value == 0 ? VERTICAL : HORIZONTAL);
            case DEVICE_TYPE:
                return Optional.ofNullable(DeviceType.getTypeByIndex(param.value));
            case LIGHT_MODE:
                return Optional.ofNullable(param.value == 0 ? DARK : LIGHT);
            case MCC:
                return Optional.ofNullable(MCC + param.value);
            case MNC:
                return Optional.ofNullable(MNC + fillUpZero(String.valueOf(param.value), 3));
            default:
                return Optional.empty();
        }
    }

    private static void appendValue(
            StringBuilder builder, ConfigType lastKeyType, ConfigType currentKeyType, String value) {
        if (builder.length() == 0) {
            builder.append(value);
            return;
        }

        String conjunction = shouldUseMccConjunction(lastKeyType, currentKeyType)
                ? MCC_CONJUNCTION
                : CONFIG_CONJUNCTION;
        builder.append(conjunction).append(value);
    }

    private static boolean shouldUseMccConjunction(ConfigType lastKeyType, ConfigType currentKeyType) {
        boolean isCurrentMccMnc = currentKeyType == ConfigType.MCC || currentKeyType == ConfigType.MNC;
        boolean isLastLanguageRegionScript =
                lastKeyType != null
                        && (lastKeyType == ConfigType.LANGUAGE
                        || lastKeyType == ConfigType.REGION
                        || lastKeyType == ConfigType.SCRIPT);

        return isCurrentMccMnc || isLastLanguageRegionScript;
    }

    /**
     * convert integer to string.
     *
     * @param value Indicates the Integer.
     *  @return the final string
     */
    private static String parseAscii(Integer value) {
        if (value == null) {
            return "";
        }
        StringBuilder result = new StringBuilder();
        int tempValue = value;
        while (tempValue > 0) {
            result.insert(0, (char) (tempValue & 0xFF));
            tempValue = tempValue >> 8;
        }
        return result.toString();
    }

    /**
     * fillup zero to string.
     *
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

    /**
     * Parse KEYS,IDSS,DATA zone.
     *
     * @param data resource byte
     */
    static void parseZone(byte[] data) {
        if (!parseKEYSZone(data)) {
            LOG.error("ResourcesParserV2 parseKEYSZone() failed");
        }
        if (!parseDATAZone(data)) {
            LOG.error("ResourcesParserV2 parseDATAZone() failed");
        }
        if (!parseIDSSZone(data)) {
            LOG.error("ResourcesParserV2 parseIDSSZone() failed");
        }
    }

    /**
     * Parse KEYS zone.
     *
     * @param data resource byte
     * @return parse result
     */
    static boolean parseKEYSZone(byte[] data) {
        if (data == null || data.length == 0) {
            LOG.error("ResourcesParserV2::parseKEYSZone data byte is null");
            return false;
        }
        ByteBuffer buf = ByteBuffer.wrap(data);
        buf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        buf.get(version);
        buf.getInt(); // length
        int keyCount = buf.getInt();
        buf.getInt(); // dataBrokeOffset
        // KEYS zone
        for (int i = 0; i < keyCount; i++) {
            Map<String, KeysItemV2> map = new HashMap<>();
            byte[] tag = new byte[TAG_BYTE_LENGTH];
            buf.get(tag);
            String tagStr = new String(tag, StandardCharsets.UTF_8);
            int resCfgId = buf.getInt();
            int keyParamCount = buf.getInt();
            for (int j = 0; j < keyParamCount; j++) {
                KeysItemV2 keysItemV2 = new KeysItemV2();
                keysItemV2.tag = tagStr;
                keysItemV2.resCfgId = resCfgId;
                byte[] type = new byte[TYPE_BYTE_LENGTH];
                buf.get(type);
                keysItemV2.type = new String(type, StandardCharsets.UTF_8);
                byte[] value = new byte[VALUE_BYTE_LENGTH];
                buf.get(value);
                keysItemV2.value = new String(value, StandardCharsets.UTF_8);
                map.put(keysItemV2.type, keysItemV2);
            }
            keysMap.put(tagStr, map);
        }
        idssOffset = buf.position();
        return true;
    }

    /**
     * Parse IDSS zone.
     *
     * @param data resource byte
     * @return parse result
     */
    static boolean parseIDSSZone(byte[] data) {
        if (data == null || data.length == 0) {
            LOG.error("ResourcesParserV2::parseIDSSZone data byte is null");
            return false;
        }
        if (idssOffset == 0) {
            LOG.error("ResourcesParserV2::parseIDSSZone idssOffset has not parse");
            return false;
        }
        ByteBuffer buf = ByteBuffer.wrap(data);
        buf.order(ByteOrder.LITTLE_ENDIAN);
        buf.position(idssOffset);
        buf.getInt(); // tag
        buf.getInt(); // len
        int typeCount = buf.getInt();
        buf.getInt(); // idCount
        // IDSS zone
        for (int i = 0; i < typeCount; i++) {
            Map<Integer, IdssItemV2> map = new HashMap<>();
            int type = buf.getInt();
            buf.getInt(); // length
            int count = buf.getInt();
            for (int j = 0; j < count; j++) {
                IdssItemV2 idssItemV2 = new IdssItemV2();
                idssItemV2.type = type;
                idssItemV2.resId = buf.getInt();
                idssItemV2.offset = buf.getInt();
                int nameLen = buf.getInt();
                byte[] name = new byte[nameLen];
                buf.get(name);
                idssItemV2.nameLen = nameLen;
                idssItemV2.name = new String(name, StandardCharsets.UTF_8);
                dataMap.get(idssItemV2.resId)
                        .values()
                        .forEach(
                                dataItemV2 -> {
                                    dataItemV2.type = idssItemV2.type;
                                    dataItemV2.name = idssItemV2.name;
                                });
                map.put(idssItemV2.resId, idssItemV2);
            }
            idssMap.put(type, map);
        }
        return true;
    }

    /**
     * Parse DATA zone.
     *
     * @param data resource byte
     * @return data zone
     */
    static boolean parseDATAZone(byte[] data) {
        if (data == null || data.length == 0) {
            LOG.error("ResourcesParserV2::parseDATAZone data byte is null");
            return false;
        }
        ByteBuffer buf = ByteBuffer.wrap(data);
        buf.order(ByteOrder.LITTLE_ENDIAN);
        byte[] version = new byte[VERSION_BYTE_LENGTH];
        buf.get(version);
        buf.getInt(); // length
        buf.getInt();
        int dataZoneOffset = buf.getInt(); // dataBrokeOffset
        buf.position(dataZoneOffset);
        buf.getInt(); // DATA tag
        buf.getInt(); // DATA length
        int resIdCount = buf.getInt();
        for (int i = 0; i < resIdCount; i++) {
            Map<Integer, DataItemV2> resCfgIdMap = new HashMap<>();
            int resId = buf.getInt();
            buf.getInt(); // length
            int resCfgIdCount = buf.getInt();
            for (int j = 0; j < resCfgIdCount; j++) {
                DataItemV2 dataItemV2 = new DataItemV2();
                dataItemV2.resId = resId;
                dataItemV2.resCfgId = buf.getInt();
                int dataOffset = buf.getInt();
                int position = buf.position();
                buf.position(dataOffset);
                int dataLen = buf.getShort() & 0xFFFF;
                byte[] tag = new byte[dataLen];
                buf.get(tag);
                dataItemV2.value = new String(tag, StandardCharsets.UTF_8);
                buf.position(position);
                resCfgIdMap.put(dataItemV2.resCfgId, dataItemV2);
            }
            dataMap.put(resId, resCfgIdMap);
        }
        return true;
    }
}

