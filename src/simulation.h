#ifndef SIMULATION_INCLUDED
#define SIMULATION_INCLUDED


#include "readFile.h"

/*Funcao que executa a simulacao para cada no da lista de adjacencias*/
void simulations(Nodes *nodes_head, Event *event_head);

/*Funcao que atualiza os tipos e custos das rotas entre os vertices no fim de cada simulacao*/
void UpdateTypesCosts(Nodes *nodes_head);

/*Funcao que reseta a contagem da ultima mensagem a ser processada em todas as filas de espera entre os vertices*/
void CleanAns(Nodes *listHead);

#endif //SIMULATION INCLUDED