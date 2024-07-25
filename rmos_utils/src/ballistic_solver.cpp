#include "rmos_utils/ballistic_solver.hpp"

namespace tool
{
    BallisticSolver::BallisticSolver() {
        return;
    }

    BallisticSolver::~BallisticSolver() { return; }

    cv::Point3f BallisticSolver::getAngleTime(cv::Point3f position,bool is_rune)
    {

        this->setBS_coeff(position,is_rune);
        double dy, angle, y_actual;
        double t_actual = 0.0;
        double y_temp = position.z / 1000.0;
        double y = y_temp;
        double x = sqrt(position.x * position.x + position.y * position.y) / 1000.0;
        
        for (int i = 0; i < 40; i++) {
            angle = atan2(y_temp, x);
            // t_actual = (exp(this->normal_ballistic_param_.k * x) - 1.0) /
              //          (this->normal_ballistic_param_.k * this->bs_coeff * this->bullet_speed_ * cos(angle));
            t_actual=x/(this->bs_coeff_*this->bullet_speed_* cos(angle));
            y_actual = double( this->bs_coeff_*bullet_speed_ * sin(angle) * t_actual - this->rune_ballistic_param_.g * t_actual * t_actual / 2.0);
            dy = y - y_actual;
            y_temp += dy;
            if (abs(dy) < 0.001)
                break;
        }
        float pitch = (angle) / M_PI * 180.0;
        float yaw = atan2(position.y,position.x)/CV_PI*180.0;
        return cv::Point3f(pitch,yaw,t_actual);
    }



    bool BallisticSolver::setBulletSpeed(float bullet_speed)
    {
        this->bullet_speed_ = bullet_speed;
        return true;

    }

    void BallisticSolver::setBS_coeff(cv::Point3f position,bool is_rune)
    {
        if(is_rune == false) // 自瞄模式下的弹速系数调节
        {
            float distance = sqrt(position.x*position.x+
                                position.y*position.y+
                                position.z*position.z);
            // std::cout<<"distance:"<<distance<<std::endl;
            this->bs_coeff_ = normal_ballistic_param_.bs_coeff_first;
            if(distance >= normal_ballistic_param_.distance_first)
            {
                this->bs_coeff_ = normal_ballistic_param_.bs_coeff_second;
            }
            if(distance >= normal_ballistic_param_.distance_second)
            {
                this->bs_coeff_ = normal_ballistic_param_.bs_coeff_third;
            }
            // std::cout<<"bs_coeff_:"<<this->bs_coeff_<<std::endl;
        }
        if(is_rune == true) // 打符模式下弹速系数的调节
        {
                 bs_coeff_ = rune_ballistic_param_.level_first;
                if (position.z >= rune_ballistic_param_.height_first && position.z < rune_ballistic_param_.height_second)
                    bs_coeff_ = rune_ballistic_param_.level_second;
                else if(position.z >= rune_ballistic_param_.height_second && position.z < rune_ballistic_param_.height_third)
                    bs_coeff_ = rune_ballistic_param_.level_third;
                else if (position.z >= rune_ballistic_param_.height_third)
                    bs_coeff_ = rune_ballistic_param_.level_fourth;

                // std::cout<<"bs_coeff_:"<<this->bs_coeff_<<std::endl;
                // std::cout<<"position.z:"<<position.z<<std::endl;
        }

     }
}