

#ifndef RMOS_SIMULATOR_NODE_HPP
#define RMOS_SIMULATOR_NODE_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>


#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/quaternion_stamped.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/pose.hpp>

//#include <geometry_msgs/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <visualization_msgs/msg/marker_array.hpp>

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int16.hpp"

#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "message_filters/subscriber.h"

//interfaces
#include "rmos_interfaces/msg/armors.hpp"
#include "rmos_interfaces/msg/armor.hpp"
#include "rmos_interfaces/msg/target.hpp"
#include "rmos_interfaces/msg/quaternion_time.hpp"
#include "rmos_interfaces/msg/bullet_speed.hpp"
#include "rmos_interfaces/msg/autoaim_state.hpp"

#include "rmos_interfaces/msg/color.hpp"
#include "rmos_interfaces/msg/bullet_speed.hpp"
#include "rmos_interfaces/msg/mode.hpp"
#include "../../Algorithm/include/Transporter/can/can.hpp"
#include "../../Algorithm/include/Debug/debug.hpp"
namespace rmos_simulator
{
    class SimulatorNode : public rclcpp::Node{
    public:
    SimulatorNode(const rclcpp::NodeOptions &options);
             ~SimulatorNode() = default;


    protected:
        rclcpp::Subscription<rmos_interfaces::msg::Target>::SharedPtr target_sub_;

        rclcpp::Publisher<rmos_interfaces::msg::QuaternionTime>::SharedPtr quaternion_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::Color>::SharedPtr color_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::BulletSpeed>::SharedPtr bs_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::Mode>::SharedPtr mode_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::AutoaimState>::SharedPtr autoaim_state_pub_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_;

        void quaternionCallBack(const geometry_msgs::msg::Pose::SharedPtr quaternion_msg);
        message_filters::Subscriber<rmos_interfaces::msg::Armors> armors_sub_;
        rclcpp::CallbackGroup::SharedPtr armors_sub_callback_group_;
        rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr quaternion_sub_;
        rclcpp::CallbackGroup::SharedPtr quaternion_sub_callback_group_;
        rclcpp::Subscription<rmos_interfaces::msg::BulletSpeed>::SharedPtr bs_sub_;
        rclcpp::CallbackGroup::SharedPtr bs_sub_callback_group_;
        rclcpp::Subscription<rmos_interfaces::msg::AutoaimState>::SharedPtr autoaim_state_sub_;
        rclcpp::CallbackGroup::SharedPtr aimstate_sub_callback_group_;

    };

}
#endif //RMOS_SIMULATOR_NODE_HPP