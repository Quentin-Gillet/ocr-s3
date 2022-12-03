#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "../include/save.h"
#include "../include/train.h"

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

// to get finals W after training from files
int to_W(int layer, int next_layer, double **W, char* f)
{

	FILE* file = fopen(f, "r");

	if (file == NULL)
	{
		//printf("FILE DOES NOT EXIST\"training is needed");
		return 1;
	}

	for (int i = 0; i < layer; i++)
	{
		for (int j = 0; j < next_layer; j++)
		{
			fscanf(file, "%lf", &W[i][j]);
		}
	}

	fclose(file);
	return 0;

}

// to get finals Biais after training from files
int to_B(int layer, double *B, char* f)
{

	FILE* file = fopen(f, "r");

	if (file == NULL)
	{
		//printf("FILE DOES NOT EXIST\"training is needed");
		return 1;
	}

	for (int i = 0; i < layer; i++)
	{
		fscanf(file, "%lf", &B[i]);

	}

	fclose(file);
	return 0;

}



int main(int argc, char** argv)
{
	if (argc == 1)
	{
		printf("training...\n");
		train(0);
		return 0;
	}
	else if (argc != 3)
	{
		printf("INPUTS ERROR");
		return 1;
	}
	else
	{
		/* Check if the input file exist
		for (int i = 1; i <= 2; i++)
		{
			if (*argv[i] != '1' && *argv[i] != '0')
			{
				printf("INPUTS ERROR : 0 1 / 1 1 / 0 0 / 1 0");
				return 1;
			}
		}
		*/

		double hiddenLayer1[NB_HIDDEN_N_L1];
		double hiddenLayer2[NB_HIDDEN_N_L1];
		double output[NB_OUTPUTS];

		double Z_hidden_1[NB_HIDDEN_N_L1];
		double Z_hidden_2[NB_HIDDEN_N_L2];
		double Z_output[NB_OUTPUTS];

		double B_hidden_1[NB_HIDDEN_N_L1];
		double B_hidden_2[NB_HIDDEN_N_L2];
		double B_output[NB_OUTPUTS];

		double** W_hidden_1 = initMatrix(NB_INPUTS,NB_HIDDEN_N_L1);//[NB_INPUTS][NB_HIDDEN_N_L1];
		double** W_hidden_2 = initMatrix(NB_HIDDEN_N_L1,NB_HIDDEN_N_L2);//[NB_HIDDEN_N_L1][NB_HIDDEN_N_L2];
		double** W_output = initMatrix(NB_HIDDEN_N_L2,NB_OUTPUTS);//[NB_HIDDEN_N_L2][NB_OUTPUTS];


		int t1 = to_W(NB_INPUTS, NB_HIDDEN_N_L1,
			W_hidden_1, "training-result/hiddenLayer1.W");
		int t2 = to_B(NB_HIDDEN_N_L1,
			B_hidden_1, "training-result/hiddenLayer1.B");
		int t3 = to_W(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2,
			W_hidden_2, "training-result/outputLayer2.W");
		int t4 = to_B(NB_HIDDEN_N_L2,
			B_hidden_2, "training-result/outputLayer2.B");
		int t5 = to_W(NB_HIDDEN_N_L2, NB_OUTPUTS,
			W_output, "training-result/outputLayer.W");
		int t6 = to_B(NB_OUTPUTS,
			B_output, "training-result/outputLayer.B");

		if (t1 || t2 || t3 || t4 || t5 || t6)
		{

			printf("A FILE DOES NOT EXIST\ntraining is needed");
			return 1;
		}


		double input[NB_INPUTS]; // = load pixel from the image input

		for (int i = 0; i < NB_INPUTS; i++)
		{
			input[i] = (float)strtod(argv[i + 1], NULL);
		}

		computeZ(NB_INPUTS, NB_HIDDEN_N_L1, input, W_hidden_1, B_hidden_1, Z_hidden_1);
		computeA(NB_HIDDEN_N_L1, Z_hidden_1, hiddenLayer1);

		computeZ(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, hiddenLayer1, W_hidden_2, B_hidden_2, Z_hidden_2);
		computeA(NB_HIDDEN_N_L2, Z_hidden_2, hiddenLayer2);

		computeZ(NB_HIDDEN_N_L2, NB_OUTPUTS, hiddenLayer2, W_output, B_output, Z_output);
		computeA(NB_OUTPUTS, Z_output, output);		

		//int r = output[0] + 0.5;

		// return plus grande valeur de tout output

		return 0;

	}


}