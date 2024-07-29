//std
#include <chrono>
#include <sstream>

//ros
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <cv_bridge/cv_bridge.h>

#include "rmos_cam/cam_node.hpp"

using namespace std;

namespace rmos_cam
{
    DahengCamNode::DahengCamNode(const rclcpp::NodeOptions &options) : CamNode("daheng_camera", options)
    {
        // cam dev
        cam_dev_ = std::make_shared<camera::DahengCam>();

        // parameter
        int Width = this->declare_parameter("width", 1280);
        int Height = this->declare_parameter("height", 720);
        int Exposure = this->declare_parameter("exposure", 2000);
        bool AutoExposure = this->declare_parameter("auto_exposure", false);
        bool AutoWhiteBalance = this->declare_parameter("auto_white_balace", false);
        int Gamma = this->declare_parameter("gain", 9);
        int Fps = this->declare_parameter("fps", 100);
        float RGain = this->declare_parameter("rgain", 19.8);
        float BGain = this->declare_parameter("bgain", 19.8);
        float GGain = this->declare_parameter("ggain", 10.0);
        this->time_offset = this->declare_parameter("time_offset", 0);
        this->auto_exp_change = this->declare_parameter("auto_exp_change", false);
        this->max_exp = this->declare_parameter("max_exp", 0);
        this->min_exp = this->declare_parameter("min_exp", 0);
        
        this->normal_Exposure = Exposure;
        this->rune_Exposure = this->declare_parameter("rune_exposure", 1200);
        this->normal_Gamma = Gamma;
        // set paramter
        cam_dev_->set_parameter(camera::CamParamType::Height, Height);
        cam_dev_->set_parameter(camera::CamParamType::Width, Width);
        cam_dev_->set_parameter(camera::CamParamType::AutoExposure, AutoExposure);
        cam_dev_->set_parameter(camera::CamParamType::Exposure, Exposure);
        cam_dev_->set_parameter(camera::CamParamType::AutoWhiteBalance, AutoWhiteBalance);
        cam_dev_->set_parameter(camera::CamParamType::RGain, RGain);
        cam_dev_->set_parameter(camera::CamParamType::GGain, GGain);
        cam_dev_->set_parameter(camera::CamParamType::BGain, BGain);
        cam_dev_->set_parameter(camera::CamParamType::Gamma, Gamma);
        cam_dev_->set_parameter(camera::CamParamType::Fps, Fps);

        cam_dev_->open();

        img_pub_ = image_transport::create_camera_publisher(this, "/image_raw", rmw_qos_profile_default);

        // load camera_info
        cam_info_manager_ = std::make_unique<camera_info_manager::CameraInfoManager>(this, "DahengCam");
        auto pkg_path = ament_index_cpp::get_package_share_directory("rmos_bringup");
        auto yaml_path = "file://" + pkg_path + "/config/daheng_cam_info.yaml";
        if (!cam_info_manager_->loadCameraInfo(yaml_path))
        {
            RCLCPP_WARN(this->get_logger(), "Load Camera Info Fail!");
        }
        else
        {
            camera_info_msg_ = cam_info_manager_->getCameraInfo();
        }

        // exp publish
        this->exp_pub_ = this->create_publisher<std_msgs::msg::Int32>("/exp_info", rclcpp::SensorDataQoS());

        // get mode
        this->mode_sub_ = this->create_subscription<std_msgs::msg::Int8>
                ("/mode_info", rclcpp::SensorDataQoS(), [this](std_msgs::msg::Int8::ConstSharedPtr mode_msg)
                {
                    // RCLCPP_INFO(this->get_logger(), "mode is %d", (*mode_msg).data);
                    this->mode = (*mode_msg).data;
                    setMode(mode);
                });
        camera_info_pub_ = create_publisher<sensor_msgs::msg::CameraInfo>("/daheng_camera_info", 10);

        capture_thread_ = std::thread{[this]() -> void
                                      {
                                          while (rclcpp::ok())
                                          {
                                              JudgeReset();
                                              if (!cam_dev_->is_open())
                                              {
                                                RCLCPP_WARN(this->get_logger(), "Faild open camera!!");
                                                cam_dev_->open();
                                              }
                                              //sensor_msgs::msg::Image image_msg_;

                                              if (cam_dev_->grab_image(image_))
                                              {
                                                  image_msg_ = cv_bridge::CvImage(std_msgs::msg::Header(),"bgr8",image_).toImageMsg();
                                                  (*image_msg_).header.stamp = camera_info_msg_.header.stamp = this->now() + rclcpp::Duration(0, this->time_offset);
                                                  (*image_msg_).header.frame_id = "camera";
                                                  camera_info_msg_.header.frame_id = (*image_msg_).header.frame_id;
                                                  camera_info_pub_->publish(camera_info_msg_);

                                                  img_pub_.publish(*image_msg_, camera_info_msg_);

                                                  if(this->auto_exp_change)
                                                    autoExpChange();

                                                  exp_msg.data = cam_dev_->params_[camera::CamParamType::Exposure];
                                                  exp_pub_->publish(exp_msg);
                                                  img_pub_.publish(*image_msg_, camera_info_msg_);
                                              }
                                              else
                                              {
                                                  std::cout << cam_dev_->error_message() << std::endl;
                                                  cam_dev_->open();
                                              }
                                          }
                                      }};
        callback_handle_ = this->add_on_set_parameters_callback(
        std::bind(&DahengCamNode::parametersCallback, this, std::placeholders::_1));

            
    }
    void DahengCamNode::setMode(int mode)
    {
        if(mode == 1)
            this->mode_ = base::Mode::NORMAL_RUNE;
        else if(mode == 2)
            this->mode_ = base::Mode::RUNE;
        else if(mode == 0)
            this->mode_ = base::Mode::NORMAL;
        else 
            this->mode_ = base::Mode::WAIT;
    }

