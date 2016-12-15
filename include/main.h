#ifndef MAIN_H
#define MAIN_H

#include "../../Projet1/include/libliste.h"
#include "../../Projet1/include/libgraphe.h"

struct sommet{
    int x;
    int y;
};

int factorielle(int n);
void printTab(int* tab, int taille, int tailleBloc);
void printTabF(int* tab, int taille, int tailleBloc);
void sublist(int* liste, int taille, int val, int* sub);

void arrangements(int n, int k, int *L, int *t, int* tab);
void effectue(int n, int* tab);

#endif //MAIN_H

float max(struct list* self, int size);

float poids(struct graph* self, int sommet_source, int sommet_cible);

void solution_exacte(struct graph* graphe);
void solution_plus_proche_voisin(struct graph* graphe);
void solution_plus_petit_detour(struct graph* graphe);
void solution_ARPM(struct graph* graphe);
void solution_christofides(struct graph* graphe);