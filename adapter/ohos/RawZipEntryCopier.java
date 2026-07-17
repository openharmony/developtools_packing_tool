/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.RandomAccessFile;

import java.nio.charset.StandardCharsets;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Copies ZIP entries without decompressing them.
 */
final class RawZipEntryCopier {
    private static final String LIBS = "libs";
    private static final int ZIP_LOCAL_FILE_HEADER_SIGNATURE = 0x04034b50;
    private static final int ZIP_CENTRAL_DIRECTORY_SIGNATURE = 0x02014b50;
    private static final int ZIP_END_OF_CENTRAL_DIRECTORY_SIGNATURE = 0x06054b50;
    private static final int ZIP64_END_OF_CENTRAL_DIRECTORY_SIGNATURE = 0x06064b50;
    private static final int ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIGNATURE = 0x07064b50;
    private static final int ZIP64_EXTRA_FIELD_HEADER_ID = 0x0001;
    private static final int ZIP_LOCAL_FILE_HEADER_SIZE = 30;
    private static final int ZIP_CENTRAL_DIRECTORY_HEADER_SIZE = 46;
    private static final int ZIP_END_OF_CENTRAL_DIRECTORY_SIZE = 22;
    private static final int ZIP64_END_OF_CENTRAL_DIRECTORY_SIZE = 56;
    private static final int ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIZE = 20;
    private static final int ZIP_MAX_COMMENT_SIZE = 0xFFFF;
    private static final int ZIP_USHORT_MAX = 0xFFFF;
    private static final long ZIP_UINT_MAX = 0xFFFFFFFFL;
    private static final int COPY_BUFFER_SIZE = 16 * 1024;

    private RawZipEntryCopier() {
    }

    static List<Entry> readEntries(String srcPath) throws IOException {
        try (RandomAccessFile zip = new RandomAccessFile(srcPath, "r")) {
            EndOfCentralDirectory eocd = readEndOfCentralDirectory(zip);
            List<Entry> entries = new ArrayList<>();
            long centralOffset = eocd.centralDirectoryOffset;
            for (int i = 0; i < eocd.entryCount; i++) {
                zip.seek(centralOffset);
                byte[] fixedHeader = new byte[ZIP_CENTRAL_DIRECTORY_HEADER_SIZE];
                zip.readFully(fixedHeader);
                if (getInt(fixedHeader, 0) != ZIP_CENTRAL_DIRECTORY_SIGNATURE) {
                    throw new IOException("Invalid central directory header");
                }
                int nameLength = getShort(fixedHeader, 28);
                int extraLength = getShort(fixedHeader, 30);
                int commentLength = getShort(fixedHeader, 32);
                int centralEntryLength = ZIP_CENTRAL_DIRECTORY_HEADER_SIZE + nameLength + extraLength
                        + commentLength;
                byte[] centralDirectoryBytes = new byte[centralEntryLength];
                System.arraycopy(fixedHeader, 0, centralDirectoryBytes, 0, fixedHeader.length);
                zip.readFully(centralDirectoryBytes, ZIP_CENTRAL_DIRECTORY_HEADER_SIZE,
                        centralEntryLength - ZIP_CENTRAL_DIRECTORY_HEADER_SIZE);
                byte[] entryNameBytes = Arrays.copyOfRange(centralDirectoryBytes, ZIP_CENTRAL_DIRECTORY_HEADER_SIZE,
                        ZIP_CENTRAL_DIRECTORY_HEADER_SIZE + nameLength);
                long localHeaderOffset = getCentralDirectoryLocalHeaderOffset(centralDirectoryBytes);
                entries.add(new Entry(getCpuAbiFromLibEntryNameBytes(entryNameBytes), localHeaderOffset,
                        centralDirectoryBytes, eocd.comment));
                centralOffset += centralEntryLength;
            }
            long centralEnd = eocd.centralDirectoryOffset + eocd.centralDirectorySize;
            if (centralOffset != centralEnd) {
                throw new IOException("Central directory size mismatch");
            }
            fillLocalSegmentLengths(zip, entries, eocd.centralDirectoryOffset);
            return entries;
        }
    }

