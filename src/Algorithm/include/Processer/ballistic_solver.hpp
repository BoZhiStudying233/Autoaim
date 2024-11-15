//
// Created by Wang on 23-7-18.
//

#ifndef RMOS_BALLISTIC_SOLVER_HPP
#define RMOS_BALLISTIC_SOLVER_HPP

#include "processer_interfaces/ballistic_solver_interface.hpp"
#include <iostream>

namespace processer
{
    struct NormalBallisticParam
    {
        // 补偿系数
        float bs_coeff_first;
        float bs_coeff_second;
        float bs_coeff_third;
        float bs_coeff_fourth;

        float distance_first;
        float distance_second;
        float distance_third;
        double k;   // 空气阻力系数/质量
        double g;   // 重力加速度
    };

    struct RuneBallisticParam
    {
        //打符补偿系数参数
        float level_first;
        float level_second;
        float level_third;
        float level_fourth;

        float height_first;
        float height_second;
        float height_third;
        // float height_fourth;
    };
    
    class BallisticSolver :  BallisticSolverInterface
    {
    public:
        BallisticSolver();
        ~BallisticSolver();
        /**
         * @brief 解算到3d目标点的枪口角度和子弹飞行时间
         * @param[in]   position 世界系下3d目标点
         * @param[in]   is_rune 是否为能量机关击打模式
         * @return 返回枪口对应pitch、yaw、子弹飞行时间
        **/
        cv::Point3f getAngleTime(cv::Point3f position, bool is_rune) override;
        /**
         * @brief  设置弹速
         * @param[in] bullet_speed 弹速
         * @return 是否设置成功
        **/
        bool setBulletSpeed(int bullet_speed)override;

        int bullet_speed_;
    private:
        void setBS_coeff(cv::Point3f position, bool is_rune);

        NormalBallisticParam normal_ballistic_param_;
        RuneBallisticParam rune_ballistic_param_;

        double bs_coeff_;        // 弹速系数
    };
}













#endif //RMOS_BALLISTIC_SOLVER_HPP
