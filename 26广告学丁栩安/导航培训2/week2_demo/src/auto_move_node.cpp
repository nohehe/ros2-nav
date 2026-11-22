// ============================================================================
//  auto_move_node.cpp —— 自动连续动作节点
//
//  作用：按固定时间顺序，循环发出「前进 → 左转 → 前进 → 右转」的速度指令，
//        让机器人在 Gazebo 里自动、连续地运动，不需要人手动控制。
//
//  注意：本节点发布到 /cmd_vel_raw（"期望速度"），不直接发 /cmd_vel。
//        由 safety_stop_node 做安全检查后再转发到 /cmd_vel，
//        这样"避障停车"才能真正拦得住自动动作。
// ============================================================================

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class AutoMoveNode : public rclcpp::Node
{
public:
  AutoMoveNode()
  : Node("auto_move_node"), phase_(0)
  {
    // mode = "sequence"：前进/左转/前进/右转 循环
    // mode = "straight"：一直前进（用来演示避障停车）
    mode_ = this->declare_parameter<std::string>("mode", "sequence");
    linear_speed_ = this->declare_parameter<double>("linear_speed", 0.3);
    angular_speed_ = this->declare_parameter<double>("angular_speed", 0.6);
    forward_time_ = this->declare_parameter<double>("forward_time", 4.0);
    turn_time_ = this->declare_parameter<double>("turn_time", 2.0);

    pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel_raw", 10);

    phase_start_ = std::chrono::steady_clock::now();
    // 10 Hz 定时器：每 100 ms 发一次速度指令（Gazebo 底盘需要持续的指令）
    timer_ = this->create_wall_timer(100ms, std::bind(&AutoMoveNode::tick, this));

    RCLCPP_INFO(
      this->get_logger(),
      "自动动作节点已启动：mode=%s, 线速度=%.2f m/s, 角速度=%.2f rad/s",
      mode_.c_str(), linear_speed_, angular_speed_);
  }

private:
  void tick()
  {
    const auto now = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(now - phase_start_).count();

    geometry_msgs::msg::Twist cmd;  // 默认全 0

    if (mode_ == "straight") {
      cmd.linear.x = linear_speed_;
    } else {
      switch (phase_) {
        case 0:  // 前进
          cmd.linear.x = linear_speed_;
          break;
        case 1:  // 左转（逆时针）
          cmd.angular.z = angular_speed_;
          break;
        case 2:  // 前进
          cmd.linear.x = linear_speed_;
          break;
        case 3:  // 右转（顺时针）
          cmd.angular.z = -angular_speed_;
          break;
        default:
          break;
      }

      // 本阶段时间到，切换到下一阶段
      const double limit = (phase_ % 2 == 0) ? forward_time_ : turn_time_;
      if (elapsed > limit) {
        phase_ = (phase_ + 1) % 4;
        phase_start_ = now;
        RCLCPP_INFO(
          this->get_logger(), "切换动作阶段 -> %d（%s）", phase_,
          phase_name(phase_));
      }
    }

    pub_->publish(cmd);
  }

  static const char * phase_name(int p)
  {
    switch (p) {
      case 0: return "前进";
      case 1: return "左转";
      case 2: return "前进";
      case 3: return "右转";
      default: return "未知";
    }
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string mode_;
  double linear_speed_;
  double angular_speed_;
  double forward_time_;
  double turn_time_;
  int phase_;
  std::chrono::steady_clock::time_point phase_start_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AutoMoveNode>());
  rclcpp::shutdown();
  return 0;
}
