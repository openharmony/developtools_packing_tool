#!/usr/bin/env bash
# Copyright (c) 2022-2026 Huawei Device Co., Ltd.
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
pack_build_out_jar_path=$2
pack_build_out_path=$3
toolchain=$4
compile_java=$5
fastjson_jar=$6
compress_jar=$7
io_jar=$8
fastjson2_jar=$9
fastjson2ext_jar=${10}
final_path=$(pwd)

jar_dir="jar"
pack_jar_file="app_packing_tool.jar"
fastjson_jar_file="fastjson-2.0.57.jar"
fastjson2_jar_file="fastjson2-2.0.57.jar"
fastjson2ext_jar_file="fastjson2-extension-2.0.57.jar"
compress_jar_file="commons-compress-1.27.1.jar"
io_jar_file="commons-io-2.19.0.jar"
jar_directory="${root_path}/jar"
pack_jar_path="${root_path}/${jar_dir}/${pack_jar_file}"
manifest_path="${root_path}/META-INF/packing_tool/MANIFEST.MF"
css_path="${root_path}/adapter/ohos/scan_template.css"
html_path="${root_path}/adapter/ohos/scan_template.html"

out_dir="${root_path}/out/${toolchain}/packTool"
if [ -d "${out_dir}/ohos" ]
    then
        echo "${out_dir}/ohos exist"
    else
        mkdir -p "${out_dir}/ohos"
fi

compile_command="javac -source 1.8 -target 1.8 \
-cp ${fastjson_jar}:${compress_jar}:${io_jar}:${fastjson2_jar}:${fastjson2ext_jar} -d ${out_dir} ${compile_java}"
eval ${compile_command}

temp_dir="$root_path/jar/packing_temp_${toolchain}"
if [ -d "${temp_dir}" ]
    then
        echo "${temp_dir} exit"
    else
        mkdir ${temp_dir}
fi

cd ${out_dir}
cp ${css_path} ./ohos
cp ${html_path} ./ohos
product_pack_jar_command="jar -cvfm ${temp_dir}/${pack_jar_file} ${manifest_path} ./ohos"
eval ${product_pack_jar_command}

# merge app_packing_tool.jar and fastjson/commons-compress
cp ${fastjson_jar} "${temp_dir}/${fastjson_jar_file}"
cp ${fastjson2_jar} "${temp_dir}/${fastjson2_jar_file}"
cp ${fastjson2ext_jar} "${temp_dir}/${fastjson2ext_jar_file}"
cp ${compress_jar} "${temp_dir}/${compress_jar_file}"
cp ${io_jar} "${temp_dir}/${io_jar_file}"
detach_pack_jar_command="jar -xvf ${pack_jar_file}"
detach_fastjson_jar_command="jar -xvf ${fastjson_jar_file}"
detach_fastjson2_jar_command="jar -xvf ${fastjson2_jar_file}"
detach_fastjson2ext_jar_command="jar -xvf ${fastjson2ext_jar_file}"
detach_io_jar_command="jar -xvf ${io_jar_file}"
detach_compress_jar_command="jar -xvf ${compress_jar_file}"
cd ${temp_dir}
eval ${detach_pack_jar_command}
eval ${detach_fastjson2ext_jar_command}
eval ${detach_fastjson2_jar_command}
eval ${detach_fastjson_jar_command}
eval ${detach_io_jar_command}
eval ${detach_compress_jar_command}
cp "$root_path/jar/NOTICE" "META-INF/NOTICE.txt"
rm ${pack_jar_file}
rm ${fastjson_jar_file}
rm ${fastjson2_jar_file}
rm ${fastjson2ext_jar_file}
rm ${compress_jar_file}
rm ${io_jar_file}

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
if [ -f "${pack_jar_file}" ]
    then
        echo "${pack_jar_file} exist"
    else
        cp -n "${temp_pack_jar_path}" "${pack_jar_file}" || true
fi
rm -rf ${temp_pack_jar_dir}
rm -rf ${temp_dir}
rm -rf ${out_dir}
