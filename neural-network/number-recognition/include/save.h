#ifndef SAVE_H
#define SAVE_H

void save(int nb_inputs, int nb_hiddenNeurons, int nb_hiddenNeurons2, int nb_outputs,
	double W1[nb_inputs][nb_hiddenNeurons], double B1[nb_hiddenNeurons],
	double W2[nb_hiddenNeurons][nb_hiddenNeurons2], double B2[nb_hiddenNerons]
	double W3[nb_hiddenNeurons2][nb_outputs], double B3[nb_outputs]);

#endif
