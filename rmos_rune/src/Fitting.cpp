#include "rmos_rune/Fitting.h"
#include <cmath>

#include <tf2_eigen/tf2_eigen.h>

#include <tf2/transform_datatypes.h>
#include <tf2/exceptions.h>


#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <tf2/convert.h>
#include <tf2_eigen/tf2_eigen.h>
namespace RuneDetector
{

    /*-----------Fitting-----------*/
    Fitting::Fitting()
    {

    }

    bool Fitting::run(base::RuneArmor armor_1,vector<cv::Point2f> &nextPosition, Eigen::Vector3d &tVector, TrackState armor_state, base::Mode rune_mode, vector<base::RuneArmor>& rune_armors, Mat camera_matrix, Mat dist_coeffs, geometry_msgs::msg::TransformStamped transform_to_world, geometry_msgs::msg::TransformStamped transform_to_camera)
    {
        // 数据处理
        if (!processDataState(armor_1, armor_state))
            return false;

        // 确定方向
        initDirection();
        if(!is_direction_inited)
            return false;

        // 对于大符模式，Fitting_data超过一定数量才能进行拟合
        if(rune_mode == base::Mode::RUNE && fitting_data.size() < N_min)
            return false;

        nextPosition.clear();
        vector<cv::Point2f> pts;
        armor_1.getPoints(pts);

        double delta = fit.run(fitting_data, N, rune_mode);            // 旋转角度
        //delta = 0;

        if (this->save_txt == 1)
        {
            double now_angle = armor_1.angle;
            double now_time = armor_1.timestamp;
            double predict_angle = now_angle + delta;
            double predict_time = now_time + fit.delay_time;
            
            drawAngle(now_angle, now_time, predict_angle, predict_time);
        }
        
        //圆拟合
        
        // getTrajData(armor_buffer, camera_matrix, dist_coeffs, transform_to_world, transform_to_camera);
        
        // this->armor_pose_points.clear();
        // this->angle_points.clear();


        // for(int i=0;i<360;i++)
        // {
        //     if(watched_points[i].is_get==true)
        //     {
        //         this->armor_pose_points.push_back(watched_points[i].point);
        //         angle_points.push_back(i);
        //     }
        // }
        // cout<<"点数为"<<armor_pose_points.size()<<std::endl;
        // // correctPoints(armor_pose_points)     此函数内的深度考虑改为[2]后测试
        // buff_trajectory = fitCircle(armor_pose_points, transform_to_world, transform_to_camera);
        // //tVector = watched_points[round(armor_1.angle+delta)].point.x,watched_points[round(armor_1.angle+delta)].point.y,watched_points[round(armor_1.angle+delta)].point.z
        // if(buff_trajectory.is_get)
        // {
        //     // correctAxis(buff_trajectory, armor_pose_points, angle_points);   应该不再需要correct了
        //     // std::cout<<"buff_trajectory.x_axis="<<buff_trajectory.x_axis<<std::endl;
        //     // std::cout<<"buff_trajectory.radius="<<buff_trajectory.radius<<std::endl;
        //     // Eigen::Vector3d Vec1 = tfPoint(transform_to_world, buff_trajectory.x_axis);
        //     // std::cout<<"1="<<Vec1<<std::endl;
        //     // std::cout<<"2="<<tfPoint(transform_to_camera, Vec1)<<std::endl;
        //     //std::cout<<"buff_trajectory.y_axis * sin(armor_1.angle+delta)="<<buff_trajectory.radius * buff_trajectory.y_axis * sin(armor_1.angle+delta)+buff_trajectory.center<<std::endl;
        //     tVector = buff_trajectory.radius * buff_trajectory.x_axis * cos(armor_1.angle+delta) + buff_trajectory.radius * buff_trajectory.y_axis * sin(armor_1.angle+delta) + buff_trajectory.center;
        //     tVector = tfPoint(transform_to_camera, tVector);
        //     std::cout<<"tVector="<<tVector<<std::endl;
        // }
        // else 
            for (int i = 0; i < 4; i++)
                nextPosition.push_back(calNextPosition(pts[i], armor_1.circle_center, delta));

        
        
        
        return true;         
    }


