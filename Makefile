CC=gcc
CFLAGS=-g -Wall -c -static -L../Project1/lib -lgraf
CLINK=-lm

EXEC=tsp

CFILES:=$(wildcard src/*.c)
COBJ:=$(addprefix obj/,$(notdir $(CFILES:.c=_c.o)))

all: $(COBJ)
	$(CC) $^ -o bin/$(EXEC) $(CLINK)

obj/%_c.o: src/%.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm obj/*.o;

mrproper: clean
	rm bin/$(EXEC)
