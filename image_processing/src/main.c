#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/manual_rotation.h"
#include "../include/line_detection.h"
#include "../include/image_split.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 2 || argc > 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    SDL_Surface* surface = loadImage(argv[1]);
    Image image = newImage(surface);
    SDL_FreeSurface(surface);


    if (argc == 3)
    {
        imageRotate(&image, atoi(argv[2]));
        saveImageToBmp(&image, "rotated");
        freeImage(&image);
        return EXIT_SUCCESS;
    }
    imageGrayscale(&image);
    saveImageToBmp(&image, "greyscale");

    imageContrastFilter(&image);
    saveImageToBmp(&image, "contrast");

    imageBinarization(&image);
    saveImageToBmp(&image, "mean");

    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur");

    imageInvert(&image);
    Image image_cells = copyImage(&image);
    saveImageToBmp(&image, "inverted");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel");

    int linesLength = 0;
    Line* lines = houghTransform(&image, 0.4f, &linesLength);

    Image cpImage = copyImage(&image);
    Image cpImage2 = copyImage(&image);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough");

    int newLinesCounts = 0;
    Line* newlines = reduce_lines(lines, linesLength, &newLinesCounts);

    drawLineOnImage(&cpImage, newlines, newLinesCounts);
    saveImageToBmp(&cpImage, "hough_line_reduced");

    //test detection carré
    Line* newlines2 = get_Bigger_Squares(newlines, newLinesCounts);
    //Line* newlines2 = print_squares(newlines, newLinesCounts);
    drawLineOnImage(&cpImage2, newlines2, 4);
    saveImageToBmp(&cpImage2, "Big_Rectangle");

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

    //struct Square * Squares[] = get_Squares();

    free(newlines);
    free(lines);
    freeImage(&image);
    freeImage(&cpImage);

    return EXIT_SUCCESS;
}
