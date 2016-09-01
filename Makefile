CFLAGS=-Wall -g -std=c99

all: build

build: clean
	gcc -c lib/bmp.c -o bin/bmp.o
	gcc -c lib/bytes.c -o bin/bytes.o
	gcc -c lib/buffer.c -o bin/buffer.o
	gcc bin/bmp.o bin/bytes.o bin/buffer.o -o bin/bmp

clean:
	rm -f ./bin/*

