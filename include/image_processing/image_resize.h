//
// Created by Quentin Gillet on 07/12/2022.
//

#ifndef IMAGE_PROCESSING_IMAGE_RESIZE_H
#define IMAGE_PROCESSING_IMAGE_RESIZE_H

#include "image.h"
#include <math.h>
#include <stdlib.h>

Image imageResize(Image *image, unsigned int width, unsigned int height);
Image copyAndResizeImage(Image* image, unsigned int width, unsigned int height);

#endif //IMAGE_PROCESSING_IMAGE_RESIZE_H
