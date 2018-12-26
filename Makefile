CC=gcc
CFLAGS=-Wall -pedantic
OUT=ashell

all:
	$(CC) $(CFLAGS) *.cpp -o $(OUT)
clean:
	rm $(OUT)
