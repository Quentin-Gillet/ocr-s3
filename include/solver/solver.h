//
// Created by Quentin Gillet on 10/12/2022.
//

#ifndef OCR_SOLVER_H
#define OCR_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void neural_network_to_sudoku(char s[]);
void return_sudoku(char *argv, char s[]);

int resolve(char* file);

#endif //OCR_SOLVER_H
