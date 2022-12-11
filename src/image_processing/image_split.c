#include "image_processing/image_split.h"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

// réduit le nombre de ligne
// Compte le nombre de ligne que l'on va avoir à la fin de la réduction
// du nombre de ligne

double LineLength(int x1, int x2, int y1, int y2);
//-----------------AutoRotation-----------//

int find_angle(struct Line line)
{
    double len1 = LineLength(line.x1, line.x2, line.y1, line.y1);
    double len2 = LineLength(line.x1, line.x2, line.y1, line.y2);
    return radToDeg(acos(len1 / len2));
}

Image auto_rotation(Image* image, struct Line *lines)
{
    int theta1 = find_angle(*lines);
    lines++;
    int theta2 = find_angle(*lines);
    lines++;
    int theta3 = find_angle(*lines);
    lines++;
    int theta4 = find_angle(*lines);

    if(theta1 > 45)
        theta1 = 90 - theta1;
    if(theta2 > 45)
        theta2 = 90 - theta2;
    if(theta3 > 45)
        theta3 = 90 - theta3;
    if(theta4 > 45)
        theta4 = 90 - theta4;

    int angle = (-1) * (theta1 + theta2 + theta3 + theta4) / 4;
    return imageRotate(image, angle);
}

//-----------------AutoRotation-----------//

int approx(int a, int b, int Threshold)
{
    return (abs(a-b) <= Threshold) ? 1 : 0;
}

// Get the intersection between two lines
// x = -1,y = -1 --> means there is no intersection between the 2 lines
struct Intersection line_intersection(struct Line line1, struct Line line2, int width, int height)
{
    double fact1 = (double)(line1.x2 - line1.x1);
    if(!fact1)
        fact1++;
    double fact2 = (double)(line2.x2 - line2.x1);
    if(!fact2)
        fact2++;
    struct Intersection intersection;
    double a1 = ((double)line1.y2 - (double)line1.y1) / fact1;
    double a2 = ((double)line2.y2 - (double)line2.y1) / fact2;
    double b1 = (double)line1.y1 - a1 * line1.x1;
    double b2 = (double)line2.y1 - a2 * line2.x1;

    if((int)a1 - (int)a2 != 0)
    {
        int x = (b1 - b2) / (a2 - a1);
        int y = a1 * (b2 - b1) / (a1 - a2) + b1;
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            intersection.x = x;
            intersection.y = y;
            return intersection;
        }
    }
    intersection.x = -1;
    intersection.y = -1;
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
double SquareArea(struct Intersection inter1, struct Intersection inter2, struct Intersection inter3, struct Intersection inter4)
{
     return LineLength(inter1.x, inter2.x, inter1.y, inter2.y) +
            LineLength(inter2.x, inter3.x, inter2.y, inter3.y) +
            LineLength(inter3.x, inter4.x, inter3.y, inter4.y) +
            LineLength(inter4.x, inter1.x, inter4.y, inter1.y);
}

//If the biggerSquare is a real square
int IsSquare(struct Square square)
{
    int len1 = (int)LineLength(square.xa, square.ya, square.xb, square.yb);
    int len2 = (int)LineLength(square.xb, square.yb, square.xc, square.yc);
    int len3 = (int)LineLength(square.xc, square.yc, square.xd, square.yd);
    int len4 = (int)LineLength(square.xd, square.yd, square.xa, square.ya);
    int max = len1;
    if (len2 > max)
        max = len2;
    if (len3 > max)
        max = len3;
    if (len4 > max)
        max = len4;

    int min = len1;
    if (len2 < min)
        min = len2;
    if (len3 < min)
        min = len3;
    if (len4 < min)
        min = len4;

    unsigned int val = max - min;

    if (val > 100)
        return 0;

    return 1;
}

//if two lines have a no commun point
int compareLines(struct Line line1, struct Line line2)
{
    if(line1.x1 == line2.x1 && line1.y1 == line2.y1)
        return 0;
    if(line1.x2 == line2.x2 && line1.y2 == line2.y2)
        return 0;
    if(line1.x2 == line2.x1 && line1.y2 == line2.y1)
        return 0;
    if(line1.x1 == line2.x2 && line1.y1 == line2.y2)
        return 0;
    return 1;
}

//Initialisation new square
int square_init(struct Intersection inter1, struct Intersection inter2,
                          struct Intersection inter3, struct Intersection inter4, struct Square *square)
{
    struct Line line1 = {
            inter1.x,
            inter1.y,
            inter2.x,
            inter2.y,
            find_angle(line1)
    };

