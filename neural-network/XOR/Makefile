
xor: xor.o
	gcc xor.o -o xor -lm

xor.o: ann_xor.c
	gcc -Wall -Wextra -c -o xor.o ann_xor.c 

clean:
	rm -f xor.o
	rm -f xor.d
	rm -f xor
	rm -rf training-result
