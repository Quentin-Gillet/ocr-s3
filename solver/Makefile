
solver: sudoku.o
	gcc sudoku.o -o solver

sudoku.o: sudoku.c
	gcc -Wall -Wextra   -c -o sudoku.o sudoku.c

clean:
	rm -f sudoku.o
	rm -f sudoku.d
	rm -f solver
	rm *.result

