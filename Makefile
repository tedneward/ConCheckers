CC=g++ -g -std=c++11

CYLCHECKERS_CPP=\
	Board.cpp

CYLCHECKERS_H=\
	Board.h

all: console test

console: consolemain.cpp $(CYLCHECKERS_CPP) $(CYLCHECKERS_H)
	$(CC) -o console consolemain.cpp $(CYLCHECKERS_CPP)

test: testing.cpp $(CYLCHECKERS_CPP) $(CYLCHECKERS_H)
	$(CC) -o test testing.cpp $(CYLCHECKERS_CPP)
