CC=gcc
CFLAGS=-W -Wall -std=c99 -pedantic -O3
LDFLAGS=
EXEC= avl avl-plot avl-test
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

avl: main.o graal_avl.o
	@$(CC) -o $@ $^ $(LDFLAGS)

avl-plot: main2.o graal_avl.o
	@$(CC) -o $@ $^ $(LDFLAGS)

avl-test: main3.o graal_avl.o
	@$(CC) -o $@ $^ $(LDFLAGS)

main3.o: graal_avl.h graal_avl.c
main2.o: graal_avl.h graal_avl.c
main.o: graal_avl.h graal_avl.c

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
