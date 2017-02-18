CC=gcc

CFLAGS= -Wall -Werror -std=c11

all: search_engine

search_engine: search_engine.c
	$(CC) $(CFLAGS) -o $@ $?

clean:
	rm -f search_engine
	rm -f pg4300.txt