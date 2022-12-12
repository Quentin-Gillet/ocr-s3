# Makefile

CC = gcc -Iinclude/

CPPFLAGS = `pkg-config --cflags sdl2 gtk+-3.0`
CFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = 
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image gtk+-3.0`
LIBS = -lpthread -ldl

BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "ann_xor.c")
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
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS)

.PHONY: clean

clean:
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
	rm -rf images/*.bmp
	clear

# END
