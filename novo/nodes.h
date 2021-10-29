#ifndef NODES_INCLUDED
#define NODES_INCLUDED

#include "readFile.h"





//Nodes

Nodes *createGraph(Nodes *listHead, int tail, int head, int type);

Nodes *createNode(Nodes *listHead, int tail);

Adj *createAdj(Adj *listHead, int tail, int head, int type);

Adj *insertAdj(Adj *listHead, Adj *newAdj);

Nodes *insertNode(Nodes *listHead, Nodes *newNode, int id);

Nodes *searchNodesList(Nodes *listHead, int id);

void Print_List_of_Nodes(Nodes *nodes_head);

void Print_List_of_Adjacencies(Nodes *list_Head);

DestNode *updateDestToNode(Nodes *process_node, int *message, int type);

DestNode *searchDestiny(DestNode *dest_head, int dest_id);

DestNode *createDestiny(DestNode *dest_head, int neigbour_id, int dest_id, int cost, int type);

void Print_List_of_Destinations(Nodes *nodes_Head, int mode);

void freeGraphNodes(Nodes *nodes_head);

void clearAllDest(Nodes *nodes_head);

void clearDestinys(DestNode *list);

void clearNeighbours(Neighbours *list);

Nodes *AdjToNode(Nodes *listHead);

DestNode *createDestinyAlgorithm(DestNode *dest_head, int node_id, int dest_id);

DestNode *initDestinyDijkstra(DestNode *dest_head, int node_id, int dest_id);

Neighbours *createNeighbourToDestiny(int *message, int type);

Neighbours *insertNeighbourtOrdered(Neighbours *neighbours_head, Neighbours *neighbour_to_insert);

Neighbours *searchForNeighbourToDestiny(Neighbours *neighbours_head, int neighbour_id, int type);

Neighbours *orderNeighboursToDestinyAscendent(Neighbours *neighbours_head);

Neighbours *switch_neighbours(Neighbours *left, Neighbours *right);

void printDestiny(DestNode *destiny_head);

int updateEstimateToDestiny(DestNode *current_dest);

#endif //NODES INCLUDED