//
// Created by brucknem on 04.02.21.
//

#include "gtest/gtest.h"
#include "CameraTestBase.hpp"
#include "StaticCalibration/residuals/DistanceResidual.hpp"
#include "StaticCalibration/residuals/DistanceFromIntervalResidual.hpp"
#include "StaticCalibration/residuals/CorrespondenceResidual.hpp"
#include <utility>
#include <StaticCalibration/residuals/CorrespondenceWithIntrinsicsResidual.hpp>

using namespace static_calibration::calibration::residuals;

namespace static_calibration {
    namespace tests {

        /**
         * Tests for the residual blocks.
         */
        class ResidualsTests : public CameraTestBase {
        protected:

            /**
             * @destructor
             */
            ~ResidualsTests() override = default;

            void assertParametricPoint(static_calibration::calibration::ParametricPoint point, Eigen::Vector2d
            expectedResidual) {
                Eigen::Vector2d residual;
                static_calibration::calibration::residuals::CorrespondenceWithIntrinsicsResidual correspondenceResidual = {
                        point.getExpectedPixel(), point,
                };
                correspondenceResidual(
                        &intrinsics[0],
                        &intrinsics[1],
                        &intrinsics[2],
                        &intrinsics[3],
                        &translation.x(), &translation.y(), &translation.z(),
                        &rotation.x(), &rotation.y(), &rotation.z(),
                        point.getLambda(), new double(1), residual.data());

                EXPECT_NEAR(residual.x(), expectedResidual.x(), 1e-6);
                EXPECT_NEAR(residual.y(), expectedResidual.y(), 1e-6);
            }

            /**
             * Asserts that calculating projecting the world position to the image space results in the expected
             * residual error.
             */
            void assertPointCorrespondenceResidual(const Eigen::Vector3d &worldPosition, const Eigen::Vector2d &pixel,
                                                   Eigen::Vector2d expectedResidual) {
                assertParametricPoint(static_calibration::calibration::ParametricPoint(pixel, worldPosition),
                                      std::move(expectedResidual));
            }

            /**
             * Asserts that calculating projecting the world position to the image space results in the expected
             * residual error.
             */
            void assertLineCorrespondenceResidual(Eigen::Vector3d lineOrigin, const Eigen::Vector3d &lineHeading, double
            lambda, const Eigen::Vector2d &pixel, const Eigen::Vector2d &expectedResidual) {
                assertParametricPoint(
                        static_calibration::calibration::ParametricPoint(pixel, std::move(lineOrigin),
                                                                         lineHeading,
                                                                         lambda, lambda, lambda),
                        expectedResidual);
            }
        };

        /**
         * Tests the point correspondence residual calculation.
         */
        TEST_F(ResidualsTests, testPointCorrespondence) {
            Eigen::Vector3d worldPosition{0, 0, 0};
            Eigen::Vector2d pixel{960, 0};

            assertPointCorrespondenceResidual(worldPosition, pixel, {0, 0});

            pixel << 0, 0;
            assertPointCorrespondenceResidual(worldPosition, pixel, {-960, 0});

            worldPosition << 4, 20, 5;
            pixel = static_calibration::camera::render(translation.data(), rotation.data(),
                                                       intrinsics.data(), worldPosition.data());
            assertPointCorrespondenceResidual(worldPosition, pixel, {0, 0});

            pixel << 50, 50;
            assertPointCorrespondenceResidual(worldPosition, pixel, {-1070, -550});

            pixel << 1250, 650;
            assertPointCorrespondenceResidual(worldPosition, pixel, {130, 50});
        }


        /**
         * Tests the line correspondence residual calculation.
         */
        TEST_F(ResidualsTests, testLineCorrespondence) {
            Eigen::Vector3d lineOrigin{0, 0, 0};
            Eigen::Vector3d lineHeading{0, 0, 1};
            Eigen::Vector2d pixel{960, 0};
            double lambda = 0;

            assertLineCorrespondenceResidual(lineOrigin, lineHeading, lambda, pixel, {0, 0});

            lambda = 5;
            pixel << 960, 600;
            assertLineCorrespondenceResidual(lineOrigin, lineHeading, lambda, pixel, {0, 0});

            lambda = 10;
            assertLineCorrespondenceResidual(lineOrigin, lineHeading, lambda, pixel, {0, -600});

            lineHeading << 0, 1, 0;
            lambda = 20;
            pixel << 960, 400;
            assertLineCorrespondenceResidual(lineOrigin, lineHeading, lambda, pixel, {0, 0});
        }

    }
}

