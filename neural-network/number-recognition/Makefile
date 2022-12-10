
# Makefile

CC = gcc -Iinclude/

CPPFLAGS = `pkg-config --cflags sdl2`
CFLAGS = -Wall -Wextra -O0 -g -fsanitize=address
LDFLAGS = 
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`
LIBS = -lpthread -ldl

BUILD := build
SOURCE_DIR := src

SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main clear

clear:
	find . -type d -empty -delete

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
	rm *.bmp
	clear

# END