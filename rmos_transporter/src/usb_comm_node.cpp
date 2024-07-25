#include <vector>
#include <Eigen/Dense>
#include "rmos_transporter/comm_node.hpp"
#include "rmos_utils/const.hpp"
#include <chrono>  
using namespace std::chrono;

namespace rmos_transporter
{
    UsbCommNode::UsbCommNode(const rclcpp::NodeOptions &options) : CommNode("usb_comm", options)
    {
        // this->setUsbPermission();
        // create callback group
        this->receive_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        this->target_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // create publisher and subscriber
        auto target_sub_options = rclcpp::SubscriptionOptions();
        target_sub_options.callback_group = this->target_sub_callback_group_;
        this->target_sub_ = this->create_subscription<rmos_interfaces::msg::Target>
                ("/target", rclcpp::SensorDataQoS(),
                 std::bind(&UsbCommNode::targetCallBack, this, std::placeholders::_1),
                 target_sub_options);

        this->receive_timer_ = this->create_wall_timer(100ms, std::bind(&UsbCommNode::recevieCallBack, this));
        // 可尝试定时器会不会出问题 再用线程
        // this->recevie_thread_ = std::thread(&UsbCommNode::recevieCallBack, this);

        quaternion_time_msg_.quaternion_stamped.header.frame_id = std::string("Imu");
        this->tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        interface_usb_vid_ = this->declare_parameter("interface_usb_vid", 0x0483);
        interface_usb_pid_ = this->declare_parameter("interface_usb_pid", 0x5740);
        interface_usb_read_endpoint_ = this->declare_parameter("interface_usb_read_endpoint", 0x81);
        interface_usb_write_endpoint_ = this->declare_parameter("interface_usb_write_endpoint", 0x01);
        interface_usb_read_timeout_ = this->declare_parameter("interface_usb_read_timeout", 1);
        interface_usb_write_timeout_ = this->declare_parameter("interface_usb_write_timeout", 1);
        
        force_mode = this->declare_parameter("force_mode", 0);
        tell_mode = this->declare_parameter("tell_mode", 0);
        
        this->debug = this->declare_parameter("debug", 0);
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
            // exit(1);
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
        if(this->debug)
        {
            if(target->gun_pitch!=0)
            {
                rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
                auto time = steady_clock_.now();
                RCLCPP_INFO(this->get_logger(), "Cost %.4f ms",(time.seconds()-last_time_) * 1000);
                last_time_ = time.seconds();
            }
        }

        if(target->suggest_fire)
        {
            // std::ofstream file("output.csv", std::ios_base::app);
            // 如果你需要包括小数秒（即毫秒或更小的部分），你需要先转换为更精细的持续时间类型  
            auto now = std::chrono::high_resolution_clock::now(); 
            auto duration = now.time_since_epoch(); 
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);  
            double milliseconds_double = milliseconds.count();  
            double seconds_with_fraction = milliseconds_double / 1000.0; // 转换为秒  
            // seconds_with_fraction -= 1.7214e+09;
            this->interval_pub_->publish(seconds_with_fraction - this->last_fire_time);
            this->last_fire_time = seconds_with_fraction;
        }

