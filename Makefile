.PHONY: clean
all: ewn

CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++
ewn: main.o play.o ewnAi.o randomAi.o mcts.o uct.o tree_node.o minimax.o game.o file.o
	g++ main.o play.o ewnAi.o randomAi.o mcts.o uct.o tree_node.o minimax.o game.o file.o -o ewn
main.o: main.cpp
	g++ main.cpp -c -o main.o
play.o: play.cpp play.h
	g++ play.cpp -c -o play.o
ewnAi.o: ewnAi.cpp ewnAi.h
	g++ ewnAi.cpp -c -o ewnAi.o
randomAi.o: randomAi.cpp randomAi.h
	g++ randomAi.cpp -c -o randomAi.o
mcts.o: mcts.cpp mcts.h
	g++ mcts.cpp -c -o mcts.o
uct.o: uct.cpp uct.h
	g++ uct.cpp -c -o uct.o
file.o: file.cpp file.h
	g++ file.cpp -c -o file.o
tree_node.o: tree_node.cpp tree_node.h
	g++ tree_node.cpp -c -o tree_node.o
minimax.o: minimax.cpp minimax.h
	g++ minimax.cpp -c -o minimax.o
game.o: game.cpp game.h
	g++ game.cpp -c -o game.o
clean:
	rm -rf ewn *.o 