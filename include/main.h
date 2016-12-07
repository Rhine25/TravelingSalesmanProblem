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
void sublist(int* liste, int taille, int val, int* sub);

void arrangements(int n, int k, int *L, int *t, int* tab);
void effectue(int n, int* tab);

#endif //MAIN_H

float max(struct list* self, int size);

float poids(struct graph* self, int sommet_source, int sommet_cible);
