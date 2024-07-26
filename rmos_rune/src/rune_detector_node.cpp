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

#include "rmos_rune/rune_detector_node.hpp"


namespace rmos_rune
{
    void RuneDetectorNode::imageCallBack(const sensor_msgs::msg::Image::ConstSharedPtr &image_msg)
    {
        if(this->mode_ != base::Mode::RUNE&&this->mode_ != base::Mode::NORMAL_RUNE)
            return;

        rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
        double timestamp = image_msg->header.stamp.sec + image_msg->header.stamp.nanosec*1e-9;
        cv::Mat image = cv_bridge::toCvShare(image_msg, "bgr8")->image;

        rmos_interfaces::msg::Armors armors_msg;
        rmos_interfaces::msg::Armor armor_msg;
        armors_msg.header = image_msg->header;
        if(this->save_image)
            saveImage(timestamp, image);

        base::RuneArmor target_rune_armor;
        std::vector<base::RuneArmor> rune_armors;
        auto time1 = steady_clock_.now();
        this->rune_detector_->DlRuneDetect(image,target_rune_armor, rune_armors);
        // std::cout<<"time:"<<(time1 - time2).seconds()<<std::endl;


        armor_msg.num_id = rune_detector_->id;
        std::string text = "id:" + std::to_string(rune_detector_->id);
        cv::putText(image, text, (cv::Point2i(0, 20), cv::Point2i(20, 20)), cv::FONT_HERSHEY_SIMPLEX, 1,
                    cv::Scalar(0, 255, 0), 0.5);

        target_rune_armor.timestamp = timestamp;
        // std::cout << "timestamp:" << timestamp<< std::endl;
        Eigen::Vector3d tVec;
        std::vector<cv::Point2f> rune_next_pos;
        geometry_msgs::msg::TransformStamped transform_to_world, transform_to_camera;
        
        // try
        //     {//实车记得改坐标系名称！ remeber to change
        //         transform_to_world = tf_buffer->lookupTransform("world", "camera", tf2::TimePointZero);
        //         transform_to_camera = tf_buffer->lookupTransform("camera", "world", tf2::TimePointZero);
        //     }
        // catch (tf2::TransformException & ex)
        //     {
        //         RCLCPP_ERROR(this->get_logger(), "Could not get the transformation!!");
        //     }
            
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

            }
            else//三维圆的情况
            {
                // cv::Point2d point2D;
                std::vector<cv::Point3f> objectPoints;
                std::vector<cv::Point2f> imagePoints;

                tVec.x() = tVec.x()/tVec.z();
                tVec.y() = tVec.y()/tVec.z();
                tVec.z() = 1;
                tVec.x() = tVec.x()/tVec.z();
                tVec.y() = tVec.y()/tVec.z();
                tVec.z() = 1;

                objectPoints.emplace_back(tVec.x(), tVec.y(), tVec.z());
                objectPoints.emplace_back(tVec.x(), tVec.y(), tVec.z());

                cv::Mat tvec = (cv::Mat_<float>(1, 3) << 0, 0, 0);
                cv::projectPoints(objectPoints, cv::Mat::zeros(1, 3, CV_64F), tvec, this->camera_matrix_, this->dist_coeffs_, imagePoints);
                cv::Point2d point2D = imagePoints[0];
                if (point2D.x > 0 && point2D.y > 0 && point2D.x < image.cols && point2D.y < image.rows)
                    cv::circle(image, point2D, 6, cv::Scalar(255,0,255), -1);
            }
            armor_msg.pose.position.x = tVec.x()/1000;
            armor_msg.pose.position.y = tVec.y()/1000;
            armor_msg.pose.position.z = tVec.z()/1000;//相机坐标系下，目标符中心的坐标  
            

            Eigen::Vector3d CamaraVector = Eigen::Vector3d(0,0,1);
            cv::Mat rVec = cv::Mat::zeros(3, 1, CV_64F);

            //tVec = Eigen::Vector3d(tVec.x()/tVec.norm(),tVec.y()/tVec.norm(),tVec.z()/tVec.norm());//获取单位向量
            //cv::eigen2cv(CamaraVector.cross(tVec), rVec);
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
            armor_msg.pose.orientation.w = tf2_quaternion.w();//四元数


