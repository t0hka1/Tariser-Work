# Week3 (2023.2.4-2023.2.10)
## 本周工作
- mugen测试例初步的批量适配
    - 通过正则将所有测试例入口处和出口处的安装和卸载命令进行替换
        ```python
        new_content = re.sub(r"DNF_INSTALL (.*?)\n","uname -r | grep 'oe\|an' \n\
        if [ $? -eq 0 ]; then  \n\
            DNF_INSTALL \\1 \n\
        else \n\
            APT_INSTALL \\1 \n\
        fi\n",content)

        new_content = re.sub(r"version=\$\(rpm -qa (.*?) \| awk -F \"-\" '\{print\$2\}'\)\n","\n",new_content)

        new_content = re.sub(r"DNF_REMOVE","APT_REMOVE",new_content)

        ```
- mugen basic套移植
    - 共138条测试例
    - 测试相关log
        - https://github.com/t0hka1/mugen-ubuntu-riscv-logs
    - 初次测试success 42条
        ```
        oe_test_IOaccess_100Mfile
        oe_test_IOaccess_500Mfile
        oe_test_ProcMgmt_crontab_cronfile
        oe_test_ProcMgmt_pgrep
        oe_test_ProcMgmt_ps
        oe_test_ProcMgmt_start_kill
        oe_test_ProcMgmt_top
        oe_test_ProcMgmt_vmstat
        oe_test_USBinfo
        oe_test_basic_UserMgmt_permission
        oe_test_basic_create_group
        oe_test_basic_modify_group
        oe_test_basic_modify_user
        oe_test_basic_set_account_expiration_time
        oe_test_basic_set_permissions
        oe_test_cmd_id
        oe_test_cmd_su_user
        oe_test_cmd_sudo
        oe_test_date
        oe_test_group_access
        oe_test_groupdel
        oe_test_hwclock
        oe_test_kernel_cgroup
        oe_test_kernel_module_operation
        oe_test_kernel_sysctl
        oe_test_lz4
        oe_test_man
        oe_test_net_cmd_ping
        oe_test_net_conninfo_lsof
        oe_test_nmcli_cancel_auto
        oe_test_nmcli_general
        oe_test_nmcli_set_bond
        oe_test_system_log_basic
        oe_test_system_log_device
        oe_test_system_log_dmesg
        oe_test_system_log_process
        oe_test_system_log_security
        oe_test_system_monitor_process
        oe_test_tail
        oe_test_uname
        oe_test_who
        oe_test_whoami
        ```
    - 初次测试failed 96条
        ```
        oe_test_IOaccess_1Gfile
        oe_test_ProcMgmt_at
        oe_test_ProcMgmt_crontab_cmd
        oe_test_ProcMgmt_iostat
        oe_test_ProcMgmt_sar
        oe_test_awk *
        # oe_test_brotli
        # oe_test_chrony_Manuall
        # oe_test_chrony_chronyc_cmd
        # oe_test_chrony_chronyc_hardwaretime
        oe_test_chrony_chronyc_ntpstat *
        # oe_test_chrony_chronyd
        oe_test_disk_graphics_card *
        oe_test_disk_schedule_specific
        oe_test_disk_schedule_udev
        oe_test_disk_tuned_disable
        oe_test_disk_tuned_install
        oe_test_disk_tuned_modify
        oe_test_disk_tuned_new
        oe_test_disk_tuned_set
        oe_test_ethtool
        oe_test_exiv2
        oe_test_home_directory
        oe_test_hostnamectl
        oe_test_json-c
        oe_test_kernel_kdump
        oe_test_localectl
        oe_test_net_IPVLAN
        oe_test_net_VRF
        oe_test_net_cmd_ifconfig
        oe_test_net_cmd_scp
        oe_test_net_cmd_telnet
        oe_test_net_config
        oe_test_net_ncat
        oe_test_net_scriprts
        oe_test_net_setmode
        oe_test_nmcli_8023link
        oe_test_nmcli_Mgntconnect
        oe_test_nmcli_add_connect
        oe_test_nmcli_bridge
        oe_test_nmcli_con_reload
        oe_test_nmcli_config_connect
        oe_test_nmcli_config_gw
        oe_test_nmcli_device
        oe_test_nmcli_macsec
        oe_test_nmcli_route
        oe_test_nmcli_set_team
        oe_test_nmcli_systemd_resolved
        oe_test_nmcli_vlan
        # oe_test_password_blank
        # oe_test_password_change
        # oe_test_patch
        oe_test_performance_monitor_pcp
        oe_test_power_measurement_internal
        oe_test_power_output_HTML
        oe_test_power_powertop2tuned_optimize
        oe_test_power_powertop_calibrate
        oe_test_power_powertop_powerup
        oe_test_power_powertop_startup
        oe_test_reboot
        # oe_test_server_git_install
        # oe_test_server_httpd_checkfirewall
        oe_test_server_httpd_port   x
        oe_test_server_httpd_recover x
        oe_test_server_httpd_restart x
        oe_test_server_httpd_tls x
        oe_test_server_mariadb_backup
        oe_test_server_mariadb_backupDB
        oe_test_server_mariadb_backuptable
        oe_test_server_mariadb_compatibilty
        oe_test_server_mariadb_copy
        oe_test_server_mariadb_dump
        oe_test_server_mariadb_dumpMulDB
        oe_test_server_mariadb_loadfile
        oe_test_server_mariadb_onlinebackup
        oe_test_server_mariadb_stop
        oe_test_server_mysql
        oe_test_server_openssh_key
        oe_test_server_openssh_restart
        oe_test_server_openssh_secure
        oe_test_server_openssh_verifykey
        oe_test_server_postgresql
        oe_test_server_squid_blacklist
        oe_test_server_squid_ip
        oe_test_server_squid_proxy
        oe_test_server_vsftpd_NKdelay
        oe_test_server_vsftpd_restart
        oe_test_server_vsftpd_transfer
        oe_test_system_log_recorded
        oe_test_system_log_view
        oe_test_system_monitor_login
        oe_test_system_monitor_reboot
        oe_test_system_monitor_share_total
        oe_test_timedatectl
        oe_test_versioninfo
        oe_test_zlib
        ```
    - 单个样例修改后的二次测试（新增10条成功样例）
        ```
        oe_test_password_blank
        oe_test_password_change
        oe_test_brotli
        oe_test_chrony_chronyc_cmd
        oe_test_chrony_chronyc_hardwaretime
        oe_test_chrony_chronyd
        oe_test_chrony_Manuall
        oe_test_patch
        oe_test_server_git_install
        oe_test_server_httpd_checkfirewall
        ```