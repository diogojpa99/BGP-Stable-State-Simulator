#include "readFile.h"
#include "nodes.h"
#include "calendar.h"
#include "algorithm.h"

extern int *costs;
extern int *types;

void Algorithm(Nodes *nodes_head)
{

    Nodes *auxT = NULL;
    //FILE *fd;
    int i=1;

    costs[0] = 0;
    
    /*
    if ((fd = fopen("dest_algorithmNOVO.txt", "w")) == NULL)
    {
        printf("Error: Could not open file \n");
        exit(0);
    }*/

    if (nodes_head == NULL)
    {
        return;
    }
    else
    {
        auxT = nodes_head;
        //printf("\n ------------ Awaken node: %d -------------- \n", nodes_head->id);
        //printf("\n ------------ %d -------------- \n", i);
        initAlgorithm(nodes_head, auxT);
        ReverseDijkstra(nodes_head, auxT);
        //UpdateTypesCosts(nodes_head);
        //Print_Destinations(nodes_head,fd);
        while (auxT->next != NULL)
        {
            auxT = auxT->next;
            //printf("\n ------------ Awaken node: %d -------------- \n", auxT->id);
            //printf("\n ------------ %d -------------- \n", i);
            ReverseDijkstra(nodes_head, auxT);
            //UpdateTypesCosts(nodes_head);
            //Print_Destinations(nodes_head,fd);
            i++;
        }
    }

    //fclose(fd);
    return;
}

void ReverseDijkstra(Nodes *nodes_head, Nodes *destiny_node)
{

    Queue *Q = NULL, *Q_1 = NULL, *Q_2 = NULL, *Q_3 = NULL; //Vão ser heaps ou listas normais ??
    /* Q_1, Q_2, Q_3 representam as cabeças de cada uma das filas ordenadas */
    Queue *last_Q1=NULL, *last_Q2=NULL, *last_Q3=NULL;

    //Iniciar o Reverse Dijkstra
    nodes_head = initReverseDijkstra(nodes_head, destiny_node);

    //Colocar o destino na pilha com custo zero
    Q_1 = InsertInLast(destiny_node, Q_1,&last_Q1);

    while ((Q_1 != NULL) || (Q_2 != NULL) || (Q_3 != NULL))
    {

        if (Q_1 != NULL)
        {
            Q = Q_1;
            Q_1 = Q_1->next;
            //retirar o nó que está na cabeça da pilhaS
            //Ver os meus vizinhos (Mas atenção às restrições comerciais)
            //printf("Node that's being removed from the Q_1: Node_id:%d | Type:%d | Cost:%d \n",Q->node_id,Q->type,Q->cost);
            //Enquanto a lista de adjacências não estiver vazia
            //Relaxation of the link uv
            //Ver os meus vizinhos (Mas atenção às restrições comerciais)
            //Vamos processar esse nó criar um elemento e inserir na respetiva pilha
            Q = Relaxation(Q, &Q_1, &Q_2, &Q_3, &last_Q1, &last_Q2,&last_Q3);
            //Depois libertamos a cabeça da pilha atual e metemos a nova cabeça
            free(Q);
            /*
            printf("Processar Q_1\n");
            PrintQ(Q_1);
            PrintQ(Q_2);
            PrintQ(Q_3);
            */
        }

        if ((Q_2 != NULL) && (Q_1 == NULL))
        {
            Q = Q_2;
            Q_2 = Q_2->next;
            //printf("Node that's being removed from the Q_2: Node_id:%d | Type:%d | Cost:%d \n",Q->node_id,Q->type,Q->cost);
            Q = Relaxation(Q, &Q_1, &Q_2, &Q_3, &last_Q1, &last_Q2, &last_Q3);
            free(Q);
            /*
            printf("Processar Q_2\n");
            PrintQ(Q_1);
            PrintQ(Q_2);
            PrintQ(Q_3);
            */
        }

        if ((Q_3 != NULL) && (Q_1 == NULL) && (Q_2 == NULL))
        {
            Q = Q_3;
            Q_3 = Q_3->next;
            //printf("Node that's being removed from the Q_3: Node_id:%d | Type:%d | Cost:%d \n",Q->node_id,Q->type,Q->cost);
            Q = Relaxation(Q, &Q_1, &Q_2, &Q_3, &last_Q1, &last_Q2, &last_Q3);
            free(Q);
            /*
            printf("Processar Q_3\n");
            PrintQ(Q_1);
            PrintQ(Q_2);
            PrintQ(Q_3);
            */
        }
    }

    return;
}

