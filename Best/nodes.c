#include "nodes.h"

#include "calendar.h"
#include "simulation.h"

extern int nr_nodes;
extern int *types;

/** createNode: Creates a new node in the Nodes List **/
Nodes *createGraph(Nodes *listHead, int tail, int head, int type){ 

    Nodes *newNode = NULL;
    Adj *newAdj = NULL;
    
    
    if (searchNodesList(listHead, tail) == NULL){ //Deve ser tail ou head ? Perguntar ao prof
        nr_nodes++;
        newNode = createNode(listHead, tail);
        listHead = insertNode(listHead, newNode, newNode->id);    
    }
    else{
        newNode = searchNodesList(listHead, tail);
    }

    newAdj = createAdj(newNode->adjHead, tail, head, type);
    newNode->adjHead = insertAdj(newNode->adjHead, newAdj);

    return listHead;
}

/** Creation of a new node (list of Nodes) **/
Nodes *createNode(Nodes *listHead, int tail){ 

    Nodes *newNode = NULL;
    
    if((newNode = (Nodes*) calloc(1, sizeof(Nodes))) == NULL){   //Creation of a New Node 
        printf("Memory is full. Couldn't register request.\n");
	    return listHead;
    } 
    
    newNode->id = tail;
    newNode->next = NULL;
    newNode->adjHead = NULL;
    newNode->destHead = NULL;

    //newNode->destHead = createDestiny(newNode->destHead, newNode->id, newNode->id, 0, 0);

    return newNode;
}

/** Creates a new adjacent nodes referent to the Node in the Nodes List **/
Adj *createAdj(Adj *listHead, int tail, int head, int type){ 

    Adj *newAdj;

    if((newAdj = (Adj*) calloc(1, sizeof(Adj))) == NULL){ //Create a new Adjacent
		return listHead;
    }

    newAdj->neighbor = tail;
    newAdj->id = head;
    newAdj->type = type;
    newAdj->An = 0;
    newAdj->next = NULL;

    return newAdj;
}

/** Insert the new Adj int the end of the respective Adj List **/
Adj *insertAdj(Adj *listHead, Adj *newAdj){ 

    Adj *auxH, *auxT;

    if(listHead == NULL){
        listHead = newAdj;
    }else{
        auxH = listHead;
        auxT = listHead->next;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->next;
        }
        auxH->next = newAdj;
        newAdj->next = NULL;
    }
    return listHead;
}

Nodes *insertNode(Nodes *listHead, Nodes *newNode, int id){ //Insert the new Node in the end of the Nodes List

    Nodes *auxT;

    /** Inserting the new node in the end of the nodes list and returning the list head**/
    if(listHead == NULL){
        listHead = newNode;
    }else{
        auxT = listHead;
        while(auxT->next != NULL){
            if( auxT->id == id){
                return listHead;
            }
            auxT=auxT->next;
        }
        auxT->next=newNode;
        newNode->next=NULL;
    }
    return listHead;
}

/*  *
* searchNodeList is a function that returns an 1 or a 0. The fuction searches in the Node List for the Node with a certain id.      *
* If there is a match the function retunrs 1, theis means that the Node is already in the Nodes List, so we don´t create the        * 
* same Nome two times. If there is no match, the fuction returns zero and we can create the new node and add him to the Nodes List. *
*   */
Nodes *searchNodesList(Nodes *list_Head, int id){ 

    Nodes *auxT;

    if(list_Head == NULL){
        return NULL;
    }else{     
        auxT = list_Head;
        if(auxT->id == id)
            return auxT;
        while(auxT->next !=NULL){
            auxT = auxT->next;
            if( auxT->id == id ){
                //printf("searchNodesList: auxT encontrado: %d\n", auxT->id); // DÚVIDA: A imprimir duas vezes, porquê ?
                return auxT;
            }
        }
    }
    return NULL;
}

void Print_List_of_Nodes(Nodes *listHead){
    
    Nodes *nodes_auxT;

    if(listHead==NULL){
        return;
    }else{
        for(nodes_auxT = listHead; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next) {
            printf("\n\t%d", nodes_auxT->id);   fflush(stdout);
        }
    }

    return;
}

