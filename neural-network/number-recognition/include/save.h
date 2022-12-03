#ifndef SAVE_H
#define SAVE_H

//W1[nb_inputs][nb_hiddenNeurons], B1[nb_hiddenNeurons]
//W2[nb_hiddenNeurons][nb_hiddenNeurons2], B2[nb_hiddenNerons]
//W3[nb_hiddenNeurons2][nb_outputs], B3[nb_outputs]
void save(int nb_inputs, int nb_hiddenNeurons, int nb_hiddenNeurons2, int nb_outputs, 
	double **W1, double *B1, double **W2, double *B2, double **W3, double *B3);

#endif
