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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEDUP_LOG_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEDUP_LOG_H

#include <sstream>

#include "log.h"

namespace OHOS {
namespace AppPackingTool {

enum class DedupLogLevel {
    INFO,
    WARNING,
    ERROR,
};

class DedupLogStream final {
public:
    explicit DedupLogStream(DedupLogLevel level) : level_(level) {}

    ~DedupLogStream()
    {
        const std::string message = stream_.str();
        switch (level_) {
            case DedupLogLevel::ERROR:
                LOGE("%s", message.c_str());
                break;
            case DedupLogLevel::WARNING:
                LOGW("%s", message.c_str());
                break;
            default:
                LOGI("%s", message.c_str());
                break;
        }
    }

    template<typename T>
    DedupLogStream& operator<<(const T& value)
    {
        stream_ << value;
        return *this;
    }

private:
    DedupLogLevel level_;
    std::ostringstream stream_;
};

}  // namespace AppPackingTool
}  // namespace OHOS

#define LOG(level) OHOS::AppPackingTool::DedupLogStream(OHOS::AppPackingTool::DedupLogLevel::level)

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_DEDUP_DEDUP_LOG_H
