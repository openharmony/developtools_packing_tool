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

#include "distro_filter.h"

#include <fstream>
#include <iostream>
#include <memory>

#include "log.h"
#include "utils.h"

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

bool PolicyValue::IsEmpty() const
{
    return policy.empty();
}

bool PolicyValue::ParseFromJson(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("Json root is null!");
        return false;
    }
    if (root->Contains(VALUE.c_str())) {
        std::unique_ptr<PtJson> valuesObj;
        if (root->GetArray(VALUE.c_str(), &valuesObj) != Result::SUCCESS) {
            LOGE("apiVersion node get %s array node failed!", VALUE.c_str());
            return false;
        }
        for (int32_t i = 0; i < valuesObj->GetSize(); i++) {
            value.push_back(valuesObj->Get(i)->GetString());
        }
    }
    if (root->Contains(POLICY.c_str())) {
        if (root->GetString(POLICY.c_str(), &policy) != Result::SUCCESS) {
            LOGE("App node get %s failed!", POLICY.c_str());
            return false;
        }
    }
    return true;
}

bool PolicyValue::ParseFromJsonApiVersion(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("Json root is null!");
        return false;
    }
    if (root->Contains(VALUE.c_str())) {
        std::unique_ptr<PtJson> valuesObj;
        if (root->GetArray(VALUE.c_str(), &valuesObj) != Result::SUCCESS) {
            LOGE("apiVersion node get %s array node failed!", VALUE.c_str());
            return false;
        }
        for (int32_t i = 0; i < valuesObj->GetSize(); i++) {
            value.push_back(std::to_string(valuesObj->Get(i)->GetInt()));
        }
    }
    if (root->Contains(POLICY.c_str())) {
        if (root->GetString(POLICY.c_str(), &policy) != Result::SUCCESS) {
            LOGE("App node get %s failed!", POLICY.c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseApiVersion(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("root node is null!");
        return false;
    }
    if (root->Contains(API_VERSION.c_str())) {
        std::unique_ptr<PtJson> apiVersionObj;
        if (root->GetObject(API_VERSION.c_str(), &apiVersionObj) != Result::SUCCESS) {
            LOGE("Json root get %s node failed!", API_VERSION.c_str());
            return false;
        }
        if (!apiVersion.ParseFromJsonApiVersion(apiVersionObj)) {
            LOGE("Parse apiVersionObj failed!");
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
        std::unique_ptr<PtJson> screenShapeObj;
        if (root->GetObject(SCREEN_SHAPE.c_str(), &screenShapeObj) != Result::SUCCESS) {
            LOGE("Json root get %s node failed!", SCREEN_SHAPE.c_str());
            return false;
        }
        if (!screenShape.ParseFromJson(screenShapeObj)) {
            LOGE("Parse screenShape failed!");
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
        std::unique_ptr<PtJson> screenDensityObj;
        if (root->GetObject(SCREEN_DENSITY.c_str(), &screenDensityObj) != Result::SUCCESS) {
            LOGE("Json root get %s node failed!", SCREEN_DENSITY.c_str());
            return false;
        }
        if (!screenDensity.ParseFromJson(screenDensityObj)) {
            LOGE("Parse screenDensity failed!");
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
        std::unique_ptr<PtJson> screenWindowObj;
        if (root->GetObject(SCREEN_WINDOW.c_str(), &screenWindowObj) != Result::SUCCESS) {
            LOGE("Json root get %s node failed!", SCREEN_WINDOW.c_str());
            return false;
        }
        if (!screenWindow.ParseFromJson(screenWindowObj)) {
            LOGE("Parse screenWindow failed!");
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
        std::unique_ptr<PtJson> countryCodeObj;
        if (root->GetObject(COUNTRY_CODE.c_str(), &countryCodeObj) != Result::SUCCESS) {
            LOGE("Json root get %s node failed!", COUNTRY_CODE.c_str());
            return false;
        }
        if (!countryCode.ParseFromJson(countryCodeObj)) {
            LOGE("Parse countryCode failed!");
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseFromJson(std::unique_ptr<PtJson>& root)
{
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
