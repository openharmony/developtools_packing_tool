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

#include <iostream>
#include <fstream>
#include "utils.h"
#include "json_utils.h"

namespace OHOS {
namespace AppPackingTool {

bool JsonUtils::IsModuleJson(const std::string& filePath)
{
    return Utils::CheckFileName(filePath, MODULE_JSON);
}

bool JsonUtils::IsConfigJson(const std::string& filePath)
{
    return Utils::CheckFileName(filePath, CONFIG_JSON);
}

bool JsonUtils::IsPatchJson(const std::string& filePath)
{
    return Utils::CheckFileName(filePath, PATCH_JSON);
}

std::unique_ptr<PtJson> JsonUtils::JsonFromFile(const std::string& filePath)
{
    std::ifstream inFile(filePath, std::ios::in);
    if (!inFile.is_open()) {
        return nullptr;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    return PtJson::Parse(fileContent);
}

bool JsonUtils::JsonToFile(const std::unique_ptr<PtJson>& json, const std::string& filePath)
{
    if (json.get() == nullptr) {
        return false;
    }
    return StrToFile(json->Stringify(), filePath);
}

bool JsonUtils::StrToFile(const std::string& str, const std::string& filePath)
{
    std::ofstream outFile(filePath, std::ios::out);
    if (!outFile.is_open()) {
        return false;
    }
    outFile << str;
    outFile.close();
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS
