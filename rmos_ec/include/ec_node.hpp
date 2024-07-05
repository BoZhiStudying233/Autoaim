#include <memory>
#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/quaternion_stamped.hpp>
#include "std_msgs/msg/int8.hpp"
#include "rmos_interfaces/msg/target.hpp"  
#include "rmos_interfaces/msg/quaternion_time.hpp"
#include "rmos_interfaces/msg/color.hpp"
#include "rmos_interfaces/msg/bullet_speed.hpp"  
// #include "rmos_interfaces/msg/mode.hpp"
#include "rmos_interfaces/msg/autoaim_state.hpp"

// #include "../../Algorithm/include/Virtual_EC/virtual_ec.hpp"

namespace rmos_ec {
    class ECNode : public rclcpp::Node {
    public:
        ECNode(const std::string &node_name, const rclcpp::NodeOptions &options) : Node(node_name, options) {

            color_pub_ = this->create_publisher<rmos_interfaces::msg::Color>("/color_info", rclcpp::SensorDataQoS());
            mode_pub_ = this->create_publisher<std_msgs::msg::Int8>("/mode_info", rclcpp::SensorDataQoS());
            autoaim_state_pub_ = this->create_publisher<rmos_interfaces::msg::AutoaimState>("/autoaim_state", rclcpp::SensorDataQoS());

        }
    protected:
        rclcpp::Publisher<rmos_interfaces::msg::Color>::SharedPtr color_pub_;
        rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr mode_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::AutoaimState>::SharedPtr autoaim_state_pub_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_;        
    };
    class VirtualECNode : public ECNode {
    public:
        VirtualECNode(const rclcpp::NodeOptions & options);
        ~VirtualECNode() ;
        void recevieCallBack();
    protected:
        /* Send Timer */
        rclcpp::TimerBase::SharedPtr receive_timer_;

        /* Callback Group */
        rclcpp::CallbackGroup::SharedPtr receive_callback_group_;

        rclcpp::Publisher<rmos_interfaces::msg::Color>::SharedPtr color_pub_; 
        rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr mode_pub_; 
        rclcpp::Publisher<rmos_interfaces::msg::AutoaimState>::SharedPtr autoaim_state_pub_; 
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_; 
    
        // msg
        rmos_interfaces::msg::Color color_msg_;
        std_msgs::msg::Int8 mode_msg_;
        rmos_interfaces::msg::AutoaimState autoaim_state_msg_;

        geometry_msgs::msg::TransformStamped transformstamped;
    
    };  // class VirtualECNode 
}  // namespace rmos_ec 