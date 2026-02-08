package com.example.softapplication

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.chaquo.python.Python
import com.chaquo.python.android.AndroidPlatform
import com.example.softapplication.databinding.ActivityMainBinding


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private lateinit var py: Python;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (!Python.isStarted()) {
            Python.start(AndroidPlatform(applicationContext))
        }
        py = Python.getInstance()

        val init = py.getModule("init");
        init.callAttr("init")

        val main = py.getModule("lib.main")
    }

    /**
     * A native method that is implemented by the 'softapplication' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'softapplication' library on application startup.
        init {
            System.loadLibrary("softapplication")
        }
    }
}