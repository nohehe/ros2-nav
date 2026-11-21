#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>
#include <memory>

using namespace std::chrono_literals;

class listener:public rclcpp::Node
{
    public:
        listener():Node("listener")
        {
           subscription_=this->create_subscription<std_msgs::msg::String>("hello_topic",10,std::bind(&listener::callback,this,std::placeholders::_1));
           publisher_=this->create_publisher<std_msgs::msg::String>("reply_topic",10);
        }

    private:
        void callback(const std_msgs::msg::String::SharedPtr msg)
        {
           RCLCPP_INFO(this->get_logger(), "收到: %s", msg->data.c_str());

            std_msgs::msg::String reply_msg;
            if (msg->data == "你好")
            {
                reply_msg.data = "你好呀";
            }
            else
            {
                reply_msg.data = "收到消息啦";
            }

            RCLCPP_INFO(this->get_logger(), "回复: %s", reply_msg.data.c_str());
            publisher_->publish(reply_msg);
        }
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        
};

int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<listener>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}