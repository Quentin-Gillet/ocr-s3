#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
			printf("%f\n",W[i][j]);
    		}
    	}
}

// to fill the array of bias: random bias for the first epoch
void computeB(int next_layer, double B[next_layer])
{
    	for (int i = 0; i < next_layer; i++) 
    	{
		B[i] = randomWB();
		printf("%f\n",B[i]);
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
void computeActivation(int layer, int next_layer, double X[layer], double W[layer][next_layer], double B[next_layer], double A[next_layer])
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
void modifWeightBiais(int layer, int back_layer, double W[back_layer][layer], double B[back_layer], double Error[layer], double X[back_layer], double lr) 
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


int main()
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


	// traning of the neural network for 10000 epochs
	int steps[] = {0,1,2,3};

	for (int epoch = 0; epoch < 10000; epoch++)
	{
		printf("\n");
	    	printf("epoch: %d	---------------------------------------------------\n", epoch);
	    	shuffleDataSet(steps, 4);

		for(int x = 0; x < 4; x++)
		{
		    	// FORWARD PROPAGATION
		    	int i = steps[x];
						
			computeZ(nb_inputs, nb_hiddenNeurons, inputs[i], hiddenLayerW, hiddenLayerB,
				Z_hidden);

			computeA(nb_hiddenNeurons, Z_hidden, hiddenLayer);

			
			computeZ(nb_hiddenNeurons, nb_outputs, hiddenLayer, outputW, outputB,
				Z_output);

			computeA(nb_outputs, Z_output, output);

			/*
			computeActivation(nb_inputs, nb_hiddenNeurons, inputs[i], hiddenLayerW,
				hiddenLayerB, hiddenLayer);

			computeActivation(nb_hiddenNeurons, nb_outputs, hiddenLayer, outputW,
				outputB, output);
			*/

			printf("Input: %f  %f |  Output: %f   |  Expected Output: %f \n",
				inputs[i][0],inputs[i][1],
				output[0],expected_output[i]);



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

	return 0;
}
