cmake_minimum_required(VERSION 3.8)
project(chat_demo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# 编译 talker
add_executable(talker src/talker.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

# 编译 listener
add_executable(listener src/listener.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

# 安装可执行文件
install(TARGETS
  talker
  listener
  DESTINATION lib/${PROJECT_NAME}
)

# 安装 launch 文件夹
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)


ament_package()