void Print_List_of_Adjacencies(Nodes *listHead){
    
    Nodes *nodes_auxT;
    Adj *adj_auxT;

    if(listHead==NULL){
        return;
    }else{
        for(nodes_auxT = listHead; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next) {
            printf("\n[%d]->", nodes_auxT->id);   fflush(stdout);
            for(adj_auxT = nodes_auxT->adjHead; adj_auxT != NULL; adj_auxT = adj_auxT->next) {
                printf("[id:%d|type:%d|nodes_list_id:%d]->", adj_auxT->id, adj_auxT->type, adj_auxT->node_pointer->id);  fflush(stdout);
            }
            printf("NULL\n");
        }
    }

    return;
}


/*
updateDestToNode: Se a tabela de encaminhamento de um dado nó for alterada (retorna-se 1), então esse nó tem que anunciar isso
aos seus vizinhos, logo é necessário criar novos eventos. Caso a tabela de encaminhamento não altere (retorna-se 0), o nó não
anuncia nada.
*/
DestNode *updateDestToNode(Nodes *process_node, int *message, int type)
{
    DestNode *current_dest = NULL;
    Neighbours *chosen_neighbour = NULL;
    Neighbours *a_neighour = NULL;

    

    current_dest = searchDestiny(process_node->destHead, message[1]);

    if(current_dest == NULL){
        //criar o destino para o nó adjacente e inserir no topo
        process_node->destHead = createDestiny(process_node->destHead, message[0], message[1], message[2], type);

        //Criar o nó vizinho a partir do qual chegamos ao destino, para o inserir na lista de nós desse destino
        chosen_neighbour = createNeighbourToDestiny(message, type);
        //Inserir ordenadamente o novo vizinho na lista de vizinhos que conseguem chegar ao destino
        process_node->destHead->neighbours_head = insertNeighbourtOrdered(process_node->destHead->neighbours_head, chosen_neighbour);  

        return process_node->destHead;
    }else{
        //O destino já existe e temos de verificar se vale apena mudar caso a estimativa melhore
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ATENÇÃO ÀS RELAÇÕES COMERCIAIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        //encontrar o vizinho pelo qual conseguimos chegar a um dado destino
        a_neighour = searchForNeighbourToDestiny(current_dest->neighbours_head, message[0], -1);
        if(a_neighour == NULL){//se não o encontrámos temos que criar
            a_neighour = createNeighbourToDestiny(message, type);
            current_dest->neighbours_head = insertNeighbourtOrdered(current_dest->neighbours_head, a_neighour);//inserimos ordenadamente para que na cabeça da lista de vizinhos que chegam a um dado destino ficar sempre o que tem melhor custo
        }else{//se o encontrámos temos que o atualizar com o novo custo e depois reordenar a lista de vizinhos que chegam a um dado destino
            a_neighour->neighbour_estim_cost = message[2] + 1;
            //printf("NEIGHBOURS HEAD: %d", current_dest->neighbours_head->neighbour_id);
            current_dest->neighbours_head = orderNeighboursToDestinyAscendent(current_dest->neighbours_head);
            
        }

        if(process_node->id != message[1]) {
            if(updateEstimateToDestiny(current_dest) == 1){
                //printf("Alteraçãooooooooo\n");
                return current_dest;
            }
            
        } // apenas atualizamos o destino se o destino não formos nós próprios

        //print list of neighbours to that destiny
        //printf("\n\t\tPRINTING DESTINIES OF NODE %d\n", process_node->id);
        //printDestiny(process_node->destHead);

    } 

    return NULL;  //Se nada mudar, então não se anuncia nada
}

