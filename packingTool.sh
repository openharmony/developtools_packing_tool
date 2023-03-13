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
pack_build_out_jar_path=$2
pack_build_out_path=$3
toolchain=$4
final_path=$(pwd)

jar_dir="jar"
pack_jar_file="app_packing_tool.jar"
fastjson_jar_file="fastjson-1.2.83.jar"
jar_directory="${root_path}/jar"
pack_jar_path="${root_path}/${jar_dir}/${pack_jar_file}"
manifest_path="${root_path}/META-INF/packing_tool/MANIFEST.MF"

out_dir="${root_path}/out/${toolchain}/packing_tool"
if [ -d "${out_dir}/ohos" ]
    then
        echo "${out_dir}/ohos exist"
    else
        mkdir -p "${out_dir}/ohos"
fi

jar_path="${root_path}/jar"
fastjson_jar_path="${root_path}/jar/fastjson-1.2.83.jar"
java_suffix=".java"
java_collection=""
declare -a compile_class=(
            "BundleException"
            "CommandParser"
            "CompressEntrance"
            "Compressor"
            "CompressVerify"
            "Log"
            "PackFormatter"
            "ShowHelp"
            "Utility"
            "ModuleJsonUtil"
            "Version"
            "FileUtils"
            "ModuleApiVersion"
            "VerifyCollection"
            "DistroFilter"
            "ApiVersion"
            "ScreenShape"
            "ScreenDensity"
            "ScreenWindow"
            "CountryCode"
            "ModuleMetadataInfo"
            "HapVerify"
            "HapVerifyInfo"
            "HQFVerify"
            "HQFInfo"
            "DependencyItem"
            )
compile_class_length=${#compile_class[@]}
for ((i=0; i<${compile_class_length};++i))
do
  java_collection="${java_collection} ${root_path}/adapter/ohos/${compile_class[$i]}${java_suffix}"
done
compile_command="javac -source 1.8 -target 1.8 -cp ${fastjson_jar_path}  -d ${out_dir} ${java_collection}"
eval ${compile_command}

temp_dir="$root_path/jar/temp_${toolchain}"
if [ -d "${temp_dir}" ]
    then
        echo "${temp_dir} exit"
    else
        mkdir ${temp_dir}
fi

cd ${out_dir}
product_pack_jar_command="jar -cvfm ${temp_dir}/${pack_jar_file} ${manifest_path} ./ohos"
eval ${product_pack_jar_command}

# merge app_packing_tool.jar and fastjson
cp ${fastjson_jar_path} "${temp_dir}/${fastjson_jar_file}"
detach_pack_jar_command="jar -xvf ${pack_jar_file}"
detach_fastjson_jar_command="jar -xvf ${fastjson_jar_file}"
cd ${temp_dir}
eval ${detach_pack_jar_command}
eval ${detach_fastjson_jar_command}
rm ${pack_jar_file}
rm ${fastjson_jar_file}

cd ${jar_directory}
temp_pack_jar_dir="${root_path}/jar/packtool_${toolchain}"
temp_pack_jar_path="${root_path}/jar/packtool_${toolchain}/${pack_jar_file}"
merge_pack_fast_jar_command="jar -cvfm ${temp_pack_jar_path} ${manifest_path} -C ${temp_dir} ."
if [ -d "${temp_pack_jar_dir}" ]
    then
        echo "${temp_pack_jar_dir} exist"
    else
        mkdir -p ${temp_pack_jar_dir}
fi
eval ${merge_pack_fast_jar_command}

# make out dir
final_pack_out_path="${final_path}/${pack_build_out_path}"
final_pack_jar_path="${final_path}/${pack_build_out_jar_path}"
if [ -d "$final_pack_out_path" ]
    then
        echo "${final_pack_out_path} exist"
    else 
        mkdir -p ${final_pack_out_path}
fi
copy_command="cp ${temp_pack_jar_path} ${final_pack_jar_path}"
eval ${copy_command}
if [ -f "${pack_jar_file}"]
    then
        echo "${pack_jar_file} exist"
    else
        cp ${temp_pack_jar_path} ${pack_jar_file}
fi
rm -rf ${temp_pack_jar_dir}
rm -rf ${temp_dir}