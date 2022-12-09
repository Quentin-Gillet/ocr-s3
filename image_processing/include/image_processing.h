//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_PROCESSING_H
#define OCR_IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "utils.h"
#include "image.h"
#include "image_loading.h"

void imageGrayscale(Image* image);
void imageInvert(Image* image);
unsigned int maxColor(Image* image);
void normalizeBrightness(Image* image);
void imageContrastFilter(Image* image);
Pixel* getAdjacentPixels(Image* image, int x, int y);
int* getImageHistogram(Image* image);
int otsuMethod(Image* image);
void imageBinarization(Image* image);
Pixel getPixelMedian(Pixel* pixels);
void imageMedianBlur(Image* image);
double convolution(Image* image, double kernel[3][3], int row, int col);
void imageSobelFilter(Image* image);

ImageMajorOrder prepareImageFileForNeuralNetwork(const char* file);

#endif
