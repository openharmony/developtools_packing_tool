/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <fstream>
#include <sstream>

#include "log.h"
#include "nlohmann/json.hpp"
#include "scan_statdulpicate.h"
#include "utils.h"
#include "zip_utils.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const std::string UNPACK_NAME = "unpack";
const std::string DUPLICATE_FOLDER_NAME = "duplicate";
const std::string EMPTY_STRING = "";
const std::string SHA_256 = "SHA-256";
const std::string RESULT_MD5 = "md5";
const std::string RESULT_SIZE = "size";
const std::string RESULT_FILES = "files";
const std::string DUPLICATE_DESC = "find the duplicated so";
const std::string DUPLICATE_PARAM = "--stat-duplicate";
const std::string TASK_DESC = "taskDesc";
const std::string START_TIME = "startTime";
const std::string STOP_TIME = "stopTime";
const std::string RESULT = "result";
const std::string DATE_FORMAT = "yyyy-MM-dd HH:mm:ss:SSS";
const uint32_t SHOW_SIZE = 10;
const uint32_t INDENT_SIZE = 4;
const std::string HTML_BUTTON_SHOW = "<button id=\"show_duplicate\" type=\"button\" "
    "onclick=\"show_duplicate()\" style=\"display: block\">more</button>";
const std::string HTML_BUTTON_HIDE = "<button id=\"hide_duplicate\" type=\"button\" "
    "onclick=\"hide_duplicate()\" style=\"display: none\">close</button>";
const std::string HTML_TABLE_BOX = "<table class=\"boxTable\">";
const std::string HTML_TABLE_END = "</table>";
const std::string HTML_TR_TD_LAYOUT = "<tr class=\"layout\"><td class=\"key\">";
const std::string HTML_TR_TD_VALUE = "</td><td class=\"value\">";
const std::string HTML_TR_TD_END = "</td></tr>";
const std::string HTML_TR_TD_RESULT = "<tr class=\"result\"><td class=\"key\">";
const std::string HTML_TR_TD_RESULT_VALUE = "</td><td class=\"value\">";
const std::string HTML_TR_TD = "<tr%s><td%s>";
const std::string HTML_TD_END_TD = "</td><td%s>";
const std::string HTML_LI_CIRCLE = "<li type=\"circle\">";
const std::string HTML_LI_END = "</li>";
const std::string HTML_TR_TD_DUPLICATE_KEY = "<tr class=\"duplicateLayout\"><td class=\"duplicateKey\">";
const std::string HTML_TD_DUPLICATE_VALUE = "</td><td class=\"duplicateValue\"><ul>";
const std::string HTML_UL_TD_TR_END = "</ul></td></tr>";
const std::string HTML_UL_HEAD = "<ul>";
const std::string CLASS_DUPLICATE_LAYOUT = " class=\"duplicateLayout\"";
const std::string CLASS_DUPLICATE_KEY = " class=\"duplicateKey\"";
const std::string CLASS_DUPLICATE_VALUE = " class=\"duplicateValue\"";
const std::string HTML_LI_HEAD = "<li>";
const std::string HTML_LI_DUPLICATE = "<li class=\"duplicate\">";
const std::string HTML_TABLE_DUPLICATE = "<table class=\"duplicateTable\">";
const std::string HTML_TABLE_LI_END = "</table></li>";
const std::string HTML_UL_END = "</ul>";
const std::string TEMPLATE_HTML = R"(
<script>
    function show(stat){<br/>
        var type = document.getElementsByClassName(stat);<br/>
        if (stat === 'fileSize') {<br/>
            for (i = 0; i < type.length; i++) {<br/>
                type[i].style.display = "table-row";<br/>
            }<br/>
        }<br/>
        else {<br/>
            for (i = 0; i < type.length; i++) {<br/>
                type[i].style.display = "list-item";<br/>
            }<br/>
        }<br/>
        var show = document.getElementById("show_" + stat);<br/>
        show.style.display = 'none';<br/>
        var hide = document.getElementById("hide_" + stat);<br/>
        hide.style.display = 'block';<br/>
    }<br/>
    function hide(stat){<br/>
        var type = document.getElementsByClassName(stat);<br/>
        for (i = 0; i < type.length; i++) {<br/>
            type[i].style.display = "none";<br/>
        }<br/>
        var show = document.getElementById("show_" + stat);<br/>
        show.style.display = 'block';<br/>
        var hide = document.getElementById("hide_" + stat);<br/>
        hide.style.display = 'none';<br/>
    }<br/>
    function show_duplicate(){<br/>
        show("duplicate");<br/>
    }<br/>
    function hide_duplicate(){<br/>
        hide("duplicate");<br/>
    }<br/>
    function show_fileSize(){<br/>
        show("fileSize");<br/>
    }<br/>
    function hide_fileSize(){<br/>
        hide("fileSize");<br/>
    }<br/>
    function show_suffix(){<br/>
        show("suffix");<br/>
    }<br/>
    function hide_suffix(){<br/>
        hide("suffix");<br/>
    }<br/>
