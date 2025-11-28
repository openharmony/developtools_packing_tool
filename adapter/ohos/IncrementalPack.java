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

package ohos;

import org.apache.commons.compress.archivers.zip.ZipArchiveEntry;
import org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Enumeration;
import java.util.Locale;
import java.util.UUID;

/**
 * IncrementalPack
 *
 * @since 2025-11-10
 */
public class IncrementalPack {
    private static final Log LOG = new Log(Compressor.class.toString());
    private static final String TEMP_EXIST_SRC_DIR = "tempExistSrcDir";

    /**
     * Determine if the current build is in incremental mode.
     *
     * @param utility common data
     * @return true if incremental mode; false otherwise
     */
    public static boolean isIncrementalMode(Utility utility) {
        String mode = utility.getMode();
        String existSrcPath = utility.getExistSrcPath();
        String isLibPathRetain = utility.getLibPathRetain();

        if (existSrcPath.isEmpty() || !"true".equals(isLibPathRetain)) {
            return false;
        }

        File srcFile = new File(existSrcPath);
        if (!srcFile.isFile()) {
            return false;
        }

        String name = srcFile.getName().toLowerCase(Locale.ENGLISH);
        boolean isHapOrHsp = name.endsWith(Constants.HAP_SUFFIX) || name.endsWith(Constants.HSP_SUFFIX);
        boolean isModeValid = Utility.MODE_HAP.equals(mode) || Utility.MODE_HSP.equals(mode);

        return isHapOrHsp && isModeValid;
    }

    /**
     * A zero-copy, no-decompression, no-recompression ZIP entry fast-copy interface.
     *
     * @param prevZip source zip file (--exist-src-path)
     * @param zos     target ZipArchiveOutputStream
     * @throws BundleException if no .so entries found
     * @throws IOException     IO errors
     */
    public static void addRawArchiveEntry(org.apache.commons.compress.archivers.zip.ZipFile prevZip,
                                          ZipArchiveOutputStream zos) throws BundleException, IOException {
        Enumeration<ZipArchiveEntry> entries = prevZip.getEntries();
        while (entries.hasMoreElements()) {
            ZipArchiveEntry entry = entries.nextElement();
            String entryName = entry.getName();

            if (entryName.startsWith("libs/")) {
                ZipArchiveEntry newEntry = new ZipArchiveEntry(entryName);
                newEntry.setMethod(entry.getMethod());
                newEntry.setSize(entry.getSize());
                newEntry.setCompressedSize(entry.getCompressedSize());
                newEntry.setCrc(entry.getCrc());

                try (InputStream rawIn = prevZip.getRawInputStream(entry)) {
                    zos.addRawArchiveEntry(newEntry, rawIn);
                }
            }
        }
    }

    /**
     * If --out-path and --exist-src-path are the same,copy the file from --exist-src-path to a temporary directory.
     *
     * @param utility common data
     * @return tempParentDir root path for newly created temporary directories.
     */
    public static String copyExistSrcFile(Utility utility) throws BundleException {
        if (!IncrementalPack.isIncrementalMode(utility)) {
            return "";
        }

        String existSrcPathStr = utility.getExistSrcPath();
        String outPathStr = utility.getOutPath();

        if (!existSrcPathStr.equals(outPathStr)) {
            return "";
        }

        Path existSrcPath = Paths.get(existSrcPathStr);
        if (!Files.exists(existSrcPath) || !Files.isRegularFile(existSrcPath)) {
            String errMsg = "Parse --exist-src-path file does not exist, parse path: " + existSrcPathStr;
            LOG.error(PackingToolErrMsg.FILE_NOT_EXIST.toString(errMsg));
            throw new BundleException("Copy file to temp dir failed: --exist-src-path file not found.");
        }

        Path oldFileParent = existSrcPath.getParent();
        Path tempParentDir = oldFileParent.resolve(TEMP_EXIST_SRC_DIR);
        Path tempDir = tempParentDir.resolve(UUID.randomUUID().toString());

        try {
            Files.createDirectories(tempDir);
            Path tempFilePath = tempDir.resolve(existSrcPath.getFileName());
            Files.copy(existSrcPath, tempFilePath, StandardCopyOption.REPLACE_EXISTING);
            utility.setExistSrcPath(tempFilePath.toString());
        } catch (IOException e) {
            LOG.error(PackingToolErrMsg.IO_EXCEPTION.toString(
                    "Copy file to tempExistSrc dir failed: " + e.getMessage()));
            throw new BundleException("Copy file to tempExistSrc dir failed.");
        }

        return tempParentDir.toString();
    }

    /**
     * delete --exist-src-path temporary directory.
     *
     * @param dir path for delete
     */
    public static void deleteExistSrcFile(String dir) {
        FileUtils.deleteDirectory(dir);
    }
}
