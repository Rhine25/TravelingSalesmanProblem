#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/main.h"
#include "../include/heap.h"
#include "../../Projet1/include/libgraphe.h"
#include "../../Projet1/include/libliste.h"


//TODO calcul du poids du parcours pour les plus proches voisins
//TODO pas besoin que le tas stocke les poids, faire des fonctions pour les récupérer facilement
//TODO les poids en float dans le tas

/*
 *  UTILI§SER ENTASSER pour le plus petit
 */

int main(int argc, char *argv[]){

    /*********************Récupération des arguments*********************/
    if(argc != 3){
        fprintf(stderr,"Usage : N nombre de sommets, g graine\n");
        return 1;
    }
    int nbSommets = atoi(argv[1]);
    struct sommet sommets[nbSommets];

    srandom((unsigned long) atoi(argv[2]));

    /**********************Création du graphe**************************/

    //Création du graphe et du tableau de sommets
    struct graph graphe = createGraphe(0, nbSommets);
    int i;
    for(i=0; i<nbSommets; i++){
        sommets[i].x = random()%100;
        sommets[i].y = random()%100;
        printf("sommet %d (%d,%d)\n", i, sommets[i].x, sommets[i].y);
        addVertex(&graphe);
    }

    //création des aretes du graphe afin de le rendre complet
    for(i=0; i<nbSommets-1; i++){
        int j;
        for(j = i+1; j<nbSommets; j++){
            //créer arete
            float d = sqrt(pow(sommets[i].x - sommets[j].x, 2) + pow(sommets[i].y - sommets[j].y, 2));
            //printf("%f\n", d);
            addEdge(&graphe, i, j, d);
        }
    }

    printGraphe(&graphe, stdout);

    /************************Calcul de la solution exacte********************/
    //TODO fiw this not working for over 6
    if(nbSommets < 7){
        //trouver tous les circuits, calculer leur cout, garder le meilleur

        int tab[nbSommets*factorielle(nbSommets)]; //tableau des parcours possibles
        effectue(nbSommets, tab);

        float couts[factorielle(nbSommets)]; //tableau des couts des parcours
        for(i=0; i<factorielle(nbSommets); i++){ //pour chaque parcours
            int j;
            float somme = 0;
            for(j=0; j<nbSommets-1; j++){ //
                struct list_node* arete = searchNode(&graphe.listesAdjacences[tab[i*nbSommets+j]], tab[i*nbSommets+j+1]); //
                somme += arete->poids;
            }
            couts[i] = somme;
        }
        int min = couts[0];
        int indice = 0;
        for(i=1; i<factorielle(nbSommets); i++){
            if(couts[i]<min){
                min = couts[i];
                indice = i;
            }
        }
        printf("Un des circuits hamiltoniens optimaux pour ce graphe a pour cout %f et est : ", couts[indice]);
        for(i = 0;i<nbSommets;i++){
            printf("%d ", tab[indice*nbSommets+i]);
        }
    }

    /*********************L'algorithme du plus court chemin*****************************/

    struct heap tasPlusProcheVoisin = createHeap();
    int parcours[nbSommets];
    int parcoursCourant = 1;

    //on commence par le sommet 0
    parcours[0] = 0;
    float cout = 0;

    //on ajoute les sommets au tas
    for(i=1; i<nbSommets; i++){
        pushHeap(&tasPlusProcheVoisin, i, poids(&graphe, 0, i));
    }

    while(!isEmptyHeap(&tasPlusProcheVoisin)){
        rearrangeHeap(&tasPlusProcheVoisin);
        struct couple couple = popHeap(&tasPlusProcheVoisin);
        parcours[parcoursCourant] = couple.sommet;
        cout += couple.poids;

        //mise à jour des poids des aretes
        updateWeights(&tasPlusProcheVoisin, &graphe, parcours[parcoursCourant]);
        parcoursCourant ++;
    }

    printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme du plus cours chemin a pour cout %f et est : ", cout);
    printTab(parcours, nbSommets, 1);

    destroyHeap(&tasPlusProcheVoisin);

    /*********************L'algorithme du plus petit détour*****************************/

    if(nbSommets >= 3){
        struct heap tasPlusPetitDetour = createHeap();
        //choisis l'arete la plus courte
        int i;
        float min = graphe.listesAdjacences[0].first->poids;
        int sommet1 = 0;
        int sommet2 = graphe.listesAdjacences[0].first->state;
        for(i=0; i<nbSommets; i++){ //pour chaque sommet du graphe
            struct list_node* walk = graphe.listesAdjacences[i].first;
            while(walk->next != NULL){ //pour chaque voisin
                if(walk->poids < min){
                    min = walk->poids;
                    sommet1 = i;
                    sommet2 = walk->state;
                }
                walk = walk->next;
            }
        }

        //crée la tournée à deux sommets
        struct list tournee = createList();
        addNode(&tournee, sommet1, 0);
        addNode(&tournee, sommet2, 0);

        printf(listToString(&tournee));

        struct arete aretes[nbSommets];//tableau qui va contenir les aretes correspondant au poids de chaque sommet du tas

        //on met les sommets pas encore dans la tournée dans un tas
        for(i=0; i<nbSommets; i++){
            if(i != sommet1 && i != sommet2){ //si le sommet n'est pas encore dans la tournee
                struct list_node* walk = tournee.first;
                float cout_detour_min = poids(&graphe, i, tournee.first->state)
                                        + poids(&graphe, i, tournee.first->next->state)
                                        - poids(&graphe, tournee.first->state, tournee.first->next->state);
                int sommetAMin = tournee.first->state;
                int sommetBMin = tournee.first->next->state;

                while(walk->next != NULL){ //pour chaque arete de la tournee
                    int sommetA = walk->state;
                    int sommetB = walk->next->state;
                    float cout_arete_initiale = poids(&graphe, sommetA, sommetB);
                    float cout_AN = poids(&graphe, i, sommetA);
                    float cout_BN = poids(&graphe, i, sommetB);
                    float cout_detour = cout_AN + cout_BN - cout_arete_initiale;
                    if(cout_detour < cout_detour_min){
                        cout_detour_min = cout_detour;
                        sommetAMin = sommetA;
                        sommetBMin = sommetB;
                    }
                    aretes[i].sommetA = sommetAMin;
                    aretes[i].sommetB = sommetBMin;
                    walk = walk->next;
                }
                pushHeap(&tasPlusPetitDetour, i, cout_detour_min);
            }
        }
    }

    return 0;
}

