#ifndef TSP_HEAP_H
#define TSP_HEAP_H

#include "../../Projet1/include/libgraphe.h"

struct heap{
    int capacity;
    int size;
    int* sommets;
    float* poids;
};

struct couple{
    int sommet;
    float poids;
};

struct arete{
    int sommetA;
    int sommetB;
};

struct heap createHeap();
struct pair createPair(int sommet, int poids);
void destroyHeap(struct heap *self);
//void pushHeap(struct heap *self, struct pair values);
void pushHeap(struct heap *self, int sommet, float poids);
//struct pair popHeap(struct heap *self);
struct couple popHeap(struct heap *self);
int isEmptyHeap(struct heap *self);
void expandHeap(struct heap *self);
void updateWeights(struct heap *self, struct graph *g, int dernierSommetParcours);

int father(int elem);
int left_child(int elem);
int right_child(int elem);
void heapUp(struct heap* self, int elem);
void rearrangeHeap(struct heap* self);
void swap(struct heap* self, int i, int j);

//parcours en profondeur
void dfsb(const struct graph *self, int state, int* parcours);
void dfsb2(const struct graph *self, int state, int* visited, int* parcours, int* parcoursCourant);
#endif //TSP_HEAP_H
