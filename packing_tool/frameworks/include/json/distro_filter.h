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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DISTRO_FILTER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DISTRO_FILTER_H

#include <list>
#include <sstream>
#include <string>

#include "pt_json.h"

namespace OHOS {
namespace AppPackingTool {
struct PolicyValue {
    std::string policy = "";
    std::list<std::string> value;

    bool ParseFromJson(std::unique_ptr<PtJson>& root);
    bool ParseFromJsonApiVersion(std::unique_ptr<PtJson>& root);
    bool IsEmpty() const;
};

typedef PolicyValue ApiVersion;
typedef PolicyValue ScreenShape;
typedef PolicyValue ScreenDensity;
typedef PolicyValue ScreenWindow;
typedef PolicyValue CountryCode;

struct DistroFilter {
    ApiVersion apiVersion;
    ScreenShape screenShape;
    ScreenDensity screenDensity;
    ScreenWindow screenWindow;
    CountryCode countryCode;

    bool ParseFromJson(std::unique_ptr<PtJson>& root);
    bool IsEmpty() const;
    std::string Dump() const;

private:
    bool ParseApiVersion(std::unique_ptr<PtJson>& root);
    bool ParseScreenShape(std::unique_ptr<PtJson>& root);
    bool ParseScreenDensity(std::unique_ptr<PtJson>& root);
    bool ParseScreenWindow(std::unique_ptr<PtJson>& root);
    bool ParseCountryCode(std::unique_ptr<PtJson>& root);
};
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DISTRO_FILTER_H
