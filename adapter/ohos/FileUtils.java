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

package ohos;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.OutputStream;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Optional;
import java.util.regex.Pattern;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import java.security.MessageDigest;

/**
 * file tools
 *
 */
class FileUtils {
    private static final int BUFFER_SIZE = 1024;
    private static final int SHA256_BUFFER_SIZE = 10240;
    private static final Log LOG = new Log(FileUtils.class.toString());
    private static final String RESOURCE_PATH = "resources/base/profile/";
    private static final String SHA256 = "SHA-256";
    private static final String PATTERN = "[0-9A-Za-z/].{0,4095}";

    /**
     * generate fileData byte stream
     *
     * @param filePath file path
     * @return byte array
     */
    public static byte[] getFileData(final String filePath) {
        File file = new File(filePath);
        long fileSize = file.length();
        byte[] buffer = new byte[(int) fileSize];
        if (fileSize > Integer.MAX_VALUE) {
            LOG.error("file too big.");
            return buffer;
        }
        FileInputStream fi = null;
        try {
            fi = new FileInputStream(file);
            int offset = 0;
            int numRead = 0;
            while (offset < buffer.length && (numRead = fi.read(buffer, offset, buffer.length - offset)) >= 0) {
                offset += numRead;
            }
        } catch (IOException exception) {
            LOG.error("getFileData IOException error: " + exception.getMessage());
            return new byte[(int) fileSize];
        } finally {
            closeStream(fi);
        }
        return buffer;
    }

    /**
     * search filePath for special fileName
     *
     * @param fileName file name
     * @param directory dir path
     * @return filePath
     */
    public static Optional<String> searchFile(final String fileName, final String directory) {
        ArrayList<String> fileList = new ArrayList<>();
        getFileList(directory, fileList);
        for (String fileItem : fileList) {
            if (fileItem.endsWith(fileName)) {
                return Optional.ofNullable(fileItem);
            }
        }
        return Optional.empty();
    }

    /**
     * get file list in filePath
     *
     * @param filePath file path
     * @param fileList file path in arrayList
     */
    public static void getFileList(final String filePath, ArrayList<String> fileList) {
        File file = new File(filePath);
        if (!file.exists()) {
            LOG.error("getFileList: file is not exists.");
            return;
        }
        File[] files = file.listFiles();
        if (files == null) {
            LOG.error("getFileList: no file in this file path.");
            return;
        }
        for (File f : files) {
            try {
                if (f.isFile()) {
                    fileList.add(f.getCanonicalPath());
                } else if (f.isDirectory()) {
                    getFileList(f.getCanonicalPath(), fileList);
                } else {
                    LOG.error("It's not file or directory.");
                }
            } catch (IOException msg) {
                LOG.error("IOException error: " + msg.getMessage());
                return;
            }
        }
    }

    /**
     * get string from file
     *
     * @param filePath file path
     * @return String for file
     */
    public static Optional<String> getFileContent(final String filePath) {
        if (filePath.isEmpty()) {
            return Optional.empty();
        }
        BufferedReader reader = null;
        FileInputStream fileInputStream = null;
        InputStreamReader inputStreamReader = null;
        StringBuilder content = new StringBuilder();
        try {
            fileInputStream = new FileInputStream(filePath);
            inputStreamReader = new InputStreamReader(fileInputStream, "UTF-8");
            reader = new BufferedReader(inputStreamReader);
            String tempString;
            while ((tempString = reader.readLine()) != null) {
                content.append(tempString);
            }
        } catch (IOException msg) {
            LOG.error(PackingToolErrMsg.GET_FILE_CONTENT_FAILED.toString(
                    "IOException: " + msg.getMessage()));
            return Optional.empty();
        } finally {
            closeStream(reader);
            closeStream(inputStreamReader);
            closeStream(fileInputStream);
        }
        return Optional.ofNullable(content.toString());
    }

    /**
     * close file stream
     *
     * @param fileStream file stream
     */
    public static void closeStream(Closeable fileStream) {
        try {
            if (fileStream != null) {
                fileStream.close();
            }
        } catch (IOException msg) {
            LOG.error(PackingToolErrMsg.CLOSE_STREAM_EXCEPTION.toString(
                    "Close stream exist IOException: " + msg.getMessage()));
        }
    }