</script>
)";
const std::string TEMPLATE_CSS = R"(
#box {
    width: 1200px;
    margin: auto;
}
.boxTable {
    border: 1px solid black;
    width: 100%;
    height: 200px;
}
.layout {
    height: 30px;
}
.key {
    border: 1px solid black;
    width: 10%;
}
.value {
    border: 1px solid black;
    width: 90%;
}.result {
     height: 30px;
 }
.duplicateTable {
    border: 1px solid black;
    width: 100%;
    height: 100px;
}
.duplicateLayout {
    height: 30px;
}
.duplicateKey {
    border: 1px solid black;
    width: 60px;
}
.duplicateValue {
    border: 1px solid black;
    width: 840px;
}
.fileSizeTable {
    border: 1px solid black;
    width: 100%;
    height: 100px;
}
.fileSizeLayout {
    height: 30px;}
.fileSizeKey {
    border: 1px solid black;
    width: 5%;
}
.fileSizeValue {
    border: 1px solid black;
    width: 95%;
}
.suffixTable {
    border: 1px solid black;
    width: 100%;
    height: 100px;
}
.suffixLayout {
    height: 30px;
}
.suffixKey {
    border: 1px solid black;
    width: 60px;
}
.suffixValue {
    border: 1px solid black;
    width: 840px;
}
.duplicate {
    display: none;
}
.suffix {
    display: none;
}
.fileSize {
    display: none;
    height: 30px;
}
.button {}
)";
const std::string HTML_START = "<!DOCTYPE html><html lang=\"en\">";
const std::string HTML_HEAD = "<head><meta charset=\"UTF-8\" name=\"stat\">"
    "<title>stat</title><link rel=\"stylesheet\" href=\"./stat.css\"></head>";
const std::string DIV_BOX = "<div id=\"box\">";
const std::string HTML_BODY = "<body>";
const std::string HTML_DIV_END = "</div>";
const std::string HTML_BODY_END = "</body>";
const std::string HTML_END = "</html>";
const std::string STAT_JSON = "stat.json";
const std::string STAT_HTML = "stat.html";
const std::string STAT_CSS = "stat.css";
const std::string HTML_BR = "<br/>";
const std::string SCAN_RESULT = "scan_result";
const std::string BACKUPS = "backups";
const std::string LINE_END = "\n";
}

ScanStatDuplicate::ScanStatDuplicate(): taskDesc_(DUPLICATE_DESC), startTime_(EMPTY_STRING), stopTime_(EMPTY_STRING)
{}

std::string ScanStatDuplicate::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowT = std::chrono::system_clock::to_time_t(now);
    std::tm* nowTm = std::localtime(&nowT);
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream timeStream;
    timeStream << std::put_time(nowTm, "%Y-%m-%d %H:%M:%S");
    std::string currentTime = timeStream.str();
    currentTime = currentTime + "." + std::to_string(ms.count());
    return currentTime;
}

