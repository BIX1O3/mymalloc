#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];


double *malloc(size_t objsize){

    int Oursize = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &

    typedef struct MetaData{ //If we want to have two ints we can use two short ints which are only 2 bytes each
        int state;  //free = -1 and allocated is set to 1 
        struct MetaData *next;   //points to next metadata block //probably don't need prev. only need state and next
    }MetaData;

    

    MetaData* head = memory;


    if ((*head).state != -1 && (*head).state != 1){
        MetaData* newnode = (MetaData*)(memory+objsize);
        newnode->state = -1;
        newnode->next = NULL;
        head->state = 1;
        head->next = newnode;
    }
    



    

}


void free(double *ptr){

}

