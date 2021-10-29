#include "simulation.h"

#include "calendar.h"
#include "nodes.h"

extern int Dn;
extern int *times_simulations;
extern int nr_nodes;

/**********************************     Simulation      *****************************************/
/* Vamos acordar cada nó da lista de nós, separadamente. Ou seja, primeiro acordamos um nó x e  */
/* preenchemos as tabelas de encaminhamento dos nós que conseguem chegar ao nó de destino x. E  */
/* Depois fazemos o mesmo para os restantes nós da lista de nós.                                */
/************************************************************************************************/
void simulations(Nodes *nodes_head, Event *event_head){
    
    Nodes *auxT = NULL; 
    int i = 1;

    times_simulations = (int*)calloc(nr_nodes, sizeof(int));
    if(times_simulations == NULL){
        printf("Error: Could not allocate memory for times_simulations");
    }
    
    if(nodes_head == NULL){
        return;
    }else{
        auxT = nodes_head;
        Dn = 0;
        //printf("\n ------------ Awaken node: %d -------------- \n", nodes_head->id);
        processCalendar(event_head, auxT, nodes_head);
        times_simulations[0] = Dn;

        while( auxT->next != NULL){
            Dn = 0;
            auxT = auxT->next;
            //printf("\n ------------ Awaken node: %d -------------- \n", auxT->id);
            processCalendar(event_head, auxT, nodes_head);
            times_simulations[i] = Dn;
            i++;
        }
    }
    return;
} 

