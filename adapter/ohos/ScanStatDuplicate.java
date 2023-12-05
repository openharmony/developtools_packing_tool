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
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

/**
 * scanDuplicate info
 *
 * @since 2023/11/27
 */

public class ScanStatDuplicate {
    private static final String UNPACK_NAME = "unpack";
    private static final String DUPLICATE_FOLDER_NAME = "duplicate";
    private static final String EMPTY_STRING = "";
    private static final String SHA_256 = "SHA-256";
    private static final String RESULT_MD5 = "md5";
    private static final String RESULT_SIZE = "size";
    private static final String RESULT_FILES = "files";
    private static final String DUPLICATE_DESC = "find the duplicated files";
    private static final String DUPLICATE_PARAM = "--stat-duplicate";
    private static final String TASK_TYPE = "taskType";
    private static final String TASK_DESC = "taskDesc";
    private static final String PARAM = "param";
    private static final String START_TIME = "startTime";
    private static final String STOP_TIME = "stopTime";
    private static final String RESULT = "result";
    private static final String DATE_FORMAT = "yyyy-MM-dd HH:mm:ss:SSS";
    private static final int MD5_BUFFER_SIZE = 1024;
    private static final long DUPLICATE_TYPE = 1L;
    private static final int MD5_LENGTH = 16;
    private static final long SHOW_SIZE = 10L;
    private static final String HTML_BUTTON_SHOW = "<button id=\"show_%s\" type=\"button\" "
            + "onclick=\"show_%s()\" style=\"display: block\">more</button>";
    private static final String HTML_BUTTON_HIDE = "<button id=\"hide_%s\" type=\"button\" "
            + "onclick=\"hide_%s()\" style=\"display: none\">close</button>";
    private static final String HTML_TABLE_BOX = "<table class=\"boxTable\">";
    private static final String HTML_TABLE_END = "</table>";
    private static final String HTML_TR_TD_LAYOUT = "<tr class=\"layout\"><td class=\"key\">";
    private static final String HTML_TR_TD_VALUE = "</td><td class=\"value\">";
    private static final String HTML_TR_TD_END = "</td></tr>";
    private static final String HTML_TR_TD_RESULT = "<tr class=\"result\"><td class=\"key\">";
    private static final String HTML_TR_TD = "<tr%s><td%s>";
    private static final String HTML_TD_END_TD = "</td><td%s>";
    private static final String HTML_LI_CIRCLE = "<li type=\"circle\">";
    private static final String HTML_LI_END = "</li>";
    private static final String HTML_TR_TD_DUPLICATE_KEY = "<tr class=\"duplicateLayout\"><td class=\"duplicateKey\">";
    private static final String HTML_TD_DUPLICATE_VALUE = "</td><td class=\"duplicateValue\"><ul>";
    private static final String HTML_UL_TD_TR_END = "</ul></td></tr>";
    private static final String HTML_UL_HEAD = "<ul>";
    private static final String CLASS_DUPLICATE_LAYOUT = " class=\"duplicateLayout\"";
    private static final String CLASS_DUPLICATE_KEY = " class=\"duplicateKey\"";
    private static final String CLASS_DUPLICATE_VALUE = " class=\"duplicateValue\"";
    private static final String HTML_LI_HEAD = "<li>";
    private static final String HTML_LI_DUPLICATE = "<li class=\"duplicate\">";
    private static final String HTML_TABLE_DUPLICATE = "<table class=\"duplicateTable\">";
    private static final String HTML_TABLE_LI_END = "</table></li>";
    private static final String HTML_UL_END = "</ul>";
    private static final Log LOG = new Log(ScanStatDuplicate.class.toString());

    private static class ParamModel {
        private String md5 = EMPTY_STRING;
        private long size;
        private List<String> files = new ArrayList<>();

        public String getMd5() {
            return this.md5;
        }
        public void setMd5(String md5) {
            this.md5 = md5;
        }
        public long getSize() {
            return this.size;
        }
        public void setSize(long size) {
            this.size = size;
        }
        public List<String> getFiles() {
            return this.files;
        }
        public void setFiles(List<String> files) {
            this.files = files;
        }
    }

