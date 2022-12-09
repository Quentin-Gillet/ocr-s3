//
// Created by Quentin Gillet on 09/12/2022.
//

#ifndef IMAGE_PROCESSING_SUDOKU_BUILDER_H
#define IMAGE_PROCESSING_SUDOKU_BUILDER_H

#include "image.h"
#include "image_resize.h"
#include "image_loading.h"

void getSudokuGrid(char* s, const char *fileName);
Image buildSudoku(const char* resultFilePath);
void copyToImage(SDL_Rect rect, Image* source, Image* toCopy);

#endif //IMAGE_PROCESSING_SUDOKU_BUILDER_H
