package com.example.softapplication

import android.content.Context
import android.graphics.Typeface
import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken

class FileManager {
    companion object {
        fun readJsonFromAsset(context: Context, fileName: String): String {
            return try {
                context.assets.open(fileName)
                    .bufferedReader()
                    .use { it.readText() }
            } catch (e: Exception) {
                Log.e("Error", "Failed to read file from assets: $fileName")
                ""
            }
        }

        inline fun <reified T> readFileFromRawToJson(context: Context, fileName: String): T {
            val type = object : TypeToken<T>() {}.type
            val jsonString = readJsonFromAsset(context, fileName)
            return Gson().fromJson(jsonString, type)
        }

        fun loadFontFromAssets(context: Context, fontPath: String): Typeface? {
            return try {
                Typeface.createFromAsset(context.assets, fontPath)
            } catch (e: Exception) {
                Log.e("FontLoader", "Failed to load font from assets: $fontPath", e)
                null
            }
        }
    }
}