#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//My program works for greedy, DP and Branch and Bound
//It has error for large input...
const int MAX = 100000;

struct Item
{
    int benefit, weight;
    float div;
};

typedef struct node{
    int benefit;
    int weight;
    int bound;//key
    int layer;
}Node;

int heapsize = 0;
int numberofItem = 0;
int capacity = 0;
int max_benefit = 0;
int array[3]; // layer, benefit, weight
void exchange(Node * x1, Node * x2)
{
    Node temp;
    temp = *x1;
    *x1 = *x2;
    *x2 = temp;
}

void Max_Heapify(Node * A[],int i)
{
    int left, right, max = i; //left:left child / right:right child / max:index which has max key
    left = i*2;
    right = i*2+1;
    if(left <= heapsize)
    {
       if(A[left-1]->bound>A[i-1]->bound) max = left;
       else max = i;
    }
    if(right <= heapsize)
    {
        if(A[right-1]->bound>A[max-1]->bound) max = right;
    }
    if(max!=i)
    {
        exchange(A[i-1], A[max-1]);
        Max_Heapify(A,max);
    }
}

void INCREASE_KEY(Node * A[],int index, int bound)
{

     if(bound < A[index-1]->bound)
        printf("the key is smaller than current key!\n");
     else
     {
         A[index-1]->bound = bound;
         while((index>1))
         {
            if(A[(index/2)-1]->bound<A[index-1]->bound)
            {
                exchange(A[index-1],A[(index/2)-1]);
                index = index/2;
            }
            else break;
         }
     }
}

void INSERT(Node * A[],int layer,int benefit,int weight,struct Item item[],int contain)
{
    heapsize++;
    A[heapsize-1] = malloc(sizeof(Node));
    if(contain==1){
        benefit += item[layer-1].benefit;
        weight  += item[layer-1].weight;
    }
    A[heapsize-1]->benefit = benefit;
    A[heapsize-1]->weight  = weight;
    A[heapsize-1]->layer  = layer;
    int temp_benefit = benefit;
    int tot_weight = weight;
    while((capacity>=(tot_weight+item[layer].weight))&&(layer<numberofItem))
    {
        tot_weight += item[layer].weight;
        benefit += item[layer].benefit;
        layer++;
    }
    int bound;
    if(layer<numberofItem)
    {
      bound = benefit + (capacity-tot_weight) * item[layer].div;
    }
    else{
      bound = benefit;
    }
    A[heapsize-1]->bound = bound;
    if(!(bound<=max_benefit || weight > capacity))
    {
        if(temp_benefit>max_benefit) max_benefit = benefit;
        INCREASE_KEY(A,heapsize,bound);
    }
    else{
        if(A[heapsize-1] != NULL){
          free(A[heapsize-1]);
          A[heapsize-1]=NULL;
          heapsize--;
        }
    }
}

int EXTRACT_MAX(Node * A[])
{
    array[0] = A[0]->layer;
    array[1] = A[0]->benefit;
    array[2] = A[0]->weight;
    int bound = A[0]->bound;
    A[0]=A[heapsize-1];
    if(heapsize>1)
    {
        A[heapsize-1] = NULL;
        free(A[heapsize-1]);
    }
    heapsize--;
    Max_Heapify(A,1);
    if(max_benefit>=bound) return 0;
    else return 1;
}

int static compare (const void * first, const void * second)
{
    struct Item * temp1 = (struct Item *)first;
    struct Item * temp2 = (struct Item *)second;

    if (temp1->div < temp2->div) return 1;
    else if (temp1->div == temp2->div) return 0;
    else return -1;
}


float GreedyKnapsackSolver(struct Item item[]){
    //float * valuePerWeight = (float *)malloc(sizeof(float)*numberofItem);
    qsort(item,numberofItem,sizeof(struct Item),compare);

    int bag = 0;
    float result = 0;
    int count = 0;
    while(capacity>=bag+item[count].weight)
    {
        bag += item[count].weight;
        result += item[count].benefit;
        count++;
    }
    if(capacity!=bag)
    {
        result += (capacity-bag) * item[count].div;
    }

    return result;
}

