#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/main.h"
#include "../include/heap.h"
#include "../../Projet1/include/libgraphe.h"
#include "../../Projet1/include/libliste.h"

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
        //printf("sommet %d (%d,%d)\n", i, sommets[i].x, sommets[i].y);
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

    //printGraphe(&graphe, stderr);

    int list[nbSommets];
    for(i=0; i<nbSommets; i++){
        list[i] = i;
    }

    /************************Calcul de la solution exacte********************/
    if(nbSommets < 10){
        //trouver tous les circuits, calculer leur cout, garder le meilleur

        int tab[nbSommets*factorielle(nbSommets)];
        effectue(nbSommets, tab);
        //printTab(tab, factorielle(nbSommets)*nbSommets, nbSommets);

        int couts[factorielle(nbSommets)];
        for(i=0; i<factorielle(nbSommets); i++){
            int j;
            int somme = 0;
            for(j=1; j<nbSommets-1; j++){
                struct list_node* arete = searchNode(&graphe.listesAdjacences[tab[i*nbSommets+j]], tab[i*nbSommets+j+1]);
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
        printf("Un des circuits hamiltoniens optimaux pour ce graphe est : ");
        for(i = 0;i<nbSommets;i++){
            printf("%d ", tab[indice*nbSommets+i]);
        }
    }

    /*********************L'algorithme du plus court chemin*****************************/

    struct heap tas = createHeap();

    printf("Tas vide : ");
    printTab(tas.tab, tas.size*2, 2);

    int parcours[nbSommets];
    int parcoursCourant = 1;

    //on commence par le sommet 0
    parcours[0] = 0;

    int k;
    for(k = 1; k<nbSommets; k++) {
        //on ajoute les sommets au tas avec leur poids en partant de 0
        struct list suivants = graphe.listesAdjacences[parcours[k-1]];
        struct list_node *nodeTmp;
        nodeTmp = suivants.first;
        while (nodeTmp->next != NULL) {
            float poids = nodeTmp->poids;
            struct pair p = createPair(nodeTmp->state, poids);
            pushHeap(&tas, p);
            nodeTmp = nodeTmp->next;
        }
        parcours[k] = tas.tab[0].elem[0];
        printf("Parcours : ");
        printTab(parcours, nbSommets, 1);
    }

    printf("Result : ");
    printTab(tas.tab, tas.size*2, 2);

    //on met à jour les poids pour accéder aux sommets
    //updateWeights(&tas, &graphe, 0);

    //printTab(tas.tab, tas.size*2, 2);

    //on fait le parcours
    while(!isEmptyHeap(&tas)){
        struct pair p = popHeap(&tas);
        parcours[parcoursCourant] = p.elem[0];
        parcoursCourant ++;
    }

    printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme du plus cours chemin est : ");
    printTab(parcours, nbSommets, 1);

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
            //printf("%d,", i+j);
        }
        printf("%d", tab[i+j]);
        //printf("%d", i+j);
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

float max(struct list* self, int size){
    struct list_node* tmp = self->first;
    float max = self->first->poids;
    while(tmp->next != NULL){
        if(tmp->poids > max){
            max = tmp->poids;
        }
        tmp = tmp->next;
    }
    return max;
}