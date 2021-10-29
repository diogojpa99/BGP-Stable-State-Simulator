
#include "calendar.h"


#include "nodes.h"
#include "simulation.h"


extern int Dn;

//AnnounceNode: Para cada ligação (adjacente), vai se criar um novo evento.
Event *announceNode(Event *event_head, Nodes *woken_node){

    Adj *neighbour;
    int wake_message[3] = {woken_node->id, woken_node->id, -1};

    /*  Antes de se anunciar aos seus vizinhos, o nó que acabou de acordar coloca na sua tabela de 
        encaminhamento que consegue chegar a ele próprio com "custo" zero. */
    woken_node->destHead = updateDestToNode(woken_node, wake_message, 0);
    //Print_List_of_Destinations(woken_node, 3);
    if(woken_node == NULL){
        return event_head;
    }else{    
        //Dn++; // Escolher o primeiro nó para acordar -> Dn=0 ; Ao anunciar o primeiro nó que acordou -> Dn=1
        neighbour = woken_node->adjHead;
            event_head = createEvent(event_head, woken_node, woken_node->id, neighbour, 0);
        while(neighbour->next != NULL){
            neighbour = neighbour->next;
            event_head = createEvent(event_head, woken_node, woken_node->id, neighbour, 0);
        }
    }
    return event_head;
}

//RepAnnouncement: Para cada ligação (adjacente), vai se criar um novo evento.
// Mas atenção às leis comerciais !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Basicamente se houver cliente enviamos, senão houver clientes barulho não se envia nada
/* 
eventHead -> cabeça do calendario
node_orig -> nó que vai enviar a mensagem aos seus adjacentes
source_node -> Nó que foi eleito para chegar a um dado destino
*/

/************************************************************
Event *RepAnnouncement(Event *eventHead, Nodes *node_orig, DestNode *source_node)

    -Descricao: Recria o anúncio recebido por um nó para todos os seus adjacentes tendo em conta as relações comerciais

    -Argumentos:*event_head - cabeça da lista do calendário
                *node_orig - nó que recebeu a mensagem e que a vai recriar e enviar para os seus adjacentes
                *source_node - nó que criou o anúncio original

    -Retorno:   cabeça da lista de calendário

************************************************************/

Event *RepAnnouncement(Event *event_head, Nodes *node_orig, DestNode *source_node){

    Adj *neighbour;

    if(node_orig == NULL){
        return event_head;
    }else{
        
        
        neighbour = node_orig->adjHead;
            if( (source_node->type == 1 || neighbour->type == 1) && neighbour->id != node_orig->id){
                //printf("\n-- 45 -- : neihbour_id = %d\n", neighbour->id);
                event_head = createEvent(event_head, node_orig, source_node->dest_id, neighbour, source_node->cost);
            }
        while(neighbour->next != NULL){
            neighbour = neighbour->next;
            if((source_node->type == 1 || neighbour->type == 1) && neighbour->id != node_orig->id){
                event_head = createEvent(event_head, node_orig, source_node->dest_id, neighbour, source_node->cost);
                //printf("-- 46 -- : neihbour_id = %d\n", neighbour->id);
            }
        }
    }
    return event_head;
}

//Funcao que cria um novo evento para ser inserido no calendario
/*Parametros:
list_head - cabeça da lista de eventos*/

Event *createEvent(Event *event_head, Nodes *node_orig, int woken_node_id, Adj *adj, int cost) 
{
    Event *new_event = NULL;
    time_t t;
    srand((unsigned) time(&t));
    
    int Sn = 1 + rand()%3;
    //printf("\n\nSN = %d", Sn);
   
    if((new_event = (Event*) calloc(1, sizeof(Event))) == NULL){   /** Creation of a New Event **/
        //printf("Memory is full. Couldn't register request.\n");
		return event_head;
    }
    
    new_event->An = Dn + Sn;//o instante do evento quee estamos a criar é o instante do calendário mais a tempo aleatório com a componente fixa de 1
    if (new_event->An < adj->An) new_event->An = adj->An;  //Tratar da fila de espera de cada ligação    
    new_event->origin_node = node_orig->id; // nó que está a enviar a sms
    new_event->dest_node = adj->id; // nó a quem se destina a sms
    new_event->dest_node_pointer=adj->node_pointer;

    //relaçao entre os dois nós, da prespetiva do nó adjacente que é o que vai ser processado 
    if (adj->type == 1){
        new_event->type = 3; 
    } 
    else if(adj->type == 3){
        new_event->type = 1;
    }
    else if(adj->type == 2){
        new_event->type = 2;
    }
    new_event->next = NULL;

    //Message sent from an node x to his neighbor
    new_event->message[0] = node_orig->id;
    new_event->message[1] = woken_node_id;
    new_event->message[2] = cost; 

    //printf("\n-- 96 --: New Event: time=%d | out_node=%d | in_node=%d | type=%d | message: %d %d %d\n",new_event->An,new_event->origin_node,new_event->dest_node,new_event->type, new_event->message[0], new_event->message[1], new_event->message[2]);

    return event_head = insertEventOrdered(event_head, new_event);
}