int updateEstimateToDestiny(DestNode *current_dest)
{
    Neighbours *aux = NULL;
    int old_id  = current_dest->chosen_neighbour_id;
    //encontrar o melhor vizinho numa lista ordenada por custos
    //devemos dr prioridade aos clientes, depois aos pares e depois aos fornecedores
    if((aux = searchForNeighbourToDestiny(current_dest->neighbours_head, -1, 1)) != NULL){//Encontrámos um vizinho que é nosso cliente
        //printf("ALTERACAO PARA UM CLIENTE\n");
        current_dest->chosen_neighbour_id = aux->neighbour_id;
        current_dest->cost = aux->neighbour_estim_cost;
        current_dest->type = aux->type;
        if(current_dest->chosen_neighbour_id != old_id)
            return 1;
        
        
    }else if((aux = searchForNeighbourToDestiny(current_dest->neighbours_head, -1, 2)) != NULL){
        //printf("ALTERACAO PARA UM PAR\n");
        current_dest->chosen_neighbour_id = aux->neighbour_id;
        current_dest->cost = aux->neighbour_estim_cost;
        current_dest->type = aux->type;
        if(current_dest->chosen_neighbour_id != old_id)
            return 1;
        
    }else if((aux = searchForNeighbourToDestiny(current_dest->neighbours_head, -1, 3)) != NULL){
        //printf("ALTERACAO PARA UM fornecedor\n");
        current_dest->chosen_neighbour_id = aux->neighbour_id;
        current_dest->cost = aux->neighbour_estim_cost;
        current_dest->type = aux->type;
        if(current_dest->chosen_neighbour_id != old_id)
            return 1;
        
    }
    return 0;
}

void printDestiny(DestNode *destiny_head)
{
    printf("\n\t\tPRINTING DESTINIES\n");
    DestNode *destiny_auxT;
    Neighbours *neighbour_auxT;

    if(destiny_head == NULL){
        return;
    }else{
        for(destiny_auxT = destiny_head; destiny_auxT != NULL; destiny_auxT = destiny_auxT->next_dest) {
            printf("\n[%d]->", destiny_auxT->dest_id);   fflush(stdout);
            for(neighbour_auxT = destiny_auxT->neighbours_head; neighbour_auxT != NULL; neighbour_auxT = neighbour_auxT->next_neighbour) {
                printf("[id:%d|type:%d|cost%d]->", neighbour_auxT->neighbour_id, neighbour_auxT->type, neighbour_auxT->neighbour_estim_cost);  fflush(stdout);
            }
            printf("NULL\n");
            printf("Choosen: %d", destiny_auxT->chosen_neighbour_id);
        }
    }
    return;
}

Neighbours *createNeighbourToDestiny(int *message, int type)
{
    Neighbours *aux = NULL;

    if((aux = (Neighbours*)calloc(1, sizeof(Neighbours))) == NULL){
        printf("Error: Could not create neighbour to destiny");
        return NULL;
    }
    aux->neighbour_id = message[0];
    aux->neighbour_estim_cost = message[2] + 1;
    aux->type = type;
    aux->next_neighbour = NULL;

    return aux;
}

Neighbours *insertNeighbourtOrdered(Neighbours *neighbours_head, Neighbours *neighbour_to_insert)
{
    Neighbours *auxH, *auxT;

    if(neighbours_head == NULL){
        return neighbour_to_insert;
    }else{
        if(neighbour_to_insert->neighbour_estim_cost < neighbours_head->neighbour_estim_cost){
            neighbour_to_insert->next_neighbour = neighbours_head;
            neighbours_head = neighbour_to_insert;
        }
        else{
            auxH = neighbours_head;
            auxT = neighbours_head->next_neighbour;
            while((auxT != NULL) && (neighbour_to_insert->neighbour_estim_cost >= auxT->neighbour_estim_cost)){
                auxH = auxT;
                auxT = auxT->next_neighbour;
            }
            neighbour_to_insert->next_neighbour = auxT;
            auxH->next_neighbour = neighbour_to_insert;
        }
    }
    //printf("\n ----------- 269 -------------- \n");
    
    //printf("\n ----------- 271 -------------- \n");
    return neighbours_head;
}

Neighbours *searchForNeighbourToDestiny(Neighbours *neighbours_head, int neighbour_id, int type)
{
    Neighbours *auxT;

    if(neighbours_head == NULL){
        return NULL;
    }else{     
        auxT = neighbours_head;
        if(type == -1){//Se type == -1 então estamos à procura de um vizinho pelo id
            if(auxT->neighbour_id == neighbour_id)
                return auxT;
            while(auxT->next_neighbour != NULL){
                auxT = auxT->next_neighbour;
                if( auxT->neighbour_id == neighbour_id ) return auxT;
            }
        }else{//Se type != -1 estamos à procura de um vizinho pelo tipo de ralçaõ comercial
            if(auxT->type == type)
                return auxT;
            while(auxT->next_neighbour != NULL){
                auxT = auxT->next_neighbour;
                if( auxT->type == type ) return auxT;
            }

        }
    }
    //Se não se tiver encontrado o destino 
    return NULL;
}

