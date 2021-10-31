#ifndef CALENDAR_INCLUDED
#define CALENDAR_INCLUDED

#include "readFile.h"




/*Funcao que cria eventos do tipo announce*/
Event *announceNode(Event *event_head, Nodes *woken_node);

/*Funcao que cria eventos do tipo announce*/
Event *RepAnnouncement(Event *eventHead, Nodes *node_orig, DestNode *woken_node);

/*Funcao que cria um evento*/
Event *createEvent(Event *event_head, Nodes *node_orig, int woken_id, Adj *adj, int cost);

/*Funcao que insere o evento ordenadamente no calendario*/
Event *insertEventOrdered(Event *listHead, Event *newEvent);

/*Funcao que imprime os eventos do calendario*/
void printEvents(Event *listHead);

/*Funcao que processa o calendario*/
void processCalendar(Event *events_Head, Nodes *woken_node, Nodes *nodes_head);

/*Funcao que processa um dado evento*/
Event *processEvent(Event *event_head, Nodes *process_node , Nodes *nodes_head);

/*Funcao que retira um evento do topo do calendario*/
Event *popEvent(Event *event_head);

#endif //CALENDAR INCLUDED