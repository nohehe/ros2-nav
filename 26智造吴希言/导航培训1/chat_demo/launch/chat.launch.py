from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    """同时启动发布者和订阅者两个节点。"""
    return LaunchDescription([
        Node(
            package='chat_demo',
            executable='publisher',
            name='chatter_publisher',
            output='screen',
        ),
        Node(
            package='chat_demo',
            executable='subscriber',
            name='chatter_subscriber',
            output='screen',
        ),
    ])
