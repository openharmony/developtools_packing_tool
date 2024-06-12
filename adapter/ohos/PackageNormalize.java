/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.serializer.SerializerFeature;
import org.apache.commons.compress.utils.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.Enumeration;
import java.util.zip.CRC32;
import java.util.zip.Deflater;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

/**
 * PackageNormalize, normalize HSP bundleName and versionCode
 *
 * @since 2024-04-06
 */
public class PackageNormalize {
    private static final Log LOG = new Log(PackageNormalize.class.toString());
    private static final int BUFFER_SIZE = 10 * 1024;
    private static final String MODULE_JSON = "module.json";
    private static final String PACK_INFO = "pack.info";
    private static final String APP = "app";
    private static final String SUMMARY = "summary";
    private static final String VERSION = "version";
    private static final String CODE = "code";
    private static final String BUNDLE_NAME = "bundleName";
    private static final String VERSION_CODE = "versionCode";
    private static final String TMP = "tmp";

    /**
     * normalize HSP bundleName and versionCode
     *
     * @param utility common data
     * @return true if normalize succeed
     */
    public static boolean normalize(Utility utility) {
        Path outPath = Paths.get(utility.getOutPath());
        for (String hspPath : utility.getFormattedHspPathList()) {
            try {
                normalize(Paths.get(hspPath), outPath, utility.getBundleName(), utility.getVersionCode());
            } catch (BundleException ex) {
                LOG.error("PackageNormalize::normalize BundleException: " + ex.getMessage());
                return false;
            } catch (IOException ex) {
                LOG.error("PackageNormalize::normalize IOException: " + ex.getMessage());
                return false;
            }
        }
        return true;
    }

    private static void normalize(Path hspPath, Path outPath, String bundleName, int versionCode)
            throws BundleException, IOException {
        Path outHspPath = outPath.resolve(hspPath.getFileName());
        Path tmpDir = Files.createTempDirectory(outPath, TMP);
        Path moduleJson = Files.createFile(tmpDir.resolve(MODULE_JSON));
        Path packInfo = Files.createFile(tmpDir.resolve(PACK_INFO));
        try (ZipFile hspFile = new ZipFile(hspPath.toFile());
             OutputStream output = Files.newOutputStream(outHspPath);
             ZipOutputStream zipOut = new ZipOutputStream(output)) {
            // 1.unzip module.json and pack.info, copy to tmp
            ZipEntry moduleEntry = hspFile.getEntry(MODULE_JSON);
            if (moduleEntry != null) {
                try (OutputStream out = Files.newOutputStream(moduleJson)) {
                    IOUtils.copy(hspFile.getInputStream(moduleEntry), out);
                }
            }
            ZipEntry packEntry = hspFile.getEntry(PACK_INFO);
            if (moduleEntry != null) {
                try (OutputStream out = Files.newOutputStream(packInfo)) {
                    IOUtils.copy(hspFile.getInputStream(packEntry), out);
                }
            }

            // 2.update bundleName, versionCode to module.json, pack.info
            updateModuleJson(moduleJson, bundleName, versionCode);
            updatePackInfo(packInfo, bundleName, versionCode);

            // 3.zip hsp, module.json, pack.info to outPath
            copyHsp(hspFile, moduleJson, packInfo, zipOut);
        } finally {
            rmdir(tmpDir);
        }
    }

