//
// Created by nuc12 on 23-7-18.
//

#include "Processer/ballistic_solver.hpp"


namespace processer
{
    BallisticSolver::BallisticSolver() {
        cv::FileStorage fs1("./src/Algorithm/configure/Processer/ballistic_solver/normal_params.xml", cv::FileStorage::READ);

        if (!fs1.isOpened()) {
            std::cout << "open processer normal_ballistic_solver param fail" << std::endl;
            exit(0);
        }

        fs1["level_one_first"] >> normal_ballistic_param_.level_one_first;
        fs1["level_two_first"] >> normal_ballistic_param_.level_two_first;
        fs1["level_two_second"] >> normal_ballistic_param_.level_two_second;
        fs1["level_three_first"] >> normal_ballistic_param_.level_three_first;
        fs1["level_three_second"] >> normal_ballistic_param_.level_three_second;
        fs1["level_three_third"] >> normal_ballistic_param_.level_three_third;
        fs1["k"] >> normal_ballistic_param_.k;
        fs1["g"] >> normal_ballistic_param_.g;
        fs1["bullet_speed"] >> bullet_speed_;

        
        fs1.release();
        
        cv::FileStorage fs2("./src/Algorithm/configure/Processer/ballistic_solver/rune_params.xml", cv::FileStorage::READ);

                if (!fs2.isOpened()) {
            std::cout << "open processer rune_ballistic_solver param fail" << std::endl;
            exit(0);
        }
        
        fs2["level_first"] >> rune_ballistic_param_.level_first;
        fs2["level_second"] >> rune_ballistic_param_.level_second;
        fs2["level_third"] >> rune_ballistic_param_.level_third;
        fs2["level_fourth"] >> rune_ballistic_param_.level_fourth;

        fs2.release();
    }

    cv::Point3f BallisticSolver::getAngleTime(cv::Point3f position,bool is_rune)
    {

        this->setBS_coeff(position,is_rune);
        double dy, angle, y_actual;
        double t_actual = 0.0;
        double y_temp = position.z / 1000.0;
        double y = y_temp;
        double x = sqrt(position.x * position.x + position.y * position.y) / 1000.0;

        bullet_speed_ = 30.0;

        for (int i = 0; i < 40; i++) {
            angle = atan2(y_temp, x);
            // t_actual = (exp(this->normal_ballistic_param_.k * x) - 1.0) /
              //          (this->normal_ballistic_param_.k * this->bs_coeff * this->bullet_speed_ * cos(angle));
                    t_actual=x/(this->bs_coeff*this->bullet_speed_* cos(angle));
            y_actual = double( this->bs_coeff*bullet_speed_ * sin(angle) * t_actual - this->normal_ballistic_param_.g * t_actual * t_actual / 2.0);
            dy = y - y_actual;
            y_temp += dy;
            if (abs(dy) < 0.001)
                break;
        }

        float pitch = (angle) / M_PI * 180.0;
        float yaw = atan2(position.y,position.x)/CV_PI*180.0;

        return cv::Point3f(pitch,yaw,t_actual);

    }



    bool BallisticSolver::setBulletSpeed(int bullet_speed)
    {
        this->bullet_speed_ = bullet_speed;
        return true;

    }

    void BallisticSolver::setBS_coeff(cv::Point3f position,bool is_rune)
    {
        if(is_rune == false)//自瞄模式下的弹速系数调节
        {
            float distance = sqrt(position.x*position.x+
                                position.y*position.y+
                                position.z*position.z);

                bs_coeff= 0.88;
            // if(position.z>0.5)
            // {
            //     bs_coeff= 0.78;
            //     if(distance>5500)bs_coeff*= 1.05;
            //     if(distance>6700)bs_coeff*= 1.035;
                
                    
            //}
            // else
            // {
                // bs_coeff = 0.675;
                    // if(distance>2500)bs_coeff*=1.25;
                    // if(distance>4800)bs_coeff*=1.07;
                    if(distance>6200)bs_coeff*=0.92;
                    if(distance>6850)bs_coeff*=1.06;
                    //因自瞄调试可能还会改代码，因此此处参数暂不拉到xml文件里。

        // }
        }
        if(is_rune == true)//打符模式下弹速系数的调节
        {
                 bs_coeff = rune_ballistic_param_.level_first;
                if (position.z >= 400 && position.z < 900)
                    bs_coeff = rune_ballistic_param_.level_second;
                else if(position.z >= 900 && position.z < 1200)
                    bs_coeff = rune_ballistic_param_.level_third;
                else if (position.z >= 1200)
                    bs_coeff = rune_ballistic_param_.level_fourth;
                

        }

     }
}