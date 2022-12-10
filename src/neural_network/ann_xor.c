#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>


// couche = zi | ai
// zi =S(wij * xi) + bias
// pour les couches suivantes xi (suivante) = ai (précédente)
// Donc : couche 2 => zi = S(wij * ai(couche precédente)) + b 
//
// w et b : sont different entre les couches
//
//
// ------------------- XOR NEURAL NETWORK ----------------------
//
// 	input:		hidden layer: 		output:
//
// 0 0 1 1 x1 		    n1
//				
//						 output: 0 1 1 0
//
// 0 1 0 1 x2		    n2
//


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
		for(size_t i = 0; i < len - 1; i++)
		{
	    		size_t j = i + rand()/(RAND_MAX/(len-i) + 1);
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
void computeW(int layer, int next_layer, double W[layer][next_layer])
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
void computeB(int next_layer, double B[next_layer])
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
void computeZ(int layer, int next_layer, double X[layer],
	double W[layer][next_layer], double B[next_layer], double Z[next_layer])
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
void computeA(int layer, double Z[layer], double A[layer])
{
    	for(int i = 0; i < layer; i++)
    	{
	    	A[i] = sigmoid(Z[i]);

			
   	}	
}

// do the same job as computeZ + computeA
void computeActivation(int layer, int next_layer, double X[layer],
	double W[layer][next_layer], double B[next_layer], double A[next_layer])
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
void modifWeightBiais(int layer, int back_layer, double W[back_layer][layer],
	double B[back_layer], double Error[layer], double X[back_layer], double lr) 
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

void print_layer(double *layer, int nb_neurons)
{
	for (int i = 0; i < nb_neurons; i++)
	{
		printf("%f ", layer[i]);
	}
	printf("\n");
}

// ----------------------- SAVE WEIGHT & BIAS ------------------
void save(int nb_inputs, int nb_hiddenNeurons, int nb_outputs,
	double W1[nb_inputs][nb_hiddenNeurons], double B1[nb_hiddenNeurons],
	double W[nb_hiddenNeurons][nb_outputs], double B[nb_outputs])
{
    	
	mkdir("training-result",0777);
    	
    	
	FILE * file_1;

	file_1 = fopen("training-result/hiddenLayer.W","w");
	
	//fprintf(file_1,"W hidden layer:\n");

	for(int i = 0; i < nb_hiddenNeurons; i++)
	{	
	    	for (int j = 0; j < nb_inputs; j++) 
		{
	    		fprintf(file_1, "%lg ", W1[i][j]);
		
	    	}
		if (i != nb_hiddenNeurons -1)
	    		fprintf(file_1, "\n");
	}
	
	//fprintf(file_return,"\n");

	//fprintf(file_return,"B hidden layer:\n");
	
	FILE * file_2;

	file_2 = fopen("training-result/hiddenLayer.B","w");

	for (int i = 0; i < nb_hiddenNeurons; i++) 
	{
		fprintf(file_2, "%lg ", B1[i]);	
	}


	FILE * file_3;

	file_3 = fopen("training-result/outputLayer.W","w");

	//fprintf(file_return,"W output layer:\n ");

	for(int i = 0; i < nb_hiddenNeurons; i++)
	{
	    	for (int j = 0; j < nb_outputs; j++) 
		{
	    		fprintf(file_3, "%lg ", W[i][j]);
		
	    	}
		if (i != nb_hiddenNeurons -1)
	    		fprintf(file_3, "\n");
	}
	//fprintf(file_return,"\n");

	//fprintf(file_return,"B output layer:\n");
	
	FILE * file_4;

	file_4 = fopen("training-result/outputLayer.B","w");


	for (int i = 0; i < nb_outputs; i++) 
	{
		fprintf(file_4, "%lg ", B[i]);	
	}

	fclose(file_1);
	fclose(file_2);
	fclose(file_3);
	fclose(file_4);

}

//training function
void train()
{
    	
	// number of neurons of each layer
	int nb_inputs = 2;
	int nb_hiddenNeurons = 2;
	int nb_outputs = 1;

	// traning input values
    	double inputs[4][2] = {{0.0f,0.0f},
    			{0.0f,1.0f},
				{1.0f,0.0f},
				{1.0f,1.0f}};
	
	// traning output values
	double expected_output[4] = {0.0f, 1.0f, 1.0f, 0.0f};

	double hiddenLayer[nb_hiddenNeurons];		// la couche cachée entre les input et
							// l'output : coucje à 2 neurones
	double output[nb_outputs];			// la couche de sortie : 1 neurone


	double Z_hidden[nb_hiddenNeurons];
	double Z_output[nb_outputs];


	double hiddenLayerB[nb_hiddenNeurons];		// la liste de biais des neurones de
							// la couche caché
	double outputB[nb_outputs];			// la liste de biais des neurones de l'output

	double hiddenLayerW[nb_inputs][nb_hiddenNeurons]; 
	// la liste de poids entre les 2 inputs et les deux neurones
	// de la couche hidden
	
	double outputW[nb_hiddenNeurons][nb_outputs];	// la liste de poidsd entre 
							// les 2 neurones cachés et l'ouptut


	const double learning_rate = 0.16f;		// learning coef



	computeW(nb_inputs,nb_hiddenNeurons,hiddenLayerW);
	computeW(nb_hiddenNeurons,nb_outputs,outputW);
	

	computeB(nb_hiddenNeurons,hiddenLayerB);
	computeB(nb_outputs,outputB);

	print_layer(hiddenLayerW[0],nb_hiddenNeurons);
	print_layer(outputW[0],nb_outputs);


	// traning of the neural network for 10000 epochs
	int steps[] = {0,1,2,3};

	for (int epoch = 0; epoch <= 10; epoch++)
	{
	    	//printf("epoch %d/10000 \r",epoch);
		//fflush(stdout);

		if (epoch == 6)
			    printf("...\n");

	    	if (epoch < 5 || epoch > 9995)
		{
			printf("\n");
	    		printf("epoch: %d	---------------------------------------------------\n", epoch);
		}
	    	shuffleDataSet(steps, 4);

		for(int x = 0; x < 4; x++)
		{
		    	// FORWARD PROPAGATION
		    	int i = steps[x];
						
			computeZ(nb_inputs, nb_hiddenNeurons, inputs[i], hiddenLayerW, hiddenLayerB,Z_hidden);
			computeA(nb_hiddenNeurons, Z_hidden, hiddenLayer);
	
			computeZ(nb_hiddenNeurons, nb_outputs, hiddenLayer, outputW, outputB,Z_output);
			computeA(nb_outputs, Z_output, output);

			/*
			computeActivation(nb_inputs, nb_hiddenNeurons, inputs[i], hiddenLayerW,
				hiddenLayerB, hiddenLayer);

			computeActivation(nb_hiddenNeurons, nb_outputs, hiddenLayer, outputW,
				outputB, output);
			*/
			if (epoch < 5 || epoch > 9995)
			{
				printf("Input: %f  %f  |  Output: %f  |  Expected Output: %f \n",
					inputs[i][0],inputs[i][1],
					output[0],expected_output[i]);
			}
	


			// BACK PROPAGATION
			//
			// compute the Error on the output layer
			double err_output[nb_outputs];
			
			for (int j = 0; j < nb_outputs; j++) 
			{
				double error = (expected_output[i] - output[j]) ;
				err_output[j] = error * dSigmoid(output[j]);
			}
			
			// compute the Error on the hidden layer
			double err_hiddenLayer[nb_hiddenNeurons];
			for (int j = 0; j < nb_hiddenNeurons; j++) 
			{
				double error = 0.0f;
				for (int k = 0; k < nb_outputs; k++) 
				{
					error += err_output[k] * outputW[j][k];

				}
				err_hiddenLayer[j] = error * dSigmoid(hiddenLayer[j]);
			}

			// apply changes
			// output layer
			modifWeightBiais(nb_outputs, nb_hiddenNeurons, outputW, outputB,
				err_output, hiddenLayer, learning_rate);

			// hidden layer
			modifWeightBiais(nb_hiddenNeurons, nb_inputs, hiddenLayerW, hiddenLayerB,
				err_hiddenLayer, inputs[i], learning_rate);

		}
	}

	save(nb_inputs, nb_hiddenNeurons, nb_outputs, hiddenLayerW, hiddenLayerB, outputW, outputB);

}


// to get finals W after training from files
int to_W(int layer, int next_layer, double W[layer][next_layer], char *f)
{
	
    	FILE* file = fopen(f,"r");

	if (file == NULL)
	{
	    	//printf("FILE DOES NOT EXIST\"training is needed");
		return 1;
	}
    	
	for (int i = 0; i < layer; i++) 
	{
		for (int j = 0; j < next_layer; j++) 
		{
			fscanf(file,"%lf",&W[i][j]);		
		}
	}

	fclose(file);
	return 0;

}

// to get finals Biais after training from files
int to_B(int layer, double B[layer], char *f)
{
	
    	FILE* file = fopen(f,"r");

	if (file == NULL)
	{
	    	//printf("FILE DOES NOT EXIST\"training is needed");
		return 1;
	}
 	
	for (int i = 0; i < layer; i++) 
	{
		fscanf(file,"%lf",&B[i]);		
		
	}

	fclose(file);
	return 0;

}



int main(int argc, char *argv[])
{
    	if (argc == 1)
	{
	    	printf("training...\n");
	    	train();
		printf("\ndone.\n");

		return 0;
	}
	else if(argc != 3)
	{
	    	printf("INPUTS ERROR");
		return 1;	
	}
	else
	{
	   	for(int i = 1; i <=2; i++)
		{
		    	if (*argv[i] != '1' && *argv[i] != '0')
			{
			    	printf("INPUTS ERROR : 0 1 / 1 1 / 0 0 / 1 0");
			    	return 1;
			}
		}

		// number of neurons of each layer
		int nb_inputs = 2;
		int nb_hiddenNeurons = 2;
		int nb_outputs = 1;

		// traning input values
    		
		// traning output values
		//double expected_output[4] = {0.0f, 1.0f, 1.0f, 0.0f};

		double hiddenLayer[nb_hiddenNeurons];				
		double output[nb_outputs];	

		double Z_hidden[nb_hiddenNeurons];
		double Z_output[nb_outputs];

		double hiddenLayerB[nb_hiddenNeurons];
		double outputLayerB[nb_outputs];

		double hiddenLayerW[nb_inputs][nb_hiddenNeurons]; 
		double outputLayerW[nb_hiddenNeurons][nb_outputs];
		
    	int t1 = to_W(nb_inputs,nb_hiddenNeurons,
			hiddenLayerW,"training-result/hiddenLayer.W");
		int t2 = to_B(nb_hiddenNeurons,
			hiddenLayerB,"training-result/hiddenLayer.B");
		int t3 =to_W(nb_hiddenNeurons,nb_outputs,
			outputLayerW,"training-result/outputLayer.W");
		int t4 =to_B(nb_outputs,
			outputLayerB,"training-result/outputLayer.B");

		if(t1 || t2 || t3 || t4)
		{
		    	
			printf("A FILE DOES NOT EXIST\ntraining is needed");
		    	return 1;
		}


		double inputs[nb_inputs];

		for(int i = 0; i< nb_inputs;i++)
		{
		    	inputs[i] = (float)strtod(argv[i+1],NULL);
		}


		computeZ(nb_inputs, nb_hiddenNeurons, inputs, hiddenLayerW, hiddenLayerB,
			Z_hidden);

		computeA(nb_hiddenNeurons, Z_hidden, hiddenLayer);

			
		computeZ(nb_hiddenNeurons, nb_outputs, hiddenLayer, outputLayerW, outputLayerB,
			Z_output);

		computeA(nb_outputs, Z_output, output);
		
		int r = output[0] + 0.5;

		printf("Input: %f %f  |  Output: %f  |  Expected Output: %d \n",
			inputs[0],inputs[1],output[0],r);
			
		return 0;
		
	}

	
}







