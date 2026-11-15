#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

using namespace std::chrono_literals; 

int main(int argc, char * argv[]) {
    
    rclcpp::init(argc, argv);

    
    auto node = rclcpp::Node::make_shared("talker_node");

    
    auto publisher = node->create_publisher<std_msgs::msg::String>("chat_topic", 10);

    
    auto timer = node->create_wall_timer(1000ms, [&]() {
        auto msg = std_msgs::msg::String();
        msg.data = "你好"; 
        
        
        RCLCPP_INFO(node->get_logger(), "发送: %s", msg.data.c_str());
        
        
        publisher->publish(msg); 
    });

    
    rclcpp::spin(node);

    
    rclcpp::shutdown();
    return 0;
}
