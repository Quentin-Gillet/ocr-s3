//
// Created by alexiscognet on 05/11/22.
//
#define Threshold 5
#include "image_split.h"

// réduit le nombre de ligne
// Compte le nombre de ligne que l'on va avoir à la fin de la réduction
// du nombre de ligne
size_t NBLines(struct Line* lines, int nbLines )
{
    size_t cpt = 0;
    for(int i = 0; i < nbLines; ++i)
    {
        Line currLine =  lines[i];
        for (int j = 0; j < nbLines; j++) {

            if(j == i)
                continue;
            Line otherLine =  lines[j];
            int x0d = currLine.x1;
            int y0d = currLine.y1;
            int x0f = currLine.x2;
            int y0f = currLine.y2;

            int x1d = otherLine.x1;
            int y1d = otherLine.y1;
            int x1f = otherLine.x2;
            int y1f = otherLine.y2;

            if(approx(x0d,x1d)
               && approx(y0d,y1d)
               && approx(x0f,x1f)
               && approx(y0f,y1f))
            {
                cpt++;
            }

        }

    }
    return cpt;
}


// Réduit le nombre de lignes résultant de la transformation de Hough
struct Line* reduce_lines(struct Line* lines, int nbLines )
{
    struct Line* newlines = calloc(NBLines(lines,nbLines), sizeof(struct Line));
    size_t countLines = 0;
    for(int i = 0; i < nbLines; ++i)
    {
        Line currLine =  lines[i];
        for (int j = 0; j < nbLines; j++) {

            if(j == i)
                continue;
            Line otherLine =  lines[j];

            int x0d = currLine.x1;
            int y0d = currLine.y1;
            int x0f = currLine.x2;
            int y0f = currLine.y2;

            int x1d = otherLine.x1;
            int y1d = otherLine.y1;
            int x1f = otherLine.x2;
            int y1f = otherLine.y2;

            if(approx(x0d,x1d)
            && approx(y0d,y1d)
            && approx(x0f,x1f)
            && approx(y0f,y1f))
            {
                newlines[countLines].x1 = (x0d + x1d)/2;
                newlines[countLines].y1 = (y0d + y1d)/2;
                newlines[countLines].x2 = (x0f + x1f)/2;
                newlines[countLines].y2 = (y0f + y1f)/2;
                countLines++;
            }

        }

    }
    return newlines;
}

int approx(int a, int b)
{
    return (abs(a-b) <= Threshold) ? 1 : 0;
}