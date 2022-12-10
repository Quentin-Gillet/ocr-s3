#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/image_rotation.h"
#include "../include/line_detection.h"
#include "../include/image_split.h"
#include "../include/sudoku_builder.h"

int main(int argc, char** argv)
{

    Image img = buildSudoku("grid_00.result");
    saveImageToBmp(&img, "result");

    return 0;
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
    Image image_cells = copyImage(&image);
    Image cpImage = copyImage(&image);

    imageInvert(&image);
    saveImageToBmp(&image, "inverted");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel");

    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur");

    int linesLength = 0;
    Line* lines = getImageLines(&image, 450, &linesLength);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough");

    //test detection carré
    Line* newlines2 = get_Bigger_Squares(lines, linesLength);
    //Line* newlines2 = print_squares(lines, linesLength);
    drawLineOnImage(&cpImage, newlines2, 4);
    saveImageToBmp(&cpImage, "Big_Rectangle");

    //test découpage
    Image *images = calloc(81, sizeof(Image));
    images = split(newlines2, &image_cells, images);
    for(int i = 0; i < 81; i++)
    {
        char name[3];
        snprintf(name, 3, "%i", i);
        saveImageToBmp(&images[i], name);
    }

    freeImage(&image_cells);
    free(images);
    free(newlines2);
    free(lines);
    freeImage(&image);

    return EXIT_SUCCESS;
}
