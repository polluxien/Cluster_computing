#ifndef BITMAP_H
#define BITMAP_H


#include "point.h"

/*
 *  A bitmap is a 2D array of bytes. Each byte represents a gray-scale pixel from 0-255 (0x00-0xFF).
 *  Used to visualize points at a certain time.
 */
typedef struct Bitmap Bitmap;
struct Bitmap {
    char *bytes;
    int   width;
    int   height;
};

// Memory management.
Bitmap bitmapAlloc(int width, int height);
void   bitmapFree(Bitmap bitmap);

// Draw calls.
void bitmapFill(Bitmap bitmap, char value);
void bitmapSetPixel(Bitmap bitmap, int x, int y, char value);
void bitmapDrawPoints(Bitmap bitmap, Point *points, int point_count, V2 bounds_min, V2 bounds_max);

// Persisting a bitmap in PGM format.
void bitmapSavePgm(Bitmap bitmap, char *path);


#endif // BITMAP_H
