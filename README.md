# IRobot自瞄代码测试版

如有使用疑问联系--李曾阳

## 快速开始

配好环境和大恒相机后

```bash
colcon build
source install/setup.bash
ros2 launch rmos_bringup normal_aim.launch.py
```

在./rmos_bringup/config中修改常用参数

## To do

- 测试整车自瞄运行状态
- 增加debug模式
- 编写详细自瞄教程
- 测试其他检测器
- rmos_ec(模拟下位机)测试暂有问题