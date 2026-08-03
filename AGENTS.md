# AGENTS.md

本文件适用于 `developtools_packing_tool` 仓库根目录及其全部子目录；若更深层目录新增了 `AGENTS.md`，以更深层文件为准。

## 1. 开始任务前的工作契约

本文件是工程导航与治理入口，不替代 `README_zh.md`、实现代码、构建定义或 Git 已跟踪的专项说明。每次开始任务时：

1. 用 Git 或实际构建工具复核当前分支、工作区状态、提交 ID 和测试结果，不要把历史描述当作当前仓库证据。
2. 只把 Git 已跟踪的代码、构建文件、测试和说明作为仓库知识来源；先读取与任务直接相关的内容，再规划修改。
3. 保留工作区中与当前任务无关的用户改动和本地构建状态，不要擅自删除、覆盖或纳入提交。
4. 在第一次实施进展说明或计划中明确写出：任务类别、目标产物（Java JAR / C++ 可执行文件 / 两者）、已读取的仓内材料、发现的高风险约束、计划执行的验证。完成这一步前不要编辑代码。

## 2. Code map

本仓是 OpenHarmony 的主机侧应用包工具。Java 工具链包含打包、拆包、扫描和 haptobin 工具；C++ 工具链只包含打包工具 `ohos_packing_tool`，不包含拆包工具、扫描工具或 haptobin 工具。对于 Java 与 C++ 共同具备的打包能力，代码修改默认要求两端行为对齐并分别验证；某项能力仅存在于一端时，不要求在另一端新增该能力。

### Java 与 C++ 版本边界

| 项目 | Java 工具链 | C++ 打包工具 |
| --- | --- | --- |
| 主源码目录 | `adapter/ohos/` | `packing_tool/frameworks/` |
| 命令入口 | 打包 `CompressEntrance.java`；拆包 `UncompressEntrance.java`；扫描 `ScanEntrance.java`；haptobin `ConvertHapToBin.java` | `src/main.cpp` → `ShellCommand` → 各类 `Packager`；仅支持打包相关模式 |
| 构建入口 | 根 `BUILD.gn` 的 `packing_tool("packing_tool")`，经 `packingtool.gni` 调用 `build.py` 和根目录 Shell 脚本 | 根 `BUILD.gn` 的 `ohos_group("ohos_packing_tool")`，依赖 `packing_tool/frameworks:ohos_packing_tool` |
| 主要产物 | `app_packing_tool.jar`、`app_unpacking_tool.jar`、`app_check_tool.jar`、`haptobin_tool.jar` | `ohos_packing_tool` 可执行文件 |
| 参数与帮助 | `CommandParser.java`、`Constants.java`、`ShowHelp.java` | `src/shell_command.cpp`、`include/constants.h` |
| JSON/包处理 | `JsonUtil.java`、`PackInfo.java`、`Compressor.java`、`Uncompress.java` 等 | `src/json/`、各 `*_packager.cpp`、ZIP 层；解压能力仅供打包/规格化内部流程使用，不构成拆包工具 |
| 测试与验证 | Java 8 编译及对应 JAR 命令行/产物烟测 | `packing_tool/frameworks/test/unittest/` 下的 GTest/HWTEST |

### 目录职责