Event *insertEventOrdered(Event *list_head, Event *new_event){

    Event *auxH, *auxT;

    if(list_head == NULL){
        return new_event;
    }else{
        if(new_event->An < list_head->An){
            new_event->next=list_head;
            list_head = new_event;
        }
        else{
            auxH = list_head;
            auxT = list_head->next;
            while((auxT != NULL) && (new_event->An >= auxT->An)){
                auxH = auxT;
                auxT = auxT->next;
            }
            new_event->next = auxT;
            auxH->next = new_event;
        }
    }
    //printf("\n ----------- 135 -------------- \n");
    //printEvents(list_head);
    //printf("\n ----------- 135 -------------- \n");
    return list_head;
}

void printEvents(Event *listHead){
    
    Event *auxH, *auxT;
    
    if(listHead == NULL){
        printf("No events\n");
        return;
    }else{
        auxH = listHead;
        printf("\nEvent: [time %d | %d -> %d | message: %d %d %d ]\n",auxH->An, auxH->origin_node, auxH->dest_node, auxH->message[0], auxH->message[1], auxH->message[2]);
        //printf("\t[time=%d|%d->%d|Type:%d]->",auxH->An,auxH->origin_node,auxH->dest_node,auxH->type); fflush(stdout);//prompt
        auxT = listHead->next;
        while( auxT != NULL){
            auxH=auxT;
            auxT=auxT->next;
            printf("\nEvent: [time %d | %d -> %d | message: %d %d %d ]\n",auxH->An, auxH->origin_node, auxH->dest_node, auxH->message[0], auxH->message[1], auxH->message[2]);
            //printf("\t[time=%d|%d->%d|Type:%d]->",auxH->An,auxH->origin_node,auxH->dest_node,auxH->type); fflush(stdout);//prompt
        }
        //printf("NULL\n");
    }

    return;
}

void processCalendar(Event *event_head, Nodes *woken_node, Nodes *nodes_head)
{
    event_head = announceNode(event_head, woken_node); //First wake up the node, create the respective events and insert them in the calendar
    
    while(event_head != NULL){
        //printf("\n ----------- 171 -------------- \n");
        //printEvents(event_head);
        //printf("\n ----------- 171 -------------- \n");
        event_head = processEvent(event_head, event_head->dest_node_pointer, nodes_head);
        event_head = popEvent(event_head);        
    }

    return;
}

/*
process_node_id - nó de destino do evento

*/
Event *processEvent(Event *event_head, Nodes *process_node , Nodes *nodes_head)
{
   
    Nodes *orig_node = NULL;
    DestNode *source_node = NULL;

    //Primeiro, encontrar o nó que queremos processar
    orig_node = process_node;
    //printf("\nNode that is being currently processed: %d\n", orig_node->id);
    Dn = event_head->An; //O instante do calendário é o instante do evento que estamos a processar
    //printf("\n\nTIME - %d\n\n", Dn);

    //Segundo, processar o evento -> Atualizar a tabela de encaminhamento
    source_node = updateDestToNode(orig_node, event_head->message, event_head->type);
    if (source_node != NULL){
        //printf("Tabela de encaminhamento do nó %d: [Nó de destino:%d| Nó vizinho: %d | Type: %d | Custo: %d]\n", orig_node->id,source_node->dest_id, source_node->chosen_neighbour_id, source_node->type, source_node->cost); 
        event_head = RepAnnouncement(event_head, orig_node, source_node);
    }
    else{
        //printf("Nó não atualizou a sua tabela de encaminhamento -> Logo não deve anunciar nada, logo não criamos eventos\n");
    }

    return event_head;
}


Event *popEvent(Event *event_head)
{
    Event *auxH = NULL;
    
    auxH = event_head;
    event_head = event_head->next;
    free(auxH);

    return event_head;
}

    
