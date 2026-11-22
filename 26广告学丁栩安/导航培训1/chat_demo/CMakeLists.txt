# CMakeLists.txt 是 CMake 的"编译说明书"，colcon build 实际上就是在调用 CMake。
# 它要回答三个问题：1) 依赖哪些包  2) 编译哪些源文件成可执行程序  3) 装到哪去

# 要求 CMake 版本至少 3.8（ROS2 Humble 的最低要求）
cmake_minimum_required(VERSION 3.8)

# 声明项目名。必须和 package.xml 里的 <name> 完全一致
project(chat_demo)

# C++ 标准：Humble 官方用 C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# 打开编译警告，帮助发现自己写的低级错误（作业里保留是好习惯）
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ====== 找依赖 ======
# ament_cmake 是 ROS2 的构建系统本体
find_package(ament_cmake REQUIRED)
# rclcpp 提供 Node、create_publisher、create_subscription 等
find_package(rclcpp REQUIRED)
# std_msgs 提供 std_msgs::msg::String
find_package(std_msgs REQUIRED)

# ====== 目标 1：发布者可执行文件 ======
# add_executable(程序名 源文件)。程序名 chat_publisher 后面 launch 里要用到
add_executable(chat_publisher src/publisher_node.cpp)
# ament_target_dependencies 把上面的 find_package 结果"接"到这个程序上，
# 它自动帮你加 include 路径和链接库，比手写 target_link_libraries 省事
ament_target_dependencies(chat_publisher rclcpp std_msgs)

# ====== 目标 2：订阅者可执行文件 ======
add_executable(chat_subscriber src/subscriber_node.cpp)
ament_target_dependencies(chat_subscriber rclcpp std_msgs)

# ====== 安装规则（非常重要，漏了就 ros2 run / ros2 launch 找不到程序） ======
# 可执行文件装到 install/chat_demo/lib/chat_demo/ 下
install(TARGETS
  chat_publisher
  chat_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

# launch 文件夹装到 install/chat_demo/share/chat_demo/launch/
# ros2 launch 就是去 share/<包名>/launch 下面找 .launch.py 文件的
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

# 收尾：生成 ament 需要的环境脚本（setup.bash 等）
ament_package()
