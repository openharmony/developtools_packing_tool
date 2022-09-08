#!/usr/bin/env bash
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
set -e
root_path=$1
haptobin_build_jar_path=$2
out_build_path=$3
final_path=$(pwd)
echo "root_path is ${root_path}"
echo ${haptobin_build_jar_path}
echo ${out_build_path}
echo ${final_path}

temp_path="."
jar_dir="jar"
haptobin_jar_file="haptobin_tool.jar"
haptobin_jar_path="$final_path/$out_build_path"
haptobin_jar_file_path="$final_path/$haptobin_build_jar_path"
echo $haptobin_jar_path
# make out dir
if [ -d "$haptobin_jar_path" ]
    then
        echo "$haptobin_jar_path exist"
    else 
        mkdir -p $haptobin_jar_path
fi
temp_jar_path="$root_path/jar/$haptobin_jar_file"
manifest_path=$root_path/META-INF/packingbin_tool/MANIFEST.MF
echo ${manifest_path}
# compile java class
out_dir="$root_path/out/production/haptobin"
java_suffix=".java"
class_suffix=".class"
out_class="$temp_path/$out_dir"
java_collection=""
declare -a compile_class=(
    "Log"
    "BinaryTool"
    "FileUtils"
    "ConvertHapToBin"
    "BundleException"
    "Utility"
)
compile_class_length=${#compile_class[@]}
for ((i=0; i<${compile_class_length};++i))
do
    java_collection="${java_collection} ${root_path}/adapter/ohos/${compile_class[$i]}${java_suffix}"
done

compile_command="javac -source 1.8 -target 1.8 -d ${out_dir} ${java_collection}"
eval ${compile_command}

cd ${out_dir}
class_collextion=""
declare -a pack_class=(
    "Log"
    "LogType"
    "BinaryTool"
    "FileUtils"
    "ConvertHapToBin"
    "BundleException"
    "Utility"
)
pack_class_length=${#pack_class[@]}
for ((i=0; i<${pack_class_length};++i))
do
    class_collextion="${class_collextion} ohos/${pack_class[$i]}${class_suffix}"
done

pack_command="jar -cvfm ${temp_jar_path} ${manifest_path} ${class_collextion}"
eval ${pack_command}
copy_command="cp ${temp_jar_path} ${haptobin_jar_file_path}"
eval ${copy_command}