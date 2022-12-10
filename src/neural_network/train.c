#include "neural_network/train.h"

// -------------------------- TOOLS --------------------------
char **initMatrixChar(unsigned int y, unsigned int x)
{
    char **matrice = NULL;
    matrice = calloc(y + 1, sizeof(char*));
    if (matrice == NULL)
    {
        return NULL;
    }
    for (int j = 0; j < y; j++)
    {
        matrice[j] = calloc(x + 1, sizeof(char));
        if (matrice[j] == NULL)
        {
            //err(1, "Memory error");
			return NULL;
        }
    }
    return matrice;
}

void freeMatrixChar(char **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

double **initMatrixDouble(unsigned int y, unsigned int x)
{
    double **matrice = NULL;
    matrice = calloc(y+1, sizeof(double*));
    if (matrice == NULL)
    {
        return NULL;
    }
    for (int j = 0; j < y; j++)
    {
        matrice[j] = calloc(x+1, sizeof(double));
        if (matrice[j] == NULL)
        {
            //err(1, "Memory error");
			return NULL;
        }
    }
    return matrice;
}
void freeMatrixDouble(double **matrice, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        free(matrice[y]);
    }
    free(matrice);
}

void print_layer(double *layer, int nb_neurons)
{
	for (int i = 0; i < nb_neurons; i++)
	{
		printf("%f ",layer[i]);
	}
	printf("\n");
}

void print_image(double* image)
{
	for(int i = 0; i < 28; i++)
	{
		for(int j = 0; j < 28; j++)
		{
			printf("%d", (int)image[28 * j + i]);
		}
		printf("\n");
	}
}

void image_to_input(ImageMajorOrder image, double *input)
{
	for (int i = 0; i < NB_INPUTS; i++)
	{
		double pix = (double)image.pixels[i].pixelAverage;
		input[i] = pix == 0 ? 1 : 0; 
	}
}

