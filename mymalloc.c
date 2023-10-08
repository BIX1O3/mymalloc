#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];


double *malloc(size_t objsize){

    objsize = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &

    typedef struct MetaData{ //use a linklist for the header which contains the size of the chunk and free or allocated
        size_t chunk_size; //payload bytes + 8 bytes from metadata header
        int state;  //free = 0 and allocated is set to 1 
        struct MetaData *next,*prev;   //points to next metadata block //probably don't need prev. only need state and next
    }MetaData;

    //MetaData *free = (void*)memory;

    /*MetaData *heap_start;

    heap_start = (void*)memory;

    heap_start->state = 1;
    heap_start->chunk_size = objsize+8;

    MetaData *n1;

    n1->state=0;
    n1->chunk_size=(MEMLENGTH*4) - objsize-8;

    heap_start->next = n1;
    n1->next=NULL;*/

    char* heap_start = (char*)memory;


    /*
        malloc is called time:
        Check to see if malloc has ever been run. 
        If not go into an if statement that creates the first *node1 at the start of the memory array. chunk size = objsize+8, state you 1(allocated)
        Still in the if create a *node2 which will be after the payload memory (node2 ptr = memory+*node1->chunk_size) Then make *node1->next=*node2 and *node2->prev=*node1. *node2->chunk_size = (MEMLENGTH*4)-*node1->chunksize. *node2->state = 0

        then after for creating new nodes when mem is already initialized
        find current node(free node with available memory) change its state to allocated, change chunksize to objsize+8.
        create new node(new free node) make state free, set current node .next to the new node and the new nodes .prev to the current. make chunk size = MEMLENGTH - position in array.

        return *p (ptr at metadata +8 to get ptr to payload)

    */
    






}


void free(double *ptr){

}