int DPKnapsackSolver(struct Item item[]) {

    int B[numberofItem+1][capacity+1];

    for(int w = 0; w<=capacity; w++)
    {
        B[0][w] = 0;
    }

    for(int i = 1; i<=numberofItem; i++)
    {
        B[i][0] = 0;
        for(int w = 1; w<= capacity; w++)
        {
            if(item[i-1].weight <= w)
            {
                if(item[i-1].benefit + B[i-1][w-item[i-1].weight] > B[i-1][w])
                    B[i][w] = item[i-1].benefit + B[i-1][w-item[i-1].weight];
                else B[i][w] = B[i-1][w];
            }
            else B[i][w] = B[i-1][w];
        }
    }
    /*
    for(int w = 0; w<=capacity; w++)
    {
        for(int i = 0; i<=numberofItem; i++)
        {
            printf("%d ",B[i][w]);
        }
        printf("\n");
    }*/

    return B[numberofItem][capacity];
}

int BranchAndBoundKnapsackSolver(struct Item item[],Node * node_array[]) {

    int layer, benefit, weight;
    qsort(item,numberofItem,sizeof(struct Item),compare);
    INSERT(node_array,0,0,0,item,0);
    while(heapsize!=0)
    {
        int check = 0;

        check = EXTRACT_MAX(node_array);

        layer = array[0];
        benefit = array[1];
        weight = array[2];

        if(check==1)
        {
            INSERT(node_array, layer+1, benefit, weight, item, 1);
            INSERT(node_array, layer+1, benefit, weight, item, 0);
        }
        else{
        }
    }

    return max_benefit;
}

int main()
{
    Node * node_array[MAX] ;

    for (int i = 0 ; i < MAX; i++)
    {
        node_array[i] = NULL;
    }
    srand(1); // can change into srand(time(NULL));

    printf("Enter Number of Items : ");
    scanf("%d",&numberofItem);

    struct Item item[10000];
    /*
    numberofItem = 4;
    capacity = 16;
    item[0].benefit = 40;
    item[1].benefit = 30;
    item[2].benefit = 50;
    item[3].benefit = 10;

    item[0].weight  = 2;
    item[1].weight  = 5;
    item[2].weight  = 10;
    item[3].weight  = 5;*/

    capacity = numberofItem * 40;

    for(int i = 0; i < numberofItem; i++)
    {
        item[i].benefit = random() % 300 + 1;
    }

    for(int i = 0; i < numberofItem; i++)
    {
        item[i].weight = random() % 100 + 1;
    }
    /*
    printf("benefit ");
    for(int i = 0; i < numberofItem; i++)
    {
        printf("%d ",benefit[i]);
    }
    printf("\n");
    printf("weight ");
    for(int i = 0; i < numberofItem; i++)
    {
        printf("%d ",weight[i]);
    }
    printf("\n");
    */
    for(int i = 0; i < numberofItem; i++)
    {
        item[i].div = item[i].benefit / (float)item[i].weight;
    }
    clock_t start, end;
    float time;
    start = clock();
    int k = DPKnapsackSolver(item);
    end = clock();
    time = (float)(end - start)/CLOCKS_PER_SEC;
    printf("%.3f / %d\n",time,k);
    start = clock();
    float j = GreedyKnapsackSolver(item);
    end = clock();
    time = (float)(end - start)/CLOCKS_PER_SEC;
    printf("%.3f / %f\n",time,j);
    start = clock();
    int l = BranchAndBoundKnapsackSolver(item,node_array);
    end = clock();
    time = (float)(end - start)/CLOCKS_PER_SEC;
    printf("%.3f / %d\n",time,l);
}
