/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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

#include "utils.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "log.h"
#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const int BUFFER_SIZE = 1024;
const int HEX_WIDTH = 2;
const char PATH_DELIMITER = '/';
const long FILE_LENGTH_1M = 1024 * 1024L;
const double FILE_SIZE_OFFSET_DOUBLE = 0.01;
const int FILE_SIZE_DECIMAL_PRECISION = 2;
const int MAX_UUID_LENGTH = 32;
const int RANGE_MIN = 0;
const int RANGE_MAX = 15;
const int TEN = 10;
}

std::string Utils::GetFileContent(const std::string filePath)
{
    std::string realFilePath;
    if (!Utils::GetRealPath(filePath, realFilePath)) {
        LOGE("get real file path failed! filePath=%s", filePath.c_str());
        return nullptr;
    }
    std::ifstream inFile(realFilePath, std::ios::in);
    if (!inFile.is_open()) {
        LOGE("open file path failed![filePath=%s][realFilePath=%s]", filePath.c_str(), realFilePath.c_str());
        return nullptr;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    return fileContent;
}

std::string Utils::ListToString(const std::list<std::string>& lst)
{
    std::stringstream ss;
    for (auto str : lst) {
        ss << str;
    }
    return ss.str();
}

void Utils::CopyListToSet(const std::list<std::string>& lst, std::set<std::string>& st)
{
    for (auto& em : lst) {
        st.insert(em);
    }
}

std::string Utils::ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }
    return str;
}

int64_t Utils::GetFileLength(const std::string filePath)
{
    struct stat statbuf = { 0 };
    if (stat(filePath.c_str(), &statbuf) != 0) {
        LOGE("file stat failed! filePath=%s", filePath.c_str());
        return -1;
    }
    return statbuf.st_size;
}

bool Utils::EndsWith(const std::string& str, const std::string& suffix)
{
    if (str.size() >= suffix.size() && std::equal(suffix.rbegin(), suffix.rend(), str.rbegin())) {
        return true;
    }
    return false;
}

// If there is no same element then returns true, else then returns false
bool Utils::CheckDisjoint(const std::list<std::string>& list1, const std::list<std::string>& list2)
{
    for (const std::string& str1 : list1) {
        for (const std::string& str2 : list2) {
            if (str1.compare(str2) == 0) {
                return false;
            }
        }
    }
    return true;
}

// If list1 contains all elements in list2, returns true, else returns false
bool Utils::CheckContainsAll(const std::list<std::string>& list1, const std::list<std::string>& list2)
{
    bool isFind = false;
    for (const std::string& str2 : list2) {
        isFind = false;
        for (const std::string& str1 : list1) {
            if (str2.compare(str1) == 0) {
                isFind = true;
                break;
            }
        }
        if (!isFind) {
            return false;
        }
    }
    return true;
}

bool Utils::CheckListContain(const std::list<std::string>& lst, const std::string& value)
{
    return std::find(lst.begin(), lst.end(), value) != lst.end();
}

long Utils::GetListDistinctCount(const std::list<std::string>& lst)
{
    std::set<std::string> st;
    std::copy(lst.begin(), lst.end(), std::inserter(st, st.end()));
    return st.size();
}

std::string Utils::GetSha256Str(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, str.c_str(), str.length());
    SHA256_Final(hash, &ctx);

    // Convert hash to hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(HEX_WIDTH) << std::setfill('0') << static_cast<uint32_t>(hash[i]);
    }
    return ss.str();
}

std::string Utils::GetSha256File(const std::string &filePath)
{
    std::string realFilePath;
    if (!GetRealPath(filePath, realFilePath)) {
        LOGE("get real file path failed! jsonFile=%s", filePath.c_str());
        return "";
    }
    std::ifstream file(realFilePath, std::ios::binary);
    if (!file) {
        return "";
    }
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    std::vector<char> buffer(BUFFER_SIZE);
    while (!file.eof()) {
        file.read(buffer.data(), BUFFER_SIZE);
        SHA256_Update(&ctx, buffer.data(), file.gcount());
    }
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    // Convert hash to hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(HEX_WIDTH) << std::setfill('0') << static_cast<uint32_t>(hash[i]);
    }
    return ss.str();
}

