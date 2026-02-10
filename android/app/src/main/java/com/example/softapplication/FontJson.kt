package com.example.softapplication

import com.google.gson.annotations.SerializedName

data class FontJson(
    @SerializedName("font_families")
    val fontFamilies: Map<String, String>,

    @SerializedName("font_size")
    val fontSize: Float
)
