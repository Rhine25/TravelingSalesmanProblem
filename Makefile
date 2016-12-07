CC=gcc
CFLAGS=-g -Wall -c
CLINK=-lm

EXEC=tsp

CFILES:=$(wildcard src/*.c)
COBJ:=$(addprefix obj/,$(notdir $(CFILES:.c=_c.o)))

all: $(COBJ)
	$(CC) $^ -o bin/$(EXEC) $(CLINK) -Llib -lgraf

obj/%_c.o: src/%.c
	$(CC) $(CFLAGS) $^ lib/libgraf.a -o $@

clean:
	rm obj/*.o;

mrproper: clean
	rm bin/$(EXEC)