    void Fitting::correctPoints(std::vector<Eigen::Vector3d> &armor_points)
    {
        if (armor_points.size() <= 0)
        {
            return;
        }
        float depth_aver = 0.0f;
        std::for_each(armor_points.begin(), armor_points.end(), [&](const Eigen::Vector3d &d)
                      { depth_aver += d[1]; });
        depth_aver /= armor_points.size();

        float depth_std = 0.0;

        std::for_each(armor_points.begin(), armor_points.end(), [&](const Eigen::Vector3d &d)
                      { depth_std += pow((d[1] - depth_aver), 2); });
        depth_std = sqrt(depth_std / armor_points.size());

        std::for_each(
            armor_points.begin(), armor_points.end(), [&](Eigen::Vector3d &armor_point)
            {
                if (armor_point[1] < depth_aver - 0.5 * depth_std || armor_point[1] > depth_aver + 0.5 * depth_std)
                {
                    armor_point = armor_point / armor_point[1] * depth_aver;
                }
            });
    }

    void Fitting::drawAngle(double now_angle, double now_time, double predict_angle, double predict_time)
    {
         if(!have_file_count)
        {
            countFilesInDirectory();
            have_file_count = true;
        }

        if(!have_first_time)
        {
            first_time = min(now_time, predict_time);
            have_first_time = true;
        }

        txt.open(filename, ios::app);
        if (txt.is_open()) 
        {
            txt << predict_angle << " " << predict_time - first_time << " " << now_angle<< " " << now_time - first_time<< endl;
            txt.close();
        }
        else
        {
            std::cerr << "Unable to open the file!";
        }        
    }
    void Fitting::countFilesInDirectory()
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
    // void Fitting::correctAxis(BuffTrajectory &buff_traj, const std::vector<Eigen::Vector3d> &armor_points, const std::vector<float> &angle_points)
    // {

