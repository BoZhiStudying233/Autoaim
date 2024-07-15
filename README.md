# IRobot2024全国赛自瞄测试代码

自瞄仓库地址：https://github.com/IRobot-Algorithm/Autoaim

如有使用疑问联系--李曾阳

## 快速开始

配置好rmos_bringup/config内的launch_params.yaml文件后运行以下命令启动自瞄程序：
```bash
colcon build
source install/setup.bash
ros2 launch rmos_bringup normal_aim.launch.py
```

在./rmos_bringup/config/node_params中修改常用参数

## Bug Lists

如有任何问题，欢迎提交issue。如有bug，及时记录于算法组bug_list文档中。https://w0ybodqyg7f.feishu.cn/wiki/NGv6wFslWiqJfwkIPUjcgYw9n3e?from=from_copylink