| 路径 | 职责 | 风险 |
| --- | --- | --- |
| `packing_tool/frameworks/include/` | C++ 公共类型、Packager 抽象、命令参数、ZIP/JSON/去重接口 | 公共接口和跨模块约束集中区 |
| `packing_tool/frameworks/src/` | C++ 打包、规格化、ZIP、打包期 SO 去重和命令分发实现 | 主执行路径；文件写入和清理属于高风险边界，不要据内部解压/扫描辅助代码推断存在独立拆包或扫描工具 |
| `packing_tool/frameworks/src/json/` | `module.json`、`config.json`、`pack.info`、`patch.json` 解析与修改 | 包格式和跨版本兼容边界 |
| `packing_tool/frameworks/src/dedup/` | APP 内 SO 去重、设备/模块集合计算、报告生成 | 安装语义、原包重写和文件删除边界 |
| `packing_tool/frameworks/test/unittest/` | C++ GTest/HWTEST 单元测试及夹具 | 新增源码或能力时同步更新对应 `BUILD.gn` |
| `adapter/ohos/` | Java 打包、解包、扫描、资源解析和各 JAR 入口 | Java 8 兼容性与 JAR 对外行为边界 |
| `adapter/scanner/` | Java `app_check_tool.jar` 的使用说明 | 扫描参数或输出变化时同步核对 |
| `build.py`、`packingtool.gni`、根目录 `*.sh` | Java 源码选择、编译和 JAR 组装 | Java 构建事实来源；新增 Java 类时检查源码清单 |
| `META-INF/` | Java 各 JAR 的 Main-Class 清单 | 入口类和产物启动边界 |
| `configcheck/`、`modulecheck/` | 配置和模块 JSON Schema | 配置契约边界；变更需评估旧工程兼容性 |
| `BUILD.gn`、`packingtool.gni`、`bundle.json` | 组件、Java 工具和安装产物的构建入口 | 源文件、依赖和产物变更必须同步登记 |
| `packing_tool/frameworks/BUILD.gn` | C++ 可执行文件 `ohos_packing_tool` 的主构建定义 | C++ 新增/删除源文件和依赖的事实来源 |
| `README_zh.md` | 已发布 CLI 用法、包解析 Java API 和数据结构说明 | 对外接口与兼容性核对入口，不得用 README 代替实现验证 |

### Where to look

| 任务类型 | Java 版本先看 | C++ 版本先看 |
| --- | --- | --- |
| 命令行参数、模式分发、帮助文本 | `CommandParser.java`、`Constants.java`、`ShowHelp.java`、对应 Entrance | `shell_command.cpp`、`constants.h`、对应 Packager |
| HAP/HSP/APP/MultiApp/FastApp 打包 | `CompressEntrance.java`、`Compressor.java`、`CompressVerify.java` | 对应 `*_packager.cpp`、`packager.cpp`、`zip_wrapper.cpp` |
| 解包或包信息解析 | `UncompressEntrance.java`、`Uncompress.java`、`UncompressVerify.java` | 不适用；C++ 没有拆包工具或公开包解析接口 |
| HQF/APPQF/增量包 | `HQFVerify.java`、`IncrementalPack.java`、`JsonUtil.java` | `hqf_packager.cpp`、`hqf_verify.cpp`、`appqf_packager.cpp`、`incremental_pack.cpp` |
| ZIP 条目或路径安全 | `Utility.java`、`FileUtils.java`、`RawZipEntryCopier.java` 及调用者 | `unzip_wrapper.cpp`、`zip_wrapper.cpp`、`zip_utils.cpp`、`utils.cpp` 及同名测试 |
| `module.json` / `config.json` | `JsonUtil.java`、`ModuleJsonUtil.java`、相关模型类 | `src/json/module_json*.cpp`、`module_json_utils.cpp` |
| `pack.info` / `patch.json` | `PackInfo.java`、`JsonUtil.java`、HQF 相关类 | `src/json/pack_info*.cpp`、`patch_json*.cpp` |
| 版本、包名或通用规格化 | `Compressor.java` 中对应 normalize 流程 | `version_normalize.cpp`、`package_normalize.cpp`、`general_normalize.cpp` 及 JSON helpers |
| 独立扫描工具 | `ScanEntrance.java`、`Scan.java`、各 `ScanStat*.java` | 不适用；C++ 没有扫描工具 |
| 打包期 SO 去重 | `SODeduplicator.java` 及 Java 打包调用链 | `scan_statdulpicate.cpp`、`src/dedup/` 及 APP/FastApp/MultiApp Packager；两端默认对齐 |
| haptobin | `ConvertHapToBin.java`、`haptobin.sh` 和对应 Manifest | 不适用；C++ 没有 haptobin 工具 |
| 构建和产物 | 根 `BUILD.gn`、`packingtool.gni`、`build.py`、根目录 Shell 脚本、`META-INF/` | 根 `BUILD.gn`、`packing_tool/frameworks/BUILD.gn`、`bundle.json` |
| 测试 | Java 8 编译 + 对应 JAR 烟测 | `packing_tool/frameworks/test/unittest/<component>/` + 该目录 `BUILD.gn` |
| 已发布 CLI 或 Java 包解析 API | `README_zh.md` 对应命令/API 表 + `adapter/ohos/` 入口、模型与调用者 | 仅核对共同打包能力对应的 `shell_command.cpp`、`constants.h` 和测试；Java 专属工具/API 不向 C++ 扩展 |
| 错误码、错误文案或诊断输出 | Java 错误枚举/消息类和调用点 | `packing_tool/frameworks/include/error/`、`packing_tool/frameworks/src/error/`、`packing_tool/frameworks/src/log.cpp`；共同打包失败场景默认对齐 |