    //     int axis_score[2][2] = {0}; //0:x,1:y 0:原，1:反
    //     Eigen::Vector3d axis_set[2][2] = {buff_traj.x_axis, -buff_traj.x_axis, buff_traj.y_axis, -buff_traj.y_axis};
    //     int index_x, index_y;
    //     for (int k = 0; k < armor_points.size(); k++)
    //     {
    //         index_x = 0;
    //         index_y = 0;
    //         double min_dis = 999.0;
    //         for (int i = 0; i < 2; i++)
    //         {
    //             for (int j = 0; j < 2; j++)
    //             {
    //                 Eigen::Vector3d predict_point = buff_traj.center + axis_set[0][i] * buff_traj.radius * cos(angle_points[k]*CV_PI/180) + axis_set[1][j] * buff_traj.radius * sin(angle_points[k]*CV_PI/180);
    //                 if ((predict_point - armor_points[k]).norm() < min_dis)
    //                 {
    //                     index_x = i;
    //                     index_y = j;
    //                     min_dis = (predict_point - armor_points[k]).norm();
    //                 }
    //             }
    //         }
    //         axis_score[index_x][index_y] += 1;  
    //     }
    //     if (axis_score[0][0] < axis_score[0][1])
    //     {
    //         buff_traj.x_axis *= -1;
    //     }
    //     if (axis_score[1][0] < axis_score[1][1])
    //     {
    //         buff_traj.y_axis *= -1;
    //     }      
    // }
    const BuffTrajectory &Fitting::fitCircle(const std::vector<Eigen::Vector3d> &armor_points, geometry_msgs::msg::TransformStamped transform_to_world, geometry_msgs::msg::TransformStamped transform_to_camera)
    {
        const int CURVE_FIT_SIZE = armor_points.size();//remeber to change it
        buff_trajectory.is_get = false;
        if (armor_points.size() <= this->Points_num)
        {
            cout<<"armor_points.size()="<<armor_points.size()<<"   点数过少，无法拟合三维圆"<<std::endl;
            return buff_trajectory;
        }
        cout<<"armor_points.size()="<<armor_points.size()<<endl;
        buff_trajectory.is_get = true;
        Eigen::MatrixXd M(CURVE_FIT_SIZE, 3);

        for (int i = 0; i < CURVE_FIT_SIZE; i++)
        { 
            for (int j = 0; j < 3; j++)
            {
                M(i, j) = armor_points[i][j];
            }
        }

        Eigen::MatrixXd  L1 = Eigen::MatrixXd::Ones(CURVE_FIT_SIZE, 1);
        
        Eigen::Vector3d A = (M.transpose() * M).inverse() * M.transpose() * L1; //平面法向量



        //计算两点之间的向量
        Eigen::MatrixXd B((CURVE_FIT_SIZE - 1) * CURVE_FIT_SIZE / 2, 3);
        int count = 0;
        for (int i = 0; i < CURVE_FIT_SIZE - 1; i++)
        {
            for (int j = i + 1; j < CURVE_FIT_SIZE; j++)
            {
                B.row(count) = M.row(j) - M.row(i);
                count++;
            }
        }
        // //求取两两平方差
        count = 0;
        Eigen::MatrixXd  L2((CURVE_FIT_SIZE - 1) * CURVE_FIT_SIZE / 2, 1);
        for (int i = 0; i < CURVE_FIT_SIZE - 1; i++)
        {
            for (int j = i + 1; j < CURVE_FIT_SIZE; j++)
            {
                L2(count, 0) = (pow(M.row(j).norm(), 2) - pow(M.row(i).norm(), 2)) / 2;
                count++;
            }
        }

        // Eigen::Matrix<double, 3, 3> matrix = B.transpose() * B;
        //get D Matrix
        Eigen::Matrix<double, 4, 4> D;

        D << B.transpose() * B, A, A.transpose(), 0;

        //get L3 Matrix
        Eigen::Vector4d L3;
        L3 << B.transpose() * L2, 1;

        // //式（7）
        Eigen::Vector4d C = D.transpose().inverse() * L3;

        Eigen::Vector3d center(C[0], C[1], C[2]);

        // // //式（8）
        float radius = 0;
        for (int i = 0; i < CURVE_FIT_SIZE; i++)
        {
            radius += (armor_points[i] - center).norm();
        }
        radius = radius / CURVE_FIT_SIZE;
        
        Eigen::Vector3d y_axis = A.cross(tfPoint(transform_to_world,Eigen::Vector3d(1, 0, 0)));
        Eigen::Vector3d x_axis = A.cross(y_axis);

        x_axis = x_axis / x_axis.norm();
        y_axis = y_axis / y_axis.norm();

        buff_trajectory.radius = radius;
        buff_trajectory.center = center;
        buff_trajectory.x_axis = x_axis;
        buff_trajectory.y_axis = y_axis;
        Eigen::Vector3d VecY = tfPoint(transform_to_camera, y_axis);
        Eigen::Vector3d VecX = tfPoint(transform_to_camera, x_axis);
        
        if (VecX[0] < 0)
        {
            buff_trajectory.x_axis = -buff_trajectory.x_axis;
        }
        if (VecY[1] < 0)
        {
            buff_trajectory.y_axis = -buff_trajectory.y_axis;
        }
        return buff_trajectory;
    }

    void Fitting::clearData()
    {
        cout << "Clear Fitting Data!" << endl;
        fitting_data.clear();
        armor_buffer.clear();
        for(int i=0; i<360; i++)
        {
            watched_points[i].is_get = false;
        }
        judge.resetJudge();
        is_direction_inited = false;
    }

    cv::Point2f Fitting::calNextPosition(cv::Point2f point, cv::Point2f org, float rotate_angle)
    {
        double radius = calDistance(point, org);
        cv::Point2f relative_point = point - org;                                         // 相对坐标
        double relative_angle = atan2(relative_point.y, relative_point.x);                // 与圆心所成角度
        double next_angle;

        if (is_clockwise) // 顺时针运动
        {
            next_angle = relative_angle + rotate_angle;
            if (next_angle > CV_PI)
                next_angle -= 2.0 * CV_PI;
        }
        else
        {
            next_angle = relative_angle - rotate_angle;
            if (next_angle < - CV_PI)
                next_angle += 2.0 * CV_PI;
        }
        return cv::Point2f(cos(next_angle) * radius, sin(next_angle) * radius) + org;
    }

