CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++11 -O3 -g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
#LDLIBS=$(shell root-config --libs)

EXESRC=../bin/gojo.exe
EXETEST=../bin/test.exe

all: target test

target: main.o evolEngine.o gameEngine.o player.o nn.o
	$(CXX) $(LDFLAGS) -o $(EXESRC) main.o evolEngine.o gameEngine.o player.o nn.o

test: test.o evolEngine.o gameEngine.o player.o nn.o
	$(CXX) $(LDFLAGS) -o $(EXETEST) test.o evolEngine.o gameEngine.o player.o nn.o

test.o: test.cpp test.h common.h player.h gameEngine.h
	$(CXX) $(CPPFLAGS) -c test.cpp

evolEngine.o: evolEngine.cpp gameEngine.h player.h common.h
	$(CXX) $(CPPFLAGS) -c evolEngine.cpp

gameEngine.o: gameEngine.cpp gameEngine.h player.h common.h
	$(CXX) $(CPPFLAGS) -c gameEngine.cpp

player.o: player.cpp player.h gameEngine.h common.h
	$(CXX) $(CPPFLAGS) -c player.cpp

nn.o: nn.cpp nn.h common.h
	$(CXX) $(CPPFLAGS) -c nn.cpp

main.o: main.cpp test.h common.h player.h gameEngine.h
	$(CXX) $(CPPFLAGS) -c main.cpp

clean:
	rm -f *.o 
	rm -f ../bin/*

