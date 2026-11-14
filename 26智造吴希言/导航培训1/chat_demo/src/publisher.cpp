// 发布者节点：每隔 1 秒往 chatter 话题发一条问候语
//
// 消息类型是 std_msgs/msg/String，话题名 chatter，
// 订阅者（subscriber.cpp）监听同一个话题。

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ChatterPublisher : public rclcpp::Node {
public:
    ChatterPublisher() : Node("chatter_publisher"), index_(0) {
        // 话题队列长度 10：发得太快时最多缓存 10 条
        publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

        // 每 1 秒触发一次 onTimer
        timer_ = this->create_wall_timer(1s, std::bind(&ChatterPublisher::onTimer, this));

        // 轮流发送这几句话
        messages_ = {"你好", "今天吃了吗", "在吗", "训练累不累"};

        RCLCPP_INFO(this->get_logger(), "发布者已启动，话题: chatter，每秒发一条");
    }

private:
    void onTimer() {
        auto msg = std_msgs::msg::String();
        msg.data = messages_[index_ % messages_.size()];
        ++index_;

        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "发送: %s", msg.data.c_str());
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::vector<std::string> messages_;
    size_t index_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ChatterPublisher>());
    rclcpp::shutdown();
    return 0;
}
