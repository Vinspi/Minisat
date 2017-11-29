CC = gcc
CFLAGS = -std=c99 -w -Wall -g

all: graphToSat satToGraph calcul_maximum

calcul_maximum.o: calcul_maximum.c
	$(CC) -o calcul_maximum.o -c calcul_maximum.c $(CFLAGS)

calcul_maximum: util.o calcul_maximum.o
	$(CC) -o calcul_maximum calcul_maximum.o util.o

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
