#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/manual_rotation.h"
#include "../include/line_detection.h"
#include "../include/image_split.h"

void pollEvent()
{
    SDL_Event e;
    while (1){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return;
            }
        }
    }
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 2 || argc > 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = loadImage(argv[1]);
    Image image = newImage(surface);
    SDL_FreeSurface(surface);


    if (argc == 3)
    {
        imageRotate(&image, atoi(argv[2]));
        saveImageToBmp(&image, "rotated");
        freeImage(&image);
        SDL_Quit();
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
    saveImageToBmp(&image, "inverted");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel");

    surface = crateSurfaceFromImage(&image);

    int linesLength = 0;
    Line* lines = houghTransform(&image, 0.4f, &linesLength);

    Image cpImage = copyImage(&image);

    drawLineOnImage(&image, lines, linesLength);
    saveImageToBmp(&image, "hough");

    int newLinesCounts = 0;
    struct Line* newlines = reduce_lines(lines, linesLength, &newLinesCounts);

    drawLineOnImage(&cpImage, newlines, newLinesCounts);
    saveImageToBmp(&cpImage, "hough_line_reduced");

    //struct Square * Squares[] = get_Squares();

    free(newlines);
    free(lines);
    SDL_FreeSurface(surface);
    freeImage(&image);
    freeImage(&cpImage);

    SDL_Quit();
    return EXIT_SUCCESS;
}
