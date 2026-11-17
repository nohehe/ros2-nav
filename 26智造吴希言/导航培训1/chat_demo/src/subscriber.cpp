// 订阅者节点：收到 chatter 上的消息后，在终端打印「原文 -> 回复」
//
// 回复规则用一张表查，表里没有的话给一句兜底回复。

#include <map>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class ChatterSubscriber : public rclcpp::Node {
public:
    ChatterSubscriber() : Node("chatter_subscriber") {
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "chatter", 10,
            std::bind(&ChatterSubscriber::onMessage, this, std::placeholders::_1));

        // 原文 -> 回复 的对照表
        replies_ = {
            {"你好", "你好呀"},
            {"今天吃了吗", "吃了，你呢"},
            {"在吗", "在的"},
            {"训练累不累", "还行，能顶住"},
        };

        RCLCPP_INFO(this->get_logger(), "订阅者已启动，等待 chatter 上的消息...");
    }

private:
    void onMessage(const std_msgs::msg::String::SharedPtr msg) {
        const std::string& text = msg->data;

        auto it = replies_.find(text);
        const std::string reply =
            (it != replies_.end()) ? it->second : "这句我还不知道怎么回";

        // 一行里同时给出原文和回复
        RCLCPP_INFO(this->get_logger(), "收到: %s  ->  回复: %s",
                    text.c_str(), reply.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    std::map<std::string, std::string> replies_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ChatterSubscriber>());
    rclcpp::shutdown();
    return 0;
}
