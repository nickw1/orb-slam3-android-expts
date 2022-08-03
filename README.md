# ORB-SLAM3 Android

An attempt to get ORB-SLAM3 working on Android. Please note that it is ***NOT SUCCESSFULLY BUILDING YET!!!***

## Steps to create project

1. Thanks to Vladimir Somers for [this extremely useful setup guide](https://github.com/VlSomers/native-opencv-android-template) for OpenCV on Android. The template project was created using this

2. Basic ORB_SLAM Android project structure taken from [Lewis Baker's repository](https://github.com/bakelew/ORB_SLAM2_Android) and added to OpenCV template above. Note this is ORB_SLAM2, not ORB_SLAM3. OpenCV references commented out as step 1 already brings in OpenCV.

3. ORB_SLAM2 code replaced by ORB-SLAM3 code taken from the Emscripten port [here](https://github.com/nickw1/ORB_SLAM3/tree/binvoc) - please use the `binvoc` branch. `Thirdparty` libraries also replaced.
