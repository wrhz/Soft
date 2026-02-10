package com.example.softapplication

import android.graphics.Color
import android.graphics.Typeface
import android.os.Bundle
import android.util.Log
import android.widget.FrameLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.chaquo.python.PyObject
import com.chaquo.python.Python
import com.chaquo.python.android.AndroidPlatform

class MainActivity : AppCompatActivity() {
    private lateinit var py: Python;

    private lateinit var mainSoft: SoftData

    private var page: PyObject? = null

    private var typeface: Typeface? = null

    private lateinit var fontJson: FontJson

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        fontJson = FileManager.readFileFromRawToJson<FontJson>(this, "config/font.json")

        if (!Python.isStarted()) {
            Python.start(AndroidPlatform(applicationContext))
        }
        py = Python.getInstance()

        val init = py.getModule("init");
        init.callAttr("init")

        val main = py.getModule("lib.main")

        mainSoft = SoftData(main.callAttr("main"))

        typeface = FileManager.loadFontFromAssets(this, "fonts/${fontJson.fontFamilies[mainSoft.defaultFontFamily]}")

        page = mainSoft.home

        val frameLayout = FrameLayout(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
            setBackgroundColor(Color.WHITE)
        }

        if (page !== null) {
            val rootElement = ElementData(page!!.call()["element"])

            draw(rootElement, frameLayout)
        }

        setContentView(frameLayout)
    }

    private fun draw(elementData: ElementData, frameLayout: FrameLayout) {
        val tag = elementData.tag

        val element = when (tag) {
            "text" -> TextView(this).apply {
                text = elementData.text
                textSize = fontJson.fontSize
                setTextColor(Color.BLACK)
                gravity = elementData.style
            }
            else -> null
        }

        if (element !== null) {
            if (typeface !== null) {
                element.typeface = typeface
            }

            frameLayout.addView(element)
        }
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