    static void copyEntries(String srcPath, List<Entry> sourceEntries, File targetZipFile, String cpuAbi)
            throws IOException {
        List<Entry> copiedEntries = new ArrayList<>();
        try (RandomAccessFile sourceZip = new RandomAccessFile(srcPath, "r");
             FileOutputStream targetZip = new FileOutputStream(targetZipFile)) {
            for (Entry entry : sourceEntries) {
                if (!entry.cpuAbi.isEmpty() && !cpuAbi.equals(entry.cpuAbi)) {
                    continue;
                }
                long newLocalHeaderOffset = targetZip.getChannel().position();
                copyRange(sourceZip, targetZip, entry.localHeaderOffset, entry.localSegmentLength);
                copiedEntries.add(new Entry(entry.cpuAbi, newLocalHeaderOffset, entry.centralDirectoryBytes,
                        entry.archiveComment));
            }
            writeCentralDirectory(targetZip, copiedEntries);
        }
    }

    private static String getCpuAbiFromLibEntryNameBytes(byte[] entryNameBytes) {
        byte[] libsPrefix = new byte[] {'l', 'i', 'b', 's'};
        if (entryNameBytes.length <= libsPrefix.length || !startsWith(entryNameBytes, libsPrefix)
                || !isZipPathSeparator(entryNameBytes[libsPrefix.length])) {
            return "";
        }
        int start = libsPrefix.length + 1;
        int end = start;
        while (end < entryNameBytes.length && !isZipPathSeparator(entryNameBytes[end])) {
            end++;
        }
        if (end == start) {
            return "";
        }
        return new String(entryNameBytes, start, end - start, StandardCharsets.UTF_8);
    }

