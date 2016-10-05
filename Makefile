CC=gcc
CFLAGS=-W -Wall -std=c99 -pedantic -O3
LDFLAGS=
EXEC=avl
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

avl: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: graal_avl.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
