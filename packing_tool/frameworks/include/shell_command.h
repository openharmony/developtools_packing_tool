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

#ifndef DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SHELLCOMMAND_H
#define DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SHELLCOMMAND_H

#include <map>
#include <string>
#include <functional>
#include <vector>

#include "packager.h"

namespace OHOS {
namespace AppPackingTool {
namespace {
const int OFFSET_REQUIRED_ARGUMENT = 2;
const std::string HELP_MSG_NO_OPTION = "error: you must specify an option at least.";
const std::string STRING_CODE = "code:";
const std::string TOOL_NAME = "ohos_packing_tool";

const std::string HELP_MSG = "usage: ohos_packing_tool <command> <options>\n"
                             "These are common ohos_packing_tool commands list:\n"
                             "  help    list available commands\n"
                             "  pack    pack a bundle  with options\n";

}  // namespace

class ShellCommand {
public:
    ShellCommand(int argc, char *argv[], std::string name);
    virtual ~ShellCommand();

    int32_t ParseParam();
    int32_t OnCommand();
    std::string ExecCommand(int32_t& ret);
    int32_t CreateCommandMap();

protected:
    static constexpr int MIN_ARGUMENT_NUMBER = 2;

    int argc_;
    char **argv_;

    std::string cmd_;
    std::string name_;
    std::map<std::string, std::function<int()>> commandMap_;
    std::map<std::string, std::string> parameterMap_;
    std::string resultReceiver_ = "";

    int32_t RunAsHelpCommand();
    int32_t RunAsPackCommand();
    int32_t RunAsUnpackCommand();

private:
    std::unique_ptr<Packager> getPackager();
};

}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // DEVELOPTOOLS_PACKING_TOOL_APT_FRAMEWORKS_INCLUDE_SHELLCOMMAND_H