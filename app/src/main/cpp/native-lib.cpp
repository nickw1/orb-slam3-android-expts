#include <jni.h>
#include <string>
//#include <System.h>
#include <opencv2/opencv.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_andorb_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    //ORB_SLAM2::System *s;
    return env->NewStringUTF(hello.c_str());
}