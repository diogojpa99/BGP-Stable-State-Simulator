#ifndef NODES_INCLUDED
#define NODES_INCLUDED

#include "readFile.h"





//Nodes

/*Funcao que cria o grafo a partir do ficheiro dado, na forma de uma lsta de adjacencias*/
Nodes *createGraph(Nodes *listHead, int tail, int head, int type);

/*Funcao que cria um vertice para a lista de adjacencias*/
Nodes *createNode(Nodes *listHead, int tail);

/*Funcao que cria um vertice adjacente para ser inserido na lista de adjacencias de um vertice*/
Adj *createAdj(Adj *listHead, int tail, int head, int type);

/*Funcao que insere o vertice adjacente na lista de vertices adjacentes de um no*/
Adj *insertAdj(Adj *listHead, Adj *newAdj);

/*Funcao que insere um vertice criado, na lista de adjacencias*/
Nodes *insertNode(Nodes *listHead, Nodes *newNode, int id);

/*Funcao que procura um dado vertice na lista de adjacencias*/
Nodes *searchNodesList(Nodes *listHead, int id);

/*Funcao que imprime a lista de vetices*/
void Print_List_of_Nodes(Nodes *nodes_head);

/*Funcao que imprime a lista de adjacencias*/
void Print_List_of_Adjacencies(Nodes *list_Head);

/*Funcao que atualiza a estimativa de um no para um destino anunciado*/
DestNode *updateDestToNode(Nodes *process_node, int *message, int type);

/*Funcao que procura por um dado destino na lista de destinos de um no*/
DestNode *searchDestiny(DestNode *dest_head, int dest_id);

/*Funcao que procura por um dado destino na lista de destinos de um vertice*/
DestNode *createDestiny(DestNode *dest_head, int neigbour_id, int dest_id, int cost, int type);

/*Funcao que imprime a lista de destinos de um vertice*/
void Print_List_of_Destinations(Nodes *nodes_Head, int mode);

/*Funcao que desaloca memoria*/
void freeGraphNodes(Nodes *nodes_head);

/*Funcao que desaloca memoria*/
void clearAllDest(Nodes *nodes_head);

/*Funcao que desaloca memoria*/
void clearDestinys(DestNode *list);

/*Funcao que desaloca memoria*/
void clearNeighbours(Neighbours *list);

/*Funcao que mete todos os nos adjacentes, na lista de adjacencias, a apontarem para si proprios na lista de adjacencias principal*/
Nodes *AdjToNode(Nodes *listHead);

/*Funcao que cria uma estrutura destino*/
DestNode *createDestinyAlgorithm(DestNode *dest_head, int node_id, int dest_id);

/*Funcao que inicializa o algoritmo de Bellman-Ford*/
DestNode *initDestinyDijkstra(DestNode *dest_head, int node_id, int dest_id);

/*Funcao que cria uma estrutura vizinho*/
Neighbours *createNeighbourToDestiny(int *message, int type);

/*Funcao que, dado um vizinho, o insere na lista de vizinhos de um dado destino, para um dado no*/
Neighbours *insertNeighbourtOrdered(Neighbours *neighbours_head, Neighbours *neighbour_to_insert);

/*Funcao que procura e devolve um dado vizinho para alcancar um dado destino*/
Neighbours *searchForNeighbourToDestiny(Neighbours *neighbours_head, int neighbour_id, int type);

/*Funcao que ordena a lista de nos que consegue chegar a um dado destino, de um dado no*/
Neighbours *orderNeighboursToDestinyAscendent(Neighbours *neighbours_head);

/*Funcao que troca a ordem de dois vizinhos de um dado no, para um dado destino*/
Neighbours *switch_neighbours(Neighbours *left, Neighbours *right);

/*Funcao que imprime a lista de destinos de um no*/
void printDestiny(DestNode *destiny_head);

/*Funcao que atualiza a estimativa de um vertice para um dado destino ao qual o vertice consegue alcancar*/
int updateEstimateToDestiny(DestNode *current_dest);


#endif //NODES INCLUDED