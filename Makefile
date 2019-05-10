CC = g++

main: main.o xDouble.o
	$(CC) -o oiak main.o operations.o

main.o: main.cpp
	$(CC) -c main.cpp

xDouble.o: operations.cpp operations.h
	$(CC) -c operations.cpp
