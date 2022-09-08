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
    parser.add_argument('--outpath', required=True)
    args = parser.parse_args()
    root_dir = os.path.dirname(os.path.realpath(__file__))

    # compile haptobin_tool.jar
    hap_to_bin_shell_path = os.path.join(root_dir, "haptobin.sh")
    command_haptobin = ['sh', hap_to_bin_shell_path, root_dir, args.haptobinOutput, args.outpath]
    child_haptobin = subprocess.Popen(command_haptobin, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out, err = child_haptobin.communicate()
    if child_haptobin.returncode != 0:
        raise Exception("compile haptobin java class failed!")

if __name__ == '__main__':
    sys.exit(main())
