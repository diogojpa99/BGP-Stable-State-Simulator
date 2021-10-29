#ifndef READFILE_INCLUDED
#define READFILE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>





#define MAX_ID 65536

#ifndef INFINITE
#define INFINITE 1000000
#endif 


enum event_type{annouce, rep_annouce};

// Implementar o grafo pelo método de lista de adjacências
typedef struct _Neighbours
{
    int neighbour_id;
    int neighbour_estim_cost;
    int type;
    struct _Neighbours *next_neighbour;   
} Neighbours;

typedef struct _Adj{
    int id, neighbor, type, An; //What's the commercial relation between me and my "list head"  and what's the cost from the link between me and my "list head"
    char message[6];
    struct _Adj *next;
    struct _Nodes *node_pointer;
} Adj;

typedef struct _DestinyNode{
    int dest_id;//destino a que consigo chegar
    int cost;
    int type;
    int chosen_neighbour_id;//vizinho por onde recebi a informacao do destino
    Neighbours *neighbours_head; 
    struct _DestinyNode *next_dest;
} DestNode;

typedef struct _Nodes{
    int id;
    Adj *adjHead;
    DestNode *destHead;
    struct _Nodes *next;
} Nodes;

typedef struct _Event{
    int An; //Tempo de chegada ao destino do evento, ou seja, tempo em que esse evento vai ser processado
    int dest_node; //Se dest_node = -1 então trata-se de uma primeira menssagem quando um nó ainda não sabe quem são sos seus vizinhos
    int origin_node;
    int type;
    int message[3];// Vetor na forma | nó1 - nó de inicio | nó2 | estimativa de custo de 1 para 2 |
    Nodes *dest_node_pointer;
    struct _Event *next;
} Event;

typedef struct _Queue{
    int node_id;
    int type;
    int cost;
    DestNode *dest;
    Nodes *node;
    struct _Queue *next;
} Queue;




void commandLineValidation(int argc, char **argv, int *origin_id, int *dest_id);

void write_times();

void write_types_costs_routs(Nodes *nodes_head, int mode);

void InteractiveMode(Nodes *nodes_head);

void writeStatistics();



#endif //READFILE INCLUDED