            armors_msg.armors.push_back(armor_msg);
        }
        armors_msg.is_rune = true;
        


        text = "Exposure:" + std::to_string(this->Exposure);
        cv::putText(image, text, (cv::Point2i(0, 50), cv::Point2i(20, 50)), cv::FONT_HERSHEY_SIMPLEX, 1,
                    cv::Scalar(0, 255, 0), 0.5);

        if(this->save_draw_image)
            saveDrawImage(timestamp, image);
        auto time2 = steady_clock_.now();

        if(this->tell_cost_time)
            RCLCPP_INFO(this->get_logger(), "Cost %.4f ms", (time2-time1).seconds() * 1000);
        
        if(true){
            debug_image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();
            debug_img_pub_.publish(*debug_image_msg_,camera_info_msg_);
        }


       
        if(armors_msg.armors.size()!=0){
            armors_pub_->publish(armors_msg); 
        }

    }
    void RuneDetectorNode::setMode(int mode)
    {
        // std::cout<<"mode:"<<mode<<std::endl;
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
            // std::cout<<"timestamp:"<<timestamp - 1.7161e+09<<std::endl;
            // std::cout<<"last_save_timestamp:"<<last_save_timestamp<<std::endl;
            // std::cout<<"delta_time:"<<delta_time<<std::endl;
            if(!image.empty() && delta_time > save_delta_time)
            {
                cv::imwrite(image_folder_path + "/" + std::to_string(image_num) + ".jpg", image);
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

            cv::imwrite(draw_image_folder_path + "/" + std::to_string(draw_image_num) + ".jpg", image);
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

    std::unique_ptr<RuneDetector::DlRuneDetector> RuneDetectorNode::initDetector()
    {
        RuneDetector::RuneParam params = {
            .show_R = this->declare_parameter("image_params.show_R", 0),

            .blue_brightness_thresh = this->declare_parameter("image_params.blue_brightness_thresh", 30),
            .blue_color_thresh = this->declare_parameter("image_params.blue_color_thresh", 50),
            .red_brightness_thresh = this->declare_parameter("image_params.red_brightness_thresh", 30),
            .red_color_thresh = this->declare_parameter("image_params.red_color_thresh", 50),
            .blue_red_diff = this->declare_parameter("image_params.blue_red_diff", 20),

 
            
            .circle_center_conf = this->declare_parameter("deep_learning_params.circle_center_conf", 0),
            .no_activate_conf = this->declare_parameter("deep_learning_params.no_activate_conf", 0.7),
            .activate_conf = this->declare_parameter("deep_learning_params.activate_conf", 0),
            .circle_center_roi_width = this->declare_parameter("deep_learning_params.circle_center_roi_width", 20),
            .max_diff_distance_ratio = this->declare_parameter("deep_learning_params.max_diff_distance_ratio", 0.5),

            .low_threshold = this->declare_parameter("image_params.low_threshold", 1000),
            .high_threshold = this->declare_parameter("image_params.high_threshold", 4000),
            .tell_area = this->declare_parameter("image_params.tell_area", 0),
    
            .delay_time = this->declare_parameter("fitting_params.delay_time", 0.45f),
            .save_txt = this->declare_parameter("fitting_params.save_txt", 0),
            .print_result = this->declare_parameter("fitting_params.print_result", 1), // 添加了 print_result 参数声明    
        };
        std::cout<<"low_threshold:"<<params.low_threshold<<std::endl;
        this->save_image = this->declare_parameter("debug_params.save_image", 0);
        this->save_draw_image = this->declare_parameter("debug_params.save_draw_image", 0);
        this->tell_cost_time = this->declare_parameter("debug_params.tell_cost_time", 0);
        // this->fitting_->fit.delay_time = this->declare_parameter("fitting_params.delay_time", 0.45f);
        this->fitting_->Points_num =  this->declare_parameter("fitting_params.points_num", 50);
        // this->fitting_->fit.save_txt = this->declare_parameter("fitting_params.save_txt", 0);
        this->fitting_->fit.delay_time = params.delay_time;
        this->fitting_->fit.print_result = params.print_result;
        this->fitting_->fit.save_txt = this->fitting_-> save_txt = params.save_txt;
        auto rune_detector = std::make_unique<RuneDetector::DlRuneDetector>(params);

        return rune_detector;
    }

}

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_rune::RuneDetectorNode)
