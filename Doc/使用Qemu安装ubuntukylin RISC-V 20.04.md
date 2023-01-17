# 使用Qemu安装ubuntukylin RISC-V 20.04 

## 注意事项

- 默认登录用户`ubuntukylin`的用户密码是`ubuntukylin`
- `root`用户密码可在登录后通过`sudo passwd`自行设置

## 使用Qemu安装ubuntukylin RISC-V 20.04 

### 系统镜像下载

#### 源

ubuntukylin RISC-V 20.04 的镜像可以在[华为云镜像站](https://mirrors.huaweicloud.com/ubuntukylin-cdimage/20.04/)下载
```shell
wget https://mirrors.huaweicloud.com/ubuntukylin-cdimage/20.04/ubuntukylin-20.04-pro-sp2-riscv64-qemu.img.xz
```
### 部署和启动

> 已验证启动脚本在Ubuntu 22.04环境下正常运行

#### 解压缩镜像文件

```shell
sudo apt install xz-utils -y
xz -dk ubuntukylin-20.04-pro-sp2-riscv64-qemu.img.xz
```

#### 在启动qemu前，请先安装如下包

```shell
sudo apt install qemu-system-misc opensbi u-boot-qemu qemu-utils -y
```

#### qemu启动参数

```shell
qemu-system-riscv64 \
-machine virt -nographic -m 2048 -smp 4 \
-bios /usr/lib/riscv64-linux-gnu/opensbi/generic/fw_jump.elf \
-kernel /usr/lib/u-boot/qemu-riscv64_smode/uboot.elf \
-device virtio-net-device,netdev=eth0 -netdev user,id=eth0 \
-drive file=ubuntukylin-20.04-pro-sp2-riscv64-qemu.img,format=raw,if=virtio
```
[可选]启动参数调整
- `-m`为虚拟机内存数目，可随需要调整
- `-smp`为cpu核心数，可随需要调整
- `-bios`的参数是QEMU启动第一阶段所需的固件。QEMU 7.0及以上版本可不需要此选项
