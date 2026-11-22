cmake_minimum_required(VERSION 3.8)
project(week2_demo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# ---- 节点 ----
add_executable(week2_node src/week2_node.cpp)
ament_target_dependencies(week2_node rclcpp geometry_msgs sensor_msgs)

install(TARGETS week2_node
  DESTINATION lib/${PROJECT_NAME}
)

# ---- launch ----
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
