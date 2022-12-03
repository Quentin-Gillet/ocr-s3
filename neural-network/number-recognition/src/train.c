
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "../include/train.h"
#include "../include/save.h"
//#include "../include/image.h"


double **initMatrix(unsigned int x, unsigned int y)
{
    double **matrice = NULL;
    matrice = calloc(y + 1, sizeof(unsigned int *));
    if (matrice == NULL)
    {
        return NULL;
    }
    for (size_t j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(unsigned int));
        if (matrice[j] == NULL)
        {
            //err(1, "Memory error");
			return NULL;
        }
    }
    return matrice;
}

void freeMatrix(double **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

// couche = zi | ai
// zi =S(wij * xi) + bias
// pour les couches suivantes xi (suivante) = ai (précédente)
// Donc : couche 2 => zi = S(wij * ai(couche precédente)) + b 
//
// w et b : sont different entre les couches
//
//
// ------------------- NUMBER RECOGNITION NEURAL NETWORK ----------------------

void LoadCSV(char* filename, char** images, int* numbers)
{

	FILE* fp = fopen(filename, "r");
			
	if (!fp)
		printf("Can't open file\n");

	else 
	{

		char buffer[1024];
		int i = 0;
		int column = 0;
		
		while (fgets(buffer,1024, fp)) 
		{
			column = 0;
			char* value = strtok(buffer, ", ");

			while (value) 
			{
				if (column == 0) 
				{
					//printf("image:");
					images[i] = strcpy(images[i], value);
				}

				if (column == 1) 
				{
					//printf("\tvalue:");
					numbers[i] = atoi(value);
				}
				//printf("%s", value);
				value = strtok(NULL, ", ");
				column++;
			}
			//printf("%s", images[i]);
			i++;
			//printf("\n");
		}

		fclose(fp);
	}

}

void zero_input(double *input)
{
	for (int i = 0; i < 784; i++)
	{
		input[i] = 0;
	}
}

void compute_expected(double *expected, int number)
{
	for (int i = 0; i < 10; i++)
	{
		expected[i] = 0;
	}
	expected[number] = 1;
}

// ACTIVATION FUNCTION : SIGMOID
double sigmoid(double z) 
{
    return 1 / (1+ (exp(-z)));
}

// DERIVATIVE FUNCTION OF THE SIGMOID FUNCTION
double dSigmoid(double z)
{
    return z * (1-z);
}

// TO SHUFFLE THE ORDER OF INPUTS
void shuffleDataSet(int *dataset, size_t len)
{
	if (len > 1)
	{
		for (size_t i = 0; i < len - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (len - i) + 1);
			// swap
			int tmp = dataset[j];
			dataset[j] = dataset[i];
			dataset[i] = tmp;
		}
	}
}


// -------------- FORWARD PROPAGATION -------------

double randomWB()
{
    double n = (((double)rand()) / ((double)RAND_MAX));
    return n;
}

// to fill the array of weight: random weight for the first epoch
//W[layer][next_layer]
void computeW(int layer, int next_layer, double **W)
{    
	for (int i = 0; i < layer; i++) 
    {
    	for (int j = 0; j < next_layer; j++) 
		{
    		W[i][j] = randomWB();
		//printf("%f\n",W[i][j]);
    	}
    }
}

// to fill the array of bias: random bias for the first epoch
//B[layer]
void computeB(int next_layer, double *B) 
{
    for (int i = 0; i < next_layer; i++) 
    {
	B[i] = randomWB();
	//printf("%f\n",B[i]);
    }
}

// compute the Z array which correspond to Z = S(W*X) +B
// S = Sum
// W = Weight from all inputs to the neuron in Z,
// B = Bias in the neuron in Z
//X[layer], W[layer][next_layer], B[next_layer], Z[next_layer]
void computeZ(int layer, int next_layer, double *X, double **W, double *B, double *Z) 
{

    for (int i = 0; i < next_layer; i++) {
		
	    double S = B[i];
	    for (int j = 0; j < layer; j++) {
		    	
		    S += W[j][i] * X[j];
	    	
	    }

	Z[i] = S;
    }
	
}

// compute the A array: apply the activation function foreach element in Z
// Z[layer], A[layer]
void computeA(int layer, double *Z, double *A)
{
    for(int i = 0; i < layer; i++)
    {
	    A[i] = sigmoid(Z[i]);

	}	
}

// do the same job as computeZ + computeA
//X[layer], W[layer][next_layer], B[next_layer], A[next_layer]
void computeActivation(int layer, int next_layer, double *X,
	double **W, double *B, double *A)
{
	for(int j = 0; j < next_layer; j++)
	{
	    	double act = B[j];
			    	
		for(int k = 0; k < layer; k++)  	
		{
		    	act += X[k] * W[k][j];
	
		}
		A[j] = sigmoid(act);
	}

}

// apply changes to Weight an Bias on the back layer by the Error on the layer
//W[back_layer][layer], B[back_layer], Error[layer], X[back_layer]
void modifWeightBiais(int layer, int back_layer, double **W,
	double *B, double *Error, double *X, double lr) 
{
	for (int j = 0; j < layer; j++) 
	{
		B[j] += Error[j] * lr;

		for (int k = 0; k < back_layer; k++) 
		{
			W[k][j] += X[k] * Error[j] * lr;
		}
	}
}


