#include "simulation.h"

#include "calendar.h"
#include "nodes.h"

extern int Dn;
extern int *times_simulations;
extern int nr_nodes;

extern int *costs;
extern int *types;

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

    UpdateTypesCosts(nodes_head);
    return;
} 

void UpdateTypesCosts(Nodes *nodes_head)
{

    Nodes *nodes_auxT;
    DestNode *dest_auxT;

    int n_dests=0;

    if (nodes_head == NULL){
        return;
    }
    else
    {
        for (nodes_auxT = nodes_head; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next)
        {
            n_dests=0;
            for (dest_auxT = nodes_auxT->destHead; dest_auxT != NULL; dest_auxT = dest_auxT->next_dest)
            {
                if (dest_auxT != NULL){
                    if (dest_auxT->type != 0)
                        types[dest_auxT->type] = types[dest_auxT->type] + 1;
                    if (dest_auxT->cost != 0)
                        costs[dest_auxT->cost] = costs[dest_auxT->cost] + 1;
                }
                n_dests++;
            }
            if((nr_nodes-n_dests)>0) types[0]=types[0] + (nr_nodes-n_dests);
        }
    }
    return;
}

