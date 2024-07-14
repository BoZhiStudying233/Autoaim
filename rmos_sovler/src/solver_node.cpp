#include "rmos_solver/solver_node.hpp"
#include "angles/angles.h"

namespace rmos_solver {
    RuneSolverNode::RuneSolverNode(const rclcpp::NodeOptions &options) : Node("solver", options) {
        RCLCPP_INFO(this->get_logger(), "Start solver_node");
        this->controler_ = std::make_shared<solver::Controler>();
        this->camera_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>("/daheng_camera_info", rclcpp::SensorDataQoS(),
                                                                                         [this](sensor_msgs::msg::CameraInfo::ConstSharedPtr camera_info_msg)
                                                                                         {
                                                                                            RCLCPP_INFO(this->get_logger(), "Receive camera infomation in solver");

                                                                                            this->camera_info_msg_ = *camera_info_msg;

                                                                                            this->camera_matrix_.create(3, 3, CV_64FC1);

                                                                                            for (int i = 0; i < 9; i++)
                                                                                            {
                                                                                            this->camera_matrix_.at<double>(i / 3, i % 3) = camera_info_msg->k[i];
                                                                                            }
                                                                                            this->camera_info_sub_.reset();
                                                                                            this->controler_->getParam(this->camera_matrix_);
                                                                                            this->finish_camera_info_set = 1;
                                                                                         });

        // set subscriber for imu_time,armor,and bullet_speed
        using rclcpp::CallbackGroupType;

        this->armors_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        armors_sub_.subscribe(this, "/rune_detector/armors", rmw_qos_profile_sensor_data);
        // Subscriber with tf2 message_filter
        tf2_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
                this->get_node_base_interface(), this->get_node_timers_interface());
        tf2_buffer_->setCreateTimerInterface(timer_interface);
        tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
        target_frame_ = "world";
        tf2_filter_ = std::make_shared<tf2_filter>(
                armors_sub_, *tf2_buffer_, target_frame_, 100, this->get_node_logging_interface(),
                this->get_node_clock_interface(), std::chrono::duration<int>(100));
        tf2_filter_->registerCallback(&RuneSolverNode::armorsCallBack, this);

        this->tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        this->quaternion_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto quaternion_sub_option = rclcpp::SubscriptionOptions();
        quaternion_sub_option.callback_group = this->quaternion_sub_callback_group_;
        this->quaternion_sub_ = this->create_subscription<rmos_interfaces::msg::QuaternionTime>("/imu_quaternion", rclcpp::SensorDataQoS(),
                                                                                                std::bind(&RuneSolverNode::quaternionCallBack, this, std::placeholders::_1),
                                                                                                quaternion_sub_option);

        this->bs_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto bs_sub_option = rclcpp::SubscriptionOptions();
        bs_sub_option.callback_group = this->bs_sub_callback_group_;
        this->bs_sub_ = this->create_subscription<rmos_interfaces::msg::BulletSpeed>("/bs_info", rclcpp::SensorDataQoS(),
                                                                                     std::bind(&RuneSolverNode::bsCallBack, this, std::placeholders::_1),
                                                                                     bs_sub_option);
        this->aimstate_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto autoaim_state_sub_option = rclcpp::SubscriptionOptions();
        autoaim_state_sub_option.callback_group = this->aimstate_sub_callback_group_;
        // this->autoaim_state_sub_ = this->create_subscription<rmos_interfaces::msg::AutoaimState>("/autoaim_state", rclcpp::SensorDataQoS(),
        //                                                                                          std::bind(&RuneSolverNode::autoaimStateCallBack, this, std::placeholders::_1),
        //                                                                                          autoaim_state_sub_option);
        this->initParams();

