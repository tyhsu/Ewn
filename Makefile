.PHONY: clean
all: ewn

CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++
ewn: main.o play.o ewnAi.o minimax.o game.o
	g++ main.o play.o ewnAi.o minimax.o game.o -o ewn
main.o: main.cpp play.o play.h
	g++ main.cpp -c -o main.o
play.o: play.cpp play.h ewnAi.o ewnAi.h game.o game.h
	g++ play.cpp -c -o play.o
ewnAi.o: ewnAi.cpp ewnAi.h minimax.o minimax.h minimax1.h minimax2.h minimax3.h minimax4.h
	g++ ewnAi.cpp -c -o ewnAi.o
minimax.o: minimax.cpp minimax.h game.o game.h
	g++ minimax.cpp -c -o minimax.o
game.o: game.cpp game.h
	g++ game.cpp -c -o game.o
clean:
	rm -rf ewn main.o play.o ewnAi.o minimax.o game.o