Neighbours *orderNeighboursToDestinyAscendent(Neighbours *neighbours_head)
{
    Neighbours *right = NULL, *left = NULL, *head, aux_struct;
    head = &aux_struct;
    head->next_neighbour = neighbours_head;

    bool trocas = true;

    //printf("\n%d\n", neighbours_head->neighbour_id); 
    //printf("\n%d\n", neighbours_head->neighbour_estim_cost); 
    //printf("\n%d\n", neighbours_head->type); 

    if((neighbours_head->next_neighbour != NULL) && (neighbours_head != NULL)){
        while(trocas){

            trocas = false;
            right = head->next_neighbour;
            left = head;

            while(right->next_neighbour != NULL){

                if(right->neighbour_estim_cost > right->next_neighbour->neighbour_estim_cost){
                    trocas = true;
                    left->next_neighbour = switch_neighbours(right, right->next_neighbour);

                }
                left = right;
                if(right->next_neighbour != NULL){
                    right = right->next_neighbour;
                }
            }

        }
    }
    right = head->next_neighbour;
    return right;
}

Neighbours *switch_neighbours(Neighbours *left, Neighbours *right)
{
    left->next_neighbour = right->next_neighbour;
    right->next_neighbour = left;
    return right;
}




DestNode *searchDestiny(DestNode *dest_head, int dest_id)
{
    DestNode *auxT;

    if(dest_head == NULL){
        return NULL;
    }else{     
        auxT = dest_head;
        if(auxT->dest_id == dest_id)
            return auxT;
        while(auxT->next_dest != NULL){
            auxT = auxT->next_dest;
            if( auxT->dest_id == dest_id ) return auxT;
        }
    }
    //Se não se tiver encontrado o destino 
    return NULL;
}


/*
chosen_neighbour_id - vizinho por onde conseguimos chegar a um dado destino
dest_id - nó de destino
type ralaçao do nó atual para o nó com o chosen_neighbour_id
*/
DestNode *createDestiny(DestNode *dest_head, int neigbour_id, int dest_id, int cost, int type)
{
    DestNode *new_dest = NULL;

    //primeiro destino
    //Se é o primeiro destino então criamos logo dois destinos:
    // - destino para o nó adjacente de onde recebemos a mensagem
    // - destino para o nó anunciado na mensagem
    if((new_dest = (DestNode*) calloc(1, sizeof(DestNode))) == NULL){   
        printf("Error: Could not add destiny");
        return NULL;
    }
    new_dest->chosen_neighbour_id = neigbour_id;
    new_dest->dest_id = dest_id;
    new_dest->cost = cost + 1;
    new_dest->type = type;
    new_dest->next_dest = NULL;

    if(dest_head == NULL){
        //se for o primeiro destino a ser criado
        dest_head = new_dest;
        dest_head->next_dest = NULL;
    }else{
        //Se já existirem destinos inserimos na cabeça da lista
        new_dest->next_dest = dest_head;
        dest_head = new_dest;
    }

    return dest_head;
}

void Print_List_of_Destinations(Nodes *nodes_Head, int mode)
{
    Nodes *nodes_auxT;
    DestNode *dest_auxT;
    FILE *fd;

    switch (mode)
    {
    case 3:
        if((fd = fopen("dest_simulation.txt", "w")) == NULL){
            printf("Error: Could not open file \n");
            exit(0);
        }
        break;
    
    case 5:
        if((fd = fopen("dest_algorithm.txt", "w")) == NULL){
                printf("Error: Could not open file \n");
                exit(0);
            }
        break;
    }
    

    if(nodes_Head==NULL){
        return;
    }else{
        for(nodes_auxT = nodes_Head; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next) {
            fprintf(fd, "\n[%d]->", nodes_auxT->id);   
            for(dest_auxT = nodes_auxT->destHead; dest_auxT != NULL; dest_auxT = dest_auxT->next_dest) {
                if(dest_auxT->chosen_neighbour_id != 1000000){
                    //fprintf(fd, "[dest_id:%d|neihbour_id:%d|type:%d|cost:%d]->", dest_auxT->dest_id,dest_auxT->chosen_neighbour_id, dest_auxT->type,dest_auxT->cost);
                    fprintf(fd, "[dest_id:%d|type:%d|cost:%d]->", dest_auxT->dest_id , dest_auxT->type,dest_auxT->cost);
                }
            }
            fprintf(fd, "NULL\n");
        }
    }
    fclose(fd);

    return;
}

