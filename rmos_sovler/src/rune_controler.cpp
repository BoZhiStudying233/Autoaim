#include "rmos_solver/rune_controler.hpp"

namespace solver
{
    

    bool Controler::getParam(cv::Mat camera_matrix)
    {
        this->camera_matrix_ = camera_matrix;
        if(camera_matrix.empty())
        {
            std::cout<<"camera matrix while get param empty"<<std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }

    bool Controler::judgeRuneFire(int num_id, uint32_t timestamp)
    {
        // 开火决策
        // 1.判断扇叶切换
        if(num_id != last_id_)
        {
            this->vane_change_timestamp_ = timestamp;
            last_id_ = num_id;
        }
        // 2.时间差判断是否给予开火权,注意陀螺仪时间戳单位为ms;2500ms后切换扇叶
        uint32_t delta_vane_change_timestamp = timestamp - this->vane_change_timestamp_;
        uint32_t delta_fire_timestamp = timestamp - this->last_fire_timestamp_;

        if(delta_vane_change_timestamp > this->ready_time_ && 
            delta_fire_timestamp > bs_fly_time_)
        {
            last_fire_timestamp_ = timestamp;
            return true;
        }
        else
        {
            return false;
        }
    }

    // Controler::~Controler()
    // {
    //     return;
    // }
}