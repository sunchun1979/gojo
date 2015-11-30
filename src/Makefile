CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++11 -g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
#LDLIBS=$(shell root-config --libs)

EXESRC=../bin/evolManager.exe

all: target 

target: evolManager.o gameEngine.o player.o
	$(CXX) $(LDFLAGS) -o $(EXESRC) evolManager.o gameEngine.o player.o

evolManager.o: evolManager.cpp gameEngine.h player.h
	$(CXX) $(CPPFLAGS) -c evolManager.cpp

gameEngine.o: gameEngine.cpp gameEngine.h player.h
	$(CXX) $(CPPFLAGS) -c gameEngine.cpp

player.o: player.cpp player.h gameEngine.h
	$(CXX) $(CPPFLAGS) -c player.cpp

clean:
	rm -f *.o 
	rm -f ../bin/*

