#include "image_processing/line_detection.h"

struct Line* getImageLines(Image *image, int threshold, int *lineLength)
{
    int width = image->width, height = image->height;
    double hough_h = (sqrt(2.0) * (double) (height > width ? height : width)) / 2.0;
    int accumulatorHeight = (int)(hough_h * 2.0), accumulatorWidth = 180;

    int accumulatorSize = accumulatorWidth * accumulatorHeight;
    unsigned int *accumulator = calloc(accumulatorSize + 1, sizeof(unsigned int));

    double center_x = width / 2.0;
    double center_y = height / 2.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image->pixels[x][y].r > 250) {
                for (int t = 0; t < 180; t++) {
                    double r = ( ((double)x - center_x) * cos(degToRad(t))) + (((double)y - center_y) * sin(degToRad(t)));
                    int i = clampInt((int)((round(r + hough_h) * 180.0)) + t, 0, accumulatorSize);
                    accumulator[i]++;
                }
            }
        }
    }

    struct Line *lines = calloc((int) (width * height), sizeof(struct Line));
    int lineIndex = 0;

    for (int r = 0; r < accumulatorHeight; r++) {
        for (int t = 0; t < accumulatorWidth; t++) {
            if ((int) accumulator[r * accumulatorWidth + t] >= threshold) {
                unsigned int max = accumulator[r * accumulatorWidth + t];
                for (int ly = -4; ly <= 4; ly++) {
                    for (int lx = -4; lx <= 4; lx++) {
                        if ((ly + r >= 0 && ly + r < accumulatorHeight) && (lx + t >= 0 && lx + t <= accumulatorWidth)) {
                            if ((int) accumulator[((r + ly) * accumulatorWidth) + (t + lx)] > (int)max) {
                                max = accumulator[((r + ly) * accumulatorWidth) + (t + lx)];
                                ly = lx = 5;
                            }
                        }
                    }
                }
                if (max > accumulator[(r * accumulatorWidth) + t])
                    continue;

                int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

                if (t >= 45 && t <= 135) {
                    x1 = 0;
                    y1 = (r - accumulatorHeight / 2) - ((x1 - (int)(width / 2)) * cos(degToRad(t))) / sin(degToRad(t)) +
                         (int)(height / 2);
                    x2 = width;
                    y2 = (r - (accumulatorHeight / 2)) - ((x2 - (int)(width / 2)) * cos(degToRad(t))) / sin(degToRad(t)) +
                         (int)(height / 2);
                } else {
                    y1 = 0;
                    x1 = (r - (accumulatorHeight / 2)) - ((y1 - (int)(height / 2)) * sin(degToRad(t))) / cos(degToRad(t)) +
                         (int)(width / 2);
                    y2 = height;
                    x2 = (r - (accumulatorHeight / 2)) - ((y2 - (int)(height / 2)) * sin(degToRad(t))) / cos(degToRad(t)) +
                         (int)(width / 2);
                }
                Line line = {x1, y1, x2, y2, t};
                lines[lineIndex] = line;
                lineIndex++;
            }
        }
    }

    free(accumulator);

    *lineLength = lineIndex;
    return lines;
}


void drawLineOnImage(Image *image, Line *lines, int linesLength) {
    for (int i = 0; i < linesLength; i++) {
        int x0 = lines[i].x1, x1 = lines[i].x2;
        int y0 = lines[i].y1, y1 = lines[i].y2;

        int dX = abs(x1 - x0);
        int dY = -abs(y1 - y0);

        int sX = x0 < x1 ? 1 : -1;
        int sY = y0 < y1 ? 1 : -1;

        int err = dX + dY;

        while (1) {
            if (0 <= x0 && x0 < image->width && 0 <= y0 && y0 < image->height) {
                setPixelValue(&image->pixels[x0][y0], 0, 0, 255);
            }

            if (x0 == x1 && y0 == y1)
                break;

            int e2 = 2 * err;
            if (e2 >= dY) {
                err += dY;
                x0 += sX;
            }
            if (e2 <= dX) {
                err += dX;
                y0 += sY;
            }
        }
    }
}