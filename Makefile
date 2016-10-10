.PHONY: clean
all: ewn

CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++
ewn: main.o game.o
	g++ main.o game.o -o ewn
main.o: main.cpp game.o
	g++ main.cpp -c -o main.o
game.o: game.h game.cpp
	g++ game.cpp -c -o game.o
clean:
	rm -rf main.o game.o