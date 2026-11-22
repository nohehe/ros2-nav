#!/usr/bin/env python3
"""导航培训2：一键启动 Gazebo 仿真 + 自动动作节点。"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():
    # 1. 先起 Gazebo 仿真（带激光雷达的机器人）
    wpr_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('wpr_simulation2'),
                'launch',
                'wpb_simple.launch.py'
            )
        )
    )

    # 2. 再起我们的自动动作节点
    week2_node = Node(
        package='week2_demo',
        executable='week2_node',
        name='week2_node',
        output='screen',
    )

    return LaunchDescription([
        wpr_launch,
        week2_node,
    ])
