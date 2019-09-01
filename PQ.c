// PQ ADT interface for Ass2 (COMP2521)
//Written by Leonard Lee (z5173917) & Well Son (z5183987)

#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

//#define START_HEAPSIZE 2

// PQ implemented just like heap discussed in lectures
typedef struct PQRep {
    ItemPQ *items;        //array of items
    int nitems;            // no. of items in array (size)
    // int nslots;            // no. of elements in array
}PQRep;

//prototypes for helper functions
static void fixUp(ItemPQ a[], int i);
static void swap(ItemPQ a[], int i, int j);
static int larger(ItemPQ a, ItemPQ b);
static void fixDown(ItemPQ a[], int i, int N);



PQ newPQ() {
    PQ new = malloc(sizeof(PQRep));
    assert(new != NULL);
    ItemPQ *newItems = malloc(sizeof(ItemPQ));
    assert(newItems != NULL);
    //Initializing newPQ
    new->items = newItems;
    new->nitems = 0;
    // new->nslots = START_HEAPSIZE;
    return new;
}

int PQEmpty(PQ p) {
    if (p->nitems == 0 || p == NULL) {
        return 1;
    }
    return 0;
}

void addPQ(PQ pq, ItemPQ element) {
    assert(pq != NULL);
    int i = 1;
    bool found_element = false;
    //If item in PQ having the same key as new element that needs to be added.
    while (i <= pq->nitems) {
        if (pq->items[i].key == element.key) {
            updatePQ(pq, pq->items[i]);
        }
        i++;
    }
    // If not, key of element is not in PQ
    if (found_element == false) {
        pq->nitems++;
        pq->items = realloc(pq->items, (pq->nitems + 1) * (sizeof(struct ItemPQ)));
        pq->items[pq->nitems] = element;
        fixUp(pq->items, pq->nitems);
    }
}


ItemPQ dequeuePQ(PQ pq) {
    assert(pq->nitems >= 1);
    ItemPQ dequeued = pq->items[1];
    if (pq->nitems == 1) {
        pq->items[1] = pq->items[0]; 
    }
    else {
        pq->items[1] = pq->items[pq->nitems];
    }
    pq->nitems--;
    // Find and bring the smallest value to the top (bring the curr first item down)
    fixDown(pq->items, 1, pq->nitems);
    return dequeued;
}

void updatePQ(PQ pq, ItemPQ element) {
    assert(pq != NULL);
    for (int i = 0; i <= pq->nitems; i++) {
        if (pq->items[i].key == element.key) {
            pq->items[i].value = element.value;
            //printf("update to %d\n", pq->items[i].value);
            if (i == 1) {
                fixDown(pq->items, i, pq->nitems);
            
            }else {
                //fix up if needed
                fixUp(pq->items, i);
            }
        }
        
    }
}

void  showPQ(PQ pq) {
    int i = 1;
    printf("(%d)", pq->items[i].value);
    i++;
    
    while (i <= pq->nitems) {
        printf("--(%d)", pq->items[i].value);
        i++;
    }
    printf("\n");
}

void  freePQ(PQ pq) {
    free(pq->items);
    free(pq);
}

//__________________________________________________________________________________
//            HELPER FUNCTIONS BELOW
//__________________________________________________________________________________
// These helper functions are given from the lecture slides on Heaps.

//If a child is smaller than its parent then swap them
static void fixUp(ItemPQ a[], int i){
    while (i > 1 && larger(a[i/2],a[i]) == 1) {
        swap(a, i, i/2);
        i = i/2;
    }
}

// Change the item location with each other
static void swap(ItemPQ a[], int i, int j){
    ItemPQ tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

// Boolean to find out if a > b
static int larger(ItemPQ a, ItemPQ b){
    if (a.value > b.value){
        return 1;
    } else {
        return 0;
    }
}

// After deletion to bring the smaller value up to the root
static void fixDown(ItemPQ a[], int i, int N){
    while (2*i <= N) {
        int j = 2*i;               // floor function usage
        //Find the smaller of two children
        if (j < N && larger(a[j+1], a[j]) == 0) j++;
        //If parent is smaller than children, heap fixed
        if (larger(a[j], a[i]) == 1) break;
        //Else parent and its smallest child changes position
        swap(a, i, j);
        i = j;
    }
}