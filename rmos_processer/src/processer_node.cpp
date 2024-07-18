#include "rmos_processer/processer_node.hpp"
#include "angles/angles.h"

namespace rmos_processer {
    ProcesserNode::ProcesserNode(const rclcpp::NodeOptions &options) : Node("processer", options) {
        RCLCPP_INFO(this->get_logger(), "Start processer_node");
        controler_ = std::make_shared<processer::Controler>();
        this->camera_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>("/daheng_camera_info", rclcpp::SensorDataQoS(),
                                                                                         [this](sensor_msgs::msg::CameraInfo::ConstSharedPtr camera_info_msg)
                                                                                         {
                                                                                            RCLCPP_INFO(this->get_logger(), "Receive camera infomation in processer");

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
        armors_sub_.subscribe(this, "/rmos_detector/armors", rmw_qos_profile_sensor_data);
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
        tf2_filter_->registerCallback(&ProcesserNode::armorsCallBack, this);

        this->quaternion_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto quaternion_sub_option = rclcpp::SubscriptionOptions();
        quaternion_sub_option.callback_group = this->quaternion_sub_callback_group_;
        this->quaternion_sub_ = this->create_subscription<rmos_interfaces::msg::QuaternionTime>("/imu_quaternion", rclcpp::SensorDataQoS(),
                                                                                                std::bind(&ProcesserNode::quaternionCallBack, this, std::placeholders::_1),
                                                                                                quaternion_sub_option);

        this->bs_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto bs_sub_option = rclcpp::SubscriptionOptions();
        bs_sub_option.callback_group = this->bs_sub_callback_group_;
        this->bs_sub_ = this->create_subscription<rmos_interfaces::msg::BulletSpeed>("/bs_info", rclcpp::SensorDataQoS(),
                                                                                     std::bind(&ProcesserNode::bsCallBack, this, std::placeholders::_1),
                                                                                     bs_sub_option);
        this->aimstate_sub_callback_group_ = this->create_callback_group(CallbackGroupType::Reentrant);
        auto autoaim_state_sub_option = rclcpp::SubscriptionOptions();
        autoaim_state_sub_option.callback_group = this->aimstate_sub_callback_group_;
        this->autoaim_state_sub_ = this->create_subscription<rmos_interfaces::msg::AutoaimState>("/autoaim_state", rclcpp::SensorDataQoS(),
                                                                                                 std::bind(&ProcesserNode::autoaimStateCallBack, this, std::placeholders::_1),
                                                                                                 autoaim_state_sub_option);
        this->initMarkers();
        this->initParams();

        this->detect_marker_pub_ =
                this->create_publisher<visualization_msgs::msg::MarkerArray>("/detect/marker", 10);
        this->process_marker_pub_ =
                this->create_publisher<visualization_msgs::msg::MarkerArray>("/process/marker", 10);
        this->target_pub_ = this->create_publisher<rmos_interfaces::msg::Target>("/target", rclcpp::SensorDataQoS());

        int control_interval = this->declare_parameter("control_interval", 10);
        auto duration = std::chrono::milliseconds(control_interval); // 毫秒
        auto timer_callback =
            [this]() -> void
            {
                if(this->finish_camera_info_set)
                    this->publishTarget();
            };
            
        this->tracker_sample_ = this->create_wall_timer(duration, timer_callback);
    }

    void ProcesserNode::armorsCallBack(const rmos_interfaces::msg::Armors::SharedPtr armors_msg)
    {
        // return ;
        if (quaternion_buf_.size() > 0) {
            uint32_t timestamp_recv = quaternion_buf_.back().timestamp_recv;

            detect_marker_array_.markers.clear();
            armor_marker_.points.clear();
            armor_marker_.id = 0;
            text_marker_.id = 0;

            std::vector <base::Armor> new_armors;
            double timestamp = armors_msg->header.stamp.sec + armors_msg->header.stamp.nanosec * 1e-9;

            for (auto &armor: armors_msg->armors) {
                geometry_msgs::msg::PoseStamped ps;
                ps.header = armors_msg->header;
                ps.pose = armor.pose;
                try {
                    armor.pose = tf2_buffer_->transform(ps, target_frame_).pose;
                }
                catch (const tf2::LookupException &ex) {
                    RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
                    return;
                }
                catch (const tf2::ExtrapolationException &ex) {
                    RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
                    return;
                }

                base::Armor new_armor;
                new_armor.num_id = armor.num_id;
                new_armor.position.x = armor.pose.position.x;
                new_armor.position.y = armor.pose.position.y;
                new_armor.position.z = armor.pose.position.z;
                new_armor.distance_to_image_center = armor.distance_to_image_center;

                // Get armor yaw
                tf2::Quaternion tf_armor_q;
                tf2::fromMsg(armor.pose.orientation, tf_armor_q);
                double armor_roll, armor_pitch, armor_yaw;
                tf2::Matrix3x3(tf_armor_q).getRPY(armor_roll, armor_pitch, armor_yaw);
                //new_armor.yaw = armor_yaw;
                new_armor.yaw = controler_->tracker_.last_yaw_ +
                                angles::shortest_angular_distance(controler_->tracker_.last_yaw_, armor_yaw);

                //erase armors ...
                new_armors.push_back(new_armor);

                armor_marker_.id++;
                armor_marker_.pose = armor.pose;
                text_marker_.id++;
                text_marker_.pose.position = armor.pose.position;
                text_marker_.pose.position.y -= 0.1;
                text_marker_.text = std::to_string(armor.num_id);
                detect_marker_array_.markers.emplace_back(text_marker_);
            }

            controler_->updateTracker(new_armors, timestamp);

            using Marker = visualization_msgs::msg::Marker;
            armor_marker_.action = (armors_msg->armors).empty() ? Marker::DELETE : Marker::ADD;
            detect_marker_array_.markers.emplace_back(armor_marker_);
            detect_marker_pub_->publish(detect_marker_array_);
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

    void ProcesserNode::publishTarget()
    {
        rmos_interfaces::msg::Target target_msg;
        if (quaternion_buf_.size() < 1)
            return;

        target_msg = this->controler_->getTarget(quaternion_buf_);

        //将瞄准点投影回2d平面，通过像素距离判断，判断开火
        geometry_msgs::msg::PoseStamped px;
        px.header = target_msg.header;
        px.pose.position.x = target_msg.position.x*1000;
        px.pose.position.y = target_msg.position.y*1000;
        px.pose.position.z = target_msg.position.z*1000;
        std::string oral_frame = "camera";
        try
        {
            px.pose = tf2_buffer_->transform(px, oral_frame).pose;
        }
        catch (const tf2::LookupException &ex)
        {
            RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
            return;
        }
        catch (const tf2::ExtrapolationException &ex)
        {
            RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
            return;
        }
        cv::Point3f aiming_point_camera(px.pose.position.x, px.pose.position.y, px.pose.position.z);
                    
        target_msg.suggest_fire = this->controler_->judgeFire(aiming_point_camera,this->controler_->tracker_.target_state(7));
        target_pub_->publish(target_msg);
        if(true)
        {
            publishMarkers(target_msg);
        }
    }

    void ProcesserNode::quaternionCallBack(const rmos_interfaces::msg::QuaternionTime::SharedPtr quaternion_msg)
    {
        this->quaternion_buf_.push(*quaternion_msg);
        if (quaternion_buf_.size() > 5)
        {
            quaternion_buf_.pop();
        }
    }

    void ProcesserNode::bsCallBack(const rmos_interfaces::msg::BulletSpeed::SharedPtr bs_msg)
    {
        // 电控暂时不发弹速，该回调不会运作，留置备用
        int bullet_speed = (*bs_msg).speed;
        this->controler_->ballistic_solver_.setBulletSpeed(bullet_speed);
    }

    void ProcesserNode:: autoaimStateCallBack(const rmos_interfaces::msg::AutoaimState::SharedPtr autoaim_state_msg)
    {
        if(this->autoaim_state_buf_.size()>0)
        {
            if(autoaim_state_buf_.back().autoaim_state == 0 &&(*autoaim_state_msg).autoaim_state == 1) //操作手按下右键即重置当前追踪器
            {
                this->controler_->tracker_.reset();
            }
        }
        this->autoaim_state_buf_.push(*autoaim_state_msg);
        if (this->autoaim_state_buf_.size() > 5)
        {
            this->autoaim_state_buf_.pop();
        }
    }

    void ProcesserNode::initMarkers()
    {
        armor_marker_.ns = "armors_cube";
        armor_marker_.header.frame_id = "world";
        armor_marker_.action = visualization_msgs::msg::Marker::ADD;
        armor_marker_.type = visualization_msgs::msg::Marker::CUBE;
        armor_marker_.scale.x = 0.02;
        armor_marker_.scale.y = 0.125;
        armor_marker_.scale.z = 0.08;
        armor_marker_.color.a = 1.0;
        armor_marker_.color.r = 1.0;
        armor_marker_.lifetime = rclcpp::Duration::from_seconds(0.1);


        text_marker_.ns = "classification";
        text_marker_.header.frame_id = "world";
        text_marker_.action = visualization_msgs::msg::Marker::ADD;
        text_marker_.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
        text_marker_.scale.z = 0.1;
        text_marker_.color.a = 1.0;
        text_marker_.color.r = 1.0;
        text_marker_.color.g = 1.0;
        text_marker_.color.b = 1.0;
        text_marker_.lifetime = rclcpp::Duration::from_seconds(0.1);

        position_marker_.ns = "position";
        position_marker_.type = visualization_msgs::msg::Marker::SPHERE;
        position_marker_.scale.x = position_marker_.scale.y = position_marker_.scale.z = 0.1;
        position_marker_.color.a = 1.0;
        position_marker_.color.g = 1.0;
        linear_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
        linear_v_marker_.ns = "linear_v";
        linear_v_marker_.scale.x = 0.03;
        linear_v_marker_.scale.y = 0.05;
        linear_v_marker_.color.a = 1.0;
        linear_v_marker_.color.r = 1.0;
        linear_v_marker_.color.g = 1.0;
        angular_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
        angular_v_marker_.ns = "angular_v";
        angular_v_marker_.scale.x = 0.03;
        angular_v_marker_.scale.y = 0.05;
        angular_v_marker_.color.a = 1.0;
        angular_v_marker_.color.b = 1.0;
        angular_v_marker_.color.g = 1.0;
        armors_marker_.ns = "armors";
        armors_marker_.type = visualization_msgs::msg::Marker::SPHERE_LIST;
        armors_marker_.scale.x = armors_marker_.scale.y = armors_marker_.scale.z = 0.1;
        armors_marker_.color.a = 1.0;
        armors_marker_.color.r = 1.0;
        aiming_marker_.ns = "aiming_point";
        aiming_marker_.type = visualization_msgs::msg::Marker::SPHERE;
        aiming_marker_.scale.x = aiming_marker_.scale.y = aiming_marker_.scale.z = 0.1;
        aiming_marker_.color.a = 1.0;
        aiming_marker_.color.b = 1.0;
    }

    void ProcesserNode::publishMarkers(const rmos_interfaces::msg::Target &target_msg)
    {
        process_marker_array_.markers.clear();
        position_marker_.header = target_msg.header;
        linear_v_marker_.header = target_msg.header;
        angular_v_marker_.header = target_msg.header;
        armors_marker_.header = target_msg.header;
        aiming_marker_.header = target_msg.header;

        // get armors num
        int armors_num = 4;
        if(this->controler_->tracker_.tracked_id == 11 || this->controler_->tracker_.tracked_id == 12 || this->controler_->tracker_.tracked_id == 13 )
        {
            armors_num = 2;
        }
        else if(this->controler_->tracker_.tracked_id == 7)
        {
            armors_num = 3;
        }

        bool is_tracking = false;

        if(controler_->tracker_.tracker_state == base::TEMP_LOST ||controler_->tracker_.tracker_state  == base::TRACKING)
        {
            is_tracking = true;
        }


        if (is_tracking) {
            double yaw = controler_->tracker_.target_state(6), r1 = controler_->tracker_.target_state(8), r2 = controler_->tracker_.another_r;
            double xc = controler_->tracker_.target_state(0), yc = controler_->tracker_.target_state(2), za = controler_->tracker_.target_state(4);
            double vx = controler_->tracker_.target_state(1), vy = controler_->tracker_.target_state(3), vz = controler_->tracker_.target_state(5);
            double dz = controler_->tracker_.dz;
            double v_yaw =  controler_->tracker_.target_state(7);
            position_marker_.action = visualization_msgs::msg::Marker::ADD;
            position_marker_.pose.position.x = xc;
            position_marker_.pose.position.y = yc;
            position_marker_.pose.position.z = za + dz / 2;


            linear_v_marker_.action = visualization_msgs::msg::Marker::ADD;
            linear_v_marker_.points.clear();
            linear_v_marker_.points.emplace_back(position_marker_.pose.position);
            geometry_msgs::msg::Point arrow_end = position_marker_.pose.position;
            arrow_end.x += vx;
            arrow_end.y += vy;
            arrow_end.z += vz;
            linear_v_marker_.points.emplace_back(arrow_end);

            angular_v_marker_.action = visualization_msgs::msg::Marker::ADD;
            angular_v_marker_.points.clear();
            angular_v_marker_.points.emplace_back(position_marker_.pose.position);
            arrow_end = position_marker_.pose.position;
            arrow_end.z += v_yaw / M_PI;
            angular_v_marker_.points.emplace_back(arrow_end);

            armors_marker_.action = visualization_msgs::msg::Marker::ADD;
            armors_marker_.points.clear();
            // Draw armors
            bool is_current_pair = true;
            size_t a_n = armors_num;
            geometry_msgs::msg::Point p_a;
            double r = 0;
            for (size_t i = 0; i < a_n; i++) {
                double tmp_yaw = yaw + i * (2 * M_PI / a_n);
                // Only 4 armors has 2 radius and height
                if (a_n == 4) {
                    r = is_current_pair ? r1 : r2;
                    p_a.z = za + (is_current_pair ? 0 : dz);
                    is_current_pair = !is_current_pair;
                } else {
                    r = r1;
                    p_a.z = za;
                }
                p_a.x = xc - r * cos(tmp_yaw);
                p_a.y = yc - r * sin(tmp_yaw);
                armors_marker_.points.emplace_back(p_a);
            }
            aiming_marker_.action = visualization_msgs::msg::Marker::ADD;
            aiming_marker_.pose.position.x = target_msg.position.x;
            aiming_marker_.pose.position.y = target_msg.position.y;
            aiming_marker_.pose.position.z = target_msg.position.z;
        } else {
            position_marker_.action = visualization_msgs::msg::Marker::DELETE;
            linear_v_marker_.action = visualization_msgs::msg::Marker::DELETE;
            angular_v_marker_.action = visualization_msgs::msg::Marker::DELETE;
            armors_marker_.action = visualization_msgs::msg::Marker::DELETE;
        }

        process_marker_array_.markers.emplace_back(position_marker_);
        process_marker_array_.markers.emplace_back(linear_v_marker_);
        process_marker_array_.markers.emplace_back(angular_v_marker_);
        process_marker_array_.markers.emplace_back(armors_marker_);
        process_marker_array_.markers.emplace_back(aiming_marker_);

        process_marker_pub_->publish(process_marker_array_);
    }

    void ProcesserNode::initParams()
    {
        this->debug_ = this->declare_parameter("debug", 1);

        this->controler_->lost_time_thres_ = this->declare_parameter("lost_time_thres", 0.0);
        this->controler_->s2qxyz_ = this->declare_parameter("s2qxyz", 1.0);
        this->controler_->s2qyaw_ = this->declare_parameter("s2qyaw", 0.5);
        this->controler_->s2qr_ = this->declare_parameter("s2qr", 0.2);
        this->controler_->r_xyz_factor = this->declare_parameter("r_xyz_factor", 1.2);
        this->controler_->r_yaw = this->declare_parameter("r_yaw", 0.3);

        this->controler_->delay_ = this->declare_parameter("delay", 0.1);
        this->controler_->true_x_ = this->declare_parameter("true_x", 640);

        this->controler_->gun_pitch_offset_ = this->declare_parameter("gun_pitch_offset", 0.0);
        this->controler_->gun_yaw_offset_ = this->declare_parameter("gun_yaw_offset", 0.0);
        this->controler_->rune_gun_pitch_offset_ = this->declare_parameter("rune_gun_pitch_offset", 0.0);
        this->controler_->rune_gun_yaw_offset_ = this->declare_parameter("rune_gun_yaw_offset", 0.0);

        this->controler_->bs_fly_time_ = this->declare_parameter("bs_fly_time", 0.0);
        this->controler_->ready_time_ = this->declare_parameter("ready_time", 0.0);

        this->controler_->ballistic_solver_.normal_ballistic_param_ = {
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

        this->controler_->ballistic_solver_.bullet_speed_ = this->declare_parameter("bullet_speed", 27);

        tool::TrackerParams tacker_params = {
            .max_match_distance_ = this->declare_parameter("max_match_distance", 0.5),
            .max_match_yaw_diff_ = this->declare_parameter("max_match_yaw_diff", 0.75),
            .tracking_thres = this->declare_parameter("tracking_thres", 5),
            .lost_thres = this->declare_parameter("lost_thres", 20),
        };
        this->controler_->tracker_.initParams(tacker_params);
    }
}

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rmos_processer::ProcesserNode)

