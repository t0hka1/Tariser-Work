# mugen迁移debian系调研

# 前言

这两周我对ubuntu/ubuntukylin/openkylin等发行版的测试框架进行了一些搜索，发现以下几种工具可能可以进行不同层面的测试

- Phoronix suite
    - 用途：性能测试
    - 结论：其实是一款linux通用的性能测试工具，功能测试难以顾及到
- autopkgtest
    - 用途：包构建自动化测试
    - 结论：发现这个工具已经被我们组的其他同学调研过，其结论是方案在ubuntu等衍生发行版兼容性一般
- 其他
    - 结论：其他谷歌到的工具一般也是以性能测试为主
    

而由于本人最近在学习mugen的进行测试，其实mugen这种主体为shell代码的测试框架，能在debian系有较优良的迁移性。

**迁移的好处：**

1.方便日后在debian系的操作系统上进行软件包的功能测试

2.组内大部分人比较熟悉mugen的操作

3.类似这种全面的测试包功能的自动化框架比较少

**迁移的麻烦点：**

1.需要对每个测试的包进行测试例的修改


# mugen初探

## 基本用法

### 安装依赖

`bash dep_install.sh`

### 配置测试环境节点

`bash [mugen.sh](http://mugen.sh) -c  --ip $ip  --password $passwd --user $user —port $port`

 环境变量文件位置:`./conf/env.json`

### 执行用例

- 执行所有用例 `bash mugen.sh -a`
- 执行指定测试套 `bash [mugen.sh](http://mugen.sh) -f testsuite`
- 执行单条用例 `bash mugen.sh -f testsuite -r testcase`
- 其他参数
    - `-s` 远程环境测试
    - `-x` debug模式
    

# mugen迁移需要更改代码的部分

由于大多数软件包的命令其实比较类似，而这个框架已经将centos系的操作（例如包的安装和包的卸载）抽象到了框架的公共函数文件中（`${OET_PATH}/libs/locallibs/common_lib.sh`），由`common_lib.sh`来选择shell执行（`common_lib_shell.sh`）和python执行的模式（`common_lib_python.sh`）

### 1.新增debian系包操作至框架公共函数

### 2.新增依赖安装函数
```shell
ubuntukylin_dep(){
    apt install expect psmisc make python3-six python3-pip python3-paramiko lshw -y
    pip3 install paramiko -i https://pypi.tuna.tsinghua.edu.cn/simple
}
```
### 3.对测试例的批量修改
```shell
# eg.1

# DNF_INSTALL wireshark
APT_INSTALL wireshark
# version=$(rpm -qa wireshark | awk -F "-" '{print$2}')
version=$(apt-cache show wireshark | awk -F "-" '{print$2}')
```

```shell
# eg.2

# DNF_REMOVE
APT_REMOVE
```
### 4.对后续个别测试例单独修改
## 部分样例修改后的测试截图(ubuntukylin本地运行mugen)

![](../Picture/ubuntukylin%E6%9C%AC%E5%9C%B0%E8%BF%90%E8%A1%8Cmugen.png)


# 初步结论
mugen由于以Shell为主体，在安装好相关依赖后便可以跨架构跨发行版运行，迁移起来相对比较容易，目前发现可能比较麻烦的点大概是后续在迁移测试中发现的不同包管理器所安装软件流程和功能的差异，这个将会导致测试例修改的幅度变大，此外的大部分测试例的修改幅度预估不大。个人认为这个迁移是成本不是那么高而且将会增加mugen测试框架的通用性，为后续相关系统的测试提供便利。