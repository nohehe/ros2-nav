# 导航培训2 —— Gazebo 仿真与激光避障

## 一、作业要求

- 成功编译工作空间
- 成功启动 Gazebo 仿真
- 使用 `/cmd_vel` 控制机器人运动
- 查看 `/scan` 激光数据
- 完成自动连续动作
- 完成避障停车

截图见 `运行截图/` 目录（仿真终端、启动日志、Gazebo画面、激光数据、避障停车）。

## 二、作业代码：week2_demo 功能包

采用「运动指令 + 安全过滤」两层设计，这也是真实机器人上常见的写法：

```
auto_move_node  ──/cmd_vel_raw──▶  safety_stop_node  ──/cmd_vel──▶  Gazebo 底盘
   （自动连续动作）                  （订阅 /scan 做安全检查）         （base_controller）
```

- `auto_move_node`：按时间顺序循环发送「前进 4s → 左转 2s → 前进 4s → 右转 2s」，
  发布到 `/cmd_vel_raw`（"期望速度"）。
  参数 `mode:=straight` 时改为一直前进，专门用来演示避障停车。
- `safety_stop_node`：订阅 `/scan`，取正前方 ±30° 扇区内的最近距离；
  - 距离 ≥ `stop_distance`（默认 0.5 m）：原样转发速度到 `/cmd_vel`
  - 距离 <  `stop_distance`：强制发布全零速度，机器人立即停车，并打印警告

这样"避障"是真正拦得住自动动作的，而不是两个节点各自抢占 `/cmd_vel`。

## 三、编译

```bash
cp -r week2_demo ~/222_ws/src/
cd ~/222_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select week2_demo
source install/setup.bash
```

## 四、运行

### 1. 启动 Gazebo 仿真

```bash
ros2 launch wpr_simulation2 wpb_simple.launch.py
```

> 本项目依赖 `wpr_simulation2` 仿真包：
> `git clone https://gitee.com/s-robot/wpr_simulation2.git`
> 并执行其 `scripts/install_for_humble.sh` 安装依赖。

### 2. 自动连续动作（循环）

```bash
ros2 launch week2_demo week2_demo.launch.py
```

### 3. 避障停车演示（一直前进直到检测到障碍）

```bash
ros2 launch week2_demo week2_demo.launch.py mode:=straight
```

终端会打印距离变化，并在 0.5 m 处自动停车：

```
[safety_stop_node]: 前方最近障碍距离: 1.34 m —— 允许通行
[safety_stop_node]: 前方最近障碍距离: 0.64 m —— 允许通行
[safety_stop_node]: 0.50 m 检测到障碍物 —— 避障停车！
[safety_stop_node]: 前方 0.46 m 有障碍物，已停车
```

### 4. 查看激光数据

```bash
ros2 topic type /scan      # sensor_msgs/msg/LaserScan
ros2 topic hz /scan        # 约 10 Hz
ros2 topic echo /scan --once
```

## 五、可调参数

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `mode` | `sequence` | `sequence` 循环动作 / `straight` 一直前进 |
| `linear_speed` | 0.3 | 前进速度 m/s |
| `angular_speed` | 0.6 | 转向角速度 rad/s |
| `stop_distance` | 0.5 | 停车距离 m |
| `front_fov_deg` | 30.0 | 前向检测扇区半角（度） |

例如把停车距离调到 1.0 m：

```bash
ros2 launch week2_demo week2_demo.launch.py mode:=straight stop_distance:=1.0
```

## 六、环境说明

本机为 VMware 虚拟机，Gazebo 图形界面（`gzclient`）由 launch 拉起时会因虚拟机
OpenGL 兼容问题退出，但 `gzserver`、机器人、所有话题均正常。解决办法是仿真照常启动后，
另开一个终端执行 `gzclient`。在真机或 Ubuntu 双系统上无此问题。
