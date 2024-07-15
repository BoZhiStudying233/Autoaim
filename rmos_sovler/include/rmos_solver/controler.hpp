#ifndef RMOS_CONTROLER_HPP
#define RMOS_CONTROLER_HPP

#include <iostream>

// OpenCV
#include <opencv2/core.hpp>
#include <queue>

// Eigen
#include <Eigen/Eigen>

#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Quaternion.h>

#include "rmos_utils/armor.hpp"
#include "rmos_utils/tracker.hpp"
#include "ballistic_solver.hpp"
#include "rmos_interfaces/msg/quaternion_time.hpp"
#include "rmos_interfaces/msg/target.hpp"

namespace solver
{
    class Controler {
    public:

        bool judgeRuneFire(int num_id,uint32_t timestamp);

        bool getParam(cv::Mat camera_matrix);

        BallisticSolver ballistic_solver_;
        
        double rune_gun_pitch_offset_;
        double rune_gun_yaw_offset_;

        cv::Mat camera_matrix_;

        float gun_pitch_offset ;
        float gun_yaw_offset ;

        //大符开火决策参数
        int last_id_{-1};
        double bs_fly_time_{0};
        double ready_time_{0};
        double vane_change_timestamp_{0};
        double last_fire_timestamp_{0};

        bool is_tracking_ = false;
        cv::Point3f aiming_point_;
        cv::Point3f center_point_;
    };
}




#endif //RMOS_CONTROLER_HPP
