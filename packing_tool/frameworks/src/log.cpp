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

#include "log.h"

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <securec.h>

namespace OHOS {
namespace AppPackingTool {
namespace {
const int MAX_LOG_SIZE = 10240;
const LOG_LEVEL DEFAULT_LOG_LEVEL = LOG_LEVEL::LOG_LEVEL_INFO;
const char LOG_LEVEL_FLAG[5] = {'D', 'I', 'W', 'E', 'F'};
const int MS_LEN = 3;
}

void Log(char *file, char *func, int32_t line, int32_t level, char *format, ...)
{
    if (level < DEFAULT_LOG_LEVEL) {
        return;
    }
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_t);
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    va_list args;
    va_start(args, format);
    char buffer[MAX_LOG_SIZE];
    int len = vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
    va_end(args);
    if (len >= 0 && len < MAX_LOG_SIZE - 1) {
        std::cout << "[" << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S")
                << '.' << std::setfill('0') << std::setw(MS_LEN) << ms.count()
                << std::setw(0) << "]"
                << "[" << file << ":" << line << "]"
                << "[" << func << "]"
                << "[" << LOG_LEVEL_FLAG[level] << "] "
                << buffer
                << std::endl;
    } else {
        std::cerr << "Error: Buffer size too small for log message." << std::endl;
    }
}
} // namespace AppPackingTool
} // namespace OHOS