std::string ScanStatDuplicate::GetHtmlRow(const std::string& key, const long long& value)
{
    return HTML_TR_TD_LAYOUT + key + HTML_TR_TD_VALUE + std::to_string(value) + HTML_TR_TD_END;
}

std::string ScanStatDuplicate::GetHtmlRow(const std::string& key, const std::string& value)
{
    return HTML_TR_TD_LAYOUT + key + HTML_TR_TD_VALUE + value + HTML_TR_TD_END;
}

std::string ScanStatDuplicate::GetHtmlRowResultClass(const std::string& key, const std::string& value)
{
    return HTML_TR_TD_RESULT + key + HTML_TR_TD_VALUE + value + HTML_TR_TD_END;
}

std::string ScanStatDuplicate::GetHtmlRowResult(const std::string& key, const std::string& value,
    const std::string& trClass, const std::string& tdClassKey, const std::string& tdClassValue)
{
    std::string result = HTML_TR_TD + key + HTML_TD_END_TD + value + HTML_TR_TD_END;
    const std::string replaceStr = "%s";
    int32_t length = replaceStr.length();
    size_t pos = result.find(replaceStr);
    if (pos != std::string::npos) {
        result.replace(result.begin() + pos, result.begin() + pos + length, trClass.begin(), trClass.end());
    }
    pos = result.find(replaceStr);
    if (pos != std::string::npos) {
        result.replace(result.begin() + pos, result.begin() + pos + length, tdClassKey.begin(), tdClassKey.end());
    }
    pos = result.find(replaceStr);
    if (pos != std::string::npos) {
        result.replace(result.begin() + pos, result.begin() + pos + length, tdClassValue.begin(), tdClassValue.end());
    }
    return result;
}

std::string ScanStatDuplicate::GetHtmlRowResult(const std::string& key, const std::vector<std::string>& value)
{
    std::stringstream resValve;
    for (auto it = value.rbegin(); it != value.rend(); ++it) {
        resValve << HTML_LI_CIRCLE << *it << HTML_LI_END;
    }
    return HTML_TR_TD_DUPLICATE_KEY + key + HTML_TD_DUPLICATE_VALUE + resValve.str() + HTML_UL_TD_TR_END;
}

std::string ScanStatDuplicate::GetResultHtml(const std::vector<ParamModel> models)
{
    std::stringstream resultHtml;
    resultHtml << HTML_UL_HEAD;
    for (uint32_t i = 0; i < models.size(); i++) {
        ParamModel model = models[i];
        std::string md5Html = GetHtmlRowResult(RESULT_MD5, model.md5, CLASS_DUPLICATE_LAYOUT, CLASS_DUPLICATE_KEY,
            CLASS_DUPLICATE_VALUE);
        std::string sizeHtml = GetHtmlRowResult(RESULT_SIZE, std::to_string(model.size), CLASS_DUPLICATE_LAYOUT,
            CLASS_DUPLICATE_KEY, CLASS_DUPLICATE_VALUE);
        std::string filesHtml = GetHtmlRowResult(RESULT_FILES, model.files);
        std::string liHtml;
        if (SHOW_SIZE > i) {
            liHtml = HTML_LI_HEAD;
        } else {
            liHtml = HTML_LI_DUPLICATE;
        }
        std::string modelHtml = liHtml + HTML_TABLE_DUPLICATE + md5Html + sizeHtml + filesHtml + HTML_TABLE_LI_END;
        resultHtml << modelHtml;
    }
    resultHtml << HTML_UL_END;
    if (models.size() > SHOW_SIZE) {
        resultHtml << HTML_BUTTON_SHOW;
        resultHtml << HTML_BUTTON_HIDE;
    }
    return resultHtml.str();
}

