//
// Created by nuc12 on 23-6-23.
//



#include "rmos_detector/detector.hpp"

namespace detector
{

    Detector::Detector(ProcessParams p_params, LightParams l_params, ArmorParams a_params)
    {
        this->process_params_ = p_params;
        this->light_params_ = l_params;
        this->armor_params_ = a_params;

        if (process_params_.enemy_color == 0)
        {
            this->enemy_color_ = base::Color::RED;
        }
        else if (process_params_.enemy_color== 1)
        {
            this->enemy_color_ = base::Color::BLUE;
        }
    }

    bool Detector::detectArmors(const cv::Mat & image, std::vector<base::Armor>& armors)
    {
        if(image.empty())return false;

        src_ = image;
        std::vector<base::LightBlob> lights;
        bool is_find_lights = this->findLights(image,lights);
        bool is_find_armors = false;
        if(is_find_lights)
        {
            is_find_armors = this->matchLights(lights,armors);
        }
        else
        {
            is_find_armors = false;
        }
        return true;
    }

    bool Detector::findLights(const cv::Mat & image, std::vector<base::LightBlob>& lights)
    {
        lights.clear();
        // cv::Mat gaussian;
        cv::Mat gray;

        cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // std::vector<cv::Mat> bgr;
        // split(image, bgr);

        /*-----------预处理得二值图-----------*/
        // cv::Mat gray_binary, color_binary, binary;
        // auto start = std::chrono::steady_clock::now();
        // if (enemy_color_ == base::RED)
        // {
        //     threshold(gray, gray_binary, process_params_.red_threshold, 255, cv::THRESH_BINARY);

        //     subtract(bgr[2], bgr[0], color_binary);
        //     threshold(color_binary, color_binary, process_params_.red_blue_diff, 255, cv::THRESH_BINARY);
        //     binary = gray_binary & color_binary;
        // }
        // else
        // {
        //     threshold(gray, gray_binary, process_params_.blue_threshold, 255, cv::THRESH_BINARY);

        //     subtract(bgr[0], bgr[2], color_binary);
        //     threshold(color_binary, color_binary, process_params_.blue_red_diff, 255, cv::THRESH_BINARY);
        //     binary = gray_binary & color_binary;
        // }
        // cv::imshow("image", image);
        // cv::imshow("gray_binary", gray_binary);
        // cv::imshow("color_binary", color_binary);
        // cv::imshow("binary", binary);
        // cv::waitKey(0);

        // cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        // cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, element);

        cv::Mat binary;
        cv::threshold(gray, binary, process_params_.bin_threshold, 255, cv::THRESH_BINARY);
        debug_binary_ = binary;
        /*-----------寻找并筛选灯条轮廓-----------*/
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        for (const auto &contour: contours)
        {
            if (cv::contourArea(contour) < 4)continue;

            cv::RotatedRect light_rrect = cv::minAreaRect(contour);
            if (cv::contourArea(contour) / light_rrect.size.area() < light_params_.size_area_min_ratio)
            {
                continue;
            }
            base::LightBlob  light(light_rrect);
            if(isLight(light))
            {
                int sum_r = 0, sum_b = 0;
                for (const auto &point : contour) {
                sum_b += image.at<cv::Vec3b>(point.y, point.x)[0];
                sum_r += image.at<cv::Vec3b>(point.y, point.x)[2];
                }
                if (std::abs(sum_r - sum_b) / static_cast<int>(contour.size()) >
                    20) {
                    light.color = sum_r > sum_b ? base::Color::RED : base::Color::BLUE;
                }

                if(enemy_color_ == light.color)
                    lights.push_back(light);
            }
        }

        if (lights.size() < 2)
        {
            return false;
        }

        return true;
    }

    bool Detector::isLight(base::LightBlob light)
    {
        cv::RotatedRect light_rrect = light.rrect;
        if (abs(light.angle) > light_params_.angle_to_vertigal_max)
        {
            return false;
        }
        if (light.length / light.width < light_params_.height_width_min_ratio)
        {
            return false;
        }

        return true;

    }

