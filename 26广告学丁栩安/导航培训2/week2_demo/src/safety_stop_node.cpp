// ============================================================================
//  safety_stop_node.cpp —— 避障停车节点（安全过滤）
//
//  作用：订阅激光雷达 /scan，计算「正前方扇区」内最近的障碍距离；
//        同时订阅 /cmd_vel_raw（其他节点想要执行的速度）。
//        - 前方距离 >= stop_distance：把 /cmd_vel_raw 原样转发到 /cmd_vel
//        - 前方距离 <  stop_distance：强制发布全零速度，机器人立即停车
//
//  这就是机器人上常见的「安全层」写法：任何运动指令都要过一遍安全检查。
// ============================================================================

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

class SafetyStopNode : public rclcpp::Node
{
public:
  SafetyStopNode()
  : Node("safety_stop_node"), front_min_(100.0), has_scan_(false), blocked_(false)
  {
    stop_distance_ = this->declare_parameter<double>("stop_distance", 0.5);
    front_fov_deg_ = this->declare_parameter<double>("front_fov_deg", 30.0);

    // 最终发给底盘的话题，名字必须是 /cmd_vel
    pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // 期望速度（来自 auto_move_node）
    raw_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel_raw", 10,
      [this](const geometry_msgs::msg::Twist::SharedPtr msg) {raw_ = *msg;});

    // 激光雷达：传感器数据用 SensorDataQoS（best effort），和 Gazebo 发布端一致
    scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/scan", rclcpp::SensorDataQoS(),
      std::bind(&SafetyStopNode::on_scan, this, std::placeholders::_1));

    // 10 Hz 持续输出，保证机器人不会因为"指令停发"而乱走
    timer_ = this->create_wall_timer(100ms, std::bind(&SafetyStopNode::tick, this));

    RCLCPP_INFO(
      this->get_logger(),
      "安全停车节点已启动：停车距离=%.2f m，前向扇区=±%.0f°", stop_distance_, front_fov_deg_);
  }

private:
  // 从一帧激光数据里，求正前方扇区内最近的障碍距离
  void on_scan(const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    const double half_fov = front_fov_deg_ * M_PI / 180.0;
    double best = std::numeric_limits<double>::infinity();

    for (size_t i = 0; i < msg->ranges.size(); ++i) {
      const double angle = msg->angle_min + static_cast<double>(i) * msg->angle_increment;
      if (std::abs(angle) > half_fov) {
        continue;  // 只关心正前方
      }
      const double r = msg->ranges[i];
      if (!std::isfinite(r) || r <= 0.0f) {
        continue;  // 无效点跳过
      }
      best = std::min(best, r);
    }

    if (std::isfinite(best)) {
      front_min_ = best;
      has_scan_ = true;
    }
  }

  void tick()
  {
    const bool blocked = has_scan_ && (front_min_ < stop_distance_);

    geometry_msgs::msg::Twist out;  // 默认全 0
    if (blocked) {
      if (!blocked_) {
        RCLCPP_WARN(
          this->get_logger(), "前方 %.2f m 检测到障碍物 —— 避障停车！", front_min_);
      }
      RCLCPP_WARN_THROTTLE(
        this->get_logger(), *this->get_clock(), 1000,
        "前方 %.2f m 有障碍物，已停车", front_min_);
      blocked_ = true;
    } else {
      if (blocked_) {
        RCLCPP_INFO(this->get_logger(), "障碍物已清除（前方 %.2f m），恢复运动", front_min_);
      }
      out = raw_;
      RCLCPP_INFO_THROTTLE(
        this->get_logger(), *this->get_clock(), 500,
        "前方最近障碍距离 %.2f m —— 允许通行", front_min_);
      blocked_ = false;
    }

    pub_->publish(out);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr raw_sub_;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  geometry_msgs::msg::Twist raw_;  // 最近一次收到的期望速度
  double stop_distance_;
  double front_fov_deg_;
  double front_min_;
  bool has_scan_;
  bool blocked_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SafetyStopNode>());
  rclcpp::shutdown();
  return 0;
}
