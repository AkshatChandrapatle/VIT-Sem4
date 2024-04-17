#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node
{
    int vertex;
    int weight;
    struct node *next;
} *A[10];

void init(struct node *A[], int n)
{
    int i;
    for (int i = 0; i < n; i++)
    {
        A[i] = NULL;
    }
}

void create(struct node *A[])
{
    struct node *new, *p;
    char ch;
    do
    {
        int v1, v2 ,w;
        printf("Enter edge (i.e v1 and v2) & weight: ");
        scanf("%d%d%d", &v1, &v2,&w);

        //printf("Enter weight between %d & %d :",v1,v2);
        //scanf("%d",&w);
        new = (struct node *)malloc(sizeof(struct node));
        new->vertex = v2;
        new->weight=w;
        new->next = NULL;
        p = A[v1];
        if (p == NULL)
        {
            A[v1] = new;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = new;
        }
        new = (struct node *)malloc(sizeof(struct node));
        new->vertex = v1;
        new->weight=w;
        new->next = NULL;
        p = A[v2];
        if (p == NULL)
        {
            A[v2] = new;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = new;
        }
        printf("Want to add more edges(y/n): ");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');
}

void dijkstra(struct node *adj_list[], int n, int source, int* dist, int* parent) {

    int visited[n];
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    int count=1;

    while (count<n) {

        int min_dist = INT_MAX;
        int nextnode = -1;
        for (int i = 0; i < n; i++) {
            if (visited[i]!=1 && dist[i] < min_dist) {
                min_dist = dist[i];
                nextnode = i;
            }
        }

        visited[nextnode] = 1;

        struct node* curr = adj_list[nextnode];
        while (curr != NULL) {
            int v = curr->vertex;
            int distance = curr->weight;
            if (visited[v]!=1 && dist[nextnode] + distance < dist[v]) {
                dist[v] = dist[nextnode] + distance;
                parent[v] = nextnode;
            }
            curr = curr->next;
        }
        count++;
    }
}

void print_path(int* parent, int node) {
    if (parent[node] == -1) {
        printf("%d", node);
    } else {
        print_path(parent, parent[node]);
        printf(" -> %d", node);
    }
}

void printgraph(struct node *A[],int n){
    for(int i=0;i<n;i++){
        struct node *a=A[i];

        printf("%d -->",i);
        while(a){
            printf("%d  ",a->vertex);
            a=a->next;

        }

        printf("\n");
    }
}


int main(int argc, char const *argv[])
{
    int n;
    printf("Enter number of vertex: ");
    scanf("%d", &n);
    init(A, n);
    create(A);
    printgraph(A,n);

    int source ;
    printf("Enter the source node :");
    scanf("%d",&source);
    int dist[n];
    int parent[n];
    dijkstra(A, n, source, dist, parent);

    for (int i = 0; i < n; i++) {
        printf("Shortest distance from node %d to node %d is %d\n", source, i, dist[i]);
    }
    return 0;
}

