CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-std=c++11 -g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
#LDLIBS=$(shell root-config --libs)

SRC=evolManager/evolManager.cpp
TEST=unitTests/unitTest.cpp
OBJSRC=$(SRC:.cpp=.o)
OBJTEST=$(TEST:.cpp=.o)
EXESRC=bin/evolManager.exe
EXETEST=bin/unitTest.exe

all: target test

target: $(OBJSRC)
	$(CXX) $(LDFLAGS) -o $(EXESRC) $(OBJSRC)

test: $(OBJTEST)
	$(CXX) $(LDFLAGS) -o $(EXETEST) $(OBJTEST)

$(OBJSRC): $(SRC)
	$(CXX) $(CPPFLAGS) $^ -c -o $@

$(OBJTEST): $(TEST)
	$(CXX) $(CPPFLAGS) $^ -c -o $@

clean:
	rm -f $(OBJSRC)
	rm -f $(OBJTEST)
	rm -f bin/*