    /**
     * delete file
     *
     * @param path file path which will be deleted
     */
    public static void deleteFile(final String path) {
        File file = new File(path);
        if (file.exists()) {
            file.delete();
        }
    }

    /**
     * unzip hap package to path
     *
     * @param hapPath zip file
     * @param destDir path after unzip file
     */
    public static void unzip(final String hapPath, final String destDir) {
        File file = new File(destDir);
        if (!file.exists()) {
            file.mkdirs();
        }

        ZipFile zipFile = null;
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        FileOutputStream fos = null;
        try {
            zipFile = new ZipFile(hapPath);
            Enumeration<? extends ZipEntry> entries = zipFile.entries();
            int entriesNum = 0;
            while (entries.hasMoreElements()) {
                entriesNum++;
                ZipEntry entry = entries.nextElement();
                if (entry == null) {
                    continue;
                }
                String filePath = destDir + File.separator + entry.getName();
                if (!matchPattern(filePath)) {
                    LOG.error("Input invalid file: " + filePath);
                    throw new BundleException("Input invalid file " + filePath);
                }
                if (entry.isDirectory()) {
                    new File(filePath).mkdirs();
                    continue;
                }

                bis = new BufferedInputStream(zipFile.getInputStream(entry));
                File newFile = new File(filePath);
                File parent = newFile.getParentFile();
                if (parent != null && (!parent.exists())) {
                    parent.mkdirs();
                }

                fos = new FileOutputStream(newFile);
                bos = new BufferedOutputStream(fos, BUFFER_SIZE);

                int count;
                int total = 0;
                byte[] data = new byte[BUFFER_SIZE];
                while ((count = bis.read(data, 0, BUFFER_SIZE)) != -1) {
                    bos.write(data, 0, count);
                    total += count;
                }

                bos.flush();
                bos.close();
                fos.close();
                bis.close();
            }
        } catch (IOException | BundleException exception) {
            LOG.error("unzip file failed " + exception.getMessage());
        } finally {
            closeStream(bos);
            closeStream(fos);
            closeStream(bis);
            closeStream(zipFile);
        }
    }

    /**
     * delete directory
     *
     * @param directory dir path which will be deleted
     */
    public static void deleteDirectory(final String directory) {
        File dir = new File(directory);
        if (!dir.exists()) {
            return;
        }

        File[] fileList = dir.listFiles();
        if (fileList == null) {
            return;
        }
        for (File file : fileList) {
            try {
                if (file.isFile()) {
                    file.delete();
                } else if (file.isDirectory()) {
                    deleteDirectory(file.getCanonicalPath());
                } else {
                    LOG.error("It's not file or directory.");
                }
            } catch (IOException msg) {
                LOG.error("deleteDirectory IOException : " + msg.getMessage());
            }
        }
        dir.delete();
    }

    /**
     * format filepath
     *
     * @param filePath file path which is need to format
     * @return canonicalPath for filePath
     */
    public static Optional<String> getFormatedPath(final String filePath) {
        String absPath;
        try {
            File file = new File(filePath);
            absPath = file.getCanonicalPath();
        } catch (IOException msg) {
            LOG.error("format path IOException : " + msg.getMessage());
            return Optional.empty();
        }
        return Optional.of(absPath);
    }

    /**
     * check file whether is exist or not
     *
     * @param filePath bin file path
     * @return true: file is exist, false: file is not exist
     */
    public static boolean checkFileIsExists(final String filePath) {
        if (filePath.isEmpty()) {
            return false;
        }

        Optional<String> absFilePath = getFormatedPath(filePath);
        if (!absFilePath.isPresent()) {
            return false;
        }

        File file = new File(absFilePath.get());
        if (!file.exists()) {
            return false;
        }
        return true;
    }

    /**
     * copy a file to another place.
     *
     * @param sourceFile is the source file
     * @param destFile is the destination file
     * @throws IOException FileNotFoundException|IOException.
     */
    public static void copyFile(File sourceFile, File destFile) throws IOException, BundleException {
        if (sourceFile == null || destFile == null) {
            String errMsg = "CompressorFileUtil::copyFile input file is null.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        InputStream inputStream = null;
        OutputStream outputStream = null;
        try {
            inputStream = new FileInputStream(sourceFile);
            outputStream = new FileOutputStream(destFile);
            byte[] buffer = new byte[BUFFER_SIZE];
            int length;
            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }
        } finally {
            Utility.closeStream(inputStream);
            Utility.closeStream(outputStream);
        }
    }

