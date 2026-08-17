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
#include <limits>
#include <memory>

#include "log.h"
#include "error/packing_tool_err_msg.h"

using packing_tool::error::PackingToolErrMsg;

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

std::string FormatValues(const std::list<std::string>& values)
{
    std::string result = "[";
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        if (iter != values.begin()) {
            result += ", ";
        }
        result += *iter;
    }
    return result + "]";
}

bool IsInt32(const std::unique_ptr<PtJson>& value)
{
    if (!value || !value->IsNumber()) {
        return false;
    }
    double number = value->GetDouble();
    if (number < std::numeric_limits<int32_t>::min() || number > std::numeric_limits<int32_t>::max()) {
        return false;
    }
    return number == static_cast<double>(static_cast<int32_t>(number));
}
}

bool PolicyValue::IsEmpty() const
{
    return policy.empty();
}

bool PolicyValue::ParseFromJson(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> valuesObj;
    Result result = root->GetLastArray(VALUE.c_str(), &valuesObj);
    if (result != Result::NOT_EXIST) {
        if (result != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("apiVersion node get " + VALUE + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < valuesObj->GetSize(); i++) {
            std::unique_ptr<PtJson> item = valuesObj->Get(i);
            if (!item || !item->IsString()) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "distributionFilter value array contains a non-string element.").c_str());
                return false;
            }
            value.push_back(item->GetString());
        }
    }
    result = root->GetLastString(POLICY.c_str(), &policy);
    if (result != Result::NOT_EXIST) {
        if (result != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + POLICY + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool PolicyValue::ParseFromJsonApiVersion(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> valuesObj;
    Result result = root->GetLastArray(VALUE.c_str(), &valuesObj);
    if (result != Result::NOT_EXIST) {
        if (result != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("apiVersion node get " + VALUE + " array node failed!").c_str()).c_str());
            return false;
        }
        for (int32_t i = 0; i < valuesObj->GetSize(); i++) {
            std::unique_ptr<PtJson> item = valuesObj->Get(i);
            if (!IsInt32(item)) {
                LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                    "distributionFilter apiVersion value array contains a non-integer element.").c_str());
                return false;
            }
            value.push_back(std::to_string(item->GetInt()));
        }
    }
    result = root->GetLastString(POLICY.c_str(), &policy);
    if (result != Result::NOT_EXIST) {
        if (result != Result::SUCCESS) {
            LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
                ("App node get " + POLICY + " failed!").c_str()).c_str());
            return false;
        }
    }
    return true;
}

bool DistroFilter::ParseApiVersion(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("root node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> apiVersionObj;
    Result result = root->GetLastObject(API_VERSION.c_str(), &apiVersionObj);
    if (result == Result::NOT_EXIST) {
        return true;
    }
    if (result != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + API_VERSION + " node failed!").c_str()).c_str());
        return false;
    }
    if (!apiVersion.ParseFromJsonApiVersion(apiVersionObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse apiVersionObj failed!").c_str());
        return false;
    }
    return true;
}

bool DistroFilter::ParseScreenShape(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("root node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> screenShapeObj;
    Result result = root->GetLastObject(SCREEN_SHAPE.c_str(), &screenShapeObj);
    if (result == Result::NOT_EXIST) {
        return true;
    }
    if (result != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + SCREEN_SHAPE + " node failed!").c_str()).c_str());
        return false;
    }
    if (!screenShape.ParseFromJson(screenShapeObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse screenShape failed!").c_str());
        return false;
    }
    return true;
}

bool DistroFilter::ParseScreenDensity(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("root node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> screenDensityObj;
    Result result = root->GetLastObject(SCREEN_DENSITY.c_str(), &screenDensityObj);
    if (result == Result::NOT_EXIST) {
        return true;
    }
    if (result != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + SCREEN_DENSITY + " node failed!").c_str()).c_str());
        return false;
    }
    if (!screenDensity.ParseFromJson(screenDensityObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse screenDensity failed!").c_str());
        return false;
    }
    return true;
}

bool DistroFilter::ParseScreenWindow(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("root node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> screenWindowObj;
    Result result = root->GetLastObject(SCREEN_WINDOW.c_str(), &screenWindowObj);
    if (result == Result::NOT_EXIST) {
        return true;
    }
    if (result != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + SCREEN_WINDOW + " node failed!").c_str()).c_str());
        return false;
    }
    if (!screenWindow.ParseFromJson(screenWindowObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse screenWindow failed!").c_str());
        return false;
    }
    return true;
}

bool DistroFilter::ParseCountryCode(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("root node is null!").c_str());
        return false;
    }
    std::unique_ptr<PtJson> countryCodeObj;
    Result result = root->GetLastObject(COUNTRY_CODE.c_str(), &countryCodeObj);
    if (result == Result::NOT_EXIST) {
        return true;
    }
    if (result != Result::SUCCESS) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs(
            ("Json root get " + COUNTRY_CODE + " node failed!").c_str()).c_str());
        return false;
    }
    if (!countryCode.ParseFromJson(countryCodeObj)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Parse countryCode failed!").c_str());
        return false;
    }
    return true;
}

bool DistroFilter::ParseFromJson(std::unique_ptr<PtJson>& root)
{
    if (!root) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("Json root is null!").c_str());
        return false;
    }
    if (!ParseApiVersion(root)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ParseApiVersion failed!").c_str());
        return false;
    }
    if (!ParseScreenShape(root)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ParseScreenShape failed!").c_str());
        return false;
    }
    if (!ParseScreenDensity(root)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ParseScreenDensity failed!").c_str());
        return false;
    }
    if (!ParseScreenWindow(root)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ParseScreenWindow failed!").c_str());
        return false;
    }
    if (!ParseCountryCode(root)) {
        LOGE("%s", PackingToolErrMsg::PARSE_JSON_FAILED.toStringWithArgs("ParseCountryCode failed!").c_str());
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
        std::string apiVersionStr = "apiVersion: policy is " + apiVersion.policy +
            ", value is " + FormatValues(apiVersion.value);
        dumpStr += " " + apiVersionStr;
    }
    if (!screenShape.policy.empty()) {
        std::string screenShapeStr = "screenShape: policy is " + screenShape.policy +
            ", value is " + FormatValues(screenShape.value);
        dumpStr += " " + screenShapeStr;
    }
    if (!screenDensity.policy.empty()) {
        std::string screenDensityStr = "screenDensity: policy is " + screenDensity.policy +
            ", value is " + FormatValues(screenDensity.value);
        dumpStr += " " + screenDensityStr;
    }
    if (!screenWindow.policy.empty()) {
        std::string screenWindowStr = "screenWindow: policy is " + screenWindow.policy +
            ", value is " + FormatValues(screenWindow.value);
        dumpStr += " " + screenWindowStr;
    }
    if (!countryCode.policy.empty()) {
        std::string countryCodeStr = "countryCode: policy is " + countryCode.policy +
            ", value is " + FormatValues(countryCode.value);
        dumpStr += " " + countryCodeStr;
    }
    return dumpStr;
}
} // namespace AppPackingTool
} // namespace OHOS
