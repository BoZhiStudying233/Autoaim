#include "rmos_rune/Fittool.h"
#include <chrono>

namespace RuneDetector
{
    bool Judgement::Judge(double& judge_speed, bool is_direction_inited, bool is_clockwise)
    {
        // 速度初筛选
        
        // if(speedJudge.size() < 15)
        // {
        //     speedJudge.push_back(judge_speed);
        //     judge_clear_num = 0;
        //     return true;
        // }//应该是改好了，待测试 

        // 方向未初始化
        if(!is_direction_inited)
        {
            if(fabs(judge_speed) > 2.7)
            {
                cout<<"bad data for no direction, or data is so big"<<endl;
                solveBadData(is_direction_inited);
                return false;
            }
        }
        
        else
        {
            // 顺时针情况
            if(is_clockwise)
            {
                if(judge_speed > 2.7 || judge_speed < -0.4)
                {
                    //cout<<"judge_speed="<<judge_speed<<endl;
                    cout<<"bad data for clockwise"<<endl;
                    solveBadData(is_direction_inited);
                    return false;
                }
            }

            // 逆时针情况
            else
            {
                if(judge_speed < -2.7 || judge_speed > 0.4) 
                {
                    cout<<"bad data for inv_clockwise"<<endl;
                    solveBadData(is_direction_inited);
                    return false;
                }
            }
        }

        // 3o筛选
        
        if(speedJudge.size() < 15)
        {
            speedJudge.push_back(judge_speed);
            judge_clear_num = 0;
            return true;
        }

        getN();
        getMean();
        getVariance();

        if(judge_speed > mean + 3*standard_deviation || judge_speed < mean - 3*standard_deviation)
        {
            solveBadData(is_direction_inited);
            return false;
        }
        
        // 确认为正确值

        speedJudge.push_back(judge_speed);

        while (speedJudge.size() > 15)
            speedJudge.erase(speedJudge.begin());
        
        judge_clear_num = 0;
        return true;

    }

    void Judgement::resetJudge()
    {
        judge_clear_num = 0;
        speedJudge.clear();
    }

    void Judgement::getN()
    {
        this->n = speedJudge.size();
    }

    void Judgement::getMean()
    {
        double sum = 0;
        for(int i = 0; i < n; i++)
            sum += this->speedJudge[i];
        this->mean = sum / n;
    }

    void Judgement::getVariance()
    {
        double square_sum = 0;
        for(int i = 0; i < n; i++)
            square_sum += pow(speedJudge[i], 2);
        this->variance = (square_sum - n*pow(mean,2)) / n;
        // 防止标准差为0,设置标准差恒为0.1
        if(this->variance < 0.01)
            this->variance = 0.01;
        if(this->variance > 0.04)
            this->variance = 0.04;
            
        this->standard_deviation = pow(this->variance, 0.5);
    }

    void Judgement::solveBadData(bool &is_direction_inited)
    {
        judge_clear_num++;
        if(judge_clear_num > 8)
        {
            judge_clear_num = 0;
            speedJudge.clear();
            is_direction_inited = false;
        }    
    }


    Fit::Fit()
    {


    }

    double Fit::run(vector<SpeedTime> Fittingdata, double N, base::Mode rune_mode)
    {   
        // 小符模式
        if(rune_mode == base::Mode::NORMAL_RUNE)
            return normal_rune_speed * delay_time;

        bool change_data = false;   // 拟合是否成功标志位
        double w_temp = this->w_min; 
        double w_best = this->w_min;
        double min_error = DBL_MAX;
        while(w_temp <= this->w_max)
        {
            Eigen::Vector3d output;
            double error;
            auto Fittime1 = std::chrono::steady_clock::now();
            if(fitting(Fittingdata, w_temp, output, error))
            {
                auto Fittime2 = std::chrono::steady_clock::now();
                // std::cout<<"time:"<<(Fittime2 - Fittime1).count()*1e-6<<std::endl;
                if(min_error > error)
                {
                    change_data = true;
                    min_error = error;
                    
                    this->start_time = Fittingdata[0].time;
                    this->w = w_temp;
                    this->P1 = output(0);
                    this->P2 = output(1);
                    this->P3 = output(2);                                        
                }
            }
            w_temp += this->dw;
        }

        // 没有成功拟合
        if(!change_data)
        {
            cout<<"fit error"<<endl;
            
            // 若数据总量小于最大拟合数量N,或start_time未初始化,则给予start_time初值
            if(Fittingdata.size() < N || this->start_time < 0)
                this->start_time = Fittingdata[0].time;
        }

        // 返回旋转角度
        
        double t = Fittingdata[Fittingdata.size() - 1].time - start_time;
        
        if(this->print_result)
            cout<< "w:" << w
             << "\t min_error:"<< min_error<<endl;   
        
        // solve_w();

        if(this->save_txt)
        {
            SpeedTime predict = predictSpeed(t);
            drawData(predict, Fittingdata[Fittingdata.size() - 1]);
        }

        return getRotateAngle(t);
    }

