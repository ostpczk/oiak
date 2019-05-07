CC = g++

main: main.o xDouble.o
	$(CC) -o oiak main.o xDouble.o

main.o: main.cpp
	$(CC) -c main.cpp

xDouble.o: xDouble.cpp xDouble.h
	$(CC) -c xDouble.cpp
