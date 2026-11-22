# ============================================================================
#  chat.launch.py  ——  启动文件
#  作用：用一条命令同时启动 chat_publisher 和 chat_subscriber 两个节点。
#  为什么用 launch？因为作业要求"同时启动"，手开两个终端太麻烦，
#  而且 launch 还能统一改参数、统一收集日志。
# ============================================================================

# LaunchDescription：launch 文件的"总清单"，最后必须返回它
from launch import LaunchDescription

# Node：描述"要启动一个节点"这个动作
from launch_ros.actions import Node


def generate_launch_description():
    """launch 文件的入口函数，名字固定，必须返回一个 LaunchDescription。"""

    # ---------------- 第 1 个节点：发布者 ----------------
    talker = Node(
        package='chat_demo',            # 功能包名，必须和 package.xml 的 <name> 一致
        executable='chat_publisher',    # CMakeLists.txt 里 add_executable 定义的程序名
        name='chat_publisher',          # 运行时的节点名（可省略，省略就用程序里的名字）
        output='screen',                # 把节点的日志直接打印到当前终端（不加这个看不到 RCLCPP_INFO）
        parameters=[{'interval': 1.0}], # 覆盖节点里的参数：每 1 秒发一条
    )

    # ---------------- 第 2 个节点：订阅者 ----------------
    listener = Node(
        package='chat_demo',
        executable='chat_subscriber',
        name='chat_subscriber',
        output='screen',
    )

    # 把两个节点一起交给 launch 系统，它会并发启动它们
    return LaunchDescription([talker, listener])