std::string Utils::GetSha256Folder(const std::string &filePath)
{
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    for (const auto& entry : fs::recursive_directory_iterator(filePath)) {
        if (fs::is_regular_file(entry)) {
            std::ifstream file(entry, std::ios::binary);
            if (!file.is_open()) {
                LOGE("file open failed! filePath=%s", entry.path().string().c_str());
                return "";
            }
            std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
            SHA256_Update(&ctx, buffer.data(), buffer.size());
        }
    }
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    // Convert hash to hex string
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(HEX_WIDTH) << std::setfill('0') << static_cast<uint32_t>(hash[i]);
    }
    return ss.str();
}

bool Utils::IsFileExists(const std::string& file)
{
    return access(file.c_str(), F_OK) == 0;
}

bool Utils::IsFile(const std::string& file)
{
    struct stat statBuf {};
    return lstat(file.c_str(), &statBuf) == 0 ? S_ISREG(statBuf.st_mode) : false;
}

bool Utils::IsDirectory(const std::string& dir)
{
    struct stat statBuf {};
    return lstat(dir.c_str(), &statBuf) == 0 ? S_ISDIR(statBuf.st_mode) : false;
}

bool Utils::RemoveFile(const std::string& file)
{
    return !IsFileExists(file) || (remove(file.c_str()) == 0);
}

bool Utils::RemoveDirectory(const std::string& dir)
{
    return !IsFileExists(dir) || (rmdir(dir.c_str()) == 0);
}

std::string Utils::GetFilePathByDir(const std::string& dir, const std::string& fileName)
{
    if (dir.empty()) {
        return fileName;
    }
    std::string filePath = dir;
    if (filePath.back() != '/') {
        filePath.push_back(PATH_DELIMITER);
    }
    filePath.append(fileName);
    return filePath;
}

bool Utils::ForceCreateDirectory(const std::string& dir)
{
    std::string::size_type index = 0;
    do {
        std::string subPath;
        index = dir.find('/', index + 1); // (index + 1) means the next char traversed
        if (index == std::string::npos) {
            subPath = dir;
        } else {
            subPath = dir.substr(0, index);
        }

        if (!IsFileExists(subPath) && mkdir(subPath.c_str(), S_IRWXU) != 0) {
            return false;
        }
    } while (index != std::string::npos);
    return IsFileExists(dir);
}