        send_package_._SOF = 0x55;
        send_package_._EOF = 0xFF;
        send_package_.ID = RMOS_SEND_ID;
        this->target2state(target, &(send_package_.AimbotState));
        send_package_.PitchAbsoluteAngle = (float)(target->gun_pitch );//* 32768.0 / 180.0
        send_package_.YawAbsoluteAngle = (float)(target->gun_yaw );
        send_package_.SystemTimer = (int16_t)(target->timestamp_recv);
        send_package_.TargetYawSpeed = 0;
        send_package_.TargetPitchSpeed = 0;
        transporter_->write((unsigned char *)&send_package_, sizeof(transporter::RMOSSendPackage));
    }

    void UsbCommNode::target2state(const rmos_interfaces::msg::Target::SharedPtr target, u_char *buf)
    {
        memset(buf, 0,3);
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
        // this->send_package_.AimbotTarget = target->id;
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
                buf[1] |= 0x40;  // sentry
                break;
            case 7:
                buf[1] |= 0x20;
                break;
            default:
                break;
        }
    }

    void UsbCommNode::recevieCallBack()
    {
        uint8_t receive_package[64];
        int read_size = transporter_->read(receive_package, 64);
        // TODO
        // if(read_size == -1)
        // {
        //     rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
        //     if((steady_clock_.now() - last_receive_time_) > rclcpp::Duration(5, 0)) // 5s无数据包
        //     {
        //         setUsbPermission();
        //         transporter_->open();
        //     }
        //     return;
        // }
        // rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
        // last_receive_time_ = steady_clock_.now();;

        switch (receive_package[1])
        {
            
            case RMOS_RECEIVE_ID:
            {
                transporter::RMOSReceivePackage package;
                memcpy(&package, receive_package, 
                sizeof(transporter::RMOSReceivePackage));

                // color
                if (package.robot_id < 10) {
                    color_msg_.color = (int) (base::Color::BLUE);
                } else if (package.robot_id > 100) {
                    color_msg_.color = (int) base::Color::RED;
                } else {
                    color_msg_.color = (int) (base::Color::BLUE);
                }
                this->color_pub_->publish(color_msg_);

                //mode 
                if (package.mode & (0x01)){
                    autoaim_state_msg_.autoaim_state = 1; // 操作手进入自瞄模式
                    autoaim_state_pub_->publish(autoaim_state_msg_);
                }
                else
                {
                    autoaim_state_msg_.autoaim_state = 0; // 操作手未进入自瞄模式
                    autoaim_state_pub_->publish(autoaim_state_msg_);
                }
                if (package.mode & (0x02)){
                    mode_msg_.data = (int) base::Mode::NORMAL;
                }
                if (package.mode & (0x04)){
                    mode_msg_.data = (int) base::Mode::RUNE;
                }
                if (package.mode & (0x08)){
                    mode_msg_.data = (int) base::Mode::NORMAL_RUNE;
                }
                ForceSetMode(mode_msg_); 
                if(this->tell_mode)
                    tellMode(mode_msg_);
                this->mode_pub_->publish(mode_msg_);

                

                quaternion_time_msg_.quaternion_stamped.header.stamp = this->now();
                quaternion_time_msg_.quaternion_stamped.quaternion.w = (double)package.q0;
                quaternion_time_msg_.quaternion_stamped.quaternion.x = (double)package.q1;
                quaternion_time_msg_.quaternion_stamped.quaternion.y = (double)package.q2;
                quaternion_time_msg_.quaternion_stamped.quaternion.z = (double)package.q3;  
                memcpy(&quaternion_time_msg_.timestamp_recv, &package.TimeStamp, 4);
                geometry_msgs::msg::TransformStamped t;
                t.header.stamp =  quaternion_time_msg_.quaternion_stamped.header.stamp;
                t.header.frame_id = "world"; //注意坐标系
                t.child_frame_id = "IMU";
                t.transform.rotation.x = quaternion_time_msg_.quaternion_stamped.quaternion.x;
                t.transform.rotation.y = quaternion_time_msg_.quaternion_stamped.quaternion.y;
                t.transform.rotation.z = quaternion_time_msg_.quaternion_stamped.quaternion.z;
                t.transform.rotation.w = quaternion_time_msg_.quaternion_stamped.quaternion.w;
                tf_publisher_->sendTransform(t);
                this->quaternion_pub_->publish(quaternion_time_msg_);
                break;
            }
            
        }
    }

    void UsbCommNode::setUsbPermission()
    {        
        system("echo 'nuc' | sudo -s chmod 777 /dev/bus/usb/001/*");
        system("echo 'nuc' | sudo -s chmod 777 /dev/bus/usb/002/*");
        system("echo 'nuc' | sudo -s chmod 777 /dev/bus/usb/003/*");
        system("echo 'nuc' | sudo -s chmod 777 /dev/bus/usb/004/*");
    }

    void UsbCommNode::ForceSetMode(std_msgs::msg::Int8 &mode_msg)
    {
        switch(this->force_mode)
        {
            case -1: break;
            case 0:
            {
                mode_msg.data = (int)base::Mode::NORMAL;
                break;
            }
            case 1:
            {
                mode_msg.data = (int)base::Mode::NORMAL_RUNE;
                break;
            }
            case 2:
            {
                mode_msg.data = (int) base::Mode::RUNE;
                break;
            }
            default:
            {
                std::cout<<"Wrong force_mode param!"<<std::endl;
            }

        }
    }

    void UsbCommNode::tellMode(std_msgs::msg::Int8 mode_msg)
    {
        if(mode_msg.data == (int) base::Mode::RUNE)
        {
            std::cout<<"RUNE"<<std::endl;
        }
        if(mode_msg.data == (int) base::Mode::NORMAL)
        {
            std::cout<<"NORMAL"<<std::endl;
        }
        if(mode_msg.data == (int) base::Mode::NORMAL_RUNE)
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
