# 导航培训1 —— ROS2 话题通信（chat_demo）

## 一、作业要求

在自己的工作空间里创建 ament_cmake 功能包，用 C++ 写一个发布者和一个订阅者。
发布者发消息，订阅者每收到一条就显示原文和对应回复；用一个 launch 文件同时启动两个节点。

## 二、功能说明

- 话题名：`/chat`，消息类型：`std_msgs/msg/String`
- `chat_publisher`：每 1 秒向 `/chat` 发布一句话（你好 / 你是谁 / 今天学什么 / 这个作业难吗 / 再见，循环发送）
- `chat_subscriber`：收到消息后查表得到回复，并打印 `收到: "原文" ---> 回复: "xxx"`
- `chat.launch.py`：一条命令同时启动上面两个节点

## 三、目录结构

```
chat_demo/
├── CMakeLists.txt
├── package.xml
├── src/
│   ├── publisher_node.cpp
│   └── subscriber_node.cpp
└── launch/
    └── chat.launch.py
```

## 四、编译命令

把 `chat_demo` 放到工作空间的 `src` 目录下，然后：

```bash
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
colcon build --packages-select chat_demo
```

编译成功会看到：

```
Summary: 1 package finished [9.77s]
```

## 五、启动命令

```bash
cd ~/ros2_ws
source /opt/ros/humble/setup.bash
source install/setup.bash

ros2 launch chat_demo chat.launch.py
```

按 `Ctrl+C` 退出。

## 六、预期输出

```
[INFO] [chat_publisher-1]: process started with pid [40768]
[INFO] [chat_subscriber-2]: process started with pid [40770]
[chat_publisher-1] [INFO] [...] [chat_publisher]: chat_publisher 已启动，每 1.0 秒向话题 /chat 发布一条消息
[chat_subscriber-2] [INFO] [...] [chat_subscriber]: chat_subscriber 已启动，正在等待话题 /chat 上的消息...
[chat_publisher-1] [INFO] [...] [chat_publisher]: 发布 -> "你好"
[chat_subscriber-2] [INFO] [...] [chat_subscriber]: 收到: "你好"  --->  回复: "你好呀"
[chat_publisher-1] [INFO] [...] [chat_publisher]: 发布 -> "你是谁"
[chat_subscriber-2] [INFO] [...] [chat_subscriber]: 收到: "你是谁"  --->  回复: "我是订阅者节点，专管自动回复"
...
```

运行截图见同目录下的 `运行截图.png`。

## 七、环境

| 项目 | 版本 |
| --- | --- |
| 操作系统 | Ubuntu 22.04.5 LTS |
| ROS2 | Humble Hawksbill |
| 构建类型 | ament_cmake（C++17） |

## 八、常用检查命令

```bash
ros2 node list                  # 应看到 /chat_publisher /chat_subscriber
ros2 topic list                 # 应看到 /chat
ros2 topic info /chat           # Type: std_msgs/msg/String
ros2 topic echo /chat           # 实时查看消息
ros2 topic hz /chat             # 发布频率约 1 Hz
```