Nodes *initAlgorithm(Nodes *list_head, Nodes *destiny_node)
{

    Nodes *auxT;

    if (list_head == NULL)
    {
        return NULL;
    }
    else
    {
        auxT = list_head;
        auxT->destHead = createDestinyAlgorithm(auxT->destHead, auxT->id, destiny_node->id);
        while (auxT->next != NULL)
        {
            auxT = auxT->next;
            auxT->destHead = createDestinyAlgorithm(auxT->destHead, auxT->id, destiny_node->id);
        }
    }

    return list_head;
}

Nodes *initReverseDijkstra(Nodes *list_head, Nodes *destiny_node)
{

    Nodes *auxT;

    if (list_head == NULL)
    {
        return NULL;
    }
    else
    {
        auxT = list_head;
        auxT->destHead = initDestinyDijkstra(auxT->destHead, auxT->id, destiny_node->id);
        while (auxT->next != NULL)
        {
            auxT = auxT->next;
            auxT->destHead = initDestinyDijkstra(auxT->destHead, auxT->id, destiny_node->id);
        }
    }

    return list_head;
}

Queue *InsertQ3(Nodes *node, Queue *Q, Queue **last) //Inserir ordenadamente;
{

    Queue *new_element = NULL, *auxH = NULL, *auxT = NULL;

    new_element = CreateNewElement(new_element, node);

    if (Q == NULL)
    {
        Q = new_element;
        *last=new_element;
    }
    else
    {
        auxH = Q;
        auxT = auxH->next;
        if (new_element->cost <= Q->cost) //Menor ou igual!!
        {
            new_element->next = Q;
            Q = new_element;
        }else{
            while ((auxT != NULL) && (new_element->cost > auxT->cost))
            {
                auxH = auxT;
                auxT = auxT->next;
            }
            new_element->next = auxT;
            auxH->next = new_element;
            if(new_element->next == NULL) *last=new_element;
        }
    }
    return Q;
}

Queue *CreateNewElement(Queue *new_element, Nodes *node)
{

    if ((new_element = (Queue *)calloc(1, sizeof(Queue))) == NULL){ //Creation of a New Node
        printf("Memory is full. Couldn't register request.\n");
        return NULL;
    }

    new_element->cost = node->destHead->cost;
    new_element->node_id = node->id;
    new_element->type = node->destHead->type;
    new_element->node = node;
    new_element->dest = node->destHead;
    new_element->next = NULL;

    return new_element;
}

/*
Queue *RemoveNodeFromQ(Queue *Q){

    Queue *auxH=NULL;

    auxH=Q;
    Q=Q->next;

    free(auxH);

    return Q;

}*/

