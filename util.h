#ifndef __UTIL__
#define __UTIL__

#include <stdio.h>
#include <stdlib.h>


#define n_max 1000

typedef int sommet;

typedef struct {
	int a[n_max][n_max];
	int n;
	int degre[n_max];
} Graph;

typedef struct maillon{
	sommet st;
	struct maillon *suiv;
} maillon;

typedef maillon *liste;

int taille_liste(liste l);
void initGraph(Graph *g, int n);
void printGraph(char* filename, Graph *g);
void loadSource(char* filename, Graph *g);
void printf_liste(liste x);
liste add_to_liste(liste x, sommet a);
void free_liste(liste x);

#endif