## 3. Knowledge routing

规划修改前，必须在进展说明或计划中声明任务类别、目标产物、实际已读材料、发现的约束和验证计划。只使用 Git 已跟踪内容作为路由目标，并用当前代码、测试和构建定义复核行为。

### Task-based routing

| 场景 | 必须先读/核对 |
| --- | --- |
| 改变共同打包模式的 CLI 参数、默认值或退出行为 | 同时读取 Java 与 C++ 的入口、参数常量、帮助文本和测试；默认同步修改并分别验证两端 |
| 修改拆包、扫描或 haptobin | 仅路由到 Java 入口、实现、脚本、Manifest 和测试；C++ 不具备这些工具，不得为对齐而擅自新增 |
| 改变已发布 Java 包解析 API、返回模型或字段语义 | `README_zh.md` 的“包解析接口/拆包工具信息字段”、`UncompressEntrance.java`、返回模型、调用者和测试；把签名、空值、异常、错误码和字段语义视为兼容性契约 |
| 改变包内文件、清单或报告格式 | 对应 JSON 类、Schema、打包与解包两端、已有测试夹具；评估旧包能否继续解析 |
| 修改路径拼接、解压、覆盖、删除或临时目录 | `packager.cpp`、`utils.cpp`、ZIP 层和所有调用者；建立输入到最终文件操作的完整数据流 |
| 修改 C++ JSON 数据访问 | `pt_json.h/.cpp` 及目标 JSON 类；区分返回值式和输出参数式 API，并检查每个 `Result` |
| 修改 SO 去重、报告或错误日志 | Java `SODeduplicator.java` 与打包调用链、C++ `src/dedup/`、`pack_info`、APP/FastApp/MultiApp 调用链及两端测试；默认保持两端共同语义一致 |
| 修改路径、ZIP 解压、临时目录或失败清理 | 核对当前 Java/C++ 共同打包链路、ZIP 实现和回归测试；Java 拆包/扫描专属路径只修改并验证 Java |
| 修改 Java 打包 | `adapter/ohos/` 对应 Java 类、C++ 等价打包实现、`build.py` 源码清单、相关 JAR Manifest/脚本和两端测试；默认对齐 C++，并以 Java 8 作为语法兼容基线 |
| 修改 C++ 打包 | `packing_tool/frameworks/` 对应类、Java 等价打包实现、主 `BUILD.gn`、两端相邻测试和根 `ohos_packing_tool` 依赖链；默认对齐 Java |
| 新增源文件、测试或第三方依赖 | 对应 `BUILD.gn`、根构建入口和 `bundle.json`；第三方依赖还需完成许可证与组件依赖评估 |

### Path-based routing

| 修改路径 | 同时检查 |
| --- | --- |
| `packing_tool/frameworks/src/*packager.cpp` | 基类 `packager.*`、模式分发、对应 C++ 测试和 Java 等价打包实现；默认保持两端行为一致 |
| `packing_tool/frameworks/src/json/` | `pt_json.*`、Schema、打包/解包消费者和 JSON 单测 |
| `packing_tool/frameworks/src/{zip,unzip}_wrapper.cpp` | 路径校验、错误关闭、CRC/写入失败处理和 ZIP 测试 |
| `packing_tool/frameworks/src/dedup/` | APP 系列 Packager、`pack.info`、报告格式和 `test/unittest/dedup/` |
| `adapter/ohos/` 的打包实现 | `build.py` 源码清单、对应 JAR 入口/Manifest、Java 8 编译和 C++ 等价打包实现；默认保持两端行为一致 |
| `adapter/ohos/` 的拆包、扫描、haptobin 实现 | 对应 Java 入口、脚本、Manifest 和测试；不路由到 C++ |
| `build.py`、`packingtool.gni`、根目录 Shell 脚本 | 四个 Java JAR 的源码集合、依赖、Main-Class、输出路径和 SDK/image 差异 |
| `modulecheck/` 或 `configcheck/` | 解析实现、合法/非法样例、旧配置兼容性 |
| `packing_tool/frameworks/include/error/`、`src/error/`、`src/log.cpp` | Java 共同打包场景的错误定义/调用点、CLI 输出、测试和错误码兼容性 |
| `README_zh.md` 的 CLI/API/字段表 | 对应 Java 入口与模型、C++ CLI（若适用）、测试和实际产物；文档与行为必须同步 |
| `packing_tool/frameworks/src/dedup/` | Java `SODeduplicator.java`、`pack.info`、报告、APP 系列调用链和两端去重测试 |
| 任意 `BUILD.gn` / `.gni` / `bundle.json` | 源文件列表、依赖方向、组件名、测试聚合和全量构建影响 |

