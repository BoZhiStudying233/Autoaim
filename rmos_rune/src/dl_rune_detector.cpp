#include "rmos_rune/rune_detector.hpp"


namespace RuneDetector
{
    DlRuneDetector::DlRuneDetector(RuneParam params)
    {
            //~~~
        rt.init("./rmos_utils/AIOF-IP/config/rtc_rm_v7_rune.yaml");
        std::cout<<rt.init_messages()<<std::endl;
        
        this->state = base::TrackState::LOST;

        
        this->param.show_R = params.show_R;
        this->param.blue_brightness_thresh = params.blue_brightness_thresh;
        this->param.red_brightness_thresh = params.red_brightness_thresh;
        this->param.red_color_thresh = params.red_color_thresh;
        this->param.blue_color_thresh = params.blue_color_thresh;
        this->param.blue_red_diff = params.blue_red_diff;
        this->param.show_bin = params.show_bin;

        this->param.circle_center_conf = params.circle_center_conf;
        this->param.no_activate_conf = params.no_activate_conf;
        this->param.activate_conf = params.activate_conf; 
        this->param.circle_center_roi_width = params.circle_center_roi_width;
        this->param.max_diff_distance_ratio = params.max_diff_distance_ratio;

        this->param.high_threshold = params.high_threshold;
        this->param.low_threshold = params.low_threshold;
        this->param.tell_area = params.tell_area;



        this->param.delay_time = params.delay_time;
        this->param.save_txt = params.save_txt;
        this->param.print_result = params.print_result;

    }

    DlRuneDetector::~DlRuneDetector(){}

