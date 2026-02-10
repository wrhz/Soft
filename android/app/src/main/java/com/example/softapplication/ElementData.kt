package com.example.softapplication

import android.util.Log
import android.view.Gravity
import com.chaquo.python.PyObject

class ElementData {
    var tag: String? = null
    var text: String? = null
    var children: MutableList<ElementData> = mutableListOf()
    var style: Int = Gravity.START or Gravity.TOP

    constructor(element: PyObject?) {
        if (element !== null) {
            var elementMap: MutableMap<String, PyObject?> = mutableMapOf()

            for ((key, value) in element.asMap()) {
                elementMap[key.toString()] = value
            }

            tag = elementMap["tag"]?.toString()
            text = elementMap["text"]?.toString()
            val childrenList = elementMap["children"]?.asList()
            if (childrenList != null) {
                for (child in childrenList) {
                    children.add(ElementData(child))
                }
            }
            style = handleStyle(elementMap["style"])
        }
    }

    private fun handleStyle(style: PyObject?): Int {
        if (style != null) {
            var horizontalAlign: Int = Gravity.START;
            var verticalAlign: Int = Gravity.TOP;
            for (styleChild in style.asMap()) {
                val key: String = styleChild.key.toString()
                val value: String = styleChild.value.toString()

                if (key == "horizontal_align") {
                    horizontalAlign = when (value) {
                        "right" -> Gravity.RIGHT
                        "center" -> Gravity.CENTER_HORIZONTAL
                        else -> Gravity.LEFT
                    }
                } else if (key == "vertical_align") {
                    verticalAlign = when (value) {
                        "bottom" -> Gravity.BOTTOM
                        "middle" -> Gravity.CENTER_VERTICAL
                        else -> Gravity.TOP
                    }
                }
            }

            return horizontalAlign or verticalAlign
        }

        return Gravity.START or Gravity.TOP
    }
}