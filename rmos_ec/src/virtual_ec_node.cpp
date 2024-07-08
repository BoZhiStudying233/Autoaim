#include <chrono>
#include <vector>
#include <Eigen/Dense>
#include "../include/ec_node.hpp"

using namespace std::chrono;

namespace rmos_ec
{
    VirtualECNode::VirtualECNode(const rclcpp::NodeOptions & options) : ECNode("virtual_ec_node", options)
    {
        // create callback group
        this->receive_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);


        // create publisher and subscriber
        this->tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        // subscriber is write by ec
        //...

        // Timer
        this->receive_timer_ = this->create_wall_timer(1ms, std::bind(&VirtualECNode::recevieCallBack, this));

        RCLCPP_INFO(this->get_logger(), "Init VirtualECNode");

    }

    VirtualECNode::~VirtualECNode()
    {
        
    }
    void VirtualECNode::recevieCallBack()
    {
        //color_msg_.color = 1;//(int)(base::Color::BLUE);
        color_msg_.color = 0;//(int)(base::Color::RED);
        this->autoaim_state_msg_.autoaim_state = ( rand() % 2 == 0 ? 0 : 1 );
        this->mode_msg_.data = (int) (0);


        this->transformstamped.transform.rotation.x = (double) 1.0;
        this->transformstamped.transform.rotation.y = (double) 1.0;
        this->transformstamped.transform.rotation.z = (double) 1.0;
        this->transformstamped.transform.rotation.w = (double) 1.0;
        //color
        this->color_pub_->publish(color_msg_);

        //autoaim state
        this->autoaim_state_pub_->publish(autoaim_state_msg_);

        //mode
        this->mode_pub_->publish(mode_msg_);

        //tf
        this->transformstamped.header.stamp = this->now();
        this->transformstamped.header.frame_id = "world";
        this->transformstamped.child_frame_id = "IMU";

        tf_publisher_->sendTransform(transformstamped);

    }


}

#include <rclcpp_components/register_node_macro.hpp>


RCLCPP_COMPONENTS_REGISTER_NODE(rmos_ec::VirtualECNode)