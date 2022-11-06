# Makefile

CC = gcc -Iinclude/

CPPFLAGS = -D_THREAD_SAFE -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
CFLAGS = -Wall -Wextra -O0 -g -fsanitize=address
LDFLAGS = 
LDLIBS = -lm -L/opt/homebrew/Cellar/sdl2_image/2.6.2/lib -L/opt/homebrew/lib -lSDL2_image -lSDL2
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
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS)

.PHONY: clean

clean:
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
	rm -r images/
	clear

# END
