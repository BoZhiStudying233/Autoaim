//
// Created by Wang on 23-6-15.
//

#ifndef RMOS_DETECTOR_HPP
#define RMOS_DETECTOR_HPP

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

// STD
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "rmos_utils/armor.hpp"
#include "rmos_utils/const.hpp"

namespace detector
{
    struct ProcessParams
    {
        double bin_threshold;
        int enemy_color;
    };
    
    struct LightParams
    {
        double angle_to_vertigal_max;
        double height_width_min_ratio;
        double size_area_min_ratio;
        bool isCornerCorrect;
    };

    struct ArmorParams
    {
        double lights_angle_max_diff;
        double lights_length_max_ratio;
        double lights_Y_max_ratio;
        double width_height_min_ratio;
        double width_height_max_ratio;
        double max_angle;
        double inside_thresh;
    };

    // 输入图像为BGR
    class Detector
    {
    public:
        Detector(ProcessParams p_params, LightParams l_params, ArmorParams a_params);
        ~Detector() = default;
        bool detectArmors(const cv::Mat &image, std::vector<base::Armor>& armors);
        /**
         * @brief 设置敌方颜色
         * @param[in] enemy_color 敌方颜色
         * @return 是否设置成功
        **/
        bool setEnemyColor(int enemy_color);
    private:
        bool findLights(const cv::Mat & image, std::vector<base::LightBlob>& lights);
        bool isLight(base::LightBlob light);
        bool matchLights(std::vector<base::LightBlob>& lights,std::vector<base::Armor>& armors);
        bool isArmor(base::LightBlob light_1,base::LightBlob light_2);

        /**
         * @brief 寻找最大亮度变化点
         * @param[in] image 输入图像
         * @param[in] start 起始点
         * @param[in] end 终止点
         * @return 最大亮度变化点
        **/
        cv::Point findMaxBrightnessChange(const cv::Mat& image, cv::Point start, cv::Point end);

        ProcessParams process_params_;
        LightParams light_params_;
        ArmorParams armor_params_;
        base::Color enemy_color_;
    public:
        cv::Mat src_;
        cv::Mat debug_binary_;
    };
}

#endif //RMOS_DETECTOR_HPP
