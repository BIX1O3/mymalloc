#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];


void *mymalloc(size_t objsize){
    
    if(objsize == 0) { // checks for request for 0 bytes
        printf("Error: Cannot allocate 0 bytes\n");
        return NULL;
    }

    long int objsize_8byte = (objsize+7) & ~7; //ensures that the object size is a multiple of 8
    
    
    if(objsize_8byte+8> MEMLENGTH*8) { // checks for a request larger than total memory size 
        printf("Error: Cannot allocate %ld byte(s)\n", objsize);
        return NULL;
    }

    typedef struct MetaData{ // If we want to have two ints we can use two short ints which are only 2 bytes each
        int state;  // free = 0 and allocated is set to 1 
        int size;   // points to next metadata block 
    }MetaData;

    if(objsize_8byte > (MEMLENGTH*8) - 8) {return NULL;} 
    

    char *heap_start = (char*)memory; // char* pointer to the start of memory for pointer arithmatic
    MetaData* head = (MetaData*) memory;
    void *ptr = NULL; // ensures that if there are no free nodes large enough to fit objsize_8byte malloc returns NULL

    int node = 0;
    
    if ((*head).state == 0 && (*head).size == 0){ // initilizes memory on malloc's first call
        head->state = 1; 
        head->size = objsize_8byte+8;

        MetaData* newnode = (MetaData*)(heap_start+(head->size)); // pointer with type MetaData to be used as the next free pointer
        newnode->state = 0;  
        newnode->size = (MEMLENGTH * 8) - (head->size);
        
        ptr = (void*)(heap_start + 8);
    } else {
        MetaData* currentNode = (MetaData*)heap_start;

        int position = 0;
        int count = 0;
        
        int old_size = 0;
        
        while(position < (MEMLENGTH*8) && node == 0) { // iterates through memory in order to find a suitable free node to allocate to
            if (count+objsize_8byte > MEMLENGTH*8){
                break;
            }
            if (currentNode->state == 0 && currentNode->size >= (objsize_8byte+8)){ // currentNode is free and is a large enough chunk to store objsize_8byte and the header
                if (currentNode->size == (objsize_8byte+8)){ // chunk is the exact needed size, just changes state to allocated
                
                    currentNode->state = 1;

                    ptr = (void*)(currentNode+1); // returns a void pointer at the start of the payload to the client
                    break;
                }
                
                old_size = currentNode->size;
                currentNode->state = 1;
                currentNode->size = objsize_8byte+8;


                MetaData* newnode = (MetaData*)((char*)currentNode+(currentNode->size)); // free node to be placed after previously allocated node
                
                newnode->state = 0;
                newnode->size = old_size - (objsize_8byte+8);

                
                node = 1;
                ptr = (void*)(currentNode+1);
                break;
                
            }else if(position + currentNode->size < MEMLENGTH * 8 ) {
                position += currentNode->size;
                currentNode = (MetaData*)((char*)currentNode + currentNode->size);
            } else {
                break;
            } 
        }
    }
    
    if(ptr == NULL) { // Reports objsize is to large to allocate
        printf("Error: Not enough memory to allocate %ld byte(s)\n", objsize);
    }
    return ptr;
}


void myfree(void *ptr){

    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;

    int wasFreed = 0;
    
    if (ptr == NULL){ // reports free of a NULL pointer was requested
        printf("Error: Cannot Deallocate NULL Pointer\n");
        return;
    }

    char* heap_start = (char*)memory; // char* pointer to the start of memory for pointer arithmatic

    MetaData* ptr_toFree = (MetaData*) ptr;

    


    MetaData* thisNode = (MetaData*)heap_start;
    MetaData* prev_node;
    int count = 0;

    
    while (thisNode->size != 0  && count < MEMLENGTH*8){ // iterates through memory in order to find the requested pointer
        if ((thisNode+1) == ptr_toFree){
            
            if (thisNode->state == 0)
                break;

            thisNode->state = 0;

            wasFreed = 1;

            if (((MetaData*)heap_start)->size+thisNode->size == MEMLENGTH*8){
                ((MetaData*)heap_start)->size = ((MetaData*)heap_start)->size + thisNode->size;
                break;
            }

            int tsize = 0;
            int tstate = 0;
            if(count + thisNode->size < MEMLENGTH * 8) {
                MetaData* tempNext = (MetaData*)(((char*)thisNode) + (thisNode->size));
                tsize = tempNext->size;
                tstate = tempNext->state;
            }
            

            if (thisNode == (MetaData*)heap_start && tstate == 0){
                thisNode->size = thisNode->size + tsize;
            }
            
            if (thisNode != (MetaData*)heap_start && prev_node->state == 0){ // coalesce free nodes to the left
                prev_node->size = prev_node->size + thisNode->size;
                if ((count+tsize)<= MEMLENGTH*8 && tstate == 0){
                    prev_node->size = prev_node->size + tsize;
                    count += tsize;
                }
            }
            
            

            if(count + thisNode->size <= MEMLENGTH * 8) {
                if (thisNode != (MetaData*)heap_start && prev_node->state == 1 && tstate ==0){ // coalesce free nodes to the right left
                    thisNode->size = thisNode->size +tsize;
                }

            }
        }
        if (count < MEMLENGTH*8)
            count += thisNode->size;
        if (count < MEMLENGTH*8){ // updates previous node
            prev_node = thisNode;
            thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));
        }
    }
    if (wasFreed == 0){ // reports address not found error
        printf("Error: Unable to free memory address\n");
    }
}


int memCleared() {
    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;
    
    int chunkSize = (((MetaData*) memory)->size);
    int free = ((MetaData*) memory)->state;

    if ((chunkSize == 0 && free == 0) || (chunkSize == MEMLENGTH*8 && free == 0 )) { // Case: Memory pool is uninitialized or fully cleared
        return 1;
    }

    return 0;
}