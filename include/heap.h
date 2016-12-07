#ifndef TSP_HEAP_H
#define TSP_HEAP_H

#include "../../Projet1/include/libgraphe.h"

struct heap{
    int capacity;
    int size;
    struct pair* tab;
};

struct pair{
    int elem[2]; //elem[0] sommet et elem[1] poids depuis le sommet courant
};

struct heap createHeap();
struct pair createPair(int sommet, int poids);
void destroyHeap(struct heap *self);
void pushHeap(struct heap *self, struct pair values);
struct pair popHeap(struct heap *self);
int isEmptyHeap(struct heap *self);
void tamiser(struct heap* self, int sommet, int n);
void expandHeap(struct heap *self);
void updateWeights(struct heap *self, struct graph *g, int dernierSommetParcours);

int father(int elem);
int left_child(int elem);
int right_child(int elem);
void heapUp(struct heap* self, int elem);
void rearrangeHeap(struct heap* self);
void swap(struct heap* self, int i, int j);

#endif //TSP_HEAP_H
