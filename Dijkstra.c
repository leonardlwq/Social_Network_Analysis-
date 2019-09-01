// Dijkstra ADT interface for Ass2 (COMP2521)
//Written by Leonard Lee (z5173917) & Well Son (z5183987)

#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static void addPred (PredNode **list,Vertex curr, int v);
static void cleanPred (PredNode **list,Vertex curr);


ShortestPaths dijkstra(Graph g, Vertex v) {

	assert (g != NULL);
	
	int nMax = numVerticies(g);
	ShortestPaths path;
	path.noNodes = nMax;
	path.src = v;
	path.dist = malloc(path.noNodes*sizeof(int));
	path.pred = malloc(path.noNodes*sizeof(PredNode));

	int visited[nMax];

	PQ new = newPQ();

	for (int i = 0; i < nMax; i++){
		if (i == v) {path.dist[i] = 0;}
		else {path.dist[i] = __INT_MAX__;}
		path.pred[i] = NULL;
		visited[i] = 0;
		ItemPQ newItem;
		newItem.key = i;
		newItem.value = path.dist[i];
		addPQ(new, newItem);
	}

	while (!PQEmpty(new)){
	
		ItemPQ cur = dequeuePQ(new);
		int n = cur.key;
		
		visited[n] = 1;

		AdjList curNode = outIncident(g, n);

		while (curNode != NULL){
			Vertex curr = curNode->w;

			if (visited[curr] == 0){
				
				int distance = 0;
				if (path.dist[n] == __INT_MAX__) { break; }
				else { distance = path.dist[n] + curNode->weight; }

				if (distance < path.dist[curr]) {

					path.dist[curr] = distance;
					cleanPred(path.pred, curr);
					addPred(path.pred ,curr,n);
					ItemPQ newItem;
					newItem.key = curr;
					newItem.value = distance;
					updatePQ(new, newItem);
					
				} else if (distance == path.dist[curr]) {
					addPred(path.pred ,curr,n);
				}
			} 
			curNode = curNode->next;
		}

	}

	for (int k = 0; k < nMax; k++){
		if (path.dist[k] == __INT_MAX__){ path.dist[k] = 0; }
	}
	
	freePQ(new);
	return path;
}

//__________________________________________________________________________________
//            HELPER FUNCTIONS BELOW
//__________________________________________________________________________________

static void cleanPred (PredNode **list,Vertex curr){
	
	while (list[curr] != NULL) {
		PredNode *tmp = list[curr];
		list[curr] = list[curr]->next;
		free(tmp);
	}
}

static void addPred (PredNode **list,Vertex curr, int v){

	PredNode *current = list[curr];

	if (current == NULL) {
		PredNode *newNode = malloc(sizeof(PredNode));
		newNode->v = v;
		newNode->next = NULL;			
		list[curr] = newNode;

	} else {
		
		if (current->v == v){ return;}

		while (current->next != NULL){

			current = current->next;
			if (current->v == v){ return;}

		}	
		
		PredNode *newNode = malloc(sizeof(PredNode));
		newNode->v = v;
		newNode->next = NULL;
		current->next = newNode;			
	}

}

void showShortestPaths(ShortestPaths paths) {
	assert (paths.pred != NULL);
	Vertex src = paths.src;
	printf("This is paths has %d nodes and start with vertex %d.\n", paths.noNodes, src);
	printf("Node %d\n Distance\n",src);
	for (int v = 0; v < paths.noNodes; v++){
		if (v == src){
			printf("   %d : x\n",v);
		
		} else {
			printf("   %d : %d\n",v, paths.dist[v]);
		}
	}
	printf(" Preds\n");
	for (int i = 0; i < paths.noNodes; i++){
			printf("  %d : ",i);
		if (paths.pred[i] == NULL){
			printf("NULL\n");
		} else {

			while (paths.pred[i]!= NULL){
					printf("[%d]->",paths.pred[i]->v);

				if (paths.pred[i]->next == NULL){
					printf("NULL\n");
					break;
				}
				paths.pred[i] = paths.pred[i]->next;
			}
		}	
	}
	
}


void  freeShortestPaths(ShortestPaths paths) {
	for (int i = 0; i < paths.noNodes; i++){
		cleanPred(paths.pred, i);
	}
	free(paths.pred);
	free(paths.dist);
}