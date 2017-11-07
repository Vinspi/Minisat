CC = gcc
CFLAGS = -std=c99 -w -Wall -g

all: graphToSat satToGraph


util.o : util.c
	$(CC) -o util.o -c util.c $(CFLAGS)

graphToSat.o : graphToSat.c
	$(CC) -o graphToSat.o -c graphToSat.c $(CFLAGS)

graphToSat : graphToSat.o util.o
	$(CC) -o graphToSat graphToSat.o util.o $(CFLAGS)

satToGraph.o : satToGraph.c
	$(CC) -o satToGraph.o -c satToGraph.c $(CFLAGS)

satToGraph : satToGraph.o util.o calcul_maximum_exact.o
	$(CC) -o satToGraph satToGraph.o util.o calcul_maximum_exact.o $(CFLAGS)


calcul_maximum_exact.o : calcul_maximum_exact.c
	$(CC) -o calcul_maximum_exact.o -c calcul_maximum_exact.c $(CFLAGS)
