cmake_minimum_required(VERSION 2.8)
project(ORB_SLAM3)

# Find OpenCV
#set( OpenCV_DIR ../../sdk/native/jni )
#find_package(OpenCV REQUIRED)
#include_directories( ${OpenCV_INCLUDE_DIRS} )

#add_library(lib_opencv SHARED IMPORTED)
#set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)


# Locate Eigen3
set(EIGEN3_INCLUDE_DIR Thirdparty/eigen-3.3.7)
set(BOOST_INCLUDE_DIR ..)

add_subdirectory(Thirdparty/g2o)
add_subdirectory(Thirdparty/DBoW2)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/CameraModels
        ${BOOST_INCLUDE_DIR}
        ${EIGEN3_INCLUDE_DIR}
        ${EIGEN3_INCLUDE_DIR}/unsupported
)

add_library(${PROJECT_NAME} SHARED
        src/System.cc
        src/Tracking.cc
        src/LocalMapping.cc
        src/LoopClosing.cc
        src/ORBextractor.cc
        src/ORBmatcher.cc
        src/FrameDrawer.cc
        src/Converter.cc
        src/MapPoint.cc
        src/KeyFrame.cc
        src/Map.cc
        src/MapDrawer.cc
        src/Optimizer.cc
        src/PnPsolver.cc
        src/Frame.cc
        src/KeyFrameDatabase.cc
        src/Sim3Solver.cc
        src/Initializer.cc
        src/Viewer.cc
        src/Atlas.cc
        src/G2oTypes.cc
        src/ImuTypes.cc
        src/MLPnPsolver.cpp
        src/OptimizableTypes.cpp
        src/TwoViewReconstruction.cc
        src/CameraModels/KannalaBrandt8.cpp
        src/CameraModels/Pinhole.cpp
        )

target_link_libraries(${PROJECT_NAME}
        lib_opencv
        #${OpenCV_LIBS}
        DBoW2
        g2o

        )