//training function
void train(int print)
{

	double input[NB_INPUTS];
	// loadInputs(inputs)

    double hiddenLayer1[NB_HIDDEN_N_L1];
    double hiddenLayer2[NB_HIDDEN_N_L1];
	double output[NB_OUTPUTS];					

	double Z_hidden_1[NB_HIDDEN_N_L1];
	double Z_hidden_2[NB_HIDDEN_N_L2];
	double Z_output[NB_OUTPUTS];

	double B_hidden_1[NB_HIDDEN_N_L1];
	double B_hidden_2[NB_HIDDEN_N_L2];
	double B_output[NB_OUTPUTS];			

	//double W_hidden_1[NB_INPUTS][NB_HIDDEN_N_L1];
	double** W_hidden_1 = initMatrix(NB_INPUTS,NB_HIDDEN_N_L1);
	//double W_hidden_2[NB_HIDDEN_N_L1][NB_HIDDEN_N_L2];
	double** W_hidden_2 = initMatrix(NB_HIDDEN_N_L1,NB_HIDDEN_N_L2);
	//double W_output[NB_HIDDEN_N_L2][NB_OUTPUTS];	
	double** W_output = initMatrix(NB_HIDDEN_N_L2,NB_OUTPUTS);

	const double learning_rate = 0.16f;		// learning coef

	computeB(NB_HIDDEN_N_L1, B_hidden_1);
	computeB(NB_HIDDEN_N_L2, B_hidden_2);
	computeB(NB_OUTPUTS, B_output);

	computeW(NB_INPUTS,NB_HIDDEN_N_L1,W_hidden_1);
	computeW(NB_HIDDEN_N_L1,NB_HIDDEN_N_L2,W_hidden_2);
	computeW(NB_HIDDEN_N_L2,NB_OUTPUTS,W_output);

	// traning of the neural network for 10000 epochs
	int steps[] = {0,1,2,3};

	for (int epoch = 0; epoch <= 10000; epoch++)
	{

		//load les pixels d'une image dans inputs
		//load expected_output
		int expected_output;

	    printf("epoch %d/10000 \r",epoch);
		fflush(stdout);
	    if (print == 1)
		{
			printf("\n");
	    	printf("epoch: %d	---------------------------------------------------\n", epoch);
		}
	    
		//shuffleDataSet(steps, 4);

		for(int x = 0; x < 4; x++)
		{
		    // FORWARD PROPAGATION
		    int i = steps[x];
						
			computeZ(NB_INPUTS, NB_HIDDEN_N_L1, input, W_hidden_1, B_hidden_1, Z_hidden_1);
			computeA(NB_HIDDEN_N_L1, Z_hidden_1, hiddenLayer1);

			computeZ(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, hiddenLayer1, W_hidden_2, B_hidden_2, Z_hidden_2);
			computeA(NB_HIDDEN_N_L2, Z_hidden_2, hiddenLayer2);

			computeZ(NB_HIDDEN_N_L2, NB_OUTPUTS, hiddenLayer2, W_output, B_output, Z_output);
			computeA(NB_OUTPUTS, Z_output, output);

		
			/*
			if (print == 1)
			{
				printf("Input: %f  %f  |  Output: %f  |  Expected Output: %f \n",
					inputs[i][0],inputs[i][1],
					output[0],expected_output[i]);
			}
			*/


			// BACK PROPAGATION
			//
			// compute the Error on the output layer
			double err_output[NB_OUTPUTS];
			
			for (int j = 0; j < NB_OUTPUTS; j++) 
			{
				double error = (expected_output - output[j]) ;
				err_output[j] = error * dSigmoid(output[j]);
			}
			
			// compute the Error on the hidden layer 2
			double err_hiddenLayer2[NB_HIDDEN_N_L2];
			for (int j = 0; j < NB_HIDDEN_N_L2; j++) 
			{
				double error = 0.0f;
				for (int k = 0; k < NB_OUTPUTS; k++) 
				{
					error += err_output[k] * W_output[j][k];

				}
				err_hiddenLayer2[j] = error * dSigmoid(hiddenLayer2[j]);
			}

			// compute the Error on the hidden layer 1
			double err_hiddenLayer1[NB_HIDDEN_N_L1];
			for (int j = 0; j < NB_HIDDEN_N_L1; j++)
			{
				double error = 0.0f;
				for (int k = 0; k < NB_HIDDEN_N_L2; k++)
				{
					error += err_output[k] * W_hidden_2[j][k];

				}
				err_hiddenLayer1[j] = error * dSigmoid(hiddenLayer1[j]);
			}


			// apply changes
			modifWeightBiais(NB_OUTPUTS, NB_HIDDEN_N_L2, W_output, B_output,
				err_output, hiddenLayer2, learning_rate);

			modifWeightBiais(NB_HIDDEN_N_L2, NB_HIDDEN_N_L1, W_hidden_2, B_hidden_2,
				err_hiddenLayer2, input, learning_rate);

			modifWeightBiais(NB_HIDDEN_N_L1, NB_INPUTS, W_hidden_1, B_hidden_1,
				err_hiddenLayer1, input, learning_rate);


		}
	}

	//save(NB_INPUTS, NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, NB_OUTPUTS, W_hidden_1, B_hidden_1, W_hidden_2, B_hidden_2, W_output, B_output);
	freeMatrix(W_hidden_1,NB_INPUTS);
	freeMatrix(W_hidden_2,NB_HIDDEN_N_L1);
	freeMatrix(W_output,NB_HIDDEN_N_L2);
}

int main()
{
	//train(0);
	char**images = NULL;
	int y = 300;
	images = calloc(y, sizeof(char*));
    for (size_t j = 0; j < y; j++)
    {
        images[j] = calloc(1024,sizeof(char));
	}
	int numbers[300];
	//get_input("C:/FG/EPITA/S3/OCR/Training Neural Network/images_link.csv",filenames,numbers,300);
	
	LoadCSV("C:/FG/EPITA/S3/OCR/Training Neural Network/images_link.csv", images, numbers);
	for(int i = 0; i < 5; i++)
	{
		printf("%s", images[i]);
		printf("\t%d", numbers[i]);
		printf("\n");
	}
	return 0;
}


