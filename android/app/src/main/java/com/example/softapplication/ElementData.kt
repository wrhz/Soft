package com.example.softapplication

import android.content.res.Resources
import android.graphics.Paint
import android.graphics.Typeface
import android.util.TypedValue
import com.chaquo.python.PyObject
import com.facebook.yoga.YogaNode
import com.facebook.yoga.YogaNodeFactory

class ElementData {
    lateinit var tag: String
    lateinit var text: String
    val children: MutableList<ElementData> = mutableListOf()
    val style: MutableMap<String, String> = mutableMapOf()
    val node: YogaNode = YogaNodeFactory.create()

    companion object {
        fun setStyle(parentNode: YogaNode, element: ElementData, typeface: Typeface?, fontSize: Float, resources: Resources) {
            Style.handleStyle(element.node, element.style)
            val tag: String = element.tag

            when (tag) {
                "text" -> {
                    val pxSize = TypedValue.applyDimension(
                        TypedValue.COMPLEX_UNIT_SP,
                        fontSize,
                        resources.displayMetrics
                    )

                    val paint = Paint().apply {
                        this.textSize = pxSize
                        this.typeface = typeface ?: Typeface.DEFAULT_BOLD
                    }

                    val width = paint.measureText(element.text)
                    val fm = paint.fontMetrics
                    val height = fm.bottom - fm.top

                    element.node.setWidth(width)
                    element.node.setHeight(height)
                }
            }

            parentNode.addChildAt(element.node, parentNode.childCount)

            for (child: ElementData in element.children) {
                setStyle(element.node, child, typeface, fontSize, resources)
            }
        }
    }

    constructor(element: PyObject?) {
        if (element !== null) {
            var elementMap: MutableMap<String, PyObject?> = mutableMapOf()

            for ((key, value) in element.asMap()) {
                elementMap[key.toString()] = value
            }

            tag = elementMap["tag"]!!.toString()
            text = elementMap["text"]!!.toString()
            val childrenList = elementMap["children"]?.asList()
            if (childrenList != null) {
                for (child in childrenList) {
                    children.add(ElementData(child))
                }
            }
            for ((key, value) in elementMap["style"]!!.asMap()) {
                style[key!!.toString()] = value!!.toString()
            }
        }
    }
}