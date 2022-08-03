# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("andorb")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${OpenCV_DIR}/jni/include)
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)
add_library( lib_opencv_core SHARED IMPORTED )
set_target_properties(lib_opencv_core PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/staticlibs/${ANDROID_ABI}/libopencv_core.a)

# Add ORB_SLAM3
add_subdirectory( ORB_SLAM3 )
set( ORB_SLAM3_INCLUDE_DIR ORB_SLAM3/include )

# Find Eigen3
SET(EIGEN3_INCLUDE_DIR "ORB_SLAM3/Thirdparty/eigen-3.3.7")

add_library( # Sets the name of the library.
        andorb

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Include libraries
include_directories(
        ${EIGEN3_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
        ${ORB_SLAM3_INCLUDE_DIR} )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        andorb
        lib_opencv
        #lib_opencv_core
        ORB_SLAM3

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