    bool Fitting::processDataState(RuneArmor armor_1, TrackState armor_state)
    {
        switch (armor_state)
        {
        case base::TrackState::DETECTING:
            clearArmorBuffer();
            armor_buffer.push_back(armor_1);
            break;

        case base::TrackState::TRACKING:
            armor_buffer.push_back(armor_1);
            while (armor_buffer.size() > 1 + DN)
            {
                double delta_time = armor_1.timestamp - armor_buffer[0].timestamp;
                if (delta_time > armor_buffer_erase_threshold)
                    armor_buffer.erase(armor_buffer.begin());
                else if (delta_time > call_speed_threshold)
                {
                    double temp_speed = calAngleSpeed(armor_1, armor_buffer[armor_buffer.size() - 1 - DN]);
                    if(judge.Judge(temp_speed, is_direction_inited, is_clockwise))
                    {
                        SpeedTime temp_state = SpeedTime(temp_speed, (armor_1.timestamp + armor_buffer[armor_buffer.size() - 1 - DN].timestamp) / 2);
                        pushFittingData(temp_state);
                    }
                    break;
                }
                else
                    break;
            }

            break;

        case TrackState::LOST:
            clearData();
            return false;
            break;
            
        default:
            return false;
            break;
        }
        while (fitting_data.size() > N)
            fitting_data.erase(fitting_data.begin());
            
        return true;
    }

    void Fitting::getTrajData(vector<RuneArmor> armor_buffer, Mat camera_matrix, Mat dist_coeffs, geometry_msgs::msg::TransformStamped transform_to_world, geometry_msgs::msg::TransformStamped transform_to_camera)
    {
        static float k = 0.85; //低通滤波参数
        int degree;
        for (int i=0; i < armor_buffer.size();i++)
        {   
            degree = armor_buffer[i].angle/M_PI*180;
            if(watched_points[degree].is_get == false)
            {

                watched_points[degree].is_get = true;

                cv::Mat tVec;
                cv::Mat rVec;
                vector<Point2f> PoiVec;
                for(int p=0;p<4;p++)
                {
                    PoiVec.push_back(armor_buffer[i].points[p]);
                    
                }
                
                bool is_solve = this -> pnp_solver_ ->solveRuneArmorPose(PoiVec,camera_matrix,dist_coeffs,tVec,rVec);
                
                if(!is_solve)
                {
                    cout<<"camera2 param empty"<<endl;
                }
                armor_buffer[i].target = cv::Point3f(tVec.at<double>(0, 0), tVec.at<double>(1, 0),tVec.at<double>(2, 0));
                
                Eigen::Vector3d target_point_in_camera = Eigen::Vector3d(armor_buffer[i].target.x,armor_buffer[i].target.y,armor_buffer[i].target.z);
                Eigen::Vector3d target_point_in_world = tfPoint(transform_to_world, target_point_in_camera);
                watched_points[degree].point =  target_point_in_world;
            }
            else if (watched_points[degree].is_get == true)
            {
                cv::Mat tVec;
                cv::Mat rVec;
                vector<Point2f> PoiVec;
                for(int p=0;p<4;p++)
                {
                    PoiVec.push_back(armor_buffer[i].points[p]);
                }
                bool is_solve = this -> pnp_solver_ ->solveRuneArmorPose(PoiVec,camera_matrix,dist_coeffs,tVec,rVec);

                if(!is_solve)
                {
                    cout<<"camera3 param empty"<<endl;
                }
                armor_buffer[i].target = cv::Point3f(tVec.at<double>(0, 0), tVec.at<double>(1, 0),tVec.at<double>(2, 0));
                Eigen::Vector3d target_point_in_camera = Eigen::Vector3d(armor_buffer[i].target.x,armor_buffer[i].target.y,armor_buffer[i].target.z);
                Eigen::Vector3d target_point_in_world = tfPoint(transform_to_world, target_point_in_camera);
                watched_points[degree].point = watched_points[degree].point * k + (1 - k) *  target_point_in_world;
            }
            // std::cout<<"watched_points[degree].point ="<<watched_points[degree].point<<std::endl;
            // std::cout<<"degree="<<degree<<std::endl;
            // std::cout<<"watched_points[degree].point ="<<tfPoint(transform_to_camera, watched_points[degree].point)<<std::endl;
        }
        
    }