    private static boolean startsWith(byte[] data, byte[] prefix) {
        if (data.length < prefix.length) {
            return false;
        }
        for (int i = 0; i < prefix.length; i++) {
            if (data[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }

    private static boolean isZipPathSeparator(byte value) {
        return value == '/' || value == '\\';
    }

    private static EndOfCentralDirectory readEndOfCentralDirectory(RandomAccessFile zip) throws IOException {
        long fileLength = zip.length();
        int searchLength = (int) Math.min(fileLength, ZIP_END_OF_CENTRAL_DIRECTORY_SIZE + ZIP_MAX_COMMENT_SIZE);
        byte[] tail = new byte[searchLength];
        zip.seek(fileLength - searchLength);
        zip.readFully(tail);
        for (int i = searchLength - ZIP_END_OF_CENTRAL_DIRECTORY_SIZE; i >= 0; i--) {
            if (getInt(tail, i) != ZIP_END_OF_CENTRAL_DIRECTORY_SIGNATURE) {
                continue;
            }
            int commentLength = getShort(tail, i + 20);
            if (i + ZIP_END_OF_CENTRAL_DIRECTORY_SIZE + commentLength != searchLength) {
                continue;
            }
            byte[] comment = Arrays.copyOfRange(tail, i + ZIP_END_OF_CENTRAL_DIRECTORY_SIZE,
                    i + ZIP_END_OF_CENTRAL_DIRECTORY_SIZE + commentLength);
            int diskNumber = getShort(tail, i + 4);
            int centralDirectoryDisk = getShort(tail, i + 6);
            int entryCountOnDisk = getShort(tail, i + 8);
            int entryCount = getShort(tail, i + 10);
            long centralDirectorySize = getUnsignedInt(tail, i + 12);
            long centralDirectoryOffset = getUnsignedInt(tail, i + 16);
            boolean needsZip64 = diskNumber == ZIP_USHORT_MAX || centralDirectoryDisk == ZIP_USHORT_MAX
                    || entryCountOnDisk == ZIP_USHORT_MAX || entryCount == ZIP_USHORT_MAX
                    || centralDirectorySize == ZIP_UINT_MAX || centralDirectoryOffset == ZIP_UINT_MAX;
            if (!needsZip64) {
                if (diskNumber != 0 || centralDirectoryDisk != 0 || entryCountOnDisk != entryCount) {
                    throw new IOException("Multi-disk ZIP is not supported");
                }
                return new EndOfCentralDirectory(entryCount, centralDirectorySize, centralDirectoryOffset, comment);
            }
            long eocdOffset = fileLength - searchLength + i;
            return readZip64EndOfCentralDirectory(zip, eocdOffset, comment);
        }
        throw new IOException("ZIP end of central directory not found");
    }

    private static EndOfCentralDirectory readZip64EndOfCentralDirectory(RandomAccessFile zip, long eocdOffset,
                                                                        byte[] comment) throws IOException {
        long locatorOffset = eocdOffset - ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIZE;
        if (locatorOffset < 0) {
            throw new IOException("ZIP64 end of central directory locator not found");
        }
        zip.seek(locatorOffset);
        byte[] locator = new byte[ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIZE];
        zip.readFully(locator);
        if (getInt(locator, 0) != ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIGNATURE) {
            throw new IOException("ZIP64 end of central directory locator not found");
        }
        if (getInt(locator, 4) != 0 || getInt(locator, 16) != 1) {
            throw new IOException("Multi-disk ZIP is not supported");
        }
        long zip64EocdOffset = getLong(locator, 8);
        zip.seek(zip64EocdOffset);
        byte[] zip64Eocd = new byte[ZIP64_END_OF_CENTRAL_DIRECTORY_SIZE];
        zip.readFully(zip64Eocd);
        if (getInt(zip64Eocd, 0) != ZIP64_END_OF_CENTRAL_DIRECTORY_SIGNATURE) {
            throw new IOException("Invalid ZIP64 end of central directory");
        }
        if (getInt(zip64Eocd, 16) != 0 || getInt(zip64Eocd, 20) != 0) {
            throw new IOException("Multi-disk ZIP is not supported");
        }
        long entryCountOnDisk = getLong(zip64Eocd, 24);
        long entryCount = getLong(zip64Eocd, 32);
        if (entryCountOnDisk != entryCount || entryCount > Integer.MAX_VALUE) {
            throw new IOException("Unsupported ZIP64 entry count");
        }
        return new EndOfCentralDirectory((int) entryCount, getLong(zip64Eocd, 40), getLong(zip64Eocd, 48), comment);
    }

    private static long getCentralDirectoryLocalHeaderOffset(byte[] centralDirectoryBytes) throws IOException {
        long localHeaderOffset = getUnsignedInt(centralDirectoryBytes, 42);
        if (localHeaderOffset != ZIP_UINT_MAX) {
            return localHeaderOffset;
        }
        Zip64ExtraInfo zip64ExtraInfo = findZip64ExtraInfo(centralDirectoryBytes);
        if (!zip64ExtraInfo.hasOffset || zip64ExtraInfo.offsetValueOffset < 0) {
            throw new IOException("ZIP64 entry offset not found");
        }
        return getLong(centralDirectoryBytes, zip64ExtraInfo.offsetValueOffset);
    }

    private static Zip64ExtraInfo findZip64ExtraInfo(byte[] centralDirectoryBytes) throws IOException {
        int nameLength = getShort(centralDirectoryBytes, 28);
        int extraLength = getShort(centralDirectoryBytes, 30);
        int extraStart = ZIP_CENTRAL_DIRECTORY_HEADER_SIZE + nameLength;
        int extraEnd = extraStart + extraLength;
        int offset = extraStart;
        while (offset + 4 <= extraEnd) {
            int headerId = getShort(centralDirectoryBytes, offset);
            int dataSize = getShort(centralDirectoryBytes, offset + 2);
            int dataStart = offset + 4;
            int dataEnd = dataStart + dataSize;
            if (dataEnd > extraEnd) {
                throw new IOException("Invalid ZIP extra field");
            }
            if (headerId == ZIP64_EXTRA_FIELD_HEADER_ID) {
                int valueOffset = dataStart;
                if (getUnsignedInt(centralDirectoryBytes, 24) == ZIP_UINT_MAX) {
                    valueOffset += Long.BYTES;
                }
                if (getUnsignedInt(centralDirectoryBytes, 20) == ZIP_UINT_MAX) {
                    valueOffset += Long.BYTES;
                }
                boolean offsetInZip64 = getUnsignedInt(centralDirectoryBytes, 42) == ZIP_UINT_MAX;
                boolean hasOffset = offsetInZip64 && valueOffset + Long.BYTES <= dataEnd;
                return new Zip64ExtraInfo(offset, dataSize, valueOffset, hasOffset);
            }
            offset = dataEnd;
        }
        if (offset != extraEnd) {
            throw new IOException("Invalid ZIP extra field");
        }
        return new Zip64ExtraInfo(-1, 0, -1, false);
    }

    private static void fillLocalSegmentLengths(RandomAccessFile zip, List<Entry> entries, long centralOffset)
            throws IOException {
        List<Entry> sortedEntries = new ArrayList<>(entries);
        sortedEntries.sort((left, right) -> Long.compare(left.localHeaderOffset, right.localHeaderOffset));
        for (int i = 0; i < sortedEntries.size(); i++) {
            Entry entry = sortedEntries.get(i);
            zip.seek(entry.localHeaderOffset);
            byte[] localHeader = new byte[ZIP_LOCAL_FILE_HEADER_SIZE];
            zip.readFully(localHeader);
            if (getInt(localHeader, 0) != ZIP_LOCAL_FILE_HEADER_SIGNATURE) {
                throw new IOException("Invalid local file header");
            }
            long nextOffset = (i + 1 < sortedEntries.size())
                    ? sortedEntries.get(i + 1).localHeaderOffset : centralOffset;
            if (nextOffset <= entry.localHeaderOffset) {
                throw new IOException("Invalid ZIP entry order or overlapping entry");
            }
            entry.localSegmentLength = nextOffset - entry.localHeaderOffset;
        }
    }

    private static void copyRange(RandomAccessFile source, OutputStream target, long offset, long length)
            throws IOException {
        byte[] buffer = new byte[COPY_BUFFER_SIZE];
        source.seek(offset);
        long remaining = length;
        while (remaining > 0) {
            int readLength = (int) Math.min(buffer.length, remaining);
            int read = source.read(buffer, 0, readLength);
            if (read < 0) {
                throw new IOException("Unexpected end of ZIP entry");
            }
            target.write(buffer, 0, read);
            remaining -= read;
        }
    }

    private static void writeCentralDirectory(FileOutputStream targetZip, List<Entry> copiedEntries)
            throws IOException {
        long centralDirectoryOffset = targetZip.getChannel().position();
        for (Entry entry : copiedEntries) {
            targetZip.write(prepareCentralDirectoryBytes(entry));
        }
        long centralDirectorySize = targetZip.getChannel().position() - centralDirectoryOffset;
        writeEndOfCentralDirectory(targetZip, copiedEntries.size(), centralDirectorySize, centralDirectoryOffset,
                copiedEntries.isEmpty() ? new byte[0] : copiedEntries.get(0).archiveComment);
    }

    private static byte[] prepareCentralDirectoryBytes(Entry entry) throws IOException {
        byte[] centralDirectoryBytes = Arrays.copyOf(entry.centralDirectoryBytes, entry.centralDirectoryBytes.length);
        boolean needsZip64Offset = entry.localHeaderOffset >= ZIP_UINT_MAX
                || getUnsignedInt(centralDirectoryBytes, 42) == ZIP_UINT_MAX;
        if (!needsZip64Offset) {
            setInt(centralDirectoryBytes, 42, entry.localHeaderOffset);
            return centralDirectoryBytes;
        }
        setInt(centralDirectoryBytes, 42, ZIP_UINT_MAX);
        return writeZip64LocalHeaderOffset(centralDirectoryBytes, entry.localHeaderOffset);
    }

    private static byte[] writeZip64LocalHeaderOffset(byte[] centralDirectoryBytes, long localHeaderOffset)
            throws IOException {
        Zip64ExtraInfo zip64ExtraInfo = findZip64ExtraInfo(centralDirectoryBytes);
        if (zip64ExtraInfo.hasOffset && zip64ExtraInfo.offsetValueOffset >= 0) {
            setLong(centralDirectoryBytes, zip64ExtraInfo.offsetValueOffset, localHeaderOffset);
            return centralDirectoryBytes;
        }
        return appendZip64LocalHeaderOffset(centralDirectoryBytes, zip64ExtraInfo, localHeaderOffset);
    }

    private static byte[] appendZip64LocalHeaderOffset(byte[] centralDirectoryBytes, Zip64ExtraInfo zip64ExtraInfo,
                                                       long localHeaderOffset) throws IOException {
        int nameLength = getShort(centralDirectoryBytes, 28);
        int extraLength = getShort(centralDirectoryBytes, 30);
        int commentLength = getShort(centralDirectoryBytes, 32);
        if (extraLength + 4 + Long.BYTES > ZIP_USHORT_MAX) {
            throw new IOException("ZIP extra field is too large");
        }
        int extraStart = ZIP_CENTRAL_DIRECTORY_HEADER_SIZE + nameLength;
        int commentStart = extraStart + extraLength;
        ByteArrayOutputStream output = new ByteArrayOutputStream(centralDirectoryBytes.length + 12);
        if (zip64ExtraInfo.headerOffset >= 0) {
            int dataEnd = zip64ExtraInfo.headerOffset + 4 + zip64ExtraInfo.dataSize;
            if (extraLength + Long.BYTES > ZIP_USHORT_MAX) {
                throw new IOException("ZIP extra field is too large");
            }
            output.write(centralDirectoryBytes, 0, dataEnd);
            writeLong(output, localHeaderOffset);
            output.write(centralDirectoryBytes, dataEnd, centralDirectoryBytes.length - dataEnd);
            setShortInOutput(output, 30, extraLength + Long.BYTES);
            setShortInOutput(output, zip64ExtraInfo.headerOffset + 2, zip64ExtraInfo.dataSize + Long.BYTES);
        } else {
            output.write(centralDirectoryBytes, 0, commentStart);
            writeShort(output, ZIP64_EXTRA_FIELD_HEADER_ID);
            writeShort(output, Long.BYTES);
            writeLong(output, localHeaderOffset);
            output.write(centralDirectoryBytes, commentStart, commentLength);
            setShortInOutput(output, 30, extraLength + 4 + Long.BYTES);
        }
        return output.toByteArray();
    }

    private static void writeEndOfCentralDirectory(FileOutputStream targetZip, int entryCount,
                                                   long centralDirectorySize, long centralDirectoryOffset,
                                                   byte[] comment) throws IOException {
        boolean needsZip64 = entryCount >= ZIP_USHORT_MAX || centralDirectorySize >= ZIP_UINT_MAX
                || centralDirectoryOffset >= ZIP_UINT_MAX;
        if (needsZip64) {
            long zip64EocdOffset = targetZip.getChannel().position();
            writeZip64EndOfCentralDirectory(targetZip, entryCount, centralDirectorySize, centralDirectoryOffset);
            writeZip64EndOfCentralDirectoryLocator(targetZip, zip64EocdOffset);
        }
        byte[] eocd = new byte[ZIP_END_OF_CENTRAL_DIRECTORY_SIZE + comment.length];
        setInt(eocd, 0, ZIP_END_OF_CENTRAL_DIRECTORY_SIGNATURE);
        setShort(eocd, 8, needsZip64 ? ZIP_USHORT_MAX : entryCount);
        setShort(eocd, 10, needsZip64 ? ZIP_USHORT_MAX : entryCount);
        setInt(eocd, 12, needsZip64 ? ZIP_UINT_MAX : centralDirectorySize);
        setInt(eocd, 16, needsZip64 ? ZIP_UINT_MAX : centralDirectoryOffset);
        setShort(eocd, 20, comment.length);
        System.arraycopy(comment, 0, eocd, ZIP_END_OF_CENTRAL_DIRECTORY_SIZE, comment.length);
        targetZip.write(eocd);
    }

    private static void writeZip64EndOfCentralDirectory(OutputStream targetZip, int entryCount,
                                                        long centralDirectorySize, long centralDirectoryOffset)
            throws IOException {
        byte[] zip64Eocd = new byte[ZIP64_END_OF_CENTRAL_DIRECTORY_SIZE];
        setInt(zip64Eocd, 0, ZIP64_END_OF_CENTRAL_DIRECTORY_SIGNATURE);
        setLong(zip64Eocd, 4, ZIP64_END_OF_CENTRAL_DIRECTORY_SIZE - Long.BYTES - Integer.BYTES);
        setShort(zip64Eocd, 12, 45);
        setShort(zip64Eocd, 14, 45);
        setLong(zip64Eocd, 24, entryCount);
        setLong(zip64Eocd, 32, entryCount);
        setLong(zip64Eocd, 40, centralDirectorySize);
        setLong(zip64Eocd, 48, centralDirectoryOffset);
        targetZip.write(zip64Eocd);
    }

    private static void writeZip64EndOfCentralDirectoryLocator(OutputStream targetZip, long zip64EocdOffset)
            throws IOException {
        byte[] locator = new byte[ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIZE];
        setInt(locator, 0, ZIP64_END_OF_CENTRAL_DIRECTORY_LOCATOR_SIGNATURE);
        setLong(locator, 8, zip64EocdOffset);
        setInt(locator, 16, 1);
        targetZip.write(locator);
    }

    private static int getShort(byte[] data, int offset) {
        return (data[offset] & 0xFF) | ((data[offset + 1] & 0xFF) << 8);
    }

    private static int getInt(byte[] data, int offset) {
        return getShort(data, offset) | (getShort(data, offset + 2) << 16);
    }

    private static long getUnsignedInt(byte[] data, int offset) {
        return getInt(data, offset) & ZIP_UINT_MAX;
    }

    private static long getLong(byte[] data, int offset) {
        return getUnsignedInt(data, offset) | (getUnsignedInt(data, offset + Integer.BYTES) << 32);
    }

    private static void setShort(byte[] data, int offset, long value) {
        data[offset] = (byte) (value & 0xFF);
        data[offset + 1] = (byte) ((value >>> 8) & 0xFF);
    }

    private static void setInt(byte[] data, int offset, long value) {
        setShort(data, offset, value);
        setShort(data, offset + 2, value >>> 16);
    }

    private static void setLong(byte[] data, int offset, long value) {
        setInt(data, offset, value);
        setInt(data, offset + Integer.BYTES, value >>> 32);
    }

    private static void writeShort(OutputStream output, long value) throws IOException {
        output.write((byte) (value & 0xFF));
        output.write((byte) ((value >>> 8) & 0xFF));
    }

    private static void writeLong(OutputStream output, long value) throws IOException {
        for (int i = 0; i < Long.BYTES; i++) {
            output.write((byte) ((value >>> (8 * i)) & 0xFF));
        }
    }

    private static void setShortInOutput(ByteArrayOutputStream output, int offset, long value) {
        byte[] data = output.toByteArray();
        setShort(data, offset, value);
        output.reset();
        output.write(data, 0, data.length);
    }

    static final class Entry {
        final String cpuAbi;
        private final long localHeaderOffset;
        private final byte[] centralDirectoryBytes;
        private final byte[] archiveComment;
        private long localSegmentLength;

        private Entry(String cpuAbi, long localHeaderOffset, byte[] centralDirectoryBytes, byte[] archiveComment) {
            this.cpuAbi = cpuAbi;
            this.localHeaderOffset = localHeaderOffset;
            this.centralDirectoryBytes = centralDirectoryBytes;
            this.archiveComment = archiveComment;
        }
    }

    private static final class EndOfCentralDirectory {
        private final int entryCount;
        private final long centralDirectorySize;
        private final long centralDirectoryOffset;
        private final byte[] comment;

        private EndOfCentralDirectory(int entryCount, long centralDirectorySize, long centralDirectoryOffset,
                                      byte[] comment) {
            this.entryCount = entryCount;
            this.centralDirectorySize = centralDirectorySize;
            this.centralDirectoryOffset = centralDirectoryOffset;
            this.comment = comment;
        }
    }

    private static final class Zip64ExtraInfo {
        private final int headerOffset;
        private final int dataSize;
        private final int offsetValueOffset;
        private final boolean hasOffset;

        private Zip64ExtraInfo(int headerOffset, int dataSize, int offsetValueOffset, boolean hasOffset) {
            this.headerOffset = headerOffset;
            this.dataSize = dataSize;
            this.offsetValueOffset = offsetValueOffset;
            this.hasOffset = hasOffset;
        }
    }
}