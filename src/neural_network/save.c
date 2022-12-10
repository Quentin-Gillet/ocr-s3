#include "neural_network/save.h"

// to get finals W after training from files
void to_W(int layer, int next_layer, double **W, char* f)
{

	FILE* file = fopen(f, "r");

	if (file == NULL)
	{
		errx(1,"FILE DOES NOT EXIST\"training is needed");
	}

	for (int i = 0; i < layer; i++)
	{
		for (int j = 0; j < next_layer; j++)
		{
			fscanf(file, "%lf", &W[i][j]);
		}
	}

	fclose(file);

}

// to get finals Biais after training from files
void to_B(int layer, double *B, char* f)
{

	FILE* file = fopen(f, "r");

	if (file == NULL)
	{
		errx(1,"FILE DOES NOT EXIST\"training is needed");
	}

	for (int i = 0; i < layer; i++)
	{
		fscanf(file, "%lf", &B[i]);
	}

	fclose(file);

}

//W1[nb_inputs][nb_hiddenNeurons], B1[nb_hiddenNeurons]
//W2[nb_hiddenNeurons][nb_hiddenNeurons2], B2[nb_hiddenNerons]
//W3[nb_hiddenNeurons2][nb_outputs], B3[nb_outputs]
void save(int nb_inputs, int nb_hiddenNeurons, int nb_hiddenNeurons2, int nb_outputs,
		double** W1, double* B1, double** W2, double* B2, double** W3, double* B3)
{
	mkdir("training-result",0777);


	//fprintf(file_1,"W hidden layer:\n");	
	FILE * file_1;
	file_1 = fopen("training-result/hiddenLayer1.W","w");
	for(int i = 0; i < nb_inputs; i++)
	{	
	    for (int j = 0; j < nb_hiddenNeurons; j++) 
		{
	    	fprintf(file_1, "%lg ", W1[i][j]);
	   	}
		if (i != nb_inputs -1)
	    	fprintf(file_1, "\n");
	}
	//fprintf(file_return,"\n");

	//fprintf(file_return,"B hidden layer:\n");
	FILE * file_2;
	file_2 = fopen("training-result/hiddenLayer1.B","w");
	for (int i = 0; i < nb_hiddenNeurons; i++) 
	{
		fprintf(file_2, "%lg ", B1[i]);	
	}


	//fprintf(file_return,"W output layer:\n ");
	FILE * file_3;
	file_3 = fopen("training-result/hiddenLayer2.W","w");
	for(int i = 0; i < nb_hiddenNeurons; i++)
	{
		for (int j = 0; j < nb_hiddenNeurons2; j++) 
		{
			fprintf(file_3, "%lg ", W2[i][j]);
	
		}
		if (i != nb_hiddenNeurons -1)
	    	fprintf(file_3, "\n");
	}
	//fprintf(file_return,"\n");

	//fprintf(file_return,"B output layer:\n");
	FILE * file_4;
	file_4 = fopen("training-result/hiddenLayer2.B","w");
	for (int i = 0; i < nb_hiddenNeurons2; i++) 
	{
		fprintf(file_4, "%lg ", B2[i]);	
	}


	//fprintf(file_return,"W output layer:\n ");
	FILE * file_5;
	file_5 = fopen("training-result/outputLayer.W", "w");
	for(int i = 0; i < nb_hiddenNeurons2; i++)
	{
	    	for (int j = 0; j < nb_outputs; j++) 
		{
	    		fprintf(file_5, "%lg ", W3[i][j]);
		
	    	}
		if (i != nb_hiddenNeurons2 -1)
	    		fprintf(file_5, "\n");
	}
	//fprintf(file_return,"\n");

	//fprintf(file_return,"B output layer:\n");
	FILE * file_6;
	file_6 = fopen("training-result/outputLayer.B","w");
	for (int i = 0; i < nb_outputs; i++) 
	{
		fprintf(file_6, "%lg ", B3[i]);	
	}

	fclose(file_1);
	fclose(file_2);
	fclose(file_3);
	fclose(file_4);
	fclose(file_5);
	fclose(file_6);

}



