#include <stdlib.h>
#include <stdio.h>

#include "../include/heap.h"
#include "../include/main.h"
#include "../../Projet1/include/libgraphe.h"
#include "../../Projet1/include/libliste.h"

struct heap createHeap(){
    struct heap self;
    //self.tab = malloc(32*sizeof(struct pair));
    self.sommets = malloc(32*sizeof(int));
    self.poids = malloc(32*sizeof(float));
    self.capacity = 32;
    self.size = 0;
    return self;
}

/*struct pair createPair(int sommet, int poids){
    struct pair p;
    p.elem[0] = sommet;
    p.elem[1] = poids;
    return p;
}*/

void destroyHeap(struct heap *self){
    //free(self->tab);
    free(self->sommets);
    free(self->poids);
}

//void pushHeap(struct heap *self, struct pair values){
void pushHeap(struct heap *self, int sommet, float poids){
    if(self->size == self->capacity){
        expandHeap(self);
    }
    //self->tab[self->size] = values;
    self->sommets[self->size] = sommet;
    self->poids[sommet] = poids;
    self->size ++;
}

//struct pair popHeap(struct heap *self){
struct couple popHeap(struct heap *self){
    //struct pair value = self->tab[0];
    struct couple value;
    value.sommet = self->sommets[0];
    value.poids = self->poids[self->sommets[0]];
    //TODO attention, le tableau poids quand un sommet est pop du tas reste de la même taille (correspond à tous les sommets quand même)
    self->size--;
    //self->tab[0] = self->tab[self->size];
    self->sommets[0] = self->sommets[self->size];
    rearrangeHeap(self);
    return value;
}

int isEmptyHeap(struct heap *self){
    return self->size == 0;
}

void expandHeap(struct heap *self){
    int new_size = 2*self->capacity;
    //self->tab = realloc(self->tab, new_size*sizeof(int));
    self->sommets = realloc(self->sommets, new_size*sizeof(int));
    self->poids = realloc(self->poids, new_size*sizeof(float));
    self->capacity = new_size;
}

void updateWeights(struct heap *self, struct graph *g, int dernierSommetParcours){

    int i;
    //pour tous les sommet du graphe sauf ceux mis dans le parcours
    for(i=0; i<self->size; i++){
        //self->tab[i].elem[1] = poids(g, dernierSommetParcours, self->tab[i].elem[0]);
        self->poids[self->sommets[i]] = poids(g, dernierSommetParcours, self->sommets[i]);
    }
    rearrangeHeap(self);
}

int father(int elem){
    return ((elem+1) / 2) - 1;
}

int left_child(int elem){
    return right_child(elem) - 1;
}

int right_child(int elem){
    return 2 * (elem + 1);
}

void heapUp(struct heap* self, int elem){
    int l = left_child(elem);
    int r = right_child(elem);
    int min = elem;
    //if(l < self->size && self->tab[l].elem[1] < self->tab[elem].elem[1]){
    if(l < self->size && self->poids[self->sommets[l]] < self->poids[self->sommets[elem]]){
        min = l;
    }
    //if(r < self->size && self->tab[r].elem[1] < self->tab[min].elem[1]){
    if(r < self->size && self->poids[self->sommets[r]] < self->poids[self->sommets[min]]){
        min = r;
    }
    if(min != elem){
        swap(self, elem, min);
        heapUp(self, min);
    }
}

void rearrangeHeap(struct heap* self){
    int i;
    for(i = ((self->size)/2)-1; i>=0; i--){ //revoir le /2 pour coller avec le commençage de l'indexage à 0
        heapUp(self, i);
    }
}

void swap(struct heap* self, int i, int j){
    int tmp = self->sommets[i];
    self->sommets[i] = self->sommets[j];
    self->sommets[j] = tmp;

    //DISCLAIMER : on ne trie pas les poids car dans le pop on accède au poids selon le numéro du sommet pour ne pas avoir à effectuer le double de tri alors qu'on peut garder les accès en instant'
}


