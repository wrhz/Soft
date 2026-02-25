package com.example.softapplication

import android.content.Context
import android.graphics.Color
import android.graphics.Point
import android.graphics.Typeface
import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowManager
import android.widget.FrameLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.chaquo.python.PyObject
import com.chaquo.python.Python
import com.chaquo.python.android.AndroidPlatform
import com.facebook.soloader.SoLoader
import com.facebook.yoga.YogaDirection
import com.facebook.yoga.YogaNode
import com.facebook.yoga.YogaNodeFactory

class MainActivity : AppCompatActivity() {
    private lateinit var py: Python

    private lateinit var mainSoft: SoftData

    private var page: PyObject? = null

    private var typeface: Typeface? = null

    private lateinit var screenSize: Pair<Int, Int>

    private lateinit var rootNode: YogaNode

    private lateinit var fontJson: FontJson

    private lateinit var frameLayout: FrameLayout

    @Suppress("DEPRECATION")
    fun getRealScreenSize(context: Context): Pair<Int, Int> {
        val windowManager = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val metrics = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                windowManager.currentWindowMetrics
            } else {
                windowManager.currentWindowMetrics
            }
            val bounds = metrics.bounds
            Pair(bounds.width(), bounds.height())
        } else {
            val display = windowManager.defaultDisplay
            val size = Point()
            display.getRealSize(size)
            Pair(size.x, size.y)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        SoLoader.init(this, false)

        screenSize = getRealScreenSize(this)

        rootNode = YogaNodeFactory.create()

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

        rootNode.setWidth(screenSize.first.toFloat())
        rootNode.setHeight(screenSize.second.toFloat())
        rootNode.setDirection(YogaDirection.LTR)

        frameLayout = FrameLayout(this).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
            setBackgroundColor(Color.WHITE)
        }

        if (page !== null) {
            val rootElement = ElementData(page!!.call()["element"])

            ElementData.setStyle(rootNode, rootElement, typeface, fontJson.fontSize)

            rootNode.calculateLayout(screenSize.first.toFloat(), screenSize.second.toFloat())

            draw(rootElement)
        }

        setContentView(frameLayout)
    }

    private fun draw(elementData: ElementData) {
        val tag = elementData.tag

        val element = when (tag) {
            "text" -> TextView(this).apply {
                text = elementData.text
                textSize = fontJson.fontSize
                typeface = this@MainActivity.typeface
                setTextColor(Color.BLACK)
            }
            "view" -> View(this).apply {}
            else -> null
        }

        if (element !== null) {
            element.x = elementData.node.layoutX
            element.y = elementData.node.layoutY

            frameLayout.addView(element)

            for (child: ElementData in elementData.children) {
                draw(child)
            }
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