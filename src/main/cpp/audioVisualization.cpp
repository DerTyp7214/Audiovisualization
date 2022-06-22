#include <jni.h>
#include <cmath>
#include <android/bitmap.h>
#include "audioVisualization.h"

extern "C" {
JNIEXPORT jfloat JNICALL
Java_de_dertyp7214_audiovisualization_components_AudioVisualizationC_calculateBottomSpace(
        __attribute__((unused)) JNIEnv *env,
        __attribute__((unused)) jobject clazz,
        jfloat x, jfloat width,
        jint bottom_left_corner,
        jint bottom_right_corner,
        jfloat bar_width,
        jfloat bar_height) {

    return calculateBottomSpace(x, width, bottom_left_corner, bottom_right_corner, bar_width,
                                bar_height);
}

JNIEXPORT jint JNICALL
Java_de_dertyp7214_audiovisualization_components_AudioVisualizationC_drawOnBitmap(
        JNIEnv *env,
        __attribute__((unused)) jobject clazz,
        jobject bitmap,
        jint size,
        jint color,
        jint bottom_left_corner,
        jint bottom_right_corner,
        jshortArray audio_data
) {
    short *local_audio_data = env->GetShortArrayElements(audio_data, (jboolean *) false);

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
            local_audio_data
    );

    return 0;
}

JNIEXPORT jshortArray JNICALL
Java_de_dertyp7214_audiovisualization_components_AudioVisualizationC_changeShortArraySize(
        JNIEnv *env,
        __attribute__((unused)) jobject clazz,
        jshortArray _receiver, jint new_size
) {
    short out_array[new_size];
    changeShortArraySize(
            env->GetArrayLength(_receiver),
            env->GetShortArrayElements(_receiver, (jboolean *) false),
            new_size,
            out_array
    );
    jshortArray result = env->NewShortArray(new_size);
    env->SetShortArrayRegion(result, 0, new_size, out_array);
    return result;
}
}