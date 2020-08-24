#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Since the program runs with the Floyd’s algorithm I changed INF value as 1000
//While testing the algorithm with negative value changed INF into biggest num
const int QUEUE_MAX_SIZE = 30; //Max Queue Size
int heapsize = 0; /*global variable for counting heap size*/

char num_city[30][30];

typedef struct Node{
    char city[30];
    int distanceFromRoot;
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

void makeConnectionNode(adjacency_list * list, int a, int b, int d)
{
    Node * node = (Node*)malloc(sizeof(Node));
    strcpy(node->city,num_city[b]);
    node -> distanceFromRoot = d;
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
        printf("%s root ", num_city[i]);
        while (temp)
        {
            printf("-> %s : %d", temp->city, temp->distanceFromRoot);
            temp = temp -> right_node;
        }
        printf("\n");
    }
}

int GETDISTANCEFROMA2B(adjacency_list * list, int a, int b)
{
  Node * temp = list -> list[a].root;
  while(temp)
  {
    if(!strcmp(num_city[b],temp->city))
    {
      return temp->distanceFromRoot;
    }
    temp = temp -> right_node;
  }
  return 1000;
}

typedef struct {
    char name[20];
    int key;
}Info;

Info * Info_Array[QUEUE_MAX_SIZE];

void exchange(Info * x1, Info * x2)
{
    Info temp;
    temp = *x2;
    temp = *x1;
    *x1 = *x2;
    *x2 = temp;
}

void Min_Heapify(Info * A[],int i)
{
    int left, right, min = i; //left:left child / right:right child / max:index which has max key
    left = i*2;
    right = i*2+1;
    if(left <= heapsize)
    {
       if(A[left-1]->key<A[i-1]->key) min = left;
       else min = i;
    }
    if(right <= heapsize)
    {
        if(A[right-1]->key<A[min-1]->key) min = right;
    }
    if(min!=i)
    {
        exchange(A[i-1], A[min-1]);
        Min_Heapify(A,min);
    }
}

void DECREASE_KEY(Info * A[],int index,int key)
{
     if(key > A[index-1]->key)
        printf("the key is greater than current key!\n");
     else
     {
         A[index-1]->key = key;
         while((index>1))
         {
            if(A[(index/2)-1]->key>A[index-1]->key)
            {
                exchange(A[index-1],A[(index/2)-1]);
                index = index/2;
            }
            else break;
         }
     }
    
}

void INSERT(Info * A[],int key,char name[])
{
     heapsize++;
     A[heapsize-1] = malloc(sizeof(Info));
     A[heapsize-1]->key = 10000;
     strcpy(A[heapsize-1]->name,name);
     DECREASE_KEY(A,heapsize,key);
}

void EXTRACT_MIN(Info * A[])
{
    A[0]=A[heapsize-1];
    heapsize--;
    Min_Heapify(A,1);
}

void Relax(Info * u, Info * v, int w)
{
  if(v->key > u->key + w)
  {
    v->key = u->key + w;
  }
}

void floyd (int count, int array[30][30]) {
    int i, j, k;
    for (k = 0;k < count; k++)
    {
        for (i = 0;i < count; i++)
        {
            for (j = 0;j < count; j++)
            {
                if (array[i][k] != 1000 && array[k][j] != 1000
                    && array[i][k] + array[k][j] < array[i][j])
                {
                    array[i][j] = array[i][k] + array[k][j];
                }
            }
        }
    }
}

int main(void)
{
    FILE * fp = fopen("hw7_data.txt","r");
    char line[1024];
    int array[30][30];
    
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
            strcpy(num_city[count],ptr);
            count ++;
            ptr = strtok(NULL," \t\n");
        }
    }
    num_city[count-1][strlen(num_city[count-1])-1] = '\0';

    for(int i = 0; i<count; i++)
    {
        fgets(line,sizeof(line),fp);
        char * ptr2 = strtok(line," \t");
        for(int j = 0; j<count+1; j++)
        {
            if(ptr2[0]==13||ptr2[0]==10)
            {
                ptr2 = strtok(NULL," \t\n");
                continue;
            }
            if(j!=0)
            {
              if(strstr(ptr2, "INF") != NULL)
              {
                array[i][j-1] = 1000;
              }
              else {
                array[i][j-1] = atoi(ptr2);
              }
            }
            ptr2 = strtok(NULL," \t");
        }
    }

    for(int i = 0; i<count; i++)
    {
      for(int j = 0; j<count; j++)
      {
        printf("%d ",array[i][j]);
      }
      printf("\n");
    }
               
    adjacency_list * list = create_adjacency_list(count);
    
    for(int i = 0; i<count; i++)
    {
        for(int j = count-1 ; j >= 0 ; j--)
        {
            if(array[i][j]>0) makeConnectionNode(list,i,j,array[i][j]);
        }
    }
    
    print(list);

    //Dijkstra's algorithm

    //From index 0 to index 1
    //for(int start=0; start<count; start++)
    int sol [30][30];

    for(int i = 0; i<30; i++)
    {
      for(int j = 0; j<30; j++)
      {
        if(i==j) sol[i][j] = 0;
      }
    }
  
  clock_t start, end;
  double result;
  start = clock();
  for(int k = 0; k<count; k++)
  {
    for(int i = 0; i<count; i++)
    {
      INSERT(Info_Array, 10000 , num_city[i]);
    }

    DECREASE_KEY(Info_Array,k+1,0);

    //DECREASE_KEY(Info * A[],int index,int key)
    //index = 0;
    while(heapsize)
    {
      int temp_name_index = 0;
      Info * temp = Info_Array[0];
      for(int i = 0; i<30; i++)
      {
          if(!strcmp(num_city[i],temp->name))
          {
            temp_name_index = i;
            break;
          }
      }
      Node * temp2 = list->list[temp_name_index].root;
      Info * temp3;
      int error = 0;
      while(temp2)
      {
        for(int i = 0; i<heapsize; i++)
        {
          if(!strcmp(Info_Array[i]->name,temp2->city))
          {
            temp3 = Info_Array[i];
            break;
          }
          if(i==heapsize-1) error = 1;
        }
        if(error == 1){
          error = 0;
          temp2 = temp2 -> right_node;
          continue;
        }
        Relax(temp,temp3,GETDISTANCEFROMA2B(list,temp_name_index,temp2->index));
        temp2 = temp2 -> right_node;
        //Relax(Info u, Info v, int w)
      }
      
      for(int i = 1; i<heapsize-1; i++)
      {
        Min_Heapify(Info_Array,i);
      }
      
      EXTRACT_MIN(Info_Array);

      sol[k][temp_name_index] = temp->key;
    }
  }
  end = clock();
  result = (double)(end - start);
  printf("It took %f mili seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n",result);
  for(int i = 0; i<count; i++)
  {
    for(int j = 0; j<count; j++)
    {
      printf("%d ",sol[i][j]);
    }
    printf("\n");
  }
  start = clock();

  floyd(count,array);

  end = clock();

  result = (double)(end-start);

  printf("It took %f mili seconds to compute shortest path between cities with Floyd algorithm as follows.\n",result);
  for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if(array[i][j]==1000)
                    printf("  INf");
                  else
                    printf("%5d", array[i][j]);
        }
        printf("\n");
    }
  fclose(fp);
}