### Vocabulary-based routing

| 术语 | 本仓含义与风险 | 先看 |
| --- | --- | --- |
| HAP / HSP / APP | OpenHarmony 应用模块、共享包和应用包格式，不是普通 ZIP；包内清单与目录结构是兼容性契约 | `README_zh.md` 对应命令、目标 Packager、JSON 层和测试 |
| HQF / APPQF | 快速修复包及其聚合包；版本和目标包匹配影响补丁可用性 | `hqf_*`、`appqf_packager.*`、`patch_json*` |
| FA / Stage | 两代应用模型，分别主要使用 `config.json` 与 `module.json`；修改一侧时必须确认另一侧行为 | `module_json_fa.cpp`、`module_json_stage.cpp` |
| normalize | 对既有包的版本、包名或通用字段重写，不是普通重新打包；必须保留无关条目和元数据 | 三类 normalize 实现及测试 |
| ZIP Slip / traversal | ZIP 条目名或命令参数逃逸目标目录的安全问题；校验必须发生在文件系统操作前 | ZIP 层、`utils.cpp`、安全回归测试 |
| `PtJson::Result` | 输出参数是否有效的显式状态；不得忽略后继续使用旧值或未初始化值 | `pt_json.h/.cpp` |
| SO deduplication / `deduplicateSo` / `[SO_DEDUP]` | 打包期原生库去重；同时牵涉报告、错误码、日志和安装后等价性，不代表 C++ 提供扫描工具 | Java `SODeduplicator.java`、C++ `src/dedup/`、APP 系列调用链和两端测试 |
| pack.info | APP/HQF 等聚合包元数据；字段语义同时影响打包、解包和下游工具 | `pack_info*` + Java `PackInfo.java` |
| error code / `Error Message` / `Try the following` | 用户和自动化可观察的失败契约，不是可随意改写的日志文本 | Java/C++ error 定义、调用点和相关测试；SO 去重核对两端错误标识与日志结构 |

## 4. Constraints and boundaries

### Architecture/domain invariants

- `Packager::MakePackage()` 的阶段顺序和失败传播属于所有模式共享约束；改变基类流程前检查全部派生类。
- CLI 模式、参数名、默认值、错误码、输出后缀和包内路径均是对外行为；除非任务明确要求，不得静默改变。
- `README_zh.md` 列出的 `UncompressEntrance` Java API、返回模型及字段语义是公开兼容边界；修改签名、空值/异常行为、字段类型或解析语义时必须同步评估调用方和文档。
- HAP/HSP/APP/HQF/APPQF/RES 是 ZIP 容器但具有固定清单和目录语义；不得用“ZIP 可打开”代替格式正确性验证。
- FA 与 Stage 模型长期并存；修改清单解析、校验或规格化时必须验证两种模型的预期范围。
- Java JAR 与 C++ 可执行文件是独立产物，但共同打包能力的行为默认必须对齐：修改任一端时同步检查、修改并验证另一端。拆包、扫描和 haptobin 是 Java 专属能力，不因“默认对齐”而扩展到 C++。
- 所有来自 CLI、JSON、ZIP 条目和包内元数据的路径都视为外部输入。创建、覆盖、重命名和递归删除前必须证明目标边界。
- `PtJson` 输出参数式访问必须先检查 `Result` 再消费输出值；类型错误、字段缺失和数值越界不能被当作成功。
- 新增 C++ 源文件必须进入 `packing_tool/frameworks/BUILD.gn`；新增单测必须进入对应测试 `BUILD.gn` 及聚合目标。
- `configcheck/`、`modulecheck/` 中的 Schema 和源码/构建清单是事实来源；生成包、JAR、Class 和临时产物不能反向手改后当作源码修复。
- 错误码、错误级别、`Error Message`/`Try the following` 结构和诊断前缀属于可观察行为；修复时保留足以定位输入、阶段和失败原因的信息，且不得记录包内敏感内容或密钥材料。