//Relaxação do link uv
Queue *Relaxation(Queue *Q, Queue **Q1, Queue **Q2, Queue **Q3,  Queue **last_Q1, Queue **last_Q2, Queue **last_Q3)
{

    Adj *neighbour = NULL;

    /*
        Nós só vamos tirar o elemento da pilha se e só se o que está na pilha for o que está no destiny do nó
    */

    if (Q == NULL)
    {
        return Q;
    }
    else
    {
        if ((Q->node->destHead->type != Q->type) || (Q->node->destHead->cost != Q->cost))
        { // Deve ser || ou && ?
            //printf(" This is not going to be processed: Node_id:%d | Type:%d | Cost:%d, because is not the most recent: Node_id:%d | Type:%d | Cost:%d\n", Q->node->id, Q->node->destHead->type, Q->node->destHead->cost,Q->node_id, Q->type,Q->cost);
            return Q;
        }
        else
        {
            
            //printf(" This Node is going to be processed: Node_id:%d | Type:%d | Cost:%d\n", Q->node_id, Q->type,Q->cost);
            neighbour = Q->node->adjHead;
            if ((Q->type <= 1 || neighbour->type <= 1) && (neighbour->id != Q->node->destHead->chosen_neighbour_id))
            { //Acho que faz sentido, mas confirmar
                //printf(" This Node is going to be relax: Node_id:%d\n", neighbour->id);
                Q = RelaxOfLink(Q, neighbour->node_pointer, neighbour->type, Q1, Q2, Q3, last_Q1, last_Q2, last_Q3);
            }
            while (neighbour->next != NULL)
            {
                neighbour = neighbour->next;
                if ((Q->type <= 1 || neighbour->type <= 1) && (neighbour->id != Q->node->destHead->chosen_neighbour_id))
                {
                    //printf(" This Node is going to be relax: Node_id:%d\n", neighbour->id);
                    Q = RelaxOfLink(Q, neighbour->node_pointer, neighbour->type, Q1, Q2, Q3, last_Q1, last_Q2, last_Q3);
                }
            }
        }
    }
    return Q;
}

Queue *RelaxOfLink(Queue *Q, Nodes *adj_node, int adj_node_type, Queue **Q1, Queue **Q2, Queue **Q3, Queue **last_Q1, Queue **last_Q2, Queue **last_Q3)
{

    if (adj_node_type == 1)
    {
        adj_node_type = 3;
    }
    else if (adj_node_type == 3)
    {
        adj_node_type = 1;
    }

    //printf("TIPO Pilha de onde se vem:%d",Q->type);

    if (adj_node_type < adj_node->destHead->type)
    {   
        //Atualização dos custos
        if(adj_node->destHead->type == INFINITE){
            types[0] = types[0] - 1;
        }else{
            types[adj_node->destHead->type] = types[adj_node->destHead->type] - 1;
            costs[adj_node->destHead->cost] = costs[adj_node->destHead->cost] - 1;
        }

        types[adj_node_type] = types[adj_node_type] + 1;
        costs[Q->cost+1] = costs[Q->cost+1] + 1;


        //printf("Link was relax: Source: %d || Types: %d < %d\n",adj_node->id,adj_node_type, adj_node->destHead->type);
        adj_node->destHead->type = adj_node_type;
        adj_node->destHead->cost = Q->cost + 1;
        adj_node->destHead->chosen_neighbour_id = Q->node_id;
        ChooseQ(Q1, Q2, Q3, adj_node, adj_node->destHead->type, last_Q1, last_Q2, last_Q3, Q->type);
    }
    else if (adj_node_type == adj_node->destHead->type)
    {
        if ((Q->cost + 1) < adj_node->destHead->cost)
        {        
            //Atualização dos custos
            if(adj_node->destHead->type == INFINITE){
                types[0] = types[0] - 1;
            }else{
                types[adj_node->destHead->type] = types[adj_node->destHead->type] - 1;
                costs[adj_node->destHead->cost] = costs[adj_node->destHead->cost] - 1;
            }

            types[adj_node_type] = types[adj_node_type] + 1;
            costs[Q->cost+1] = costs[Q->cost+1] + 1;
            
            //printf("Link was relax: Source: %d || Types: %d = %d || Types: %d < %d\n",adj_node->id,adj_node_type, adj_node->destHead->type, (Q->cost+1),adj_node->destHead->cost);
            adj_node->destHead->type = adj_node_type;
            adj_node->destHead->cost = Q->cost + 1;
            adj_node->destHead->chosen_neighbour_id = Q->node_id;
            ChooseQ(Q1, Q2, Q3, adj_node, adj_node->destHead->type, last_Q1, last_Q2, last_Q3, Q->type);
        }
    }

    return Q;
}

