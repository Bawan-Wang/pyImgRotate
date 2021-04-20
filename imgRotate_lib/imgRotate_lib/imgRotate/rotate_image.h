#ifndef ROTATE_IMAGE_H
#define ROTATE_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define    INVERSE_MAPPING    1
#define    INT_MAX_NUM        2147483647
#define    GET_RAD            0.0174532925 //PI / 180
#define    do_round(var)      ((int)((var)+0.5))

typedef unsigned char BYTE;

typedef struct _Point2D {
	int x;
	int y;
} Point2D;

typedef struct _Point2f {
	double x;
	double y;
} Point2f;

typedef struct _SizeConvert {
	int old_width;
	int old_height;
	int new_width;
	int new_height;
} SizeConvert;


BYTE *rotate_image(BYTE* img, int width, int height, int rot, int *new_width, int *new_height);
void rotate_image_release(BYTE ** releaseImg);

#ifdef __cplusplus
}
#endif

#endif
