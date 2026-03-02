package com.example.softapplication

class NativeLib {
    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("softapplication")
        }
    }
}