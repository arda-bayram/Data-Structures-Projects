//ARDA BAYRAM - 150116029
//This program calculate centrality, closeness and betweenness degrees in a social group
//Reference-1: https://www.geeksforgeeks.org/graph-and-its-representations/  
//Reference-2: https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#define SIZE 10
#define INFINITY 9999

int edges = 0;
int betArray[SIZE]; // Stores intermedia points
int betArray2[SIZE]; // Stores path points 
    
// A structure to represent an adjacency list node
struct AdjListNode
{
    char name[15];
    struct AdjListNode* next;
};
typedef struct AdjListNode AdjListNode;
 
// A structure to represent an adjacency list
struct AdjList 
{
    struct AdjListNode *head; // pointer to head node of the list
};
typedef struct AdjList AdjList;
 
// A structure to represent a graph
struct Graph 
{
    int V; // Number of vertices
    struct AdjList* array;
};
typedef struct Graph Graph;
 
// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(char name[15]) 
{
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
Graph* createGraph(int V) 
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
 
    // Create an array of adjacency lists. Size of array will be V
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
 
    // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(Graph* graph, int src, char name[15]) 
{
	edges++; // Count number of edges
	
    // Add an edge from src to dest.  A new node is added to the adjacency
    AdjListNode* newNode = newAdjListNode(name);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
}

// Print adjacency matrix 
void printMatrix(Graph *graph, int G[SIZE][SIZE])
{
	int i, j, k, p;
	
	const char *names[10] = {"Cem", "Ay�e", "Belma", "Edip", "Dundar", "Gamze", "Ferit", "Halit", "Ilke", "Jale"};
	printf("  ");
	
	for(i = 0; i < 10; i++)
	{
		printf("%10s", names[i]);
	}
	printf("\n");
	
	for(i = 0; i < 10; i++)
	{ 	
		printf("%-10s", names[i]);
		
		for(k = 0; k < 10; k++) // Check connections between nodes
		{
			p = 0;
			AdjListNode* matrix = graph->array[i].head;
			while(matrix != NULL)
			{	
				if(strcmp(matrix->name, names[k]) == 0)
				{
					p = 1;
					break;
				}
				matrix = matrix->next;
			}
			printf("%-10d", p);
			G[i][k] = p;
		}
		printf("\n");
    }
}

// Print Degree Centrality table
void centDegree(Graph *graph)
{
	int v, cnt;
	const char *names[10] = {"Cem", "Ay�e", "Belma", "Edip", "Dundar", "Gamze", "Ferit", "Halit", "Ilke", "Jale"};
	
	printf("%-10s%-10s%-10s\n", "Node", "Score", "Standardized Score");
	printf("---------------------------------------\n");
	
    for (v = 0; v < graph->V; v++) 
	{
    	cnt = 0;
        AdjListNode* cent = graph->array[v].head;
        while(cent) 
		{
			cnt++;
            cent = cent->next;
        }
     	printf("%-10s%-10d%lf\n", names[v], cnt, (float)cnt/(graph->V-1));  
    }
}

// Print Closeness Centrality table 
void closeCent(int G[SIZE][SIZE],int n,int startnode)
{
	// pred[] stores the predecessor of each node
    int cost[SIZE][SIZE],distance[SIZE],pred[SIZE];
    // Count gives the number of nodes seen so far
    int visited[SIZE],count,minDistance,nextnode,i,j, result;
   	const char *names[10] = {"Cem", "Ay�e", "Belma", "Edip", "Dundar", "Gamze", "Ferit", "Halit", "Ilke", "Jale"};
    
    // Create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    // Initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    
    distance[startnode]=0; 
    visited[startnode]=1;
    count=1;
    
    while(count < n-1)
    {
        minDistance = INFINITY;
        
        // Nextnode gives the node at minimum distance
        for(i = 0; i < n; i++)
            if(distance[i] < minDistance && !visited[i])
            {
                minDistance = distance[i];
                nextnode = i;
            }
            
        // Check if a better path exists through nextnode            
        visited[nextnode] = 1;
        for(i = 0; i < n; i++)
            if(!visited[i])
                if(minDistance+cost[nextnode][i] < distance[i])
                {
                    distance[i] = minDistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }
 
 	result = 0;
 	
    // Calculate sum of distances
    for(i=0;i<n;i++)
        if(i!=startnode)
        	result += distance[i];

    printf("%-10s%d/%-10d%lf\n", names[startnode], 1, result, ((float)(SIZE-1)/result)); 
}

// Print Betweenness Centrality table 
void between(int G[SIZE][SIZE],int n,int startnode)
{
    // pred[] stores the predecessor of each node
    int cost[SIZE][SIZE],distance[SIZE],pred[SIZE];
    // Count gives the number of nodes seen so far
    int visited[SIZE],count,minDistance,nextnode,i,j, result;
    const char *names[10] = {"Cem", "Ay�e", "Belma", "Edip", "Dundar", "Gamze", "Ferit", "Halit", "Ilke", "Jale"};
    const int nameSize[10] = {3, 4, 5, 4, 6, 5, 5, 5, 4, 4};
    
    // Create the cost matrix
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];
    
    // Initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    
    while(count < n-1)
    {
        minDistance = INFINITY;
        
        // Nextnode gives the node at minimum distance
        for(i = 0; i < n; i++)
            if(distance[i] < minDistance && !visited[i])
            {
                minDistance = distance[i];
                nextnode = i;
            }
            
        // Check if a better path exists through nextnode            
        visited[nextnode] = 1;
        for(i = 0; i < n; i++)
            if(!visited[i])
                if(minDistance + cost[nextnode][i] < distance[i])
                {
                    distance[i] = minDistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
        count++;
    }
 
 	result = 0;
 	
 	int k, l, pred2[10], cnt, cnt2;
 	
    // Print the path and intermedia nodes between two nodes
    for(i = 0; i < n; i++)
    {
        if(i > startnode)
        {
        	printf("%-10s%-10s", names[startnode], names[i]);
            
            j = i;
            cnt = 0; 
            cnt2 = 0; // Use cnt2 to calculate spaces
            
            do
            {
                j = pred[j];
                cnt++;
                pred2[cnt] = j;
            }while(j != startnode);
            
            printf("%-4s", " ");
			if(cnt == 1){
				printf("%s", "-");
				cnt2 = 1;
			}
				
			else
			{
				for(k = cnt - 1; k > 1; k--)
				{
					printf("%s-", names[pred2[k]]);

					for(l = 0; l < SIZE; l++)
                		if(pred2[k] == l) betArray[l]++;

					cnt2 += nameSize[pred2[k]] + 1;
					
				}	
				printf("%s", names[pred2[k]]);
				for(l = 0; l < SIZE; l++)
                	if(pred2[k] == l) betArray[l]++;
                cnt2 += nameSize[pred2[k]];	
			}
			
			for(k = 0; k <= 25 - cnt2; k++)
				printf(" ");
				
			for(k = cnt; k > 0; k--)
			{
				printf("%s->", names[pred2[k]]);
				for(l = 0; l < SIZE; l++)
                	if(pred2[k] == l) betArray2[l]++;
			}
            	
            printf("%s", names[i]);
			for(l = 0; l < SIZE; l++)
                	if(i == l) betArray2[l]++;	
			printf("\n");	
		}
    }	
}

void main()
{
	setlocale(LC_ALL, "Turkish"); // Allow Turkish characters

    int G[SIZE][SIZE], i, j;
	
	// Create the graph
    Graph* graph = createGraph(SIZE);
    
    addEdge(graph, 0, "Ay�e");addEdge(graph, 0, "Ferit");
    addEdge(graph, 0, "Dundar");addEdge(graph, 1, "Cem");
    addEdge(graph, 1, "Ferit");addEdge(graph, 1, "Dundar");
    addEdge(graph, 1, "Belma");addEdge(graph, 2, "Ay�e");
    addEdge(graph, 2, "Dundar");addEdge(graph, 2, "Edip");
    addEdge(graph, 3, "Belma");addEdge(graph, 3, "Dundar");
    addEdge(graph, 3, "Gamze");addEdge(graph, 4, "Ay�e"); 
    addEdge(graph, 4, "Belma");addEdge(graph, 4, "Cem"); 
    addEdge(graph, 4, "Ferit");addEdge(graph, 4, "Gamze");
    addEdge(graph, 4, "Edip");addEdge(graph, 5, "Dundar");
    addEdge(graph, 5, "Edip");addEdge(graph, 5, "Ferit");
    addEdge(graph, 5, "Halit");addEdge(graph, 6, "Ay�e");
	addEdge(graph, 6, "Cem");addEdge(graph, 6, "Dundar");
	addEdge(graph, 6, "Gamze");addEdge(graph, 6, "Halit");
	addEdge(graph, 7, "Ferit");addEdge(graph, 7, "Gamze");
	addEdge(graph, 7, "Ilke");addEdge(graph, 8, "Halit");
	addEdge(graph, 8, "Jale");addEdge(graph, 9, "Ilke");
    
    for(i = 0; i < SIZE; i++) // Initialize betArray and betArray2
	{
		betArray[i] = 0;
		betArray2[i] = 0;
	}
	
    printMatrix(graph, G); // Print matrix	
    
	// Print tables one by one	
    printf("\n\n**Degree Centrality**\n\n"); 
    centDegree(graph);
    
    printf("\n\n**Closeness Centrality**\n\n");
	printf("%-10s%-10s%-10s\n", "Node", "Score", "Standardized Score");
	printf("---------------------------------------\n");
	for(i = 0; i < SIZE; i++)
		closeCent(G, SIZE, i);	 	
    
    printf("\n\n**Betweenness Centrality**");
	printf("\n\n%-10s%-13s%-27s%-10s\n", "Source", "Target", "Intermedia Nodes", "Path");
	printf("-------------------------------------------------------------------------------------------\n");
	for(i = 0; i < SIZE; i++)
		between(G, SIZE, i);
	
	printf("\n\n%-10s%-10s%-10s\n", "Node", "Score", "Standardized Score");	
	printf("---------------------------------------\n");
	for(i = 0; i < SIZE; i++)
	{
		if(betArray[i] == 0) printf("%-10d%-11d%lf\n", i+1, 0, 0);
		else
		{
			if(betArray[i] >= 10) printf("%-10d%d/%-8d%lf\n", i+1, betArray2[i], betArray[i],
										 ((float)betArray[i]/(betArray2[i]*(SIZE-1)*(SIZE-2)/2)));
			else printf("%-10d%d/%-8d%lf\n", i+1, betArray2[i], betArray[i],
										 ((float)betArray[i]/(betArray2[i]*(SIZE-1)*(SIZE-2)/2)));
		}	
	}
}
