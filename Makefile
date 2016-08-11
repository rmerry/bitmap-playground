CFLAGS=-Wall -g -std=c99

all: build

run: build
	./bmp /home/richard/Pictures/earth.bmp

build: lib/bmp

clean:
	rm -f ./bmp
