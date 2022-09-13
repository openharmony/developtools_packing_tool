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
final_path=$(pwd)
echo ${root_path}
echo ${pack_build_out_jar_path}
echo ${pack_build_out_path}
echo ${final_path}

jar_dir="jar"
pack_jar_file="app_packing_tool.jar"
fastjson_jar_file="fastjson-1.2.83.jar"
jar_directory="$root_path/jar"
pack_jar_path="$root_path/$jar_dir/$pack_jar_file"
manifest_path="$root_path/META-INF/packing_tool/MANIFEST.MF"

out_dir="$root_path/out/production/packing_tool"
if [ -d "$out_dir/ohos" ]
    then
        echo "$out_dir/ohos exist"
    else
        mkdir -p "$out_dir/ohos"
fi

jar_path="$root_path/jar"
fastjson_jar_path="$root_path/jar/fastjson-1.2.83.jar"
java_suffix=".java"
java_collection=""
declare -a compile_class=(
            "BundleException"
            "CommandParser"
            "CompressEntrance"
            "Compressor"
            "CompressVerify"
            "Log"
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
            "HQFVerifyInfo"
            )
compile_class_length=${#compile_class[@]}
for ((i=0; i<${compile_class_length};++i))
do
  java_collection="${java_collection} ${root_path}/adapter/ohos/${compile_class[$i]}${java_suffix}"
done
compile_command="javac -source 1.8 -target 1.8 -cp ${fastjson_jar_path}  -d ${out_dir} ${java_collection}"
eval ${compile_command}

class_collection=""
declare -a pack_class=(
            "BundleException.class"
            "CommandParser.class"
            "CompressEntrance.class"
            "Compressor.class"
            "CompressVerify.class"
            "Log.class"
            "LogType.class"
            "ShowHelp.class"
            "Utility.class"
            "ModuleJsonUtil.class"
            "Version.class"
            "FileUtils.class"
            "ModuleApiVersion.class"
            "VerifyCollection.class"
            "DistroFilter.class"
            "ApiVersion.class"
            "ScreenShape.class"
            "ScreenDensity.class"
            "ScreenWindow.class"
            "CountryCode.class"
            "ModuleMetadataInfo.class"
            "HapVerify.class"
            "HapVerifyInfo.class"
            "HQFVerify.class"
            "HQFVerifyInfo.class"
)
pack_class_length=${#pack_class[@]}
for ((i=0; i<${pack_class_length};++i))
do
  class_collection="${class_collection} ohos/${pack_class[$i]}"
done
cd $out_dir
product_pack_jar_command="jar -cvfm $pack_jar_path $manifest_path $class_collection"
eval ${product_pack_jar_command}

# merge app_packing_tool.jar and fastjson
temp_dir="$root_path/jar/temp"
temp_two="2"
if [ -d "$temp_dir" ]
    then
        temp_dir="$temp_dir$temp_two"
    else
        echo "$temp_dir not exit"
fi
mkdir $temp_dir
cp $pack_jar_path "$temp_dir/$pack_jar_file"
cp $fastjson_jar_path "$temp_dir/$fastjson_jar_file"

detach_pack_jar_command="jar -xvf $pack_jar_file"
detach_fastjson_jar_command="jar -xvf $fastjson_jar_file"
cd $temp_dir
eval $detach_pack_jar_command
eval $detach_fastjson_jar_command
rm $pack_jar_file
rm $fastjson_jar_file
cd $jar_directory
merge_pack_fast_jar_command="jar -cvfm $pack_jar_file $manifest_path -C $temp_dir ."
eval $merge_pack_fast_jar_command
if [ -d "$temp_dir" ]
    then
        rm -rf $temp_dir
    else
        echo "$temp_dir not exit"
fi

# make out dir
final_pack_out_path="$final_path/$pack_build_out_path"
final_pack_jar_path="$final_path/$pack_build_out_jar_path"
if [ -d "$final_pack_out_path" ]
    then
        echo "$final_pack_out_path exist"
    else 
        mkdir -p $final_pack_out_path
fi
copy_command="cp $pack_jar_path $final_pack_jar_path"
eval ${copy_command}
