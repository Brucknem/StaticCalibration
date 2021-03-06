//
// Created by brucknem on 06.07.21.
//

#include "StaticCalibration/CameraPoseEstimation.hpp"

namespace static_calibration {
    namespace calibration {

        ceres::ResidualBlockId
        CameraPoseEstimation::addCorrespondenceResidualBlock(ceres::Problem &problem, const ParametricPoint &point,
                                                             ceres::LossFunction *lossFunction) {
            return problem.AddResidualBlock(
                    residuals::CorrespondenceResidual::create(
                            point.getExpectedPixel(),
                            point,
                            intrinsics
                    ),
                    lossFunction,
                    &translation.x(),
                    &translation.y(),
                    &translation.z(),
                    &rotation.x(),
                    &rotation.y(),
                    &rotation.z(),
                    point.getLambda(),
                    weights[weights.size() - 1]
            );
        }

        CameraPoseEstimation::CameraPoseEstimation(const std::vector<double> &intrinsics) : CameraPoseEstimationBase(
                intrinsics) {}

        int CameraPoseEstimation::getCorrespondenceLossUpperBound() const {
            return CameraPoseEstimationBase::getCorrespondenceLossUpperBound() * 10;
        }
    }
}