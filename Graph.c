#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

/*represents an edge or origin vertice and value of the vertice */
typedef struct item{
    int ver,val;
}*Item;


typedef struct node{
    Item edge;
    struct node *next;
}*link;


typedef struct graph{
    link *heads;
    int Size;
}*Graph;






int isConnected(link head, int j){
    link t;
    for(t = head;t != NULL;t = t->next)
        if(t->edge->ver == j) return TRUE;
    return FALSE;
}

link insertNode(link head,int v2,int val){
    link t = (link)malloc(sizeof(struct node));
    t->edge = (Item)malloc(sizeof(struct item));

    t->edge->ver = v2;
    t->edge->val = val;
    t->next = head;
    
    return t;
}

Graph initGraph(int size,float sparseness,int directed,int maxDist){
    int i,j,dist,counter = 0;
    float random;
    Graph new = (Graph)malloc(sizeof(struct graph));
    new->heads = (link*)malloc(sizeof(struct node)*size);
    new->Size = size;

    for(i = 0; i < size; i++) new->heads[i] = NULL;

    for(i = 0;i < size; i++){
        for(j = size-1; j >= 0; j--){
            if(i != j){
                random = (float)rand()/RAND_MAX;
                dist = rand()%maxDist+1;/*minimum distance is 1 */

                if(random > sparseness){
                    if(directed == TRUE || isConnected(new->heads[j],i) == FALSE){
                        counter++;
                        new->heads[i] = insertNode(new->heads[i],j,dist);
                        if(directed == FALSE){
                            new->heads[j] = insertNode(new->heads[j],i,dist);
                        }
                    }
                }
            }
        }
    }
    /*info about the graph */ 
    printf("Edges: %d | Edges/Max: %f | Edges/Vertex: %f\n",
    counter,(float)counter/(size*(size-1)),(float)counter/size);
    return new;
}

void printGraph(Graph g){
    int i;
    link t;
    for(i = 0;i < g->Size; i++){
        printf("%d |",i);
        for(t = g->heads[i];t != NULL;t = t->next){
            printf(" (%d,%d)",t->edge->ver,t->edge->val);
        }
        printf("\n");
    }
}

void freeGraph(Graph g){
    int i;
    link t,prev;
    for(i = 0;i < g->Size;i++){
        for(t = g->heads[i];t != NULL;prev = t,t = t->next,free(prev->edge),free(prev));
    }
    free(g->heads);
    free(g);
}

int Dijkstra(Graph g,int vi,int vf){
    Item new = (Item)malloc(sizeof(struct item)),aux;
    Item *vertices = (Item*)malloc(sizeof(Item)*g->Size);
    link t;
    int i,dist,shortest = __INT32_MAX__;
    int flag = FALSE;/*used to know if there is no solution */
    
    for(i = 0;i < g->Size;i++)vertices[i] = NULL;/*just to be sure everything starts with NULL */

    vertices[vi] = (Item)malloc(sizeof(struct item));
    vertices[vi]->ver = -1;/*beginning */
    vertices[vi]->val = 0;
    

    while(TRUE){
        for(i = 0;i < g->Size;i++){
            if(vertices[i] != NULL){
                for(t = g->heads[i];t != NULL;t = t->next){
                    if(vertices[t->edge->ver] == NULL){
                        flag = TRUE;
                        dist = vertices[i]->val+t->edge->val;
                        if(dist < shortest){
                            shortest = dist;
                            new->ver = t->edge->ver;
                            new->val = i;
                        }
                    }
                }
            }
        }
        if(flag == FALSE){
            printf("It's impossible to reach vertex %d from %d\n",vf,vi);
            return shortest;
        }else flag = FALSE;

        vertices[new->ver] = (Item)malloc(sizeof(struct item));/*Dijkstra guarantees new->ver wasn't yet malloced */
        vertices[new->ver]->val = shortest;
        vertices[new->ver]->ver = new->val;

        if(new->ver == vf){
            /*notice the order is going to be backwards */
            aux = new;
            while(aux->ver != -1){
                printf("--%d",aux->ver);
                aux = vertices[aux->ver];
            }
            printf("\n");
            
            /*quick free of local mallocs */
            for(i = 0;i< g->Size;i++)if(vertices[i] != NULL)free(vertices[i]);
            free(vertices);
            free(new);
            return shortest;
        }
        shortest = __INT32_MAX__;
    }
}


int main(){
    int size,maxDist,directed,PrintGraph,dist,vi,vf;
    float sparseness;
    char isDirected,end;
    Graph g;
    printf("Insert size of the graph: ");
    scanf("%d",&size);
    printf("Insert maximum distance between two vertices of the graph: ");
    scanf("%d",&maxDist);
    printf("Insert sparseness of the graph: ");
    scanf("%f",&sparseness);
    printf("Insert (y/n) for directed graph: ");
    getchar();
    isDirected = getchar();

    if(isDirected == 'y')directed = TRUE;
    else directed = FALSE;

    g = initGraph(size,sparseness,directed,maxDist);

    printf("Print Graph? (y/n) ");
    getchar();
    PrintGraph = getchar();

    if(PrintGraph == 'y')printGraph(g);

    do{
        printf("Insert origin vertex: ");
        scanf("%d",&vi);
        printf("Insert target vertex: ");
        scanf("%d",&vf);

        dist = Dijkstra(g,vi,vf);
        printf("%d\n",dist);
    
        printf("Insert x to exit or anything else to new paths: ");
        getchar();
        end = getchar();
    }while(end != 'x');
    
    freeGraph(g);

    return 0;
}

