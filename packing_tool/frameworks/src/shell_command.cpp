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

#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>

#include "app_packager.h"
#include "constants.h"
#include "fast_app_packager.h"
#include "hap_packager.h"
#include "hsp_packager.h"
#include "log.h"
#include "multiapp_packager.h"
#include "packager.h"
#include "shell_command.h"

namespace OHOS {
namespace AppPackingTool {
ShellCommand::ShellCommand(int argc, char *argv[], std::string name)
{
    opterr = 0;
    argc_ = argc;
    argv_ = argv;
    name_ = name;

    if (argc < MIN_ARGUMENT_NUMBER) {
        cmd_ = Constants::CMD_HELP;
        return;
    }
    cmd_ = argv[1];
}

ShellCommand::~ShellCommand()
{}

int32_t ShellCommand::CreateCommandMap()
{
    commandMap_ = {
        {"help", std::bind(&ShellCommand::RunAsHelpCommand, this)},
        {"pack", std::bind(&ShellCommand::RunAsPackCommand, this)},
        {"unpack", std::bind(&ShellCommand::RunAsUnpackCommand, this)},
    };
    return ERR_OK;
}

int32_t ShellCommand::ParseParam()
{
    int n = 0;
    while (n < Constants::OPTIONS_SIZE) {
        int32_t option = getopt_long(argc_, argv_, Constants::SHORT_OPTIONS, Constants::LONG_OPTIONS, nullptr);
        if (optind < 0 || optind > argc_) {
            return ERR_INVALID_VALUE;
        }
        if (option < 0) {
            LOGI("ParseParam finish.");
            break;
        } else if (option == '?') {
            resultReceiver_.append("not support param: ").append(argv_[optind - 1]).append("\n");
            return ERR_INVALID_VALUE;
        } else {
            // example: --mode hap
            std::string paramName = argv_[optind - OFFSET_REQUIRED_ARGUMENT];
            paramName = paramName.substr(Constants::PARAM_PREFIX.length());
            std::string paramValue = optarg;
            parameterMap_[paramName] = optarg;
        }
        n++;
    }
    return ERR_OK;
}

int32_t ShellCommand::OnCommand()
{
    auto respond = commandMap_[cmd_];
    if (respond == nullptr) {
        resultReceiver_.append("not support command: ").append(cmd_).append("\n");
        respond = commandMap_[Constants::CMD_HELP];
    }
    respond();
    return ERR_OK;
}

std::string ShellCommand::ExecCommand()
{
    int32_t result = CreateCommandMap();
    if (result != ERR_OK) {
        resultReceiver_.append("failed to create command map.\n");
        return resultReceiver_;
    }
    result = ParseParam();
    if (result != ERR_OK) {
        resultReceiver_.append("failed to init parameter map.\n");
        return resultReceiver_;
    }

    result = OnCommand();
    if (result != ERR_OK) {
        resultReceiver_.append("failed to execute your command.\n");
        return resultReceiver_;
    }
    return resultReceiver_;
}

int32_t ShellCommand::RunAsHelpCommand()
{
    resultReceiver_.append(HELP_MSG);
    return ERR_OK;
}

int32_t ShellCommand::RunAsPackCommand()
{
    LOGI("RunAsPackCommand ");
    std::unique_ptr<Packager> packager = getPackager();
    if (packager != nullptr) {
        packager->MakePackage();
    }
    return ERR_OK;
}

int32_t ShellCommand::RunAsUnpackCommand()
{
    LOGI("RunAsUnpackCommand ");
    return ERR_OK;
}

std::unique_ptr<Packager> ShellCommand::getPackager()
{
    std::string mode = parameterMap_[Constants::PARAM_MODE];
    if (mode == Constants::MODE_HAP) {
        std::unique_ptr<Packager> packager =
            std::make_unique<HapPackager>(parameterMap_, resultReceiver_);
        return packager;
    } else if (mode == Constants::MODE_HSP) {
        std::unique_ptr<Packager> packager =
            std::make_unique<HspPackager>(parameterMap_, resultReceiver_);
        return packager;
    } else if (mode == Constants::MODE_APP) {
        std::unique_ptr<Packager> packager =
            std::make_unique<AppPackager>(parameterMap_, resultReceiver_);
        return packager;
    } else if (mode == Constants::MODE_MULTIAPP) {
        std::unique_ptr<Packager> packager =
            std::make_unique<MultiAppPackager>(parameterMap_, resultReceiver_);
        return packager;
    } else if (mode == Constants::MODE_FAST_APP) {
        std::unique_ptr<Packager> packager =
            std::make_unique<FastAppPackager>(parameterMap_, resultReceiver_);
        return packager;
    }
    resultReceiver_.append("not support --mode: ").append(mode).append("\n");
    return nullptr;
}
} // namespace AppPackingTool
} // namespace OHOS