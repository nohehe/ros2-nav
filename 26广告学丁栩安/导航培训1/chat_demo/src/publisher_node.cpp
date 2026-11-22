// ============================================================================
//  publisher_node.cpp  ——  发布者节点
//  作用：每隔一段时间，把一句话（消息）发到话题 /chat 上，别的节点就能收到。
//  关键概念：Node（节点）、Publisher（发布者）、Message（消息）、Topic（话题）、Timer（定时器）
// ============================================================================

#include <chrono>      // std::chrono，用来表示时间间隔
#include <functional>  // std::bind，用来把成员函数包成回调
#include <memory>      // std::make_shared，智能指针
#include <string>      // std::string
#include <vector>      // std::vector，存句子列表

// ROS2 的 C++ 总头文件，Node、spin、init 等都在里面
#include "rclcpp/rclcpp.hpp"
// 我们要用的消息类型：std_msgs 包里的 String 消息，里面只有一个 std::string data 字段
#include "std_msgs/msg/string.hpp"

// 让 1s、500ms 这种写法可用（字面量后缀）
using namespace std::chrono_literals;

// ---------------------------------------------------------------------------
// 自定义节点类：继承 rclcpp::Node
// 继承之后，这个类就"是一个节点"了，才能创建发布者、定时器这些 ROS 资源
// ---------------------------------------------------------------------------
class ChatPublisher : public rclcpp::Node
{
public:
  // 构造函数。: Node("chat_publisher") 是初始化基类，
  // 括号里的字符串就是节点名（ros2 node list 会显示这个名字）
  ChatPublisher()
  : Node("chat_publisher"), index_(0)
  {
    // ---------- 第 1 步：创建发布者 ----------
    // create_publisher<消息类型>("话题名", 队列长度)
    //   /chat            话题的名字，发布者和订阅者必须一字不差
    //   10               队列长度（QoS depth）：网络/处理来不及时最多缓存 10 条，超出的丢掉
    publisher_ = this->create_publisher<std_msgs::msg::String>("/chat", 10);

    // ---------- 第 2 步：声明一个参数 ----------
    // 参数可以让 launch 文件在启动时改值，不用重新编译。
    // declare_parameter<double>("参数名", 默认值) 返回默认值
    const double interval = this->declare_parameter<double>("interval", 1.0);

    // 准备要发布的句子。这里模拟一个"提问者"轮流问 5 句话
    questions_ = {"你好", "你是谁", "今天学什么", "这个作业难吗", "再见"};

    // ---------- 第 3 步：创建定时器 ----------
    // 每隔 interval 秒自动执行一次 timer_callback()，这就是"周期性发布"的常见写法
    timer_ = this->create_wall_timer(
      std::chrono::duration<double>(interval),                        // 间隔时间
      std::bind(&ChatPublisher::timer_callback, this));               // 到点后调用哪个函数

    // 在终端打印一行日志（ros2 launch 里带上 output='screen' 才能看到）
    RCLCPP_INFO(
      this->get_logger(),
      "chat_publisher 已启动，每 %.1f 秒向话题 /chat 发布一条消息", interval);
  }

private:
  // 定时器的回调函数：定时器到点就调用它
  void timer_callback()
  {
    // 1) 造一条空消息
    auto msg = std_msgs::msg::String();

    // 2) 填入内容。用 index_ % size() 实现"循环取句子"，取完最后一句回到第一句
    msg.data = questions_[index_ % questions_.size()];

    // 3) 真正发出去。publish() 之后，所有订阅了 /chat 的节点都会收到
    publisher_->publish(msg);

    // 4) 自己也打印一下，方便对照
    RCLCPP_INFO(this->get_logger(), "发布 -> \"%s\"", msg.data.c_str());

    ++index_;  // 下一句
  }

  // 成员变量：一律用 SharedPtr（智能指针），ROS2 里资源都是共享管理的
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;  // 发布者句柄
  rclcpp::TimerBase::SharedPtr timer_;                            // 定时器句柄
  std::vector<std::string> questions_;                            // 句子表
  size_t index_;                                                  // 当前发到第几句
};

// ---------------------------------------------------------------------------
// main：程序的入口，ROS2 程序的固定三段式
// ---------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  // 1) 初始化 ROS2 客户端库，解析命令行参数（比如 --ros-args -p interval:=2.0）
  rclcpp::init(argc, argv);

  // 2) 创建节点并「自旋」。
  //    spin 是一个死循环：不断地把收到的消息/到期的定时器分发给对应回调函数。
  //    没有 spin，回调函数永远不会被调用，程序只会建好节点就退出。
  rclcpp::spin(std::make_shared<ChatPublisher>());

  // 3) 收到 Ctrl+C 后清理资源退出
  rclcpp::shutdown();
  return 0;
}
