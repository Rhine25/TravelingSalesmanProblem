#ifndef MAIN_H
#define MAIN_H

#include "../../Projet1/include/libliste.h"

struct sommet{
    int x;
    int y;
};

int factorielle(int n);
void printTab(int* tab, int taille, int tailleBloc);
void sublist(int* liste, int taille, int val, int* sub);

void arrangements(int n, int k, int *L, int *t, int* tab);
void effectue(int n, int* tab);

float max(struct list* self, int size);

#endif //MAIN_H