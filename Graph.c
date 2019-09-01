// Graph ADT interface for Ass2 (COMP2521)
// Written by Leonard Lee (z5173917) & Well Son (z5183987)

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Graph.h"

typedef struct GraphRep {
    int nV;        //No. vertices in the graph
    int nE;        //No. of edges in the graph
    adjListNode **edges;  // Edges adjacency list
} GraphRep;


Graph newGraph(int noNodes) {
    assert (noNodes != 0);
    Graph new = malloc(sizeof(GraphRep));
    assert (new != NULL);
    
    new->nV = noNodes;
    new->nE = 0;
    new->edges = malloc(noNodes * sizeof(adjListNode *));
    assert (new->edges != NULL);
    for(int i = 0; i < noNodes; i++) {
        new->edges[i] = NULL;
    }
    return new;
}


int numVerticies(Graph g) {
    assert (g != NULL);
    return g->nV;
}

void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    assert (g != NULL && src >= 0);
    AdjList newE = malloc(sizeof(adjListNode));
    // Creating the new edge
    newE->weight = weight;
    newE->w = dest;
    newE->next = NULL;
    AdjList curr = g->edges[src];
    // If scr doesn't have edges
    if (curr == NULL) {
        g->edges[src] = newE;
    }
    // go to the end of src list
    else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newE;
    }
    g->nE++;
}

void  removeEdge(Graph g, Vertex src, Vertex dest) {
    assert (g != NULL);
    AdjList curr = g->edges[src];
    //Removing an edge at the start of AdjList
    if (curr->w == dest) {
        g->edges[src] = curr->next;
        free(curr);
        return;
    }
    // Removing an edge at the middle or the end of AdjList
    while (curr->next != NULL) {
        if (curr->next->w == dest) {
            AdjList temp = curr->next;
            curr->next = curr->next->next;
            free(temp);
            return;
        }
        curr = curr->next;
    }
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
    assert (g != NULL);
    AdjList curr;
    for (curr = g->edges[src]; curr != NULL; curr = curr->next) {
        if (curr->w == dest) {
            return true;
        }
    }
    return false;
}

AdjList outIncident(Graph g, Vertex v) {
    assert (g != NULL);
    return g->edges[v];
}

AdjList inIncident(Graph g, Vertex v) {
    assert (g != NULL);
    AdjList head = NULL;
    AdjList temp = NULL;
    for (int i = 0; i < g->nV; i++) {
        AdjList cur = g->edges[i];
        while (cur != NULL) {
            //If edge exists create a new node and add it into created list
            if (cur->w == v) {
                AdjList new = malloc(sizeof(adjListNode));
                new->w = i;
                new->weight = cur->weight;
                new->next = NULL;
                //empty list case
                if (head == NULL) {
                    head = new;
                    temp = new;
                }
                else {
                    temp->next = new;
                    temp = temp->next;
                }
            }
            cur = cur->next;
        }
    }
    return head;
}

void  showGraph(Graph g) {
    assert (g != NULL);
    printf("#Vertices in graph = %d,  #Edges in graph = %d\n", g->nV, g->nE);
    for (int i = 0; i < g->nV; i++) {
        AdjList curr = g->edges[i];
        printf("Vertices connected to vertex %d:  ", i);
        while (curr != NULL) {
            printf("%d with weight %d  ", curr->w, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
}

void  freeGraph(Graph g) {
    assert (g != NULL);
    for (int v = 0; v < g->nV; v++) {
        AdjList curr = g->edges[v];
        while (curr != NULL) {
            AdjList temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(g);
}

