OS := $(shell uname)

ifeq ($(OS), Darwin)
    CC := gcc -g -pedantic -std=c99 - Wall -Wextra

else
    ifeq ($(OS), Linux)
    	CC := gccx
    endif
endif

all: knn

knn: knn.c csvparser.c csvparser.h
	$(CC) knn.c csvparser.c -lm -o knn

csvparser: csvparser.c csvparser.h
	$(CC) -o csvparser csvparser.c

clean:
	rm -rf *.o knn *.dSYM core