std::vector<ParamModel> ScanStatDuplicate::GetDuplicateResList(const std::vector<std::string> &fileList)
{
    std::vector<ParamModel> resList = {};
    for (const auto &filePath : fileList) {
        bool addFlag = true;
        std::string md5 = Utils::GetSha256File(filePath);
        for (auto &element : resList) {
            if (element.md5 == md5) {
                element.files.push_back(SplitPath(filePath, UNPACK_NAME));
                addFlag = false;
            }
        }
        if (addFlag) {
            ParamModel model = {};
            model.size = Utils::GetFileLength(filePath);
            model.md5 = md5;
            model.files.push_back(SplitPath(filePath, UNPACK_NAME));
            resList.push_back(model);
        }
    }
    return resList;
}

std::string ScanStatDuplicate::StatDuplicate(const std::vector<std::string> fileList, std::string &jsonStr)
{
    nlohmann::json jsonObject;
    startTime_ = GetCurrentTime();

    std::vector<ParamModel> resList = GetDuplicateResList(fileList);
    std::vector<ParamModel> filterList = {};
    for (const ParamModel &model : resList) {
        if (model.files.size() > 1) {
            filterList.push_back(model);
        }
    }
    stopTime_ = GetCurrentTime();
    std::string taskDescHtml = GetHtmlRow(TASK_DESC, taskDesc_);
    std::string startTimeHtml = GetHtmlRow(START_TIME, startTime_);
    std::string stopTimeHtml = GetHtmlRow(STOP_TIME, stopTime_);
    std::string resultValue = GetResultHtml(filterList);
    std::string resultHtml = GetHtmlRowResultClass(RESULT, resultValue);
    std::string htmlStr = HTML_TABLE_BOX + taskDescHtml + startTimeHtml + stopTimeHtml
        + resultHtml + HTML_TABLE_END;
    jsonObject[RESULT] = nlohmann::json::array();
    for (const auto &model : filterList) {
        jsonObject[RESULT].push_back({{RESULT_FILES, model.files},
                                    {RESULT_MD5, model.md5},
                                    {RESULT_SIZE, model.size}});
    }
    jsonObject[START_TIME] = startTime_;
    jsonObject[STOP_TIME] = stopTime_;
    jsonObject[TASK_DESC] = taskDesc_;
    jsonStr = jsonObject.dump(INDENT_SIZE);
    return htmlStr;
}

std::vector<std::string> ScanStatDuplicate::GetAllInputFileList(const std::string& inputApp,
    const std::string& unZipPath)
{
    std::vector<std::string> fileList;
    if (!fs::exists(fs::path(unZipPath))) {
        fs::create_directory(fs::path(unZipPath));
    }
    ZipUtils::Unzip(inputApp, unZipPath);
    if (!fs::exists(unZipPath) || !fs::is_directory(unZipPath)) {
        LOGE("unzip path is invalid, unZipPath = %s", unZipPath.c_str());
        return fileList;
    }
    std::vector<fs::path> entryPaths;
    for (const auto& entry : fs::directory_iterator(fs::path(unZipPath))) {
        entryPaths.push_back(entry.path());
    }
    std::string copyPath = unZipPath + Constants::LINUX_FILE_SEPARATOR + BACKUPS;
    if (!fs::exists(fs::path(copyPath))) {
        fs::create_directory(fs::path(copyPath));
    }
    for (const auto& entryPath : entryPaths) {
        std::string fileName = entryPath.filename().string();
        std::string filePath = entryPath.string();
        if (!Utils::EndsWith(fileName, Constants::HAP_SUFFIX) && !Utils::EndsWith(fileName, Constants::HSP_SUFFIX)) {
            continue;
        }
        std::string targetPath = copyPath + Constants::LINUX_FILE_SEPARATOR + fileName;
        if (!Utils::CopyFile(filePath, targetPath)) {
            LOGE("copyFile failed, filePath = %s, targetPath = %s", filePath.c_str(), targetPath.c_str());
            return {};
        }
        fs::remove(entryPath);
        std::string outPath = unZipPath + Constants::LINUX_FILE_SEPARATOR + fileName;
        if (!fs::exists(fs::path(outPath))) {
            fs::create_directory(fs::path(outPath));
        }
        ZipUtils::Unzip(targetPath, outPath);
    }
    if (fs::exists(fs::path(copyPath))) {
        fs::remove_all(fs::path(copyPath));
    }
    for (const auto& entry : fs::recursive_directory_iterator(unZipPath)) {
        if (entry.is_regular_file()) {
            fileList.push_back(entry.path().string());
        }
    }
    return fileList;
}

