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

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Comparator;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * scan info
 *
 * @since 2023/11/27
 */

public class ScanStatSuffix {
    private static final String TRUE = "true";
    private static final String FALSE = "false";
    private static final String SUFFIX_FOLDER_NAME = "suffix";
    private static final String UNPACK_NAME = "unpack";
    private static final String EMPTY_STRING = "";
    private static final String RESULT_FILES = "files";
    private static final String RESULT_TOTAL_SIZE = "totalSize";
    private static final String SUFFIX_DESC = "show files group by file type[.suffix]";
    private static final String SUFFIX_PARAM = "--stat-suffix";
    private static final String SUFFIX_TYPE_UNKNOWN = "unknownType";
    private static final String TASK_TYPE = "taskType";
    private static final String TASK_DESC = "taskDesc";
    private static final String PARAM = "param";
    private static final String START_TIME = "startTime";
    private static final String STOP_TIME = "stopTime";
    private static final String PATH_LIST = "pathList";
    private static final String RESULT = "result";
    private static final String HAP = ".hap";
    private static final String HSP = ".hsp";
    private static final String FILE_TYPE_SO = "so";
    private static final String TMP_FOLDER_NAME = "temporary";
    private static final String LIBS_NAME = "libs";
    private static final String USER_DIR = "user.dir";
    private static final String STATEMENT_FILE = "file:";
    private static final String STATEMENT_SIZE = "size:";
    private static final String STATEMENT_COMPRESS = "compress:";
    private static final int BUFFER_SIZE = 10 * 1024;
    private static final long SUFFIX_TYPE = 3L;
    private static final long SCAN_LEVEL = 2L;
    private static final long SHOW_SIZE = 10L;
    private static final String SPLIT_POINT = "\\.";
    private static final String DATE_FORMAT = "yyyy-MM-dd HH:mm:ss:SSS";

    private static final String HTML_BUTTON_SHOW = "<button id=\"show_%s\" type=\"button\" "
            + "onclick=\"show_%s()\" style=\"display: block\">more</button>";
    private static final String HTML_BUTTON_HIDE = "<button id=\"hide_%s\" type=\"button\" "
            + "onclick=\"hide_%s()\" style=\"display: none\">close</button>";
    private static final String TABLE_BOXTABLE = "<table class=\"boxTable\">";
    private static final String TABLE_END = "</table>";
    private static final String TABLE_SUFFIX = "<table class=\"suffixTable\">";
    private static final String HTML_UL_HEAD = "<ul>";
    private static final String HTML_UL_END = "</ul>";
    private static final String HTML_LI_HEAD = "<li>";
    private static final String HTML_LI_END = "</li>";
    private static final String HTML_BR_HEAD = "<br>";
    private static final String HTML_LI_SUFFIX = "<li class=\"suffix\">";
    private static final String HTML_TR_STYLE = "<tr class=\"layout\"><td class=\"key\">";
    private static final String HTML_TD_STYLE = "</td><td class=\"value\">";
    private static final String HTML_TDTR_END = "</td></tr>";
    private static final String HTML_TDTR_HEAD = "<tr%s><td%s>";
    private static final String HTML_TDTR_SEND = "</td><td%s>";
    private static final String HTML_TRTD_STYLE = "<tr class=\"result\"><td class=\"key\">";
    private static final String CLASS_SUFFIXLAYOUT = " class=\"suffixLayout\"";
    private static final String CLASS_SUFFIXKEY = " class=\"suffixKey\"";
    private static final String CLASS_SUFFIXVALUE = " class=\"suffixValue\"";
    private static final String CLASS_TRTD_SUFFIXVALUE = "<tr class=\"suffixLayout\"><td class=\"suffixKey\">";
    private static final String CLASS_TD_SUFFIXVALUE = "</td><td class=\"suffixValue\">";
    private static final Log LOG = new Log(ScanStatSuffix.class.toString());

    private static class FileInfo {
        private String file;
        private long size;

        public String getFile() {
            return this.file;
        }
        public void setFile(String file) {
            this.file = file;
        }
        public long getSize() {
            return this.size;
        }
        public void setSize(long size) {
            this.size = size;
        }
    }

