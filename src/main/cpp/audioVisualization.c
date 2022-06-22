#include <jni.h>
#include <math.h>
#include <android/bitmap.h>
#include "audioVisualization.h"

JNIEXPORT jfloat JNICALL
Java_de_dertyp7214_audiovisualization_components_AudioVisualization_calculateBottomSpace(
        __attribute__((unused)) JNIEnv *env,
        __attribute__((unused)) jobject class,
        jfloat x, jfloat width,
        jint bottom_left_corner,
        jint bottom_right_corner,
        jfloat bar_width,
        jfloat bar_height) {

    return calculateBottomSpace(x, width, bottom_left_corner, bottom_right_corner, bar_width,
                                bar_height);
}

JNIEXPORT jint JNICALL
Java_de_dertyp7214_audiovisualization_components_AudioVisualization_drawOnBitmap(
        JNIEnv *env,
        __attribute__((unused)) jobject class,
        jobject bitmap,
        jint size,
        jint color,
        jint bottom_left_corner,
        jint bottom_right_corner,
        jshortArray audio_data
) {
    if (!bitmap) {
        return -1;
    }

    AndroidBitmapInfo info;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        return -2;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return -3;
    }

    if (!info.width || !info.height) {
        return -4;
    }

    if (!info.stride) {
        return -5;
    }

    drawOnBitmap(
            env, bitmap, info, size, color,
            bottom_left_corner, bottom_right_corner,
            audio_data
    );

    return 0;
}