    void DahengCamNode::JudgeReset()
    {
        if(this->mode_ != this->previous_mode_ && this->mode_ != base::Mode::WAIT)
        {
            // std::cout<<"mode_num:"<<mode_num<<std::endl;
            // this->mode_num++;   
            // if(this->mode_num < 0)
            // {
            //     return;
            // }
            // this->mode_num = 0;
            // std::cout<<"change mode"<<std::endl;
            if(this->mode_ == base::Mode::NORMAL)
                cam_dev_->changeExp(this->normal_Exposure);
            else
                cam_dev_->changeExp(this->rune_Exposure);   
            this->previous_mode_ = this->mode_;             
        }
        else
            this->mode_num = 0;

    }

    void DahengCamNode::autoExpChange()
    {
        int now_exp = cam_dev_->params_[camera::CamParamType::Exposure];
        if(this->change_num < 5)
        {
            change_num++;
            return;
        }
        change_num = 0;
        now_exp += 100;
        if(now_exp < this->min_exp || now_exp > this->max_exp)
            now_exp = this->min_exp;
        cam_dev_->changeExp(now_exp);
        std::cout<<"exp:"<< cam_dev_->params_[camera::CamParamType::Exposure] << std::endl;
    }

    DahengCamNode::~DahengCamNode()
    {
        if (capture_thread_.joinable())
        {
            capture_thread_.join();
        }
        cam_dev_->close();
        RCLCPP_INFO(this->get_logger(), "Camera node destroyed!");
    }

    rcl_interfaces::msg::SetParametersResult DahengCamNode::parametersCallback(
        const std::vector<rclcpp::Parameter> &parameters)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "success";
        // Here update class attributes, do some actions, etc.

        for (const auto &param: parameters)
        {
            RCLCPP_INFO(this->get_logger(), "%s", param.get_name().c_str());
            RCLCPP_INFO(this->get_logger(), "%s", param.get_type_name().c_str());
            RCLCPP_INFO(this->get_logger(), "%s", param.value_to_string().c_str());

            if (param.get_name() == "exposure") {
                if(this->mode_ == base::Mode::NORMAL){
                    this->normal_Exposure = param.as_int();
                    cam_dev_->changeExp(this->normal_Exposure);
                }
            }  else if (param.get_name() == "time_offset") {
                this->time_offset = param.as_int();
            } else if (param.get_name() == "rune_exposure") {
                if(this->mode_ == base::Mode::RUNE || this->mode_ == base::Mode::NORMAL_RUNE){
                    this->rune_Exposure = param.as_int();
                    cam_dev_->changeExp(this->rune_Exposure);
                }
            } else {
                RCLCPP_WARN(this->get_logger(), "我没在相机节点写这个参数... %s", param.get_name().c_str());
            }

        }
        return result;
    }
} // namespace rmos_cam


#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_cam::DahengCamNode)