    Eigen::Vector3d Fitting::tfPoint(geometry_msgs::msg::TransformStamped transform_to, Eigen::Vector3d Vec)
    {
        Eigen::Vector3d translation(
            transform_to.transform.translation.x,
            transform_to.transform.translation.y,
            transform_to.transform.translation.z
        );

        // 提取旋转分量
        Eigen::Quaterniond rotation(
            transform_to.transform.rotation.w,
            transform_to.transform.rotation.x,
            transform_to.transform.rotation.y,
            transform_to.transform.rotation.z
        );

        // 将旋转分量转换为旋转矩阵
        Eigen::Matrix3d rotation_matrix = rotation.toRotationMatrix();

        // 计算转换后的向量
        Eigen::Vector3d target_transformed = rotation_matrix * Vec + translation;

        // 现在Vec_world包含了转换后的Eigen::Vector3d
        return target_transformed;
    }
    void Fitting::pushFittingData(SpeedTime new_data)
    {
        if (fitting_data.empty())
        {
            fitting_data.push_back(new_data);
            return;
        }
        SpeedTime flag_data = fitting_data[fitting_data.size() - 1];
        
        double n = new_data.time - flag_data.time;

        // 时间差过长则清除数据;0.8为超参数,可调 
        if (n > clear_data_threshold)
        {
            
            clearData();
            return;
        }

        fitting_data.push_back(new_data);

        // 去除线性插值
        // if ((double)new_data.time - (double)flag_data.time - DT < 0)
        // {
        //     return;
        // }

        // double T = DT;

        // for (int i = 0; i < (int)n; i++)
        // {
        //     double temp_T = T * (i + 1);
        //     double delta = (double)new_data.time - (double)flag_data.time - temp_T;
        //     SpeedTime temp = SpeedTime(new_data.angle_speed * (temp_T / (temp_T + delta)) + flag_data.angle_speed * (delta / (temp_T + delta)), flag_data.time + (double)temp_T);  
        //     fitting_data.push_back(temp);  
        // }
    }

    void Fitting::initDirection()
    {
        if (fitting_data.size() < N_min)
            return;

        int clock = 0, clock_inv = 0;
        for (int i = 0; i < fitting_data.size(); i++)
        {
            //std::cout<<"fitting_data["<<i<<"].angle_speed="<<fitting_data[i].angle_speed<<std::endl;
            if (fitting_data[i].angle_speed > 0)         
                clock++;
            else
                clock_inv++;
        }
        is_direction_inited = true;
        is_clockwise = clock > clock_inv;
        
    }

    void Fitting::clearArmorBuffer()
    {
        armor_buffer.clear();
        judge.resetJudge();
    }

    double Fitting::calAngleSpeed(RuneArmor armor_1, RuneArmor armor_2)
    {
        double time_diff = (double)(armor_1.timestamp - armor_2.timestamp);

        if(time_diff < 0.005)
            time_diff += 0.005;//防止时间差过小导致除完之后速度过大

        double angle_diff = armor_1.angle - armor_2.angle;
        if (armor_1.angle < -CV_PI / 2.0 && armor_2.angle > CV_PI / 2.0)
            angle_diff = angle_diff + CV_PI * 2.0;
        else if (armor_1.angle > CV_PI / 2.0 && armor_2.angle < -CV_PI / 2.0)
            angle_diff = angle_diff - CV_PI * 2.0;
        
        return angle_diff / time_diff; // 转换单位
    }

}