    private static class SoFile extends FileInfo {
        private String compress = FALSE;

        public String getCompress() {
            return this.compress;
        }
        public void setCompress(String compress) {
            this.compress = compress;
        }
    }

    private static class ParamModelSuffix {
        private String suffix;
        private long totalSize;
        private List files = new ArrayList<>();

        public String getSuffix() {
            return this.suffix;
        }
        public void setSuffix(String suffix) {
            this.suffix = suffix;
        }
        public long getTotalSize() {
            return this.totalSize;
        }
        public void setTotalSize(long totalSize) {
            this.totalSize = totalSize;
        }
        public List getFiles() {
            return this.files;
        }
        public void setFiles(List files) {
            this.files = files;
        }
    }

    private static class SuffixResult {
        private long taskType;
        private String taskDesc;
        private String param;
        private String startTime;
        private String stopTime;
        private List<String> pathList = new ArrayList<>();
        private List<ParamModelSuffix> result = new ArrayList<>();

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
        public List<ParamModelSuffix> getResult() {
            return this.result;
        }
        public void setResult(List<ParamModelSuffix> result) {
            this.result = result;
        }
        public List<String> getPathList() {
            return this.pathList;
        }
        public void setPathList(List<String> pathList) {
            this.pathList = pathList;
        }
    }

    /**
     * scan statSuffix.
     *
     * @param utility utility data
     * @param jsonList List<String> data
     * @param fileList List<String> data
     * @return suffix html
     * @throws BundleException Throws this exception if unpacking exception
     */
    public String statSuffix(Utility utility, List<String> jsonList, List<String> fileList)
            throws BundleException {
        SuffixResult suffixResult = new SuffixResult();
        suffixResult.setTaskType(SUFFIX_TYPE);
        suffixResult.setTaskDesc(SUFFIX_DESC);
        suffixResult.setParam(SUFFIX_PARAM);
        suffixResult.setStartTime(getCurrentTime());
        String currentDir = System.getProperty(USER_DIR);
        String outPath = currentDir + File.separator + TMP_FOLDER_NAME;
        String packageName = utility.getInput();
        unpackHap(packageName, outPath);
        ArrayList<String> soList = new ArrayList<>();
        suffixResult.setPathList(getPathListData(outPath, packageName, soList));
        List<ParamModelSuffix> resulList = setMapData(outPath, fileList, soList);
        File deleteFile = new File(outPath);
        deleteFile(deleteFile);
        resulList.sort(Comparator.comparing(ParamModelSuffix::getTotalSize).reversed());
        suffixResult.setResult(resulList);
        File parentFile = new File(utility.getOutPath());
        if (!parentFile.exists() && !parentFile.mkdirs()) {
            LOG.error(ScanErrorEnum.SUFFIX_MKDIRS_ERROR.toString());
        }
        suffixResult.setStopTime(getCurrentTime());
        String htmlStr = setHtmlData(suffixResult);
        String jsonStr = JSON.toJSONString(suffixResult, new SerializerFeature[] {
                SerializerFeature.PrettyFormat, SerializerFeature.WriteMapNullValue,
                SerializerFeature.WriteDateUseDateFormat});
        jsonList.add(jsonStr);
        return htmlStr;
    }

    private List<String> getPathListData(String outPath, String packageName, ArrayList<String> soList) {
        List<String> pathList = new ArrayList<>();
        FileUtils.getFileList(outPath, soList);
        File pack = new File(packageName);
        for (String file : soList) {
            if (file.contains(HAP) || file.contains(HSP)) {
                file = splitPath(file, TMP_FOLDER_NAME);
                pathList.add(pack.getName() + file);
            }
        }
        return pathList;
    }

