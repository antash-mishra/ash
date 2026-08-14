CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb

heap: shell.o
	$(CC) $(CFLAGS) -o $@ $^

shell.o: shell.c vector.h
	$(CC) $(CFLAGS) -c shell.c -o shell.o

clean:
	rm -f heap shell.o
