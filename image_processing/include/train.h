#ifndef TRAIN_H
#define TRAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "image.h"

#define NB_INPUTS 784
#define NB_HIDDEN_N_L1 128
#define NB_HIDDEN_N_L2 32
#define NB_OUTPUTS 10

// ---------------------------- TOOLS ----------------------------------

char **initMatrixChar(unsigned int y, unsigned int x);
void freeMatrixChar(char **matrice, size_t height);

double **initMatrixDouble(unsigned int y, unsigned int x);
void freeMatrixDouble(double **matrice, size_t height);

void print_layer(double* layer, int nb_neurons);
void print_image(double* image);
void image_to_input(ImageMajorOrder image, double *input);
int return_output(double* output);

// -------------- NUMBER RECOGNITION NEURAL NETWORK --------------------

void LoadCSV(char* filename, char** images, int* numbers);
void compute_expected(double *expected, int number);

double sigmoid(double z);
double softmax(double z,double sum);

double dSoftmax(double z);
double dSigmoid(double z);

void shuffleDataSet(int* dataset, int len);
void compute_steps(int* steps,int nb_steps);
double randomWB();

// ----------------------- FORWARD PROPAGATION --------------------------

void computeW(int layer, int next_layer, double** W);
void computeB(int next_layer, double* B);
void computeZ(int layer, int next_layer, double* X, double** W, double* B, double* Z);

void computeA_Output(int layer, double *Z, double *A);
void computeA_Layer(int layer, double *Z, double *A);

// ----------------------- BACKWARD PROPAGATION -------------------------

void computeActivation(int layer, int next_layer, double* X, double** W, double* B, double* A);
void modifWeightBiais(int layer, int back_layer, double** W, double* B, double* Error, double* X, double lr);

// ----------------------- TRAINING ------------------------------------

void train(int print);

#endif // 