    /**
     * make a directory if not exist.
     *
     * @param dirFile is the directory file
     * @throws IOException FileNotFoundException|IOException.
     */
    public static void makeDir(File dirFile) throws IOException, BundleException {
        if (dirFile == null) {
            String errMsg = "CompressorFileUtil::makeDir input file is null.";
            LOG.error(errMsg);
            throw new BundleException(errMsg);
        }
        dirFile.mkdirs();
    }

    /**
     * check json type code in haps.
     *
     * @param srcFile source file to zip
     * @return true is for successful and false is for failed
     * @throws BundleException FileNotFoundException|IOException.
     */
    public static String getJsonInZips(File srcFile, String jsonName) throws BundleException {
        String fileStr = srcFile.getPath();
        ZipFile zipFile = null;
        FileInputStream zipInput = null;
        ZipInputStream zin = null;
        InputStream inputStream = null;
        InputStreamReader reader = null;
        BufferedReader br = null;
        ZipEntry entry = null;
        StringBuilder jsonStr = new StringBuilder();
        try {
            zipFile = new ZipFile(srcFile);
            zipInput = new FileInputStream(fileStr);
            zin = new ZipInputStream(zipInput);
            while ((entry = zin.getNextEntry()) != null) {
                if (entry.getName().toLowerCase().equals(jsonName)) {
                    inputStream = zipFile.getInputStream(entry);
                    reader = new InputStreamReader(inputStream);
                    br = new BufferedReader(reader);
                    String line;
                    while ((line = br.readLine()) != null) {
                        jsonStr.append(line);
                    }
                    inputStream.close();
                }
            }
            jsonStr = new StringBuilder(jsonStr.toString().replaceAll("\r|\n|\t", ""));
        } catch (IOException exception) {
            LOG.error("Compressor::checkModuleTypeInHaps io exception: " + exception.getMessage());
            throw new BundleException("Compressor::checkModuleTypeInHaps failed.");
        } finally {
            Utility.closeStream(zipFile);
            Utility.closeStream(zipInput);
            Utility.closeStream(zin);
            Utility.closeStream(inputStream);
            Utility.closeStream(reader);
            Utility.closeStream(br);
        }
        return jsonStr.toString();
    }

    /**
     * get all resource file in profile.
     *
     * @param zipFile is the hap file
     * @throws BundleException when get profile json file failed
     */
    static HashMap<String, String> getProfileJson(ZipFile zipFile) throws BundleException {
        HashMap<String, String> resourceMap = new HashMap<>();
        try {
            final Enumeration<? extends ZipEntry> entries = zipFile.entries();
            while (entries.hasMoreElements()) {
                final ZipEntry entry = entries.nextElement();
                if (entry.getName().contains(RESOURCE_PATH)) {
                    String filePath = entry.getName();
                    String fileName = filePath.replaceAll(RESOURCE_PATH, "");
                    String fileContent = getFileStringFromZip(filePath, zipFile);
                    resourceMap.put(fileName, fileContent);
                }
            }
        } catch (IOException e) {
            LOG.error(PackingToolErrMsg.IO_EXCEPTION.toString("Get profile json exist IOExpection: " + e.getMessage()));
            throw new BundleException("Get profile json failed.");
        }
        return resourceMap;
    }

