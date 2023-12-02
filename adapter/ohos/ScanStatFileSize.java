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

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializerFeature;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

/**
 * scanFileSize info
 *
 * @since 2023/11/27
 */

public class ScanStatFileSize {
    private static final String FILE_SIZE_FOLDER_NAME = "fileSize";
    private static final String EMPTY_STRING = "";
    private static final String FILE_SIZE_DESC = "find files whose size exceed the limit size";
    private static final String FILE_SIZE_PARAM = "--stat-file-size";
    private static final String FILE_SIZE_PARAM_SEPARATOR = " ";
    private static final String FILE_SIZE_PARAM_VALUE = "%s";
    private static final String FILE_SIZE_RESULT_SIZE = "size";
    private static final String FILE_SIZE_RESULT_FILE = "file";
    private static final String TASK_TYPE = "taskType";
    private static final String TASK_DESC = "taskDesc";
    private static final String PARAM = "param";
    private static final String START_TIME = "startTime";
    private static final String STOP_TIME = "stopTime";
    private static final String RESULT = "result";
    private static final long FILE_SIZE_TYPE = 2L;
    private static final long SHOW_SIZE = 10L;
    private static final String HTML_BUTTON_SHOW = "<button id=\"show_%s\" type=\"button\" "
            + "onclick=\"show_%s()\" style=\"display: block\">more</button>";
    private static final String HTML_BUTTON_HIDE = "<button id=\"hide_%s\" type=\"button\" "
            + "onclick=\"hide_%s()\" style=\"display: none\">close</button>";
    private static final String UNPACK_NAME = "unpack";
    private static final String HTML_TABLE = "<table%s>";
    private static final String HTML_TABLE_END = "</table>";
    private static final String CLASS_BOX_TABLE = " class=\"boxTable\"";
    private static final String CLASS_FILE_SIZE_TABLE = " class=\"fileSizeTable\"";
    private static final String CLASS_FILE_SIZE_LAYOUT = " class=\"fileSizeLayout\"";
    private static final String CLASS_FILE_SIZE_KEY = " class=\"fileSizeKey\"";
    private static final String CLASS_FILE_SIZE_VALUE = " class=\"fileSizeValue\"";
    private static final String CLASS_FILE_SIZE = " class=\"fileSize\"";
    private static final String HTML_TR = "<tr%s>";
    private static final String HTML_TR_END = "</tr>";
    private static final String HTML_TD = "<td%s>";
    private static final String HTML_TD_END = "</td>";
    private static final String CLASS_LAYOUT = " class=\"layout\"";
    private static final String CLASS_KEY = " class=\"key\"";
    private static final String CLASS_VALUE = " class=\"value\"";
    private static final String DATE_FORMAT = "yyyy-MM-dd HH:mm:ss:SSS";
    private static final Log LOG = new Log(ScanStatFileSize.class.toString());

    private static class ParamModelFileSize {
        private String file = EMPTY_STRING;
        private long size;

        public long getSize() {
            return this.size;
        }
        public void setSize(long size) {
            this.size = size;
        }
        public String getFile() {
            return file;
        }
        public void setFile(String file) {
            this.file = file;
        }
    }

    private static class FileSizeResult {
        private long taskType = FILE_SIZE_TYPE;
        private String taskDesc = FILE_SIZE_DESC;
        private String param = FILE_SIZE_PARAM + FILE_SIZE_PARAM_SEPARATOR + FILE_SIZE_PARAM_VALUE;
        private String startTime = EMPTY_STRING;
        private String stopTime = EMPTY_STRING;
        private List<ParamModelFileSize> result = new ArrayList<>();

        public long getTaskType() {
            return this.taskType;
        }
        public void setTaskType(long taskType) {
            this.taskType = taskType;
        }
        public String getTaskDesc() {
            return this.taskDesc;
        }
        public void setTaskDesc(String taskDesc) {
            this.taskDesc = taskDesc;
        }
        public String getParam() {
            return this.param;
        }
        public void setParam(String param) {
            this.param = param;
        }
        public String getStartTime() {
            return this.startTime;
        }
        public void setStartTime(String startTime) {
            this.startTime = startTime;
        }
        public String getStopTime() {
            return this.stopTime;
        }
        public void setStopTime(String stopTime) {
            this.stopTime = stopTime;
        }
        public List<ParamModelFileSize> getResult() {
            return this.result;
        }
        public void setResult(List<ParamModelFileSize> result) {
            this.result = result;
        }
    }

