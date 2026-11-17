from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(package='chat_demo', executable='talker', output='screen'),
        Node(package='chat_demo', executable='listener', output='screen')
    ])
