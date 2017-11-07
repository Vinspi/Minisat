#include "calcul_maximum_exact.h"
#include "util.h"





//Fonction remplissant la liste XS avec l'ensemble des sommets de degré non-marqué minimum, et retourne le degré non-marqué minimum, sauf si tout les sommets restants ont le même degré, auquel cas retourne -1.
int sommets_degre_min(Graph *g, int degre[], sous_graphe_max *xs){

	liste x = NULL;
	int min = n_max, imin = -1, allsame = 1;
	for(int i = 0; i < g->n; i++){
		if(degre[i] > -1 && degre[i] <= min){
			imin = i;
			min = degre[i];
		}
	}


	for(int i = 0; i < g->n; i++){
		if(degre[i] == min){
			//printf("Ajout de %d\n",i);
			x = add_to_liste(x, i);
			xs->taille = (xs->taille) + 1;
		}
		else if(degre[i] > -1 && degre[i] != min){
			allsame = 0;
		}

	}

	xs->lx = x;
	if (allsame == 1) return -1;
	return min;

}



sous_graphe_max* calcul_maximum_exact_rec(Graph *g, int degre[], sous_graphe_max *sgm){


	/*Choisir le nouveau sommet i*/
	sous_graphe_max *lns = malloc(sizeof(sous_graphe_max));
	int d = sommets_degre_min(g,degre,lns);


  //S'il n'y a plus de sommets à ajouter, c'est que sgm est un sous-graphe maximal. On renvoie donc sgm, le sous-graphe maximum.
	if(lns->lx == NULL){
		return sgm;
	}

	//S'il n'y a qu'un seul sommet de degré non-marqué minimum, alors il n'y a pas divergence : il devient inutile de faire des copies des structures.
  //On ajoute simplement le sommet au sous-ensemble de sommets, en effectuant les opérations nécessaires, à savoir incrémentation de la taille, et marquage de l'adjacence.
	if (lns->taille == 1){

		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);
		/*Mettre à jour les degrés et l'adjacence*/
		degre[lns->lx->st] = -1;
		for(int j=0;j<g->n;j++){
      if(g->a[sgm->lx->st][j] == 1 && degre[j] > -1){
				degre[j] = -1;
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(degre[z])--;
					}
				}
			}
		}
		return calcul_maximum_exact_rec(g,degre,sgm);

	}

  //S'il y a plusieurs sommets de degré non-marqué minimum, il y a divergence : on doit explorer toutes les possibilités possibles, et donc tester tout les sommets un à un.
  //... Sauf si l'on tombe sur un cas particulier.
	else {

    //Si le degré minimum est égal à 0, on peut prendre tout les sommets ayant le degré minimum. (Voir Rapport Partie V.5. Optimisation)
		if(d == 0){


      //On ajoute simplement chaque sommet de degré non-marqué minimum à la liste en modifiant les degrés.
			sgm->taille += lns->taille;

			while(lns->lx != NULL){
				sgm->lx = add_to_liste(sgm->lx,lns->lx->st);
				degre[lns->lx->st] = -1;
				for(int j=0;j<g->n;j++){
					if(g->a[lns->lx->st][j] == 1 && degre[j] > -1){
						degre[j] = -1;
						for(int z = 0; z < g->n; z++){
							if(g->a[j][z] == 1){
								(degre[z])--;
							}
						}
					}
				}
				lns->lx=lns->lx->suiv;

			}
			return calcul_maximum_exact_rec(g,degre,sgm);
		}

    //Si tout les sommets ont le même degré (càd le degré non-marqué minimum) à un moment donné, le choix du sommet à cet instant n'aura aucune influence sur la taille du sous-graphe final. On choisira donc le premier par défaut. (Voir Rapport Partie V.5. Optimisation)
		else if(d == -1){
			sgm->taille++;
			sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

			/*Mettre à jour les degrés et l'adjacence*/
			degre[lns->lx->st] = -1;

			for(int j=0;j<g->n;j++){
				if(g->a[lns->lx->st][j] == 1 && degre[j] > -1){
					degre[j] = -1;

					for(int z = 0; z < g->n; z++){
						if(g->a[j][z] == 1){
							(degre[z])--;
						}
					}
				}
			}

			return calcul_maximum_exact_rec(g,degre,sgm);

		}

    //Sinon, cas général : on essaye toutes les possiblités, en duplicant nos structures afin de faire des calculs en parrallèle.
		else {

      //On crée ensuite un tampon et un max. On comparera par la suite la taille du tampon, contenant le sous-graphe maximum "potentiel" courant, et celle du maximum courant.
			sous_graphe_max *sgmtmp,*dmax;
			int max = sgm->taille;

			//On parcourt tout les sommets partageant le degré minimum.
			while(lns->lx != NULL){

				//Copie du tableau de degré dans un nouveau tableau : besoin d'une instance par possiblité pour la divergence (sinon structures partagées).
        int copie_degre[n_max];
        for(int i = 0; i < g->n;i++){
            copie_degre[i] = degre[i];

        }


				//Copie du sous-ensemble de sommets courant : ici aussi, on crée une instance par possibilité pour la divergence (sinon structures partagées).
				sous_graphe_max* tmp = malloc(sizeof(sous_graphe_max));
				liste ltmp = sgm->lx;
				tmp->lx = NULL;
				while(ltmp != NULL){
					tmp->lx = add_to_liste(tmp->lx,ltmp->st);
					ltmp = ltmp -> suiv;
				}
				tmp->taille = sgm->taille;


        //On ajoute ensuite le sommet "potentiel" courant dans le sous-ensemble des sommets de degré non-marqué minimum, en effectuant les opérations habituelles.
				tmp->taille++;
				tmp->lx = add_to_liste(tmp->lx,lns->lx->st);
				copie_degre[lns->lx->st] = -1;

				for(int j=0;j<g->n;j++){
					if(g->a[tmp->lx->st][j] == 1 && copie_degre[j] > -1){
							copie_degre[j] = -1;
							for(int z = 0; z < g->n; z++){
								if(g->a[j][z] == 1){
									(copie_degre[z])--;
								}
							}
					}
				}

				//Choix ou omission du sous-graphe maximum "potentiel".
				sgmtmp = calcul_maximum_exact_rec(g,copie_degre,tmp);
				if (sgmtmp->taille > max){
          dmax = sgmtmp;
          max = dmax->taille;
        }
				lns->lx = lns->lx->suiv;

			}
			//Enfin, on retourne le plus grand sous-graphe maximum "potentiel" généré lors de la divergence.
			return dmax;
		}

	}
}


//Fonction principale.
sous_graphe_max* calcul_maximum_exact(Graph *g){


    int copie_degre[n_max];
    for(int i = 0; i < g->n;i++){
        copie_degre[i] = g->degre[i];
    }


  	sous_graphe_max *sgm = malloc(sizeof(sous_graphe_max));
		sgm->lx = NULL;
		sgm->taille = 0;

  	return calcul_maximum_exact_rec(g,copie_degre,sgm);
}