    /**
     *  scan statFileSize.
     *
     * @param utility utility data
     * @param jsonList List<String> data
     * @param fileList List<String> data
     * @return fileSize html
     */
    public String statFileSize(Utility utility, List<String> jsonList, List<String> fileList) {
        FileSizeResult fileSizeResult = new FileSizeResult();
        fileSizeResult.setStartTime(getCurrentTime());
        List<ParamModelFileSize> resList = new ArrayList<>();
        for (String filePath : fileList) {
            long statFileSize = Long.parseLong(utility.getStatFileSize());
            long size = FileUtils.getFileSize(filePath);
            if (size > statFileSize) {
                ParamModelFileSize model = new ParamModelFileSize();
                model.setFile(splitPath(filePath, UNPACK_NAME));
                model.setSize(size);
                resList.add(model);
            }
        }
        resList.sort(Comparator.comparing(ParamModelFileSize::getSize).reversed());
        fileSizeResult.setResult(resList);
        fileSizeResult.setParam(String.format(fileSizeResult.getParam(), utility.getStatFileSize()));
        fileSizeResult.setStopTime(getCurrentTime());
        File parentFile = new File(utility.getOutPath());
        if (!parentFile.exists() && !parentFile.mkdirs()) {
            LOG.error("Scan::statFileSize create target file parent directory failed.");
        }
        String jsonStr = JSON.toJSONString(fileSizeResult, new SerializerFeature[] {
                SerializerFeature.PrettyFormat, SerializerFeature.WriteMapNullValue,
                SerializerFeature.WriteDateUseDateFormat});
        String taskTypeHtml = getHtmlRow(TASK_TYPE, fileSizeResult.getTaskType());
        String taskDescHtml = getHtmlRow(TASK_DESC, fileSizeResult.getTaskDesc());
        String paramHtml = getHtmlRow(PARAM, fileSizeResult.getParam());
        String startTimeHtml = getHtmlRow(START_TIME, fileSizeResult.getStartTime());
        String stopTimeHtml = getHtmlRow(STOP_TIME, fileSizeResult.getStopTime());
        String resultValue = getResultHtmlFileSize(fileSizeResult.getResult());
        String resultHtml = getHtmlRowResultClass(RESULT, resultValue);
        String htmlStr = String.format(HTML_TABLE, CLASS_BOX_TABLE) + taskTypeHtml + taskDescHtml + paramHtml
                + startTimeHtml + stopTimeHtml + resultHtml + HTML_TABLE_END;
        jsonList.add(jsonStr);
        return htmlStr;
    }

    private String splitPath(String path, String packageName) {
        String[] split = path.split(packageName);
        if (split.length > 1) {
            return split[1];
        } else {
            return path;
        }
    }

    private static String getResultHtmlFileSize(List<ParamModelFileSize> models) {
        StringBuilder resultHtml = new StringBuilder(EMPTY_STRING);
        resultHtml.append(String.format(HTML_TABLE, CLASS_FILE_SIZE_TABLE));
        String resultFieldHtml = getHtmlRowResult(FILE_SIZE_RESULT_FILE, FILE_SIZE_RESULT_SIZE,
                CLASS_FILE_SIZE_LAYOUT, CLASS_FILE_SIZE_KEY, CLASS_FILE_SIZE_VALUE);
        resultHtml.append(resultFieldHtml);
        for (int i = 0; i < models.size(); i++) {
            ParamModelFileSize model = models.get(i);
            String resultRowHtml;
            if (SHOW_SIZE > i) {
                resultRowHtml = getHtmlRowResult(model.getFile(), model.getSize(),
                        CLASS_FILE_SIZE_LAYOUT, CLASS_FILE_SIZE_KEY, CLASS_FILE_SIZE_VALUE);
            } else {
                resultRowHtml = getHtmlRowResult(model.getFile(), model.getSize(),
                        CLASS_FILE_SIZE, CLASS_FILE_SIZE_KEY, CLASS_FILE_SIZE_VALUE);
            }
            resultHtml.append(resultRowHtml);
        }
        resultHtml.append(HTML_TABLE_END);
        if (models.size() > SHOW_SIZE) {
            resultHtml.append(String.format(HTML_BUTTON_SHOW, FILE_SIZE_FOLDER_NAME, FILE_SIZE_FOLDER_NAME));
            resultHtml.append(String.format(HTML_BUTTON_HIDE, FILE_SIZE_FOLDER_NAME, FILE_SIZE_FOLDER_NAME));
        }
        return resultHtml.toString();
    }

    private static String getHtmlRow(String key, String valve) {
        return String.format(HTML_TR, CLASS_LAYOUT) + String.format(HTML_TD, CLASS_KEY) + key + HTML_TD_END
                + String.format(HTML_TD, CLASS_VALUE) + valve + HTML_TD_END + HTML_TR_END;
    }

    private static String getHtmlRow(String key, long valve) {
        return String.format(HTML_TR, CLASS_LAYOUT) + String.format(HTML_TD, CLASS_KEY) + key + HTML_TD_END
                + String.format(HTML_TD, CLASS_VALUE) + valve + HTML_TD_END + HTML_TR_END;
    }

    private static String getHtmlRowResultClass(String key, String valve) {
        return String.format(HTML_TR, CLASS_LAYOUT) + String.format(HTML_TD, CLASS_KEY) + key + HTML_TD_END
                + String.format(HTML_TD, CLASS_VALUE) + valve + HTML_TD_END + HTML_TR_END;
    }

    private static String getHtmlRowResult(String key, String valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TR + HTML_TD + key + HTML_TD_END + HTML_TD + valve + HTML_TD_END + HTML_TR_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getHtmlRowResult(String key, long valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TR + HTML_TD + key + HTML_TD_END + HTML_TD + valve + HTML_TD_END + HTML_TR_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getCurrentTime() {
        long currentTimeMillis = System.currentTimeMillis();
        return new SimpleDateFormat(DATE_FORMAT).format(currentTimeMillis);
    }
}