    if (line1.theta > 30)
    {
        line1.x1 = inter2.x;
        line1.x2 = inter3.x;
        line1.y1 = inter2.y;
        line1.y2 = inter3.y;
        line1.theta = find_angle(line1);
        if(line1.theta > 30)
        {
            line1.x1 = inter3.x;
            line1.x2 = inter4.x;
            line1.y1 = inter3.y;
            line1.y2 = inter4.y;
            line1.theta = find_angle(line1);
        }
    }

    struct Line line2 = {
            inter1.x,
            inter1.y,
            inter2.x,
            inter2.y,
            find_angle(line2)
    };

    if(line2.theta > 45 || !compareLines(line1, line2))
    {
        line2.x1 = inter2.x;
        line2.x2 = inter3.x;
        line2.y1 = inter2.y;
        line2.y2 = inter3.y;
        line2.theta = find_angle(line2);
        if(line2.theta > 45 || !compareLines(line1, line2))
        {
            line2.x1 = inter3.x;
            line2.x2 = inter4.x;
            line2.y1 = inter3.y;
            line2.y2 = inter4.y;
            line2.theta = find_angle(line2);
        }
        if(line2.theta > 45 || !compareLines(line1, line2))
        {
            line2.x1 = inter4.x;
            line2.x2 = inter1.x;
            line2.y1 = inter4.y;
            line2.y2 = inter1.y;
            line2.theta = find_angle(line2);
        }
    }

    if(line2.theta > 45 || !compareLines(line1, line2))
        return 0;

    if(line1.x1 < line1.x2)
    {
        square->xa = line1.x1;
        square->ya = line1.y1;
        square->xb = line1.x2;
        square->yb = line1.y2;
    }
    else
    {
        square->xa = line1.x2;
        square->ya = line1.y2;
        square->xb = line1.x1;
        square->yb = line1.y1;
    }

    if(line2.x1 > line2.x2)
    {
        square->xc = line2.x1;
        square->yc = line2.y1;
        square->xd = line2.x2;
        square->yd = line2.y2;
    }
    else
    {
        square->xc = line2.x2;
        square->yc = line2.y2;
        square->xd = line2.x1;
        square->yd = line2.y1;
    }
    square->perimeter = SquareArea(inter1, inter2, inter3, inter4);
    return 1;
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

//Get all squares of the grid
void get_all_squares(struct Line *lines, int NBLine, struct list *squares, int width, int height)
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
            inter1 = line_intersection(lines[h], lines[i], width, height);
            if(inter1.x != -1 && inter1.y != -1)
            {
                for(int j = 2; j < NBLine; j++)
                {
                    if (h == j && i == j)
                        continue;
                    inter2 = line_intersection(lines[i], lines[j], width, height);
                    if(inter1.x == inter2.x && inter1.y == inter2.y)
                        continue;
                    if(inter2.x != -1 && inter2.y != -1)
                    {
                        for(int k = 3; k < NBLine; k++)
                        {
                            if (h == k && i == k && j == k)
                                continue;
                            inter3 = line_intersection(lines[j], lines[k], width, height);
                            if(inter1.x == inter3.x && inter1.y == inter3.y)
                                continue;
                            if(inter2.x == inter3.x && inter2.y == inter3.y)
                                continue;
                            if(inter3.x != -1 && inter3.y != -1)
                            {
                                if(k == h)
                                    continue;
                                inter4 = line_intersection(lines[k], lines[h], width, height);
                                if(inter1.x == inter4.x && inter1.y == inter4.y)
                                    continue;
                                if(inter2.x == inter4.x && inter2.y == inter4.y)
                                    continue;
                                if(inter3.x == inter4.x && inter3.y == inter4.y)
                                    continue;
                                if(inter4.x != -1 && inter4.y != -1)
                                {
                                    if(!square_init(inter1, inter2, inter3, inter4, &square))
                                        continue;
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

struct Line* print_squares(struct Line* lines, int NBLine, int width, int height)
{
    struct list *squares = malloc(sizeof(struct list));
    list_init(squares);
    get_all_squares(lines, NBLine, squares, width, height);

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
struct Line* get_Bigger_Squares(struct Line* lines, int NBLine, int width, int height)
{
    struct list *squares = malloc(sizeof(struct list));
    list_init(squares);
    get_all_squares(lines, NBLine, squares, width, height);
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