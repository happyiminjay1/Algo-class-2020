/* This assignment was conducted on repl.it */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name [20];
    int key;
}Info;

const int QUEUE_MAX_SIZE = 30; //Max Queue Size
const int KEY_MAX_SIZE = 10; //Max Key Size
const int CHARACTER_MAX_SIZE = 20; //Max Character Size
int heapsize = 0; /*global variable for counting heap size*/

/* exchange the two variable of the Info array item*/
void exchange(Info * x1, Info * x2)
{
    Info temp;
    temp = *x2;
    temp = *x1;
    *x1 = *x2;
    *x2 = temp;
}

/*Max-Heap sort from the given i(index)*/
void Max_Heapify(Info * A[],int i)
{
    int left, right, max = i; //left:left child / right:right child / max:index which has max key
    left = i*2;
    right = i*2+1;
    if(left <= heapsize)
    {
       if(A[left-1]->key>A[i-1]->key) max = left;
       else max = i;
    }
    if(right <= heapsize)
    {
        if(A[right-1]->key>A[max-1]->key) max = right;
    }
    if(max!=i)
    {
        exchange(A[i-1], A[max-1]);
        Max_Heapify(A,max);
    }
}
//increas Key value as given index
void INCREASE_KEY(Info * A[],int index,int key)
{
    
     if(key < A[index-1]->key)
        printf("the key is smaller than current key!\n");
     else
     {
         A[index-1]->key = key;
         while((index>1))
         {
            if(A[(index/2)-1]->key<A[index-1]->key)
            {
                exchange(A[index-1],A[(index/2)-1]);
                index = index/2;
            }
            else break;
         }
     }
    
}
//Insert and do max-heap sort
void INSERT(Info * A[],int key,char name[])
{
     heapsize++;
     A[heapsize-1] = malloc(sizeof(Info));
     A[heapsize-1]->key = 0;
     strcpy(A[heapsize-1]->name,name);
     INCREASE_KEY(A,heapsize,key);
}
//Delete the first value and do max-heap sort
void EXTRACT_MAX(Info * A[])
{
    A[0]=A[heapsize-1];
    if(heapsize>1)
    {
        free(A[heapsize-1]);
    }
    heapsize--;
    Max_Heapify(A,1);
}

int main(int argc, const char * argv[]) {
    
    char select;
    int key = 0;
    char name[CHARACTER_MAX_SIZE];
    
    Info * Info_Array[QUEUE_MAX_SIZE];

    while(1)
    {
        printf("*********** MENU ***********\n");
        printf("I : Insert new element into queue.\n");
        printf("D : Delete element with largest key from queue.\n");
        printf("R : Retrieve element with largest key from queue.\n");
        printf("K : Increase key of element in queue.\n");
        printf("P : Print out all elements in queue.\n");
        printf("Q : Quit\n\n");
        
        printf("Choose menu:  ");
        scanf(" %c",&select);
        
        if(select=='I'|select=='i')
        {
            if(heapsize==30) printf("Queue is fulled");
            else {
                printf("Enter name of element: ");
                scanf("%s",name);
                printf("Enter key calue of element : ");
                scanf("%d",&key);
                if(key<1 | key>10)
                {
                    printf("Invalid Key value!\n");
                    continue;
                }
                INSERT(Info_Array, key, name);
                printf("New element [%s, %d] is inserted.\n\n",name,key);
            }
        }
        else if(select=='D'|select=='d')
        {
            if(heapsize<1) printf("heap underflow\n\n");
            else{
                printf("[%s, %d] is deleted.\n\n",Info_Array[0]->name,Info_Array[0]->key);
                EXTRACT_MAX(Info_Array);
            }
        }
        else if(select=='R'|select=='r')
        {
            if(heapsize<1) printf("heap underflow\n");
            else{
                printf("[%s, %d]\n\n",Info_Array[0]->name,Info_Array[0]->key);
            }
        }
        else if(select=='K'|select=='k')
        {
            int index;
            printf("Enter index of element: ");
            scanf("%d",&index);
            printf("Enter new key value: ");
            scanf("%d",&key);
            INCREASE_KEY(Info_Array, index, key);
            printf("\n");
        }
        else if(select=='P'|select=='p')
        {
            for(int i = 0; i<heapsize; i++)
            {
                printf("[%s, %d] ",Info_Array[i]->name,Info_Array[i]->key);
            }
            printf("\n\n");
        }
        else if(select=='Q'|select=='q')
        {
            printf("Thank you. Bye!");
            break;
        }
        else{
            printf("Not allowed command. Please try again.\n\n");
        }
    }
}

