#include <jni.h>
#include <string>
//#include <System.h>
#include <opencv2/opencv.hpp>
#include <System.h>

#include <android/log.h>

#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <pwd.h>

using namespace std;

ORB_SLAM3::System *slam = NULL;
vector<double> mapPoints;
bool isProcessingFrame= false;

void initSystem(const char*);
static void *loggingFunction(void*);
static int runLoggingThread();

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_andorb_MainActivity_initCPP(
        JNIEnv* env,
        jobject /* this */,
        jstring fs_root) {
    const char *c_str = env->GetStringUTFChars(fs_root, 0);
    std::string msg = "ORB_SLAM3 initialising...";

    runLoggingThread();

    __android_log_print(ANDROID_LOG_INFO, "ORB_SLAM_LOG", "native-lib initialising: c_str=%s", c_str);
    initSystem(c_str);

    struct passwd *pwd = getpwuid(getuid());
    __android_log_print(ANDROID_LOG_INFO, "ORB_SLAM_LOG", "Username %s", pwd->pw_name);

    return env->NewStringUTF(msg.c_str());
}

void initSystem(const char* c_str) {

    std::string s = c_str;
    slam = new ORB_SLAM3::System(s + "/ORBvoc.bin", s + "/Pixel3.yaml", ORB_SLAM3::System::MONOCULAR, false);
}

extern "C" JNIEXPORT void JNICALL Java_com_example_andorb_MainActivity_processFrame(
        JNIEnv *env, jobject, jlong addrGrayImage
        ) {
    if(isProcessingFrame || slam==NULL) { cout << "Ignoring frame as still processing" << endl; return; }
    isProcessingFrame = true;
    cout << "----------------" << endl << "Processing frame" << endl;
    cv::Mat *gray_image = (cv::Mat*) addrGrayImage;
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = t1.time_since_epoch();
    slam->TrackMonocular(*gray_image, dur.count());

    mapPoints.clear();

    vector<ORB_SLAM3::MapPoint*> points = slam->GetTrackedMapPoints();
    cout << "Got " << points.size() <<  "tracked map points." << endl;
    mapPoints.reserve(points.size() * sizeof(double) * 3);
    int nWorldPosFound = 0;
    for(int i=0; i<points.size(); i++) {
        cv::Mat worldPos = points[i]->GetWorldPos();
        if(worldPos.total() > 0) {
            nWorldPosFound++;
            for(int j=0; j<3; j++) {
                mapPoints.push_back(worldPos.at<double>(j));
            }
        }
    }

    ORB_SLAM3::Atlas *atlas = slam->getAtlas();
    vector<ORB_SLAM3::MapPoint*> allPoints = atlas->GetAllMapPoints();
    cout << "From atlas, there are " << allPoints.size() << " points." << endl;

    int nAllWorldPosFound = 0;
    for(int i=0; i<allPoints.size(); i++) {
        cv::Mat worldPos = allPoints[i]->GetWorldPos();
        if(worldPos.total() > 0) {
            nAllWorldPosFound++;
        }
    }
    cout << "Number of world positions found=" << nWorldPosFound << ". Number of world positions rom atlas found=" << nAllWorldPosFound << endl;
    isProcessingFrame = false;
}

extern "C" JNIEXPORT void JNICALL Java_com_example_andorb_MainActivity_shutdownCPP(
        JNIEnv *env, jobject
        ) {
    if(slam != NULL) {
        slam->Shutdown();
        delete slam;
    }
}


// Redirect stdout/stderr to logcat
// see https://codelab.wordpress.com/2014/11/03/how-to-use-standard-output-streams-for-logging-in-android-apps/

static int pfd[2];
static pthread_t loggingThread;
static const char *LOG_TAG = "ORB_SLAM_LOG";

static void *loggingFunction(void*) {
    ssize_t readSize;
    char buf[128];

    while((readSize = read(pfd[0], buf, sizeof buf - 1)) > 0) {
        if(buf[readSize - 1] == '\n') {
            --readSize;
        }

        buf[readSize] = 0;  // add null-terminator

        __android_log_write(ANDROID_LOG_DEBUG, LOG_TAG, buf); // Set any log level you want
    }

    return 0;
}

static int runLoggingThread() { // run this function to redirect your output to android log
    setvbuf(stdout, 0, _IOLBF, 0); // make stdout line-buffered
    setvbuf(stderr, 0, _IONBF, 0); // make stderr unbuffered

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if(pthread_create(&loggingThread, 0, loggingFunction, 0) == -1) {
        return -1;
    }

    pthread_detach(loggingThread);

    return 0;
}