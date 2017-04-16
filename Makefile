.PHONY: clean
all: ewn

CXXFLAGS = -g -Wall -std=c++11 -c
LDLIBS = -lstdc++

PROG = main play ewnAi randomAi mcts uct file tree_node minimax game

ewn: $(foreach n, $(PROG), $(n).o)
	g++ -std=c++11 $^ -o $@
%.o: %.cpp %.h
	g++ $(CXXFLAGS) $< -o $@

clean:
	rm -rf ewn *.o
