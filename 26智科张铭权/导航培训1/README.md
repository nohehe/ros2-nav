# 导航培训1 - 26智科张铭权

## 一、作业说明

ROS2 工作空间里的 `ament_cmake` C++ 功能包 `msg_pub_cub`，包含一个发布者和一个订阅者：

- `talker`：每 0.5 秒往话题 `/chatter` 发布一条消息，内容为 `Hello ROS2`。
- `listener`：订阅 `/chatter`，收到后打印 `收到消息：Hello ROS2`。
- `start_pub_sub.launch.py`：一个 launch 文件同时启动两个节点。

目录结构：

```text
导航培训1/
├── msg_pub_cub/
│   ├── package.xml
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── publish_node.cpp      # 发布者 talker
│   │   └── subscriber_node.cpp   # 订阅者 listener
│   └── launch/
│       └── start_pub_sub.launch.py
├── 运行截图.png
└── README.md
```

## 二、运行环境

- Ubuntu 22.04
- ROS2 Humble
- colcon 构建工具

## 三、编译命令

把 `msg_pub_cub` 功能包放进工作空间的 `src` 目录，然后在工作空间根目录编译：

```bash
mkdir -p ~/ros2_ws/src
cp -r msg_pub_cub ~/ros2_ws/src/
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select msg_pub_cub
```

## 四、启动命令

每打开一个新终端，都要先加载环境变量，否则会报 `Package 'msg_pub_cub' not found`。

```bash
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch msg_pub_cub start_pub_sub.launch.py
```

停止运行按 `Ctrl+C`，launch 会把两个节点一起关闭。

## 五、预期输出

两个节点的输出在同一个终端里交替打印，每 0.5 秒一组：

```text
[talker_node-1] [INFO] [talker_node]: 发布: Hello ROS2
[listener_node-2] [INFO] [listener_node]: 收到消息：Hello ROS2
```

运行截图见本目录下的 `运行截图.png`。

## 六、验证命令

另开一个终端（同样先执行两条 source），可以检查话题是否正常收发：

```bash
ros2 topic list                      # 应该能看到 /chatter
ros2 topic echo /chatter             # 持续输出 data: Hello ROS2
ros2 topic hz /chatter               # 约 2 Hz，说明每 0.5 秒发布一条
```
