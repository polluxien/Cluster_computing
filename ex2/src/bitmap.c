#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Bitmap bitmapAlloc(int width, int height) {
    Bitmap bitmap = {0};
    bitmap.bytes  = (char *)malloc(width * height);
    bitmap.height = height;
    bitmap.width  = width;
    return bitmap;
}

void bitmapFree(Bitmap bitmap) {
    free(bitmap.bytes);
}

void bitmapFill(Bitmap bitmap, char value) {
    memset(bitmap.bytes, value, bitmap.height * bitmap.width);
}

void bitmapSetPixel(Bitmap bitmap, int x, int y, char value) {
    // Only draw pixels at valid coordinates.
    if (x >= 0 && y >= 0 && x < bitmap.width && y < bitmap.height) {
        bitmap.bytes[y * bitmap.width + x] = value;
    }
}

void bitmapDrawPoints(Bitmap bitmap, Point *points, int pointCount, V2 boundsMin, V2 boundsMax) {
    // Clear the whole picture.
    bitmapFill(bitmap, 0x00);

    // Draw each point.
    for (int index = 0; index < pointCount; index++) {
        V2 p = points[index].position;
        double velocity = v2Length(points[index].velocity);
    
        // Transform the world position to the pixel position with respect to the bounds from the parameters.
        double x = bitmap.width  * (p.x - boundsMin.x) / (boundsMax.x - boundsMin.x);
        double y = bitmap.height * (p.y - boundsMin.y) / (boundsMax.y - boundsMin.y);
        
        char color = 0xFF;   // white
        double radius = 2.5; // Circle diameter = 5
        int pixelRadius = (int)(radius + 0.5); // Round up to integer.

        // iterate over potential pixels on the circle.
        for (int dy = -pixelRadius; dy <= pixelRadius; dy++) {
            for (int dx = -pixelRadius; dx <= pixelRadius; dx++) {
                // Draw if the distance is smaller than the radius.
                if (dx*dx + dy*dy <= radius * radius)
                    bitmapSetPixel(bitmap, (int)x+dx, (int)y+dy, color);
            }
        }
    }
}

/*
 *  PGM starts with a magic sequence "P5", then, whitespace separated, follows the width, height and the
 *  maximum value of a pixel. If the maximum value fits into a byte, each pixel is expected to be one byte.
 *  Then the values follow in row by row sequence from top to bottom.
 */
void bitmapSavePgm(Bitmap bitmap, char *path) {
    FILE *file = fopen(path, "wb");
    fprintf(file, "P5 %d %d 255\n", bitmap.width, bitmap.height);
    fwrite(bitmap.bytes, bitmap.height * bitmap.width, 1, file);
    fclose(file);
}

