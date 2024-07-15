//ROS
#include <image_transport/image_transport.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2/LinearMath/Matrix3x3.h>
#include "std_msgs/msg/bool.hpp"

//STD
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cv_bridge/cv_bridge.h>

//OpenCV
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "rmos_detector/detector_node.hpp"

namespace rmos_detector
{
    BasicDetectorNode::BasicDetectorNode(const rclcpp::NodeOptions &options) : BaseDetectorNode("basic_detector", options)
    {
        RCLCPP_INFO(this->get_logger(), "Starthing basic_detector node");

        //subscriber
        this->image_sub_ = std::make_shared<image_transport::Subscriber>(image_transport::create_subscription(
                this, "/image_raw", std::bind(&BasicDetectorNode::imageCallBack, this, std::placeholders::_1),
                "raw", rmw_qos_profile_default));
        this->camera_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>("/camera_info", rclcpp::SensorDataQoS(),
                                                                                            [this](sensor_msgs::msg::CameraInfo::ConstSharedPtr camera_info_msg)
                                                                                            {
                                                                                                RCLCPP_INFO(this->get_logger(), "Receive camera infomation");

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

        this->color_sub_ = this->create_subscription<rmos_interfaces::msg::Color>
                ("/color_info", rclcpp::SensorDataQoS(), [this](rmos_interfaces::msg::Color::ConstSharedPtr color_msg)
                {
                    int enemy_color = (*color_msg).color;
                    this->detector_->setEnemyColor(enemy_color);
                });

        this->mode_sub_ = this->create_subscription<std_msgs::msg::Int8>
                ("/mode_info", rclcpp::SensorDataQoS(), [this](std_msgs::msg::Int8::ConstSharedPtr mode_msg)
                {
                    //RCLCPP_INFO(this->get_logger(), "mode is %d", (*mode_msg).mode);
                    this->mode_ = (*mode_msg).data==0 ? base::Mode::NORMAL : base::Mode::WAIT;
                });
        // publisher
        this->armors_pub_ = this->create_publisher<rmos_interfaces::msg::Armors>("/rmos_detector/armors", rclcpp::SensorDataQoS());

        //debug info publisher
        debug_img_pub_ = image_transport::create_camera_publisher(this, "/debug_image", rmw_qos_profile_default);
        debug_bin_img_pub_ = image_transport::create_camera_publisher(this, "/debug_bin_image", rmw_qos_profile_default);

        this->detector_ = initDetector();
        this->onnx_classifier_ = std::make_shared<detector::OnnxClassifier>();
        this->pnp_solver_ = std::make_shared<detector::PnpSolver>();

        /*publish static TF*/
        this->tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

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
        t.header.frame_id = "IMU";
        t.child_frame_id = "camera";
        t.transform.rotation.x = tf2_cam2IMU_quaternion.x();
        t.transform.rotation.y = tf2_cam2IMU_quaternion.y();
        t.transform.rotation.z = tf2_cam2IMU_quaternion.z();
        t.transform.rotation.w = tf2_cam2IMU_quaternion.w();

        t.transform.translation.x = this->declare_parameter("camera2imu_offset_x", 0.0);
        t.transform.translation.y = this->declare_parameter("camera2imu_offset_y", 0.0);
        t.transform.translation.z = this->declare_parameter("camera2imu_offset_z", 0.0);
        this->tf_publisher_->sendTransform(t);

        this->debug = this->declare_parameter("debug", 0);
    }

    void BasicDetectorNode::imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg)
    {
        if(this->mode_ != base::Mode::NORMAL)
        {
            return;
        }

        rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
        auto time1 = steady_clock_.now();
        auto image = cv_bridge::toCvShare(image_msg, "bgr8")->image;
        std::vector<base::Armor> armors;
        detector_->detectArmors(image,armors);
        onnx_classifier_->classifyArmors(image,armors);

        rmos_interfaces::msg::Armors armors_msg;
        rmos_interfaces::msg::Armor armor_msg;
        armors_msg.header = image_msg->header;

        for(auto &armor : armors)
        {
            //pnp solve
            cv::Mat tVec;
            cv::Mat rVec;
            bool is_solve;
            is_solve = this->pnp_solver_->solveArmorPose(armor,this->camera_matrix_,this->dist_coeffs_,tVec,rVec);
            if(!is_solve)
            {
                RCLCPP_WARN(this->get_logger(), "camera param empty");
                continue;
            }
            armor_msg.pose.position.x = tVec.at<double>(0, 0)/1000;
            armor_msg.pose.position.y = tVec.at<double>(1, 0)/1000;
            armor_msg.pose.position.z = tVec.at<double>(2, 0)/1000;
            armor.position = cv::Point3f(armor_msg.pose.position.x, armor_msg.pose.position.y, armor_msg.pose.position.z);
            // rvec to 3x3 rotation matrix
            cv::Mat rotation_matrix;
            cv::Rodrigues(rVec, rotation_matrix);
            // rotation matrix to quaternion
            tf2::Matrix3x3 tf2_rotation_matrix(
                    rotation_matrix.at<double>(0, 0), rotation_matrix.at<double>(0, 1),
                    rotation_matrix.at<double>(0, 2), rotation_matrix.at<double>(1, 0),
                    rotation_matrix.at<double>(1, 1), rotation_matrix.at<double>(1, 2),
                    rotation_matrix.at<double>(2, 0), rotation_matrix.at<double>(2, 1),
                    rotation_matrix.at<double>(2, 2));
            tf2::Quaternion tf2_quaternion;
            tf2_rotation_matrix.getRotation(tf2_quaternion);
            armor_msg.pose.orientation.x = tf2_quaternion.x();
            armor_msg.pose.orientation.y = tf2_quaternion.y();
            armor_msg.pose.orientation.z = tf2_quaternion.z();
            armor_msg.pose.orientation.w = tf2_quaternion.w();
            cv::Point2f center(image.rows/2,image.cols/2);
            armor_msg.distance_to_image_center = sqrt((center.x-armor.rrect.center.x)*(center.x-armor.rrect.center.x)+
                                                      (center.y-armor.rrect.center.y)*(center.y-armor.rrect.center.y));
            armor_msg.num_id = armor.num_id;
            armor_msg.distance = sqrt(armor_msg.pose.position.x*armor_msg.pose.position.x+
                                           armor_msg.pose.position.y*armor_msg.pose.position.y+
                                           armor_msg.pose.position.z*armor_msg.pose.position.z);

            this->drawArmor(image, armor);

            tf2::Quaternion tf_armor_q;
            tf2::fromMsg(armor_msg.pose.orientation, tf_armor_q);
            // double armor_roll, armor_pitch, armor_yaw;
            tf2::Matrix3x3(tf_armor_q).getRPY(armor.pyr.z, armor.pyr.x, armor.pyr.y);
            armor.pyr = armor.pyr*180.0 / 3.1415926535;
            
            armors_msg.armors.push_back(armor_msg);
        }
        auto time2 = steady_clock_.now();

        if(this->debug)RCLCPP_INFO(this->get_logger(), "Cost %.4f ms", (time2-time1).seconds() * 1000);

        if(this->debug)
        {
            debug_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
            debug_img_pub_.publish(*debug_image_msg_,camera_info_msg_);
        }
        if(this->debug)
        {
            debug_bin_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", this->detector_->debug_binary_).toImageMsg();
            debug_bin_img_pub_.publish(*debug_bin_image_msg_,camera_info_msg_);
        }

        armors_pub_->publish(armors_msg);
    }

    std::unique_ptr<detector::Detector> BasicDetectorNode::initDetector()
    {
        detector::ProcessParams p_params = {
            .bin_threshold = this->declare_parameter("image_process_params.bin_threshold", 90.0),
            .enemy_color = this->declare_parameter("image_process_params.enemy_color", 1)
        };

        detector::LightParams l_params = {
            .angle_to_vertigal_max = this->declare_parameter("light_params.angle_to_vertical_max", 35.0),
            .height_width_min_ratio = this->declare_parameter("light_params.height_width_min_ratio", 1.3),
            .size_area_min_ratio = this->declare_parameter("light_params.size_area_min_ratio", 0.4)
        };

        detector::ArmorParams a_params = {
            .lights_angle_max_diff = this->declare_parameter("armor_params.lights_angle_max_diff", 10.0),
            .lights_length_max_ratio = this->declare_parameter("armor_params.lights_length_max_ratio", 1.5),
            .lights_Y_max_ratio = this->declare_parameter("armor_params.lights_Y_max_ratio", 1.5),
            .width_height_min_ratio = this->declare_parameter("armor_params.width_height_min_ratio", 1.1),
            .width_height_max_ratio = this->declare_parameter("armor_params.width_height_max_ratio", 6.5),
            .max_angle = this->declare_parameter("armor_params.max_angle", 35.0),
            .inside_thresh = this->declare_parameter("armor_params.inside_thresh", 85.0)
        };

        auto detector = std::make_unique<detector::Detector>(p_params, l_params, a_params);

        return detector;
    }

    void BasicDetectorNode::drawArmor(cv::Mat image, base::Armor armor)
    {
        std::string text1 = std::to_string(armor.num_id);
        std::string text2 = std::to_string(int(armor.confidence*100));
        cv::putText(image, text1, armor.left.up, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(0, 255, 0), 1.8);
        cv::putText(image, text2, armor.right.up, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(0, 255, 0), 0.5);
        cv::line(image,armor.left.up,armor.left.down ,cv::Scalar(255, 0, 255),1);
        cv::line(image,armor.left.down,armor.right.down ,cv::Scalar(255, 0, 255),1);
        cv::line(image,armor.right.down,armor.right.up ,cv::Scalar(255, 0, 255),1);
        cv::line(image,armor.right.up,armor.left.up ,cv::Scalar(255, 0, 255),1);
        float distance = sqrt(pow(armor.position.x,2)+pow(armor.position.y,2)+pow(armor.position.z,2));
        std::string text3 = std::to_string(distance);
        cv::putText(image, text3, armor.right.down, cv::FONT_HERSHEY_SIMPLEX, 1,
                    cv::Scalar(0, 255, 0), 1);
    }
}


#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_detector::BasicDetectorNode)
