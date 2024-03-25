//
// Created by Wu on 24-3-14.
//

#include <vector>
#include <Eigen/Dense>
#include "../include/comm_node.hpp"

using namespace std::chrono;

namespace rmos_transporter
{
    UsbCommNode::UsbCommNode(const rclcpp::NodeOptions &options) : CommNode("usb_comm", options)
    {

        std::map<std::string, int> transporter_params {
            {"interface_usb_vid", 0},
            {"interface_usb_pid", 0},
            {"interface_usb_read_endpoint", 0},
            {"interface_usb_write_endpoint", 0},
            {"interface_usb_read_timeout", 0},
            {"interface_usb_write_timeout", 0},
        };

        this->declare_parameters("", transporter_params);
        // transporter parameter
        this->get_parameter<int>("interface_usb_vid", interface_usb_vid_);
        this->get_parameter<int>("interface_usb_pid", interface_usb_pid_);
        this->get_parameter<int>("interface_usb_read_endpoint", interface_usb_read_endpoint_);
        this->get_parameter<int>("interface_usb_write_endpoint", interface_usb_write_endpoint_);
        this->get_parameter<int>("interface_usb_read_timeout", interface_usb_read_timeout_);
        this->get_parameter<int>("interface_usb_write_timeout", interface_usb_write_timeout_);

        // create callback group
        this->receive_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        this->target_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // create publisher and subscriber
        this->tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        auto target_sub_options = rclcpp::SubscriptionOptions();
        target_sub_options.callback_group = this->target_sub_callback_group_;
        this->target_sub_ = this->create_subscription<rmos_interfaces::msg::Target>
                ("/target", rclcpp::SensorDataQoS(),
                 std::bind(&UsbCommNode::targetCallBack, this, std::placeholders::_1),
                 target_sub_options);

        this->receive_timer_ = this->create_wall_timer(1ms, std::bind(&UsbCommNode::recevieCallBack, this));
        // 可尝试定时器会不会出问题 再用线程
        // this->recevie_thread_ = std::thread(&UsbCommNode::recevieCallBack, this);

        quaternion_time_msg_.quaternion_stamped.header.frame_id = std::string("Imu");

        RCLCPP_INFO(this->get_logger(), "Init Transporter");
        transporter_ = std::make_shared<transporter_sdk::UsbcdcTransporter>(
            interface_usb_vid_, 
            interface_usb_pid_, 
            interface_usb_read_endpoint_, 
            interface_usb_write_endpoint_, 
            interface_usb_read_timeout_, 
            interface_usb_write_timeout_
        );

        RCLCPP_INFO(this->get_logger(), "Open Transporter");
        if (transporter_->open() == true) {
            RCLCPP_INFO(this->get_logger(), "Success");
        }
        else {
            RCLCPP_INFO(this->get_logger(), "FAILED!!!");
        }
        RCLCPP_INFO(this->get_logger(), "Finish Init");

    }

    UsbCommNode::~UsbCommNode()
    {
        // if (recevie_thread_.joinable())
        //     recevie_thread_.join();
    }

    void UsbCommNode::targetCallBack(const rmos_interfaces::msg::Target::SharedPtr target)
    {
        if(debug::get_debug_option(base::SHOW_TOTAL_COST))
        {
            if(target->gun_pitch!=0)
            {
                rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
                auto time = steady_clock_.now();
                RCLCPP_INFO(this->get_logger(), "Cost %.4f ms",(time.seconds()-last_time_) * 1000);
                last_time_ = time.seconds();
            }
        }
        send_package_._SOF = 0x55;
        send_package_._EOF = 0xFF;
        send_package_.ID = RMOS_SEND_ID;
        this->target2state(target, &(send_package_.AimbotState));
        send_package_.PitchRelativeAngle = (int16_t)(target->gun_pitch * 32768.0 / 180.0);
        send_package_.YawRelativeAngle = (int16_t)(target->gun_yaw * 32768.0 / 180.0);
        send_package_.SystemTimer = (int16_t)(target->timestamp_recv);
        transporter_->write((unsigned char *)&send_package_, sizeof(transporter::RMOSSendPackage));
    }

    void UsbCommNode::target2state(const rmos_interfaces::msg::Target::SharedPtr target, u_char *buf)
    {
        memset(buf, 0, 3);
        switch ((base::TrackState)target->track_state)
        {
            case base::TrackState::TRACKING:
            case base::TrackState::TEMP_LOST:
                buf[0] |= 0x01;
                break;
            default:
                break;
        }
        switch(target->outpost_direction)
        {
            case 1:
                buf[0] |= 0x40;
            case -1:
                buf[0] |= 0;

        }
        if (target->suggest_fire)
            buf[0] |= 0x02;
        switch (target->id)
        {
            case 0:
                buf[1] |= 0x80;
                break;
            case 1:
                buf[1] |= 0x01;
                break;
            case 2:
                buf[1] |= 0x02;
                break;
            case 3:
            case 11:
                buf[1] |= 0x04;
                break;
            case 4:
            case 12:
                buf[1] |= 0x08;
                break;
            case 5:
                buf[1] |= 0x10;
                break;
            case 6:
                buf[1] |= 0x40;
                break;
            case 7:
                buf[1] |= 0x20;
                break;
            default:
                break;
        }

        double distance = sqrt(target->position.x *  target->position.x +
                               target->position.y *  target->position.y +
                               target->position.z *  target->position.z);

        if(distance<3){

            buf[2] |= 0x01;

        }
        else if(3< distance&&distance<5){

            buf[2] |= 0x02;

        }
        else{

            buf[2] |= 0x03;

        }
    }

