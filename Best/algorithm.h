#ifndef ALGORITHM_INCLUDED
#define ALGORITHM_INCLUDED

#include "readFile.h"

/*Funcao que executa o algoritmo de Bellman-Ford para todos os destinos*/
void Algorithm(Nodes *nodes_head);

/*Funcao que executa o algoritmo de Bellman-Ford para um dado destino*/
void ReverseDijkstra(Nodes *nodes_head, Nodes *destiny_node, int mode);

/*Funcao que cria uma estrutura do tipo destino em todos os nos*/
Nodes *initAlgorithm(Nodes *list_head, Nodes *destiny_node);

/*Funcao que faz as inicializacoes necessarias*/
Nodes *initReverseDijkstra(Nodes *list_head, Nodes *destiny_node);



/*Funcao que cria um novo elemento para uma das queues*/
Queue *CreateNewElement(Queue *new_element, Nodes *node);

/*Funcao que executa a relaxacao de uma aresta mediante restricoes comerciais e de custos*/
Queue *Relaxation(Queue *Q, Queue **Q1, Queue **Q2, Queue **Q3,  Queue **last_Q1, Queue **last_Q2, Queue **last_Q3);

/*Funcao que executa a relaxacao de uma aresta*/
Queue *RelaxOfLink(Queue *Q, Nodes *adj_node, int adj_node_type, Queue **Q1, Queue **Q2, Queue **Q3, Queue **last_Q1, Queue **last_Q2, Queue **last_Q3);

/*Funcao que insere um dado elemento de queue numa queue conforme a sua relacao comercial*/
void ChooseQ(Queue **Q1, Queue **Q2, Queue **Q3, Nodes *node, int type, Queue **last_Q1, Queue **last_Q2, Queue **last_Q3, int flag_type);

/*Funcao que imprime a queue*/
void PrintQ(Queue *Q);

/*Funcao que imprime num ficheiro a lista de destinos*/
FILE *Print_Destinations(Nodes *nodes_Head, FILE *fd);

/*Funcao que umprime num ficheiro as estatisticas*/
FILE *Print_CCDF(Nodes *nodes_Head, FILE *fd);

/*Funcao que insere no fim da queue um dado elemento de queue*/
Queue *InsertInLast(Nodes *node, Queue *Q, Queue **last);

#endif //READFILE INCLUDED