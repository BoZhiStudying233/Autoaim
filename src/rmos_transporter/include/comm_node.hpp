//
// Created by Wang on 23-7-9.
//

#ifndef RMOS_COMM_NODE_HPP
#define RMOS_COMM_NODE_HPP


#include <memory>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/quaternion_stamped.hpp>

#include "rmos_interfaces/msg/target.hpp"
#include "rmos_interfaces/msg/quaternion_time.hpp"
#include "rmos_interfaces/msg/color.hpp"
#include "rmos_interfaces/msg/bullet_speed.hpp"
#include "rmos_interfaces/msg/mode.hpp"
#include "rmos_interfaces/msg/autoaim_state.hpp"


#include "../../Algorithm/include/Transporter/can/can.hpp"
#include "../../Algorithm/include/Transporter/usb/usb.hpp"
#include "../../Algorithm/include/Debug/debug.hpp"


namespace rmos_transporter
{
    class CommNode : public rclcpp::Node{
    public:
        CommNode(const std::string &node_name, const rclcpp::NodeOptions &options) : Node(node_name, options)
        {
            RCLCPP_INFO(this->get_logger(), "Start Communicate Node: %s", node_name.c_str());

            this->quaternion_pub_ = this->create_publisher<rmos_interfaces::msg::QuaternionTime>("/imu_quaternion", rclcpp::SensorDataQoS());

            this->color_pub_ = this->create_publisher<rmos_interfaces::msg::Color>("/color_info", rclcpp::SensorDataQoS());
            this->bs_pub_ = this->create_publisher<rmos_interfaces::msg::BulletSpeed>("/bs_info", rclcpp::SensorDataQoS());
            this->mode_pub_ = this->create_publisher<rmos_interfaces::msg::Mode>("/mode_info", rclcpp::SensorDataQoS());
            this->autoaim_state_pub_ = this->create_publisher<rmos_interfaces::msg::AutoaimState>("/autoaim_state", rclcpp::SensorDataQoS());
        }

    protected:
        rclcpp::Subscription<rmos_interfaces::msg::Target>::SharedPtr target_sub_;

        rclcpp::Publisher<rmos_interfaces::msg::QuaternionTime>::SharedPtr quaternion_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::Color>::SharedPtr color_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::BulletSpeed>::SharedPtr bs_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::Mode>::SharedPtr mode_pub_;
        rclcpp::Publisher<rmos_interfaces::msg::AutoaimState>::SharedPtr autoaim_state_pub_;
        std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_;

        std::thread recevie_thread_;

    };

    class CanCommNode : public CommNode{
    public:
        CanCommNode(const rclcpp::NodeOptions & options);
        ~CanCommNode();
    protected:
        /**
         *  @brief  target_sub_的回调函数，将msg转换后通过can发送
         */
        void targetCallBack(const rmos_interfaces::msg::Target::SharedPtr target);

        /**
         *  @brief  将msg中的pitch、yaw、imu_time，转换为字节形式
         */
        void target2data(const rmos_interfaces::msg::Target::SharedPtr target, u_char *buf);

        /**
         *  @brief  将msg中的自瞄状态信息，转换为字节形式
         */
        void target2state(const rmos_interfaces::msg::Target::SharedPtr target, u_char *buf);

        /**
         *  @brief  将收到的四元数字节数据转换为double类型
         *  @param  buf 收到的字节数组，数组容量为8
         *  @param  quaternion  转换得到的四元数，数组容量为4
         */
        void transfer2Quaternion(u_char *buf, double *quaterion);

        /**
         *  @brief  发送自瞄角度的回调函数
         */
        void sendDataCallBack();

        /**
         *  @brief  发送自瞄状态的回调函数
         */
        void sendStateCallBack();

        /**
         *  @brief  读取can数据的线程函数
         */
        void recevieCallBack();

        void ForceSetMode(rmos_interfaces::msg::Mode &mode_msg);
        void tellMode(rmos_interfaces::msg::Mode mode_msg);

        transporter::Can can_;   // can通信接口

        /* Buffer */
        u_char data_buf_[8];    // pitch、yaw、imu_time
        u_char state_buf_[3];   // 自瞄状态

        /* Send Timer */
        rclcpp::TimerBase::SharedPtr send_data_timer_;      // 发送 data_buf_ 的定时器
        rclcpp::TimerBase::SharedPtr send_state_timer_;     // 发送 state_buf_ 的定时器

        /* Callback Group */
        rclcpp::CallbackGroup::SharedPtr send_callback_group_;
        rclcpp::CallbackGroup::SharedPtr target_sub_callback_group_;

        /*time*/
        double last_time_;

    };

    class UsbCommNode : public CommNode{
    public:
        UsbCommNode(const rclcpp::NodeOptions & options);
        ~UsbCommNode();
    protected:
        /**
         *  @brief  target_sub_的回调函数，将msg转换后通过can发送
         */
        void targetCallBack(const rmos_interfaces::msg::Target::SharedPtr target);

        /**
         *  @brief  将msg中的自瞄状态信息，转换为字节形式
         */
        void target2state(const rmos_interfaces::msg::Target::SharedPtr target, u_char *buf);

        /**
         *  @brief  读取usb数据的线程函数
         */
        void recevieCallBack();

        void ForceSetMode(rmos_interfaces::msg::Mode &mode_msg);
        void tellMode(rmos_interfaces::msg::Mode mode_msg);

        std::shared_ptr<transporter_sdk::TransporterInterface> transporter_; // usb通信接口

        /* package */
        transporter::RMOSSendPackage send_package_;

        /* Callback Group */
        rclcpp::CallbackGroup::SharedPtr receive_callback_group_;
        rclcpp::CallbackGroup::SharedPtr target_sub_callback_group_;

        /*time*/
        double last_time_;

        /* Send Timer */
        rclcpp::TimerBase::SharedPtr receive_timer_;
        
        // msg
        rmos_interfaces::msg::QuaternionTime quaternion_time_msg_;
        rmos_interfaces::msg::Color color_msg_;
        rmos_interfaces::msg::Mode mode_msg_;
        rmos_interfaces::msg::BulletSpeed bs_msg_;
        rmos_interfaces::msg::AutoaimState autoaim_state_msg_;

        // params
        int interface_usb_vid_;
        int interface_usb_pid_;
        int interface_usb_read_endpoint_;
        int interface_usb_write_endpoint_;
        int interface_usb_read_timeout_;
        int interface_usb_write_timeout_;

    };

}


#endif //RMOS_COMM_NODE_HPP