    private static class DuplicateResult {
        private long taskType = DUPLICATE_TYPE;
        private String taskDesc = DUPLICATE_DESC;
        private String param = DUPLICATE_PARAM;
        private String startTime = EMPTY_STRING;
        private String stopTime = EMPTY_STRING;
        private List<ParamModel> result = new ArrayList<>();

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
        public List<ParamModel> getResult() {
            return this.result;
        }
        public void setResult(List<ParamModel> result) {
            this.result = result;
        }
    }

    /**
     * scan statDuplicate.
     *
     * @param utility utility data
     * @param jsonList List<String> data
     * @param fileList List<String> data
     * @return duplicate html
     * @throws IOException Throws this exception if getDuplicateResList exception
     * @throws NoSuchAlgorithmException Throws this exception if getDuplicateResList exception
     */
    public String statDuplicate(Utility utility, List<String> jsonList, List<String> fileList)
            throws IOException, NoSuchAlgorithmException {
        DuplicateResult duplicateResult = new DuplicateResult();
        duplicateResult.setStartTime(getCurrentTime());
        List<ParamModel> resList = getDuplicateResList(fileList);
        File parentFile = new File(utility.getOutPath());
        if (!parentFile.exists() && !parentFile.mkdirs()) {
            LOG.error(ScanErrorEnum.STAT_DUPLICATE_CREATE_FILE_ERROR.toString());
        }
        List<ParamModel> filterList = new ArrayList<>();
        for (ParamModel model : resList) {
            List<String> files = model.getFiles();
            if (files.size() > 1) {
                filterList.add(model);
            }
        }
        duplicateResult.setResult(filterList);
        duplicateResult.setStopTime(getCurrentTime());
        String taskTypeHtml = getHtmlRow(TASK_TYPE, duplicateResult.getTaskType());
        String taskDescHtml = getHtmlRow(TASK_DESC, duplicateResult.getTaskDesc());
        String paramHtml = getHtmlRow(PARAM, duplicateResult.getParam());
        String startTimeHtml = getHtmlRow(START_TIME, duplicateResult.getStartTime());
        String stopTimeHtml = getHtmlRow(STOP_TIME, duplicateResult.getStopTime());
        String resultValue = getResultHtml(duplicateResult.getResult());
        String resultHtml = getHtmlRowResultClass(RESULT, resultValue);
        String htmlStr = HTML_TABLE_BOX + taskTypeHtml + taskDescHtml + paramHtml
                + startTimeHtml + stopTimeHtml + resultHtml + HTML_TABLE_END;
        String jsonStr = JSON.toJSONString(duplicateResult, new SerializerFeature[] {
                SerializerFeature.PrettyFormat, SerializerFeature.WriteMapNullValue,
                SerializerFeature.WriteDateUseDateFormat});
        jsonList.add(jsonStr);
        return htmlStr;
    }

    private static String getCurrentTime() {
        long currentTimeMillis = System.currentTimeMillis();
        return new SimpleDateFormat(DATE_FORMAT).format(currentTimeMillis);
    }

    private List<ParamModel> getDuplicateResList(List<String> fileList) throws IOException, NoSuchAlgorithmException {
        List<ParamModel> resList = new ArrayList<>();
        for (String filePath : fileList) {
            boolean addFlag = true;
            String md5 = md5HashCode(filePath);
            for (ParamModel element : resList) {
                String eleMd5 = element.getMd5();
                if (eleMd5.equals(md5)) {
                    List<String> eleFiles = element.getFiles();
                    eleFiles.add(splitPath(filePath, UNPACK_NAME));
                    element.setFiles(eleFiles);
                    addFlag = false;
                }
            }
            if (addFlag) {
                ParamModel model = new ParamModel();
                long size = FileUtils.getFileSize(filePath);
                model.setMd5(md5);
                model.setSize(size);
                List<String> files = model.getFiles();
                files.add(splitPath(filePath, UNPACK_NAME));
                resList.add(model);
            }
        }
        return resList;
    }

    private static String getHtmlRow(String key, long valve) {
        return HTML_TR_TD_LAYOUT + key + HTML_TR_TD_VALUE + valve + HTML_TR_TD_END;
    }

