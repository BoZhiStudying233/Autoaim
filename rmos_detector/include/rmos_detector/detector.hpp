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
        ~Detector();
        bool detectArmors(const cv::Mat &image, std::vector<base::Armor>& armors);
        bool setEnemyColor(int enemy_color);
    private:
        bool findLights(const cv::Mat & image, std::vector<base::LightBlob>& lights);
        bool isLight(base::LightBlob light);
        bool matchLights(std::vector<base::LightBlob>& lights,std::vector<base::Armor>& armors);
        bool isArmor(base::LightBlob light_1,base::LightBlob light_2);

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
