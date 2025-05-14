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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_FAST_APP_PACKAGER_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_FAST_APP_PACKAGER_H

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>

#include "packager.h"
#include "zip_wrapper.h"

namespace OHOS {
namespace AppPackingTool {
class FastAppPackager : public Packager {
public:
    FastAppPackager(const std::map<std::string, std::string> &parameterMap, std::string &resultReceiver);
    ~FastAppPackager() override {}
    bool IsVerifyValidInFastAppMode();
    bool FormatPath();
    bool IsVerifyValid();
    bool IsFormatPathValid(const std::string &inputPath, std::list<std::string> &formattedPathList);
    bool IsHapPathValid(const std::list<std::string> &formatPathList);
    bool IsHspPathValid(const std::list<std::string> &formatPathList, const std::string &hspSuffix);
    bool ModuleJsonAndPackInfoExists(const std::list<std::string> &hapPathList,
        const std::list<std::string> &hspPathList);
    bool CheckBundleTypeConsistency(const std::list<std::string> &hapPathList,
        const std::list<std::string> &hspPathList);
    bool IsPackInfoValid(const fs::path &packInfo, const std::list<std::string> &hapPathList,
        const std::list<std::string> &hspPathList);
    bool IsPackInfoPathListValid(const std::list<std::string> &pathList, std::set<std::string> &packages,
        const std::set<std::string> &allPackageSet);
    bool GetPackageNameFromPath(const fs::path &path, std::list<std::string> &packageNameList);
    bool CompressFastAppMode();
    bool CheckHapAndPackFastApp(std::list<std::string> &fileList, const fs::path &tmpDir);
    bool Pack(const std::string &tmpDir, std::list<std::string> &fileList);
    bool Pack(const fs::path &inputPath, const fs::path &appPackInfo, const fs::path &outPath, fs::path &path);
    bool PackDir(const fs::path &inputPath, const fs::path &appPackInfo, const fs::path &outPath, fs::path &path);
    bool RepackHsp(const fs::path &inputPath, const fs::path &appPackInfo, const fs::path &outPath, fs::path &path);
    bool PackFastApp(const std::list<std::string> &fileList);
    bool AddHapListToApp(const std::list<std::string> &fileList);
    bool AddSignatureAndCertificateToApp();
    bool packSingleThread(const fs::path &inputPath, const fs::path &appPackInfo,
        const fs::path &outHap, fs::path &path);
    bool AddOtherFileToZip(const fs::path &inputPath);
    bool packMultiThread(const fs::path &inputPath, const fs::path &appPackInfo,
        const fs::path &outHap, fs::path &path);
    void GenBuildHash(const fs::path &inputPath, std::string &jsonString);
    std::string GetBundleTypeFromPath(const std::string &filePath);
    std::string GetModuleJsonContentFromPath(const fs::path &path);
    std::string ReadFileToString(const fs::path &path);
    std::string GetBundleTypeFromModuleJson(const std::string &moduleJsonContent);
    std::string GetPackInfoContentFromPath(const fs::path &path);
    std::string UzipHspAndRemovePackInfo(const std::string& hspPath, const std::string& unzipPathString);

protected:
    int32_t InitAllowedParam() override;
    int32_t PreProcess() override;
    int32_t Process() override;
    int32_t PostProcess() override;

private:
    bool isSharedApp_ = false;
    bool compressNativeLibs_ = false;
    bool isGenerateBuildHash_ = false;
    std::string packInfoPath_;
    std::list<std::string> formattedEntryCardPathList_;
    std::list<std::string> formattedHapPathList_;
    std::list<std::string> formattedHspPathList_;
    ZipWrapper zipWrapper_;
    std::map<std::string, std::shared_ptr<HapVerifyInfo>> hapVerifyInfoMap_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_FAST_APP_PACKAGER_H