int return_output(double* output)
{
	int max = 0;
	for (int i = 0; i < NB_OUTPUTS; i++)
	{
		if (output[i] > output[max])
		{
			max = i;
		}
	}
	return max;
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

void compute_expected(double *expected, int number)
{
	for (int i = 0; i < NB_OUTPUTS; i++)
	{
		expected[i] = 0;
	}
	expected[number] = 1;
}


// ACTIVATION FUNCTION : SIGMOID
double sigmoid(double z) 
{
	return 1 / (1 + (exp(-z)));
	//return fmax(0, z);
}

double softmax(double z, double sum) 
{
    return exp(z) / sum;
}

// DERIVATIVE FUNCTION OF THE SIGMOID FUNCTION
double dSigmoid(double a)
{
    return a * (1-a);
	//return (a > 0) ? 1 : 0;
}

double dSoftmax(double a)
{
    return a * (1-a);
}

// TO SHUFFLE THE ORDER OF INPUTS
void shuffleDataSet(int *dataset, int len)
{
	if (len > 1)
	{
		for (int i = 0; i < len-1; i++)
		{
			int j = i + rand() / (RAND_MAX / (len - i) + 1);
			// swap
			int tmp = dataset[j];
			dataset[j] = dataset[i];
			dataset[i] = tmp;
		}
	}
}

void compute_steps(int steps[], int nb_steps)
{
	for (int i = 0; i < nb_steps; i++)
	{
		steps[i] = i;
	}
}


// -------------- FORWARD PROPAGATION -------------

double randomWB()
{
    double n = (1*((double)rand()) / ((double)RAND_MAX)-0.5f);
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
void computeA_Output(int layer, double *Z, double *A)
{
	double sum = 0;
    for(int i = 0; i < layer; i++)
    {
		sum += exp(Z[i]);

	}	
	for(int i = 0; i < layer; i++)
    {
	    A[i] = softmax(Z[i],sum);
	}
}

void computeA_Layer(int layer, double *Z, double *A)
{
	for(int i = 0; i < layer; i++)
    {
	    A[i] = sigmoid(Z[i]);
		//printf("%f --> %f\n",Z[i],sigmoid(Z[i]));
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
	size_t nb_images = 5001;
	int nb_epochs = 10000;

	char** images = initMatrixChar(nb_images,1024);
	int numbers[nb_images];
	LoadCSV("images_link.csv", images, numbers);

	double** inputs = initMatrixDouble(nb_images, NB_INPUTS);
	const char * path = "images/";
	printf("---------------------------------\n");
	for(int i = 0; i < nb_images; i++)
	{
		char * name = images[i];
		char * input_image = concat(path,name);
		ImageMajorOrder im = prepareImageFileForNeuralNetwork(input_image);
		double input[NB_INPUTS];
		image_to_input(im, input);

		for(int j = 0; j < NB_INPUTS; j++)
		{
			inputs[i][j] = input[j];
		}
		
		printf("pre-processing : image %d/%ld\r",i,nb_images-1);
		fflush(stdout);
	}
	printf("\n");
	
    double hiddenLayer1[NB_HIDDEN_N_L1];
    double hiddenLayer2[NB_HIDDEN_N_L1];
	double output[NB_OUTPUTS];	

	double expected_output[NB_OUTPUTS];				

	double Z_hidden_1[NB_HIDDEN_N_L1];
	double Z_hidden_2[NB_HIDDEN_N_L2];
	double Z_output[NB_OUTPUTS];

	double B_hidden_1[NB_HIDDEN_N_L1];
	double B_hidden_2[NB_HIDDEN_N_L2];
	double B_output[NB_OUTPUTS];			

	//double W_hidden_1[NB_INPUTS][NB_HIDDEN_N_L1];
	double** W_hidden_1 = initMatrixDouble(NB_INPUTS,NB_HIDDEN_N_L1);
	//double W_hidden_2[NB_HIDDEN_N_L1][NB_HIDDEN_N_L2];
	double** W_hidden_2 = initMatrixDouble(NB_HIDDEN_N_L1,NB_HIDDEN_N_L2);
	//double W_output[NB_HIDDEN_N_L2][NB_OUTPUTS];	
	double** W_output = initMatrixDouble(NB_HIDDEN_N_L1,NB_OUTPUTS);

	double learning_rate = 0.004f;		// learning coef

	computeB(NB_HIDDEN_N_L1, B_hidden_1);
	computeB(NB_HIDDEN_N_L2, B_hidden_2);
	computeB(NB_OUTPUTS, B_output);
	
	computeW(NB_INPUTS,NB_HIDDEN_N_L1,W_hidden_1);
	computeW(NB_HIDDEN_N_L1,NB_HIDDEN_N_L2,W_hidden_2);
	computeW(NB_HIDDEN_N_L2,NB_OUTPUTS,W_output);

	printf("OK\n");

	printf("training :\n");
	// traning of the neural network for 10000 epochs
	int steps[nb_images];
	compute_steps(steps,nb_images);

	for (int epoch = 0; epoch <= nb_epochs; epoch++)
	{
		
		//load les pixels d'une image dans inputs
	    	    
		shuffleDataSet(steps, nb_images);

		int i;
		for(int x = 0; x < nb_images; x++)
		{
		    // FORWARD PROPAGATION
		    i = steps[x];
			printf("epoch %d/%d - image %d/%ld - index %d\r",epoch,nb_epochs,x,nb_images-1,i);
			fflush(stdout);
	
			compute_expected(expected_output,numbers[i]);

			computeZ(NB_INPUTS, NB_HIDDEN_N_L1, inputs[i], W_hidden_1, B_hidden_1, Z_hidden_1);
			computeA_Layer(NB_HIDDEN_N_L1, Z_hidden_1, hiddenLayer1);
	
			computeZ(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, hiddenLayer1, W_hidden_2, B_hidden_2, Z_hidden_2);
			computeA_Layer(NB_HIDDEN_N_L2, Z_hidden_2, hiddenLayer2);
	
			computeZ(NB_HIDDEN_N_L2, NB_OUTPUTS, hiddenLayer2, W_output, B_output, Z_output);
			computeA_Output(NB_OUTPUTS, Z_output, output);
						
			
			// BACK PROPAGATION

			// compute the Error on the output layer
			double err_output[NB_OUTPUTS];
			double error[NB_OUTPUTS];
			for (int j = 0; j < NB_OUTPUTS; j++) 
			{
				error[j] = (expected_output[j] - output[j]) ;
				err_output[j] = error[j] * dSoftmax(output[j]);
			}
			
			// compute the Error on the hidden layer 2
			double err_hiddenLayer2[NB_HIDDEN_N_L2];
			for (int j = 0; j < NB_HIDDEN_N_L2; j++) 
			{
				err_hiddenLayer2[j] = 0.0f;
				for (int k = 0; k < NB_OUTPUTS; k++) 
				{
					err_hiddenLayer2[j] += err_output[k] * W_output[j][k];
				}
				err_hiddenLayer2[j] *= dSigmoid(hiddenLayer2[j]);
			}
			
			// compute the Error on the hidden layer 1
			double err_hiddenLayer1[NB_HIDDEN_N_L1];
			for (int j = 0; j < NB_HIDDEN_N_L1; j++)
			{
				err_hiddenLayer1[j] = 0.0f;
				for (int k = 0; k < NB_HIDDEN_N_L2; k++)
				{
					err_hiddenLayer1[j] += err_hiddenLayer2[k] * W_hidden_2[j][k];

				}
				err_hiddenLayer1[j] *= dSigmoid(hiddenLayer1[j]);
			}

			// apply changes
			modifWeightBiais(NB_OUTPUTS, NB_HIDDEN_N_L2, W_output, B_output,
				err_output, hiddenLayer2, learning_rate);

			modifWeightBiais(NB_HIDDEN_N_L2, NB_HIDDEN_N_L1, W_hidden_2, B_hidden_2,
				err_hiddenLayer2, hiddenLayer1, learning_rate);

			modifWeightBiais(NB_HIDDEN_N_L1, NB_INPUTS, W_hidden_1, B_hidden_1,
				err_hiddenLayer1, inputs[i], learning_rate);

		}
	}

	save(NB_INPUTS, NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, NB_OUTPUTS, W_hidden_1, B_hidden_1, W_hidden_2, B_hidden_2, W_output, B_output);
	
	printf("\n");
	freeMatrixDouble(W_hidden_1,NB_INPUTS);
	freeMatrixDouble(W_hidden_2,NB_HIDDEN_N_L1);
	freeMatrixDouble(W_output,NB_HIDDEN_N_L2);
	//freeMatrixDouble(inputs,nb_images);
	freeMatrixChar(images,nb_images);

	printf("OK\n");
	printf("---------------------------------\n");

}



