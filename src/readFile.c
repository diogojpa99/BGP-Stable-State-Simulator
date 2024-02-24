/*******************************************************************************************
 _____
|     |             
| IST | TECNICO     Autores: Diogo Araújo (ist193906) e Miguel Ribeiro (ist193148)
 \   /  LISBOA      
  \ /              
*******************************************************************************************/
#include "readFile.h"

#include "calendar.h"
#include "nodes.h"
#include "simulation.h"
#include "algorithm.h"

int Dn = 0;
int *times_simulations = NULL;
int nr_nodes = 0;
int *costs = NULL;
int *types = NULL;


int main(int argc, char **argv)
{
    enum modes_{help, none, interactive_sim, simulation, interactive_algo, algorithm };
    enum modes_ mode = none;

    FILE *fp;

    int tail, head, type, opt;
    char buffer[128];

    Nodes *nodes_head = NULL, *interactive_orig_node=NULL, *interactive_dest_node=NULL;
    DestNode *interactive_dest=NULL;
    Event *event_head = NULL;
    
    char file[128];

    costs = (int*)calloc(1024, sizeof(int));
    types = (int*)calloc(4, sizeof(int));



    //validacao da linha de comandos
    while((opt = getopt(argc, argv, "m:i:")) != -1){
        switch (opt)
        {
        case 'm':
            strcpy(buffer, optarg);
            if(strcmp(buffer, "interactive_sim") == 0){
                commandLineValidation(argc, argv);
                mode = interactive_sim;

            }else if(strcmp(buffer, "interactive_algo") == 0){
                commandLineValidation(argc, argv);
                mode = interactive_algo;

            }else if(strcmp(buffer, "help") == 0){
                commandLineValidation(argc, argv);

            }else if(strcmp(buffer, "simulation") == 0){
                mode = simulation;

            }else if(strcmp(buffer, "algorithm") == 0){
                mode = algorithm;

            }else{
                printf("Error: Bad arguments in program call\n");
                printf("Type: ./graph -m help\n");
                exit(0);
            }
            break;
        case 'i':
            strcpy(file, argv[4]);
        }
    }
    
    fp = fopen(file,"r");

    srand(time(0));

    if(fp == NULL){
        perror("Error opening the text file");
        return(-1);
    }

    while( fscanf(fp, "%d %d %d\n", &tail, &head, &type) != EOF ){
        nodes_head = createGraph(nodes_head, tail, head, type);
    }

    //Por os Adjacentes a apontar para a posição deles na lista de nós
    nodes_head = AdjToNode(nodes_head);


    switch (mode)
    {
        case interactive_sim:
            //tem que dar como output o tipo e o comprimento da rota entre um destino e uma source dados na funcao commandLineValidation
            interactive_dest_node = InteractiveMode(nodes_head, &interactive_orig_node);
            processCalendar(event_head, interactive_dest_node, nodes_head);
            interactive_dest = searchDestiny(interactive_dest_node->destHead, interactive_dest_node->id);

            if(interactive_orig_node->destHead == NULL){
                printf("The source node %d can't reach that destiny \n", interactive_orig_node->id);
            } else{
                printf("Node %d can reach node %d with: TYPE:%d  &  COST:%d\n", interactive_orig_node->id, interactive_dest->dest_id, interactive_orig_node->destHead->type, interactive_orig_node->destHead->cost );
            }
            
            break;

        case simulation:
            simulations(nodes_head, event_head);
            write_times();
            writeStatistics();
            Print_List_of_Destinations(nodes_head, simulation);
            break;

        case interactive_algo:
            interactive_dest_node = InteractiveMode(nodes_head, &interactive_orig_node);
            ReverseDijkstra(nodes_head, interactive_dest_node,1);
            
            if(interactive_orig_node->destHead->type == INFINITE){
                printf("The source node %d can't reach that destiny \n", interactive_orig_node->id);
            } else{
                printf("Node %d can reach node %d with: TYPE:%d  &  COST:%d\n", interactive_orig_node->id, interactive_dest_node->id, interactive_orig_node->destHead->type, interactive_orig_node->destHead->cost );
            }
            
            break;

        case algorithm:
            Algorithm(nodes_head);
            writeStatistics();
            break;

        default:
            printf("Error: Bad arguments in program call\n");
            printf("Type: ./graph -m help\n");
            exit(0);

            break;
    }

    clearAllDest(nodes_head);
    freeGraphNodes(nodes_head);
    fclose(fp);
    free(times_simulations);
    free(types);
    free(costs);
    return 0;
}

void commandLineValidation(int argc, char **argv)
{
    

    if(argc != 5)
    {
        printf("Error: Bad arguments in program call\n");
        printf("Type: ./graph -m help\n");
        exit(0);
    }
    if( argc == 5 && (strcmp(argv[2], "help") == 0))
    {
        printf("A aplicação graph é invocada com o comando\n\tgraph -m <interactive_sim/interactive_algo/algorithm/simulation/help> -i<net file>\n");
        exit(1);
    }

    return;
}




void write_times()
{
    FILE *fp;
    int i;

    fp = fopen("times.txt","w");
    

    for(i = 0; i < nr_nodes; i++){
        fprintf(fp, "%d\n", times_simulations[i]);
    }
    fclose(fp);
}


Nodes *InteractiveMode(Nodes *nodes_head, Nodes **interactive_orig_node){

    Nodes *node, *dest;
    int source_node_id, dest_node_id;
    char buffer_source[128], buffer_dest[128];

    printf("\nWelcome to interactive mode!\n");

    printf("Please insert source node: "); fflush(stdout);
    if(fgets(buffer_source, 128, stdin)!=NULL){
        while(sscanf(buffer_source, "%d", &source_node_id)!=1){
            printf("Please insert an integer: "); fflush(stdout);
            fgets(buffer_source, 128, stdin);
        }
    }

    if ((node = searchNodesList(nodes_head,source_node_id)) == NULL){
        printf("Sorry but that node does not exists in this network.\n"); 
        exit(0);
    }

    fflush(stdin);

    printf("Please insert destiny node: "); fflush(stdout);
    if(fgets(buffer_dest, 128, stdin)!=NULL){
        while(sscanf(buffer_dest, "%d", &dest_node_id)!=1){
            printf("Please insert an integer: "); fflush(stdout);
            fgets(buffer_dest, 128, stdin);
        }
    }

    if ( searchNodesList(nodes_head,dest_node_id) == NULL){
        printf("Sorry but that node does not exists in this network.\n"); 
        exit(0);
    }


    dest = searchNodesList(nodes_head,dest_node_id);
    *interactive_orig_node = node;

    return dest;
}


void writeStatistics(){

    FILE *fd_t, *fd_c;
    int i;

    costs[0]=0;
       
    if((fd_t = fopen("types.txt","w")) == NULL){
        printf("Error: Could not open file \n");
        exit(0);
    }

    if((fd_c = fopen("costs.txt","w")) == NULL){
        printf("Error: Could not open file \n");
        exit(0);
    }

    for(i=0; i<4; i++){
        fprintf(fd_t,"%d\n", types[i]);
    }

    for(i=0; i<1024; i++){
        fprintf(fd_c,"%d\n", costs[i]);
    }

    fclose(fd_c);
    fclose(fd_t);
    return;
}