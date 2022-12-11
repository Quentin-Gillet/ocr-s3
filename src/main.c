#include "../include/image_processing/image_processing.h"
#include "../include/image_processing/image_loading.h"
#include "../include/image_processing/image_rotation.h"
#include "../include/image_processing/line_detection.h"
#include "../include/image_processing/image_split.h"
#include "../include/image_processing/sudoku_builder.h"
#include "../include/interface/interfacegtk.h"

int main(int argc, char** argv)
{

    // Checks the number of arguments.
    if(argc == 1)
    {
        start_gui();
        return EXIT_SUCCESS;
    }

    Image image = getImageFromPng(argv[1]);
    if (argc == 3)
    {
        Image img = imageRotate(&image, atoi(argv[2]));
        saveImageToBmp(&img, "rotated", "");
        freeImage(&img);
        return EXIT_SUCCESS;
    }
    imageGrayscale(&image);
    saveImageToBmp(&image, "greyscale", "");

    imageContrastFilter(&image);
    saveImageToBmp(&image, "contrast", "");

    imageBinarization(&image);
    saveImageToBmp(&image, "mean", "");
    Image image_cells = copyImage(&image);
    Image cpImage = copyImage(&image);

    imageInvert(&image);
    saveImageToBmp(&image, "inverted", "");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel", "");

    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur", "");

    int linesLength = 0;
    Line* lines = getImageLines(&image, 300, &linesLength);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough", "");

    //test detection carré
    Line* newlines = get_Bigger_Squares(lines, linesLength, image.width, image.height);
    //Line* newlines = print_squares(lines, linesLength, image.width, image.height);
    drawLineOnImage(&cpImage, newlines,  4);
    saveImageToBmp(&cpImage, "biggest-rectangle", "");

    //image = auto_rotation(&image, newlines);
    //saveImageToBmp(&image, "auto-rotated-image", "");

    //test découpage
    Image *images = calloc(81, sizeof(Image));
    images = split(newlines, &image_cells, images);
    for(int i = 0; i < 81; i++)
    {
        char name[3];
        snprintf(name, 3, "%i", i);
        saveImageToBmp(&images[i], name, "numbers");
        freeImage(&images[i]);
    }

    freeImage(&cpImage);
    freeImage(&image_cells);
    free(images);
    free(newlines);
    free(lines);

    return EXIT_SUCCESS;
}
