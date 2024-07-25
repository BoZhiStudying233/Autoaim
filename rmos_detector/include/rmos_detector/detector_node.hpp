//
// Created by Wang on 23-6-16.
//

#ifndef RMOS_DETECTOR_NODE_HPP
#define RMOS_DETECTOR_NODE_HPP

// ROS
#include <rclcpp/rclcpp.hpp>
#include <image_transport/image_transport.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int8.hpp"
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include <Eigen/Eigen>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

// STD
#include <memory>
#include <string>
#include <vector>

//interfaces
#include "rmos_interfaces/msg/armors.hpp"
#include "rmos_interfaces/msg/armor.hpp"
#include "rmos_interfaces/msg/color.hpp"

#include "detector.hpp"
#include "onnx_classifier.hpp"
#include "rmos_utils/pnp_solver.hpp"

namespace rmos_detector
{
    class BaseDetectorNode : public rclcpp::Node
    {
    public:
        BaseDetectorNode(const std::string &node_name,
                         const rclcpp::NodeOptions &options) : Node(node_name, options)
        {
            RCLCPP_INFO(this->get_logger(), "Starting node [%s]", node_name.c_str());
        }

        cv::Mat camera_matrix_;
        cv::Mat dist_coeffs_;

        std::shared_ptr<image_transport::Subscriber> image_sub_;
        rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_sub_;
        rclcpp::Subscription<rmos_interfaces::msg::Color>::SharedPtr color_sub_;
        rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr mode_sub_;
        base::Mode mode_ = base::Mode::NORMAL;
    };

    class BasicDetectorNode : public virtual BaseDetectorNode
    {
    public:
        BasicDetectorNode(const rclcpp::NodeOptions &options);

    protected:
        void imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg);

        std::unique_ptr<detector::Detector> initDetector();

        std::unique_ptr<detector::Detector> detector_;
        std::shared_ptr<detector::OnnxClassifier> onnx_classifier_;
        std::shared_ptr<tool::PnpSolver> pnp_solver_;

        void drawArmor(cv::Mat image, base::Armor armor);

        /* Publisher */
        rclcpp::Publisher<rmos_interfaces::msg::Armors>::SharedPtr armors_pub_;
        std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher_;

        /*debug*/
        image_transport::CameraPublisher debug_img_pub_;
        image_transport::CameraPublisher debug_bin_img_pub_;
        sensor_msgs::msg::Image::SharedPtr debug_image_msg_;
        sensor_msgs::msg::Image::SharedPtr debug_bin_image_msg_;      

        //camera param
        sensor_msgs::msg::CameraInfo camera_info_msg_;

        bool debug;
    };
}


#endif //RMOS_DETECTOR_NODE_HPP
