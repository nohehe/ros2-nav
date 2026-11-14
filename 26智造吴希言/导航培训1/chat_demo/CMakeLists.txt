cmake_minimum_required(VERSION 3.15)
project(chat_demo)

# 用 C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# ---- 发布者 ----
add_executable(publisher src/publisher.cpp)
# ROS2 Lyrical 起，依赖直接用标准 CMake target 链接；
# 老的 ament_target_dependencies() 已经拆成独立包，不默认导出。
target_link_libraries(publisher
  rclcpp::rclcpp
  std_msgs::std_msgs
)

# ---- 订阅者 ----
add_executable(subscriber src/subscriber.cpp)
target_link_libraries(subscriber
  rclcpp::rclcpp
  std_msgs::std_msgs
)

# 装可执行文件：不写这行，ros2 run / ros2 launch 会找不到
install(TARGETS
  publisher
  subscriber
  DESTINATION lib/${PROJECT_NAME}
)

# 装 launch 文件：不写这行，ros2 launch 会找不到
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
