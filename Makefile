.PHONY: clean
all: ewn

CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++
ewn: main.o play.o game.o
	g++ main.o play.o game.o -o ewn
main.o: main.cpp play.o play.h
	g++ main.cpp -c -o main.o
play.o: play.cpp play.h game.o game.h
	g++ play.cpp -c -o play.o
game.o: game.cpp game.h
	g++ game.cpp -c -o game.o
clean:
	rm -rf main.o play.o game.o