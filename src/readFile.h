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

typedef struct _Neighbours
{
    int neighbour_id;
    int neighbour_estim_cost;
    int type;
    struct _Neighbours *next_neighbour;   
} Neighbours;


/*
id - id do vizinho
neighbor - 
type - tipo de relacao comercial na perspetiva do do adjacente
An - instante da ultima mensagem a que esta na fila de espera da ligacao com o adjacente
next - apontador para o proximo adjacente
node_pointer - apontador para o correspondente no na lista principal de nos
*/
typedef struct _Adj{
    int id, neighbor, type, An;
    struct _Adj *next;
    struct _Nodes *node_pointer;
} Adj;


/*
dest_id - id do destino
cost - custo para o destino
type - relacao comercial com o no eleito para chegar ao destino
chosen_neighbour_id - id do no escolhido para chegar ao destino
neighbours_head - apontador para a cabeca da lista de vizinhos pelos quais um no consegue chegar ao destino de id dest_id
next_dest - apontador para o proximo destino
*/
typedef struct _DestinyNode{
    int dest_id;
    int cost;
    int type;
    int chosen_neighbour_id;
    Neighbours *neighbours_head; 
    struct _DestinyNode *next_dest;
} DestNode;

/*
id - id do no
adjHead - apontador para a cabeca da lista de nos adjacentes
destHead - apontador para a lista de destinos a que o no consegue chegar
next - apontador para o proximo no
*/
typedef struct _Nodes{
    int id;
    Adj *adjHead;
    DestNode *destHead;
    struct _Nodes *next;
} Nodes;

/*
An - instante em que o evento sera processado
dest_node - no a que se destina a mensagem constante no evento
origin_node - no que evia a mensagem constante no evento
type - relacao comercial entre o dest_node e o origin_node
message - mensagem na forma | nó que envia a mensagem | nó a que consegue chegar | custo |
dest_node_pointer - apontador para o no dest_node
next - apontador para o proximo evento
*/
typedef struct _Event{
    int An; //Tempo de chegada ao destino do evento, ou seja, tempo em que esse evento vai ser processado
    int dest_node; //Se dest_node = -1 então trata-se de uma primeira menssagem quando um nó ainda não sabe quem são sos seus vizinhos
    int origin_node;
    int type;
    int message[3];// Vetor na forma | nó1 - nó de inicio | nó2 | estimativa de custo de 1 para 2 |
    Nodes *dest_node_pointer;
    struct _Event *next;
} Event;

/*
node_id - id do no
type - relacao comercial com o no que o antecede
cost - estimativa do custo do caminho entre o no e o destino
dest - apontador para o no de destino
node - apontador para no
next - apontador para o proximo elemento na queue
*/
typedef struct _Queue{
    int node_id;
    int type;
    int cost;
    DestNode *dest;
    Nodes *node;
    struct _Queue *next;
} Queue;



/*Funcao que valida a linha de comandos*/
void commandLineValidation(int argc, char **argv);

/*Funcao que imprime num ficheiro a contagem dos tempos de cada simulacao*/
void write_times();

/*Funcao que executa o modo interativo*/
Nodes *InteractiveMode(Nodes *nodes_head, Nodes **interactive_orig_node);

/*Funcao que imprime num ficheiro a contagem dos custos e tipos de rotas*/
void writeStatistics();



#endif //READFILE INCLUDED