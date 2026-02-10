package com.example.softapplication

import android.util.Log
import com.chaquo.python.PyObject

class SoftData {
    var home: PyObject? = null
    var routes: Map<String, PyObject>? = null
    var defaultFontFamily: String? = null

    constructor(soft: PyObject) {
        home = soft["home"]
        routes = soft["routes"]
        defaultFontFamily = soft["default_font_family"]?.toString()
    }
}