    private static void rmdir(Path dir) throws IOException {
        Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {
            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult postVisitDirectory(Path dir, IOException ex) throws IOException {
                Files.delete(dir);
                return FileVisitResult.CONTINUE;
            }
        });
    }

    private static void updateModuleJson(Path moduleJson, String bundleName, int versionCode)
            throws BundleException, IOException {
        try (FileInputStream input = new FileInputStream(moduleJson.toFile())) {
            JSONObject jsonObject = JSON.parseObject(input, JSONObject.class);
            if (jsonObject == null) {
                LOG.error("updateModuleJson failed, parse json is null.");
                throw new BundleException("updateModuleJson failed, parse json is null.");
            }
            if (!jsonObject.containsKey(APP)) {
                LOG.error("updateModuleJson failed, json format not invalid.");
                throw new BundleException("updateModuleJson failed, json format invalid.");
            }
            JSONObject appObject = jsonObject.getJSONObject(APP);
            appObject.put(BUNDLE_NAME, bundleName);
            appObject.put(VERSION_CODE, versionCode);
            Files.write(moduleJson, JSON.toJSONBytes(jsonObject, SerializerFeature.WriteMapNullValue,
                            SerializerFeature.WriteDateUseDateFormat, SerializerFeature.SortField),
                    StandardOpenOption.TRUNCATE_EXISTING, StandardOpenOption.CREATE);
        }
    }

    private static void updatePackInfo(Path packInfo, String bundleName, int versionCode)
            throws BundleException, IOException {
        try (FileInputStream input = new FileInputStream(packInfo.toFile())) {
            JSONObject jsonObject = JSON.parseObject(input, JSONObject.class);
            if (jsonObject == null) {
                LOG.error("updatePackInfo failed, json format invalid.");
                throw new BundleException("updatePackInfo failed, json format invalid.");
            }
            JSONObject summaryObject = jsonObject.getJSONObject(SUMMARY);
            if (summaryObject == null) {
                LOG.error("updatePackInfo failed, json format invalid.");
                throw new BundleException("updatePackInfo failed, json format invalid.");
            }
            JSONObject appObject = summaryObject.getJSONObject(APP);
            if (appObject == null) {
                LOG.error("updatePackInfo failed, json format invalid.");
                throw new BundleException("updatePackInfo failed, json format invalid.");
            }
            appObject.put(BUNDLE_NAME, bundleName);
            JSONObject versionObject = appObject.getJSONObject(VERSION);
            if (versionObject == null) {
                LOG.error("updatePackInfo failed, json format invalid.");
                throw new BundleException("updatePackInfo failed, json format invalid.");
            }
            versionObject.put(CODE, versionCode);
            Files.write(packInfo, JSON.toJSONBytes(jsonObject, SerializerFeature.WriteMapNullValue,
                            SerializerFeature.WriteDateUseDateFormat, SerializerFeature.SortField),
                    StandardOpenOption.TRUNCATE_EXISTING, StandardOpenOption.CREATE);
        }
    }

    private static void copyHsp(ZipFile hspFile, Path moduleJson, Path packInfo, ZipOutputStream zipOut)
            throws BundleException, IOException {
        zipOut.setLevel(Deflater.BEST_SPEED);
        boolean isStored = true;
        Enumeration<? extends ZipEntry> entries = hspFile.entries();
        while (entries.hasMoreElements()) {
            ZipEntry zipEntry = entries.nextElement();
            if (MODULE_JSON.equals(zipEntry.getName()) || PACK_INFO.equals(zipEntry.getName())) {
                isStored = zipEntry.getMethod() == ZipEntry.STORED;
                continue;
            }
            ZipEntry newEntry = zipEntry.getMethod() ==
                    ZipEntry.STORED ? new ZipEntry(zipEntry) : new ZipEntry(zipEntry.getName());
            zipOut.putNextEntry(newEntry);
            if (!zipEntry.isDirectory()) {
                IOUtils.copy(hspFile.getInputStream(zipEntry), zipOut);
            }
            zipOut.closeEntry();
        }
        compressFile(moduleJson, isStored, MODULE_JSON, zipOut);
        compressFile(packInfo, isStored, PACK_INFO, zipOut);
    }

    private static void compressFile(Path file, boolean isStored, String entryName, ZipOutputStream zipOut)
            throws BundleException, IOException {
        try (InputStream input = Files.newInputStream(file)) {
            if (isStored) {
                ZipEntry newEntry = new ZipEntry(entryName);
                newEntry.setMethod(ZipEntry.STORED);
                File jsonFile = file.toFile();
                newEntry.setCompressedSize(jsonFile.length());
                CRC32 crc = getCrcFromFile(jsonFile);
                newEntry.setCrc(crc.getValue());
                zipOut.putNextEntry(newEntry);
            } else {
                ZipEntry newEntry = new ZipEntry(entryName);
                newEntry.setMethod(ZipEntry.DEFLATED);
                zipOut.putNextEntry(newEntry);
            }
            IOUtils.copy(input, zipOut);
            zipOut.closeEntry();
        }
    }

    private static CRC32 getCrcFromFile(File file) throws BundleException {
        CRC32 crc = new CRC32();
        try (FileInputStream fileInputStream = new FileInputStream(file)) {
            byte[] buffer = new byte[BUFFER_SIZE];
            int count = fileInputStream.read(buffer);
            while (count > 0) {
                crc.update(buffer, 0, count);
                count = fileInputStream.read(buffer);
            }
        } catch (FileNotFoundException ignored) {
            LOG.error("PackageNormalize::getCrcFromFile file not found exception: " + ignored.getMessage());
            throw new BundleException("Get Crc from file failed.");
        } catch (IOException exception) {
            LOG.error("PackageNormalize::getCrcFromFile io exception: " + exception.getMessage());
            throw new BundleException("Get Crc from file failed, io exception.");
        }
        return crc;
    }
}