    void UsbCommNode::recevieCallBack()
    {
        uint8_t receive_package[64];
        int read_size = transporter_->read(receive_package, 64);
        // RCLCPP_INFO(gimbal_hw_node_->get_logger(), "read size : %d", read_size);

        switch (receive_package[1])
        {
            
            case RMOS_REFEREE_RECEIVE_ID:
            {
                transporter::RMOSRefereeReceivePackage package;
                memcpy(&package, receive_package, 
                    sizeof(transporter::RMOSRefereeReceivePackage));

                // color
                if (package.robot_id < 10) {
                    color_msg_.color = (int) (base::Color::BLUE);
                } else if (package.robot_id > 100) {
                    color_msg_.color = (int) base::Color::RED;
                } else {
                    color_msg_.color = (int) (base::Color::BLUE);
                }
                this->color_pub_->publish(color_msg_);

                // TODO : 除哨兵外的车需要的信息

                break;
            }
            case RMOS_IMU_0_RECEIVE_ID:
            {
                transporter::RMOSIMUReceivePackage package;
                memcpy(&package, receive_package, 
                    sizeof(transporter::RMOSIMUReceivePackage));
                int time_offset = 0;
                quaternion_time_msg_.quaternion_stamped.header.stamp = this->now() + rclcpp::Duration(0,time_offset);
                quaternion_time_msg_.quaternion_stamped.quaternion.w = (double)package.q0;
                quaternion_time_msg_.quaternion_stamped.quaternion.x = (double)package.q1;
                quaternion_time_msg_.quaternion_stamped.quaternion.y = (double)package.q2;
                quaternion_time_msg_.quaternion_stamped.quaternion.z = (double)package.q3;

                geometry_msgs::msg::TransformStamped t;
                t.header.stamp =  quaternion_time_msg_.quaternion_stamped.header.stamp;
                t.header.frame_id = "world"; //注意坐标系
                t.child_frame_id = "IMU";
                t.transform.rotation.x = quaternion_time_msg_.quaternion_stamped.quaternion.x;
                t.transform.rotation.y = quaternion_time_msg_.quaternion_stamped.quaternion.y;
                t.transform.rotation.z = quaternion_time_msg_.quaternion_stamped.quaternion.z;
                t.transform.rotation.w = quaternion_time_msg_.quaternion_stamped.quaternion.w;
                tf_publisher_->sendTransform(t);
                memcpy(&quaternion_time_msg_.timestamp_recv, &package.TimeStamp, 4);
                    this->quaternion_pub_->publish(quaternion_time_msg_);
                break;
            }
            case RMOS_IMU_1_RECEIVE_ID:
            {
                transporter::RMOSIMUReceivePackage package;
                memcpy(&package, receive_package, 
                    sizeof(transporter::RMOSIMUReceivePackage));
                int time_offset = 0;
                quaternion_time_msg_.quaternion_stamped.header.stamp = this->now() + rclcpp::Duration(0,time_offset);
                quaternion_time_msg_.quaternion_stamped.quaternion.w = (double)package.q0;
                quaternion_time_msg_.quaternion_stamped.quaternion.x = (double)package.q1;
                quaternion_time_msg_.quaternion_stamped.quaternion.y = (double)package.q2;
                quaternion_time_msg_.quaternion_stamped.quaternion.z = (double)package.q3;
                        
                geometry_msgs::msg::TransformStamped t;
                t.header.stamp =  quaternion_time_msg_.quaternion_stamped.header.stamp;
                t.header.frame_id = "world"; //注意坐标系
                t.child_frame_id = "IMU";
                t.transform.rotation.x = quaternion_time_msg_.quaternion_stamped.quaternion.x;
                t.transform.rotation.y = quaternion_time_msg_.quaternion_stamped.quaternion.y;
                t.transform.rotation.z = quaternion_time_msg_.quaternion_stamped.quaternion.z;
                t.transform.rotation.w = quaternion_time_msg_.quaternion_stamped.quaternion.w;
                tf_publisher_->sendTransform(t);
                memcpy(&quaternion_time_msg_.timestamp_recv, &package.TimeStamp, 4);
                    this->quaternion_pub_->publish(quaternion_time_msg_);
                break;
            }
        
        }
    }

    void UsbCommNode::ForceSetMode(rmos_interfaces::msg::Mode &mode_msg)
    {
        cv::FileStorage fs("./src/Algorithm/configure/Detector/detector/rune_detector/Rune.xml",cv::FileStorage::READ);
        if(!fs.isOpened())
        {
            std::cout<<"Rune.xml is not opened!"<<std::endl;
        }
        int force_mode;
        fs["force_mode"]>>force_mode;

        switch(force_mode)
        {
            case 0: break;
            case 1:
            {
                mode_msg.mode = (int)base::Mode::NORMAL;
                break;
            }
            case 2:
            {
                mode_msg.mode = (int)base::Mode::NORMAL_RUNE;
                break;
            }
            case 3:
            {
                mode_msg.mode = (int) base::Mode::RUNE;
                break;
            }
            default:
            {
                std::cout<<"Wrong force_mode param!"<<std::endl;
            }

        }
    }
    void UsbCommNode::tellMode(rmos_interfaces::msg::Mode mode_msg)
    {
        if(mode_msg.mode == (int) base::Mode::RUNE)
        {
            std::cout<<"RUNE"<<std::endl;
        }
        if(mode_msg.mode == (int) base::Mode::NORMAL)
        {
            std::cout<<"NORMAL"<<std::endl;
        }
        if(mode_msg.mode == (int) base::Mode::NORMAL_RUNE)
        {
            std::cout<<"NORMAL_RUNE"<<std::endl;
        }
    }

} // namespace rmos_comm
#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_transporter::UsbCommNode)
