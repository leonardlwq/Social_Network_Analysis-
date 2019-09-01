/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by Leonard Lee (z5173917) & Well Son (z5183987)
  COMP2521 2019 T1
*/

#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#define numVertices numVerticies
#include <string.h>
#include <math.h>

/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */

//prototypes for helper functions
static Dendrogram initDen(int v);
static Dendrogram *updateDendrogram(Dendrogram *arr, int verV, int verW, int size); 
static double **updateDistance(double **distance, int size, int verV, int verW, int method);


Dendrogram LanceWilliamsHAC(Graph g, int method) {
    // First, creating the array for Dendrograms, allocating memory for them.
    int noV = numVertices(g);
    Dendrogram *a = malloc(sizeof(Dendrogram) * noV);

    for (int i = 0; i < noV; i++) {
        a[i] = initDen(i);
    }
    // Creating the 2d distance array 
    double **distA = malloc(sizeof(double) * noV);

    for (int i = 0; i < noV; i++) {
        distA[i] = malloc(sizeof(double) * noV);
        memset(distA[i], 0, sizeof(double) * noV);
    }
    // Creating starting distances for the individual single clusters
    for (Vertex src = 0; src < noV; src++) {
        for (int i = src+1; i < noV; i++) {
            // no data for x=y array
            AdjList out = outIncident(g, src);
            AdjList in = outIncident(g, i);
            int maxEdgWeight = 0;
            // for out incident edges 
            if (out != NULL) {
                AdjList temp = out;
                while(temp != NULL) {
                    if(temp->w == i && temp->weight > maxEdgWeight) { 
                        maxEdgWeight = temp->weight;
                    }
                    temp = temp->next;
                }
            }
            // for in incident edges 
            if (in != NULL) {
                AdjList temp = in;
                while (temp != NULL) {
                    if (temp->w == src && temp->weight > maxEdgWeight) {
                        maxEdgWeight = temp->weight;
                    }
                    temp = temp->next;
                }
            }
            if (maxEdgWeight != 0) {
                distA[src][i] = 1 / (double)maxEdgWeight;
                distA[i][src] = distA[src][i];
            } else {
                distA[src][i] = __INT_MAX__;
                distA[i][src] = distA[src][i];
            }

        }
    }


    for (int i = noV; i > 1; i--) {
        double minEdgWeight = distA[0][1];
        int verV = 0;
        int verW = 1;
        for (int x = 0; x < i; x++) {
            for (int y = x+1; y < i; y++) {
                if (distA[x][y] < minEdgWeight) {
                    minEdgWeight = distA[x][y];
                    verV = x;
                    verW = y;
                }
            }
        }
        //updating the tree and distance arrays
        a = updateDendrogram(a, verV, verW, i);       
        distA = updateDistance(distA, i, verV, verW, method);
    }
    Dendrogram result = a[0];
    free(a);
    free(distA[0]);
    free(distA);
    return result;
}


void freeDendrogram(Dendrogram d) {
    if (d == NULL) {
        return;
    }
    if (d->left != NULL) freeDendrogram(d->left);
    if (d->right != NULL) freeDendrogram(d->right);
    free(d);
}

//__________________________________________________________________________________
//            HELPER FUNCTION(S) BELOW
//__________________________________________________________________________________

static Dendrogram initDen(int v) {
    Dendrogram init = malloc(sizeof(DNode));
    init->left = init->right = NULL;
    init->vertex = v;
    return init;
}


static double **updateDistance(double **distance, int size, int verV, int verW, int method) {
    //allocate new distance matrix with dimension of (size-1)*(size-1)
    double **newDis = malloc(sizeof(double)*(size-1));
    for(int i = 0; i < (size-1); i++) {
        newDis[i] = malloc(sizeof(double)*(size-1));
        memset(newDis[i], 0, sizeof(double)*(size-1));
    }
    //copying old distance matrix to new one because verV and verW will be updated
    int nCol = 0, nRow = 0;
    for(int i = 0; i < size; i++) {
        if(i == verV || i == verW) continue;
        for(int x = 0; x < size; x++) {
            if(x == verV || x == verW) continue;
            newDis[nRow][nCol] = distance[i][x];
            nCol++;
        }
        nCol = 0;
        nRow++;
    }
    //calculate distance for the new clusters
    int dCol = 0;
    double minD = 0;
    double maxD = 0;
    for(int i = 0; i < size; i++) {
        if(i == verV || i == verW) continue;
        //Using the method to decide how to calculate new distance
        // 1 == single link
        // 2 == complete link
        if(method == 1) {
            if (distance[verV][i] <= distance[verW][i]) {
                minD = distance[verV][i];
            }
            else {
                minD = distance[verW][i];
            }
            
            newDis[size-2][dCol] = minD;
            newDis[dCol][size-2] = minD;
        } else if (method == 2) {
            if (distance[verV][i] >= distance[verW][i]) {
                maxD = distance[verV][i];
            }
            else {
                maxD = distance[verW][i];
            }

            newDis[size-2][dCol] = maxD;
            newDis[dCol][size-2] = maxD;
        }
        dCol++;
    }

    for(int i = 0; i < size; i++) {
        free(distance[i]);
    }
    free(distance);
    return newDis;
}

static Dendrogram *updateDendrogram(Dendrogram *arr, int verV, int verW, int size) {
    Dendrogram *newArr = malloc(sizeof(Dendrogram)*(size-1));
    int newCol = 0;
    for(int i = 0; i < size; i++) {
      if(i == verV || i == verW) continue;
      newArr[newCol] = arr[i];
      newCol++;
    }
    //merge the new tree and place it at the end of new array
    Dendrogram combi = initDen(-1);
    combi->left = arr[verV];
    combi->right = arr[verW];
    newArr[size-2] = combi;
    free(arr);
    return newArr;  
}