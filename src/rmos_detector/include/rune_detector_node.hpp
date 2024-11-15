//
//Created by Duan on 24-1-20
//

#ifndef RUNE_DETECTOR_NODE_HPP
#define RUNE_DETECTOR_NODE_HPP

// ROS
#include <rclcpp/rclcpp.hpp>
#include <image_transport/image_transport.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include "std_msgs/msg/bool.hpp"
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include <Eigen/Eigen>
#include <tf2_ros/static_transform_broadcaster.h>

// STD
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>

//interfaces
#include "rmos_interfaces/msg/armors.hpp"
#include "rmos_interfaces/msg/armor.hpp"
#include "rmos_interfaces/msg/color.hpp"
#include "rmos_interfaces/msg/mode.hpp"
#include "rmos_interfaces/msg/exp.hpp"


#include "../../Algorithm/include/Dectector/detector_interfaces/detector_interface.hpp"
#include "../../Algorithm/include/Dectector/detector/cj_detector/cj_detector.hpp"
#include "../../Algorithm/include/Dectector/detector/traditional_detector/detector.hpp"
#include "../../Algorithm/include/Dectector/detector/rune_detector/rune_detector.hpp"
#include "../../Algorithm/include/Dectector/classifier/cj_classifier/cj_classifier.hpp"
#include "../../Algorithm/include/Dectector/classifier/onnx_classifier/onnx_classifier.hpp"
#include "../../Algorithm/include/Dectector/Fitting/Fitting.h"
#include "../../Algorithm/include/Dectector/solver/pnp_solver/pnp_solver.hpp"
#include "../../Algorithm/include/Debug/debug.hpp"
#include "../../Algorithm/include/Dectector/detector/mix_detector/mix_detector.hpp"

