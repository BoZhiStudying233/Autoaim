#ifndef RMOS_ARMOR_HPP
#define RMOS_ARMOR_HPP

#include <opencv2/core.hpp>

#include "base.hpp"

namespace base
{
    class LightBlob
    {
        public:
            LightBlob();
            ~LightBlob(){};
            LightBlob(cv::RotatedRect box);

            cv::Point2f up;
            cv::Point2f down;
            double length;
            double width;
            cv::RotatedRect rrect;
            Color color;
            int matched_count{0};

            float angle;
    };

    enum ArmorType
    {
        WRONG,
        SMALL,
        BIG,
        RUNE_ARMOR,
        GRAY_BIG_ARMOR,
        GRAY_SMALL_ARMOR,
        DEAD_ARMOR,
    };

    class Armor
    {
        public:
            Armor();
            Armor(const LightBlob & l1, const LightBlob & l2);
            ~Armor(){};

            cv::RotatedRect rrect;
            cv::Rect2d rect;

            LightBlob left;
            LightBlob right;

            int num_id;
            ArmorType type;
            double distance_to_image_center;

            cv::Point2d center_point;

            double confidence;
            std::vector<cv::Point2d> points;

            double yaw;
            cv::Point3f position;
            cv::Point3d pyr;
    };
}
#endif //RMOS_ARMOR_HPP
