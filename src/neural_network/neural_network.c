//
// Created by Quentin Gillet on 10/12/2022.
//

#include "neural_network/neural_network.h"

char int_to_char(int number)
{
    if (number == 0)
        return '.';
    char c = '0' + number;
    return c;
}


char* int_to_stringfile(int number)
{
    char *str = malloc(sizeof(char) * 10);
    sprintf(str, "%d.bmp", number);
    return str;
}

int Recognition_Solve()
{
    int nb_images = 81;
    char sudoku[nb_images];

    // matrice de 81 lignes et 784 colonnes.
    double **inputs = initMatrixDouble(nb_images, NB_INPUTS); 

    // chemin vers les images découpées.
    const char *path = "images/numbers/";
    
    // Pre-processing des images.
    printf("---------------------------------\n");
    for (int i = 0; i < nb_images; i++) 
    {
        // récuperation du nom de l'image. i.bmp
        char *name = int_to_stringfile(i);
        char *input_image = concat(path, name);
        free(name);
          
        ImageMajorOrder im = prepareImageFileForNeuralNetwork(input_image);
        
        double input[NB_INPUTS];
        // convertir l'image en un tableau de double 0 ou 1.
        image_to_input(im, input);

        for (int j = 0; j < NB_INPUTS; j++) 
        {
            inputs[i][j] = input[j];
        }
        printf("pre-processing : image %d/%d\r", i+1, nb_images);
        fflush(stdout);

        free(im.pixels);
        free(input_image);
    }
    printf("\n");

    // Initialisation du réseau de neurones.
    double hiddenLayer1[NB_HIDDEN_N_L1];
    double hiddenLayer2[NB_HIDDEN_N_L1];
    double output[NB_OUTPUTS];

    double Z_hidden_1[NB_HIDDEN_N_L1];
    double Z_hidden_2[NB_HIDDEN_N_L2];
    double Z_output[NB_OUTPUTS];

    // initialisation des poids et des biais.
    double B_hidden_1[NB_HIDDEN_N_L1];
    double B_hidden_2[NB_HIDDEN_N_L2];
    double B_output[NB_OUTPUTS];

    //double W_hidden_1[NB_INPUTS][NB_HIDDEN_N_L1];
    double **W_hidden_1 = initMatrixDouble(NB_INPUTS, NB_HIDDEN_N_L1);
    //double W_hidden_2[NB_HIDDEN_N_L1][NB_HIDDEN_N_L2];
    double **W_hidden_2 = initMatrixDouble(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2);
    //double W_output[NB_HIDDEN_N_L2][NB_OUTPUTS];
    double **W_output = initMatrixDouble(NB_HIDDEN_N_L2, NB_OUTPUTS);

    // chargement des poids et des biais à partir de l'entrainement.
    to_W(NB_INPUTS, NB_HIDDEN_N_L1, W_hidden_1, "training-result-01/hiddenLayer1.W");
    to_B(NB_HIDDEN_N_L1, B_hidden_1, "training-result-01/hiddenLayer1.B");

    to_W(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, W_hidden_2, "training-result-01/hiddenLayer2.W");
    to_B(NB_HIDDEN_N_L2, B_hidden_2, "training-result-01/hiddenLayer2.B");

    to_W(NB_HIDDEN_N_L2, NB_OUTPUTS, W_output, "training-result-01/outputLayer.W");
    to_B(NB_OUTPUTS, B_output, "training-result-01/outputLayer.B");

    // calcul de la sortie du réseau de neurones en fonction de l'image d'enrtrée.
    for (int i = 0; i < nb_images; i++) 
    {
        computeZ(NB_INPUTS, NB_HIDDEN_N_L1, inputs[i], W_hidden_1, B_hidden_1, Z_hidden_1);
        computeA_Layer(NB_HIDDEN_N_L1, Z_hidden_1, hiddenLayer1);

        computeZ(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, hiddenLayer1, W_hidden_2, B_hidden_2, Z_hidden_2);
        computeA_Layer(NB_HIDDEN_N_L2, Z_hidden_2, hiddenLayer2);

        computeZ(NB_HIDDEN_N_L2, NB_OUTPUTS, hiddenLayer2, W_output, B_output, Z_output);
        computeA_Output(NB_OUTPUTS, Z_output, output);

        sudoku[i] = int_to_char(return_output(output));
    }
    // transformation du tableau de char en fichier texte contenant le sudoku.
    neural_network_to_sudoku(sudoku);

    // résolution du sudoku.
    int resolved = resolve("sudokuPresentation");
    if(!resolved)
    {
        Image resultImage = buildSudoku("sudokuPresentation.result");
        saveImageToBmp(&resultImage, "sudokuPresentation.result", "");
    }

    freeMatrixDouble(W_hidden_1, NB_INPUTS);
    freeMatrixDouble(W_hidden_2, NB_HIDDEN_N_L1);
    freeMatrixDouble(W_output, NB_HIDDEN_N_L2);
    freeMatrixDouble(inputs,nb_images);

    return 0;
}