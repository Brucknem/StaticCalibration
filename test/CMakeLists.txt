cmake_minimum_required(VERSION 3.10)

########################################################################################################################
### Packages ###
########################################################################################################################

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(../misc/)

########################################################################################################################
### TESTING ###
########################################################################################################################
add_executable(Tests
        CameraTestBase.cpp
        RenderingPipelineTests.cpp
        CameraPoseEstimationTests.cpp
        ResidualsTests.cpp
        DataSetTests.cpp
        )
target_link_libraries(Tests
        PUBLIC StaticCalibration-lib
        gtest_main
        Eigen3::Eigen
        )
target_include_directories(Tests PUBLIC tests "${PROJECT_BINARY_DIR}" ${PROJECT_SOURCE_DIR}/include)

add_test(AllTests Tests)
