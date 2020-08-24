#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int num_alpha[20];

typedef struct Node{
    char alpha;
    int index;
    struct Node * right_node;
}Node;

typedef struct NodeList{
    struct Node * root;
}NodeList;

typedef struct adjacency_list{
    int num;
    struct NodeList * list;
}adjacency_list;

adjacency_list * create_adjacency_list(int num)
{
    adjacency_list * list = (adjacency_list*) malloc(sizeof(adjacency_list));
    list -> num = num;
    list -> list = (NodeList*) malloc(sizeof(NodeList) * num);
    
    // Each adjacency root is set to NULL
    for (int i = 0; i < num; i++)
    {
        list -> list[i].root = NULL;
    }

    return list;
}

void makeConnectionNode(adjacency_list * list, int a, int b)
{
    Node * node = (Node*)malloc(sizeof(Node));
    node -> alpha = num_alpha[b];
    node -> index = b;
    node -> right_node = NULL;
    node -> right_node = list->list[a].root;
    list -> list[a].root = node;
}

void print(adjacency_list * list)
{
    for (int i = 0; i < list->num ; i++)
    {
        Node * temp = list -> list[i].root;
        printf("%c root ", num_alpha[i]);
        while (temp)
        {
            printf("-> %c", temp->alpha);
            temp = temp -> right_node;
        }
        printf("\n");
    }
}

adjacency_list * do_transpose_list(adjacency_list * transpose_list, adjacency_list * list)
{
    Node * temp;
    
    for (int i = 0; i < list->num ; i++)
    {
        temp = list -> list[i].root;
        while (temp)
        {
            makeConnectionNode(transpose_list,temp->index,i);
            temp = temp -> right_node;
        }
    }
    
    for (int i = 0; i < list->num ; i++)
    {
        temp = transpose_list -> list[i].root;
        Node * tail = NULL;
        Node * curNode;
        
        while (temp)
        {
            curNode = temp -> right_node;
            temp->right_node = tail;
            tail = temp;
            temp = curNode;
        }
        transpose_list -> list[i].root = tail;
    }
    return transpose_list;
}

int main(void)
{
    FILE * fp = fopen("hw6_.data.txt","r");
    char line[1024];
    int array[20][20];
    
    int count = 0;

    if (fp == NULL) printf("can't open file");
    else
    {
        fgets(line,sizeof(line),fp);
        char * ptr = strtok(line," \t\n");
        while(ptr != NULL) {
            if(ptr[0]==13||ptr[0]==10)
            {
                ptr = strtok(NULL," \t\n");
                continue;
            }
            num_alpha[count] = ptr[0];
            count ++;
            ptr = strtok(NULL," \t\n");
        }
    }
    
    for(int i = 0; i<count; i++)
    {
        fgets(line,sizeof(line),fp);
        char * ptr2 = strtok(line," \t");
        for(int j = 0; j<count+1; j++)
        {
            if(j!=0) array[i][j-1] = atoi(ptr2);
            ptr2 = strtok(NULL," \t");
        }
    }
    
    adjacency_list * list = create_adjacency_list(count);
    
    for(int i = 0; i<count; i++)
    {
        for(int j = count-1 ; j >= 0 ; j--)
        {
            if(array[i][j]==1) makeConnectionNode(list,i,j);
        }
    }
    printf("Array of adjacency list of above graph\n");
    print(list);
    
    adjacency_list * transpose_list = create_adjacency_list(count);
    transpose_list = do_transpose_list(transpose_list,list);
    printf("Array of adjacency list of transpose graph\n");
    print(transpose_list);
    
    fclose(fp);
}


