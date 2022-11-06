#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "save.h"

void save(int nb_inputs, int nb_hiddenNeurons, int nb_hiddenNeurons2, int nb_outputs,
	double W1[nb_inputs][nb_hiddenNeurons], double B1[nb_hiddenNeurons],
	double W2[nb_hiddenNeurons][nb_hiddenNeurons2], double B2[nb_hiddenNeurons],
	double W3[nb_hiddenNeurons2][nb_outputs], double B3[nb_outputs])
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

	file_3 = fopen("training-result/hiddenLayer2.W","w");

	//fprintf(file_return,"W output layer:\n ");

	for(int i = 0; i < nb_hiddenNeurons2; i++)
	{
	    	for (int j = 0; j < nb_hiddenNeurons; j++) 
		{
	    		fprintf(file_3, "%lg ", W2[i][j]);
		
	    	}
		if (i != nb_hiddenNeurons2 -1)
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




	FILE * file_5;

	file_5 = fopen("training-result/outputLayer.W","w");

	//fprintf(file_return,"W output layer:\n ");

	for(int i = 0; i < nb_hiddenNeurons2; i++)
	{
	    	for (int j = 0; j < nb_outputs; j++) 
		{
	    		fprintf(file_5, "%lg ", W3[i][j]);
		
	    	}
		if (i != nb_hiddenNeurons -1)
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



