#include "../include/line_detection.h"


struct Line* houghTransform(Image* image, float threshold, int* lineLength)
{
	// Save the image dimensions
    const double width = image->width, height = image->height;
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

    unsigned int **accumulator = initMatrix(nbTheta + 1, nbRho + 1);

    // We intialize the accumulator with all the value
    // In the same time, we search for the max value in the accumulator

    unsigned int max = 0;
    double rho;
    int croppedRho;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image->pixels[x][y].pixelAverage == 255)
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
                double r = arrRhos[prev_rho], t = arrThetas[prev_theta];

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

                // Calculate one point of the edge
                x1 = x0 + (int)(diagonal * (-s));
                y1 = y0 + (int)(diagonal * c);

                // Calculate the other point of the edge
                x2 = x0 - (int)(diagonal * (-s));
                y2 = y0 - (int)(diagonal * c);

                struct Line line;
                line.x1 = clamp(x1, 0, image->width);
                line.x2 = clamp(x2, 0, image->width);
                line.y1 = clamp(y1, 0, image->height);
                line.y2 = clamp(y2, 0, image->height);
                line.theta = theta;

				lines[indexLine] = line;
				indexLine++;
            }
        }
    }

	*lineLength = indexLine;

    // Free cos and sin arrays
    free(arrThetas);
    free(arrRhos);

    freeMatrix(accumulator, nbTheta + 1);

	return lines;
}

void drawLineOnImage(Image* image, Line* lines, int linesLength)
{
    for(int i = 0; i < linesLength; i++)
    {
        int x0 = lines[i].x1, x1 = lines[i].x2;
        int y0 = lines[i].y1, y1 = lines[i].y2;

        int dX = abs(x1 - x0);
        int dY = -abs(y1 - y0);

        int sX = x0 < x1 ? 1 : -1;
        int sY = y0 < y1 ? 1 : -1;

        int err = dX + dY;

        while(1)
        {
            if(0 <= x0 && x0 < image->width && 0 <= y0 && y0 < image->height)
            {
                setPixelValue(&image->pixels[x0][y0], 255, 0, 0);
            }

            if(x0 == x1 && y0 == y1)
                break;

            int e2 = 2 * err;
            if(e2 >= dY)
            {
                err += dY;
                x0 += sX;
            }
            if(e2 <= dX)
            {
                err += dX;
                y0 += sY;
            }
        }
    }
}