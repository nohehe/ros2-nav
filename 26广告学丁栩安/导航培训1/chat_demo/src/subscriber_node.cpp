// ============================================================================
//  subscriber_node.cpp  ——  订阅者节点
//  作用：监听话题 /chat，每收到一条消息，就在终端打印「原文」和「对应回复」。
//  关键概念：Subscription（订阅者）、Callback（回调函数）、Message（消息）
// ============================================================================

#include <functional>     // std::bind / std::placeholders
#include <memory>         // std::make_shared
#include <string>         // std::string
#include <unordered_map>  // 哈希表：原文 -> 回复 的对照表

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// ---------------------------------------------------------------------------
// 自定义节点类：订阅者
// ---------------------------------------------------------------------------
class ChatSubscriber : public rclcpp::Node
{
public:
  ChatSubscriber()
  : Node("chat_subscriber")   // 节点名
  {
    // ---------- 第 1 步：准备「原文 -> 回复」对照表 ----------
    // 这就是作业里"消息和回复自己定"的部分，你想加多少条都行
    replies_ = {
      {"你好", "你好呀"},
      {"你是谁", "我是订阅者节点，专管自动回复"},
      {"今天学什么", "今天学 ROS2 的话题通信"},
      {"这个作业难吗", "不难，跟着敲一遍就会了"},
      {"再见", "再见，明天见！"},
    };

    // ---------- 第 2 步：创建订阅者 ----------
    // create_subscription<消息类型>(
    //     "话题名",              必须和发布者一致，否则收不到
    //     队列长度,              10：最多缓存 10 条待处理消息
    //     回调函数               每收到一条消息就调用它
    // )
    // std::placeholders::_1 是占位符，表示"回调的第 1 个参数"（也就是那条消息）
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "/chat", 10,
      std::bind(&ChatSubscriber::on_message, this, std::placeholders::_1));

    RCLCPP_INFO(
      this->get_logger(), "chat_subscriber 已启动，正在等待话题 /chat 上的消息...");
  }

private:
  // ---------- 第 3 步：回调函数 ----------
  // 参数是消息的共享指针，用 -> 访问字段。msg->data 就是发布者填进去的字符串
  void on_message(const std_msgs::msg::String::SharedPtr msg)
  {
    const std::string question = msg->data;

    // 在对照表里查这句话的回复
    auto it = replies_.find(question);
    const std::string reply = (it != replies_.end())
      ? it->second                              // 找到了：取对应的回复
      : "这句话我还没学会，先记下来啦";          // 没找到：给个兜底回复

    // 打印「原文」和「回复」——这就是作业要求的终端效果
    RCLCPP_INFO(
      this->get_logger(),
      "收到: \"%s\"  --->  回复: \"%s\"",
      question.c_str(), reply.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;  // 订阅者句柄
  std::unordered_map<std::string, std::string> replies_;                 // 对照表
};

// ---------------------------------------------------------------------------
// main：入口，和发布者一样的三段式
// ---------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);                                  // 初始化
  rclcpp::spin(std::make_shared<ChatSubscriber>());          // 自旋，等待消息
  rclcpp::shutdown();                                        // 退出清理
  return 0;
}
