# 打包拆包工具使用说明

## 简介

packing_tool子系统用于生成打包工具和拆包工具，其中打包工具用于hap，app，hqf，appqf，hsp包的生成，拆包工具用于对hap，app，hqf，appqf，har，hsp包的拆包及对hap，hsp，app，appqf的解析。具体的功能介绍如下：

打包工具子系统架构图如下：

![](https://gitee.com/openharmony/developtools_packing_tool/raw/master/img/%E6%89%93%E5%8C%85%E5%B7%A5%E5%85%B7%E6%9E%B6%E6%9E%84%E5%9B%BE.webp)

## 1. 打包指令说明

### 1.1 hap包模式打包指令

#### 1.1.1 示例

- Stage模型的打包命令示例：


```
java -jar app_packing_tool.jar --mode hap --json-path <option> --resources-path <option> --ets-path <option> --index-path <option> --pack-info-path <option> --out-path path\out\srcEntrance.hap --force true --compress-level 5 --pkg-context-path <option>
```

- FA模型的打包命令示例：


```
java -jar app_packing_tool.jar --mode hap --json-path <option> --maple-so-path [option] --profile-path [option] --maple-so-dir [option] --dex-path [option] --lib-path [option] --resources-path [option] --index-path [option] --out-path <option> --force [option] --compress-level 5
```

#### 1.1.2 参数含义及规范

| 指令               | 是否必选项 | 选项                   | 描述                                                        | 备注         |
|------------------|-------|----------------------|-----------------------------------------------------------|------------|
| --mode           | 是     | hap                  | 打包类型。                                                     | NA         |
| --json-path      | 是     | NA                   | .json文件路径，FA模型文件名必须为config.json；stage模型文件名必须为module.json。 | NA         |
| --profile-path   | 否     | NA                   | CAPABILITY.profile文件路径。                                   | NA         |
| --maple-so-path  | 否     | NA                   | maple so文件输入路径，so文件路径，文件名必须以.so为后缀。如果是多个so需要用“，”分隔。       | NA         |
| --maple-so-dir   | 否     | NA                   | maple so目录输入路径。                                           | NA         |
| --dex-path       | 否     | NA                   | 1.dex文件路径，文件名必须以.dex为后缀。如果是多个dex需要用“，”分隔。2.dex文件路径也可以为目录。 | NA         |
| --lib-path       | 否     | NA                   | lib库文件路径。                                                 | NA         |
| --resources-path | 否     | NA                   | resources资源包路径。                                           | NA         |
| --index-path     | 否     | NA                   | .index文件路径，文件名必须为resources.index。                         | NA         |
| --pack-info-path | 否     | NA                   | pack.info文件路径，文件名必须为pack.info。                            | NA         |
| --rpcid-path     | 否     | NA                   | rpcid.sc文件路径，文件名必须为rpcid.sc。                              | NA         |
| --js-path        | 否     | NA                   | 存放js文件目录路径。                                               | 仅stage模型生效 |
| --ets-path       | 否     | NA                   | 存放ets文件目录路径。                                              | 仅stage模型生效 |
| --out-path       | 是     | NA                   | 目标文件路径，文件名必须以.hap为后缀。                                     | NA         |
| --force          | 否     | true或者false          | 默认值为false，如果为true，表示当目标文件存在时，强制删除。                        | NA         |
| --an-path        | 否     | NA                   | 存放an文件的路径。                                                | 仅stage模型生效 |
| --ap-path        | 否     | NA                   | 存放ap文件的路径。                                                | 仅stage模型生效 |
| --dir-list       | 否     | NA                   | 可指定目标文件夹列表，将其打入hap包内。                                   | NA         |
| --compress-level | 否     | number               | 压缩等级，默认值1，可选等级1-9。在应用配置compressNativeLibs参数为true的情况下生效，数值越大压缩率越高、压缩速度越慢。  | NA    |
| --pkg-context-path      | 否         | NA            | 可指定语境信息表文件路径，文件名必须为pkgContextInfo.json。 | 仅stage模型生效              |
| --hnp-path | 否 | NA | 指定native软件包文件路径，将native软件包打入hap包内。 | NA |

### 1.2 res模式打包指令

#### 1.2.1 示例

```
java -jar app_packing_tool.jar --mode res --entrycard-path [option] --pack-info-path [option] --out-path [option] --force [option]
```

#### 1.2.2 参数含义及规范

| 指令               | 是否必选项 | 选项            | 描述                                 |
|------------------|-------|---------------|------------------------------------|
| --mode           | 是     | res           | 命令类型。                              |
| --entrycard-path | 是     | NA            | 快照目录的路径。                           |
| --pack-info-path | 是     | NA            | pack.info文件路径，包含卡片信息。              |
| --out-path       | 是     | NA            | 目标文件路径，文件名必须以.res为后缀。              |
| --force          | 否     | true或者false   | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

### 1.3 app包模式打包指令

#### 1.3.1 示例

```
java -jar app_packing_tool.jar --mode app --hap-path <option> --hsp-path <option> --out-path [option] --signature-path [option] --certificate-path [option] --pack-info-path [option] --force [option]
```

#### 1.3.2 参数含义及规范

| 指令                 | 是否必选项 | 选项          | 描述                                                           |
|--------------------|-------|-------------|--------------------------------------------------------------|
| --mode             | 是     | app         | 多个hap需满足hap的合法性校验。                                           |
| --hap-path         | 否     | NA          | 1.hap包文件路径，文件名必须以.hap为后缀。如果时多个hap包需要用“，”分隔。2.hap包文件路径也可以是目录。 |
| --hsp-path         | 否     | NA          | 1.hsp包文件路径，文件名必须以.hsp为后缀。如果时多个hsp包需要用“，”分隔。2.hsp包文件路径也可以是目录。 |
| --pack-info-path   | 是     | NA          | 文件名必须为pack.info。                                             |
| --out-path         | 是     | NA          | 目标文件路径，文件名必须以.app为后缀。                                        |
| --signature-path   | 否     | NA          | 签名路径。                                                        |
| --certificate-path | 否     | NA          | 证书路径。                                                        |
| --pack-res-path    | 否     | NA          | 打包res的目标文件路径，文件名需要为pack.res。                                 |
| --force            | 否     | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。                           |
| --encrypt-path     | 否     | NA          | 文件名必须为encrypt.json。                                          |

#### 1.3.3 打包app时hap的合法性校验

在对工程内的hap、hsp包打包生成app包时，需要保证被打包的每个hap、hsp在json文件中配置的bundleName，versionCode，minCompatibleVersionCode，minAPIVersion，targetAPIVersion相同，moduleName唯一，对于fa模型，还需要保证json文件中配置的package唯一。hap模块之间需要保证apiReleaseType相同，hsp模块不校验apiReleaseType。

#### 1.3.4 打包app时的压缩规则

打包app时，对release模式的hap、hsp包会进行压缩，对debug模式的hap、hsp包不会压缩。

### 1.4 多工程打包

#### 1.4.1 示例

```
java -jar app_packing_tool.jar --mode multiApp --hap-list 1.hap,2.hap --app-list 1.app,2.app --out-path final.app
```

#### 1.4.2 参数含义及规范

| 指令             | 是否必选项 | 选项              | 描述                                                                                                |
|----------------|-------|-----------------|---------------------------------------------------------------------------------------------------|
| --mode         | 是     | multiApp        | 打包类型，在将多个hap打入同一个app时，需保证每个hap满足合法性校验规则。                                                          |
| --hap-list     | 否     | hap的路径          | 1.hap包文件路径，文件名必须以.hap为后缀。如果是多个hap包需要”，“分隔。2.hap文件路径也可以是目录。                                        |
| --hsp-list     | 否     | hsp的路径          | 1.hsp包文件路径，文件名必须以.hsp为后缀。如果是多个hsp包需要”，“分隔。2.hsp文件路径也可以是目录。                                        |
| --app-list     | 否     | app的路径          | 1.app文件路径，文件名必须以.app为后缀。如果是多个app包需要用”，“分隔。2.app文件路径也可以是目录。3.--hap-list，--hsp-list，--app-list不可以都不传。 |
| --out-path     | 是     | NA              | 目标文件路径，文件名必须以.app为后缀。            |
| --force        | 否     | 默认值为false       | 默认值为false，如果为true，表示当目标文件存在时，强制删除。                                                                |
| --encrypt-path | 否     | encrypt.json的路径 | 文件名必须为encrypt.json。                                                                |

#### 1.4.3 多工程打包hap合法性校验

需要保证被打包的每个hap在json文件中配置的bundleName，versionCode，minCompatibleVersionCode相同，minAPIVersion，targetAPIVersion相同，moduleName唯一，同一设备entry唯一，对于fa模型，还需要保证json文件中配置的package唯一。hap模块之间需要保证apiReleaseType相同，hsp模块不校验apiReleaseType。

### 1.5 hqf模式打包指令

#### 1.5.1 示例

```
java -jar app_packing_tool.jar --mode hqf --json-path <option> --lib-path <option> --ets-path <option> --resources-path <option> --out-path <option>
```

#### 1.5.2 参数含义及规范

| 指令          | 是否必选项 | 选项          | 描述                                 |
|-------------|-------|-------------|------------------------------------|
| --mode      | 是     | hqf         | 打包类型。                              |
| --json-path | 是     | NA          | .json文件路径，文件名必须为patch.json。        |
| --lib-path  | 否     | NA          | lib库文件的路径。                         |
| --ets-path  | 否     | NA          | 存放ets文件目录路径。                       |
| --resources-path  | 否     | NA          | resources资源包路径。                       |
| --out-path  | 是     | NA          | 目标文件路径，文件名必须以.hqf为后缀。              |
| --force     | 否     | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

### 1.6 appqf模式打包指令

#### 1.6.1 示例

```
java -jar app_packing_tool.jar --mode appqf --hqf-list <option> --out-path <option>
```

#### 1.6.2 参数含义及规范

| 指令         | 是否必选项 | 选项          | 描述                                 |
|------------|-------|-------------|------------------------------------|
| --mode     | 是     | appqf       | 打包类型。                              |
| --hqf-list | 是     | NA          | hqf文件路径，多个hqf以英文逗号隔开。              |
| --out-path | 是     | NA          | 目标文件路径，文件名必须以.appqf为后缀。            |
| --force    | 否     | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |


### 1.7 hsp模式打包指令

#### 1.7.1 示例
```
java -jar path\app_packing_tool.jar --mode hsp --json-path <option> --resources-path <option> --ets-path <option> --index-path <option> --pack-info-path <option> --out-path path\out\library.hsp --force true --compress-level 5 --pkg-context-path <option>
```

#### 1.7.2 参数含义及规范

| 指令               | 是否必选项 | 选项                   | 描述                                                        |
|------------------|-------|----------------------|-----------------------------------------------------------|
| --mode           | 是     | hsp                  | 打包类型。                                                     |
| --json-path      | 是     | NA                   | .json文件路径，文件名必须为module.json。                              |
| --profile-path   | 否     | NA                   | CAPABILITY.profile文件路径。                                   |
| --dex-path       | 否     | NA                   | 1.dex文件路径，文件名必须以.dex为后缀。如果是多个dex需要用“，”分隔。2.dex文件路径也可以为目录。 |
| --lib-path       | 否     | NA                   | lib库文件路径。                                                 |
| --resources-path | 否     | NA                   | resources资源包路径。                                           |
| --index-path     | 否     | NA                   | .index文件路径，文件名必须为resources.index。                         |
| --pack-info-path | 否     | NA                   | pack.info文件路径，文件名必须为pack.info。                            |
| --js-path        | 否     | NA                   | 存放js文件目录路径。                                               |
| --ets-path       | 否     | NA                   | 存放ets文件目录路径。                                              |
| --out-path       | 是     | NA                   | 目标文件路径，文件名必须以.hsp为后缀。                                     |
| --force          | 否     | true或者false          | 默认值为false，如果为true，表示当目标文件存在时，强制删除。                        |
| --an-path        | 否     | NA                   | 存放an文件的路径。                                                |
| --ap-path        | 否     | NA                   | 存放ap文件的路径。                                                |
| --dir-list       | 否     | NA                   | 可指定目标文件夹列表，将其打入hap包内                               |
| --compress-level | 否     | number               | 压缩等级，默认值1，可选等级1-9。在应用配置compressNativeLibs参数为true的情况下生效，数值越大压缩率越高、压缩速度越慢。  |
| --pkg-context-path      | 否         | NA            | 可指定语境信息表文件路径，文件名必须为pkgContextInfo.json。 |

### 1.8 versionNormalize模式指令

如果一个应用由多个团队开发，当只有一个hap或hsp需要修改升级时，可以调用此命令，将多个hap、hsp的版本统一。本命令会修改所传入的hap、hsp的版本号和版本名称，并在指定目录生成修改后的同名hap、hsp，以及一个version_record.json文件，用于记录所有hap、hsp原有的版本号、版本名称。

#### 1.8.1 示例
```
java -jar path\app_packing_tool.jar --mode versionNormalize --input-list 1.hap,2.hsp --version-code 1000001 --version-name 1.0.1 --out-path path\out\
```

#### 1.8.2 参数含义及规范

| 指令             | 是否必选项 | 选项               | 描述                                                                |
|----------------|-------|------------------|-------------------------------------------------------------------|
| --mode         | 是     | versionNormalize | 命令类型。                                                             |
| --input-list   | 是     | hap或hsp的路径       | 1.hap或hsp包文件路径，文件名必须以.hap或.hsp为后缀。如果是多个hap包需要”，“分隔。2.hap包或hsp包目录。 |
| --version-code | 是     | 版本号              | 指定的版本号，hap、hsp的版本号会被修改为该版本。需要为整数，且不小于所有传入的hap、hsp的版本号。            |
| --version-name | 是     | 版本名称             | 指定的版本名称，hap、hsp的版本名称会被修改为该版本名称。                                   |                                  
| --out-path     | 是     | NA               | 目标文件路径，需要为一个目录。                                                   |

### 1.9 packageNormalize模式指令

可以调用此命令，将集成态HSP的包名和版本号设置成应用侧的包名和版本号，并在指定目录生成修改后的同名HSP。

#### 1.9.1 示例
```
java -jar path\app_packing_tool.jar --mode packageNormalize --hsp-list path\1.hsp,path\2.hsp --bundle-name com.example.myapplication --version-code 1000001 --out-path path\out\
```

#### 1.9.2 参数含义及规范

| 指令             | 是否必选项 | 选项               | 描述                                                  |
|----------------|-------|------------------|-----------------------------------------------------|
| --mode         | 是     | packageNormalize | 命令类型。                                               |
| --hsp-list     | 是     | HSP的路径           | 1.HSP包文件路径，文件名必须以.hsp为后缀。如果是多个HSP包需要”，“分隔。2.HSP包目录。 |
| --bundle-name  | 是     | 包名               | 指定的包名，HSP的包名会被修改为指定的包名。                             |
| --version-code | 是     | 版本号              | 指定的版本号，HSP的版本号会被修改为该版本号。需要为整数，且大于0。                 |                                  
| --out-path     | 是     | NA               | 目标文件路径，需要为一个目录。                                     |

### 1.10 fastApp模式打包指令

#### 1.10.1 示例

```
java -jar app_packing_tool.jar --mode fastApp --hap-path <option> --hsp-path <option> --out-path [option] --signature-path [option] --certificate-path [option] --pack-info-path [option] --force [option]
```

#### 1.10.2 参数含义及规范

| 指令                 | 是否必选项 | 选项          | 描述                                                                                                    |
|--------------------|-------|-------------|-------------------------------------------------------------------------------------------------------|
| --mode             | 是     | fastApp     | 多个hap需满足hap的合法性校验。                                                                                    |
| --hap-path         | 否     | NA          | hap包文件目录路径，目录内要包含一个完整的hap包的所有文件。允许传入多个路径，多个路径需要用英文“,”分隔。                                              |
| --hsp-path         | 否     | NA          | 1.hsp包文件路径，文件名必须以.hsp为后缀。如果时多个hsp包需要用“，”分隔。2.hsp包文件目录路径，目录内要包含一个完整的hsp包的所有文件。允许传入多个路径，多个路径需要用英文“,”分隔。 |
| --pack-info-path   | 是     | NA          | 文件名必须为pack.info。                                                                                      |
| --out-path         | 是     | NA          | 目标文件路径，文件名必须以.app为后缀。                                                                                 |
| --signature-path   | 否     | NA          | 签名路径。                                                                                                 |
| --certificate-path | 否     | NA          | 证书路径。                                                                                                 |
| --pack-res-path    | 否     | NA          | 打包res的目标文件路径，文件名需要为pack.res。                                                                          |
| --force            | 否     | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。                                                                    |
| --encrypt-path     | 否     | NA          | 文件名必须为encrypt.json。                                                                    |

#### 1.10.3 打包app时hap、hsp的合法性校验

在对工程内的hap、hsp包打包生成app包时，需要保证被打包的每个hap、hsp在json文件中配置的bundleName，versionCode，minCompatibleVersionCode，minAPIVersion，targetAPIVersion相同，moduleName唯一。hap模块之间需要保证apiReleaseType相同，hsp模块不校验apiReleaseType。

## 2. 拆包指令说明

### 2.1 hap包模式拆包指令

#### 2.1.1 示例

```
java -jar app_unpacking_tool.jar --mode <option> --hap-path <options> --out-path [option] --force [option]
```

#### 2.1.2 参数含义及规范

| 指令       | 是否必选项 | 选项          | 描述                                                         |
| ---------- | ---------- | ------------- | ------------------------------------------------------------ |
| --mode     | 是         | hap           | 拆包类型。                                                   |
| --hap-path | 是         | NA            | hap包路径。                                                  |
| --rpcid    | 否         | true或者false | 是否单独将rpcid文件从hap包中提取到指定目录，如果为true，将仅提取rpcid文件，不对hap包进行拆包。 |
| --out-path | 是         | NA            | 拆包目标文件路径。                                           |
| --force    | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。  |

### 2.2 har包模式拆包指令

#### 2.2.1 示例

```
java -jar app_unpacking_tool.jar --mode <options> --har-path <option> --out-path [option] --force [option]
```

#### 2.2.2 参数含义及规范

| 指令       | 是否必选项 | 选项          | 描述                                                        |
| ---------- | ---------- |-------------| ----------------------------------------------------------- |
| --mode     | 是         | har         | 拆包类型。                                                  |
| --har-path | 是         | NA          | har包路径。                                                 |
| --out-path | 是         | NA          | 拆包目标文件路径。                                          |
| --force    | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

### 2.3 app包模式拆包指令

#### 2.3.1 示例

```
java -jar app_unpacking_tool.jar --mode <options> --app-path <option> --out-path [option] --force [option]
```

#### 2.3.2 参数含义及规范

| 指令       | 是否必选项 | 选项          | 描述                                                        |
| ---------- | ---------- |-------------| ----------------------------------------------------------- |
| --mode     | 是         | app         | 拆包类型。                                                  |
| --app-path | 是         | NA          | app包路径。                                                 |
| --out-path | 是         | NA          | 拆包目标文件路径。                                          |
| --force    | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

### 2.4 从hap包中获取rpcid文件

#### 2.4.1 示例

```
java -jar app_unpacking_tool.jar --mode hap --rpcid true --hap-path <option> --out-path <option> --force true
```

#### 2.4.2 参数含义及规范

| 指令       | 是否必选项 | 选项          | 描述                                                         |
| ---------- | ---------- | ------------- | ------------------------------------------------------------ |
| --mode     | 是         | hap           | 拆包类型                                                     |
| --rpcid    | 否         | true或者false | 是否单独将rpcid文件从hap包中提取到指定目录，如果为true，将仅提取rpcid文件，不对hap包进行拆包。 |
| --hap-path | 是         | NA            | hap包路径                                                    |
| --out-path | 是         | NA            | 拆包rpcid目标文件路径                                        |
| --force    | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。  |

### 2.5 hsp包模式拆包指令

#### 2.5.1 示例

```
java -jar app_unpacking_tool.jar --mode <option> --hsp-path <options> --out-path [option] --force [option]
```

#### 2.5.2 参数含义及规范

| 指令         | 是否必选项 | 选项          | 描述                                 |
|------------| ---------- |-------------|------------------------------------|
| --mode     | 是         | hsp         | 拆包类型。                              |
| --hsp-path | 是         | NA          | hsp包路径。                            |
| --out-path | 是         | NA          | 拆包目标文件路径。                          |
| --force    | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

### 2.6 appqf模式拆包指令

#### 2.6.1 示例

```
java -jar app_unpacking_tool.jar --mode <option> --appqf-path <options> --out-path [option] --force [option]
```

#### 2.6.2 参数含义及规范

| 指令           | 是否必选项 | 选项          | 描述                                 |
|--------------| ---------- |-------------|------------------------------------|
| --mode       | 是         | appqf       | 拆包类型。                              |
| --appqf-path | 是         | NA          | appqf包路径。                          |
| --out-path   | 是         | NA          | 拆包目标文件路径。                          |
| --force      | 否         | true或者false | 默认值为false，如果为true，表示当目标文件存在时，强制删除。 |

## 3. 包解析接口

### 3.1 接口目录

| 类名               | 接口原型                                                     | 类型     | 接口详细描述                                                            |
| ------------------ | ------------------------------------------------------------ | -------- |-------------------------------------------------------------------|
| UncompressEntrance | UncomperssResult parseApp(String appPath,String parseMode,String deviceType,String hapName) | Java接口 | 接口功能：根据参数解析app包的pack.info信息 输入参数：app包路径，解析参数 返回值：UncomperssResult |
| UncompressEntrance | UncomperssResult parseApp(InputStream input,String parseMode,String deviceType,String hapName,String outPath) | Java接口 | 接口功能：根据参数解析app包的pack.info信息 输入参数：app文件流，解析参数 返回值：UncomperssResult |
| UncompressEntrance | UncomperssResult parseHap(String hapPath)                    | Java接口 | 接口功能：根据参数解析app包的json配置文件 输入参数：hap包路径，解析参数 返回值：UncomperssResult    |
| UncompressEntrance | UncomperssResult parseHap(InputStream input)                 | Java接口 | 接口功能：根据参数解析app包的json配置文件 输入参数：hap包文件流，解析参数 返回值：UncomperssResult   |

## 4. 拆包工具信息字段

### 4.1 UncomperssResult（Bundle信息）结构体信息

| 字段            | 类型               | 描述                                     | 备注 |
| ----------------| ------------------ |----------------------------------------| ---- |
| result          | boolean            | 标识此次解析是否成功                             | NA   |
| message         | String             | 解析失败时返回失败原因                            | NA   |
| packInfos       | List\<PackInfo>    | bundle中pack.info文件的packages信息          | NA   |
| profileInfos    | List\<profileInfo> | 应用的配置信息                                | NA   |
| profileInfosStr | List\<String>      | 应用的配置信息 | NA   |
| icon            | String             | 返回入口组件的icon路径，如果没有入口组件，则返回第一个组件的icon信息 | NA   |
| label           | String             | 返回入口组件的label，如果没有入口组件，则返回第一个组件的label信息 | NA   |
| packageSize     | long               | 表示app包的大小,单位字节                   |   NA  |

### 4.2 PackInfo结构体信息

| 字段                | 类型          | 描述                                    | 备注 |
| ------------------- | ------------- | --------------------------------------- | ---- |
| name                | String        | 包名                                    | NA   |
| moduleName          | String        | Hap名称                                 | NA   |
| moduleType          | String        | module的类型                            | NA   |
| deviceType          | List\<String> | 表示当前hap包所支持的设备类型           | NA   |
| deliveryWithInstall | boolean       | 标识当前hap是否在用户主动安装的时候安装 | NA   |

### 4.3 ProfileInfo结构体信息

| 字段         | 类型                           | 描述                                       | 备注                                                         |
| ------------ | ------------------------------ | ------------------------------------------ | ------------------------------------------------------------ |
| hapName      | String                         | 标识当前解析的hap包名称                    | NA                                                           |
| appInfo      | AppInfo结构体（见下述AppInfo） | 标识APP信息的结构体（见下述AppInfo信息）   | NA                                                           |
| deviceConfig | Map\<String,DeviceConfig>      | 标识设备信息                               | 存储类型为Map\<String,String>，存储设备类型名称及对应设备类型的信息，在stage模型中，这个字段存储在app结构体中 |
| hapInfo      | HapInfo结构体（见下述HapInfo） | 标识hap包中module信息（见下述HapInfo信息） | NA                                                           |

### 4.4 AppInfo结构体信息

| 字段                             | 类型      | 描述                                                                          | 备注          |
|--------------------------------|---------|-----------------------------------------------------------------------------|-------------|
| bundleName                     | String  | 标识app的包名称                                                                   | NA          |
| vendor                         | String  | 标识app的供应商信息                                                                 | NA          |
| relatedBundleName              | String  | 标识app相关bundle的包名                                                            | NA          |
| versionName                    | String  | 标识app中的versionName信息                                                        | NA          |
| versionCode                    | String  | 标识app中的versionCode信息                                                        | NA          |
| targetApiVersion               | int     | 标识应用运行需要的API目标版本                                                            | NA          |
| compatibleApiVersion           | int     | 标识应用兼容的API版本                                                                | NA          |
| appName                        | String  | 标识显示在桌面上的ability的label                                                      | NA          |
| appNameEN                      | String  | 标识显示在桌面上的ability的label                                                      | NA          |
| releaseType                    | String  | 标识应用运行需要的API目标版本的类型                                                         | NA          |
| shellVersionCode               | String  | 标识HarmonyOS中的API版本                                                          | NA          |
| shellVersionName               | String  | 标识HarmonyOS中的API版本名称                                                        | NA          |
| multiFrameworkBundle           | String  | 标识bundleName                                                                | NA          |
| debug                          | boolean | 标识应用是否可调试                                                                   | NA          |
| icon                           | String  | 标识应用的图标路径                                                                   | NA          |
| label                          | String  | 标识应用的label                                                                  | NA          |
| description                    | String  | 标识应用的描述信息                                                                   | stage模型新增   |
| minCompatibleVersionCode       | int     | 标识应用能够兼容的最低版本号                                                              | NA          |
| distributedNotificationEnabled | boolean | 标记该应用是否开启分布式通知                                                              | stage模型新增   |
| bundleType                     | String  | 标识bundle的类型，取值：<br/>- app：普通应用<br/>- atomicService：元服务 <br/>- shared：应用间共享库 | NA   |
| compileSdkVersion              | String  | 标识编译该应用时使用的sdk版本                                                            | stage模型新增   |
| compileSdkType                 | String  | 标识编译该应用时使用的sdk类别                                                            | stage模型新增   |
| labels                         | HashMap\<String, String> | 标识多语言应用程序AppJson的标签。                                                        | NA          |
| descriptions                   | HashMap\<String, String> | 标识多语言应用程序AppJson的说明。                                                        | NA          |

### 4.5 HapInfo结构体信息

| 字段                 | 类型                                          | 描述                               | 备注                                   |
| ---------------------|-----------------------------------------------|------------------------------------| ---------------------------------------|
| appModel             | AppModel枚举值                                | 标识应用的框架模型<br/>- FA：FA模型<br/>- STAGE：Stage模型 | NA |
| packageStr           | String                                        | 标识应用的包信息                    | FA模型特有             |
| name                 | String                                        | 标识当前module的名字                | NA                 |
| description          | String                                        | 标识hap包的描述信息                 | FA模型特有                       |
| supportedModes       | List\<String>                                 | 标识hap包的支持的模式               | NA                           |
| abilities            | List\<AbilityInfo>                            | 标识hap包ability信息                | NA                             |
| defPermissions       | List\<DefPermission>                          | 标识hap包DefPermission信息          | NA                             |
| definePermissions    | List\<DefinePermission>                       | 标识hap包DefinePermission信息       | NA                             |
| defPermissionsGroups | List\<DefPermissionsGroups>                   | 标识hap包DefPermissionsGroups 信息  | NA                             |
| distro               | Distro结构体                                 | 标识hap包的distro信息               | NA                             |
| reqCapabilities      | List\<String>                                 | 标识hap包reqCapabilities信息        | NA                           |
| deviceType           | List\<String>                                 | 标识hap可以运行在哪类设备上         | 对应stage模型中的deviceTypes       |
| metaData             | MetaData结构体（见下述metaData）              | 标识hap的自定义元信息                | NA           |
| dependencies         | List\<DependencyItem>                         | 标识hap包DependencyItem信息         | NA                             |
| isJs                 | boolean                                       | 标识该应用是否是js应用              | FA模型特有            |
| reqPermissions       | list\<ReqPermission>                          | 标识应用申请的权限的集合             | 对应stage模型的requestPermissions |
| commonEvents         | CommonEvent结构体（见下述CommentEvent）       | 标识静态事件                         | NA     |
| shortcuts            | list\<Shortcut>                               | 标识应用的shortcuts信息              | NA                  |
| distroFilter         | DistroFilter结构体                            | 标识应用市场按设备形态分发的信息     | NA               |
| srcEntrance          | String                                        | 标识应用对应的入口代码路径           | stage模型新增          |
| process              | String                                        | 标识hap的进程名                      | stage模型新增       |
| mainElement          | String                  | 标识hap的入口ability名称或者extension名称 | stage模型新增，FA模型将mainAbility的值赋值给mainElement |
| uiSyntax             | String                                        | 定义该ETS Component的语法类型         | stage模型新增       |
| pages                | List\<String>                                 | 列举ETS Component中每个页面信息       | stage模型新增       |
| extensionAbilityInfos| List\<ExtensionAbilityInfo>                   | 描述extensionAbility的配置信息       | stage模型新增        |
| moduleAtomicService  | ModuleAtomicService结构体（见下述ModuleAtomicService） | 描述hap的元服务信息          | NA              |
| formInfos            | List\<AbilityFormInfo>                        | 描述卡片的信息                       | NA              |
| descriptions         | HashMap\<String, String>                      | 标识hap的说明信息                    | NA             |
| compressedSize       | long                                          | 标识Hap包压缩后的大小，单位字节         | NA             |
| originalSize         | long                                          | 标识Hap包的原始大小，单位字节         | NA             |

### 4.6 AbilityInfo结构体信息

| 字段                | 类型                       | 描述                                              | 备注                        |
|-------------------|--------------------------| ------------------------------------------------- | ------------------------------- |
| name              | String                   | 标识当前ability的逻辑名                           | NA                              |
| description       | String                   | 标识ability的描述                                 | NA                              |
| descriptionRes    | String                   | 标识ability资源的描述                             | NA                              |
| icon              | String                   | 标识ability图标                                   | NA                              |
| iconPath          | String                   | 标识ability图标路径                               | NA                              |
| label             | String                   | 标识ability对用户显示的名称                       | NA                              |
| labelRes          | String                   | 标识ability对用户显示的名称资源                   | NA                              |
| type              | String                   | 标识ability类型                                   | Stage模型下该值直接赋予page类型 |
| formsEnabled      | boolean                  | 标识ability卡片是否使能                           | NA                              |
| formInfo          | FormInfo结构体           | 描述卡片的信息                                    | NA                              |
| uri               | String                   | 标识ability的uri信息                              | FA模型支持                      |
| launchType        | String                   | 标识ability中的launcherType信息                   | NA                              |
| orientation       | String                   | 标识ability中的orientation信息                    | NA                              |
| visible           | boolean                  | 标识ability中的visible信息                        | NA                              |
| grantPermission   | boolean                  | 标识ability中的grantPermission信息                | NA                              |
| readPermission    | String                   | 标识ability中的readPermission信息                 | NA                              |
| writePermission   | String                   | 标识ability中的writePermission信息                | NA                              |
| uriPermissionMode | String                   | 标识ability中的uriPermissionMode信息              | NA                              |
| uriPermissionPath | String                   | 标识ability中的uriPermissionPath信息              | NA                              |
| directLaunch      | boolean                  | 标识ability中的directLaunch信息                   | NA                              |
| mission           | String                   | 标识ability中的mission信息                        | NA                              |
| targetAbility     | String                   | 标识ability中的targetAbility信息                  | NA                              |
| multiUserShared   | boolean                  | 标识ability中的multiUserShared信息                | NA                              |
| supportPipMode    | boolean                  | 标识ability中的supportPipMode信息                 | NA                              |
| srcLanguage       | String                   | 标识ability中的srcLanguage信息                    | NA                              |
| srcPath           | String                   | 标识ability中的srcPath信息                        | NA                              |
| srcEntrance       | String                   | 标识ability中的srcEntrance信息                    | NA                              |
| continuable       | boolean                  | 标识ability中的continuable信息                    | NA                              |
| metaData          | MetaData结构体（见下述MetaData） | 标识ability的自定义元信息                  | NA                             |
| configChanges     | List\<String>            | 标识ability中的configChanges信息                  | NA                              |
| formInfos         | List\<AbilityFormInfo>   | 标识ability中的forms信息                          | NA                              |
| permissions       | List\<String>            | 标识ability中的permissions信息                    | NA                              |
| skills            | List\<SkillInfo>         | 标识ability中的skills信息                         | NA                              |
| backgroundModes   | List\<String>            | 标识ability中的backgroundModes信息                | NA                              |
| labels            | HashMap\<String, String> | 标识多语言下ability对用户显示的名称                | NA                             |
| descriptions      | HashMap\<String, String> | 标识多语言下ability的描述                         | NA                              |

### 4.7 Distro结构体信息

| 字段                | 类型    | 描述                                    | 备注                                                         |
| ------------------- |---------| --------------------------------------- | ------------------------------------------------------------ |
| moduleName          | String  | 标识当前module的名字                    | 对应Stage模型中module结构体中的moduleName字段                 |
| moduleType          | String  | 标识当前hap的类型                       | 对应Stage模型中module结构体中的moduleType字段                 |
| deliveryWithInstall | boolean | 标识当前hap是否在用户主动安装的时候安装 | 对应Stage模型中module结构体中的deliveryWithInstall字段         |
| installationFree    | int     | 标识当前hap是否支持免安装特性           | 对应Stage模型中module结构体中的installationFree字段，json文件中如果配置了该字段为true，返回1，配置为false，返回0，未配置返回2 |
| virtualMachine      | String  | 标识当前hap运行的目标虚拟机类型，供云端分发使用，如应用市场和分发中心 | 对应Stage模型中module结构体中的virtualMachine字段 |

### 4.8 MetaData结构体信息

| 字段           | 类型                 | 描述                             | 备注                      |
| -------------- |----------------------| -------------------------------- | ------------------------- |
| parameters     | List\<MetaDataInfo>  | 标识Metadata的参数信息           | FA模型特有，stage模型废弃  |
| results        | List\<MetaDataInfo>  | 标识Metadata的results信息        | FA模型特有，stage模型废弃  |
| customizeDatas | List\<CustomizeData> | 标识Metadata的customizeDatas信息 | NA                         |

### 4.9 MetaDataInfo结构体信息

| 字段        | 类型   | 描述                              | 备注                      |
| ----------- | ------ | --------------------------------- | ------------------------- |
| name        | String | 标识MetaDataInfo的name信息        | FA模型特有，stage模型废弃  |
| description | String | 标识MetaDataInfo的description信息 | FA模型特有，stage模型废弃  |
| type        | String | 标识MetaDataInfo的type信息        | FA模型特有，stage模型废弃  |

### 4.10 CustomizeData结构体信息

| 字段  | 类型   | 描述                         | 备注                    |
| ----- | ------ | ---------------------------- | ----------------------- |
| name  | String | 标识CustomizeData的name信息  | 对应stage模型的metadata |
| value | String | 标识CustomizeData的value信息 | 对应stage模型的metadata |
| extra | String | 标识CustomizeData的extra信息 | 对应stage模型的metadata |

### 4.11 ReqPermission结构体信息

| 字段      | 类型                               | 描述                                                       | 备注 |
| --------- | ---------------------------------- |------------------------------------------------------------| ---- |
| name      | String                             | 标识ReqPermission的名称信息                                 | NA   |
| reason    | String                             | 当申请的权限为user_grant权限时此字段必填，描述申请权限的原因 | NA   |
| usedScene | UsedScene结构体（见下述UsedScene） | 描述权限使用的场景和时机。场景类型有：ability、调用时机（when），可配置多个ability | NA   |
| reasons   | HashMap\<String, String>           | 当申请的权限为user_grant权限时此字段必填，描述申请权限的原因 | NA    |

### 4.12 UsedScene结构体信息

| 字段    | 类型          | 描述                                                         | 备注 |
| ------- | ------------- | ------------------------------------------------------------ | ---- |
| ability | List\<String> | 标识需要使用到该权限的元能力（ability），该标签值为数组形式   | NA   |
| when    | String        | 标识使用该权限的时机，值为inuse/always，表示为仅允许前台使用和前后台都可使用 | NA   |

### 4.13 Shortcut结构体信息

| 字段       | 类型                   | 描述                                                         | 备注 |
| ---------- | ---------------------- | :----------------------------------------------------------- | ---- |
| shortcutId | String                 | 标识ShortCut的Id                                             | NA   |
| label      | String                 | 标识ShortCut的标签信息                                       | NA   |
| icon       | String                 | 标识ShortCut的图标信息                                       | NA   |
| intents    | List\<IntentInfo>      | 标识快捷方式内定义的目标intent信息集合，每个intent可配置两个子标签，targetClass,targetBundle | NA   |
| labels     | HashMap\<String, String> | 标识多语言下ShortCut对用户显示的名称                        | NA   |

### 4.14 IntentInfo结构体信息

| 字段         | 类型   | 描述             | 备注 |
| ------------ | ------ | ---------------- | ---- |
| targetClass  | String | 快捷方式目标类型 | NA   |
| targetBundle | String | 快捷方式目标包名 | NA   |

### 4.15 DistroFilter结构体信息

| 字段          | 类型                | 描述                                  | 备注 |
| ------------- | ------------------- | ------------------------------------- | ---- |
| apiVersion    | ApiVersion结构体    | 标识DistroFilter中的apiVersion信息    | NA   |
| screenShape   | ScreenShape结构体   | 标识DistroFilter中的screenShape信息   | NA   |
| screenDensity | ScreenDensity结构体 | 标识DistroFilter中的screenDensity信息 | NA   |
| screenWindow  | ScreenWindow结构体  | 标识DistroFilter中的screenWindow信息  | NA   |
| countryCode   | CountryCode结构体   | 标识DistroFilter中的countryCode信息   | NA   |

### 4.16 ApiVersion结构体信息

| 字段   | 类型          | 描述                     | 备注 |
| ------ | ------------- | ------------------------ | ---- |
| policy | String        | 标识结构体中的policy信息 | NA   |
| value  | List\<String> | 标识结构体中的value信息  | NA   |

### 4.17 ScreenShape结构体信息

| 字段   | 类型          | 描述                     | 备注 |
| ------ | ------------- | ------------------------ | ---- |
| policy | String        | 标识结构体中的policy信息 | NA   |
| value  | List\<String> | 标识结构体中的value信息  | NA   |

### 4.18 ScreenDensity结构体信息

| 字段   | 类型          | 描述                     | 备注 |
| ------ | ------------- | ------------------------ | ---- |
| policy | String        | 标识结构体中的policy信息 | NA   |
| value  | List\<String> | 标识结构体中的value信息  | NA   |

### 4.19 ScreenWindow结构体信息

| 字段   | 类型          | 描述                     | 备注 |
| ------ | ------------- | ------------------------ | ---- |
| policy | String        | 标识结构体中的policy信息 | NA   |
| value  | List\<String> | 标识结构体中的value信息  | NA   |

### 4.20 CountryCode结构体信息

| 字段   | 类型          | 描述                     | 备注 |
| ------ | ------------- | ------------------------ | ---- |
| policy | String        | 标识结构体中的policy信息 | NA   |
| value  | List\<String> | 标识结构体中的value信息  | NA   |

### 4.21 ExtensionAbilityInfo结构体信息

| 字段            | 类型                     | 描述                                                  | 备注                                |
| --------------- | ------------------------ | ----------------------------------------------------- | ----------------------------------- |
| name            | String                   | 标识当前extensionAbility的逻辑名                      | stage模型支持                      |
| srcEntrance     | String                   | 标识extensionAbility所对应的js代码路径                | stage模型支持                      |
| icon            | String                   | 标签标识extensionAbility图标                          | stage模型支持                      |
| label           | String                   | 标识extensionAbility对用户显示的名称                  | stage模型支持                      |
| description     | String                   | 标识extensionAbility的描述                            | stage模型支持                      |
| type            | String                   | 标识extensionAbility的类型：form、workScheduler、inputMethod、service、accessibility、dataShare、fileShare、wallpaper、backup | stage模型支持，目前仅解析了form、staticSubscriber的信息、其他类型（如：workScheduler、inputMethod、service、accessibility、dataShare、fileShare、wallpaper、backup）暂未解析        |
| permissions     | List\<String>            | 标识被其它应用的ability调用时需要申请的权限的集合       | stage模型支持                      |
| readPermission  | String                   | 标识读取ability的数据所需的权限                         | stage模型支持                    |
| writePermission | String                   | 标识向ability写数据所需的权限                           | stage模型支持                   |
| visible         | boolean                  | 标识extensionAbility是否可以被其它应用调用              | stage模型支持                   |
| skills          | List\<SkillInfo>         | 标识extensionAbility能够接收的意图的特征集              | stage模型支持                    |
| metadataInfos   | List\<ModuleMetadataInfo>| 标识extensionAbility能够接收的元数据信息                | stage模型支持                    |
| metadata        | MetaData结构体           | 标识extensionAbility的元信息                            | 将metadata中的信息赋值到CustomizeData中 |
| uri             | String                   | 标识extensionAbility提供的数据uri                       | stage模型支持                           |
| descriptions    | HashMap\<String, String> | 标识多语言下extensionAbility的描述                      | NA                              |
| labels          | HashMap\<String, String> | 标识多语言下extensionAbility对用户显示的名称            | NA                             |

### 4.22 SkillInfo结构体信息

| 字段     | 类型                | 描述                   | 备注 |
| -------- | ------------------- |----------------------| ---- |
| actions  | List\<String>       | 标识能够接收的意图的action值的集合 | NA   |
| entities | List\<String>       | 标识能够接收的意图的元能力的类别集合   | NA   |

### 4.23 UriInfo结构体信息

| 字段          | 类型   | 描述                     | 备注 |
| ------------- | ------ |------------------------| ---- |
| schema        | String | 标识ModuleUriInfo的范式信息   | NA   |
| host          | String | 标识ModuleUriInfo的宿主信息   | NA   |
| port          | String | 标识ModuleUriInfo的端口信息   | NA   |
| pathStartWith | String | 标识ModuleUriInfo的路径前缀   | NA   |
| pathRegex     | String | 标识ModuleUriInfo的路径正则信息 | NA   |
| path          | String | 标识ModuleUriInfo的路径信息   | NA   |
| type          | String | 标识ModuleUriInfo的种类     | NA   |

### 4.24 AbilityFormInfo结构体信息

| 字段                | 类型                     | 描述                                                         | 备注        |
| ------------------- | -------------------------| ------------------------------------------------------------ |-----------|
| name                | String                   | 标识forms的名称                                              | NA        |
| type                | String                   | 标签标识卡片的类型                                           | NA        |
| updateEnabled       | boolean                  | 标识该卡片是否支持定时刷新                                   | NA        |
| scheduledUpdateTime | String                   | 标签标识卡片顶点刷新的时间，采用24小时计数，精确到分钟       | NA        |
| updateDuration      | int                      | 标识卡片定时刷新的更新频率，单位为30分钟，取值为30的倍数值   | NA        |
| supportDimensions   | List\<String>            | 标识卡片外观规格，取值为”1 * 2“，”2 * 2“，”2 * 4“，”4 * 4“   | NA        |
| defaultDimension    | String                   | 标识卡片默认外观规格，取值必须在supportDimensions配置的列表中 | NA        |
| MetaData            | MetaData                 | 标识卡片的自定义信息                                         | NA        |
| description         | String                   | 标识forms的描述                                              | stage模型新增 |
| src                 | String                   | 标签JS卡片对应的UI代码                                       | NA        |
| windowInfo          | ModuleWindowInfo结构体   | 标签能力窗体的窗口。                                       | NA        |
| isDefault           | boolean                  | 标识该卡片是否为默认卡片，每个hap有且只能有一个默认卡片      | NA        |
| colorMode           | String                   | 标识卡片的色调，取值为auto、dark、light其中之一              | NA        |
| formConfigAbility   | String                   | 标识卡片调整的Ability名称                                    | NA        |
| formVisibleNotify   | String                   | 标识卡片是否被允许使用卡片可见性通知                         | NA        |
| providerAbility     | String                   | 卡片的提供方所在的Ability或者extension名称，1.FA模型：如果卡片配置在service类型的ability中，providerAbility配置为mainAbility 2.FA模型：如果卡片配置在Page类型的Ability中，providerAbility配置为当前Ability 3.FA模型：如果没有配置mainAbility，providerAbility配置为当前hap包中的优先使用system.home，否则第一个page的Ability 4.stage模型中（follow上述规则），providerAbility配置为mainElement | NA |
| descriptions        | HashMap\<String, String> | 标识多语言下ability的描述                                     | NA     |



### 4.25 CommonEvent结构体信息

| 字段       | 类型          | 描述                                   | 备注                                             |
| ---------- | ------------- | -------------------------------------- | ------------------------------------------------ |
| name       | String        | 当前静态公共事件对应的类名             | Stage模型从staticSubscriber类型的Extension中获取 |
| permission | String        | 标识实现该静态公共事件需要申请的权限   | Stage模型从staticSubscriber类型的Extension中获取 |
| data       | List\<String> | 当前静态公共时间需要携带的额外数据数组 | Stage模型从staticSubscriber类型的Extension中获取 |
| type       | List\<String> | 配置当前静态公共时间的类别数组         | Stage模型从staticSubscriber类型的Extension中获取 |
| events     | List\<String> | 标识能够接收的意图的event值的集合      | Stage模型从staticSubscriber类型的Extension中获取 |

### 4.26 DependencyItem结构体信息

| 字段           | 类型   | 描述           | 备注 |
|--------------| ------ |--------------| ---- |
| bundleName   | String | 共享包的bundleName | NA   |
| moduleName   | String | 共享包的moduleName | NA   |
| versionCode  | String | 共享包的版本号      | NA   |

### 4.27 ModuleAtomicService结构体信息

| 字段         | 类型                   | 描述           | 备注 |
|--------------|------------------------|----------------| ---- |
| preloadItems | list\<PreloadItem>     | 预加载对象     | NA   |

### 4.28 PreloadItem结构体信息

| 字段         | 类型   | 描述           | 备注 |
|--------------|--------|----------------| ---- |
| moduleName   | String | 预加载的模块名 | NA   |

### 4.29 DeviceConfig结构体信息

| 字段                           | 类型    | 描述                                     | 备注 |
|--------------------------------|-------- |------------------------------------------| ---- |
| targetReqSdk                   | String  | 标识应用程序DeviceConfig的目标请求Sdk版本  | NA   |
| compatibleReqSdk               | String  | 标识应用程序DeviceConfig的兼容请求Sdk版本  | NA   |
| jointUserid                    | String  | 标识应用程序DeviceConfig的jointUserid      | NA   |
| process                        | String  | 标识应用程序DeviceConfig的进程             | NA   |
| arkFlag                        | String  | 标识应用程序DeviceConfig的arkFlag          | NA   |
| targetArkVersion               | String  | 标识应用程序DeviceConfig的targetArkVersion | NA   |
| compatibleArkVersion           | String  | 标识应用程序DeviceConfig的兼容ArkVersion   | NA   |
| directLaunch                   | boolean | 标识应用程序DeviceConfig的直接启动         | NA   |
| distributedNotificationEnabled | boolean | 标识应用程序AppJson的distributedNotificationEnabled | NA   |

### 4.30 DefPermission结构体信息

| 字段           | 类型                     | 描述                                      | 备注 |
|----------------|--------------------------|-------------------------------------------| ---- |
| name           | String                   | 标识指示DefPermission的名称               | NA   |
| grantMode      | String                   | 标识DefPermission的grantMode              | NA   |
| group          | String                   | 标识DefPermission的组                     | NA   |
| label          | String                   | 标识DefPermission的标签                   | NA   |
| description    | String                   | 标识DefPermission的描述                   | NA   |
| availableScope | List\<String>            | 标识DefPermission的可用范围               | NA   |
| labels         | HashMap\<String, String> | 标识多语言应用程序DefPermission的标签     | NA   |
| descriptions   | HashMap\<String, String> | 标识多语言应用程序DefPermission的说明     | NA   |

### 4.31 DefinePermission结构体信息

| 字段                   | 类型                     | 描述                                               | 备注 |
|------------------------|--------------------------|----------------------------------------------------| ---- |
| name                   | String                   | 标识DefPermission的名称                         | NA   |
| grantMode              | String                   | 标识DefPermission的grantMode                        | NA   |
| availableLevel         | String                   | 标识DefPermission的组                               | NA   |
| provisionEnable        | boolean                  | 标识模块定义权限的提供启用                           | NA   |
| distributedSceneEnable | boolean                  | 标识ModuleDefinePermissions的distributedSceneEnable | NA   |
| label                  | String                   | 标识DefPermission的标签                              | NA   |
| description            | String                   | 标识DefPermission的描述                              | NA   |
| descriptions           | HashMap\<String, String> | 标识多语言应用程序DefPermission的说明                | NA   |
| labels                 | HashMap\<String, String> | 标识多语言应用程序DefPermission的标签                | NA   |

### 4.32 DefPermissionsGroups结构体信息

| 字段        | 类型    | 描述                         | 备注 |
|-------------|---------|------------------------------| ---- |
| name        | String  | 标识DefPermissionGroup的名称 | NA   |
| order       | String  | 标识DefPermissionGrou的顺序  | NA   |
| icon        | String  | 标识DefPermissionGroup的图标 | NA   |
| label       | String  | 标识DefPermissionGroup的标签 | NA   |
| description | String  | 标识DefPermissionGroup的描述 | NA   |
| request     | boolean | 标识DefPermissionGroup的请求 | NA   |

### 4.33 FormInfo结构体信息

| 字段          | 类型          | 描述                     | 备注 |
|---------------|---------------|--------------------------| ---- |
| formEntity    | List\<String> | 标识formInfo的formEntity | NA   |
| minHeight     | String        | 标识formInfo的最小高度   | NA   |
| defaultHeight | String        | 标识formInfo的默认高度   | NA   |
| minWidth      | String        | 标识formInfo的最小宽度   | NA   |
| defaultWidth  | String        | 标识formInfo的默认宽度   | NA   |

### 4.34 ModuleMetadataInfo结构体信息

| 字段     | 类型    | 描述                         | 备注 |
|----------|---------|------------------------------| ---- |
| name     | String  | 标识ModuleMetadataInfo的名称 | NA   |
| value    | String  | 标识ModuleMetadataInfo的值   | NA   |
| resource | String  | 标识ModuleMetadataInfo的资源 | NA   |

### 4.35 ModuleWindowInfo结构体信息

| 字段            | 类型    | 描述                                | 备注 |
|-----------------|---------|-------------------------------------| ---- |
| designWidth     | int     | 标识模块已用场景的设计宽度           | NA   |
| autoDesignWidth | boolean | 标识ModuleUsedScene的autoDesignWidth | NA   |