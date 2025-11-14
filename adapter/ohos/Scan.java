/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * scan info
 *
 * @since 2023/11/23
 */

public class Scan {
    private static final String LINUX_FILE_SEPARATOR = "/";
    private static final String EMPTY_STRING = "";
    private static final String HTML_START = "<!DOCTYPE html><html lang=\"en\">";
    private static final String HTML_END = "</html>";
    private static final String STAT_JSON = "stat.json";
    private static final String STAT_HTML = "stat.html";
    private static final String STAT_CSS = "stat.css";
    private static final String HTML_HEAD = "<head><meta charset=\"UTF-8\" name=\"stat\">"
            + "<title>stat</title><link rel=\"stylesheet\" href=\"./stat.css\"></head>";
    private static final String UNPACK_NAME = "unpack";
    private static final String BACKUPS = "backups";
    private static final String HAP = ".hap";
    private static final String HSP = ".hsp";
    private static final String TEMPLATE_HTML = "ohos/scan_template.html";
    private static final String TEMPLATE_CSS = "ohos/scan_template.css";
    private static final String DIV_BOX = "<div id=\"box\">";
    private static final String HTML_BODY = "<body>";
    private static final String HTML_DIV_END = "</div>";
    private static final String HTML_BODY_END = "</body>";
    private static final String USER_DIR = "user.dir";
    private static final String HTML_BR = "<br/>";
    private static final String SCAN_RESULT = "scan_result";
    private static final String SO_SUFFIX = ".*\\.so(?:\\.\\d+)*$";
    private static final int BUFFER_SIZE = 10 * 1024;
    private static final Log LOG = new Log(Scan.class.toString());


    /**
     * start scan.
     *
     * @param utility common data
     * @return scanProcess if scan succeed
     */
    public boolean scanProcess(Utility utility) {
        File destFile = new File(utility.getOutPath());
        File outParentFile = destFile.getParentFile();
        if ((outParentFile != null) && (!outParentFile.exists())) {
            if (!outParentFile.mkdirs()) {
                LOG.error(ScanErrorEnum.SCAN_MKDIRS_ERROR.toString());
                return false;
            }
        }
        boolean scanResult = true;
        try {
            scanExecute(utility);
        } catch (FileNotFoundException exception) {
            scanResult = false;
            LOG.error(ScanErrorEnum.SCAN_NOT_FOUND_ERROR + exception.getMessage());
        } catch (BundleException | NoSuchAlgorithmException | IOException exception) {
            scanResult = false;
            LOG.error(ScanErrorEnum.SCAN_REMIND_ERROR + exception.getMessage());
        } finally {
            if (!scanResult) {
                LOG.error(ScanErrorEnum.SCAN_COMPRESS_ERROR.toString());
                if (!destFile.delete()) {
                    LOG.error(ScanErrorEnum.SCAN_DELETE_ERROR.toString());
                }
            }
        }
        return scanResult;
    }

    private static boolean isSharedLibrary(String filePath) {
        return Pattern.compile(SO_SUFFIX).matcher(filePath).find();
    }

    public void scanSoFiles(Utility utility)
        throws BundleException, IOException, NoSuchAlgorithmException {
        List<String> jsonList = new ArrayList<>();
        String templateHtml = getJsTemplate(TEMPLATE_HTML);
        templateHtml = templateHtml.replace(HTML_BR, System.lineSeparator());
        String htmlStr = HTML_START + HTML_HEAD + DIV_BOX + HTML_BODY + templateHtml;
        String currentDir = System.getProperty(USER_DIR);
        String targetPath = currentDir + LINUX_FILE_SEPARATOR + UNPACK_NAME;
        List<String> fileList = getAllInputFileList(utility, targetPath);
        fileList.removeIf(filePath -> !isSharedLibrary(filePath));

        ScanStatDuplicate scanStatDuplicate = new ScanStatDuplicate();
        String duplicateHtml = scanStatDuplicate.statDuplicateForSo(utility, jsonList, fileList);
        htmlStr = htmlStr + duplicateHtml;
        htmlStr = htmlStr + HTML_DIV_END + HTML_BODY_END + HTML_END;
        String reportPath = utility.getOutPath() + LINUX_FILE_SEPARATOR + SCAN_RESULT;
        File reportDir = new File(reportPath);
        if (!reportDir.exists()) {
            reportDir.mkdirs();
        }
        String jsonPath = reportPath + LINUX_FILE_SEPARATOR + STAT_JSON;
        String htmlPath = reportPath + LINUX_FILE_SEPARATOR + STAT_HTML;
        String cssPath = reportPath + LINUX_FILE_SEPARATOR + STAT_CSS;
        String jsonStr = jsonList.get(0);
        writeFile(jsonPath, jsonStr);
        writeFile(htmlPath, htmlStr);
        String templateCss = getJsTemplate(TEMPLATE_CSS);
        writeFile(cssPath, templateCss);
        File deleteFile = new File(targetPath);
        deleteFile(deleteFile);
    }