bool ScanStatDuplicate::ScanSoFiles(const std::string& outPath)
{
    std::string templateHtml = TEMPLATE_HTML;
    size_t pos = 0;
    while ((pos = templateHtml.find(HTML_BR, pos)) != std::string::npos) {
        templateHtml.replace(templateHtml.begin() + pos, templateHtml.begin() + pos + HTML_BR.length(),
            LINE_END.begin(), LINE_END.end());
    }
    std::string htmlStr = HTML_START + HTML_HEAD + DIV_BOX + HTML_BODY + templateHtml;

    fs::path filePath(outPath);
    std::string targetPath = filePath.parent_path().string() + Constants::LINUX_FILE_SEPARATOR + UNPACK_NAME;
    std::vector<std::string> fileList = GetAllInputFileList(outPath, targetPath);

    std::regex pattern(Constants::LIB_SO_PATTERN);
    fileList.erase(std::remove_if(fileList.begin(), fileList.end(),
        [pattern](const std::string& file) {return !std::regex_match(file, pattern);}),
        fileList.end());

    std::string jsonStr;
    std::string duplicateHtml = StatDuplicate(fileList, jsonStr);
    htmlStr = htmlStr + duplicateHtml;
    htmlStr = htmlStr + HTML_DIV_END + HTML_BODY_END + HTML_END;
    std::string reportDir = filePath.parent_path().string() + Constants::LINUX_FILE_SEPARATOR + SCAN_RESULT;
    if (!fs::exists(fs::path(reportDir))) {
        fs::create_directory(fs::path(reportDir));
    }
    std::string jsonPath = reportDir + Constants::LINUX_FILE_SEPARATOR + STAT_JSON;
    std::string htmlPath = reportDir + Constants::LINUX_FILE_SEPARATOR + STAT_HTML;
    std::string cssPath = reportDir + Constants::LINUX_FILE_SEPARATOR + STAT_CSS;
    if (!WriteFile(jsonPath, jsonStr)) {
        LOGE("write failed, jsonPath = %s, jsonStr = %s", jsonPath.c_str(), jsonStr.c_str());
        return false;
    }
    if (!WriteFile(htmlPath, htmlStr)) {
        LOGE("write failed, htmlStr = %s", htmlStr.c_str());
        return false;
    }
    if (!WriteFile(cssPath, TEMPLATE_CSS)) {
        LOGE("write failed, htmlStr = %s", htmlStr.c_str());
        return false;
    }
    if (fs::exists(fs::path(targetPath))) {
        fs::remove_all(fs::path(targetPath));
    }
    return true;
}

std::string ScanStatDuplicate::SplitPath(const std::string &path, const std::string &packageName)
{
    size_t pos = path.find(packageName);
    uint32_t packageNameLength = packageName.length();
    if (pos != std::string::npos) {
        return path.substr(pos + packageNameLength);
    } else {
        return path;
    }
}

bool ScanStatDuplicate::WriteFile(const std::string &filePath, const std::string &data)
{
    std::string realFilePath;
    if (!Utils::GetRealPathOfNoneExistFile(filePath, realFilePath)) {
        LOGE("get real file path failed! filePath = %s", filePath.c_str());
        return false;
    }
    if (data.empty()) {
        LOGE("data is empty");
        return false;
    }
    std::ofstream outFile(realFilePath);
    if (outFile.is_open()) {
        outFile << data.c_str();
        outFile.close();
    } else {
        LOGE("Failed to open file for writing");
        return false;
    }
    return true;
}
} // namespace AppPackingTool
} // namespace OHOS