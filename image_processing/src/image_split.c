//
// Created by alexiscognet on 05/11/22.
//
#define Threshold 4
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

// Get the intersection between two lines
// x = NULL,y = NULL --> means there is no intersection between the 2 lines
struct Intersection line_intersection(struct Line Line1, struct Line Line2)
{
    struct Intersection intersection;
    intersection.x = NULL;
    intersection.y = NULL;
    intersection.line = Line1;

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
struct Intersection* get_intersections(struct Line Line, struct Line otherLines[],int NBLine)
{
    struct Intersection* intersections_list= calloc(9,sizeof(struct Intersection));
    size_t countIntersections = 0;

    for (int i = 0; i < NBLines; i++) {

        struct Line currLine =  otherLines[i];
        struct Intersection intersection = line_intersection(Line,currLine);

        if ((intersection.x == NULL) && (intersection.y == NULL))
            continue;
        else
        {
            intersections_list[countIntersections] = intersection;
            countIntersections++;
        }
    }
    return intersections_list;
}


struct Intersection * __get_Squares(struct Line line, int NBLines,
                                    struct Intersection intersections_square[4],int j, struct Line firstLine, struct Line lines[])
{
    struct Intersection* intersections_line = get_intersections(line,lines, NBLines);
    if (j == 3)
    {
        for(int i = 0; (intersections_line[i].x != NULL) && (intersections_line[i].y != NULL); i++)
        {
            Intersection intersection = intersections_line[i];
            if (firstLine.x1 == intersection.line.x1 && firstLine.x2 == intersection.line.x2 &&
                firstLine.y1 == intersection.line.y1 && firstLine.y2 == intersection.line.y2 )
            {
                intersections_square[i] = intersections_line[i];
                struct Line line1;
                struct Line line2;

                line1.x1 = intersections_square[0].x;
                line1.y1 = intersections_square[0].y;
                line1.x2 = intersections_square[2].x;
                line1.y2 = intersections_square[2].y;

                line2.x1 = intersections_square[1].x;
                line2.y1 = intersections_square[1].y;
                line2.x2 = intersections_square[3].x;
                line2.y2 = intersections_square[3].y;

                struct Intersection diagIntersection = line_intersection(line1, line2);
                if (diagIntersection.x != NULL && diagIntersection.y != NULL)
                    return intersections_square;

            }
            return NULL;
        }
    }
    else
    {
        for(int j = 0; (intersections_line[j].x != NULL) && (intersections_line[j].y != NULL) ; j++)
        {
            intersections_square[j] = intersections_line[j];
            __get_Squares(intersections_line[j].line, NBLines, intersections_square, j + 1, firstLine,lines);
        }
    }
}


struct Square* get_Squares(struct Line * lines, int NBLines)
{
    struct Square* squares;
    struct Square square;
    struct Intersection * intersections_square = calloc(4, sizeof (Intersection));
    int j = 0;

    for(int i = 0; i > NBLines; i++)
    {
        intersections_square = __get_Squares(lines[i], NBLines, intersections_square, 0,lines[0],lines);
        if(intersections_square != NULL)
        {
            intersections_square[0].x = square.xa;
            intersections_square[0].y = square.ya;
            intersections_square[1].x = square.xb;
            intersections_square[1].y = square.yb;
            intersections_square[2].x = square.xc;
            intersections_square[2].y = square.yc;
            intersections_square[3].x = square.xd;
            intersections_square[3].y = square.yd;
            squares[j] = square;
            j++;
        }
    }
    return squares;
}