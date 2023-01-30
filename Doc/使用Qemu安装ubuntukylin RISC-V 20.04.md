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

#### qemu启动
参考openEuler-riscv的qemu启动文件进行可修改为如下

```shell
#!/bin/bash

# start.sh

# The script is created for starting a riscv64 qemu virtual machine with specific parameters

## Configuration
vcpu=8 # CPU 核心数。
memory=8 # 运行存储大小，单位 GiB。
memory_append=`expr $memory \* 1024`
drive="ubuntukylin-20.04-pro-sp2-riscv64-qemu.img" # 根文件系统路径
kernel="/usr/lib/u-boot/qemu-riscv64_smode/uboot.elf" # 内核文件路径
bios="/usr/lib/riscv64-linux-gnu/opensbi/generic/fw_jump.elf" # BIOS 文件路径，设置为 none 表示 kernel 已包含相关文件。
ssh_port=12055 # 控制端口映射：将虚拟机内部的端口 22 映射到外部的 ssh_port。设置为空表示不启用该功能。
vnc_port=12056 # 控制 VNC 端口。设置为空表示不启用该功能。
spice_port=12057 # 控制 spice 端口。设置为空表示不启用该功能。
remoteip=localhost # 控制随后显示脚本的本机地址。
[[ $spice_port ]] && audiobackend="spice" || audiobackend="none" # 当 Spice 启用时，设置音频后端为 spice，否则为 none。可按需改动。
sleeptime=0 # 启动前等待时间

cmd="qemu-system-riscv64 \
  -nographic -machine virt \
  -smp "$vcpu" -m "$memory"G \
  -audiodev "$audiobackend",id=snd0 \
  -kernel "$kernel" \
  -bios "$bios" \
  -drive file="$drive",format=raw,id=virtio \
  -object rng-random,filename=/dev/urandom,id=rng0 \
  -device virtio-vga \
  -device virtio-rng-device,rng=rng0 \
  -device virtio-blk-device,drive=virtio \
  -device virtio-net-device,netdev=usernet \
  -device qemu-xhci -usb -device usb-kbd -device usb-tablet -device usb-audio,audiodev=snd0 \
  -append 'root=/dev/vda1 rw console=ttyS0 swiotlb=1 loglevel=3 systemd.default_timeout_start_sec=600 selinux=0 highres=off mem="$memory_append"M earlycon' "

echo -e "\033[37mUsing the following configuration: \033[0m"
echo ""
echo -e "\033[37mvCPU Cores:      \033[0m \033[34m"$vcpu"\033[0m"
echo -e "\033[37mMemory:          \033[0m \033[34m"$memory"\033[0m"
echo -e "\033[37mDisk Path:       \033[0m \033[34m"$drive"\033[0m"
echo -e "\033[37mKernal Path:     \033[0m \033[34m"$kernel"\033[0m"
echo -e "\033[37mBIOS Path:       \033[0m \033[34m"$bios"\033[0m"

if [ $ssh_port ]
then
echo -e "\033[37mSSH Port:        \033[0m \033[34m"$ssh_port"\033[0m"
cmd="${cmd} -netdev user,id=usernet,hostfwd=tcp::"$ssh_port"-:22 "
else
cmd="${cmd} -netdev user,id=usernet "
fi

if [ $vnc_port ]
then
echo -e "\033[37mVNC Port:        \033[0m \033[34m"$vnc_port"\033[0m"
cmd="${cmd} -vnc :"$((vnc_port-5900))
fi

if [ $spice_port ]
then
echo -e "\033[37mSPICE Port:      \033[0m \033[34m"$spice_port"\033[0m"
cmd="${cmd} -device virtio-serial-pci \
-device virtserialport,chardev=spicechannel0,name=com.redhat.spice.0 \
-chardev spicevmc,id=spicechannel0,name=vdagent \
-spice port=${spice_port},disable-ticketing=on"
fi
echo ""
if [ $ssh_port ]
then
echo -e "\033[37mUse the following command to connect SSH server:\0\033[0m\033[34m ssh root@"$remoteip" -p "$ssh_port"\033[0m"
fi
if [ $vnc_port ]
then
echo -e "\033[37mUse the following address to connect VNC server:\0\033[0m\033[34m "$remoteip":"$vnc_port"\033[0m"
fi
if [ $spice_port ]
then
echo -e "\033[37mUse the following address to connect SPICE server:\0\033[0m\033[34m spice://"$remoteip":"$spice_port"\033[0m"
fi
echo ""
echo -e "\033[37mUsing the following command to start VM: \033[0m"
echo ""
echo $cmd
echo ""
sleep $sleeptime
echo -e "\033[37mStarting VM... \033[0m"
eval $cmd
echo -e "Exit."
```


### 虚拟机扩容
> 在参考了[KotorinMinami同学的openKylin启动教程后](https://github.com/KotorinMinami/plct-working/tree/main/openKylin)，发现ubuntuKylin的磁盘空间比较小，下也需要进行类似的qemu扩容操作，遂对文档进行该部分的补充

操作如下：

1.宿主机安装`qemu-img`，以Debian系为例，`apt install qemu-img -y`

2.UbuntuKylin Risc-V虚拟机上安装`growpart`，`apt install cloud-initramfs-growroot -y`

值得注意的是growpart在默认语言为中文的操作系统下执行会出现问题，所以进行如下操作(注销重新登陆即可生效)

加载英文模块
`locale-gen en_US.UTF-8 && update-locale` 

更改语言（建议LANG LANGUAGE LC_ALL全部更改）
```shell
update-locale LANG=en_US.UTF-8
update-locale LANGUAGE=en_US.UTF-8
update-locale LC_ALL=en_US.UTF-8
```

3.关闭虚拟机，在宿主机上进行扩容操作，此处扩容20G
`qemu-img resize ubuntukylin-20.04-pro-sp2-riscv64-qemu.img +20G`

4.打开虚拟机，执行`lsblk`查看分区情况
```shell
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
vda    252:0    0   29G  0 disk 
├─vda1 252:1    0    1M  0 part 
├─vda2 252:2    0    4M  0 part 
├─vda3 252:3    0  500M  0 part /boot
└─vda4 252:4    0  8.5G  0 part /

```
此处执行`growpart /dev/vda 4`进行扩展

再次执行`lsblk`可以看到已经成功扩展

之后扩展文件系统，执行 `resize2fs /dev/vda4`

执行 `df -h`验证 / 分区的大小

### 远程连接
可选用spice/ssh连接
#### spice连接
```shell
sudo apt install virt-viewer            #安装virt-viewer
remote-viewer spice://localhost:12057   #使用spice连接虚拟机
```
#### ssh连接
在虚拟机中`vi /etc/ssh/sshd_config`

修改sshd_config ，找到#PermitRootLogin一行将后面那个改为yes并取消注释,即PermitRootLogin yes 然后重启sshd

`systemctl restart sshd`

在宿主机中直接执行`ssh -p 12055 ubuntu@127.0.0.1`即可连接