### Do not

- 不要为通过测试而放宽路径、ZIP 条目、JSON 类型、包结构或签名校验。
- 不要对用户提供的输入目录或输出目录执行无边界的递归删除；失败清理只能删除本次创建且已证明归属的产物。
- 不要在路径校验前创建目录、打开输出文件、重命名或删除文件。
- 不要忽略 minizip/cJSON/文件流的返回值、关闭失败、CRC 错误或部分写入。
- 不要把包格式、Schema、`pack.info` 或报告字段改动当作内部重构；必须进行兼容性评估。
- 不要只修改 Java/C++ 一端的共同打包能力；确实无法对齐时，必须先说明原因并取得用户确认。
- 不要把 Java 专属的拆包、扫描或 haptobin 能力扩展到 C++，除非用户明确提出新增能力。
- 不要手工修改生成产物、编译输出、JAR/Class 或临时产物来伪造验证结果。
- 不要新增第三方依赖而不更新构建声明、`bundle.json` 和许可证评估。
- 不要添加源文件或测试后遗漏对应 `BUILD.gn` 和测试聚合项。
- 不要为减少输出或对齐测试而吞掉错误、降低错误级别、删除诊断上下文，或把不同失败原因折叠为成功。

### Ask before

- 删除或重命名既有 CLI 参数、模式、输出文件、包内条目或 JSON 字段。
- 改变默认行为、错误码、退出码、兼容旧包/旧配置的策略或容错范围。
- 放宽路径、文件类型、Schema、签名、哈希或包结构校验。
- 改变递归清理范围、允许覆盖既有目录/文件，或触碰任务范围外的真实用户产物。
- 修改 SO 去重的等价性判定、设备安装集合算法、报告语义或失败策略。
- 修改公开 Java API 签名/返回模型、CLI 错误码/退出码、错误日志结构或其他下游可解析文本。
- 新增/升级第三方依赖，或修改组件发布、安装和许可证信息。
- 进行仓库外写入、修改父级 OpenHarmony 源码树、连接真实设备或执行不可恢复操作。

### Common failure patterns

- 只验证正常输入，没有覆盖 `..`、绝对路径、反斜杠、盘符路径、符号链接和部分写入失败。
- JSON getter 返回失败后仍使用输出参数，或把整型范围错误截断成有效值。
- 规格化/重打包时丢失未知 ZIP 条目、压缩属性或未修改的元数据。
- 修改 Packager 基类后只运行一个模式的测试。
- 测试直接访问私有成员成功，却没有覆盖真实 CLI 到文件系统的端到端路径。
- 将本机生成的临时目录、IDE 文件或测试包纳入提交。

## 5. Verification

官方 GN/HB 构建命令应从已配置产品的 OpenHarmony 源码根目录执行，而不是在本子仓目录假定存在完整构建环境。

### Minimum checks

根据变更范围至少执行：

```bash
# Java 打包工具链
hb build --build-target packing_tool

# C++ 主工具
hb build --build-target ohos_packing_tool

# C++ 受影响测试目标
hb build --build-target <affected_test_target>

# 补丁和空白错误
git diff --check
```

如果当前 checkout 没有已配置的 OpenHarmony 产品，必须明确报告“未运行”及具体阻塞原因；可补充运行仓库已有的局部语法检查，但不能把替代验证表述为官方构建通过。

本仓没有独立、统一的 lint 命令；不要虚构 lint 结果。至少执行 `git diff --check`，C++/Java 的静态、编译期检查分别以受影响 GN 目标和 Java 8 编译结果为准。

