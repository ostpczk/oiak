CC = g++
CXXFLAGS = -m32 -mmmx -pg -Wall

main: main.o operations.o
	$(CC) $(CXXFLAGS) -o oiak main.o operations.o

main.o: main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

operations.o: operations.cpp operations.h
	$(CC) $(CXXFLAGS) -c operations.cpp
