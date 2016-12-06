#include <stdlib.h>
#include <stdio.h>

#include "../include/heap.h"
#include "../include/main.h"
#include "../../Projet1/include/libgraphe.h"
#include "../../Projet1/include/libliste.h"

struct heap createHeap(){
    struct heap self;
    self.tab = malloc(32*sizeof(struct pair));
    self.capacity = 32;
    self.size = 0;
    return self;
}

struct pair createPair(int sommet, int poids){
    struct pair p;
    p.elem[0] = sommet;
    p.elem[1] = poids;
    return p;
}

void destroyHeap(struct heap *self){
    free(self->tab);
    free(self);
}

void pushHeap(struct heap *self, struct pair values){
    if(self->size == self->capacity){
        expandHeap(self);
    }
    self->tab[self->size] = values;
    self->size ++;
    sortHeap(self);
}

struct pair popHeap(struct heap *self){
    struct pair tmp;
    int indexUp = 1;
    int indexDn = 2;

    struct pair value;
    value.elem[0] = -1;
    value.elem[1] = -1;
    if(isEmptyHeap(self)) return value;

    value = self->tab[1];
    self->tab[1] = self->tab[self->size];
    self->size--;

    while(indexDn<=self->size)
    {
        if(indexDn+1 <= self->size && self->tab[indexDn].elem[1] < self->tab[indexDn+1].elem[1])
        {
            indexDn++;
        }
        if(self->tab[indexDn].elem[1] > self->tab[indexUp].elem[1])
        {
            tmp = self->tab[indexDn];
            self->tab[indexDn] = self->tab[indexUp];
            self->tab[indexUp] = tmp;
        }
        indexUp = indexDn;
        indexDn *= 2;
    }
    return value;
}

int isEmptyHeap(struct heap *self){
    return self->size == 0;
}

/*void rearrangeHeap(struct heap *self){
    //TODO fix not sorting,
    struct pair tmp;
    int size = self->size;
    int index = size/2;
    while(self->tab[index].elem[1] > self->tab[size].elem[1] && size>1) {
        tmp = self->tab[size];
        self->tab[size] = self->tab[index];
        self->tab[index] = tmp;

        index/=2;
        size/=2;
    }
}*/

void sortHeap(struct heap *self){
    //TODO fix not sorting,
    struct pair tmp;
    int size = self->size;
    int index = size/2;
    while(self->tab[index].elem[1] > self->tab[size].elem[1] && size>1) {
        int i;
        //printTab(self->tab, self->size*2, 2);
        tmp = self->tab[size];
        self->tab[size] = self->tab[index];
        self->tab[index] = tmp;
        index/=2;
        size/=2;
    }
}

void expandHeap(struct heap *self){
    int new_size = 2*self->capacity;
    self->tab = realloc(self->tab, new_size*sizeof(int));
    self->capacity = new_size;
}

void updateWeights(struct heap *self, struct graph *g, int dernierSommetParcours){
    struct list list = g->listesAdjacences[dernierSommetParcours];
    int i;
    struct list_node* nodeTmp = malloc(sizeof(struct list_node));
    for(i = 0; i<dernierSommetParcours; i++){
        nodeTmp = searchNode(&list, i);
        self->tab[i].elem[1] = nodeTmp->poids;
        //sortHeap(self);
    }
    for(i=i+1; i<=self->size; i++){
        nodeTmp = searchNode(&list, i);
        self->tab[i-1].elem[1] = nodeTmp->poids;
        //sortHeap(self);
    }
    int size = self->size;
    int index = size/2;

    free(nodeTmp);
}