        // this->detect_marker_pub_ =
        //         this->create_publisher<visualization_msgs::msg::MarkerArray>("/detect/marker", 10);
        // this->process_marker_pub_ =
        //         this->create_publisher<visualization_msgs::msg::MarkerArray>("/process/marker", 10);
        this->target_pub_ = this->create_publisher<rmos_interfaces::msg::Target>("/target", rclcpp::SensorDataQoS());

        // int control_interval = this->declare_parameter("control_interval", 10);
        // auto duration = std::chrono::milliseconds(control_interval); // 毫秒
        // auto timer_callback =
        //     [this]() -> void
        //     {
        //         if(this->finish_camera_info_set)
        //             this->publishTarget();
        //     };
            
        // this->tracker_sample_ = this->create_wall_timer(duration, timer_callback);
    }

    void RuneSolverNode::armorsCallBack(const rmos_interfaces::msg::Armors::SharedPtr armors_msg)
    {
        // std::cout<<"已开始回调"<<std::endl;
        this->mode__ = base::Mode::RUNE;
        if(this->mode__ != base::Mode::RUNE&&this->mode__ != base::Mode::NORMAL_RUNE)
            return;
        if (quaternion_buf_.size() > 0) {
            uint32_t timestamp_recv = quaternion_buf_.back().timestamp_recv;
            rmos_interfaces::msg::Armor target_rune_armor;
            target_rune_armor = armors_msg->armors[0];
            cv::Point3f aiming_point;
            geometry_msgs::msg::PoseStamped ps;
            ps.header = armors_msg->header;
            ps.pose = target_rune_armor.pose;
            try {
                target_rune_armor.pose = tf2_buffer_->transform(ps, target_frame_).pose;
            }
            catch (const tf2::LookupException &ex) {
                RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
                return;
            }
            catch (const tf2::ExtrapolationException &ex) {
                RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
                return;
            }

            rmos_interfaces::msg::Target target_msg;
            // base::Armor new_armor;
            // new_armor.position.x = armor.pose.position.x;
            // new_armor.position.y = armor.pose.position.y;
            // new_armor.position.z = armor.pose.position.z;
            // new_armor.distance_to_image_center = armor.distance_to_image_center;
            aiming_point.x = target_rune_armor.pose.position.x;
            aiming_point.y = target_rune_armor.pose.position.y;
            aiming_point.z = target_rune_armor.pose.position.z;

            cv::Point3f p_y_t = controler_->ballistic_solver_.getAngleTime(aiming_point*1000, armors_msg->is_rune);
            float new_pitch = p_y_t.x;
            float new_yaw = p_y_t.y;
            
            rmos_interfaces::msg::QuaternionTime gimble_pose = quaternion_buf_.back();
            tf2::Quaternion tf_gimble_q;
            tf2::fromMsg(gimble_pose.quaternion_stamped.quaternion, tf_gimble_q);
            double gimble_roll, gimble_pitch, gimble_yaw;
            tf2::Matrix3x3(tf_gimble_q).getRPY(gimble_roll, gimble_pitch, gimble_yaw);
            float pitch = gimble_pitch * 180.0 / 3.1415926535;
            float yaw = gimble_yaw * 180.0 / 3.1415926535;
            float gun_pitch = -new_pitch + controler_->rune_gun_pitch_offset_;
            std::cout<<"gun_pitch:"<<gun_pitch<<std::endl;
            float gun_yaw = new_yaw + controler_->rune_gun_yaw_offset_;

            target_msg.id = 20;
            target_msg.track_state = base::TRACKING;
            target_msg.position.x = aiming_point.x;
            target_msg.position.y = aiming_point.y;
            target_msg.position.z = aiming_point.z;

            // std::cout<<"distance____"<<sqrt(target_msg.position.x*target_msg.position.x+
            //                                 target_msg.position.y*target_msg.position.y+
            //                                 target_msg.position.z*target_msg.position.z)<<std::endl;

            // std::cout<<"timestamp:"<<timestamp<<std::endl;
            bool is_fire = this->controler_->judgeRuneFire(target_rune_armor.num_id,timestamp_recv);
            target_msg.suggest_fire = is_fire;

            target_msg.gun_pitch = gun_pitch;
            target_msg.gun_yaw = gun_yaw;
            target_pub_->publish(target_msg);
        }
        else
        {
            std::cout << "no imu data" << std::endl;
            imu_data_count_++;
            if(imu_data_count_>100)
            {
                exit(0);
            }
            return;
        }
    }


    void RuneSolverNode::quaternionCallBack(const rmos_interfaces::msg::QuaternionTime::SharedPtr quaternion_msg)
    {
        this->quaternion_buf_.push(*quaternion_msg);
        if (quaternion_buf_.size() > 5)
        {
            quaternion_buf_.pop();
        }
        geometry_msgs::msg::TransformStamped t;
        t.header.stamp =  quaternion_msg->quaternion_stamped.header.stamp;
        t.header.frame_id = "world"; //注意坐标系
        t.child_frame_id = "IMU";
        t.transform.rotation.x = quaternion_msg->quaternion_stamped.quaternion.x;
        t.transform.rotation.y = quaternion_msg->quaternion_stamped.quaternion.y;
        t.transform.rotation.z = quaternion_msg->quaternion_stamped.quaternion.z;
        t.transform.rotation.w = quaternion_msg->quaternion_stamped.quaternion.w;
        tf_publisher_->sendTransform(t);
    }

    void RuneSolverNode::bsCallBack(const rmos_interfaces::msg::BulletSpeed::SharedPtr bs_msg)
    {
        // 电控暂时不发弹速，该回调不会运作，留置备用
        int bullet_speed = (*bs_msg).speed;
        this->controler_->ballistic_solver_.setBulletSpeed(bullet_speed);
    }

    // void RuneSolverNode:: autoaimStateCallBack(const rmos_interfaces::msg::AutoaimState::SharedPtr autoaim_state_msg)
    // {
    //     if(this->autoaim_state_buf_.size()>0)
    //     {
    //         if(autoaim_state_buf_.back().autoaim_state == 0 &&(*autoaim_state_msg).autoaim_state == 1) //操作手按下右键即重置当前追踪器
    //         {
    //             this->controler_->tracker_.reset();
    //         }
    //     }
    //     this->autoaim_state_buf_.push(*autoaim_state_msg);
    //     if (this->autoaim_state_buf_.size() > 5)
    //     {
    //         this->autoaim_state_buf_.pop();
    //     }
    // }

    void RuneSolverNode::initParams()
    {
        this->controler_->rune_gun_pitch_offset_ = this->declare_parameter("rune_gun_pitch_offset", 0.0f);
        this->controler_->rune_gun_yaw_offset_ = this->declare_parameter("rune_gun_yaw_offset", 0.0f);
        this->controler_->ballistic_solver_.bullet_speed_ = this->declare_parameter("bullet_speed", 27);
        this->controler_->bs_fly_time_ = this->declare_parameter("bs_fly_time", 0.0f);
        this->controler_->ready_time_ = this->declare_parameter("ready_time", 0.0f);


        this->controler_->ballistic_solver_.rune_ballistic_param_ = {
            .bs_coeff_first = this->declare_parameter("bs_coeff_first", 1.0),
            .bs_coeff_second = this->declare_parameter("bs_coeff_second", 1.0),
            .bs_coeff_third = this->declare_parameter("bs_coeff_third", 1.0),
            .bs_coeff_fourth = this->declare_parameter("bs_coeff_fourth", 1.0),
            .distance_first  = this->declare_parameter("distance_first", 0),
            .distance_second  = this->declare_parameter("distance_second", 0),
            .distance_third  = this->declare_parameter("distance_third", 0),
            .k = this->declare_parameter("k", 0.0402),
            .g = this->declare_parameter("g", 9.75),
        };

    }
}

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_solver::RuneSolverNode)

