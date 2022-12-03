#ifndef TRAIN_H
#define TRAIN_H

#define NB_INPUTS 784
#define NB_HIDDEN_N_L1 16
#define NB_HIDDEN_N_L2 16
#define NB_OUTPUTS 10

double sigmoid(double z);

double dSigmoid(double z);

void shuffleDataSet(int* dataset, size_t len);

double randomWB();

void computeW(int layer, int next_layer, double** W);
void computeB(int next_layer, double* B);
void computeZ(int layer, int next_layer, double* X, double** W, double* B, double* Z);
void computeA(int layer, double* Z, double* A);

void computeActivation(int layer, int next_layer, double* X, double** W, double* B, double* A);

void modifWeightBiais(int layer, int back_layer, double** W, double* B, double* Error, double* X, double lr);

void train(int print);

#endif // 
