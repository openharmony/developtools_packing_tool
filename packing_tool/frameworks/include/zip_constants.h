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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_CONSTANTS_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_CONSTANTS_H

#include <filesystem>
#include <string>
#include <map>

#include "contrib/minizip/unzip.h"
#include "contrib/minizip/zip.h"

namespace fs = std::filesystem;

namespace OHOS {
namespace AppPackingTool {
enum ZipErrCode {
    ZIP_ERR_SUCCESS = 0,
    ZIP_ERR_FAILURE
};

enum class ZipLevel : int32_t {
    ZIP_LEVEL_DEFAULT = Z_DEFAULT_COMPRESSION,
    ZIP_LEVEL_0 = Z_NO_COMPRESSION,
    ZIP_LEVEL_1 = Z_BEST_SPEED,
    ZIP_LEVEL_2,
    ZIP_LEVEL_3,
    ZIP_LEVEL_4,
    ZIP_LEVEL_5,
    ZIP_LEVEL_6,
    ZIP_LEVEL_7,
    ZIP_LEVEL_8,
    ZIP_LEVEL_9 = Z_BEST_COMPRESSION
};

static const ZipLevel ZIP_LEVEL_NO_COMPRESS = ZipLevel::ZIP_LEVEL_0;
static const ZipLevel ZIP_LEVEL_BEST_SPEED = ZipLevel::ZIP_LEVEL_1;
static const ZipLevel ZIP_LEVEL_BEST_COMPRESS = ZipLevel::ZIP_LEVEL_9;

static const int32_t ZIP_FILE_ATTR_DEFAULT = 0;
static const int32_t ZIP_FILE_ATTR_DIRECTORY = 16;

static const int32_t MAX_ZIP_BUFFER_SIZE = 4096;
}  // namespace AppPackingTool
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_ZIP_CONSTANTS_H
