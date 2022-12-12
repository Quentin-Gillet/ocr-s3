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
    //train();
    //int nb_images = 2500;
    
    int nb_images = 81;
    char sudoku[nb_images];

    /*
    char **images = initMatrixChar(nb_images, 1024);
    int numbers[nb_images];
    LoadCSV("images_test_link.csv", images, numbers);
    */

    double **inputs = initMatrixDouble(nb_images, NB_INPUTS);

    const char *path = "images/numbers/";
    //const char *path = "images_test/";

    printf("---------------------------------\n");
    for (int i = 0; i < nb_images; i++) 
    {
        char *name = int_to_stringfile(i);
        //char *name = images[i];
        char *input_image = concat(path, name);
        free(name);
        ImageMajorOrder im = prepareImageFileForNeuralNetwork(input_image);
        
        double input[NB_INPUTS];
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
    double **W_hidden_1 = initMatrixDouble(NB_INPUTS, NB_HIDDEN_N_L1);
    //double W_hidden_2[NB_HIDDEN_N_L1][NB_HIDDEN_N_L2];
    double **W_hidden_2 = initMatrixDouble(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2);
    //double W_output[NB_HIDDEN_N_L2][NB_OUTPUTS];
    double **W_output = initMatrixDouble(NB_HIDDEN_N_L2, NB_OUTPUTS);

    to_W(NB_INPUTS, NB_HIDDEN_N_L1, W_hidden_1, "training-result-01/hiddenLayer1.W");
    to_B(NB_HIDDEN_N_L1, B_hidden_1, "training-result-01/hiddenLayer1.B");

    to_W(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, W_hidden_2, "training-result-01/hiddenLayer2.W");
    to_B(NB_HIDDEN_N_L2, B_hidden_2, "training-result-01/hiddenLayer2.B");

    to_W(NB_HIDDEN_N_L2, NB_OUTPUTS, W_output, "training-result-01/outputLayer.W");
    to_B(NB_OUTPUTS, B_output, "training-result-01/outputLayer.B");

    for (int i = 0; i < nb_images; i++) 
    {
        //printf("\n");
        //print_image(inputs[i]);

        computeZ(NB_INPUTS, NB_HIDDEN_N_L1, inputs[i], W_hidden_1, B_hidden_1, Z_hidden_1);
        computeA_Layer(NB_HIDDEN_N_L1, Z_hidden_1, hiddenLayer1);

        computeZ(NB_HIDDEN_N_L1, NB_HIDDEN_N_L2, hiddenLayer1, W_hidden_2, B_hidden_2, Z_hidden_2);
        computeA_Layer(NB_HIDDEN_N_L2, Z_hidden_2, hiddenLayer2);

        computeZ(NB_HIDDEN_N_L2, NB_OUTPUTS, hiddenLayer2, W_output, B_output, Z_output);
        computeA_Output(NB_OUTPUTS, Z_output, output);

        /*
        if (return_output(output) == numbers[i])
        {
            printf("image %d/%d : %d -- OK\n",i,nb_images-1,return_output(output));
            count++;
        }
        else
        {
            printf("image %d/%d : %d (expected %d)\n",i,nb_images-1,return_output(output),numbers[i]);
        }
        */
        sudoku[i] = int_to_char(return_output(output));
    }
    //printf("ok\n");
    neural_network_to_sudoku(sudoku);
    int resolved = resolve("sudoku");
    if(!resolved)
    {
        Image resultImage = buildSudoku("sudoku.result");
        saveImageToBmp(&resultImage, "sudoku.result", "");
    }
    
    //double winrate = (double) count / (double) nb_images;
    //printf("winrate : %f\n", winrate * 100);

    freeMatrixDouble(W_hidden_1, NB_INPUTS);
    freeMatrixDouble(W_hidden_2, NB_HIDDEN_N_L1);
    freeMatrixDouble(W_output, NB_HIDDEN_N_L2);
    freeMatrixDouble(inputs,nb_images);

    return 0;
}