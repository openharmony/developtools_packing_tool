# fastjson1.2.83.jar构建说明

1. 获取最新版本的说明文档，请参考如下：[fastjson README](https://github.com/alibaba/fastjson/blob/1.2.83/README.md)
2. 官方发布二进制获取地址：[fastjson1.2.83发布地址](https://github.com/alibaba/fastjson/releases/tag/1.2.83)
3. 构建指导

```bash
# 获取代码
git clone --branch 1.2.83 https://github.com/alibaba/fastjson.git
git checkout 1.2.83
# 编译fastjson1.2.83.jar，需要使用openjdk-8-jdk
mvn compile
mvn package -Dmaven.test.skip=true
```
