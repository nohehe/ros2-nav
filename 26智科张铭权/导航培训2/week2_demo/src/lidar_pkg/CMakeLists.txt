cmake_minimum_required(VERSION 3.8)
project(lidar_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# 读取激光数据：打印正前方那一束的距离
add_executable(lidar_data src/lidar_data.cpp)
ament_target_dependencies(lidar_data "rclcpp" "sensor_msgs")

# 遇障停车：前方 1.5 m 内有障碍就转向，否则直行
add_executable(lidar_behavior src/lidar_behavior.cpp)
ament_target_dependencies(lidar_behavior "rclcpp" "sensor_msgs" "geometry_msgs")

install(TARGETS lidar_data lidar_behavior
  DESTINATION lib/${PROJECT_NAME})

ament_package()
