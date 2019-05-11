CC = g++

main: main.o xDouble.o
	$(CC) -m32 -o oiak main.o operations.o

main.o: main.cpp
	$(CC) -m32 -c main.cpp

xDouble.o: operations.cpp operations.h
	$(CC) -m32 -c operations.cpp
