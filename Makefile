interface : interfacegtk.c
	gcc `pkg-config --cflags gtk+-3.0` -Wall -O3    interfacegtk.c  `pkg-config --libs gtk+-3.0` -o interface

clean :
	rm -rf *.o
	rm interface
