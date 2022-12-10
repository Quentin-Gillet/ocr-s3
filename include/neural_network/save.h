#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <err.h>

void to_B(int layer, double *B, char* f);
void to_W(int layer, int next_layer, double **W, char* f);

//W1[nb_inputs][nb_hiddenNeurons], B1[nb_hiddenNeurons]
//W2[nb_hiddenNeurons][nb_hiddenNeurons2], B2[nb_hiddenNerons]
//W3[nb_hiddenNeurons2][nb_outputs], B3[nb_outputs]
void save(int nb_inputs, int nb_hiddenNeurons, int nb_hiddenNeurons2, int nb_outputs, 
	double **W1, double *B1, double **W2, double *B2, double **W3, double *B3);

#endif
