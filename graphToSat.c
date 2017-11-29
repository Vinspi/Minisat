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

void combinaison_lineaire_recursive(int tab[],int k,int n, int max, FILE *f,int *nb_clause){

  int sum=0;
  for(int i=0;i<n;i++){
    sum+=tab[i];
  }

  if(sum == max){

    for(int i=0;i<n;i++){
      if(tab[i]){
        fprintf(f,"%d ", i+1);
      }

    }
    fprintf(f, "0\n");
    *nb_clause = *nb_clause+1;
    return;
  }

  if(k == n){
      return;
  }
  else{

    tab[k] = 0;
    combinaison_lineaire_recursive(tab,k+1,n,max,f,nb_clause);

    tab[k] = 1;
    combinaison_lineaire_recursive(tab,k+1,n,max,f,nb_clause);
  }
}

int combinaison_lineaire(int k, int n, FILE *f,int *nb_clause){
  int tab[n];
  for(int i=0;i<k;i++){
    tab[i] = 0;
  }



  combinaison_lineaire_recursive(tab,0,n,k,f,nb_clause);



}

void stable2Sat(Graph *g, int k){
  FILE *out = NULL;


  out = fopen(FILE_OUT,"w+");

  if(out == NULL){
    printf("impossible d'ecrire dans %s\n", FILE_OUT);
    exit(0);
  }


  /* on écrit la préface de l'instance */
  fprintf(out, "p cnf %d %d     \n", nb_var_prop, nb_clause);



  /* pour chaque sommet que l'on prends on ne dois pas prendre les adjaçent */


  fprintf(out, "c chaque sommet si il est choisi on ne dois pas prendre ses adjaçent\n");

  for(int i=0;i<g->n;i++){
    fprintf(out, "c sommet %d choisi :\n",i);
    for(int j=0;j<g->n;j++){
      if(g->a[i][j]){
        fprintf(out, "-%d -%d 0\n", i+1, j+1);
        nb_clause++;
      }
    }
  }


  //combinaison_lineaire(g->n-k+1,g->n,out,&nb_clause);

  //On "force" maintenant la taille du stable : pour cela, on associe k positions xij uniques à des sommets différents, qui seront les positions dans un stable de taille k. xij = (i)*(k+1)+j. k+1 car k positions plus la position "négative"
  fprintf(out, "c association des positions\n");
  for(int i = 0; i < g->n;i++){

      for(int j = 0; j < k+1; j++){    //Boucle de parcours des k+1 positions attribuable au sommet.
        fprintf(out, "c sommet %d : une seule position %d\n", i+1,j);
        fprintf(out, "-%d %d 0\n", i*(k+1)+j+1+g->n, i+1);    //Si on positionne un sommet dans le stable, c'est qu'il y appartient.
        nb_clause++;
        for(int l = 0; l < k+1; l++){  //Boucle de négations des k autres positions.

          if(j != l){
            fprintf(out, "-%d -%d 0\n", (i*(k+1)+j+1+g->n), (i*(k+1)+l+1+g->n));
            nb_clause++;
          }
        }
        fprintf(out, "c sommet %d position %d : pas d'autres sommets ayant cette position\n",i+1,j);
        for(int a = 0; a < g->n; a++){ //Boucle d'interdictions aux autres sommets de pouvoir utiliser cette position.

            if(i != a){
              fprintf(out, "-%d -%d 0\n", (i*(k+1)+j+1+g->n),(a*(k+1)+j+1+g->n));
              nb_clause++;
            }
        }
      }

  }

  //On peut le faire directement dans la boucle, mais pour le moment on divise bien le problème :
  fprintf(out, "c clauses d'obligations d'attributions des positions\n");
  for(int j = 1; j < k+1; j++){
    for(int i = 0; i < g->n; i++){
        fprintf(out, "%d ", (i*(k+1)+j+1+g->n));
    }
    fprintf(out, "0\n");
    nb_clause++;
  }


  fseek(out,0,SEEK_SET);
  fprintf(out, "p cnf %d %d \n", nb_var_prop, nb_clause);

  fclose(out);


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
