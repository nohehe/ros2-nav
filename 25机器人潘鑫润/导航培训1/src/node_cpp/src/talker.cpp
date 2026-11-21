#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>
#include <memory>

using namespace std::chrono_literals;

class Talker:public rclcpp::Node
{
    public:
        Talker():Node("talker")
        {
            publisher_=this->create_publisher<std_msgs::msg::String>("hello_topic",10);
            timer_=this->create_wall_timer(1s,std::bind(&Talker::time_callback,this));
        }

    private:
        void time_callback()
        {
            auto msg=std_msgs::msg::String();
            msg.data="你好";
            publisher_->publish(msg);
            RCLCPP_INFO(this->get_logger(),"发送：%s",msg.data.c_str());
        }
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Talker>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}