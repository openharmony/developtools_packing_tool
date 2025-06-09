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

#ifndef MOCK_FAST_APP_MODULE_JSON_UTILS_H
#define MOCK_FAST_APP_MODULE_JSON_UTILS_H

#include <vector>
#include "hap_verify_info.h"

namespace OHOS {

using namespace OHOS::AppPackingTool;
using ResultSeries = std::vector<std::pair<bool, HapVerifyInfo>>;

class MockFastAppModuleJsonUtils {
public:
    static bool mockGetStageHapVerifyInfo_;
    static bool mockGetFaHapVerifyInfo_;
    static bool mockCheckHapsIsValid_;
    static bool mockIsModuleHap_;
    static bool mockCheckHapsIsValidResult_;
    static bool mockIsModuleHapResult_;
    static bool mockCheckAppAtomicServiceCompressedSizeValid_;
    static bool mockCheckAppAtomicServiceCompressedSizeValidResult_;
    static bool mockGetHapVerifyInfosMapfromFileList_;
    static bool mockGetHapVerifyInfosMapfromFileListResult_;
    static ResultSeries mockGetStageHapVerifyInfoResultSeries_;
    static ResultSeries::const_iterator mockGetStageHapVerifyInfoResultSeriesIter_;
    static ResultSeries mockGetFaHapVerifyInfoResultSeries_;
    static ResultSeries::const_iterator mockGetFaHapVerifyInfoResultSeriesIter_;

public:
    static void Reset()
    {
        mockGetStageHapVerifyInfo_ = false;
        mockGetFaHapVerifyInfo_ = false;
        mockCheckHapsIsValid_ = false;
        mockIsModuleHap_ = false;
        mockCheckHapsIsValidResult_ = false;
        mockIsModuleHapResult_ = false;
        mockCheckAppAtomicServiceCompressedSizeValid_ = false;
        mockCheckAppAtomicServiceCompressedSizeValidResult_ = false;
        mockGetHapVerifyInfosMapfromFileList_ = false;
        mockGetHapVerifyInfosMapfromFileListResult_ = false;
        mockGetStageHapVerifyInfoResultSeries_.clear();
        mockGetStageHapVerifyInfoResultSeriesIter_ = mockGetStageHapVerifyInfoResultSeries_.cbegin();
        mockGetFaHapVerifyInfoResultSeries_.clear();
        mockGetFaHapVerifyInfoResultSeriesIter_ = mockGetStageHapVerifyInfoResultSeries_.cbegin();
    }

    static void MockGetStageHapVerifyInfo(const ResultSeries& resultSeries)
    {
        mockGetStageHapVerifyInfo_ = true;
        mockGetStageHapVerifyInfoResultSeries_ = resultSeries;
        mockGetStageHapVerifyInfoResultSeriesIter_ = mockGetStageHapVerifyInfoResultSeries_.cbegin();
    }

    static void MockGetStageHapVerifyInfo(const bool result, const HapVerifyInfo& hapVerifyInfo)
    {
        ResultSeries series;
        series.emplace_back(result, hapVerifyInfo);
        MockGetStageHapVerifyInfo(series);
    }

    static void MockGetFaHapVerifyInfo(const ResultSeries& resultSeries)
    {
        mockGetFaHapVerifyInfo_ = true;
        mockGetFaHapVerifyInfoResultSeries_ = resultSeries;
        mockGetFaHapVerifyInfoResultSeriesIter_ = mockGetStageHapVerifyInfoResultSeries_.cbegin();
    }

    static void MockGetFaHapVerifyInfo(const bool result, const HapVerifyInfo& hapVerifyInfo)
    {
        ResultSeries series;
        series.emplace_back(result, hapVerifyInfo);
        MockGetFaHapVerifyInfo(series);
    }

    static void MockCheckHapsIsValid(const bool result)
    {
        mockCheckHapsIsValid_ = true;
        mockCheckHapsIsValidResult_ = result;
    }

    static void MockIsModuleHap(const bool result)
    {
        mockIsModuleHap_ = true;
        mockIsModuleHapResult_ = result;
    }

    static void MockCheckAppAtomicServiceCompressedSizeValid(const bool result)
    {
        mockCheckAppAtomicServiceCompressedSizeValid_ = true;
        mockCheckAppAtomicServiceCompressedSizeValidResult_ = result;
    }

    static void MockGetHapVerifyInfosMapfromFileList(const bool result)
    {
        mockGetHapVerifyInfosMapfromFileList_ = true;
        mockGetHapVerifyInfosMapfromFileListResult_ = result;
    }
};
} // namespace OHOS
#endif // MOCK_FAST_APP_MODULE_JSON_UTILS_H