void freeGraphNodes(Nodes *nodes_head)
{
    Nodes *auxH = NULL; 
    Adj *aux_adj = NULL;

    if(nodes_head == NULL)
    {
        return;
    }else{
        
        while (nodes_head != NULL)
        {
            auxH = nodes_head;
            nodes_head = nodes_head->next;

            while(auxH->adjHead != NULL)
            {
                aux_adj = auxH->adjHead;
                auxH->adjHead = auxH->adjHead->next; 
                free(aux_adj);
            }
            free(auxH);
        }
        

    }
}

void clearAllDest(Nodes *nodes_head){

    Nodes *auxT=NULL;

    if(nodes_head == NULL){
        return;
    } else{
        auxT= nodes_head;
        clearDestinys(auxT->destHead);
        while (auxT->next !=NULL){
            auxT=auxT->next;
            clearDestinys(auxT->destHead);
        }
    }
    return;
}

void clearDestinys(DestNode *list){

  if(list!=NULL){
    clearDestinys(list->next_dest);
    clearNeighbours(list->neighbours_head);
    free(list);
  }

  return;
}

void clearNeighbours(Neighbours *list){

  if(list!=NULL){
    clearNeighbours(list->next_neighbour);
    free(list);
  }

  return;
}

Nodes *AdjToNode(Nodes *listHead){
    
    Nodes *nodes_auxT, *auxT;
    Adj *adj_auxT;
    int nr_nodes = 0;


    if(listHead==NULL){
        return listHead;
    }else{
        for(nodes_auxT = listHead; nodes_auxT != NULL; nodes_auxT = nodes_auxT->next) {
            //printf("\nNODE_id: %d\n", nodes_auxT->id); 
            nr_nodes++;
            for(adj_auxT = nodes_auxT->adjHead; adj_auxT != NULL; adj_auxT = adj_auxT->next) {
                if(listHead == NULL){
                    return NULL;
                }else{     
                    auxT = listHead;
                    if(auxT->id == adj_auxT->id)
                       adj_auxT->node_pointer=auxT;
                    while(auxT->next !=NULL){
                        auxT = auxT->next;
                        if(auxT->id == adj_auxT->id)
                            adj_auxT->node_pointer=auxT;
                    }
                }
            }
        }
    }
    //printf("\nNR NODES: %d", nr_nodes); fflush(stdout);
    return listHead;
}

DestNode *createDestinyAlgorithm(DestNode *dest_head, int node_id, int dest_id)
{
    DestNode *new_dest = NULL;

    if((new_dest = (DestNode*) calloc(1, sizeof(DestNode))) == NULL){   
        printf("Error: Could not add destiny");
        return NULL;
        }

    if(node_id == dest_id){
        new_dest->chosen_neighbour_id = dest_id;
        new_dest->dest_id = dest_id;
        new_dest->cost = 0;
        new_dest->type = 0;
        new_dest->next_dest = NULL;
    }else{
        new_dest->chosen_neighbour_id = INFINITE;
        new_dest->dest_id = dest_id;
        new_dest->cost = INFINITE;
        new_dest->type = INFINITE;
        new_dest->next_dest = NULL;
    }
    
    if(dest_head == NULL){
        dest_head = new_dest;
        dest_head->next_dest = NULL;
    }else{
        new_dest->next_dest = dest_head;
        dest_head = new_dest;
    }
    return dest_head;
}

DestNode *initDestinyDijkstra(DestNode *dest_head, int node_id, int dest_id)
{

    if(node_id == dest_id){
        dest_head->chosen_neighbour_id = dest_id;
        dest_head->dest_id = dest_id;
        dest_head->cost = 0;
        dest_head->type = 0;
        dest_head->next_dest = NULL;
    }else{
        dest_head->chosen_neighbour_id = INFINITE;
        dest_head->dest_id = dest_id;
        dest_head->cost = INFINITE;
        dest_head->type = INFINITE;
        dest_head->next_dest = NULL;
        types[0] = types[0] + 1;
    }
    
    return dest_head;
}