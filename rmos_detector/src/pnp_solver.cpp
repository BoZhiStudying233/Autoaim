#include "rmos_detector/pnp_solver.hpp"

namespace detector
{
    bool PnpSolver::solveArmorPose(const base::Armor& armor,const cv::Mat& camera_matrix,const cv::Mat& dist_coeffs,cv::Mat &tVec, cv::Mat &rVec)
    {
        if (camera_matrix.empty())
            return false;

        std::vector<cv::Point2d> point2D;
        point2D = armor.points;
        switch (armor.type)
        {
            case base::ArmorType::SMALL:
                cv::solvePnP(small_armor, point2D, camera_matrix, dist_coeffs, rVec, tVec, false, cv::SOLVEPNP_ITERATIVE);
                break;
            case base::ArmorType::BIG:
                cv::solvePnP(big_armor, point2D, camera_matrix, dist_coeffs, rVec, tVec, false, cv::SOLVEPNP_ITERATIVE);
                break;
            default:
                cv::solvePnP(big_armor, point2D, camera_matrix, dist_coeffs, rVec, tVec, false, cv::SOLVEPNP_ITERATIVE);
                break;
        }

        return true;
    }
    
    bool PnpSolver::solveRuneArmorPose(std::vector<cv::Point2f> rune_next_pos,const cv::Mat& camera_matrix,const cv::Mat& dist_coeffs,cv::Mat &tVec, cv::Mat &rVec)
    {
        if (camera_matrix.empty())
            return false;

        cv::solvePnP(rune_armor,rune_next_pos, camera_matrix, dist_coeffs, rVec, tVec, false,cv::SOLVEPNP_IPPE);
  
        return true;

    }

}
