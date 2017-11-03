#include "graphToSat.h"
#include "util.h"

#define FILE_OUT "instance_sat.in"

int main(int argc, char const *argv[]) {

  if(argc != 3){
    /* print usage */
    printf("graphToSat [filename] k-coloration\n");
    exit(0);
  }


  Graph* g = malloc(sizeof(Graph));

  loadSource(argv[1],g);
  //printGraph("graph_mat",g);
  stable2Sat(g,atoi(argv[2]));


  return 0;
}

/*
  on défini la formule suivante :
  num_var_prop = k*num_sommet + num_couleur + 1

*/
int var_prop(int k, int sommet, int couleur){
  return k*sommet+couleur+1;
}

void stable2Sat(Graph *g, int k){
  FILE *out = NULL;

  out = fopen(FILE_OUT,"w+");

  if(out == NULL){
    printf("impossible d'ecrire dans %s\n", FILE_OUT);
    exit(0);
  }

  int nb_var_prop = g->n;
  int nb_clause = g->n;

  /* on écrit la préface de l'instance */
  fprintf(out, "p cnf %d %d \n", nb_var_prop, nb_clause);


  /* pour chaque sommet que l'on prends on ne dois pas prendre les adjaçent */

  fprintf(out, "c chaque sommet si il est choisi on ne dois pas prendre ses adjaçent\n");
  for(int i=0;i<g->n;i++){
    fprintf(out, "c sommet %d choisi :\n",i);
    for(int j=0;j<g->n;j++){
      if(g->a[i][j])
        fprintf(out, "-%d -%d 0\n", i+1, j+1);
    }
  }

}


void graph2Sat(Graph *g, int k){

  FILE *out = NULL;
  out = fopen(FILE_OUT,"w+");

  if(out == NULL){
    printf("impossible d'ecrire dans %s\n", FILE_OUT);
    exit(0);
  }

  int nb_var_prop = g->n*k;
  int nb_clause = k;

  /* on écrit la préface de l'instance */
  fprintf(out, "p cnf %d %d \n", nb_var_prop, nb_clause);

  /* premiere partie de l'instance */
  /* pour cette premiere partie le nombre de var est égal à nb_somet * nb_couleur */
  /* le nombre de clauses est donc le nombre de couleur que l'on cherche à attribuer */

  fprintf(out, "c chaque sommet à au moins une couleur\n");
  for(int i=0;i<g->n;i++){
    fprintf(out, "c sommet %d à au moins une couleur\n",i);
    for(int j=0;j<k;j++){
      fprintf(out, "%d ", var_prop(k,i,j));
    }
    fprintf(out, "0\n");
  }

  /* pour chaque sommet */

  for(int i=0;i<g->n;i++){
    fprintf(out, "c pour le sommet %d, aucun de ses voisins ne doit avoir la meme couleur\n", i);
    /* pour chaque couleur possible */
    for(int l=0;l<k;l++){
    /* pour tout voisin de ce sommet */
      for(int j=0;j<g->n;j++){
        if(g->a[i][j]){
          /* j est un voisin */
          printf("%d est voisin de %d\n",i,j);
          fprintf(out, "-%d -%d 0\n", var_prop(k,i,l), var_prop(k,j,l));
          nb_clause++;
        }
      }
    }
  }

  fseek(out,0,SEEK_SET);
  fprintf(out, "p cnf %d %d \n", nb_var_prop, nb_clause);

  fclose(out);
}
