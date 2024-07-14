#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <iostream>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"

struct DetectResult
{
    cv::Rect2d bbox;
    std::vector<cv::Point2d> points;
    int id;
    double conf;
    float temp_tensor[50];
};

using DetectResultList = std::vector<DetectResult>;
using SegmentResultList = std::vector<cv::Mat>;


#endif