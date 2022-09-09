#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (c) 2022 Huawei Device Co., Ltd.
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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--haptobin', required=True)
    parser.add_argument('--haptobinOutput', required=True)
    parser.add_argument('--unpackOutput', required=True)
    parser.add_argument('--packOutput', required=True)
    parser.add_argument('--outpath', required=True)
    args = parser.parse_args()
    print(args.haptobinOutput)
    print(args.unpackOutput)
    print(args.packOutput)
    print(args.outpath)
    root_dir = os.path.dirname(os.path.realpath(__file__))
    time_out = 5000

    # compile haptobin_tool.jar
    hap_to_bin_shell_path = os.path.join(root_dir, "haptobin.sh")
    command_haptobin = ['bash', hap_to_bin_shell_path, root_dir, args.haptobinOutput, args.outpath]
    child_haptobin = subprocess.Popen(command_haptobin, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    haptobin_out, haptobin_err = child_haptobin.communicate(timeout=time_out)
    if child_haptobin.returncode != 0:
        print(haptobin_out.decode('utf-8'))
        print(haptobin_err.decode('utf-8'))
        raise Exception("compile haptobin java class failed!")

    # compile app_unpacking_tool.jar
    unpack_tool_shell_path = os.path.join(root_dir, "unpackingTool.sh")
    command_unpack = ['bash', unpack_tool_shell_path, root_dir, args.unpackOutput, args.outpath]
    child_unpack = subprocess.Popen(command_unpack, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    unpack_out, unpack_err = child_unpack.communicate(timeout=time_out)
    if child_unpack.returncode != 0:
        print(unpack_out.decode('utf-8'))
        print(unpack_err.decode('utf-8'))
        raise Exception("compile unapcking tool java class failed!")
    
    #compile app_packing_tool.jar
    pack_tool_shell_path = os.path.join(root_dir, "packingTool.sh")
    command_pack = ['bash', pack_tool_shell_path, root_dir, args.packOutput, args.outpath]
    child_pack = subprocess.Popen(command_pack, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    pack_out, pack_err = child_pack.communicate(timeout=time_out)
    if child_pack.returncode != 0:
        print(pack_out.decode('utf-8'))
        print(pack_err.decode('utf-8'))
        raise Exception("compile packing tool java class failed!")

if __name__ == '__main__':
    sys.exit(main())
