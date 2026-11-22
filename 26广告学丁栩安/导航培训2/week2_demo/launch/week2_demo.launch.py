# ============================================================================
#  week2_demo.launch.py —— 第二周作业启动文件
#
#  一条命令同时启动：
#    1) auto_move_node    自动连续动作（发布 /cmd_vel_raw）
#    2) safety_stop_node  激光避障停车（订阅 /scan + /cmd_vel_raw，输出 /cmd_vel）
#
#  用法：
#    ros2 launch week2_demo week2_demo.launch.py                 # 默认：循环动作
#    ros2 launch week2_demo week2_demo.launch.py mode:=straight  # 一直前进，用于避障演示
# ============================================================================

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # mode: sequence（前进/左转/前进/右转 循环） 或 straight（一直前进）
    mode = LaunchConfiguration('mode')
    # stop_distance: 前方多少米以内有障碍就停车
    stop_distance = LaunchConfiguration('stop_distance')

    auto_move = Node(
        package='week2_demo',
        executable='auto_move_node',
        name='auto_move_node',
        output='screen',
        parameters=[{
            'mode': mode,
            'linear_speed': 0.3,
            'angular_speed': 0.6,
            'forward_time': 4.0,
            'turn_time': 2.0,
        }],
    )

    safety_stop = Node(
        package='week2_demo',
        executable='safety_stop_node',
        name='safety_stop_node',
        output='screen',
        parameters=[{
            'stop_distance': stop_distance,
            'front_fov_deg': 30.0,
        }],
    )

    return LaunchDescription([
        DeclareLaunchArgument('mode', default_value='sequence',
                              description='sequence=循环动作, straight=一直前进'),
        DeclareLaunchArgument('stop_distance', default_value='0.5',
                              description='避障停车距离（米）'),
        auto_move,
        safety_stop,
    ])
