# 导航培训1 · ROS2 话题通信

一个 `ament_cmake` 功能包，里面两个 C++ 节点：

- `publisher` —— 每秒往 `chatter` 话题发一条问候语
- `subscriber` —— 收到消息后在终端打印「原文 → 回复」

用 `chat.launch.py` 一次把两个节点都拉起来。

## 环境

| 项 | 版本 |
|---|---|
| 系统 | Ubuntu 26.04.1 LTS（VMware 虚拟机） |
| ROS2 | Lyrical Luth（`/opt/ros/lyrical`） |
| 编译器 | g++ 15.2.0 |
| CMake | 4.2.3 |
| 构建工具 | colcon |

> Ubuntu 26.04 对应的是 **ROS2 Lyrical**，不是 Humble 或 Jazzy，装的时候注意版本别搞混。

## 目录

```
导航培训1/
├── chat_demo/
│   ├── package.xml
│   ├── CMakeLists.txt
│   ├── src/
│   │   ├── publisher.cpp
│   │   └── subscriber.cpp
│   └── launch/
│       └── chat.launch.py
├── 运行截图.png
└── README.md
```

## 编译

先把包放进工作空间的 `src/`：

```bash
mkdir -p ~/ros2_ws/src
cp -r chat_demo ~/ros2_ws/src/
cd ~/ros2_ws
colcon build --packages-select chat_demo
```

编译完必须 source 一下，不然 `ros2 run` 找不到包：

```bash
source install/setup.bash
```

## 启动

一条命令同时起两个节点：

```bash
ros2 launch chat_demo chat.launch.py
```

如果只想单独跑某一个：

```bash
ros2 run chat_demo publisher     # 只开发布者
ros2 run chat_demo subscriber    # 只开订阅者
```

## 运行效果

终端里会看到两个节点的输出交替出现：

```
[publisher-1]  [INFO] [chatter_publisher]: 发送: 你好
[subscriber-2] [INFO] [chatter_subscriber]: 收到: 你好  ->  回复: 你好呀
[publisher-1]  [INFO] [chatter_publisher]: 发送: 今天吃了吗
[subscriber-2] [INFO] [chatter_subscriber]: 收到: 今天吃了吗  ->  回复: 吃了，你呢
[publisher-1]  [INFO] [chatter_publisher]: 发送: 在吗
[subscriber-2] [INFO] [chatter_subscriber]: 收到: 在吗  ->  回复: 在的
[publisher-1]  [INFO] [chatter_publisher]: 发送: 训练累不累
[subscriber-2] [INFO] [chatter_subscriber]: 收到: 训练累不累  ->  回复: 还行，能顶住
```

完整效果见 `运行截图.png`。

## 代码要点

**话题和消息类型**

两个节点用的是同一个话题名 `chatter`，消息类型是 `std_msgs/msg/String`。名字对上才能通，大小写和命名空间都要一致。

**回复逻辑**

订阅者里维护了一张 `原文 → 回复` 的 `std::map`，收到消息查表；表里没有的给一句兜底回复：

```cpp
auto it = replies_.find(text);
const std::string reply =
    (it != replies_.end()) ? it->second : "这句我还不知道怎么回";
```

**CMake 依赖写法（踩过的坑）**

ROS2 Lyrical 把 `ament_target_dependencies()` 拆成了独立的包，不再默认导出。直接写会报：

```
Unknown CMake command "ament_target_dependencies"
```

改用标准 CMake target 链接：

```cmake
target_link_libraries(publisher
  rclcpp::rclcpp
  std_msgs::std_msgs
)
```

**两个必须写的 install**

少一个都会出问题：

```cmake
# 不写这个，ros2 run / ros2 launch 报找不到可执行文件
install(TARGETS publisher subscriber DESTINATION lib/${PROJECT_NAME})

# 不写这个，ros2 launch 报找不到 launch 文件
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
```

## 验证过的东西

- `colcon build --packages-select chat_demo` 通过，无警告
- `ros2 pkg executables chat_demo` 能列出 `publisher` 和 `subscriber`
- `ros2 launch chat_demo chat.launch.py` 两个节点都起来，消息正常收发
- 中文输出正常，没有乱码
