#include "satToGraph.h"


int main(int argc, char const *argv[]) {

  if(argc != 2){
    /* print usage */
    printf("staToGraph [filename]\n");
    exit(0);
  }

  Graph *g = malloc(sizeof(Graph));

  initGraph(g,n_max);

  sat2graph(g,"instance_sat.in");

  printGraph("out",g);

  //Résultat :
  liste X = calcul_maximum_exact(g)->lx;
  printf("Le sous-graphe maximum exact généré est :\n");
  printf_liste(X);

  return 0;
}

/* fonction utilitaire servant a determiner si c est un digit */

int isDigit(char c){
  return (c <= '9' && c >= '0');
}

/* fonction servant a parser une ligne au format minisat (se terminant par zero ) */

int parseLine(int *result,char buffer[]){

  char tmp[80];
  int i = 0,j = 0,k=0;


  while (buffer[i] != '0') {
    /* parsing number */
    j=0;
    while(isDigit(buffer[i]) || buffer[i] == '-'){
      tmp[j] = buffer[i];
      j++;
      i++;
    }
    /* converting number */
    tmp[j] = '\n';

    result[k] = atoi(tmp);

    k++;
    i++;
  }
  /* on renvoie combien de nombre on a recupéré */
  return k;
}

/* fonction principale au nom évocateur */

int sat2graph(Graph *g, const char* filename){
  FILE *instance_sat = fopen(filename,"r+");
  char buffer[BUFFER_SIZE];
  int nb_sommet;
  int variables_de_clause[BUFFER_SIZE]; /* simple buffer pour la fonction parseLine */
  int tmp;
  int conversion_sommet_litteral[n_max]; /* tableau de correspondance litteral -> sommet */
  int k = 0;  /* indice pour le tableau de conversion */


  if(!instance_sat){
    printf("impossible d'ouvrir le fichier %s\n", filename);
    exit(0);
  }


  while (fgets(buffer,BUFFER_SIZE,instance_sat) != NULL) {
    if(buffer[0] == 'c'){
      printf("commentaire skipping ...\n");
    }
    else if(buffer[0] == 'p'){

      printf("debut du fichier : \n");
    }
    else {
      
      tmp = parseLine(variables_de_clause,buffer);
      for(int i=0;i<tmp-1;i++){
        for(int j=i+1;j<tmp;j++){
          g->a[k+i][k+j] = 1;
          g->a[k+j][k+i] = 1;
        }
      }
      for(int i=0;i<tmp;i++){
        conversion_sommet_litteral[k] = variables_de_clause[i];
        k++;
      }
      nb_sommet+=tmp;

    }
  }

  for(int i=0;i<nb_sommet;i++){
    for(int j=0;j<nb_sommet;j++){
      if(conversion_sommet_litteral[i] == -1*conversion_sommet_litteral[j]){
        /* alors il y a un arc entre i et j */
        g->a[i][j] = 1;
        g->a[j][i] = 1;
      }
    }
  }
  g->n = nb_sommet;
  printf("nb_sommet = %d\n", nb_sommet);

}
