#include <sys/types.h>
#include <android/log.h>
#include <stdio.h>

static float easeInQuad(float x) {
    return x * x;
}

static float calculateBottomSpace(
        float x, float width,
        int bottom_left_corner,
        int bottom_right_corner,
        float bar_width,
        float bar_height
) {

    float cornerBottomSpace = 0.0;
    if (bar_height != 0.0) {
        float leftRadius = bottom_left_corner * 1.1;
        float rightRadius = bottom_right_corner * 1.1;
        if (x + bar_width < leftRadius) {
            float point = leftRadius - x;
            cornerBottomSpace = point * easeInQuad(1.0 / leftRadius * point);
        } else if (x - bar_width > (width - rightRadius)) {
            float point = rightRadius - (width - x);
            cornerBottomSpace = point * easeInQuad(1.0 / rightRadius * point);
        }
    }
    return cornerBottomSpace;
}

void draw_color(u_int32_t *pixel, u_int32_t color) {
    uint32_t alpha = (color & 0xFF000000) >> 24;
    uint32_t blue = (color & 0x00FF0000) >> 16;
    u_short green = (color & 0x0000FF00) >> 8;
    u_char red = color & 0x00000FF;
    *pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

void draw(jint stride, void *pixels, u_short x, u_short y, u_int32_t color) {
    pixels = (char *) pixels + y * stride;
    u_int32_t *pixel = ((u_int32_t *) pixels) + x;
    draw_color(pixel, color);
}

static void drawOnBitmap(
        JNIEnv *env,
        jobject bitmap, AndroidBitmapInfo bitmapInfo, int size, int color,
        int bottomLeftCorner, int bottomRightCorner, short *audioData
) {
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return;
    }

    float width = bitmapInfo.width;
    float height = bitmapInfo.height;
    float space = (width / size) / 10;
    float barWidth = width / size - space;

    int i = 0;
    float x = space / 2;

    void *pixels = 0;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        return;
    }

    for (int py = 0; py < width; py++) {
        for (int px = 0; px < height; px++) {
            draw(bitmapInfo.stride, pixels, px, py, 0x00000000);
        }
    }

    while (i < size) {
        int barHeight = roundf(height / 256 * (audioData[i] * .2));

        int cornerBottomSpace = roundf(calculateBottomSpace(
                x, width,
                bottomLeftCorner, bottomRightCorner,
                barWidth, barHeight
        ));

        for (int py = x; py < x + barWidth; py++) {
            for (int px = height - 1; px > fmaxf(0,barHeight + cornerBottomSpace); px--) {
                draw(bitmapInfo.stride, pixels, py, px, color);
            }
        }

        i++;
        x += barWidth + space;
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}