    void Fit::solve_w()
    {
        this->w_storage.push_back(w);
        if(this->w_storage.size()>100)
            this->w_storage.erase( this->w_storage.begin() );
        if(compare_w(this->w_storage) == 1)
        {
            this->w_max += 0.05;
            this->w_min += 0.05;
        }
        else if(compare_w(this->w_storage) == -1)
        {
            this->w_max -= 0.05;
            this->w_min -= 0.05;            
        }
        
    }
    int Fit::compare_w(std::vector<double> vec)//返回值为是否上调or下调
    {
        if(80 > vec.size()) return 0;
        for(int i = vec.size()-1;i>=vec.size() - 80;i--)
        {
            if (vec[i-1] != vec[i]) return 0;
        }
        if (vec.back() == this->w_max)
            return 1;
        else if (vec.back() == this->w_min)
            return -1;
        else return 0;
    }
    bool Fit::fitting(vector<SpeedTime> Fittingdata, double w, Eigen::Vector3d &output, double &error)
    {
        if(Fittingdata.empty())
            return false;

        // X,y 赋值
        Eigen::Matrix3d M = Eigen::Matrix3d::Zero();
        Eigen::Vector3d N = Eigen::Vector3d::Zero();
        for(int i = 0; i < Fittingdata.size(); i++)
        {
            double x1i = sin(w * (Fittingdata[i].time - Fittingdata[0].time));
            double x2i = cos(w * (Fittingdata[i].time - Fittingdata[0].time));
            Eigen::Vector3d Xi(x1i, x2i, 1);
            M += Xi * Xi.transpose();
            double yi = Fittingdata[i].angle_speed;
            N += Xi * yi;
        }
        // 判断M是否有逆矩阵;为0,矩阵不可逆
        if(M.determinant() == 0)
            return 0;
        output = M.inverse() * N;
        // cout<<"output:"<<output<<endl;

        // 计算误差
        double temp_error = 0.0;
        for(int i = 0; i < Fittingdata.size(); i++)
        {
            double x1i = sin(w * (Fittingdata[i].time - Fittingdata[0].time));
            double x2i = cos(w * (Fittingdata[i].time - Fittingdata[0].time));           
            
            double yi = Fittingdata[i].angle_speed;
            double temp = x1i*output(0) + x2i*output(1) + output(2) - yi;
            temp_error += pow(temp,2);
        }
        error = temp_error;
        return true;
    }

    double Fit::getRotateAngle(double t)
    {
        double temp1 = -(P1/w)*(cos(w*(t+delay_time))-cos(w*t));
        double temp2 = (P2/w)*(sin(w*(t+delay_time))-sin(w*t)); 
        double temp3 = P3 * delay_time;       
        double rotate_angle = fabs(temp1 + temp2 + temp3);
        return rotate_angle;
    }

    SpeedTime Fit::predictSpeed(double t)
    {
        double speed = P1*sin(w*(t+delay_time)) + P2*cos(w*(t+delay_time)) + P3;
        double time = start_time + t + delay_time;
        return SpeedTime(speed, time);
    }  

    void Fit::drawData(SpeedTime predict, SpeedTime now)
    {        
        if(!have_file_count)
        {
            countFilesInDirectory();
            have_file_count = true;
        }

        if(!have_first_time)
        {
            first_time = min(predict.time, now.time);
            have_first_time = true;
        }

        txt.open(filename, ios::app);
        if (txt.is_open()) 
        {
            txt << predict.angle_speed<< " " << predict.time - first_time << " " << now.angle_speed<< " " << now.time - first_time << endl;
            txt.close();
        }
        else
        {
            std::cerr << "Unable to open the file.";
        }        
    } 

    void Fit::countFilesInDirectory()
    {
        this->fileCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path)) 
        {
            if (entry.is_regular_file() || entry.is_directory()) {
                this->fileCount++;
            }
        }
        // 创建文件
        this->filename = path + to_string(fileCount) + ".txt";
    }

}