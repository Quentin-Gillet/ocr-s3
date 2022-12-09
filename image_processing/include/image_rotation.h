#ifndef OCR_MANUAL_ROTATION_H
#define OCR_MANUAL_ROTATION_H

#include "utils.h"
#include "image.h"
#include "image_resize.h"

Image imageRotate(Image* image, int angleDegree);
void getMaxSize(Image* image, int angleDegree, int *maxTop, int *maxLeft, int *minTop, int *minLeft);

#endif
