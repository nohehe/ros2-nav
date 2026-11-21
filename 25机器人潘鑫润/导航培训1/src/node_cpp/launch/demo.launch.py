import launch
import launch_ros

def generate_launch_description():
    talker_node=launch_ros.actions.Node(
        package='node_cpp',
        executable='talker_node',
        output='screen',
    )

    listener_node=launch_ros.actions.Node(
            package='node_cpp',
            executable='listener_node',
            output='screen',
        )

    return launch.LaunchDescription([
        talker_node,
        listener_node
    ])