cmake_minimum_required(VERSION 3.8)
project(week2_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(auto_move_node src/auto_move_node.cpp)
ament_target_dependencies(auto_move_node rclcpp geometry_msgs)

add_executable(safety_stop_node src/safety_stop_node.cpp)
ament_target_dependencies(safety_stop_node rclcpp geometry_msgs sensor_msgs)

install(TARGETS
  auto_move_node
  safety_stop_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
