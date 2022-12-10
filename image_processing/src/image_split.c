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

            if(approx(x0d,x1d, 2)
               && approx(y0d,y1d, 2)
               && approx(x0f,x1f, 2)
               && approx(y0f,y1f, 2))
            {
                cpt++;
            }
        }
    }
    return cpt;
}


// Réduit le nombre de lignes résultant de la transformation de Hough
struct Line* reduce_lines(struct Line* lines, int nbLines, int* newLinesCount)
{
    struct Line* newlines = calloc(NBLines(lines,nbLines), sizeof(struct Line));
    int countLines = 0;
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

            if(approx(x0d,x1d, 2)
               && approx(y0d,y1d, 2)
               && approx(x0f,x1f, 2)
               && approx(y0f,y1f, 2))
            {
                newlines[countLines].x1 = (x0d + x1d)/2;
                newlines[countLines].y1 = (y0d + y1d)/2;
                newlines[countLines].x2 = (x0f + x1f)/2;
                newlines[countLines].y2 = (y0f + y1f)/2;
                countLines++;
            }
        }
    }
    *newLinesCount = countLines;
    return newlines;
}

int approx(int a, int b, int Threshold)
{
    return (abs(a-b) <= Threshold) ? 1 : 0;
}

// Get the intersection between two lines
// x = -1,y = -1 --> means there is no intersection between the 2 lines
struct Intersection line_intersection(struct Line Line1, struct Line Line2)
{
    struct Intersection intersection;
    intersection.x = -1;
    intersection.y = -1;
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


//Length of a line
double LineLength(int x1, int x2, int y1, int y2)
{
    double px = abs(x1 - x2);
    double py = abs(y1 - y2);
    return sqrt(px * px + py * py);
}

//Perimeter of a square
double SquareArea(struct Intersection inter1, struct Intersection inter2, struct Intersection inter3)
{
     return LineLength(inter1.x, inter2.x, inter1.y, inter2.y) *
            LineLength(inter2.x, inter3.x, inter2.y, inter3.y);
}

//If the biggerSquare is a real square
int IsSquare(struct Square square)
{
    int len1 = (int)LineLength(square.xa, square.ya, square.xb, square.yb);
    int len2 = (int)LineLength(square.xb, square.yb, square.xc, square.yc);
    int len3 = (int)LineLength(square.xc, square.yc, square.xd, square.yd);
    int len4 = (int)LineLength(square.xd, square.yd, square.xa, square.ya);
    return  approx(len1, len2, 1000) &&
            approx(len2, len3, 1000) &&
            approx(len3, len4, 1000);
}

//Initialisation new square
void square_init(struct Intersection inter1, struct Intersection inter2,
                          struct Intersection inter3, struct Intersection inter4, struct Square *square)
{
    square->xa = inter1.x;
    square->ya = inter1.y;
    square->xb = inter2.x;
    square->yb = inter2.y;
    square->xc = inter3.x;
    square->yc = inter3.y;
    square->xd = inter4.x;
    square->yd = inter4.y;
    square->perimeter = SquareArea(inter1, inter2, inter3);
}

//Initialise a null Square (all values == -1)
struct Square null_Square()
{
    struct Square square = {
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1
    };
    return square;
}

int compare_intersections(Intersection inter1, Intersection inter2, Intersection inter3, Intersection inter4)
{
    if(inter1.x == inter2.x)
    {
        if(inter1.y == inter2.y)
            return 0;
    }
    if(inter1.x == inter3.x)
    {
        if(inter1.y == inter3.y)
            return 0;
    }
    if(inter1.x == inter4.x)
    {
        if(inter1.y == inter4.y)
            return 0;
    }
    if(inter2.x == inter3.x)
    {
        if(inter2.y == inter3.y)
        {
            return 0;
        }
    }
    if(inter2.x == inter4.x)
    {
        if(inter2.y == inter4.y)
            return 0;
    }
    if(inter3.x == inter4.x)
    {
        if(inter3.y == inter4.y)
            return 0;
    }
    return 1;
}

//Get all squares of the grid
void get_all_squares(struct Line *lines, int NBLine, struct list *squares)
{
    struct Square square;
    struct Intersection inter1;
    struct Intersection inter2;
    struct Intersection inter3;
    struct Intersection inter4;

    for(int h = 0; h < NBLine; h++)
    {
        for(int i = 1; i < NBLine; i++)
        {
            if (h == i)
                continue;
            inter1 = line_intersection(lines[h], lines[i]);
            if(inter1.x != -1 && inter1.y != -1)
            {
                for(int j = 2; j < NBLine; j++)
                {
                    if (h == j && i == j)
                        continue;
                    inter2 = line_intersection(lines[i], lines[j]);
                    if(inter2.x != -1 && inter2.y != -1)
                    {
                        for(int k = 3; k < NBLine; k++)
                        {
                            if (h == k && i == k && j == k)
                                continue;
                            inter3 = line_intersection(lines[j], lines[k]);
                            if(inter3.x != -1 && inter3.y != -1)
                            {
                                if(k == h)
                                    continue;
                                inter4 = line_intersection(lines[k], lines[h]);
                                if(inter4.x != -1 && inter4.y != -1)
                                {
                                    if(compare_intersections(inter1, inter2, inter3, inter4))
                                    {
                                        square_init(inter1, inter2, inter3, inter4, &square);
                                        if(!IsSquare(square))
                                            continue;
                                        list_push_front(squares, square);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

struct Line* print_squares(struct Line* lines, int NBLine)
{
    struct list *squares = malloc(sizeof(struct list));
    list_init(squares);
    get_all_squares(lines, NBLine, squares);
    printf("%u", squares->length);

    struct Line *final = calloc(squares->length * 4, sizeof(struct Line));
    struct list *p = squares->next;
    for(int i = 0; p; p = p->next, i += 4)
    {
        struct Line line1 = {
                .x1 = p->data.xa,
                .y1 = p->data.ya,
                .x2 = p->data.xb,
                .y2 = p->data.yb
        };
        struct Line line2 = {
                .x1 = p->data.xb,
                .y1 = p->data.yb,
                .x2 = p->data.xc,
                .y2 = p->data.yc
        };
        struct Line line3 = {
                .x1 = p->data.xc,
                .y1 = p->data.yc,
                .x2 = p->data.xd,
                .y2 = p->data.yd
        };
        struct Line line4 = {
                .x1 = p->data.xd,
                .y1 = p->data.yd,
                .x2 = p->data.xa,
                .y2 = p->data.ya
        };
        final[i] = line1;
        final[i + 1] = line2;
        final[i + 2] = line3;
        final[i + 3] = line4;
    }
    free_list(squares);
    return final;
}

//Get the bigger square of the grid
struct Line* get_Bigger_Squares(struct Line* lines, int NBLine)
{
    struct list *squares = malloc(sizeof(struct list));
    list_init(squares);
    get_all_squares(lines, NBLine, squares);
    struct Square biggerSquare = null_Square();
    for(struct list *p = squares->next; p; p = p->next)
    {
        if(p->data.perimeter > biggerSquare.perimeter)
        {
            biggerSquare = p->data;
        }
    }
    free_list(squares);

    struct Line *final;
    final = calloc(4, sizeof(struct Line));


    struct Line line1 = {
            .x1 = biggerSquare.xa,
            .y1 = biggerSquare.ya,
            .x2 = biggerSquare.xb,
            .y2 = biggerSquare.yb
    };
    struct Line line2 = {
            .x1 = biggerSquare.xb,
            .y1 = biggerSquare.yb,
            .x2 = biggerSquare.xc,
            .y2 = biggerSquare.yc
    };
    struct Line line3 = {
            .x1 = biggerSquare.xc,
            .y1 = biggerSquare.yc,
            .x2 = biggerSquare.xd,
            .y2 = biggerSquare.yd
    };
    struct Line line4 = {
            .x1 = biggerSquare.xd,
            .y1 = biggerSquare.yd,
            .x2 = biggerSquare.xa,
            .y2 = biggerSquare.ya
    };
    final[0] = line1;
    final[1] = line2;
    final[2] = line3;
    final[3] = line4;
    return final;
}

//-----------------AutoRotation-----------//

int find_angle(struct Line line)
{
    double len1 = LineLength(line.x1, line.x2, line.y1, line.y1);
    double len2 = LineLength(line.x1, line.x2, line.y1, line.y2);
    return radToDeg(acos(len1 / len2));
}

void auto_rotation(Image* image, struct Intersection* intersection)
{
    struct Line line1 = {
            intersection[0].x,
            intersection[0].y,
            intersection[1].x,
            intersection[1].y,
            find_angle(line1)
    };
    struct Line line2 = {
            intersection[1].x,
            intersection[1].y,
            intersection[2].x,
            intersection[2].y,
            find_angle(line2)
    };
    if(approx(line1.theta, line2.theta, 20))
    {
        line2.x1 = intersection[2].x;
        line2.y1 = intersection[2].y;
        line2.x2 = intersection[3].x;
        line2.y2 = intersection[3].y;
        line2.theta = find_angle(line2);
    }
    int rotationAngle = 90 - MAX(line1.theta, line2.theta);
    imageRotate(image, rotationAngle);
}

//-----------------AutoRotation-----------//


//-----------------FindCell-----------//


Image cropImage(Image *image, SDL_Rect *rect)
{
    Image res = {
            rect->w,
            rect->h,
            malloc((rect->w + 1) * sizeof(Pixel *))
    };
    for(int i = 0; i < rect->w; i++)
    {
        res.pixels[i] = (Pixel *)malloc((rect->h + 1) * sizeof(Pixel));
    }

    for(int x = rect->x; x < rect->x + rect->w; x++)
    {
        for(int y = rect->y; y < rect->y + rect->h; y++)
        {
            res.pixels[x - rect->x][y - rect->y].r = image->pixels[x][y].r;
            res.pixels[x - rect->x][y - rect->y].g = image->pixels[x][y].g;
            res.pixels[x - rect->x][y - rect->y].b = image->pixels[x][y].b;
        }
    }
    return res;
}

//split (marche seulement si l'image est droite)
Image* split(struct Line *lines, Image *image, Image *cells)
{
    //recherche le point le plus en haut à gauche
    int startX = lines->x1;
    for(int i = 0; i < 4; i++)
    {
        startX = MIN(startX, (lines + i)->x1);
        startX = MIN(startX, (lines + i)->x2);
    }

    int startY = lines->y1;
    for(int i = 0; i < 4; i++)
    {
        startY = MIN(startY, (lines + i)->x1);
        startY = MIN(startY, (lines + i)->x2);
    }

    //recherche un côté horizontal
    struct Line lineX = *lines;
    if(find_angle(lineX) >= 50)
    {
        lineX = *(lines + 1);

        if(find_angle(lineX) >= 50)
        {
            lineX = *(lines + 2);
        }
    }

    //recherche un côté vertical
    struct Line lineY = *lines;

    if(find_angle(lineY) <= 50)
    {
        lineY = *(lines + 1);

        if(find_angle(lineY) <= 50)
        {
            lineY = *(lines + 2);
        }
    }

    int xPlus = (int)LineLength(lineX.x1, lineX.x2, lineX.y1, lineX.y2) / 9;
    int yPlus = (int)LineLength(lineY.x1, lineY.x2, lineY.y1, lineY.y2) / 9;
    int i = 0;
    int j = 0;
    int count = 0;
    SDL_Rect cell;

    for(int x = startX + 10; i < 9; i++, j = 0, x += xPlus)
    {
        for(int y = startY + 10; j < 9; j++, y += yPlus)
        {
            cell.x = x;
            cell.y = y;
            cell.w = xPlus - 20;
            cell.h = yPlus - 20;

            Image newImage = cropImage(image, &cell);

            cells[count++] = imageResize(&newImage, 28, 28);
        }
    }
    return cells;
}