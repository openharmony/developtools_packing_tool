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
#include <memory>
#include "distro_filter.h"
#include "utils.h"
#include "log.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
    const std::string API_VERSION = "apiVersion";
    const std::string SCREEN_SHAPE = "screenShape";
    const std::string SCREEN_DENSITY = "screenDensity";
    const std::string SCREEN_WINDOW = "screenWindow";
    const std::string COUNTRY_CODE = "countryCode";
    const std::string POLICY = "policy";
    const std::string VALUE = "value";
}

bool PolicyValue::ParseFromString(const std::string& jsonString)
{
    if (jsonString.length() == 0) {
        LOGE("Json length is zero!");
        return false;
    }
    std::unique_ptr<PtJson> root = PtJson::Parse(jsonString);
    if (!root) {
        LOGE("Json root is null!");
        return false;
    }
    if (root->Contains(POLICY.c_str())) {
        if (root->GetString(POLICY.c_str(), &policy) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", POLICY.c_str());
            return false;
        }
    }
    if (root->Contains(VALUE.c_str())) {
        std::unique_ptr<PtJson> valueObj;
        if (root->GetArray(VALUE.c_str(), &valueObj) != Result::SUCCESS) {
            LOGE("Json root get %s array node failed!", VALUE.c_str());
            return false;
        }
        for (int i = 0; i< valueObj->GetSize(); i++) {
            value.push_back(valueObj->Get(i)->GetString());
        }
    }
    return true;
}

bool PolicyValue::IsEmpty() const
{
    return policy.empty();
}

bool DistroFilter::ParseApiVersion(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(API_VERSION.c_str())) {
        std::string apiVersionJsonStr;
        if (root->GetString(API_VERSION.c_str(), &apiVersionJsonStr) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", API_VERSION.c_str());
            return false;
        }
        if (!apiVersion.ParseFromString(apiVersionJsonStr)) {
            LOGE("ApiVersion parse from string failed![%s]", apiVersionJsonStr.c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseScreenShape(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(SCREEN_SHAPE.c_str())) {
        std::string screenShapeJsonStr;
        if (root->GetString(SCREEN_SHAPE.c_str(), &screenShapeJsonStr) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", SCREEN_SHAPE.c_str());
            return false;
        }
        if (!screenShape.ParseFromString(screenShapeJsonStr)) {
            LOGE("ScreenShape parse from string failed![%s]", screenShapeJsonStr.c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseScreenDensity(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(SCREEN_DENSITY.c_str())) {
        std::string screenDensityJsonStr;
        if (root->GetString(SCREEN_DENSITY.c_str(), &screenDensityJsonStr) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", SCREEN_DENSITY.c_str());
            return false;
        }
        if (!screenDensity.ParseFromString(screenDensityJsonStr)) {
            LOGE("ScreenDensity parse from string failed![%s]", screenDensityJsonStr.c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseScreenWindow(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(SCREEN_WINDOW.c_str())) {
        std::string screenWindowJsonStr;
        if (root->GetString(SCREEN_WINDOW.c_str(), &screenWindowJsonStr) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", SCREEN_WINDOW.c_str());
            return false;
        }
        if (!screenWindow.ParseFromString(screenWindowJsonStr)) {
            LOGE("ScreenWindow parse from string failed![%s]", screenWindowJsonStr.c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseCountryCode(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(COUNTRY_CODE.c_str())) {
        std::string countryCodeJsonStr;
        if (root->GetString(COUNTRY_CODE.c_str(), &countryCodeJsonStr) != Result::SUCCESS) {
            LOGE("Json root get %s failed!", COUNTRY_CODE.c_str());
            return false;
        }
        if (!countryCode.ParseFromString(countryCodeJsonStr)) {
            LOGE("CountryCode parse from string failed![%s]", countryCodeJsonStr.c_str());
            return false;
        }
    }
    return true;
}


bool DistroFilter::ParseFromString(const std::string& jsonString)
{
    if (jsonString.length() == 0) {
        LOGE("Json length is zero!");
        return false;
    }
    std::unique_ptr<PtJson> root = PtJson::Parse(jsonString);
    if (!root) {
        LOGE("Json root is null!");
        return false;
    }
    if (!ParseApiVersion(root)) {
        LOGE("ParseApiVersion failed!");
        return false;
    }
    if (!ParseScreenShape(root)) {
        LOGE("ParseScreenShape failed!");
        return false;
    }
    if (!ParseScreenDensity(root)) {
        LOGE("ParseScreenDensity failed!");
        return false;
    }
    if (!ParseScreenWindow(root)) {
        LOGE("ParseScreenWindow failed!");
        return false;
    }
    if (!ParseCountryCode(root)) {
        LOGE("ParseCountryCode failed!");
        return false;
    }
    return true;
}

bool DistroFilter::IsEmpty() const
{
    if (apiVersion.IsEmpty() && screenShape.IsEmpty() && screenDensity.IsEmpty() &&
        screenWindow.IsEmpty() && countryCode.IsEmpty()) {
        return true;
    }
    return false;
}

std::string DistroFilter::Dump() const
{
    std::string dumpStr = "";
    if (apiVersion.policy.empty() && screenShape.policy.empty() && screenDensity.policy.empty()
        && screenWindow.policy.empty() && countryCode.policy.empty()) {
        return dumpStr;
    }
    dumpStr = "distroFilter:";
    if (!apiVersion.policy.empty()) {
        std::string apiVersionStr = "apiVersion: policy is " + apiVersion.policy + ", value is "
            + Utils::ListToString(apiVersion.value);
        dumpStr += " " + apiVersionStr;
    }
    if (!screenShape.policy.empty()) {
        std::string screenShapeStr = "screenShape: policy is " + screenShape.policy + ", value is "
        + Utils::ListToString(screenShape.value);
        dumpStr += " " + screenShapeStr;
    }
    if (!screenDensity.policy.empty()) {
        std::string screenDensityStr = "screenDensity: policy is " + screenDensity.policy + ", value is "
            + Utils::ListToString(screenDensity.value);
        dumpStr += " " + screenDensityStr;
    }
    if (!screenWindow.policy.empty()) {
        std::string screenWindowStr = "screenWindow: policy is " + screenWindow.policy + ", value is "
            + Utils::ListToString(screenWindow.value);
        dumpStr += " " + screenWindowStr;
    }
    if (!countryCode.policy.empty()) {
        std::string countryCodeStr = "countryCode: policy is " + countryCode.policy + ", value is "
            + Utils::ListToString(countryCode.value);
        dumpStr += " " + countryCodeStr;
    }
    return dumpStr;
}
} // namespace AppPackingTool
} // namespace OHOS
