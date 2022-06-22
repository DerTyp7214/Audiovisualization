package de.dertyp7214.audiovisualization.components

import android.graphics.Bitmap
import androidx.annotation.ColorInt

object AudioVisualizationC {
    fun init() {
        System.loadLibrary("audiovisualization")
    }

    external fun calculateBottomSpace(
        x: Float,
        width: Float,
        bottomLeftCorner: Int,
        bottomRightCorner: Int,
        barWidth: Float,
        barHeight: Float
    ): Float

    external fun drawOnBitmap(
        bitmap: Bitmap,
        size: Int,
        @ColorInt color: Int,
        bottomLeftCorner: Int,
        bottomRightCorner: Int,
        audioData: ShortArray
    ): Int

    external fun ShortArray.changeShortArraySize(newSize: Int): ShortArray
}