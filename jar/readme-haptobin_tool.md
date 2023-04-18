# haptobin_tool.jar构建说明
```bash
#获取代码
repo init -u https://gitee.com/openharmony/manifest.git -b OpenHarmony-3.2-Release --no-repo-verify
repo sync -c
repo forall -c 'git lfs pull'
bash build/prebuilts_download.sh
# 编译haptobin_tool.jar，需要使用openjdk-8-jdk
./build.sh --product-name ohos-sdk --ccache
# 编译后haptobin_tool.jar位置：./developtools/packing_tool/haptobin_tool.jar
```