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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_LOG_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_LOG_H

#include <cstdint>

namespace OHOS {
namespace AppPackingTool {
enum LOG_LEVEL {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

void Log(char *file, char *func, int32_t line, int32_t level, char *format, ...);

#define LOGD(format, ...) Log((char *)__FILE__, (char *)__func__, __LINE__, LOG_LEVEL_DEBUG, \
    (char *)format, ##__VA_ARGS__)
#define LOGI(format, ...) Log((char *)__FILE__, (char *)__func__, __LINE__, LOG_LEVEL_INFO, \
    (char *)format, ##__VA_ARGS__)
#define LOGW(format, ...) Log((char *)__FILE__, (char *)__func__, __LINE__, LOG_LEVEL_WARN, \
    (char *)format, ##__VA_ARGS__)
#define LOGE(format, ...) Log((char *)__FILE__, (char *)__func__, __LINE__, LOG_LEVEL_ERROR, \
    (char *)format, ##__VA_ARGS__)
#define LOGF(format, ...) Log((char *)__FILE__, (char *)__func__, __LINE__, LOG_LEVEL_FATAL, \
    (char *)format, ##__VA_ARGS__)
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_UTILS_H
