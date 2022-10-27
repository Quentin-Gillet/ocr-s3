
xor: xor.o
	gcc xor.o -o xor -lm

xor.o: cnn_xor.c
	gcc -Wall -Wextra -c -o xor.o cnn_xor.c 

clean:
	rm -f xor.o
	rm -f xor.d
	rm -f xor
	rm -rf training-result