    private void scanExecute(Utility utility) throws BundleException, IOException, NoSuchAlgorithmException {
        List<String> jsonList = new ArrayList<>();
        String templateHtml = getJsTemplate(TEMPLATE_HTML);
        templateHtml = templateHtml.replace(HTML_BR, System.lineSeparator());
        String htmlStr = HTML_START + HTML_HEAD + DIV_BOX + HTML_BODY + templateHtml;
        String currentDir = System.getProperty(USER_DIR);
        String targetPath = currentDir + LINUX_FILE_SEPARATOR + UNPACK_NAME;
        List<String> fileList = getAllInputFileList(utility, targetPath);
        if (utility.getStatDuplicate()) {
            ScanStatDuplicate scanStatDuplicate = new ScanStatDuplicate();
            String duplicateHtml = scanStatDuplicate.statDuplicate(utility, jsonList, fileList);
            htmlStr = htmlStr + duplicateHtml;
        }
        if (null != utility.getStatFileSize() && !utility.getStatFileSize().isEmpty()) {
            ScanStatFileSize scanStatFileSize = new ScanStatFileSize();
            String fileSizeHtml = scanStatFileSize.statFileSize(utility, jsonList, fileList);
            htmlStr = htmlStr + fileSizeHtml;
        }
        if (utility.getStatSuffix()) {
            ScanStatSuffix scanStatSuffix = new ScanStatSuffix();
            String suffixHtml = scanStatSuffix.statSuffix(utility, jsonList, fileList);
            htmlStr = htmlStr + suffixHtml;
        }
        if (!((!utility.getStatDuplicate()) && !utility.getStatSuffix()
                && EMPTY_STRING.equals(utility.getStatFileSize()))) {
            htmlStr = htmlStr + HTML_DIV_END + HTML_BODY_END + HTML_END;
            String jsonPath = utility.getOutPath() + LINUX_FILE_SEPARATOR + STAT_JSON;
            String htmlPath = utility.getOutPath() + LINUX_FILE_SEPARATOR + STAT_HTML;
            String cssPath = utility.getOutPath() + LINUX_FILE_SEPARATOR + STAT_CSS;
            writeFile(jsonPath, jsonList.toString());
            writeFile(htmlPath, htmlStr);
            String templateCss = getJsTemplate(TEMPLATE_CSS);
            writeFile(cssPath, templateCss);
        }
        File deleteFile = new File(targetPath);
        deleteFile(deleteFile);
    }

    private List<String> getAllInputFileList(Utility utility, String path) throws BundleException, IOException {
        ArrayList<String> fileList = new ArrayList<>();
        unpackHap(utility.getInput(), path);
        File file = new File(path);
        File[] files = file.listFiles();
        if (files == null) {
            LOG.error(ScanErrorEnum.SCAN_NO_FILE_ERROR.toString());
            return fileList;
        }
        String copyPath = path + LINUX_FILE_SEPARATOR + BACKUPS;
        for (File f : files) {
            String fileName = f.getName();
            if (fileName.endsWith(HSP) || fileName.endsWith(HAP)) {
                String absolutePath = f.getCanonicalPath();
                File destDir = new File(copyPath);
                if (!destDir.exists()) {
                    destDir.mkdirs();
                }
                String targetPath = copyPath + LINUX_FILE_SEPARATOR + fileName;
                File targetFile = new File(targetPath);
                File sourceFile = new File(absolutePath);
                FileUtils.copyFile(sourceFile, targetFile);
                File deleteFile = new File(absolutePath);
                deleteFile(deleteFile);
                String outPath = path + LINUX_FILE_SEPARATOR + fileName;
                File outDir = new File(outPath);
                if (!outDir.exists()) {
                    outDir.mkdirs();
                }
                unpackHap(targetPath, outPath);
            }
        }
        File deleteFile = new File(copyPath);
        deleteFile(deleteFile);
        FileUtils.getFileList(path, fileList);
        return fileList;
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
            LOG.error(ScanErrorEnum.SCAN_UNPACK_ERROR + e.getMessage());
            throw new BundleException(ScanErrorEnum.SCAN_UNPACK_ERROR.msg + e.getMessage());
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
                LOG.error(ScanErrorEnum.SCAN_UNPACK_ERROR + e.getMessage());
                throw new BundleException(ScanErrorEnum.SCAN_UNPACK_ERROR.msg + e.getMessage());
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

    private static String getJsTemplate(String fileName) throws IOException {
        ClassLoader classLoader = Scan.class.getClassLoader();
        URL resource = classLoader.getResource(fileName);
        try (BufferedReader bufferedReader = new BufferedReader(
                new InputStreamReader(resource.openStream(), StandardCharsets.UTF_8))) {
            StringBuilder template = new StringBuilder();
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                template.append(line);
            }
            return template.toString();
        } catch (IOException e) {
            LOG.error(ScanErrorEnum.SCAN_GET_JS_TEMPLATE_ERROR + e.getMessage());
            throw new IOException(ScanErrorEnum.SCAN_GET_JS_TEMPLATE_ERROR.msg + e.getMessage());
        }
    }

    private static void writeFile(String targetPath, String data) throws IOException {
        try (FileWriter fileWriter = new FileWriter(targetPath)) {
            fileWriter.write(data);
            fileWriter.flush();
        } catch (IOException e) {
            LOG.error(ScanErrorEnum.SCAN_WRITEFILE_ERROR + e.getMessage());
            throw new IOException(ScanErrorEnum.SCAN_WRITEFILE_ERROR.msg + e.getMessage());
        }
    }
}
