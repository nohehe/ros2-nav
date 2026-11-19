from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="msg_pub_cub",
            executable="talker",
            name="talker_node",
            output="screen"
        ),
        Node(
            package="msg_pub_cub",
            executable="listener",
            name="listener_node",
            output="screen"
        )
    ])