    private List<ParamModelSuffix> setMapData(String outPath, List<String> fileList, ArrayList<String> soList) {
        HashMap<String, List<FileInfo>> sameSuffixMap = new HashMap<>();
        List<ParamModelSuffix> resulList = new ArrayList<>();
        for (String filePath : fileList) {
            sameSuffixMap = accountFileType(sameSuffixMap, filePath);
        }
        Iterator<String> iterator = sameSuffixMap.keySet().iterator();
        while (iterator.hasNext()) {
            String next = iterator.next();
            ParamModelSuffix paramModelSuffix = new ParamModelSuffix();
            paramModelSuffix.setSuffix(next);
            if (next.equalsIgnoreCase(FILE_TYPE_SO)) {
                List<SoFile> soFiles = new ArrayList<>();
                List<FileInfo> fileInfoList = sameSuffixMap.get(next);
                long sum = 0L;
                for (FileInfo param : fileInfoList) {
                    SoFile soFile = new SoFile();
                    soFile.setSize(param.getSize());
                    String soFilePath = param.getFile();
                    soFilePath = splitPath(soFilePath, UNPACK_NAME);
                    soFile.setFile(soFilePath);
                    int index = soFilePath.indexOf(File.separator + LIBS_NAME);
                    String hapPath = soFilePath.substring(0, index);
                    File hapFile = new File(hapPath);
                    long oldSize = getOldSize(soList, hapFile);
                    long newSize = getNewSize(fileInfoList, hapFile);
                    soFile.setCompress(oldSize < newSize ? TRUE : FALSE);
                    soFiles.add(soFile);
                    sum += param.getSize();
                }
                File deleteFile = new File(outPath);
                deleteFile(deleteFile);
                paramModelSuffix.setTotalSize(sum);
                soFiles.sort(Comparator.comparing(SoFile::getSize).reversed());
                paramModelSuffix.setFiles(soFiles);
            } else {
                List<FileInfo> fileInfoList = sameSuffixMap.get(next);
                for (FileInfo fileInfo : fileInfoList) {
                    String path = fileInfo.getFile();
                    fileInfo.setFile(splitPath(path, UNPACK_NAME));
                }
                fileInfoList.sort(Comparator.comparing(FileInfo::getSize).reversed());
                paramModelSuffix.setFiles(fileInfoList);
                long sum = fileInfoList.stream().mapToLong(FileInfo::getSize).sum();
                paramModelSuffix.setTotalSize(sum);
            }
            resulList.add(paramModelSuffix);
        }
        return resulList;
    }

    private String setHtmlData(SuffixResult suffixResult) {
        String pathHtml = EMPTY_STRING;
        if (suffixResult.getPathList() != null && !suffixResult.getPathList().isEmpty()) {
            pathHtml = getPathListHtml(suffixResult.getPathList());
        }
        String pathListHtml = EMPTY_STRING;
        if (!pathHtml.isEmpty() || !EMPTY_STRING.equals(pathHtml)) {
            pathListHtml = getHtmlRow(PATH_LIST, pathHtml);
        }
        String taskTypeHtml = getHtmlRow(TASK_TYPE, suffixResult.getTaskType());
        String taskDescHtml = getHtmlRow(TASK_DESC, suffixResult.getTaskDesc());
        String paramHtml = getHtmlRow(PARAM, suffixResult.getParam());
        String startTimeHtml = getHtmlRow(START_TIME, suffixResult.getStartTime());
        String stopTimeHtml = getHtmlRow(STOP_TIME, suffixResult.getStopTime());
        String resultValue = getResultHtmlOfSuffix(suffixResult.getResult());
        String resultHtml = getHtmlRowResultClass(RESULT, resultValue);
        return TABLE_BOXTABLE + taskTypeHtml + taskDescHtml + paramHtml
                + startTimeHtml + stopTimeHtml + pathListHtml + resultHtml + TABLE_END;
    }

    private String splitPath(String path, String packageName) {
        String[] split = path.split(packageName);
        if (split.length > 1) {
            return split[1];
        } else {
            return path;
        }
    }

    private long getOldSize(List<String> soList, File hapFile) {
        long oldSize = 0L;
        for (String file : soList) {
            File tmp = new File(file);
            if (tmp.getName().equals(hapFile.getName())) {
                oldSize = tmp.length();
            }
        }
        return oldSize;
    }

    private long getNewSize(List<FileInfo> fileInfoList, File hapFile) {
        long newSize = 0L;
        for (FileInfo fileInfo : fileInfoList) {
            if (fileInfo.getFile().contains(hapFile.getPath())) {
                File tmp = new File(fileInfo.getFile());
                newSize += tmp.length();
            }
        }
        return newSize;
    }

