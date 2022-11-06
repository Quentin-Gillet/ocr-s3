//
// Created by alexiscognet on 05/11/22.
//
#define Threshold 2
#include "../include/image_split.h"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

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
struct Line* reduce_lines(struct Line* lines, int nbLines, int* temp)
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
    *temp = countLines;
    return newlines;
}

int approx(int a, int b)
{
    return (abs(a-b) <= Threshold) ? 1 : 0;
}
// Get the intersection between two lines
// x = NULL,y = NULL --> means there is no intersection between the 2 lines
struct Intersection line_intersection(struct Line Line1, struct Line Line2)
{
    struct Intersection intersection;
    intersection.x = NULL;
    intersection.y = NULL;

    int x1 = Line1.x1;
    int x2 = Line1.x2;
    int y1 = Line1.y1;
    int y2 = Line1.y2;

    int x3 = Line2.x1;
    int x4 = Line2.x2;
    int y3 = Line2.y1;
    int y4 = Line2.y2;

    int d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // If d is zero, there is no intersection
    if (d == 0)
    {
        return intersection;
    }

    int pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    int x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    int y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

    if ( x < MIN(x1, x2) || x > MAX(x1, x2) ||
         x < MIN(x3, x4) || x > MAX(x3, x4) )
        return intersection;

    if ( y < MIN(y1, y2) || y > MAX(y1, y2) ||
         y < MIN(y3, y4) || y > MAX(y3, y4) )
        return intersection;

    intersection.x = x;
    intersection.y = y;

    return intersection;

}

// max intersection = 9
struct Intersection* get_intersections(struct Line* lines, int NBLines)
{
    struct Intersection* intersections_list= calloc(9,sizeof(struct Intersection));
    size_t countIntersections = 0;
    for(int i = 0; i < NBLines; ++i)
    {
        Line currLine =  lines[i];
        for (int j = 0; j < NBLines; j++) {

            if(j == i)
                continue;

            Line otherLine =  lines[j];
            struct Intersection intersection = line_intersection(currLine,otherLine);

            if ((intersection.x == NULL) && (intersection.y == NULL))
                continue;
            else
            {
                intersections_list[countIntersections] = intersection;
                countIntersections++;
            }
        }
    }
    return intersections_list;
}


struct Square * get_Squares(struct Line * lines,int NBLines)
{

}
