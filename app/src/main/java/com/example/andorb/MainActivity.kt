package com.example.andorb

import android.os.Bundle
import android.util.Log
import android.view.SurfaceView
import android.view.WindowManager
import org.opencv.android.*
import java.util.*

class MainActivity : CameraActivity() {

    var jcv1: JavaCameraView? = null

    val mLoaderCallback = object: BaseLoaderCallback(this) {
        override fun onManagerConnected(status: Int) {
            when(status) {
                LoaderCallbackInterface.SUCCESS -> {
                    Log.i("opencvnew", "OpenCV loaded successfully")
                    jcv1?.enableView()
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
        if(!OpenCVLoader.initDebug()) {
            Log.e("opencvnew", "Error initialising OpenCV")
        } else {
            Log.i("opencvnew", "OpenCV successfully initialised")
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)
        }
    }

    override fun onPause() {
        super.onPause()
        jcv1?.disableView()
    }

    override fun getCameraViewList(): MutableList<CameraBridgeViewBase> {
        return Collections.singletonList(jcv1)
    }


    /**
     * A native method that is implemented by the 'andorb' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'andorb' library on application startup.
        init {
            System.loadLibrary("andorb")
        }
    }
}