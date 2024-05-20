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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "packager.h"
#include "errors.h"

namespace OHOS {
namespace AppPackingTool {
namespace {}

Packager::Packager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver)
    : parameterMap_(parameterMap), resultReceiver_(resultReceiver)
{}

Packager::~Packager() {}

std::string Packager::MakePackage()
{
//    for (const auto &pair : parameterMap_) {
//        std::cout << "** key: " << pair.first << ", value: " << pair.second << std::endl;
//    }
//    std::cout << "** resultReceiver: " << resultReceiver_ << std::endl;
    if (InitAllowedParam() != OHOS::ERR_OK) {
        std::cout << "PreCheck err" << std::endl;
    }

    if (PreProcess() != OHOS::ERR_OK) {
        std::cout << "PreCheck err" << std::endl;
    }

    if (Process() != OHOS::ERR_OK) {
        std::cout << "DoPackage err" << std::endl;
    }

    if (PostProcess() != OHOS::ERR_OK) {
        std::cout << "PostCheck err" << std::endl;
    }

    return "OHOS::ERR_OK";
}

ErrCode Packager::PreProcess()
{
    std::cout << "PreCheck" << std::endl;
    resultReceiver_.append("do PreCheck\n");
    return OHOS::ERR_OK;
}

ErrCode Packager::Process()
{
    std::cout << "DoPackage" << std::endl;
    resultReceiver_.append("do DoPackage\n");
    return OHOS::ERR_OK;
}

ErrCode Packager::PostProcess()
{
    std::cout << "PostCheck" << std::endl;
    resultReceiver_.append("do PostCheck\n");
    return OHOS::ERR_OK;
}

void Packager::AddFileToZip(zipFile zf, const fs::path &filePath, const fs::path &zipPath, zip_fileinfo &zipfi)
{
    if (fs::is_directory(filePath)) {
        for (const auto &entry : fs::directory_iterator(filePath)) {
            fs::path tmpPath = zipPath / entry.path().filename();
            //std::cout << "zipPath: " << tmpPath << ", entry.path: " << entry.path() << ", tmpPath: " << tmpPath
            //          << std::endl;
            AddFileToZip(zf, entry.path(), tmpPath, zipfi);
        }
    } else if (fs::is_regular_file(filePath)) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "2 err opening file for read: " << filePath << std::endl;
        }
        if (zipOpenNewFileInZip(zf, zipPath.string().c_str(), &zipfi, nullptr, 0, nullptr, 0, nullptr, 0,
                                Z_DEFAULT_COMPRESSION) == ZIP_OK) {
            char buffer[Constants::BUF_SIZE];
            while (file.good()) {
                file.read(buffer, sizeof(buffer));
                auto bytesRead = file.gcount();
                if (bytesRead > 0) {
                    if (zipWriteInFileInZip(zf, buffer, bytesRead) < 0) {
                        std::cout << "3 err write to zip file: " << filePath << std::endl;
                        break;
                    }
                }
            }
            zipCloseFileInZip(zf);
        } else {
            std::cout << "4 err add file to zip: " << filePath << std::endl;
        }
        file.close();
    }
}

void Packager::WriteStringToZip(zipFile zf, const std::string &content, const fs::path &zipPath, zip_fileinfo &zipfi)
{
    if (zipOpenNewFileInZip(zf, zipPath.string().c_str(), &zipfi, nullptr, 0, nullptr, 0, nullptr, 0,
                            Z_DEFAULT_COMPRESSION) == ZIP_OK) {
        if (zipWriteInFileInZip(zf, content.data(), content.length()) < 0) {
            std::cout << "zipWriteInFileInZip err" << std::endl;
            return;
        }
        zipCloseFileInZip(zf);
    }
}

bool Packager::ParseJsonFile(nlohmann::json &jsonObject, std::string filePath)
{
    std::ifstream i(filePath);
    if (!i.is_open()) {
        std::cout<<"failed to open json file, errno: " << errno << std::endl;
        return false;
    }
    i.seekg(0, std::ios::end);
    int len = static_cast<int>(i.tellg());
    if (len == 0) {
        i.close();
        std::cout << "json file is empty" << std::endl;
        return true;
    }
    i.seekg(0, std::ios::beg);
    jsonObject = nlohmann::json::parse(i, nullptr, false);
    if (jsonObject.is_discarded()) {
        i.close();
        std::cout << "ParseJsonFile failed due to data is discarded." << std::endl;
        return false;
    }
    i.close();
    return true;
}

bool Packager::endWith(const std::string &str, const std::string &suffix) {
    if (str.length() >= suffix.length()) {
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix);
    }
    return false;
}

bool Packager::CheckFileValid(const std::string &filePath, const std::string &filename) 
{
    if (fs::is_regular_file(filePath)) {
        std::string name = fs::path(filePath).filename();
        if (endWith(name, filename)) {
            return true;
        }
    }
    return false; 
}


} // namespace AppPackingTool
} // namespace OHOS