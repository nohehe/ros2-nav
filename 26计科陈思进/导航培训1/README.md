先进行环境配置
Ubuntu22.04 + ROS2（Humble）
通过colcon build 构建工具

然后进行编译：
①在根目录下
②编译功能包：colcon build
③加载环境变量：source install/setup.sh
④使用 Launch 文件同时启动发布者和订阅者：ros2 launch chat_demo chat.launch.py

最后终端出现预期结果：
[talker_node] 发送：你好

[listener_node] 收到原文：你好

[listener_node] 对应回复：你好呀

