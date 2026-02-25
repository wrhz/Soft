package com.example.softapplication

import com.facebook.yoga.YogaAlign
import com.facebook.yoga.YogaEdge
import com.facebook.yoga.YogaFlexDirection
import com.facebook.yoga.YogaJustify
import com.facebook.yoga.YogaNode
import com.facebook.yoga.YogaWrap

class Style {
    companion object {
        fun handleStyle(node: YogaNode, style: Map<String, String>) {
            val display = style["display"]

            node.setMargin(YogaEdge.ALL, 5f)

            for ((key, value) in style) {
                if (key == "display") continue

                when (key) {
                    "flex_direction" -> if (display == "flex") {
                        node.flexDirection = if (value == "column") {
                            YogaFlexDirection.COLUMN
                        } else {
                            YogaFlexDirection.ROW
                        }
                    }

                    "flex_wrap" -> if (display == "flex") {
                        node.wrap = when (value) {
                            "wrap" -> YogaWrap.WRAP
                            "wrap-reverse" -> YogaWrap.WRAP_REVERSE
                            else -> YogaWrap.NO_WRAP
                        }
                    }

                    "flex_grow" -> if (display == "flex") {
                        node.flexGrow = value.toFloat()
                    }

                    "justify_content" -> if (display == "flex") {
                        node.justifyContent = when (value) {
                            "flex-start" -> YogaJustify.FLEX_START
                            "flex-end" -> YogaJustify.FLEX_END
                            "center" -> YogaJustify.CENTER
                            "space-between" -> YogaJustify.SPACE_BETWEEN
                            "space-around" -> YogaJustify.SPACE_AROUND
                            else -> YogaJustify.SPACE_EVENLY
                        }
                    }

                    "align_items" -> if (display == "flex") {
                        node.alignItems = when (value) {
                            "flex-start" -> YogaAlign.FLEX_START
                            "flex-end" -> YogaAlign.FLEX_END
                            "center" -> YogaAlign.CENTER
                            "stretch" -> YogaAlign.STRETCH
                            else -> YogaAlign.BASELINE
                        }
                    }

                    "margin" -> node.setMargin(YogaEdge.ALL, value.toFloat())
                    "margin_top" -> node.setMargin(YogaEdge.TOP, value.toFloat())
                    "margin_right" -> node.setMargin(YogaEdge.RIGHT, value.toFloat())
                    "margin_bottom" -> node.setMargin(YogaEdge.BOTTOM, value.toFloat())
                    "margin_left" -> node.setMargin(YogaEdge.LEFT, value.toFloat())

                    "padding" -> node.setPadding(YogaEdge.ALL, value.toFloat())
                    "padding_top" -> node.setPadding(YogaEdge.TOP, value.toFloat())
                    "padding_right" -> node.setPadding(YogaEdge.RIGHT, value.toFloat())
                    "padding_bottom" -> node.setPadding(YogaEdge.BOTTOM, value.toFloat())
                    "padding_left" -> node.setPadding(YogaEdge.LEFT, value.toFloat())
                }
            }
        }
    }
}