CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11 -g

LIBS = -lraylib -lm

SRC = src/main.c src/pipe.c src/bird.c src/bg_gnd.c src/score.c

OBJ = $(SRC:src/%.c=obj/%.o)

TARGET = fb

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean
