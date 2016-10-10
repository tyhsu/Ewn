all: ewn

CXXFLAGS+=-g -Wall
LDLIBS+=-lstdc++
main: game.o main.o