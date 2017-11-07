#include "util.h"

typedef struct {
	int taille;
	liste lx;
} sous_graphe_max;

sous_graphe_max* calcul_maximum_exact(Graph *g);
