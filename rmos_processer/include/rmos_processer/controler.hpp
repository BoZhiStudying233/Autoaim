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

namespace processer
{
    class Controler {
    public:
        Controler();
        ~Controler();

        /**
         * @brief   得到最优瞄准点
         * @param[in]   armors detector得到的装甲板序列
         * @param[in out]   aiming_point 目标击打点
         * @param[in]   timestamp imu的时间戳
         * @param[in out] gun_aim_point 目标车辆中心点
         * @return  待击打目标状态
        **/
        rmos_interfaces::msg::Target getTarget(std::queue<rmos_interfaces::msg::QuaternionTime> quaternion_buf_);

        int getAimingPoint(cv::Point3f& aiming_point, cv::Point3f& gun_aim_point);// 1:move 2:slow_move 3:stop

        void updateTracker(std::vector<base::Armor> armors, double timestamp);

        /**
         * @brief   重投影回图像坐标系判断是否开火
         * @param[in]   aimming_point_camera 在2d图像系下待击打点坐标
         * @param[in]   v_yaw 目标角速度
         * @return  是否开火
        **/
        bool judgeFire(cv::Point3f aimming_point_camera, double v_yaw);

        bool judgeRuneFire(int num_id,uint32_t timestamp);

        bool getParam(cv::Mat camera_matrix);

        BallisticSolver ballistic_solver_;
        tool::Tracker tracker_;
        
        // 枪管偏置
        double gun_pitch_offset_;
        double gun_yaw_offset_;
        double rune_gun_pitch_offset_;
        double rune_gun_yaw_offset_;

        double dt_{0};
        double lost_time_thres_;
        cv::Mat camera_matrix_;

        double s2qxyz_{0};
        double s2qyaw_{0};
        double s2qr_{0};
        double r_xyz_factor{0};
        double r_yaw{0};

        int true_x_;

        double last_time_ {0};
        double delay_{0};

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
