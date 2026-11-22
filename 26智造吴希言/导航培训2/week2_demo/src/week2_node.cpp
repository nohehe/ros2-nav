// week2_node.cpp — 自动连续动作 + 遇障停车
// 发布 /cmd_vel，订阅 /scan

#include <chrono>
#include <memory>
#include <cmath>
#include <algorithm>
#include <limits>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

class Week2Node : public rclcpp::Node
{
public:
  Week2Node() : Node("week2_node")
  {
    forward_speed_ = 0.2;      // m/s
    turn_speed_ = 0.5;         // rad/s
    forward_duration_ = 3.0;   // s
    turn_duration_ = 2.0;      // s
    obstacle_distance_ = 0.5;  // m
    front_half_angle_ = 0.3;   // rad

    pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/scan", 10,
      std::bind(&Week2Node::scan_callback, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(
      100ms, std::bind(&Week2Node::timer_callback, this));

    start_time_ = this->now();

    RCLCPP_INFO(this->get_logger(), "week2_node started: auto motion + obstacle stop");
    RCLCPP_INFO(this->get_logger(),
      "  forward: %.2f m/s x %.1fs | turn: %.2f rad/s x %.1fs | stop if front < %.2f m",
      forward_speed_, forward_duration_, turn_speed_, turn_duration_, obstacle_distance_);
  }

private:
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    if (msg->ranges.empty()) {
      return;
    }

    // 雷达正前方是 0 弧度，在数组里不在正中间，得按角度参数换算索引
    const int center = static_cast<int>(
      std::round((0.0 - msg->angle_min) / msg->angle_increment));

    // 正前方左右各 front_half_angle_ 弧度这一段
    const int half_n = static_cast<int>(
      std::round(front_half_angle_ / msg->angle_increment));

    int lo = std::max(0, center - half_n);
    int hi = std::min(static_cast<int>(msg->ranges.size()) - 1, center + half_n);

    float min_dist = std::numeric_limits<float>::infinity();
    for (int i = lo; i <= hi; ++i) {
      const float r = msg->ranges[i];
      // inf 表示这一束没打到东西，0.0 表示太近，两种都不能当障碍
      if (!std::isfinite(r) || r < msg->range_min) {
        continue;
      }
      if (r < min_dist) {
        min_dist = r;
      }
    }

    front_min_dist_ = min_dist;

    const bool blocked = (min_dist < obstacle_distance_);
    if (blocked != obstacle_detected_) {
      // 每 0.1 秒判一次，只在状态变化时打日志
      if (blocked) {
        RCLCPP_WARN(this->get_logger(),
          "OBSTACLE detected at %.2f m (threshold %.2f m) -> STOP",
          min_dist, obstacle_distance_);
      } else {
        RCLCPP_INFO(this->get_logger(),
          "path clear (front min %.2f m) -> resume motion", min_dist);
      }
    }
    obstacle_detected_ = blocked;
  }

  void timer_callback()
  {
    geometry_msgs::msg::Twist cmd;

    if (obstacle_detected_) {
      cmd.linear.x = 0.0;
      cmd.angular.z = 0.0;
      phase_ = Phase::STOPPED;
    } else {
      const double elapsed =
        (this->now() - start_time_).seconds();

      switch (phase_) {
        case Phase::FORWARD:
          cmd.linear.x = forward_speed_;
          cmd.angular.z = 0.0;
          if (elapsed - phase_start_ >= forward_duration_) {
            phase_ = Phase::TURNING;
            phase_start_ = elapsed;
            RCLCPP_INFO(this->get_logger(), "phase -> TURNING");
          }
          break;

        case Phase::TURNING:
          cmd.linear.x = 0.0;
          cmd.angular.z = turn_speed_;
          if (elapsed - phase_start_ >= turn_duration_) {
            phase_ = Phase::FORWARD;
            phase_start_ = elapsed;
            RCLCPP_INFO(this->get_logger(), "phase -> FORWARD");
          }
          break;

        case Phase::STOPPED:
          // 障碍解除，从前进重新开始
          phase_ = Phase::FORWARD;
          phase_start_ = elapsed;
          cmd.linear.x = forward_speed_;
          cmd.angular.z = 0.0;
          RCLCPP_INFO(this->get_logger(), "phase -> FORWARD (resume)");
          break;
      }
    }

    pub_->publish(cmd);
  }

  enum class Phase { FORWARD, TURNING, STOPPED };

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time start_time_;

  Phase phase_{Phase::FORWARD};
  double phase_start_{0.0};

  bool obstacle_detected_{false};
  float front_min_dist_{std::numeric_limits<float>::infinity()};

  double forward_speed_;
  double turn_speed_;
  double forward_duration_;
  double turn_duration_;
  double obstacle_distance_;
  double front_half_angle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Week2Node>());
  rclcpp::shutdown();
  return 0;
}