namespace rune_detector
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
        rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_sub_;//接收相机参数、畸变参数等
        rclcpp::Subscription<rmos_interfaces::msg::Color>::SharedPtr color_sub_;//接收敌人的颜色
        rclcpp::Subscription<rmos_interfaces::msg::Mode>::SharedPtr mode_sub_;//模式，如Rune
        rclcpp::Subscription<rmos_interfaces::msg::Exp>::SharedPtr exp_sub_;//曝光值

    };


    class RuneDetectorNode : public BaseDetectorNode
    {
    public:
        RuneDetectorNode(const rclcpp::NodeOptions &options) : BaseDetectorNode("rune_detector", options)
        {
            RCLCPP_INFO(this->get_logger(), "Starting rune_detector node");

            //subscriber
            this->image_sub_ = std::make_shared<image_transport::Subscriber>(image_transport::create_subscription(
                    this, "/image_raw", std::bind(&RuneDetectorNode::imageCallBack, this, std::placeholders::_1),
                    "raw",
                    rmw_qos_profile_sensor_data));

            this->camera_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>("/daheng_camera_info", rclcpp::SensorDataQoS(),
                                                                                             [this](sensor_msgs::msg::CameraInfo::ConstSharedPtr camera_info_msg)
                                                                                             {
                                                                                                 RCLCPP_INFO(this->get_logger(), "Receive camera infomation!!");

                                                                                                 this->camera_info_msg_ = *camera_info_msg;

                                                                                                 this->camera_matrix_.create(3, 3, CV_64FC1);
                                                                                                 this->dist_coeffs_.create(1, 5, CV_64FC1);

                                                                                                 for (int i = 0; i < 9; i++)
                                                                                                 {
                                                                                                     this->camera_matrix_.at<double>(i / 3, i % 3) = camera_info_msg->k[i];
                                                                                                 }
                                                                                                 for (int i = 0; i < camera_info_msg->d.size(); i++)
                                                                                                 {
                                                                                                     this->dist_coeffs_.at<double>(0, i) = camera_info_msg->d[i];
                                                                                                 }

                                                                                                 this->camera_info_sub_.reset();

                                                                                             });
            tf_buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());
            tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);
            
            this->color_sub_ = this->create_subscription<rmos_interfaces::msg::Color>
                    ("/color_info", rclcpp::SensorDataQoS(), [this](rmos_interfaces::msg::Color::ConstSharedPtr color_msg)
                    {
                        int enemy_color = (*color_msg).color;
                        this->detector_->setEnemyColor(enemy_color);//有两个？(ˇˍˇ)
                        this->rune_detector_->setEnemyColor(enemy_color);
                    });
 
            this->mode_sub_ = this->create_subscription<rmos_interfaces::msg::Mode>
                ("/mode_info", rclcpp::SensorDataQoS(), [this](rmos_interfaces::msg::Mode::ConstSharedPtr mode_msg)
                {
                    //RCLCPP_INFO(this->get_logger(), "mode is %d", (*mode_msg).mode);
                    int mode = (*mode_msg).mode;
                    setMode(mode);
                    
                });
            this->exp_sub_ = this->create_subscription<rmos_interfaces::msg::Exp>
                ("/exp_info", rclcpp::SensorDataQoS(), [this](rmos_interfaces::msg::Exp::ConstSharedPtr exp_msg)
                {
                    
                    //RCLCPP_INFO(this->get_logger(), "exp is %d", (*exp_msg).exp);
                    this->Exposure = (*exp_msg).exp;
                });

            // publisher
            this->armors_pub_ = this->create_publisher<rmos_interfaces::msg::Armors>("/rmos_detector/armors", rclcpp::SensorDataQoS());

            //debug info publisher
            debug_img_pub_ = image_transport::create_camera_publisher(this, "/debug_image", rmw_qos_profile_default);
            debug_bin_img_pub_ = image_transport::create_camera_publisher(this, "/debug_bin_image", rmw_qos_profile_default);


            //cj_detector_ = std::make_shared<detector::CjDetector>();
            detector_ = std::make_shared<detector::Detector>();
            rune_detector_ = std::make_shared<RuneDetector::DlRuneDetector>();
            fitting_ = std::make_shared<RuneDetector::Fitting>();
            //cj_classifier_ = std::make_shared<detector::CjClassifier>();
            pnp_solver_ = std::make_shared<detector::PnpSolver>();
            onnx_classifier_ =  std::make_shared<detector::OnnxClassifier>();
            // mix_detector_ = std::make_shared<MixDetect::ArmorDetector>();

            /*publish static TF*/
            this->tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        }

    protected:
        void imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg);

        /*mode*/
        void setMode(int mode);
        base::Mode mode_ = base::Mode::NORMAL;
        base::Mode last_mode_ = base::Mode::NORMAL;        

        /*save image*/
        void saveImage(double timestamp, cv::Mat image);
        std::string path = "./rmos_bringup/image_save/";
        std::string image_folder_path;
        int image_num = 0;
        bool have_mkdir = false;
        double last_save_timestamp = 0;
        double save_delta_time;     // 保存图片的时间间隔     

        void saveDrawImage(double timestamp, cv::Mat image);
        std::string path_draw = "./rmos_bringup/draw_image_save/";
        std::string draw_image_folder_path;
        int draw_image_num = 0;
        bool have_mkdir_draw = false;
        int Exposure = 0;           

        /**
         * @brief 计算文件夹内文件数量
        */        
        int countFilesInDirectory(std::string path); 

        //std::shared_ptr<detector::CjDetector> cj_detector_;
        std::shared_ptr<detector::Detector> detector_;
        std::shared_ptr<RuneDetector::DlRuneDetector> rune_detector_;

        std::shared_ptr<RuneDetector::Fitting> fitting_;

        // std::shared_ptr<detector::CjClassifier> cj_classifier_;
        std::shared_ptr<detector::OnnxClassifier> onnx_classifier_;

        // std::shared_ptr<MixDetect::ArmorDetector> mix_detector_;

        std::shared_ptr<detector::PnpSolver> pnp_solver_;

        /* Publisher */
        rclcpp::Publisher<rmos_interfaces::msg::Armors>::SharedPtr armors_pub_;
        std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher_;


        /*debug*/
        image_transport::CameraPublisher debug_img_pub_;
        image_transport::CameraPublisher debug_bin_img_pub_;
        image_transport::CameraPublisher debug_R_img_pub_;
        sensor_msgs::msg::Image::SharedPtr debug_image_msg_;
        sensor_msgs::msg::Image::SharedPtr debug_bin_image_msg_;      
        sensor_msgs::msg::Image::SharedPtr debug_R_image_msg_;           

        //camera param
        sensor_msgs::msg::CameraInfo camera_info_msg_;

        std::shared_ptr<tf2_ros::TransformListener> tf_listener{nullptr};
        std::unique_ptr<tf2_ros::Buffer> tf_buffer;
    };
}

#endif //RUNE_DETECTOR_NODE_HPP