    private static HashMap<String, List<FileInfo>> accountFileType(HashMap<String, List<FileInfo>> map, String path) {
        File file = new File(path);
        String[] split = file.getName().split(SPLIT_POINT);
        if (split.length == SCAN_LEVEL) {
            if (map.containsKey(split[1])) {
                accountType(map, file, split[1]);
            } else {
                accountTypeUnknown(map, file, split[1]);
            }
        } else if (split.length == 1) { // no suffix
            if (map.containsKey(SUFFIX_TYPE_UNKNOWN)) {
                accountType(map, file, SUFFIX_TYPE_UNKNOWN);
            } else {
                accountTypeUnknown(map, file, SUFFIX_TYPE_UNKNOWN);
            }
        }
        return map;
    }

    private static void accountType(HashMap<String, List<FileInfo>> hashMap, File file, String key) {
        FileInfo fileInfo = getFileInfo(file);
        List<FileInfo> fileInfoList = hashMap.get(key);
        fileInfoList.add(fileInfo);
        hashMap.put(key, fileInfoList);
    }

    private static void accountTypeUnknown(HashMap<String, List<FileInfo>> hashMap, File file, String key) {
        FileInfo fileInfo = getFileInfo(file);
        List<FileInfo> fileInfoList = new ArrayList<>();
        fileInfoList.add(fileInfo);
        hashMap.put(key, fileInfoList);
    }

    private static FileInfo getFileInfo(File file) {
        FileInfo fileInfo = new FileInfo();
        fileInfo.setFile(file.getPath());
        long size = FileUtils.getFileSize(file.getPath());
        fileInfo.setSize(size);
        return fileInfo;
    }

    private static String getPathListHtml(List<String> models) {
        StringBuilder pathListHtml = new StringBuilder(EMPTY_STRING);
        for (String strHtml : models) {
            pathListHtml.append(strHtml).append(HTML_BR_HEAD);
        }
        return pathListHtml.toString();
    }

    private static String getResultHtmlOfSuffix(List<ParamModelSuffix> models) {
        StringBuilder resultHtml = new StringBuilder(EMPTY_STRING);
        resultHtml.append(HTML_UL_HEAD);
        for (int i = 0; i < models.size(); i++) {
            ParamModelSuffix model = models.get(i);
            String filesHtml;
            if (model.getSuffix().equalsIgnoreCase(FILE_TYPE_SO)) {
                filesHtml = getHtmlRowFileSo(RESULT_FILES, model.getFiles());
            } else {
                filesHtml = getHtmlRowFile(RESULT_FILES, model.getFiles());
            }
            String liHtml;
            if (SHOW_SIZE > i) {
                liHtml = HTML_LI_HEAD;
            } else {
                liHtml = HTML_LI_SUFFIX;
            }
            String suffixHtml = getHtmlRowResult(SUFFIX_FOLDER_NAME, model.getSuffix(),
                    CLASS_SUFFIXLAYOUT, CLASS_SUFFIXKEY, CLASS_SUFFIXVALUE);
            String totalSizeHtml = getHtmlRowResult(RESULT_TOTAL_SIZE, model.getTotalSize(),
                    CLASS_SUFFIXLAYOUT, CLASS_SUFFIXKEY, CLASS_SUFFIXVALUE);
            String modelHtml = liHtml + TABLE_SUFFIX
                    + suffixHtml + totalSizeHtml + filesHtml + TABLE_END + HTML_LI_END;
            resultHtml.append(modelHtml);
        }
        resultHtml.append(HTML_UL_END);
        if (models.size() > SHOW_SIZE) {
            resultHtml.append(String.format(HTML_BUTTON_SHOW, SUFFIX_FOLDER_NAME, SUFFIX_FOLDER_NAME));
            resultHtml.append(String.format(HTML_BUTTON_HIDE, SUFFIX_FOLDER_NAME, SUFFIX_FOLDER_NAME));
        }
        return resultHtml.toString();
    }

    private static String getHtmlRow(String key, String valve) {
        return HTML_TR_STYLE + key + HTML_TD_STYLE + valve + HTML_TDTR_END;
    }

    private static String getHtmlRow(String key, long valve) {
        return HTML_TR_STYLE + key + HTML_TD_STYLE + valve + HTML_TDTR_END;
    }

