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
#include "std_msgs/msg/int8.hpp"
#include "rmos_interfaces/msg/exp.hpp"


// #include "../../Algorithm/include/Dectector/detector_interfaces/detector_interface.hpp"
// #include "../../Algorithm/include/Dectector/detector/cj_detector/cj_detector.hpp"
// #include "../../Algorithm/include/Dectector/detector/traditional_detector/detector.hpp"
#include "rune_detector.hpp"
// #include "../../Algorithm/include/Dectector/classifier/cj_classifier/cj_classifier.hpp"
// #include "../../Algorithm/include/Dectector/classifier/onnx_classifier/onnx_classifier.hpp"
#include "rmos_rune/Fitting.h"
#include "rmos_utils/pnp_solver.hpp"
// #include "../../Algorithm/include/Debug/debug.hpp"
// #include "../../Algorithm/include/Dectector/detector/mix_detector/mix_detector.hpp"

namespace rmos_rune
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
        rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr mode_sub_;//模式，如Rune
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
                        this->rune_detector_->setEnemyColor(enemy_color);
                    });
 
            this->mode_sub_ = this->create_subscription<std_msgs::msg::Int8>
                ("/mode_info", rclcpp::SensorDataQoS(), [this](std_msgs::msg::Int8::ConstSharedPtr mode_msg)
                {
                    //RCLCPP_INFO(this->get_logger(), "mode is %d", (*mode_msg).mode);
                    int mode = (*mode_msg).data;
                    setMode(mode);
                });
            this->exp_sub_ = this->create_subscription<rmos_interfaces::msg::Exp>
                ("/exp_info", rclcpp::SensorDataQoS(), [this](rmos_interfaces::msg::Exp::ConstSharedPtr exp_msg)
                {
                    
                    //RCLCPP_INFO(this->get_logger(), "exp is %d", (*exp_msg).exp);
                    this->Exposure = (*exp_msg).exp;
                });

            
            // publisher
            this->armors_pub_ = this->create_publisher<rmos_interfaces::msg::Armors>("/rune_detector/armors", rclcpp::SensorDataQoS());

            //debug info publisher
            debug_img_pub_ = image_transport::create_camera_publisher(this, "/debug_image", rmw_qos_profile_default);
            debug_bin_img_pub_ = image_transport::create_camera_publisher(this, "/debug_bin_image", rmw_qos_profile_default);


            //cj_detector_ = std::make_shared<detector::CjDetector>();
            // detector_ = std::make_shared<detector::Detector>();            
            fitting_ = std::make_shared<RuneDetector::Fitting>();
            //cj_classifier_ = std::make_shared<detector::CjClassifier>();
            pnp_solver_ = std::make_shared<detector::PnpSolver>();
            // onnx_classifier_ =  std::make_shared<detector::OnnxClassifier>();
            // mix_detector_ = std::make_shared<MixDetect::ArmorDetector>();

            /*publish static TF*/
            this->tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
            this->rune_detector_ = initDetector();
            


            //发布相机到陀螺仪的静态tf
            cv::Mat cam2IMU_matrix;
            cam2IMU_matrix = (cv::Mat_<double>(3, 3) <<0,0,1,-1,0,0,0,-1,0);
            tf2::Matrix3x3 tf2_cam2IMU_matrix(
                    cam2IMU_matrix.at<double>(0, 0), cam2IMU_matrix.at<double>(0, 1),
                    cam2IMU_matrix.at<double>(0, 2), cam2IMU_matrix.at<double>(1, 0),
                    cam2IMU_matrix.at<double>(1, 1), cam2IMU_matrix.at<double>(1, 2),
                    cam2IMU_matrix.at<double>(2, 0), cam2IMU_matrix.at<double>(2, 1),
                    cam2IMU_matrix.at<double>(2, 2));
            
            tf2::Quaternion tf2_cam2IMU_quaternion;
            tf2_cam2IMU_matrix.getRotation(tf2_cam2IMU_quaternion);

            geometry_msgs::msg::TransformStamped t;
            // t.header.stamp =  image_msg->header.stamp;
            // t.header.stamp = this->now();
            t.header.frame_id = "IMU";
            t.child_frame_id = "camera";
            t.transform.rotation.x = tf2_cam2IMU_quaternion.x();
            t.transform.rotation.y = tf2_cam2IMU_quaternion.y();
            t.transform.rotation.z = tf2_cam2IMU_quaternion.z();
            t.transform.rotation.w = tf2_cam2IMU_quaternion.w();
            //相机到IMU存在位置的偏移，每辆车不同，请在参数文件自行更改
            t.transform.translation.x = this->declare_parameter("camera2imu_offset_x", 0.0);
            t.transform.translation.y = this->declare_parameter("camera2imu_offset_y", 0.0);
            t.transform.translation.z = this->declare_parameter("camera2imu_offset_z", 0.0);
            this->tf_publisher_->sendTransform(t) ;
        }

    protected:
        void imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg);

        /*mode*/
        void setMode(int mode);
        base::Mode mode_ = base::Mode::NORMAL;
        base::Mode last_mode_ = base::Mode::NORMAL;        

        /*save image*/
        void saveImage(double timestamp, cv::Mat image);
        std::string path = "./rmos_rune/drawRune/image_save/";
        std::string image_folder_path;
        int image_num = 0;
        bool have_mkdir = false;
        double last_save_timestamp = 0;
        double save_delta_time = 0.3;     // 保存图片的时间间隔     

        void saveDrawImage(double timestamp, cv::Mat image);
        std::string path_draw = "./rmos_rune/drawRune/draw_image_save/";
        std::string draw_image_folder_path;
        int draw_image_num = 0;
        bool have_mkdir_draw = false;
        int Exposure = 0;           



        bool save_image;
        bool save_draw_image;
        bool tell_cost_time;


        /**
         * @brief 计算文件夹内文件数量
        */        
        int countFilesInDirectory(std::string path); 

        //std::shared_ptr<detector::CjDetector> cj_detector_;
        // std::shared_ptr<detector::Detector> detector_;
        std::unique_ptr<RuneDetector::DlRuneDetector> initDetector();
        std::shared_ptr<RuneDetector::DlRuneDetector> rune_detector_;

        std::shared_ptr<RuneDetector::Fitting> fitting_;

        // std::shared_ptr<detector::CjClassifier> cj_classifier_;
        // std::shared_ptr<detector::OnnxClassifier> onnx_classifier_;

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
