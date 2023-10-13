#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];


void *malloc(size_t objsize){

    int objsize = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &

    typedef struct MetaData{ //If we want to have two ints we can use two short ints which are only 2 bytes each
        int state;  //free = 0 and allocated is set to 1 
        int size;   //points to next metadata block 
    }MetaData;

    
    char *heap_start = (char*)memory;
    MetaData* head = memory;
    void *ptr;

    if ((*head).state == 0 && (*head).size == 0){
        MetaData* newnode = (MetaData*)(heap_start+objsize);
        head->state = 1;
        head->size = objsize+8;
        newnode->state = 0;  //originally 0 so this is is redundant
        newnode->size = (MEMLENGTH * 8) - (head->size);

        ptr = heap_start + 8;
    } else {
        MetaData* currentNode = head;
        int position = 0;
        int node = 0;
        int old_size = 0;
        while(position < (MEMLENGTH*8) || node == 1) { //&& currentNode->state != 0 && currentNode->size != 0
            
            if (currentNode->state = 0 && currentNode->size >= objsize){
                MetaData* newnode = (MetaData*)(currentNode+objsize); 
                currentNode->state = 1;
                currentNode->size = old_size;
                currentNode->size = objsize+8;
                newnode->state = 0;  //originally 0 so this is is redundant
                newnode->size = old_size - (objsize+8); // check to see if this logic works
                node = 1;
            }else{
                currentNode = currentNode + currentNode->size;
            }
            position += currentNode->size;
            
            
            /*currentNode = (MetaData*)(currentNode + currentNode->size);
            position++;
            if(position == 512) {
                // idfk man
            }*/
        }
        /*MetaData* newnode = (MetaData*)(currentNode + objsize);
        newnode->state = -1;
        newnode->size = 4096 - (8*(position+objsize));
        currentNode->state = 1;
        currentNode-> size = objsize;*/
    }
    



    return ptr;
}


void free(double *ptr){

}

int main(int argc, char** argv){


    return 0;
}