CC = gcc
CFLAGS = -std=c99 -w -Wall

all: graphToSat
	./graphToSat graph1 2 && cat instance_sat.in

util.o : util.c
	$(CC) -o util.o -c util.c $(CFLAGS)

graphToSat.o : graphToSat.c
	$(CC) -o graphToSat.o -c graphToSat.c $(CFLAGS)

graphToSat : graphToSat.o util.o
	$(CC) -o graphToSat graphToSat.o util.o $(CFLAGS)
