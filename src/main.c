#include "image_processing/image_processing.h"
#include "image_processing/image_loading.h"
#include "image_processing/image_rotation.h"
#include "image_processing/line_detection.h"
#include "image_processing/image_split.h"
#include "image_processing/sudoku_builder.h"
#include "neural_network/neural_network.h"
#include "interface/interfacegtk.h"

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
    Line* lines = getImageLines(&image, 450, &linesLength);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough", "");

    //test detection carré
    Line* newlines2 = get_Bigger_Squares(lines, linesLength,cpImage.width,cpImage.height);
    //Line* newlines2 = print_squares(lines, linesLength);
    drawLineOnImage(&cpImage, newlines2, 4);
    saveImageToBmp(&cpImage, "biggest-rectangle", "");

    //test découpage
    Image *images = calloc(81, sizeof(Image));
    images = split(newlines2, &image_cells, images);
    for(int i = 0; i < 81; i++)
    {
        char name[3];
        snprintf(name, 3, "%i", i);
        saveImageToBmp(&images[i], name, "numbers");
    }

    for(int i = 0; i < 81; i++)
    {
        freeImage(&images[i]);
    }
    
    //number recognition + solve sudoku
    Recognition_Solve();
    
    freeImage(&image_cells);
    freeImage(&cpImage);
    free(images);
    free(newlines2);
    free(lines);
    freeImage(&image);

    return EXIT_SUCCESS;
}