    /**
     * get file content in string from zip
     *
     * @param fileName is the file name we want to read
     * @param zipFile is the zip file
     */
    public static String getFileStringFromZip(String fileName, ZipFile zipFile)
            throws IOException {
        ZipEntry entry = zipFile.getEntry(fileName);
        if (entry == null) {
            LOG.debug("Uncompress::readStringFromFile " + fileName + " not found exception.");
            return "";
        }
        InputStream fileInputStream = null;
        BufferedReader bufferedReader = null;
        try {
            fileInputStream = zipFile.getInputStream(entry);
            bufferedReader = new BufferedReader(new InputStreamReader(fileInputStream, "UTF-8"));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line);
            }
            return sb.toString();
        } finally {
            Utility.closeStream(bufferedReader);
            Utility.closeStream(fileInputStream);
        }
    }

    /**
     * get sha-256 for file
     *
     * @param hapPath is the input path of file
     */
    public static String getSha256(String hapPath) {
        String sha256 = "";
        BufferedInputStream inputStream = null;
        try {
            File file = new File(hapPath);
            inputStream = new BufferedInputStream(new FileInputStream(file));
            byte[] buffer = new byte[SHA256_BUFFER_SIZE];
            MessageDigest md5 = MessageDigest.getInstance(SHA256);
            int size = -1;
            while ((size = inputStream.read(buffer)) != -1) {
                md5.update(buffer, 0, size);
            }
            sha256 = toHex(md5.digest());
        } catch (FileNotFoundException e) {
            LOG.error("input hap file is not found: " + e.getMessage());
        } catch (NoSuchAlgorithmException e) {
            LOG.error("can not provide sha-256 algorithm: " + e.getMessage());
        } catch (IOException e) {
            LOG.error("input hap IO exception: " + e.getMessage());
        } finally {
            Utility.closeStream(inputStream);
        }
        return sha256;
    }

    private static String toHex(byte[] data) {
        StringBuilder hexString = new StringBuilder();
        for (byte item : data) {
            hexString.append(Integer.toHexString(item & 0xFF));
        }
        return hexString.toString();
    }

    /**
     * unzip file
     *
     * @param zipFilePath is the zipFilePath
     * @param destDirPath is the output dest path
     */
    public static boolean unzipFile(String zipFilePath, String destDirPath) {
        boolean success = false;
        File destDir = new File(destDirPath);
        if (!destDir.exists()) {
            destDir.mkdirs();
        }
        ZipInputStream zipInputStream = null;
        try {
            zipInputStream = new ZipInputStream(new FileInputStream(zipFilePath));
            ZipEntry entry = zipInputStream.getNextEntry();
            while (entry != null) {
                String filePath = destDirPath + File.separator + entry.getName();
                if (!matchPattern(filePath)) {
                    LOG.error("Input invalid file " + filePath + ".");
                    return false;
                }
                if (!entry.isDirectory()) {
                    extractFile(zipInputStream, filePath);
                } else {
                    File dir = new File(filePath);
                    dir.mkdirs();
                }
                zipInputStream.closeEntry();
                entry = zipInputStream.getNextEntry();
            }
            success = true;
        } catch (IOException e) {
            LOG.error("FileUtil::unzipFile failed, IOException is " + e.getMessage());
        } finally {
            Utility.closeStream(zipInputStream);
        }
        return success;
    }

    private static void extractFile(ZipInputStream zipInputStream, String filePath) {
        BufferedOutputStream bufferedOutputStream = null;
        try {
            if (!matchPattern(filePath)) {
                LOG.error("input invalid file: " + filePath);
                throw new BundleException("input invalid file " + filePath + ".");
            }
            bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(filePath));
            byte[] bytes = new byte[BUFFER_SIZE];
            int readLength = 0;
            while ((readLength = zipInputStream.read(bytes)) != -1) {
                bufferedOutputStream.write(bytes, 0, readLength);
            }
        } catch (IOException | BundleException e) {
            LOG.error("FileUtil::extractFile failed, Exception is " + e.getMessage());
        } finally {
            Utility.closeStream(bufferedOutputStream);
        }
    }

    static boolean matchPattern(String path) {
        if (!Pattern.matches(PATTERN, path)) {
            LOG.error("input invalid file of " + path + ".");
            return false;
        }
        return true;
    }

    /**
     * getFileSize
     *
     * @param filePath is the input file path
     * @return file length
     */
    public static long getFileSize(String filePath) {
        File file = new File(filePath);
        if (file.exists() && file.isFile()) {
            return file.length();
        }
        String errMsg = "input " + filePath + " is not a valid file.";
        LOG.error(PackingToolErrMsg.GET_FILE_SIZE_FAILED.toString(errMsg));
        return 0;
    }

    /**
     * Create a parent directory for a file
     *
     * @param file file
     */
    public static void createParentDir(File file) {
        if (file != null && file.getParentFile() != null && !file.getParentFile().exists()) {
            file.getParentFile().mkdirs();
        }
    }

    /**
     * copy Stream
     *
     * @param inputStream input stream
     * @param outputStream out stream
     * @throws IOException IOException.
     */
    public static void copyStream(InputStream inputStream, OutputStream outputStream) throws IOException {
        byte[] buffer = new byte[BUFFER_SIZE];
        int bytesRead = 0;
        while ((bytesRead = inputStream.read(buffer)) != -1) {
            outputStream.write(buffer, 0, bytesRead);
        }
    }
}