int factorielle(int n){
    if(n == 1){
        return 1;
    }
    return n * factorielle(n-1);
}

void printTab(int* tab, int taille, int tailleBloc){
    int i;
    printf("[");
    for(i = 0; i<taille; i+=tailleBloc){
        int j;
        printf("[");
        for(j = 0; j<tailleBloc-1; j++){
            printf("%d,", tab[i+j]);
        }
        printf("%d", tab[i+j]);
        printf("],");
    }
    printf("]\n");
}

void sublist(int* liste, int taille, int val, int* sub){
    int c = 0;
    while(liste[c] != val){
        sub[c] = liste[c];
        c++;
    }
    while(c < taille){
        sub[c] = liste[c+1];
        c++;
    }
}

void arrangements(int n, int k, int *L, int *t, int* tab) {
    int i;
    int j;
    int j1;
    int t2[n]; //

    //si on est dans la dernière itération
    if(k==n) {
        //pour chaque numéro de sommet
        for(i=0;i<n;i++) {
            //on cherche la première case du tableau qui n'est pas encore un sommet
            for(j=0; j<n*factorielle(n) && tab[j]!=n; j++){
            }
            //et on y met le sommet courant i
            tab[j] = L[i];
        }
        return;
    }

    //pour chaque numéro de sommet de l'itération courante
    for(i=0;i<n-k;i++) {
        //on met
        L[k] = t[i];
        for(j=0, j1=0;j<n-k;j++) {
            if(j != i) {
                t2[j1] = t[j];
                j1++;
            }
        }
        arrangements(n, k+1, L, t2, tab);
    }

}

void effectue(int n, int* tab) {
    int L[n]; //
    int t[n]; //tableau contenant les numéros de sommet dans l'ordre
    int i;
    //initialisation du tableau avec les numéros de sommets du graphe
    for(i=0;i<n;i++) {
        t[i] = i;
    }
    //initialisation du tableau qui va recevoir les différents arragements
    for(i=0;i<(n*factorielle(n));i++) {
        tab[i] = n;
    }
    arrangements(n, 0, L, t, tab);
}

float poids(struct graph* self, int sommet_source, int sommet_cible){
    struct list_node* tmp = searchNode(&self->listesAdjacences[sommet_source], sommet_cible);
    return tmp->poids;
}