### Task-specific checks

| 变更类型 | 最小验证 |
| --- | --- |
| CLI 参数或模式分发 | `shell_command_test` + 实际执行帮助/成功/非法参数路径 |
| 某一 Packager | 对应 `<mode>_packager_test` + 至少一个真实产物打包并检查包内结构 |
| Packager 基类、ZIP 或通用文件工具 | 所有直接相关单测 + 至少 HAP/HSP/APP 各一条受影响路径；安全变更需增加失败用例 |
| JSON/Schema | 对应 JSON 单测 + 合法、缺失、错类型、边界值样例 + 受影响打包/解包路径 |
| normalize | 对应 normalize 单测 + 验证目标字段改变且无关条目保持不变 |
| SO 去重 | `test/unittest/dedup/` + APP/FastApp/MultiApp 受影响测试 + 报告可解析性和原包完整性 |
| Java 源码或构建脚本 | 构建 `packing_tool` 目标，或使用仓库声明依赖执行 `javac --release 8`；再运行受影响 JAR 的命令行/产物烟测 |
| C++ 源码 | 构建 `ohos_packing_tool` 目标 + 对应 GTest/HWTEST + 实际可执行文件烟测 |
| Java/C++ 共同打包能力 | 同时构建 Java `packing_tool` 和 C++ `ohos_packing_tool`，分别运行对应测试及等价 CLI/产物场景，核对成功结果、失败行为和包结构 |
| Java 专属拆包、扫描或 haptobin | 只构建并验证对应 Java JAR、入口和脚本；明确记录 C++ 不适用 |
| 构建配置 | 受影响 GN 目标 + `bundle.json`/依赖一致性检查 |
| 仅测试变更 | 运行新增/修改测试及至少一个相邻相关测试，证明测试既能通过也能捕获目标失败 |
| 公开 Java API、CLI、错误码或可解析输出 | 对照 `README_zh.md`/专项契约，运行兼容输入、非法输入和已有调用方式；文档、帮助文本、实现和测试保持一致 |
| 测试 demo/fixture | 逐条记录实际执行命令、退出码、产物和断言；生成 fixture 不等于执行通过 |
| 仅 `AGENTS.md` 或仓内说明 | `git diff --check` + 核对新增路径/命令在当前仓库存在或明确标注需在 OpenHarmony 根目录执行；按四维评分规则复评 |

### Done definition

任务仅在以下条件满足时完成：

1. 请求的行为已经实现，变更范围内没有遗留占位逻辑。
2. 相关构建、单测、静态/格式检查已运行；无法运行的项目记录了命令、原因和替代证据。
3. 新增/修改的测试覆盖成功路径、关键失败路径和兼容性边界。
4. `git diff` 中没有无关格式化、本地产物、临时文件或用户已有改动。
5. 外部格式、CLI、安全和兼容性影响已明确评估；共同打包能力已分别说明并验证 Java/C++ 对齐，Java 专属能力已明确标注 C++ 不适用。
6. 已复核最终 `git status`，确认没有误改、遗漏或纳入本地生成物。
7. 最终回复中的每个“通过”都能对应到本次实际执行的命令和结果；未运行项不会被替代检查冒充。

### Final response format

最终回复应包含：

- 变更摘要；
- 变更文件列表；
- 实际运行的验证命令和结果；
- CLI/包格式/兼容性/安全影响（如相关）；
- 未运行项、遗留风险或后续建议。

## 6. Code review

当用户要求“检视代码”“审查 PR”“安全审计”或类似只读评审时，默认先报告问题，不直接修改源码，除非用户同时明确要求修复。

- 先界定提交、diff、文件或模块范围，并读取本文件的对应路由项。
- 每条发现必须包含可定位的 `file:line`、触发条件、影响和判断依据；不报告只有风格偏好、没有行为影响的意见。
- 优先检查路径/文件系统边界、ZIP/JSON 外部输入、错误传播、资源关闭、整数边界、包格式兼容性以及 C++/Java 行为差异。
- 按严重程度排序，先列发现；若没有发现，明确说明仍未覆盖的测试或环境风险。
- 用户确认修复后，再按本文件的 Constraints 和 Verification 执行实现任务。
