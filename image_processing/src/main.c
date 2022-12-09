#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/image_rotation.h"
#include "../include/line_detection.h"
#include "../include/image_split.h"
#include "sudoku_builder.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 2 || argc > 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    Image image = getImageFromPng(argv[1]);

    if (argc == 3)
    {
        Image img = imageRotate(&image, atoi(argv[2]));
        saveImageToBmp(&img, "rotated");
        freeImage(&img);
        return EXIT_SUCCESS;
    }
    imageGrayscale(&image);
    saveImageToBmp(&image, "greyscale");

    imageContrastFilter(&image);
    saveImageToBmp(&image, "contrast");

    imageBinarization(&image);
    saveImageToBmp(&image, "mean");

    imageInvert(&image);
    Image image_cells = copyImage(&image);
    saveImageToBmp(&image, "inverted");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel");

    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur");

    int linesLength = 0;
    Line* lines = getImageLines(&image, 450, &linesLength);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough");

    free(lines);
    freeImage(&image);

    return EXIT_SUCCESS;
}