    bool DlRuneDetector::setEnemyColor(int enemy_color)
    {
        if (enemy_color == 0)
        {
            this->enemy_color_ = base::Color::RED;
            return true;
        }
        else if (enemy_color == 1)
        {
            this->enemy_color_ = base::Color::BLUE;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool DlRuneDetector::DlRuneDetect(Mat& image,base::RuneArmor& target_rune_armor, vector<base::RuneArmor>& rune_armors)//第三个参数存储所有识别到的扇叶，获得点来拟合三维圆
    {
        // 设置网络模型            
        std::vector<DetectResultList> outputs;
        std::vector<cv::Mat> inputs;
        std::vector<cv::Mat> result_mats;
        outputs.clear();
        inputs.clear();
        result_mats.clear();
        inputs.push_back(image);
        rt.run(inputs, outputs);
        std::vector<DetectResult> output = outputs[0];
        // std::cout<<"size:"<<output.size()<<std::endl;
        image.copyTo(src);
        DlPreDeal(image);
                
        drawDlMessage(image, output);

        FilterDetectResult(output);

        if(classifer(output, rune_armors))
        {
            //用于找到二维圆的圆心
            
            DlDetectCircleCenter();
            this->target.circle_center = circle_center;
            
            DlDetectRuneArmor();

            drawTargetMessage(image);

            setFoundState();

            target_rune_armor = this->target;
            last_target = this->target;

            return true;
        }

        if (lost_times < lost_num && state != base::TrackState::LOST)
        {
            state = base::TrackState::TEMP_LOST;
            lost_times++;
            std::cout << "Lost Times: " << lost_times << std::endl;
        }
        else
        {
            lost_times = 0;
            state = base::TrackState::LOST;
        }

        return false;
    }

    bool DlRuneDetector::ifOldArmor()
    {
        // 通过距离判断是否为旧符
        float max_diff_distance_ratio= calDistance(target.armor_center, last_target.armor_center) / calDistance(target.armor_center, target.circle_center);

        
        if (max_diff_distance_ratio > param.max_diff_distance_ratio)
        {
            this->vane_change_count++;
            if(this->vane_change_count < this->vane_change_threshold)
            {
                this->target = this->last_target;
                return true;
            }
            this->vane_change_count = 0;
            cout<<"vane is changing!"<<endl;
            this->id++;
            // std::cin.get();
            return false;
        }

        return true;
    }

    void DlRuneDetector::setFoundState()
    {
        // the first for finding
        if(state == base::TrackState::LOST)
            state = base::TrackState::DETECTING;
        
        else
        {
            // the vane is changing
            if(!ifOldArmor())
                state = base::TrackState::DETECTING;
            else
            {
                if(lost_times < 5)
                    state = base::TrackState::TRACKING;
                else
                    state = base::TrackState::DETECTING;    
            } 
        }

        lost_times = 0;
    }

    void DlRuneDetector::drawDlMessage(Mat& image, std::vector<DetectResult> output)
    {
        for(int i = 0; i < output.size(); i++)
        {
            int point_num = output[i].points.size();
            for(int j = 0; j < point_num; j++)
            {
                cv::circle(image, output[i].points[j], 2, Scalar(255,255,0), -1);
                cv::line(image, output[i].points[j], output[i].points[(j+1)%point_num], Scalar(0, 100, 50));
            }
            std::string txt = "id:" + std::to_string(output[i].id) + " conf:" + std::to_string(output[i].conf);
            cv::putText(image, txt, output[i].bbox.br(), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                                    cv::Scalar(0, 255, 0), 1.8);        
        }        
    }

    void DlRuneDetector::drawTargetMessage(Mat& image)
    {
        circle(image, this->circle_center, 4, Scalar(255,0,0), -1);
        circle(image, this->target.armor_center, 4, Scalar(127,127,127), -1);
        
        for(int i = 0; i < 4; i++)
            circle(image, this->target.points[i], 6, Scalar(127,127,127), -1);
    }

    bool DlRuneDetector::FilterDetectResult(std::vector<DetectResult>& output)
    {
        int vane_red_num = 0;
        int vane_blue_num = 0;
        // 初步阈值筛选，同时确定扇叶颜色
        for(auto iter = output.begin(); iter != output.end();)
        {
            // 扇叶初筛选
            if((*iter).id == 0 || (*iter).id == 3)
            {
                if(iter->conf < this->param.circle_center_conf)
                {
                    output.erase(iter);
                    continue;
                }
            }
            if((*iter).id == 1 || (*iter).id == 4)
            {
                if((*iter).conf < this->param.no_activate_conf)
                {
                    output.erase(iter);
                    continue;
                }
            }
            if((*iter).id == 2 || (*iter).id == 5)
            {
                if((*iter).conf < this->param.activate_conf)
                {
                    output.erase(iter);
                    continue;
                }
            }

            // 判断扇叶颜色
            if((*iter).id < 3) // 0-2为蓝色扇叶或圆心
                vane_blue_num++;
            else // 3-5为红色扇叶或圆心
                vane_red_num++;
            ++iter;
        }

        if(output.size() == 0)
            return false;
        
        if(this->enemy_color_ == base::Color::BLUE)
            this->vane_color = base::Color::RED;
        else
            this->vane_color = base::Color::BLUE;            
        
        return true;
    }

    bool DlRuneDetector::classifer(std::vector<DetectResult> output, vector<base::RuneArmor>& rune_armors)
    {
        if(output.size() == 0)
            return false;        

        // 大符颜色与敌方颜色相符，则为打符;否则为反打符
        // 打符模式，返回深度学习解算的待激活扇叶与对应圆心(本程序已写死)
        bool find_no_activate = (this->vane_color != this->enemy_color_);
        bool find_blue = (this->vane_color == base::Color::BLUE);
        bool find_target = false;
        double last_target_conf = 0.0;
        double last_circle_center_conf = 0.0;
        // double min_distance = DBL_MAX;
        this->high_size = DBL_MAX;
        for(int i = 0; i < output.size(); i++)
        {
            if(find_blue)
            {
                // if(output[i].id == 0 && output[i].conf > last_circle_center_conf)
                // {
                //     this->dl_circle = output[i];
                //     last_circle_center_conf = output[i].conf;
                // }

                if(find_no_activate && output[i].id == 1)
                {
                    changePoints(output[i].points);
                    int temp_size = calculateBinarySize(output[i].points);


                    if(temp_size < this->param.low_threshold)//若小于此阈值，则舍去
                        continue;
                    if(temp_size > this->param.high_threshold)//若大于阈值，则舍去
                        continue;
                    
                    if(this->high_size > temp_size )
                    {
                        this->high_size = temp_size;
                        this->target = base::RuneArmor(output[i].points);
                        last_target_conf = output[i].conf; 
                        find_target = true;        
                        rune_armors.push_back(base::RuneArmor(output[i].points));
                    }
                }
                
                if(find_no_activate&&output[i].id==2)
                {
                    rune_armors.push_back(base::RuneArmor(output[i].points));//get rune_armors
                }//此处这么把已击打扇叶也加了进来，不知道会不会出问题。。


                //此处用于反符,但效果不好没应用过，暂且把他注释掉以写三维圆
                // else if(!find_no_activate && output[i].id == 2)/
                // {
                //     // 第一次发现目标
                //     if(this->state == base::TrackState::LOST)
                //     {
                //         this->target = base::RuneArmor(output[i].points);
                //     }
                //     else
                //     {
                //         base::RuneArmor temp_rune_armor = base::RuneArmor(output[i].points);
                //         double temp_distance = calDistance(temp_rune_armor.dl_armor_center, this->last_target.dl_armor_center); 
                //         if(temp_distance < min_distance)
                //         {
                //             this->target = temp_rune_armor;  
                //             min_distance = temp_distance;                         
                //         }
                //     }
                //     find_target = true; 
                // }
            }

            else
            {
                // if(output[i].id == 3 && output[i].conf > last_circle_center_conf)
                // {
                //     this->dl_circle= output[i];
                //     last_circle_center_conf = output[i].conf;
                // }

                if(find_no_activate && output[i].id == 4)
                {
                    changePoints(output[i].points);
                    int temp_size = calculateBinarySize(output[i].points);

                    if(temp_size < param.low_threshold)//若小于此阈值，则舍去
                        continue;
                    if(temp_size > param.high_threshold)//若大于阈值，则舍去
                        continue;

                    if(this->high_size > temp_size)
                    {
                        this->high_size = temp_size;
                        this->target = base::RuneArmor(output[i].points);
                        // std::cout<<"output[i].conf:"<<output[i].conf<<std::endl;
                        last_target_conf = output[i].conf; 
                        find_target = true;        
                        rune_armors.push_back(base::RuneArmor(output[i].points));
                    }  
                }
                
                if(find_no_activate && output[i].id == 5)
                {
                    rune_armors.push_back(base::RuneArmor(output[i].points));
                }


                // else if(!find_no_activate && output[i].id == 5)
                // {
                //     // 第一次发现目标
                //     if(this->state == base::TrackState::LOST)
                //     {
                //         this->target = base::RuneArmor(output[i].points);        
                //     } 
                //     else
                //     {
                //         base::RuneArmor temp_rune_armor = base::RuneArmor(output[i].points);
                //         double temp_distance = calDistance(temp_rune_armor.dl_armor_center, this->last_target.dl_armor_center); 
                //         if(temp_distance < min_distance)
                //         {
                //             this->target = temp_rune_armor;  
                //             min_distance = temp_distance;                         
                //         }
                //     } 
                //     find_target = true;  
                // }
            }
        }

        
        
        
        
        // 若返回关键点点数量不等于5,即有关键点缺损，则返回false
        if(!this->target.have_correct_points)
            return false;


        /*目前已经有了待击打扇叶，我们根据待击打扇叶离圆心最近的点和我们识别到的圆心的距离大小
        判断哪个圆心正确，认为距离最近的圆心正确。*/
        double min_distance = DBL_MAX;
        if(find_target)
            if(find_blue)
            {
                for(int i = 0; i < output.size(); i++)
                {
                    if(output[i].id == 0)
                    {
                        cv::Point2f center = (output[i].points[0] + output[i].points[1] + output[i].points[2] + output[i].points[3] + output[i].points[4])/5;
                        double distance = calDistance(center, this->target.circle_point);
                        if(distance < min_distance)
                        {
                            this->dl_circle = output[i];
                            min_distance = distance;
                        }
                    }
                }
            }
            else 
            {
                for(int i = 0; i < output.size(); i++)
                {
                    if(output[i].id == 3)
                    {
                        cv::Point2f center = (output[i].points[0] + output[i].points[1] + output[i].points[2] + output[i].points[3] + output[i].points[4])/5;
                        double distance = calDistance(center, this->target.circle_point);
                        if(distance < min_distance)
                        {
                            this->dl_circle = output[i];
                            min_distance = distance;
                        }
                    }
                }
            }
        return find_target;
    }
    void DlRuneDetector::changePoints(vector<cv::Point2d>& points)
    {
        if(points.size() != 5)
            std::cout<<"Rune points size is not 5!"<<std::endl;
        vector<cv::Point2d> temp_points;

        temp_points.push_back(points[2]);
        temp_points.push_back(points[1]);
        temp_points.push_back(points[0]);
        temp_points.push_back(points[4]);

        temp_points.push_back(points[3]);

        
        points = temp_points;
    }
    int DlRuneDetector::calculateBinarySize(std::vector<cv::Point2d> points)
    {
        cv::Mat binaryImage = this->bin;
        cv::Mat mask = cv::Mat::zeros(binaryImage.size(), binaryImage.type());
        std::vector<cv::Point2i> int_points;
        for (int i = 0; i < 4; ++i) {
            int_points.push_back(points[i]);
        }
        cv::fillPoly(mask, int_points, 255); // 填充多边形区域
        // 应用掩膜，只保留多边形内的像素
        cv::Mat maskedImage;
        cv::bitwise_and(binaryImage, mask, maskedImage);
        // imshow("maskedImage", maskedImage);
        // waitKey(1);
        // 计算面积，统计为True的像素数量
        int area = 0;
        for (int y = 0; y < maskedImage.rows; ++y) {
            for (int x = 0; x < maskedImage.cols; ++x) {
                if (maskedImage.at<uchar>(y, x) == 255) {
                    area++;
                }
            }
        }
        if(this->param.show_bin)
        {
            cv::imshow("maskedImage", maskedImage);
            waitKey(1);
        }
        // area现在包含了多边形内部的像素数量
        if(this->param.tell_area)
            std::cout << "Area of the object: " << area << std::endl;
        return area;
    }

    void DlRuneDetector::DlPreDeal(Mat frame)
    {
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        std::vector<Mat> bgr;
        split(frame, bgr);
        Mat gray_bin, color_bin;

        if (this->vane_color == base::Color::RED)
        {
            // imshow("gray",gray);
            // waitKey(1);
            threshold(gray, gray_bin, param.red_brightness_thresh, 255, THRESH_BINARY);
            // imshow("gray_bin",gray_bin);
            // waitKey(1);
            subtract(bgr[2], bgr[0], color_bin);
            threshold(color_bin, color_bin, param.red_color_thresh, 255, THRESH_BINARY);
            // imshow("color_bin",color_bin);
            // waitKey(1);
        }
        else
        {
            threshold(gray, gray_bin, param.blue_brightness_thresh, 255, THRESH_BINARY);
            subtract(bgr[0], bgr[2], color_bin);
            threshold(color_bin, color_bin, param.blue_color_thresh, 255, THRESH_BINARY);
        }

        this->bin = gray_bin & color_bin;
        // Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
        // cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        // cv::Mat element = (cv::Mat_<float>(3, 3) << 1, 1, 1,
        //                                             1, 1, 1,	
        //                                             1, 1, 1);

        // cv::Mat element = (cv::Mat_<float>(5, 5) << 1, 1, 1, 1, 1,
        //                                     1, 1, 1, 1, 1,	
        //                                     1, 1, 1, 1, 1,
        //                                     1, 1, 1, 1, 1,
        //                                     1, 1, 1, 1, 1);              
        // cv::imshow("bin", this->bin);
        // waitKey(1);
    }

    bool DlRuneDetector::DlDetectCircleCenter()
    {
        Point2f all_points = Point2f(0.0,0.0);
        int points_num = dl_circle.points.size();
        if(points_num < 5)
        {
            cout<<"not enough points for circle center!"<<endl;
            return false;
        }
        for(int i = 0; i < points_num; i++)
            all_points += Point2f(this->dl_circle.points[i]);
        Point2f dl_circle_center = all_points / points_num; 
        float R_roi_width = this->param.circle_center_roi_width;
        // 判断roi越界
        if(dl_circle_center.x - R_roi_width < 0 || dl_circle_center.x + R_roi_width > bin.cols || dl_circle_center.y - R_roi_width < 0 || dl_circle_center.y + R_roi_width > bin.rows)
        {
            cout<<"roi越界"<<endl;
            this->circle_center = dl_circle_center;
            return true;
        }
        cv::Rect2f R_roi_rect = cv::Rect2f(dl_circle_center.x - R_roi_width, dl_circle_center.y - R_roi_width, 2 * R_roi_width, 2 * R_roi_width);
        R_roi_rect &= cv::Rect2f(cv::Point2f(0, 0), cv::Point2f(bin.cols, bin.rows));
        this->R_bin = bin(R_roi_rect);
        
        if(param.show_R)
        {
            cv::imshow("R_Bin", this->R_bin);
            waitKey(1);
        }

        std::vector<std::vector<Point>> contours;
        std::vector<Vec4i> hierarchy;
        findContours(R_bin, contours, hierarchy, cv::RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, cv::Point2f(R_roi_rect.x, R_roi_rect.y));
        if (contours.size() <= 0)
        {
            cout<<"未检测到轮廓"<<endl;
            this->circle_center = dl_circle_center;
            return true;
        }

        cv::RotatedRect R_rect;
        double max_area = 0.0;
        for(int i = 0; i < contours.size(); i++)
        {
            float area = contourArea(contours[i]);     
            if(area > max_area)
            {
                R_rect = minAreaRect(contours[i]);
                max_area = area;
            }       
        }

        this->circle_center = R_rect.center;

        return true;
    }

    bool DlRuneDetector::DlDetectRuneArmor()
    {
        Point2f all_points = Point2f(0.0, 0.0);
        for(int i = 0; i < 4; i++)
        {
            // bool is_show_point = false;
            // if(i == 0)
            //     is_show_point = true;屎山，考虑删了

            this->target.points[i] = this->target.dl_points[i];
            all_points += this->target.points[i];
        }
        this->target.armor_center = all_points / 4;

        target.r_direction = target.armor_center - target.circle_center;
        target.angle = atan2(target.r_direction.y, target.r_direction.x);

        // 角度转换 
        if(target.angle < 0)
            target.angle += 2 * CV_PI;

        return true;
    }
}