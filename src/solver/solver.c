#include "solver/solver.h"

// SUDOKU SOLVER MADE INLINE
// ARRAY OF 1 DIMENSION

int test_line(int n, int line, char s[])
{
    int pos = line * 9;
    int a = pos + 9;

    while (pos < a) {
        if (s[pos] == n)
            return 0;
        pos++;
    }
    return 1;
}

int test_column(int n, int column, char s[])
{
    int pos = column;
    int a = 81;

    while (pos < a) {
        if (s[pos] == n)
            return 0;
        pos += 9;
    }
    return 1;
}

int test_box(int n, int line, int column, char s[])
{
    line = line - (line % 3);
    column = column - (column % 3);

    int line_a = line + 3;
    int column_a = column + 3;
    int b = column;

    while (line < line_a) {
        while (column < column_a) {
            if (s[line * 9 + column] == n) {
                return 0;
            }
            column++;
        }
        column = b;
        line = line + 1;

    }
    return 1;
}

int is_empty(int pos, char s[])
{
    if (s[pos] == '.')
        return 1;
    return 0;
}

// FILL THE SUDOKU - BACKTRACKING ALGO
int sudoku_solver(int pos, int empty, char s[])
{
    if (pos == 81) {
        return 0;
    }
    if (empty == 0) {
        return sudoku_solver(pos + 1, is_empty(pos + 1, s), s);
    } else {
        char sol[9] = "123456789";
        int n = 0;
        while (n < 9) {
            if (test_line(sol[n], pos / 9, s) == 1
                && test_column(sol[n], pos % 9, s) == 1
                && test_box(sol[n], pos / 9, pos % 9, s) == 1) {
                s[pos] = sol[n];
                if (sudoku_solver(pos + 1, is_empty(pos + 1, s), s) == 0)
                    return 0;
                else
                    s[pos] = '.';
            }
            n++;
        }
        if (n == 9)
            return -1;
    }
    return 0;
}

// RETURN THE SOLVED SUDOKU IN A FILE .result
void return_sudoku(char *argv, char s[])
{
    FILE *file_return;
    file_return = fopen(strcat(argv, ".result"), "w");

    if (file_return == NULL)
        exit(EXIT_FAILURE);

    int i = 0;
    while (s[i] != 0) {
        if (i != 0) {
            if (i % 27 == 0)
                fprintf(file_return, "\n\n");
            else if (i % 9 == 0)
                fprintf(file_return, "\n");
            else if (i % 3 == 0)
                fprintf(file_return, " ");
            fprintf(file_return, "%c", s[i]);
            i++;
        } else {
            fprintf(file_return, "%c", s[i]);
            i++;
        }
    }
    fclose(file_return);
    exit(EXIT_SUCCESS);
}

// for check_error
int test_len(char *argv) {

    FILE *file;
    file = fopen(argv, "r");

    int i = 0;
    char c;

    while (!feof(file)) {
        c = fgetc(file);
        if (c == '\n' || c == ' ')
            continue;
        else
            i++;
    }
    fclose(file);
    return i;
}

// transform the input file into an 1D array
void toTab(char s[], char *argv)
{
    FILE *file;
    file = fopen(argv, "r");
    char c;
    int i = 0;
    while (!feof(file)) {
        c = fgetc(file);

        if (c == '.') {
            s[i] = c;
            i++;
        } else if ((c == ' ') || (c == '\n'))
            continue;
        else {
            s[i] = c;
            i++;
        }
    }
    fclose(file);
}

int check_error(char *argv, char s[])
{
    FILE *fptr;
    fptr = fopen(argv, "r");
    if (fptr == NULL) {
        printf("ERROR OPENING FILE !\n");
        return 1;
    }

    int l = test_len(argv);
    if (l != 82) {
        printf("INCORRECT FILE !\n");
        return 1;

    }
    s[l - 1] = 0;

    fclose(fptr);

    int i = 0;
    while (s[i] != 0) {
        if (s[i] != '.' && (s[i] < '0' || s[i] > '9')) {
            printf("INCORRECT SUDOKU !\n");
            return 1;
        }
        i++;
    }
    return 0;
}

int resolve(char* file)
{
    char s[81];
    toTab(s, file);
    if (check_error(file, s) == 1)
        return 1;
    else {
        int a = sudoku_solver(0, is_empty(0, s), s);
        if (a == -1) {
            printf("INCORRECT SUDOKU !\n");
            return 1;
        } else {
            return_sudoku(file, s);
            return 0;
        }
    }

}

