// Graph ADT interface for Ass2 (COMP2521)
//Written by Leonard Lee (z5173917) & Well Son (z5183987) 

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//prototypes for helper functions
static double recordWays (ShortestPaths paths, double *between, Vertex current, Vertex v);

NodeValues outDegreeCentrality(Graph g){
    NodeValues result;
    result.noNodes = numVerticies(g);
    result.values = malloc(result.noNodes * sizeof(double));
    Vertex v = 0;
    while (v < result.noNodes) {
        AdjList curOut = outIncident(g, v);
        double count = 0;
        while (curOut != NULL) {
            count ++;
            curOut = curOut->next;
        }
        result.values[v] = count;
        v++;
    }
    return result;
}


NodeValues inDegreeCentrality(Graph g){
    NodeValues result;
    result.noNodes = numVerticies(g);
    result.values = malloc(result.noNodes * sizeof(double));
    Vertex v = 0;
    while (v < result.noNodes) {
        AdjList curIn = inIncident(g, v);
        double count = 0;
        while (curIn != NULL) {
            count ++;
            curIn = curIn->next;
        }
        result.values[v] = count;
        v++;
    }
    return result;
}


NodeValues degreeCentrality(Graph g) {
    NodeValues result;
    result.noNodes = numVerticies(g);
    result.values = malloc(result.noNodes * sizeof(double));
    
    NodeValues out = outDegreeCentrality(g);
    NodeValues in = inDegreeCentrality(g);
    
    for (int i = 0;i < numVerticies(g); i++){
        result.values[i] = out.values[i] + in.values[i];
    }
    return result;
}

NodeValues closenessCentrality(Graph g){
    NodeValues result;
    result.noNodes = numVerticies(g);
    result.values = malloc(result.noNodes * sizeof(double));

    for (int i = 0; i < result.noNodes; i++) {
        //count for the number of nodes that vertex i can reach
        double count = 1;
        //count for the distance that vertex i took to reach
        double totalDist = 0;

        ShortestPaths paths = dijkstra(g, i);

        for (int j = 0; j < numVerticies(g); j++){
            if (paths.dist[j] > 0) { count++; }
            totalDist += paths.dist[j];
        }
        // Wasserman and Faust formula 
        double n = count - 1;
        double N = result.noNodes - 1;
        // If the total distance is 0 then change to 1 to prevent zero divide error
        if (totalDist == 0) {totalDist = 1;}
        double wff = (n * n)/(totalDist * N);
        result.values[i] = wff;
        freeShortestPaths(paths);
    }
    
    return result;
}


NodeValues betweennessCentrality(Graph g){
    NodeValues result;
    result.noNodes = numVerticies(g);
    result.values = malloc(result.noNodes * sizeof(double));
    result.values = memset(result.values, 0, result.noNodes * sizeof(double));
    
    // count Vertex v in between start and dest
    for (Vertex v = 0; v < result.noNodes; v++) {

        // Loop for start
        for (Vertex w = 0; w < result.noNodes; w++) {

            // if start is equal to the vertex we want to search for betweenness 
            // then go to new starting point
            if (v == w) continue;
            ShortestPaths tempDijk = dijkstra(g, w);

        // Loop for dest 
            for (Vertex x = 0; x < result.noNodes; x++) {

                // If the v is equal to either starting point or end point then change new start and dest
                if (x == w || x == v) continue;    

                // count for the Vertex v in between start: w and dest: x
                double countBtw = 0;      
               
                // count for the total number of way from src to dest    
                double countTot = recordWays(tempDijk, &countBtw, x, v);

                // if the countTot is zero will cause the zero divider error so change it to 1
                if (countTot == 0) countTot = 1;
                result.values[v] += countBtw / countTot;
            }    
        }
    }

    return result;
}

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues resultN = betweennessCentrality(g);
    if (numVerticies(g) <2) {return resultN;}
    double nV = numVerticies(g);
	double denominator = 1/((nV - 1)*(nV - 2));

	int i;
	for (i = 0; i < nV; i++) {
		resultN.values[i] *= denominator;
	}
     return resultN;
}

void showNodeValues(NodeValues values){
    int i;
	for (i = 0; i < values.noNodes; i++) {
		printf("%d: %f\n", i, values.values[i]);
	}
}
void freeNodeValues(NodeValues values){
    free(values.values);
}


//__________________________________________________________________________________
//            HELPER FUNCTION(S) BELOW
//__________________________________________________________________________________
//TODO: 
static double recordWays (ShortestPaths paths, double *between, Vertex current, Vertex v) {

    // If reached the dest then make increment
    if (current == paths.src) return 1;

    // If the current node is not linking to anything, don't count
    if (paths.pred[current] == NULL) return 0;
    else {

        double countTot = 0;
        // Loop through the predecessor list to find countBtw
        for (PredNode *curr = paths.pred[current]; curr != NULL; curr = curr->next) {

            countTot += recordWays (paths, between, curr->v, v);

        }
        if (current == v) *between += countTot;
        return countTot;
    }  
      

}