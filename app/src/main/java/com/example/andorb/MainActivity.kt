package com.example.andorb

import android.app.AlertDialog
import android.content.Context
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.SurfaceView
import android.view.WindowManager
import org.opencv.android.*
import java.util.*

class MainActivity : CameraActivity() {

    var jcv1: JavaCameraView? = null
    var cppInit = false

    val mLoaderCallback = object : BaseLoaderCallback(this) {
        override fun onManagerConnected(status: Int) {
            when (status) {
                LoaderCallbackInterface.SUCCESS -> {
                    Log.i("ORB_SLAM_LOG", "OpenCV loaded successfully")
                    jcv1?.enableView()
                    val fsRoot = this@MainActivity.applicationInfo.dataDir
                    //val fsRoot = "/storage/emulated/0"
                    if (!cppInit) {
                        Log.d("ORB_SLAM_LOG", "Initialising C++")
                        val str = initCPP(fsRoot)
                        AlertDialog.Builder(this@MainActivity).setPositiveButton("OK", null)
                            .setMessage(str).show()
                        cppInit = true
                    }
                }
                else -> {
                    super.onManagerConnected(status)
                }
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        setContentView(R.layout.activity_main)
        jcv1 = findViewById(R.id.jcv1)
        jcv1?.visibility = SurfaceView.VISIBLE
    }

    override fun onResume() {
        super.onResume()
        if (!OpenCVLoader.initDebug()) {
            Log.e("ORB_SLAM_LOG", "Error initialising OpenCV")
        } else {
            Log.i("ORB_SLAM_LOG", "OpenCV successfully initialised")
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)
        }
    }

    override fun onPause() {
        super.onPause()
        jcv1?.disableView()
    }

    override fun onDestroy() {
        shutdownCPP()
        super.onDestroy()
    }

    override fun getCameraViewList(): MutableList<CameraBridgeViewBase> {
        return Collections.singletonList(jcv1)
    }


    /**
     * A native method that is implemented by the 'andorb' native library,
     * which is packaged with this application.
     */
    external fun initCPP(fsRoot: String): String
    external fun processFrame(): Unit
    external fun shutdownCPP(): Unit

    companion object {
        // Used to load the 'andorb' library on application startup.
        init {
            System.loadLibrary("andorb")
        }
    }
}