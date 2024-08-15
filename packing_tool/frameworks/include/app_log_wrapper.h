/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_LOG_WRAPPER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_LOG_WRAPPER_H

#include "hilog/log.h"
#include <string>

namespace OHOS {
namespace AppExecFwk {
#ifndef LOG_DOMAIN
#define LOG_DOMAIN 0xD001100
#endif

#ifndef APP_LOG_TAG
#define APP_LOG_TAG "BundleMgrService"
#endif

enum class AppLogLevel { DEBUG = 0, INFO, WARN, ERROR, FATAL };

static constexpr OHOS::HiviewDFX::HiLogLabel APP_LABEL = {LOG_CORE, LOG_DOMAIN, APP_LOG_TAG};

class AppLogWrapper {
public:
    static bool JudgeLevel(const AppLogLevel &level);

    static void SetLogLevel(const AppLogLevel &level)
    {
        level_ = level;
    }

    static const AppLogLevel &GetLogLevel()
    {
        return level_;
    }

    static std::string GetBriefFileName(const char *str);

private:
    static AppLogLevel level_;
};

#define FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define APP_LOGD(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_DEBUG, LOG_DOMAIN, APP_LOG_TAG, \
    "[%{public}s(%{public}s:%{public}d)]" fmt, FILENAME, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define APP_LOGI(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_INFO, LOG_DOMAIN, APP_LOG_TAG, \
    "[%{public}s(%{public}s:%{public}d)]" fmt, FILENAME, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define APP_LOGW(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_WARN, LOG_DOMAIN, APP_LOG_TAG, \
    "[%{public}s(%{public}s:%{public}d)]" fmt, FILENAME, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define APP_LOGE(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_ERROR, LOG_DOMAIN, APP_LOG_TAG, \
    "[%{public}s(%{public}s:%{public}d)]" fmt, FILENAME, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define APP_LOGF(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_FATAL, LOG_DOMAIN, APP_LOG_TAG, \
    "[%{public}s(%{public}s:%{public}d)]" fmt, FILENAME, __FUNCTION__, __LINE__, ##__VA_ARGS__))

#define APP_LOGI_NOFUNC(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_INFO, LOG_DOMAIN, APP_LOG_TAG, fmt, ##__VA_ARGS__))
#define APP_LOGW_NOFUNC(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_WARN, LOG_DOMAIN, APP_LOG_TAG, fmt, ##__VA_ARGS__))
#define APP_LOGE_NOFUNC(fmt, ...)            \
    ((void)HILOG_IMPL(LOG_CORE, LOG_ERROR, LOG_DOMAIN, APP_LOG_TAG, fmt, ##__VA_ARGS__))
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_APP_LOG_WRAPPER_H