    bool Detector::matchLights(std::vector<base::LightBlob>& lights,std::vector<base::Armor>& armors)
    {
        armors.clear();
        std::vector<base::Armor> temp_armors;
        auto cmp = [](base::LightBlob a, base::LightBlob b) -> bool
        {
            return a.rrect.center.x < b.rrect.center.x;
        };

        sort(lights.begin(), lights.end(), cmp);
        for (int i = 0; i < lights.size() - 1; i++)
        {
            for (int j = i + 1; j < lights.size(); j++)
            {
                int match_flag = 1;
                if (!isArmor(lights[i], lights[j]))
                {
                    continue;
                }
                float x_min = lights[i].rrect.center.x < lights[j].rrect.center.x? lights[i].rrect.center.x :lights[j].rrect.center.x;
                float x_max = lights[i].rrect.center.x > lights[j].rrect.center.x? lights[i].rrect.center.x :lights[j].rrect.center.x;
                float y_min = lights[i].up.y < lights[j].up.y? lights[i].up.y :lights[j].up.y;
                float y_max = lights[i].down.y > lights[j].down.y? lights[i].down.y :lights[j].down.y;
                for(int m=i+1;m<j;m++)
                {
                    float light_center_x = lights[m].rrect.center.x;
                    float light_center_y = lights[m].rrect.center.y;
                    if((light_center_x>x_min&&light_center_x<x_max)&&(light_center_y>y_min&&light_center_y<y_max))
                    {
                        match_flag = 0;
                        break;
                    }
                }
                if(match_flag == 1)
                {
                    lights[i].matched_count ++;
                    lights[j].matched_count ++;
                    temp_armors.push_back(base::Armor(lights[i], lights[j]));
                }
                else
                {
                    continue;
                }
            }

        }

        armors = temp_armors;

//        auto cmp_2 = [](base::Armor a, base::Armor b) -> bool
//        {
//            return a.rrect.center.x < b.rrect.center.x;
//        };
//
//        sort(temp_armors.begin(), temp_armors.end(), cmp_2);
//
//        if(temp_armors.size()>1)
//        {
//            for(int i=0;i<temp_armors.size();i++)
//            {
//                std::cout<<"left"<<temp_armors[i].left.matched_count<<std::endl;
//                std::cout<<"right"<<temp_armors[i].right.matched_count<<std::endl;
//                if(temp_armors[i].left.matched_count==2)
//                {
//
//                    float light_angle_1 = abs(temp_armors[i].left.angle-temp_armors[i].right.angle);
//                    float light_angle_2 = abs(temp_armors[i-1].left.angle-temp_armors[i-1].right.angle);
//                    if(light_angle_1<light_angle_2)
//                    {
//                        std::cout<<"777"<<std::endl;
//                        armors.pop_back();
//                        armors.push_back(temp_armors[i]);
//                    }
//                }
//                else
//                {
//                    armors.push_back(temp_armors[i]);
//                }
//
//            }
//
//        }
//        else
//        {
//            armors = temp_armors;
//        }


        return true;

    }

    bool  Detector::isArmor(base::LightBlob light_1,base::LightBlob light_2)
    {

        float width = light_2.rrect.center.x - light_1.rrect.center.x;
        float height = (light_2.length + light_1.length) / 2.0;
        float angle_i_org = light_1.angle;
        float angle_j_org = light_2.angle;

        // 匹配灯条：角度差、长度比、高度差、组成的宽高比，滤除/\ \/
        if (abs(angle_i_org - angle_j_org) > armor_params_.lights_angle_max_diff)
        {
            return false;
        }

        if(std::max(light_1.length, light_2.length) / std::min(light_1.length, light_2.length) > armor_params_.lights_length_max_ratio)
        {
            return false;
        }


        if(width / height > armor_params_.width_height_max_ratio ||
           width / height < armor_params_.width_height_min_ratio)
        {
            return false;
        }

        base::Armor temp(light_1, light_2);
        if (fabs(temp.rrect.angle) > armor_params_.max_angle)
        {
            return false;
        }




        //  滤除类如窗户形成的伪装甲板，即装甲板区域过亮
        cv::Mat temp_inarmor = src_(temp.rect & cv::Rect2d(cv::Point(0, 0), src_.size()));
        cv::cvtColor(temp_inarmor, temp_inarmor, cv::COLOR_BGR2GRAY);
        if (cv::mean(temp_inarmor)[0] > armor_params_.inside_thresh)
        {
            return false;
        }

        return true;

    }

    bool Detector::setEnemyColor(int enemy_color)
    {
        this->process_params_.enemy_color = enemy_color;
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
}