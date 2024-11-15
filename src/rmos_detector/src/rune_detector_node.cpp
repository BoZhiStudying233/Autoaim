//
//Created by Duan on 24-1-20
//

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

#include "../include/rune_detector_node.hpp"


namespace rune_detector
{
    void RuneDetectorNode::imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg)
    {

        if(this->mode_ == base::Mode::RUNE||this->mode_ == base::Mode::NORMAL_RUNE)
        {
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
            t.header.stamp =  image_msg->header.stamp;
            t.header.frame_id = "IMU";
            t.child_frame_id = "camera";
            t.transform.rotation.x = tf2_cam2IMU_quaternion.x();
            t.transform.rotation.y = tf2_cam2IMU_quaternion.y();
            t.transform.rotation.z = tf2_cam2IMU_quaternion.z();
            t.transform.rotation.w = tf2_cam2IMU_quaternion.w();
            
            //相机到IMU存在位置的偏移，每辆车不同，请在参数文件自行更改
            t.transform.translation.x = 0.005;
            t.transform.translation.y = 0;
            t.transform.translation.z = 0;
            this->tf_publisher_->sendTransform(t) ;



            rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
            double timestamp = t.header.stamp.sec + t.header.stamp.nanosec*1e-9;
            auto time1 = steady_clock_.now();
            cv::Mat image = cv_bridge::toCvShare(image_msg, "bgr8")->image;
            if(debug::get_debug_option(base::SAVE_IMAGE))
            {
                saveImage(timestamp, image);
            }
            if(debug::get_debug_option(base::SAVE_DRAW_IMAGE))
            {
                saveDrawImage(timestamp, image);
            }
            rmos_interfaces::msg::Armors armors_msg;
            rmos_interfaces::msg::Armor armor_msg;
            armors_msg.header = image_msg->header;
            if(this->mode_ == base::Mode::RUNE || this->mode_ == base::Mode::NORMAL_RUNE)
            {
                base::RuneArmor target_rune_armor;
                std::vector<base::RuneArmor> rune_armors;
            
                this->rune_detector_->DlRuneDetect(image,target_rune_armor, rune_armors);

                armor_msg.num_id = rune_detector_->id;
                std::string text = "id:" + std::to_string(rune_detector_->id);
                cv::putText(image, text, (cv::Point2i(0, 20), cv::Point2i(20, 20)), cv::FONT_HERSHEY_SIMPLEX, 1,
                            cv::Scalar(0, 255, 0), 0.5);

                target_rune_armor.timestamp = timestamp;
                // std::cout << "timestamp:" << timestamp<< std::endl;
                Eigen::Vector3d tVec;
                std::vector<cv::Point2f> rune_next_pos;
                geometry_msgs::msg::TransformStamped transform_to_world, transform_to_camera;
                try
                    {//实车记得改坐标系名称！ remeber to change
                        transform_to_world = tf_buffer->lookupTransform("world", "camera", tf2::TimePointZero);
                        transform_to_camera = tf_buffer->lookupTransform("camera", "world", tf2::TimePointZero);
                    }
                catch (tf2::TransformException & ex)
                    {
                        RCLCPP_ERROR(this->get_logger(), "Could not get the transformation!!");
                        }
                        
                if(fitting_->run(target_rune_armor, rune_next_pos, tVec, rune_detector_->state, this->mode_, rune_armors, this->camera_matrix_, this->dist_coeffs_, transform_to_world, transform_to_camera))
                {
                    if(rune_next_pos.size() == 4)//二维圆的情况
                    {
                        // 若预测后的点在图片上才可画图，否则程序会异常终止
                        bool can_draw = true;
                        for(int i = 0; i < 4; i++)
                        {
                            
                            if(rune_next_pos[i].x < 0 || rune_next_pos[i].y < 0 || rune_next_pos[i].x > image.cols || rune_next_pos[i].y > image.rows)
                                can_draw = false;
                        }
                        
                        if(can_draw)
                        {
                            for(int i = 0; i < 4; i++)
                            {
                                cv::circle(image, rune_next_pos[i], 6, cv::Scalar(255,0,255), -1);
                                cv::line(image, rune_next_pos[i], rune_next_pos[(i+1)%4], cv::Scalar(50, 100, 50));
                            }
                        }
                        if(debug::get_debug_option(base::SHOW_ARMOR))
                        {
                            debug_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
                            debug_img_pub_.publish(*debug_image_msg_,camera_info_msg_);
                            cv::imshow("image", image);
                            cv::waitKey(1);
                            //std::cout<<"debug_image_running!!"<<std::endl;
                        }
                        //pnp solve
                        cv::Mat tvec;
                        cv::Mat rvec;
                        bool is_solve;
                        is_solve = this->pnp_solver_->solveRuneArmorPose(rune_next_pos,this->camera_matrix_,this->dist_coeffs_,tvec,rvec);
                        
                        if(!is_solve)
                        {
                            RCLCPP_WARN(this->get_logger(), "camera param empty");
                        }
                        
                        tVec.x() = tvec.at<double>(0, 0);
                        tVec.y() = tvec.at<double>(1, 0);
                        tVec.z() = tvec.at<double>(2, 0);
                        // std::cout<<"tVector_in_camera="<<tVec<<std::endl;
                    }
                    else//三维圆的情况
                    {
                        // cv::Point2d point2D;
                        std::vector<cv::Point3f> objectPoints;
                        std::vector<cv::Point2f> imagePoints;

                        // tVec.x() = tVec.x()/tVec.z();
                        // tVec.y() = tVec.y()/tVec.z();
                        // tVec.z() = 1;

                        objectPoints.emplace_back(tVec.x()/tVec.z(), tVec.y()/tVec.z(), 1);

                        cv::Mat tvec = (cv::Mat_<float>(1, 3) << 0, 0, 0);
                        cv::projectPoints(objectPoints, cv::Mat::zeros(1, 3, CV_64F), tvec, this->camera_matrix_, this->dist_coeffs_, imagePoints);
                        cv::Point2d point2D = imagePoints[0];
                        if (point2D.x > 0 && point2D.y > 0 && point2D.x < image.cols && point2D.y < image.rows)
                            cv::circle(image, point2D, 6, cv::Scalar(255,0,255), -1);
                        // std::cout<<"233"<<std::endl;
                    }
                    armor_msg.pose.position.x = tVec.x()/1000;
                    armor_msg.pose.position.y = tVec.y()/1000;
                    armor_msg.pose.position.z = tVec.z()/1000;//相机坐标系下，目标符中心的坐标
                    // std::cout<<"armor_msg.pose.position.x:"<<armor_msg.pose.position.x<<" armor_msg.pose.position.y:"<<armor_msg.pose.position.y<<" armor_msg.pose.position.z:"<<armor_msg.pose.position.z<<std::endl;
                    Eigen::Vector3d CamaraVector = Eigen::Vector3d(0,0,1);
                    cv::Mat rVec = cv::Mat::zeros(3, 1, CV_64F);

                    //tVec = Eigen::Vector3d(tVec.x()/tVec.norm(),tVec.y()/tVec.norm(),tVec.z()/tVec.norm());//获取单位向量
                    //cv::eigen2cv(CamaraVector.cross(tVec), rVec);

                    //double VectorTheta = acos(CamaraVector.dot(tVec)/(CamaraVector.norm()*tVec.norm()));
                    
                    // rVec = rVec/rVec.norm() * VectorTheta;
                    
                        // rvec to 3x3 rotation matrix
                        cv::Mat rotation_matrix;
                    
                    rVec.at<double>(0) = 0.1;//胡乱赋一个值

                    cv::Rodrigues(rVec, rotation_matrix);
                    // rotation matrix to quaternion
                    tf2::Matrix3x3 tf2_rotation_matrix(
                            rotation_matrix.at<double>(0, 0), rotation_matrix.at<double>(0, 1),
                            rotation_matrix.at<double>(0, 2), rotation_matrix.at<double>(1, 0),
                            rotation_matrix.at<double>(1, 1), rotation_matrix.at<double>(1, 2),
                            rotation_matrix.at<double>(2, 0), rotation_matrix.at<double>(2, 1),
                        rotation_matrix.at<double>(2, 2));//旋转矩阵
                    tf2::Quaternion tf2_quaternion;
                    tf2_rotation_matrix.getRotation(tf2_quaternion);
                    armor_msg.pose.orientation.x = tf2_quaternion.x();
                    armor_msg.pose.orientation.y = tf2_quaternion.y();
                    armor_msg.pose.orientation.z = tf2_quaternion.z();
                    armor_msg.pose.orientation.giw = tf2_quaternion.w();//四元数

                    armors_msg.armors.push_back(armor_msg);
                }
                armors_msg.is_rune = true;
            }
            auto time2 = steady_clock_.now();

            std::string text = "Exposure:" + std::to_string(this->Exposure);
            cv::putText(image, text, (cv::Point2i(0, 50), cv::Point2i(20, 50)), cv::FONT_HERSHEY_SIMPLEX, 1,
                        cv::Scalar(0, 255, 0), 0.5);
            if(debug::get_debug_option(base::SAVE_DRAW_IMAGE))
            {
                saveDrawImage(timestamp, image);
            }

            if(debug::get_debug_option(base::SHOW_DETECT_COST))
                RCLCPP_INFO(this->get_logger(), "Cost %.4f ms", (time2-time1).seconds() * 1000);

            if(debug::get_debug_option(base::SHOW_ARMOR))
            {
                debug_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
                debug_img_pub_.publish(*debug_image_msg_,camera_info_msg_);
                cv::imshow("image", image);
                cv::waitKey(1);
            }

            if(debug::get_debug_option(base::SHOW_BIN))
            {
                if(this->mode_ == base::Mode::NORMAL_RUNE || this->mode_ == base::Mode::RUNE)
                    debug_bin_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", this->rune_detector_->bin).toImageMsg();
                else
                    debug_bin_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", this->detector_->debug_binary_).toImageMsg();

                debug_bin_img_pub_.publish(*debug_bin_image_msg_,camera_info_msg_);
            }
            // double t1 = armors_msg.header.stamp.sec+armors_msg.header.stamp.nanosec*1e-9;//这段用于测试程序耗时用
            // double t2 = rclcpp::Clock().now().nanoseconds()*1e-9;
            //std::cout<<"time="<<t2-t1<<std::endl;
            armors_pub_->publish(armors_msg); 
        }
        else if (this->mode_ == base::Mode::NORMAL)
        {
            cv::Scalar colors[4] = {cv::Scalar(255, 255, 0),
                        cv::Scalar(0, 255, 0),
                        cv::Scalar(255, 255, 0),
                        cv::Scalar(0, 255, 255)};

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
            t.header.stamp =  image_msg->header.stamp;
            t.header.frame_id = "IMU";
            t.child_frame_id = "camera";
            t.transform.rotation.x = tf2_cam2IMU_quaternion.x();
            t.transform.rotation.y = tf2_cam2IMU_quaternion.y();
            t.transform.rotation.z = tf2_cam2IMU_quaternion.z();
            t.transform.rotation.w = tf2_cam2IMU_quaternion.w();

            //相机到IMU存在位置的偏移，每辆车不同，请在参数文件自行更改
            t.transform.translation.x = 0.005;
            t.transform.translation.y = 0;
            t.transform.translation.z = 0;
            this->tf_publisher_->sendTransform(t) ;
    
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

                std::string text1 = std::to_string(armor.num_id);
                // std::string text2 = std::to_string(int(armor.confidence*100));
                cv::putText(image, text1, armor.left.up, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(0, 255, 0), 1.8);
                // cv::putText(image, text2, armor.right.up, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                //             cv::Scalar(0, 255, 0), 0.5);
                cv::line(image,armor.left.up,armor.right.down ,cv::Scalar(255, 0, 255),1);
                cv::line(image,armor.left.down,armor.right.up ,cv::Scalar(255, 0, 255),1);

                //pnp solve
                cv::Mat tVec;
                cv::Mat rVec;
                bool is_solve;
                is_solve = this->pnp_solver_->solveArmorPose(armor,this->camera_matrix_,this->dist_coeffs_,tVec,rVec);
                if(!is_solve)
                {
                    RCLCPP_WARN(this->get_logger(), "camera param empty");
                    exit(0);
                }
                // std::cout << "tVec_____" << tVec << std::endl;
                // std::cout << "rVec_____" << rVec << std::endl;
                armor_msg.pose.position.x = tVec.at<double>(0, 0)/1000;
                armor_msg.pose.position.y = tVec.at<double>(1, 0)/1000;
                armor_msg.pose.position.z = tVec.at<double>(2, 0)/1000;
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

                double distance = sqrt(armor_msg.pose.position.x*armor_msg.pose.position.x+
                                            armor_msg.pose.position.y*armor_msg.pose.position.y+
                                            armor_msg.pose.position.z*armor_msg.pose.position.z);
                std::string text3 = std::to_string(float(distance));
                cv::putText(image, text3, armor.right.down, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(0, 255, 0), 0.5);
                armors_msg.armors.push_back(armor_msg);

                // std::cout << "distance_________" << distance << std::endl;
            }
            armors_msg.is_rune = false;

            auto time2 = steady_clock_.now();

            if(debug::get_debug_option(base::SHOW_DETECT_COST))RCLCPP_INFO(this->get_logger(), "Cost %.4f ms", (time2-time1).seconds() * 1000);



            if(debug::get_debug_option(base::SHOW_ARMOR))
            {
                debug_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
                debug_img_pub_.publish(*debug_image_msg_,camera_info_msg_);
            }
            if(debug::get_debug_option(base::SHOW_BIN))
            {
                debug_bin_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "mono8", this->detector_->debug_binary_).toImageMsg();
                debug_bin_img_pub_.publish(*debug_bin_image_msg_,camera_info_msg_);
            }

            armors_pub_->publish(armors_msg);
        }
    }
    void RuneDetectorNode::setMode(int mode)
    {
        if(mode == 1)
            this->mode_ = base::Mode::NORMAL_RUNE;
        else if(mode == 2)
            this->mode_ = base::Mode::RUNE;
        else if(mode == 0)
            this->mode_ = base::Mode::NORMAL;
        // normal就是自瞄状态
        else
            this->mode_ = this->last_mode_;

        this->last_mode_ = this->mode_;
    }

    void RuneDetectorNode::saveImage(double timestamp, cv::Mat image)
    {
        if(!have_mkdir)
        {
            this->image_folder_path = this->path + std::to_string(countFilesInDirectory(path));
            
            int state = mkdir(image_folder_path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            if(state == -1)
                std::cout<< "error to init dir"<<std::endl;
            
            else
                have_mkdir = true;
        }
        
        else
        {
            double delta_time = timestamp - last_save_timestamp;
            if(!image.empty() && delta_time > save_delta_time)
            {
                cv::imwrite(image_folder_path + "/" + std::to_string(image_num) + ".png", image);
                image_num++;
                last_save_timestamp = timestamp;
            }
        }
    }

    void RuneDetectorNode::saveDrawImage(double timestamp, cv::Mat image)
    {
        if(!have_mkdir_draw)
        {
            this->draw_image_folder_path = path_draw + std::to_string(countFilesInDirectory(path_draw));
            
            int state = mkdir(draw_image_folder_path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            if(state == -1)
                std::cout<< "error to init dir"<<std::endl;
            
            else
                have_mkdir_draw = true;
        }
        
        else
        {
            cv::imwrite(draw_image_folder_path + "/" + std::to_string(draw_image_num) + ".png", image);
            draw_image_num++;
        }
    }

    int RuneDetectorNode::countFilesInDirectory(std::string path)
    {
        int fileCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path)) 
        {
            if (entry.is_regular_file() || entry.is_directory()) {
                fileCount++;
            }
        }
        return fileCount;
    }

}

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rune_detector::RuneDetectorNode)
