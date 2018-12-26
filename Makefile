CC=gcc
CFLAGS=-Wall -pedantic
OUT=ashell

all:
	$(CC) $(CFLAGS) *.c -o $(OUT)
clean:
	rm $(OUT)
