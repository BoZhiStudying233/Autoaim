#include <vector>
#include <Eigen/Dense>
#include "../include/simulator_node.hpp"



namespace rmos_simulator
{
    SimulatorNode::SimulatorNode(const rclcpp::NodeOptions &options) : Node("simulator", options)
    {
        RCLCPP_INFO(this->get_logger(), "Start Simulator Node");

            this->quaternion_pub_ = this->create_publisher<rmos_interfaces::msg::QuaternionTime>("/imu_quaternion", rclcpp::SensorDataQoS());

            this->color_pub_ = this->create_publisher<rmos_interfaces::msg::Color>("/color_info", rclcpp::SensorDataQoS());
            this->bs_pub_ = this->create_publisher<rmos_interfaces::msg::BulletSpeed>("/bs_info", rclcpp::SensorDataQoS());
            this->mode_pub_ = this->create_publisher<rmos_interfaces::msg::Mode>("/mode_info", rclcpp::SensorDataQoS());
            this->autoaim_state_pub_ = this->create_publisher<rmos_interfaces::msg::AutoaimState>("/autoaim_state", rclcpp::SensorDataQoS());
       
        this->tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);



        using rclcpp::CallbackGroupType;


        this->quaternion_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto quaternion_sub_option = rclcpp::SubscriptionOptions();
        quaternion_sub_option.callback_group = this->quaternion_sub_callback_group_;
        this->quaternion_sub_ = this->create_subscription<geometry_msgs::msg::Pose>("/camera_pose", rclcpp::SensorDataQoS(),
                                                                                                std::bind(&SimulatorNode::quaternionCallBack, this, std::placeholders::_1),
                                                                                                quaternion_sub_option);

    }



   void SimulatorNode::quaternionCallBack(const geometry_msgs::msg::Pose::SharedPtr quaternion_msg)
  {
    rmos_interfaces::msg::Color color_msg;
    rmos_interfaces::msg::Mode mode_msg;
    rmos_interfaces::msg::BulletSpeed bs_msg;
    rmos_interfaces::msg::AutoaimState autoaim_state_msg;
    rmos_interfaces::msg::QuaternionTime quaternion_time_msg;
    int time_offset = 0;
    quaternion_time_msg.quaternion_stamped.header.stamp = this->now() + rclcpp::Duration(0,time_offset);
    quaternion_time_msg.quaternion_stamped.header.frame_id="camera_optical_frame";

    //q_temp = Eigen::Quaterniond(quaternion);
    //q_temp.normalize();
    quaternion_time_msg.quaternion_stamped.quaternion.w = (*quaternion_msg).orientation.w;
    quaternion_time_msg.quaternion_stamped.quaternion.x = (*quaternion_msg).orientation.x;
    quaternion_time_msg.quaternion_stamped.quaternion.y = (*quaternion_msg).orientation.y;
    quaternion_time_msg.quaternion_stamped.quaternion.z = (*quaternion_msg).orientation.z;

    geometry_msgs::msg::TransformStamped t;
    t.header.stamp =  quaternion_time_msg.quaternion_stamped.header.stamp;
    t.header.frame_id = "world";
    t.child_frame_id = "camera_optical_frame";
    t.transform.rotation.x = quaternion_time_msg.quaternion_stamped.quaternion.x;
    t.transform.rotation.y = quaternion_time_msg.quaternion_stamped.quaternion.y;
    t.transform.rotation.z = quaternion_time_msg.quaternion_stamped.quaternion.z;
    t.transform.rotation.w = quaternion_time_msg.quaternion_stamped.quaternion.w;
    tf_publisher_->sendTransform(t);
    bs_msg.speed = 30;
    this->bs_pub_->publish(bs_msg);
    color_msg.color = (int) base::Color::RED;
    //color_msg.color = (int) (base::Color::BLUE);
    this->color_pub_->publish(color_msg);
    this->quaternion_pub_->publish(quaternion_time_msg);
    
    }

}


#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_simulator::SimulatorNode)






