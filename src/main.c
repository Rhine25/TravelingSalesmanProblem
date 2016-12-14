#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/main.h"
#include "../include/heap.h"
#include "../../Projet1/include/libgraphe.h"
#include "../../Projet1/include/libliste.h"

//TODO faire un joli affichage uniformisé pour les parcours

int main(int argc, char *argv[]){

    /*********************Récupération des arguments*********************/
    if(argc != 3){
        fprintf(stderr,"Usage : N nombre de sommets, g graine\n");
        return 1;
    }
    int nbSommets = atoi(argv[1]);
    srandom((unsigned long) atoi(argv[2]));

    /**********************Création du graphe**************************/

    //Création du graphe et du tableau de sommets
    struct graph graphe = createGraphe(0, nbSommets);
    struct sommet sommets[nbSommets];
    int i;
    printf("***********************Les sommets***********************\n");
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

    //printGraphe(&graphe, stdout);

    /*Calcul de la solution exacte*/
    //TODO fiw this not working for over 6
    if(nbSommets < 7){ //< 10
        solution_exacte(&graphe);
    }

    /*L'algorithme du plus proche voisin*/

    solution_plus_proche_voisin(&graphe);

    /*L'algorithme du plus petit détour*/

    solution_plus_petit_detour(&graphe);

    /*L'algorithme de l'ARPM*/

    solution_ARPM(&graphe);

    /*L'algorithme de Christofides*/

    solution_christofides(&graphe);

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

void solution_exacte(struct graph* graphe){

    int nbSommets = graphe->nbMaxSommets;
    int i;

    //trouver tous les circuits, calculer leur cout, garder le meilleur

    int tab[nbSommets*factorielle(nbSommets)]; //tableau des parcours possibles
    effectue(nbSommets, tab);

    float couts[factorielle(nbSommets)]; //tableau des couts des parcours
    for(i=0; i<factorielle(nbSommets); i++){ //pour chaque parcours
        int j;
        float somme = 0;
        for(j=0; j<nbSommets-1; j++){ //
            struct list_node* arete = searchNode(&graphe->listesAdjacences[tab[i*nbSommets+j]], tab[i*nbSommets+j+1]); //
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

void solution_plus_proche_voisin(struct graph* graphe){

    int nbSommets = graphe->nbMaxSommets;
    int i;

    struct heap tasPlusProcheVoisin = createHeap();
    int parcours[nbSommets];
    int parcoursCourant = 1;

    //on commence par le sommet 0
    parcours[0] = 0;
    float coutProcheVoisin = 0;

    //on ajoute les sommets au tas
    for(i=1; i<nbSommets; i++){
        pushHeap(&tasPlusProcheVoisin, i, poids(graphe, 0, i));
    }

    while(!isEmptyHeap(&tasPlusProcheVoisin)){
        rearrangeHeap(&tasPlusProcheVoisin);
        struct couple couple = popHeap(&tasPlusProcheVoisin);
        parcours[parcoursCourant] = couple.sommet;
        coutProcheVoisin += couple.poids;

        //mise à jour des poids des aretes
        updateWeights(&tasPlusProcheVoisin, graphe, parcours[parcoursCourant]);
        parcoursCourant ++;
    }

    printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme du plus proche voisin a pour cout %f et est : ", coutProcheVoisin);
    printTab(parcours, nbSommets, 1);

    destroyHeap(&tasPlusProcheVoisin);
}

void solution_plus_petit_detour(struct graph* graphe){

    int nbSommets = graphe->nbMaxSommets;

    if(nbSommets >= 2){
        struct heap tasPlusPetitDetour = createHeap();
        //choisis l'arete la plus grande (on sait qu'elle ne sera pas dans le parcours final)
        int i;
        float max = graphe->listesAdjacences[0].first->poids;
        int sommet1 = 0;
        int sommet2 = graphe->listesAdjacences[0].first->state;
        for(i=0; i<nbSommets; i++){ //pour chaque sommet du graphe
            struct list_node* walk = graphe->listesAdjacences[i].first;
            while(walk->next != NULL){ //pour chaque voisin
                if(walk->poids > max){
                    max = walk->poids;
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
        float coutPetitDetour = poids(graphe, sommet1, sommet2);
        //printf(listToString(&tournee));

        struct arete aretes[nbSommets];//tableau qui va contenir les aretes correspondant au poids de chaque sommet du tas

        //on met les sommets pas encore dans la tournée dans un tas
        for(i=0; i<nbSommets; i++){
            if(i != sommet1 && i != sommet2){ //si le sommet n'est pas encore dans la tournee

                /*Le premier est temporairement le minimum*/
                struct list_node* walk = tournee.first;
                float cout_detour_min = poids(graphe, i, tournee.first->state)
                                        + poids(graphe, i, tournee.first->next->state)
                                        - poids(graphe, tournee.first->state, tournee.first->next->state);
                int sommetAMin = tournee.first->state;
                int sommetBMin = tournee.first->next->state;

                //printf("DEFAULT : Going from %d to %d by %d costs %f by default\n", sommetAMin, sommetBMin, i, cout_detour_min);

                while(walk->next != NULL){ //pour chaque arete de la tournee
                    /*Calcul le cout du détour et cherche par quelle arete il est le plus petit*/
                    int sommetA = walk->state;
                    int sommetB = walk->next->state;
                    float cout_arete_initiale = poids(graphe, sommetA, sommetB);
                    float cout_AN = poids(graphe, i, sommetA);
                    float cout_BN = poids(graphe, i, sommetB);
                    float cout_detour = cout_AN + cout_BN - cout_arete_initiale;
                    if(cout_detour < cout_detour_min){
                        cout_detour_min = cout_detour;
                        sommetAMin = sommetA;
                        sommetBMin = sommetB;
                        //printf("TAKEN : Going from %d to %d by %d costs %f\n", sommetAMin, sommetBMin, i, cout_detour_min);
                    }
                    else{
                        //printf("REJECTED : Going from %d to %d by %d would cost %f\n", sommetA, sommetB, i, cout_detour);
                    }
                    aretes[i].sommetA = sommetAMin;
                    aretes[i].sommetB = sommetBMin;
                    walk = walk->next;
                }
                pushHeap(&tasPlusPetitDetour, i, cout_detour_min);
            }
        }
        rearrangeHeap(&tasPlusPetitDetour);

        //printf(listToString(&tournee));

        //printTab(tasPlusPetitDetour.sommets, tasPlusPetitDetour.size, 1);

        while(!isEmptyHeap(&tasPlusPetitDetour)){
            //pop
            //TODO ajouter dans lib liste addNodeAfter
            struct list_node* node = malloc(sizeof(struct list_node));
            struct couple c = popHeap(&tasPlusPetitDetour);
            node->state = c.sommet;
            node->poids = 0;
            struct list_node* walk = tournee.first;
            while(walk->next->state != aretes[node->state].sommetB){
                walk = walk->next;
            }
            node->next = walk->next;
            walk->next = node;
            coutPetitDetour += c.poids;

            //printf(listToString(&tournee));

            //mettre à jour les poids
            for(i=0; i<tasPlusPetitDetour.size; i++){ //pour tous les sommets du tas
                walk = tournee.first;
                float cout_detour_min = poids(graphe, tasPlusPetitDetour.sommets[i], tournee.first->state)
                                        + poids(graphe, tasPlusPetitDetour.sommets[i], tournee.first->next->state)
                                        - poids(graphe, tournee.first->state, tournee.first->next->state);
                int sommetAMin = tournee.first->state;
                int sommetBMin = tournee.first->next->state;

                //printf("DEFAULT : Going from %d to %d by %d costs %f by default\n", sommetAMin, sommetBMin, tasPlusPetitDetour.sommets[i], cout_detour_min);

                while(walk->next != NULL){ //pour chaque arete de la tournee
                    int sommetA = walk->state;
                    int sommetB = walk->next->state;
                    float cout_arete_initiale = poids(graphe, sommetA, sommetB);
                    float cout_AN = poids(graphe, tasPlusPetitDetour.sommets[i], sommetA);
                    float cout_BN = poids(graphe, tasPlusPetitDetour.sommets[i], sommetB);
                    float cout_detour = cout_AN + cout_BN - cout_arete_initiale;
                    if(cout_detour < cout_detour_min){
                        cout_detour_min = cout_detour;
                        sommetAMin = sommetA;
                        sommetBMin = sommetB;
                        //printf("TAKEN : Going from %d to %d by %d costs %f\n", sommetAMin, sommetBMin, tasPlusPetitDetour.sommets[i], cout_detour_min);
                    }
                    else{
                        //printf("REJECTED : Going from %d to %d by %d would cost %f\n", sommetA, sommetB, tasPlusPetitDetour.sommets[i], cout_detour);
                    }
                    aretes[tasPlusPetitDetour.sommets[i]].sommetA = sommetAMin;
                    aretes[tasPlusPetitDetour.sommets[i]].sommetB = sommetBMin;
                    walk = walk->next;
                }
                //pushHeap(&tasPlusPetitDetour, i, cout_detour_min);
                tasPlusPetitDetour.poids[tasPlusPetitDetour.sommets[i]] = cout_detour_min;
            }
            rearrangeHeap(&tasPlusPetitDetour);
        }

        printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme du plus petit détour a pour cout %f et est : ", coutPetitDetour);
        //TODO revoir le cout donné ici qui ne parait pas correct (avec 6 5)
        printf(listToString(&tournee));

        destroyList(&tournee);
        destroyHeap(&tasPlusPetitDetour);
    }
}

void solution_ARPM(struct graph* graphe){

    int nbSommets = graphe->nbMaxSommets;
    int i;
    int j;
    int aretes[nbSommets]; //sommet source indice, sommet cible en aretes[sommetSource]
    struct heap tas = createHeap();
    struct graph arpm = createGraphe(0, nbSommets);
    addVertex(&arpm);

    //choisir un sommet
    int sommetsArbre[nbSommets];
    sommetsArbre[0] = 0;
    int nbSommetsArbre = 1;

    //faire un tas avec les autres, leur poids est le poids entre chacun d'eux et le sommet initial 0
    //en même temps ajoute les sommets à l'ARPM
    for(i=1; i<nbSommets; i++){
        addVertex(&arpm);
        pushHeap(&tas, i, poids(graphe, 0, i));
    }

    //faire l'ARPM
    while(!isEmptyHeap(&tas)) {
        for (i = 0; i < tas.size; i++) {
            int sommetTas = tas.sommets[i];
            //printf("Sommet tas : %d\n", sommetTas);
            //faire le minimum temporaire
            int sommetArbreMin = sommetsArbre[0];
            float min = poids(graphe, sommetTas, sommetArbreMin);
            //printf("DEFAULT sommet arbre : %d ; poids = %f\n", sommetArbreMin, min);
            aretes[sommetTas] = sommetArbreMin;
            //faire la boucle sur les sommets de l'arbre
            for (j = 0; j < nbSommetsArbre; j++) {
                float poids_arete = poids(graphe, tas.sommets[i], sommetsArbre[j]);
                if (poids_arete < min) {
                    min = poids_arete;
                    sommetArbreMin = sommetsArbre[j];
                    aretes[sommetTas] = sommetArbreMin;
                    //printf("TAKEN sommet arbre : %d ; poids = %f\n", sommetArbreMin, min);
                }
                else{
                    //printf("REJECTED sommet arbre : %d ; poids = %f\n", sommetsArbre[j], poids_arete);
                }
            }
            tas.poids[sommetTas] = min;
        }
        rearrangeHeap(&tas);
        struct couple c = popHeap(&tas);
        struct arete arete_sure;
        arete_sure.sommetA = c.sommet;
        arete_sure.sommetB = aretes[c.sommet];
        addEdge(&arpm, arete_sure.sommetA, arete_sure.sommetB, c.poids);
        sommetsArbre[nbSommetsArbre] = c.sommet;
        nbSommetsArbre ++;
        //printf("added %d -> %d : %f", arete_sure.sommetA, arete_sure.sommetB, c.poids);
    }

    //printGraphe(&arpm, stdout);

    //faire le parcours en profondeur de l'ARPM
    int parcoursDfs[nbSommets];
    dfsb(&arpm, 0, parcoursDfs);
    float cout = 0;
    for(i=1; i<nbSommets; i++){
        cout += poids(graphe, parcoursDfs[i-1], parcoursDfs[i]);
    }
    printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme de l'ARPM a pour cout %f et est : ", cout);
    printTab(parcoursDfs, nbSommets, 1);
}

void solution_christofides(struct graph* graphe){
    int nbSommets = graphe->nbMaxSommets;
    int i;
    int j;
    int aretes[nbSommets]; //sommet source indice, sommet cible en aretes[sommetSource]
    struct heap tas = createHeap();
    struct graph arpm = createGraphe(0, nbSommets);
    addVertex(&arpm);

    //choisir un sommet
    int sommetsArbre[nbSommets];
    sommetsArbre[0] = 0;
    int nbSommetsArbre = 1;

    //faire un tas avec les autres, leur poids est le poids entre chacun d'eux et le sommet initial 0
    //en même temps ajoute les sommets à l'ARPM
    for(i=1; i<nbSommets; i++){
        addVertex(&arpm);
        pushHeap(&tas, i, poids(graphe, 0, i));
    }

    //faire l'ARPM
    while(!isEmptyHeap(&tas)) {
        for (i = 0; i < tas.size; i++) {
            int sommetTas = tas.sommets[i];
            //printf("Sommet tas : %d\n", sommetTas);
            //faire le minimum temporaire
            int sommetArbreMin = sommetsArbre[0];
            float min = poids(graphe, sommetTas, sommetArbreMin);
            //printf("DEFAULT sommet arbre : %d ; poids = %f\n", sommetArbreMin, min);
            aretes[sommetTas] = sommetArbreMin;
            //faire la boucle sur les sommets de l'arbre
            for (j = 0; j < nbSommetsArbre; j++) {
                float poids_arete = poids(graphe, tas.sommets[i], sommetsArbre[j]);
                if (poids_arete < min) {
                    min = poids_arete;
                    sommetArbreMin = sommetsArbre[j];
                    aretes[sommetTas] = sommetArbreMin;
                    //printf("TAKEN sommet arbre : %d ; poids = %f\n", sommetArbreMin, min);
                }
                else{
                    //printf("REJECTED sommet arbre : %d ; poids = %f\n", sommetsArbre[j], poids_arete);
                }
            }
            tas.poids[sommetTas] = min;
        }
        rearrangeHeap(&tas);
        struct couple c = popHeap(&tas);
        struct arete arete_sure;
        arete_sure.sommetA = c.sommet;
        arete_sure.sommetB = aretes[c.sommet];
        addEdge(&arpm, arete_sure.sommetA, arete_sure.sommetB, c.poids);
        sommetsArbre[nbSommetsArbre] = c.sommet;
        nbSommetsArbre ++;
        //printf("added %d -> %d : %f", arete_sure.sommetA, arete_sure.sommetB, c.poids);
    }

    //on cherche les sommets de degré impair
    int impairs[nbSommets];
    int nbSommetsDegreImpair = 0;
    for(i=0; i<nbSommets; i++){
        if(listSize(&graphe->listesAdjacences[i]) % 2 == 1){
            impairs[nbSommetsDegreImpair] = i;
            nbSommetsDegreImpair ++;
        }
    }

    //on fait le couplage de poids minimum
    int tailleTab = nbSommetsDegreImpair*factorielle(nbSommetsDegreImpair);
    int tab[tailleTab]; //tableau des parcours possibles
    //effectue(nbSommetsDegreImpair, tab);

    //printTab(tab, tailleTab, nbSommetsDegreImpair);

    //printf("\nLe circuit hamiltonien approximativement optimal pour ce graphe avec l'algorithme de Christofides a pour cout %f et est : ", cout);
    //printTab(parcours, nbSommets, 1);
}



