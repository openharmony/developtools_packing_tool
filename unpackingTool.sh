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
set -eux
set -o pipefail
root_path=$1
unpack_build_out_jar_path=$2
unpack_build_out_path=$3
big_version=$4
toolchain=$5
compile_java=$6
fastjson_jar=$7
final_path=$(pwd)

jar_dir="jar"
unpack_jar_file="app_unpacking_tool.jar"
jar_directory="${root_path}/${jar_dir}"
unpack_jar_path="${root_path}/${jar_dir}/${unpack_jar_file}"
manifest_path="${root_path}/META-INF/unpacking_tool/MANIFEST.MF"
out_dir="${root_path}/out/${toolchain}/unpacking_tool"
if [ -d "${out_dir}/ohos" ]
    then
        echo "${out_dir}/ohos exist"
    else
        mkdir -p "${out_dir}/ohos"
fi

unpack_out_jar_path="${final_path}/${unpack_build_out_jar_path}"
unpack_out_path="${final_path}/${unpack_build_out_path}"

if [ "$big_version" == "true" ]
    then
        compile_command="javac --release 8 -cp ${fastjson_jar}  -d ${out_dir} ${compile_java}"
        eval ${compile_command}
    else
        compile_command="javac -source 1.8 -target 1.8 -cp ${fastjson_jar}  -d ${out_dir} ${compile_java}"
        eval ${compile_command}
fi

cd $out_dir
temp_unpack_jar_path="${root_path}/${jar_dir}/unpack_${toolchain}/${unpack_jar_file}"
temp_unpack_jart_dir="${root_path}/${jar_dir}/unpack_${toolchain}"
product_unpack_jar_command="jar -cvfm ${temp_unpack_jar_path} $manifest_path ./ohos"
if [ -d "${temp_unpack_jart_dir}" ]
    then
        echo "${temp_unpack_jart_dir} exist"
    else
        mkdir -p "${temp_unpack_jart_dir}"
fi
eval ${product_unpack_jar_command}
if [ -f "${unpack_jar_path}" ]
    then
        echo "${unpack_jar_path} exist"
    else
        cp ${temp_unpack_jar_path} ${unpack_jar_path}
fi
# make out dir
if [ -d "$unpack_out_path" ]
    then
        echo "$unpack_out_path exist"
    else 
        mkdir -p $unpack_out_path
fi
copy_command="cp ${temp_unpack_jar_path} ${unpack_out_jar_path}"
eval ${copy_command}
rm -rf ${temp_unpack_jart_dir}