//
// Created by Quentin Gillet on 09/12/2022.
//

#include "../include/sudoku_builder.h"

void getSudokuGrid(char* s, const char *fileName)
{
    FILE* file;
    file = fopen(fileName,"r");
    char c;
    int i = 0;
    while (!feof(file))
    {
        c = fgetc(file);
        if (c == '.')
        {
            s[i] = c;
            i++;
        }
        else if ((c == ' ') || (c == '\n'))
            continue;
        else
        {
            s[i] = c;
            i++;
        }
    }
    fclose(file);
}
//105 x 125

void copyToImage(SDL_Rect rect, Image* source, Image* toCopy)
{
    int x = 0, y = 0;
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        for(int j = rect.y; j < rect.y + rect.h; j++)
        {
            printf("%i %i\n", i, j);
            source->pixels[i][j] = copyPixel(&toCopy->pixels[x][y]);
            y++;
        }
        y = 0;
        x++;
    }
}


/*Image img = buildSudoku("grid_00.result");
saveImageToPdf(&img, "result");

return 0;*/

Image buildSudoku(const char* resultFilePath)
{
    char* sudokuGrid = calloc(82, sizeof (char));
    getSudokuGrid(sudokuGrid, resultFilePath);

    Image imageGrid = getImageFromPng("sudoku_builder/blank-sudoku-grid.png");
    Image numbers[10];

    for(int i = 1; i < 10; i++)
    {
        char numberChar[2];
        snprintf(numberChar, 2, "%i", i);
        numbers[i] = getImageFromPng(concat("sudoku_builder/", concat(numberChar, ".png")));
    }
    //30 x + 15 y
    int x = 0, y = 0; // 30 , 25
    int numberIndex = 0;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            SDL_Rect rect = {x, y, 100, 100};
            int index = (int)sudokuGrid[numberIndex] - 48;
            copyToImage(rect, &imageGrid, &numbers[index]);
            y = clampInt(y + 110, 0, imageGrid.height);
            numberIndex++;
        }
        y = 0;
        x = clampInt(x + 130, 0, imageGrid.width);
    }

    free(sudokuGrid);

    return imageGrid;
}