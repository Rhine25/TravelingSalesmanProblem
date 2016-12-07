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
    //printf("Pushed smtg : ");
    //printTab(self->tab, self->size*2, 2);
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

void sortHeap(struct heap *self, int size){
    int i;
    for(i = size/2; i>=0; i--){
        tamiser(self, i, size);
    }
    for(i = size; i>=1; i--){
        struct pair tmp = self->tab[i];
        self->tab[i] = self->tab[0];
        self->tab[0] = tmp;
        tamiser(self, 1, i-1);
    }
    printf("Sorted : ");
    printTab(self->tab, self->size*2, 2);
}

void tamiser(struct heap* self, int sommet, int n){
    int k = sommet;
    int j = 2 * k;

    while(j <= n){
        if(j < n && self->tab[j].elem[1] < self->tab[j+1].elem[1]){
            j ++;
        }
        if(self->tab[k].elem[1] < self->tab[j].elem[1]){
            struct pair tmp = self->tab[k];
            self->tab[k] = self->tab[j];
            self->tab[j] = tmp;
            k = j;
            j = 2 * k;
        }
        else{
            j = n;
        }
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
    int last = self->size-1;
    //int pere = father(current);

    //TODO check if < ou <= self size
    /* Jolie boucle pour si on ne trie pas
     * for(i=0; i<=self->size; i++){
        int sommet = self->tab[i].elem[0];
        nodeTmp = searchNode(&list, sommet);
        float poids = nodeTmp->poids;
        self->tab[i].elem[1] = poids;
        printTab(self->tab, self->size*2, 2);
    }*/

    /* Presque Utile boucle pour si on trie à chaque poids
    for(i=0; i<=self->size; i++){
        int sommet = self->tab[last].elem[0];
        nodeTmp = searchNode(&list, sommet);
        float poids = nodeTmp->poids;
        self->tab[last].elem[1] = poids;
        sortHeap(self);
        printTab(self->tab, self->size*2, 2);
    }*/

    //pour tous les sommet du graphe sauf ceux mis dans le parcours
    for(i=0; i<g->nbMaxSommets-1; i++){
        int sommet = self->tab[i].elem[0];
        nodeTmp = searchNode(&list, sommet);
        float poids = nodeTmp->poids;
        self->tab[i].elem[1] = poids;
    }
    printf("Weight : ");
    printTab(self->tab, self->size*2, 2);
    sortHeap(self, self->size);

    free(nodeTmp);
}

int father(int elem){
    return ((elem+1) / 2) - 1;
}

int left_child(int elem){
    right_child(elem) - 1;
}

int right_child(int elem){
    return 2 * (elem + 1);
}

void heapUp(struct heap* self, int elem){
    int l = left_child(elem);
    int r = right_child(elem);
    int min = elem;
    if(l < self->size && self->tab[l].elem[1] < self->tab[elem].elem[1]){
        min = l;
    }
    if(r < self->size && self->tab[r].elem[1] < self->tab[min].elem[1]){
        min = r;
    }
    if(min != elem){
        swap(self, elem, min);
        heapUp(self, min);
    }
}

void rearrangeHeap(struct heap* self){
    int i;
    for(i = ((self->size+1)/2)-1; i>=1; i--){ //revoir le /2 pour coller avec le commençage de l'indexage à 0
        heapUp(self, i);
    }
}

void swap(struct heap* self, int i, int j){
    struct pair tmp = self->tab[i];
    self->tab[i] = self->tab[j];
    self->tab[j] = tmp;
}


