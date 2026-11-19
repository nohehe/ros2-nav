#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("talker_node");

  auto pub = node->create_publisher<std_msgs::msg::String>("/chatter", 10);

  auto timer = node->create_wall_timer(
    500ms,
    [pub, node]()
    {
      std_msgs::msg::String msg;
      msg.data = "Hello ROS2";
      pub->publish(msg);
      RCLCPP_INFO(node->get_logger(), "发布: %s", msg.data.c_str());
    }
  );

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