bool Utils::ForceRemoveDirectory(const std::string& dir, bool isDeleteSelf)
{
    if (IsFile(dir)) {
        return RemoveFile(dir);
    } else if (IsDirectory(dir)) {
        DIR* dirPtr = opendir(dir.c_str());
        if (dirPtr == nullptr) {
            return false;
        }
        struct dirent* dirInfo = nullptr;
        while ((dirInfo = readdir(dirPtr)) != nullptr) {
            // do not process the special dir
            if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0) {
                continue;
            }
            std::string filePath = GetFilePathByDir(dir, dirInfo->d_name);
            if (!ForceRemoveDirectory(filePath)) {
                closedir(dirPtr);
                return false;
            }
        }
        closedir(dirPtr);
        if (isDeleteSelf && !RemoveDirectory(dir)) {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

double Utils::GetCeilFileSize(long fileSize, int sizeLimit)
{
    double threshold = static_cast<double>(sizeLimit) + FILE_SIZE_OFFSET_DOUBLE;
    double size = static_cast<double>(fileSize) / FILE_LENGTH_1M;
    size = std::round(size * std::pow(TEN, FILE_SIZE_DECIMAL_PRECISION)) / std::pow(TEN, FILE_SIZE_DECIMAL_PRECISION);
    if (size < threshold && size >= sizeLimit) {
        size = threshold;
    }
    return size;
}

bool Utils::IsPositiveInteger(const std::string& str, int min, int max)
{
    if (str.empty()) {
        return false;
    }
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    try {
        int number = std::stoi(str);
        return number >= min && number <= max;
    } catch (const std::out_of_range& e) {
        LOGE("Number %s is Out of Range!", str.c_str());
        return false;
    }
    return true;
}

bool Utils::StringToBool(const std::string& str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "true") {
        return true;
    }
    return false;
}

bool Utils::StringToArray(const std::string& str, std::list<std::string>& array)
{
    array.clear();
    std::istringstream iss(str);
    std::string item;

    while (std::getline(iss, item, ',')) {
        array.push_back(item);
    }
    return true;
}

bool Utils::CheckFileName(const std::string& filePath, const std::string& fileName)
{
    fs::path fsFilePath(filePath);
    if (fs::is_regular_file(fsFilePath) && fsFilePath.filename().compare(fileName) == 0) {
        return true;
    }
    return false;
}

bool Utils::CheckFileSuffix(const std::string& filePath, const std::string& suffix)
{
    fs::path fsFilePath(filePath);
    if (fs::is_regular_file(fsFilePath) && EndsWith(fsFilePath.filename(), suffix)) {
        return true;
    }
    return false;
}

std::string Utils::GenerateUUID()
{
    static const char* hexDigits = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(RANGE_MIN, RANGE_MAX);

    std::stringstream ss;
    std::vector<int> format = {8, 4, 4, 4, 12};
    int flag = 1;
    for (int i = 0; i < MAX_UUID_LENGTH; ++i) {
        ss << hexDigits[dis(gen)];
        if ((i + 1) == format[0] && i != MAX_UUID_LENGTH - 1) {
            ss << "-";
            format[0] += format[flag];
            flag++;
        }
    }
    return ss.str();
}

bool Utils::CopyFile(const std::string& srcPath, const std::string& dstPath)
{
    std::string realSrcPath;
    std::string realDstPath;
    if (!GetRealPath(srcPath, realSrcPath)) {
        LOGE("get real src path failed! srcPath=%s", srcPath.c_str());
        return false;
    }
    fs::path fsDstPath(dstPath);
    std::string parentOfDstPath = fsDstPath.parent_path().string();
    std::string dstFileName = fsDstPath.filename();
    if (!GetRealPathOfNoneExistFile(parentOfDstPath, realDstPath)) {
        LOGE("get real dst path failed! dstPath=%s", dstPath.c_str());
        return false;
    }
    realDstPath += fs::path::preferred_separator + dstFileName;
    std::ifstream srcFile(realSrcPath, std::ios::binary);
    std::ofstream dstFile(realDstPath, std::ios::binary);
    if (!srcFile.is_open()) {
        LOGE("Open srcPath failed![srcPath=%s][realSrcPath=%s]", srcPath.c_str(), realSrcPath.c_str());
        return false;
    }
    if (!dstFile.is_open()) {
        LOGE("Open dstPath failed![dstPath=%s][realDstPath=%s]", dstPath.c_str(), realDstPath.c_str());
        return false;
    }
    dstFile << srcFile.rdbuf();
    srcFile.close();
    dstFile.close();
    return true;
}

bool Utils::GetFormattedPath(const std::string& path, std::string& formattedPath)
{
    if (path.empty()) {
        formattedPath = "";
        return true;
    }
    try {
        formattedPath = fs::canonical(path).string();
    } catch (const fs::filesystem_error& err) {
        LOGE("GetFormattedPath exception: ", err.what());
        return false;
    }
    return true;
}

bool Utils::GetRealPath(const std::string& path, std::string& realPath)
{
    if (path.length() >= PATH_MAX) {
        return false;
    }
    char buffer[PATH_MAX] = {0};
    if (realpath(path.c_str(), buffer) == nullptr) {
        return false;
    }
    realPath = std::string(buffer);
    return true;
}

bool Utils::GetRealPathOfNoneExistFile(const std::string& path, std::string& realPath)
{
    fs::path fsPath = fs::path(path);
    std::string filePath = fsPath.parent_path().string();
    std::string fileName = fsPath.filename().string();
    if (path.length() >= PATH_MAX) {
        return false;
    }
    char buffer[PATH_MAX] = {0};
    if (realpath(filePath.c_str(), buffer) == nullptr) {
        return false;
    }
    realPath = std::string(buffer) + fs::path::preferred_separator + fileName;
    return true;
}

bool Utils::RemoveAllFilesInDirectory(const std::string& directoryPath)
{
    fs::path fsPath(directoryPath);
    if (!fs::exists(fsPath) || !fs::is_directory(fsPath)) {
        return false;
    }
    for (auto& entry : fs::directory_iterator(fsPath)) {
        if (fs::is_regular_file(entry)) {
            fs::remove(entry);
        }
    }
    return true;
}

std::string Utils::ArrayToString(const std::list<std::string> &array)
{
    std::unique_ptr<PtJson> jArray = PtJson::CreateArray();
    for (const auto& item : array) {
        jArray->Push(item.c_str());
    }
    return jArray->Stringify();
}

std::string Utils::BoolToString(bool value)
{
    return value ? "true" : "false";
}
} // namespace AppPackingTool
} // namespace OHOS
