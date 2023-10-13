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
        int size;   //points to next metadata block 
    }MetaData;

    

    MetaData* head = memory;


    if ((*head).state != -1 && (*head).state != 1){
        MetaData* newnode = (MetaData*)(memory+objsize);
        newnode->state = -1;
        newnode->size = 4096 - (8*objsize);
        head->state = 1;
        head->size = objsize;
    } else {
        MetaData* currentNode = head;
        int count = 0;
        while(count < 512 && currentNode->state != 0 && currentNode->size != 0) {
            currentNode = (MetaData*)(currentNode + currentNode->size);
            count++;
            if(count == 512) {
                // idfk man
            }
        }
        MetaData* newnode = (MetaData*)(currentNode + objsize);
        newnode->state = -1;
        newnode->size = 4096 - (8*(count+objsize));
        currentNode->state = 1;
        currentNode-> size = objsize;
    }
    



    

}


void free(double *ptr){

}

