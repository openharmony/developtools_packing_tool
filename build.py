#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2022-2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""
Archives a set of files.
"""


import os
import sys
import argparse
import subprocess
import re


def run_cmd(title, cmd, time_out):
    child_process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cmd_out, cmd_err = child_process.communicate(timeout=time_out)
    if child_process.returncode != 0:
        print(cmd_out.decode('utf-8'))
        print(cmd_err.decode('utf-8'))
        raise Exception('compile module: ' + title + ' failed!')


def get_compile_str(src_path, java_sources):
    compile_java = ''
    for src in java_sources:
        if len(compile_java) == 0:
            compile_java = '{}'.format(os.path.join(src_path, src))
        else:
            compile_java = '{} {}'.format(compile_java, os.path.join(src_path, src))
    return compile_java


def compile_haptobin_tool(root_path, src_path, jar_output, out_path, 
                          toolchain, fastjson_jar):
    java_sources = [
        'BinaryTool.java',
        'BundleException.java',
        'ConvertHapToBin.java',
        'FileUtils.java',
        'Log.java',
        'PackFormatter.java', 
        'Utility.java'
    ]
    compile_java = get_compile_str(src_path, java_sources)

    shell_path = os.path.join(root_path, "haptobin.sh")
    command = ['bash', shell_path, root_path, jar_output, out_path, 
               toolchain, compile_java, fastjson_jar]
    print('command: ', command)
    run_cmd('haptobin_tool', command, 5000)
    return


def compile_unpacking_tool(root_path, src_path, jar_output, out_path, big_version, 
                           toolchain, fastjson_jar):
    java_sources = [
        'AbilityFormInfo.java', 'AbilityInfo.java', 'ApiVersion.java', 
        'AppInfo.java', 'AppModel.java', 'APPQFResult.java',
        'BundleException.java', 'CommandParser.java', 'CommonEvent.java',
        'CountryCode.java', 'CustomizeData.java', 'DefinePermission.java',
        'DefPermissionGroup.java', 'DefPermission.java', 'DependencyItem.java',
        'DeviceConfig.java', 'DistroFilter.java', 'Distro.java',
        'ExtensionAbilityInfo.java', 'FileUtils.java', 'FormInfo.java',
        'HapInfo.java', 'HapZipInfo.java', 'HQFInfo.java',
        'IntentInfo.java', 'JsInfo.java', 'JsonUtil.java',
        'Log.java', 'MetaDataInfo.java', 'MetaData.java',
        'ModuleAbilityInfo.java', 'ModuleAdaption.java', 'ModuleAppInfo.java',
        'ModuleAtomicService.java', 'ModuleDeviceType.java', 'ModuleInfo.java',
        'ModuleMetadataInfo.java', 'ModuleProfileInfo.java', 'ModuleResult.java',
        'ModuleShortcut.java', 'PackFormatter.java', 'PackInfo.java',
        'PreloadItem.java', 'ProfileInfo.java', 'ReqPermission.java',
        'ResourceIndexResult.java', 'ResourcesParser.java', 'ScreenDensity.java',
        'ScreenShape.java', 'ScreenWindow.java', 'Shortcut.java',
        'ShowHelp.java', 'SkillInfo.java', 'UncompressEntrance.java',
        'Uncompress.java', 'UncompressResult.java', 'UncompressVerify.java',
        'UriInfo.java', 'UsedScene.java', 'Utility.java', 'Want.java'
    ]
    compile_java = get_compile_str(src_path, java_sources)

    shell_path = os.path.join(root_path, "unpackingTool.sh")
    command = ['bash', shell_path, root_path, jar_output, out_path, big_version, 
               toolchain, compile_java, fastjson_jar]
    print('command: ', command)
    run_cmd('app_unpacking_tool', command, 5000)
    return


def compile_packing_tool(root_path, src_path, jar_output, out_path, 
                         toolchain, fastjson_jar, compress_jar):
    java_sources = [
        'ApiVersion.java',
        'BundleException.java',
        'CommandParser.java',
        'CompressEntrance.java',
        'Compressor.java',
        'CompressVerify.java',
        'CountryCode.java',
        'DependencyItem.java',
        'DistroFilter.java',
        'FileUtils.java',
        'HapVerify.java',
        'HapVerifyInfo.java',
        'HQFInfo.java',
        'HQFVerify.java',
        'Log.java',
        'ModuleApiVersion.java',
        'ModuleJsonUtil.java',
        'ModuleMetadataInfo.java',
        'PackFormatter.java',
        'PreloadItem.java',
        'ScreenDensity.java',
        'ScreenShape.java',
        'ScreenWindow.java',
        'ShowHelp.java',
        'Utility.java',
        'VerifyCollection.java',
        'Version.java'
    ]
    compile_java = get_compile_str(src_path, java_sources)

    shell_path = os.path.join(root_path, "packingTool.sh")
    command = ['bash', shell_path, root_path, jar_output, out_path, 
               toolchain, compile_java, fastjson_jar, compress_jar]
    print('command: ', command)
    run_cmd('app_packing_tool', command, 5000)
    return


def compile_check_tool(root_path, src_path, jar_output, out_path, 
                       toolchain, fastjson_jar):
    java_sources = [
        'BundleException.java',
        'CommandParser.java',
        'FileUtils.java',
        'Log.java',
        'PackFormatter.java',
        'Scan.java',
        'ScanEntrance.java',
        'ScanErrorEnum.java',
        'ScanStatDuplicate.java',
        'ScanStatFileSize.java',
        'ScanStatSuffix.java',
        'ScanVerify.java',
        'ShowHelp.java',
        'Utility.java'
    ]
    compile_java = get_compile_str(src_path, java_sources)

    shell_path = os.path.join(root_path, "checkTool.sh")
    command = ['bash', shell_path, root_path, jar_output, out_path, 
               toolchain, compile_java, fastjson_jar]
    print('command: ', command)
    run_cmd('app_check_tool', command, 5000)
    return


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--haptobin', required=True)
    parser.add_argument('--haptobinOutput', required=True)
    parser.add_argument('--unpackOutput', required=True)
    parser.add_argument('--packOutput', required=True)
    parser.add_argument('--checkOutput', required=True)
    parser.add_argument('--outpath', required=True)
    parser.add_argument('--toolchain', required=True)
    parser.add_argument('--compileTarget', required=True)
    args = parser.parse_args()
    print('packingTool args: ', args)
    root_dir = os.path.dirname(os.path.realpath(__file__))
    src_dir = os.path.join(root_dir, "./adapter/ohos/")
    toolchain = args.toolchain
    tool_list = toolchain.split(':')
    toolchain = tool_list[-1]
    toolchain += "_" + args.compileTarget
    
    fastjson_jar = os.path.join(root_dir, '../../prebuilts/packing_tool/fastjson-1.2.83.jar')
    compress_jar = os.path.join(root_dir, '../../prebuilts/packing_tool/commons-compress-1.24.0.jar')
    print('packingTool: ', toolchain, fastjson_jar, compress_jar)

    # compile haptobin_tool.jar
    compile_haptobin_tool(root_dir, src_dir, args.haptobinOutput, args.outpath, 
                          toolchain, fastjson_jar)

    # compile app_unpacking_tool.jar
    version = subprocess.check_output(['javac', '-version'], stderr=subprocess.STDOUT)
    version = version.decode('utf-8')
    array = re.findall(r'\d+', version)
    compatible_version = 8
    big_version = ''
    if int(array[0]) > compatible_version:
        big_version = 'true'
    else:
        big_version = 'false'
    compile_unpacking_tool(root_dir, src_dir, args.unpackOutput, args.outpath, big_version, 
                           toolchain, fastjson_jar)

    #compile app_packing_tool.jar
    compile_packing_tool(root_dir, src_dir, args.packOutput, args.outpath, 
                         toolchain, fastjson_jar, compress_jar)

    #compile app_check_tool.jar
    compile_check_tool(root_dir, src_dir, args.checkOutput, args.outpath, 
                       toolchain, fastjson_jar)

if __name__ == '__main__':
    sys.exit(main())