void ChooseQ(Queue **Q1, Queue **Q2, Queue **Q3, Nodes *node, int type, Queue **last_Q1, Queue **last_Q2, Queue **last_Q3, int flag_type)
{

    //printf("Node %d is going to be insert in Queue %d\n",node->id,type);
    if (type == 1)
    {
        *Q1 = InsertInLast(node, *Q1, last_Q1);
    }
    else if (type == 2)
    {   
        *Q2 = InsertInLast(node, *Q2, last_Q2);
    }
    else if (type == 3)
    {
        /*if (flag_type <= 2){
            //printf("--1--\n");
            *Q3 = InsertQ3(node, *Q3, last_Q3);
        } else{*/
            //printf("--2--\n");
            *Q3 = InsertInLast(node, *Q3, last_Q3);
        //}
    }

    return;
}

Queue *InsertInLast(Nodes *node, Queue *Q, Queue **last){
    
    Queue *old_last=NULL, *new_element = NULL;

    new_element = CreateNewElement(new_element, node);

    if( Q == NULL){
        Q=new_element;
        *last=new_element;
    }else{
        old_last=*last;
        old_last->next=new_element;
        *last=new_element;
    }

    return Q;
}

void PrintQ(Queue *Q)
{

    Queue *auxT;

    if (Q == NULL)
    {
        return;
    }
    else
    {
        printf("-------- Q_%d ---------\n", Q->type);
        auxT = Q;
        printf("Q_%d: Node:%d | Cost:%d\n", Q->type, Q->node_id, Q->cost);
        auxT = auxT->next;
        while (auxT != NULL)
        {
            printf("Q_%d: Node:%d | Cost:%d\n", auxT->type, auxT->node_id, auxT->cost);
            auxT = auxT->next;
        }
        printf("-------- Q_%d ---------\n", Q->type);
    }

    return;
}

FILE *Print_Destinations(Nodes *nodes_Head, FILE *fd)
{
    Nodes *nodes_auxT;

    if (nodes_Head == NULL)
    {
        return fd;
    }
    else
    {
        fprintf(fd, "\n[Destiny_id: %d]->", nodes_Head->destHead->dest_id);
        for (nodes_auxT = nodes_Head; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next)
        {
            //if(nodes_auxT->destHead->chosen_neighbour_id != INFINITE){
            //fprintf(fd, "[dest_id:%d|neihbour_id:%d|type:%d|cost:%d]->", dest_auxT->dest_id,dest_auxT->chosen_neighbour_id, dest_auxT->type,dest_auxT->cost);
            fprintf(fd, "[Orig_id:%d|type:%d|cost:%d]->", nodes_auxT->id, nodes_auxT->destHead->type, nodes_auxT->destHead->cost);
            //}
        }
        fprintf(fd, "NULL\n");
    }

    return fd;
}

// Não vamos usar
FILE *Print_CCDF(Nodes *nodes_Head, FILE *fd)
{
    Nodes *nodes_auxT;

    if (nodes_Head == NULL)
    {
        return fd;
    }
    else
    {
        for (nodes_auxT = nodes_Head; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next)
        {
            fprintf(fd, "%d %d\n", nodes_auxT->destHead->type, nodes_auxT->destHead->cost);
        }
    }

    return fd;
}

void UpdateTypesCosts(Nodes *nodes_head)
{

    Nodes *nodes_auxT;

    if (nodes_head == NULL){
        return;
    }
    else
    {
        for (nodes_auxT = nodes_head; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next)
        {
            if (nodes_auxT->destHead->type == INFINITE){
                types[0] = types[0] + 1;
            } else {
                if (nodes_auxT->destHead->type != 0)
                    types[nodes_auxT->destHead->type] = types[nodes_auxT->destHead->type] + 1;
                if (nodes_auxT->destHead->cost != 0)
                    costs[nodes_auxT->destHead->cost] = costs[nodes_auxT->destHead->cost] + 1;
            }
        }
    }
    return;
}
