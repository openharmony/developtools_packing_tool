#!/usr/bin/env bash
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
set -e
root_path=$1
haptobin_build_jar_path=$2
out_build_path=$3
toolchain=$4
compile_java=$5
fastjson_jar=$6
final_path=$(pwd)

temp_path="."
jar_dir="jar"
haptobin_jar_file="haptobin_tool.jar"
haptobin_jar_path="${final_path}/${out_build_path}"
haptobin_jar_file_path="${final_path}/${haptobin_build_jar_path}"
# make out dir
if [ -d "${haptobin_jar_path}" ]
    then
        echo "${haptobin_jar_path} exist"
    else 
        mkdir -p ${haptobin_jar_path}
fi
final_jar_path="${root_path}/jar/${haptobin_jar_file}"
manifest_path=${root_path}/META-INF/packingbin_tool/MANIFEST.MF

# compile java class
out_dir="${root_path}/out/${toolchain}/haptobin"
if [ -d "${out_dir}/ohos" ]
    then
        echo "${out_dir}/ohos exist"
    else
        mkdir -p "${out_dir}/ohos"
fi

compile_command="javac -source 1.8 -target 1.8 -cp ${fastjson_jar} -d ${out_dir} ${compile_java}"
eval ${compile_command}
cd ${out_dir}
temp_jar_path="${root_path}/jar/haptobin_${toolchain}/${haptobin_jar_file}"
temp_jar_dir="${root_path}/jar/haptobin_${toolchain}"
pack_command="jar -cvfm ${temp_jar_path}/ ${manifest_path} ./ohos"
if [ -d "${temp_jar_dir}" ]
    then
        echo "${temp_jar_dir} exist"
    else
        mkdir -p "${temp_jar_dir}"
fi
eval ${pack_command}

copy_command="cp ${temp_jar_path} ${haptobin_jar_file_path}"
eval ${copy_command}
# copy to developtoo/packintool/jar
if [ -f "${final_jar_path}" ]
    then
        echo "${final_jar_path} exist"
    else
        eval "cp ${temp_jar_path} ${final_jar_path}"
fi
rm -rf ${temp_jar_dir}