    private static String getHtmlRow(String key, String valve) {
        return HTML_TR_TD_LAYOUT + key + HTML_TR_TD_VALUE + valve + HTML_TR_TD_END;
    }

    private static String getHtmlRowResultClass(String key, String valve) {
        return HTML_TR_TD_RESULT + key + HTML_TR_TD_VALUE + valve + HTML_TR_TD_END;
    }

    private static String getHtmlRowResult(String key, long valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TR_TD + key + HTML_TD_END_TD + valve + HTML_TR_TD_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getHtmlRowResult(String key, String valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TR_TD + key + HTML_TD_END_TD + valve + HTML_TR_TD_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getHtmlRowResult(String key, List<String> valve) {
        StringBuilder resValve = new StringBuilder(EMPTY_STRING);
        for (String ele : valve) {
            resValve.insert(0, HTML_LI_CIRCLE + ele + HTML_LI_END);
        }
        return HTML_TR_TD_DUPLICATE_KEY + key + HTML_TD_DUPLICATE_VALUE + resValve + HTML_UL_TD_TR_END;
    }

    private static String getResultHtml(List<ParamModel> models) {
        StringBuilder resultHtml = new StringBuilder(EMPTY_STRING);
        resultHtml.append(HTML_UL_HEAD);
        for (int i = 0; i < models.size(); i++) {
            ParamModel model = models.get(i);
            String md5Html = getHtmlRowResult(RESULT_MD5, model.getMd5(),
                    CLASS_DUPLICATE_LAYOUT, CLASS_DUPLICATE_KEY, CLASS_DUPLICATE_VALUE);
            String sizeHtml = getHtmlRowResult(RESULT_SIZE, model.getSize(),
                    CLASS_DUPLICATE_LAYOUT, CLASS_DUPLICATE_KEY, CLASS_DUPLICATE_VALUE);
            String filesHtml = getHtmlRowResult(RESULT_FILES, model.getFiles());
            String liHtml;
            if (SHOW_SIZE > i) {
                liHtml = HTML_LI_HEAD;
            } else {
                liHtml = HTML_LI_DUPLICATE;
            }
            String modelHtml = liHtml + HTML_TABLE_DUPLICATE
                    + md5Html + sizeHtml + filesHtml + HTML_TABLE_LI_END;
            resultHtml.append(modelHtml);
        }
        resultHtml.append(HTML_UL_END);
        if (models.size() > SHOW_SIZE) {
            resultHtml.append(String.format(HTML_BUTTON_SHOW, DUPLICATE_FOLDER_NAME, DUPLICATE_FOLDER_NAME));
            resultHtml.append(String.format(HTML_BUTTON_HIDE, DUPLICATE_FOLDER_NAME, DUPLICATE_FOLDER_NAME));
        }
        return resultHtml.toString();
    }

    private String md5HashCode(String filePath) throws IOException, NoSuchAlgorithmException {
        try (InputStream fis = new FileInputStream(filePath)) {
            MessageDigest md = MessageDigest.getInstance(SHA_256);
            byte[] buffer = new byte[MD5_BUFFER_SIZE];
            int length;
            while ((length = fis.read(buffer, 0, MD5_BUFFER_SIZE)) != -1) {
                md.update(buffer, 0, length);
            }
            byte[] md5Bytes = md.digest();
            BigInteger bigInt = new BigInteger(1, md5Bytes);
            return bigInt.toString(MD5_LENGTH);
        } catch (IOException e) {
            LOG.error(ScanErrorEnum.STAT_DUPLICATE_INPUT_STREAM_ERROR + e.getMessage());
            throw new IOException(ScanErrorEnum.STAT_DUPLICATE_INPUT_STREAM_ERROR.msg + e.getMessage());
        } catch (NoSuchAlgorithmException e) {
            LOG.error(ScanErrorEnum.STAT_DUPLICATE_MESSAGE_DIGEST_ERROR + e.getMessage());
            throw new NoSuchAlgorithmException(ScanErrorEnum.STAT_DUPLICATE_MESSAGE_DIGEST_ERROR.msg
                    + e.getMessage());
        }
    }

    private String splitPath(String path, String packageName) {
        String[] split = path.split(packageName);
        if (split.length > 1) {
            return split[1];
        } else {
            return path;
        }
    }
}
