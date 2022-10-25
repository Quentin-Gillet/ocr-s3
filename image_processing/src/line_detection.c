#include "../include/line_detection.h"


struct Line* houghTransform(SDL_Surface* surface, float threshold, int* lineLength)
{
	// Save the image dimensions
    const double width = surface->w, height = surface->h;
    // Calculate the diagonal of the image
    const double diagonal = sqrt(width * width + height * height);

    // Initialize the constant values for theta and rho
    const double maxTheta = 180.0, minTheta = 0.0;
    const double maxRho = diagonal, minRho = -diagonal;
    const double nbRho = 2 * diagonal, nbTheta = nbRho;

    // Create an array of rhos
    double rhoStep = (maxRho - minRho) / nbRho;
    double *arrRhos = calloc(nbRho + 1, sizeof(double));
    int index = 0;
    for (double val = minRho; val <= maxRho && index < nbTheta;
         val += rhoStep, index++)
    {
        arrRhos[index] = val;
    }

    // Create an array of theta
    double step = (maxTheta - minTheta) / nbTheta;
    double *arrThetas = calloc(nbTheta + 1, sizeof(double));
    index = 0;
    for (double val = minTheta; val <= maxTheta && index < nbTheta;
         val += step, index++)
    {
        arrThetas[index] = val;
    }

    // Create a save of cos and sin value for each theta, to optimize
    // performance.
    double *saveCos = calloc(nbTheta + 1, sizeof(double));
    double *saveSin = calloc(nbTheta + 1, sizeof(double));
    for (int theta = 0; theta < nbTheta; theta++)
    {
        // Convert each value of theta's array into radians
        arrThetas[theta] = degToRad(arrThetas[theta]);

        // Save each cos(theta) and sin(theta) into their respective arrays
        saveCos[theta] = cos(arrThetas[theta]);
        saveSin[theta] = sin(arrThetas[theta]);
    }

    unsigned int **accumulator = initMatrice(nbTheta + 1, nbRho + 1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator

    unsigned int max = 0;
    double rho;
    int croppedRho;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
			Uint32 pixel = getPixel(surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            if ((r + g + b) / 3 == 255)
            {
                for (int theta = 0; theta <= nbTheta; theta++)
                {
                    rho = x * saveCos[theta] + y * saveSin[theta];
                    croppedRho = rho + diagonal;
                    accumulator[croppedRho][theta]++;
                    if (accumulator[croppedRho][theta] > max)
                    {
                        max = accumulator[croppedRho][theta];
                    }
                }
            }
        }
    }

    free(saveCos);
    free(saveSin);

	int lineThreshold = max * threshold;

	struct Line* lines = calloc((int)(nbTheta * nbRho + 2), sizeof(struct Line));
	double tempMaxTheta = 0.0;
    unsigned int histogram[181] = { 0 };
    unsigned int rounded_angle;

    int prev = accumulator[0][0];
    int prev_theta = 0, prev_rho = 0;
    int boolIsIncreasing = 1;
	int indexLine = 0;

	for (int theta = 0; theta <= nbTheta; theta++)
    {
        for (int rho = 0; rho <= nbRho; rho++)
        {
            int val = accumulator[rho][theta];

            if (val >= prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                boolIsIncreasing = 1;
                continue;
            }
            else if (val < prev && boolIsIncreasing)
            {
                boolIsIncreasing = 0;
            }
            else if (val < prev)
            {
                prev = val;
                prev_rho = rho;
                prev_theta = theta;
                continue;
            }

            if (val >= lineThreshold)
            {
                double r = arrRhos[rho], t = arrThetas[theta];

                if (t > tempMaxTheta)
                {
                    tempMaxTheta = t;
                    rounded_angle = (unsigned int)radToDeg(t);
                    histogram[rounded_angle]++;
                }

                double c = cos(t), s = sin(t);
                int x0, y0, x1, x2, y1, y2;
                // Calculate d0 point
                x0 = (int)(c * r);
                y0 = (int)(s * r);

                x1 = (int)(x0 + 1000 * (-s));
                y1 = (int)(y0 + 1000 * (c));
                x2 = (int)(x0 - 1000 * (-s));
                y2 = (int)(y0 - 1000 * (c));

                // Calculate one point of the edge
                /*x1 = x0 + (int)(diagonal * (-s));
                y1 = y0 + (int)(diagonal * c);

                // Calculate the other point of the edge
                x2 = x0 - (int)(diagonal * (-s));
                y2 = y0 - (int)(diagonal * c);*/

                struct Line line;
                line.x1 = truncate(x1, 0, surface->w);
                line.x2 = truncate(x2, 0, surface->w);
                line.y1 = truncate(y1, 0, surface->h);
                line.y2 = truncate(y2, 0, surface->h);

				lines[indexLine] = line;
				indexLine++;
            }
        }
    }

	*lineLength = indexLine;

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);

	freeMatrice(accumulator, nbTheta + 1);

	return lines;
}