    private static String getHtmlRowResultClass(String key, String valve) {
        return HTML_TRTD_STYLE + key + HTML_TD_STYLE + valve + HTML_TDTR_END;
    }

    private static String getHtmlRowResult(String key, String valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TDTR_HEAD + key + HTML_TDTR_SEND + valve + HTML_TDTR_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getHtmlRowResult(String key, long valve,
                                           String trClass, String tdClassKey, String tdClassValue) {
        return String.format(HTML_TDTR_HEAD + key + HTML_TDTR_SEND + valve + HTML_TDTR_END,
                trClass, tdClassKey, tdClassValue);
    }

    private static String getHtmlRowFile(String key, List<FileInfo> files) {
        StringBuilder resValve = new StringBuilder(EMPTY_STRING);
        for (FileInfo file : files) {
            resValve.insert(0, STATEMENT_FILE + file.getFile() + HTML_BR_HEAD);
            resValve.insert(0, STATEMENT_SIZE + file.getSize() + HTML_BR_HEAD);
        }
        return CLASS_TRTD_SUFFIXVALUE + key + CLASS_TD_SUFFIXVALUE + resValve + HTML_TDTR_END;
    }

    private static String getHtmlRowFileSo(String key, List<SoFile> files) {
        StringBuilder resValve = new StringBuilder(EMPTY_STRING);
        for (SoFile soFile : files) {
            resValve.insert(0, STATEMENT_FILE + soFile.getFile() + HTML_BR_HEAD);
            resValve.insert(0, STATEMENT_SIZE + soFile.getSize() + HTML_BR_HEAD);
            resValve.insert(0, STATEMENT_COMPRESS + soFile.getCompress() + HTML_BR_HEAD);
        }
        return CLASS_TRTD_SUFFIXVALUE + key + CLASS_TD_SUFFIXVALUE + resValve + HTML_TDTR_END;
    }

    private static String getCurrentTime() {
        long currentTimeMillis = System.currentTimeMillis();
        return new SimpleDateFormat(DATE_FORMAT).format(currentTimeMillis);
    }

    private static void unpackHap(String srcPath, String outPath) throws BundleException {
        try (FileInputStream fis = new FileInputStream(srcPath);
             ZipInputStream zipInputStream = new ZipInputStream(new BufferedInputStream(fis))) {
            File destDir = new File(outPath);
            if (!destDir.exists()) {
                destDir.mkdirs();
            }
            unpackEntryToFile(zipInputStream, outPath);
        } catch (IOException e) {
            LOG.error(ScanErrorEnum.SUFFIX_UNPACK_ERROR + e.getMessage());
            throw new BundleException(ScanErrorEnum.SUFFIX_UNPACK_ERROR.msg + e.getMessage());
        }
    }

    private static void unpackEntryToFile(ZipInputStream zipInputStream, String outPath)
            throws BundleException, IOException {
        ZipEntry entry;
        while ((entry = zipInputStream.getNextEntry()) != null) {
            String entryName = entry.getName();
            File entryFile = new File(outPath, entryName);

            if (entry.isDirectory()) {
                entryFile.mkdirs();
                zipInputStream.closeEntry();
                continue;
            }
            File parent = entryFile.getParentFile();
            if (!parent.exists()) {
                parent.mkdirs();
            }
            try (FileOutputStream fos = new FileOutputStream(entryFile)) {
                byte[] buffer = new byte[BUFFER_SIZE];
                int bytesRead;
                while ((bytesRead = zipInputStream.read(buffer)) != -1) {
                    fos.write(buffer, 0, bytesRead);
                }
            } catch (IOException e) {
                LOG.error(ScanErrorEnum.SUFFIX_UNPACK_STREAM_ERROR + e.getMessage());
                throw new BundleException(ScanErrorEnum.SUFFIX_UNPACK_STREAM_ERROR.msg + e.getMessage());
            }
            zipInputStream.closeEntry();
        }
    }

    private static void deleteFile(File file) {
        if (file == null || !file.exists()) {
            return;
        }
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            for (File fileTmp : files) {
                deleteFile(fileTmp);
            }
        }
        file.delete();
    }
}
