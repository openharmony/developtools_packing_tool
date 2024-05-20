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

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>

#include "app_packager.h"
#include "constants.h"
#include "contrib/minizip/unzip.h"
#include "contrib/minizip/zip.h"
#include "errors.h"
#include "packager.h"

namespace OHOS {
namespace AppPackingTool {
namespace {}

AppPackager::AppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : Packager(parameterMap, resultReceiver)
{}

ErrCode AppPackager::InitAllowedParam()
{
//    std::cout << "Hsp Init" << std::endl;
//    resultReceiver_.append("Hsp Init\n");
    return OHOS::ERR_OK;
}
ErrCode AppPackager::PreProcess()
{
//    std::cout << "Hsp PreCheck" << std::endl;
//    resultReceiver_.append("Hsp PreCheck\n");
    return OHOS::ERR_OK;
}

void updatePackInfoToZip(std::string &srcZip, std::string &destZip, std::string packInfoPath) {
    std::cout << "updatePackInfoToZip" << std::endl;
    zipFile zf = zipOpen64(destZip.c_str(), APPEND_STATUS_CREATE);
    if (zf == nullptr) {
        std::cout << "err zipOpen64 null" << std::endl;
    }
    std::cout << "zipOpen64:" << destZip << std::endl;
    unzFile zipFile = unzOpen(srcZip.c_str());
    if (zipFile == nullptr) {
        std::cout << "err unzOpen null" << std::endl;
        return;
    }
    std::cout << "unzOpen:" << srcZip << std::endl;
    unz_global_info globalInfo;
    if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
        std::cerr << "Failed to get global info from ZIP file" << std::endl;  
        unzClose(zipFile);  
        return; 
    }
    std::cout << "unzGetGlobalInfo: " << globalInfo.number_entry << std::endl;
    unz_file_info fileInfo;
    for (int i = 0; i < globalInfo.number_entry; i++) {
        char entryName[256] = {0};
        int status = unzGetCurrentFileInfo(zipFile, &fileInfo, entryName, sizeof(entryName), NULL, 0, NULL, 0);
        if (status != UNZ_OK) {
            std::cerr << "Failed to get file info from ZIP file" << std::endl;  
            unzClose(zipFile);  
            return; 
        }
        std::cout << "entryName: " << entryName << std::endl;
        if (std::string(entryName) == Constants::PACK_INFO) {
            std::ifstream inFile(packInfoPath);
            if (!inFile.is_open()) {
                std::cerr << "Failed to open file pack.info." << std::endl;
                return;
            }
            zip_fileinfo fi = {};
            if (zipOpenNewFileInZip(zf, Constants::PACK_INFO.c_str(), &fi, nullptr, 0, nullptr, 0, nullptr, 0,
                                Z_DEFAULT_COMPRESSION) == ZIP_OK) {
                char buffer[Constants::BUF_SIZE];
                while (inFile.good()) {
                    inFile.read(buffer, sizeof(buffer));
                    auto bytesRead = inFile.gcount();
                    if (bytesRead > 0) {
                        if (zipWriteInFileInZip(zf, buffer, bytesRead) < 0) {
                            std::cout << "3 err write to zip file: " << packInfoPath << std::endl;
                            break;
                        }
                    }
                }
                zipCloseFileInZip(zf);
            } else {
                std::cout << "4 err add file to zip: " << packInfoPath << std::endl;
            }
            inFile.close();
        } else {
            status = unzOpenCurrentFile(zipFile);
            if (status != UNZ_OK) {
                std::cerr << "Failed to get file info from ZIP file" << std::endl;  
                unzClose(zipFile);  
                zipClose(zf, nullptr);
                return; 
            }
            zip_fileinfo fi = {};
            if (zipOpenNewFileInZip(zf, entryName, &fi, nullptr, 0, nullptr, 0, nullptr, 0,
                                Z_DEFAULT_COMPRESSION) == ZIP_OK) {
                char buffer[Constants::BUF_SIZE];
                while (int len = unzReadCurrentFile(zipFile, buffer, Constants::BUF_SIZE)) {
                    if (len > 0) {
                        if (zipWriteInFileInZip(zf, buffer, len) < 0) {
                            std::cout << "3 err write to zip file: " << packInfoPath << std::endl;
                            break;
                        }
                    } else {
                        std::cout << "len: " << len << std::endl;
                        break;
                    }
                }
                zipCloseFileInZip(zf);
            } else {
                std::cout << "4 err add file to zip: " << packInfoPath << std::endl;
            }
        }
        unzCloseCurrentFile(zipFile);
        unzGoToNextFile(zipFile);
    }
    unzClose(zipFile);
    zipClose(zf, nullptr);
}

std::string randomString(size_t len) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 1);
    std::string str(len, 0);
    for (char& c : str) {
        c = alphanum[dis(gen)];
    }
    return str;
}

ErrCode AppPackager::Process()
{
    std::cout << "App DoPackage" << std::endl;
    std::string outPath = parameterMap_.at(Constants::PARAM_OUT_PATH);
    zipFile zf = zipOpen64(outPath.c_str(), APPEND_STATUS_CREATE);
    if (zf == nullptr) {
        std::cout << "err zipOpen64 null" << std::endl;
        return OHOS::ERR_INVALID_VALUE;
    }
    zip_fileinfo fi = {};
    std::map<std::string, std::string>::const_iterator it = parameterMap_.find(Constants::PARAM_PACK_INFO_PATH);
    std::string packInfoPath;
    if (it != parameterMap_.end()) {
        //std::cout << "zip packInfoPath: " << it->second << std::endl;
        packInfoPath = it->second;
        AddFileToZip(zf, fs::path(it->second), fs::path(Constants::PACK_INFO), fi);
    }
    std::vector<std::string> hapList;
    it = parameterMap_.find(Constants::PARAM_HAP_PATH);
    if (it != parameterMap_.end()) {
        //std::cout << "zip hapPath: " << it->second << std::endl;
        std::string hapPath = it->second;
        if (hapPath != "") {
            std::string path;
            std::istringstream iss(hapPath);
            while(std::getline(iss, path, ',')) {
                hapList.push_back(path);
            }
        }
    }
    it = parameterMap_.find(Constants::PARAM_HSP_PATH);
    if (it != parameterMap_.end()) {
        //std::cout << "zip hspPath: " << it->second << std::endl;
        std::string hspPath = it->second;
        if (hspPath != "") {
            std::string path;
            std::istringstream iss(hspPath);
            while(std::getline(iss, path, ',')) {
                hapList.push_back(path);
            }
        }
    }
    fs::path tempDir = fs::temp_directory_path() / randomString(8);
    fs::create_directory(tempDir);
    for(std::string hap : hapList) {
        std::string tempFile = tempDir / fs::path(hap).filename();
        std::cout << "tempFile: " << tempFile << ", hapPath:" << hap << std::endl;
        updatePackInfoToZip(hap, tempFile, packInfoPath);
        AddFileToZip(zf, fs::path(tempFile), fs::path(tempFile).filename(), fi);
    }
    fs::remove_all(tempDir);
    zipClose(zf, nullptr);
    std::cout << "app package" << std::endl;
    return OHOS::ERR_OK;
}



ErrCode AppPackager::PostProcess()
{
//    std::cout << "Hsp PostCheck" << std::endl;
//    resultReceiver_.append("Hsp PostCheck\n");
    return OHOS::ERR_OK;
}

} // namespace AppPackingTool
} // namespace OHOS