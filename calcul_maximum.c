#include "util.h"


liste calcul_stable_rec(Graph *g,int mask[], int k, int taille){
  if(k < g->n){
    mask[k] = 0;
    liste l1 = calcul_stable_rec(g,mask,k+1,taille);
    if(l1 != NULL)
      return l1;
    mask[k] = 1;
    liste l2 = calcul_stable_rec(g,mask,k+1,taille);
    return l2;
  }
  else{
    liste l = NULL;
    for (int i = 0; i < g->n; i++) {
      if(mask[i] == 1){
        l = add_to_liste(l,i);
      }
    }
    if(verification_graphe_desert(g,l) && taille <= taille_liste(l)){
      return l;
    }
    return NULL;
  }
}

/* ici on ne calcule pas un stable maximum mais on cherche seulement un stable de taille taille */
liste calcul_stable(Graph *g,int taille){
  int mask[g->n];

  liste l = calcul_stable_rec(g,mask,0,taille);
  return l;
}

liste calcul_maximum_rec(Graph *g,int mask[], int k){

  if(k < g->n){
    mask[k] = 0;
    liste l1 = calcul_maximum_rec(g,mask,k+1);

    mask[k] = 1;
    liste l2 = calcul_maximum_rec(g,mask,k+1);
    if(taille_liste(l1) > taille_liste(l2)){
      return l1;
    }
    return l2;
  }
  else{
    liste l = NULL;
    for (int i = 0; i < g->n; i++) {
      if(mask[i] == 1){
        l = add_to_liste(l,i);
      }
    }
    if(verification_graphe_desert(g,l)){
      return l;
    }
    return NULL;
  }
}

/* ici on calcule de façon exacte un stable maximum */
liste calcul_maximum(Graph *g){

  int mask[g->n];

  liste l = calcul_maximum_rec(g,mask,0);
  return l;

}


/* simple fonction de verification renvoie 1 si x est un stable de g (ceci pourrait faire office de certificat polynomial) */
int verification_graphe_desert(Graph *g, liste x){

	liste tmp_parcour = x, tmp_parcour2;
	int resultat = 1;

	while(tmp_parcour != NULL && resultat == 1){
		tmp_parcour2 = tmp_parcour -> suiv;
		while (tmp_parcour2 != NULL && resultat == 1) {
			if(g->a[tmp_parcour->st][tmp_parcour2->st])
				resultat = 0;
			tmp_parcour2 = tmp_parcour2 -> suiv;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	return resultat;
}
