#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>




#define MEMLENGTH 512
static double memory[MEMLENGTH];


void *mymalloc(size_t objsize){

    if(objsize == 0) {
        
        printf("Error: Cannot allocate 0 bytes");
        return NULL;
    }

    int objsize_8byte = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &
    
    typedef struct MetaData{ //If we want to have two ints we can use two short ints which are only 2 bytes each
        int state;  //free = 0 and allocated is set to 1 
        int size;   //points to next metadata block 
    }MetaData;

    if(objsize_8byte > MEMLENGTH - 8) {return NULL;}
    

    char *heap_start = (char*)memory;
    MetaData* head = (MetaData*) memory;
    void *ptr = NULL;

    int node = 0;

    if ((*head).state == 0 && (*head).size == 0){
        head->state = 1;
        head->size = objsize_8byte+8;

        MetaData* newnode = (MetaData*)(heap_start+(head->size));
        newnode->state = 0;  //originally 0 so this is is redundant
        newnode->size = (MEMLENGTH * 8) - (head->size);
        
        ptr = (void*)(heap_start + 8);
    } else {
        MetaData* currentNode = (MetaData*)heap_start;
        int position = 0;
        
        int old_size = 0;
        
        while(position < (MEMLENGTH*8) && node == 0) { //&& currentNode->state != 0 && currentNode->size != 0

            //printf("state: %d  size: %d", currentNode->state, currentNode->size);
            //printf("\tposition: %d\n", position);

            if (currentNode->state == 0 && currentNode->size >= (objsize_8byte+8)){

                if (currentNode->size == (objsize_8byte+8)){
                
                    currentNode->state = 1;

                    ptr = (void*)(currentNode+1);
                    break;
                }
                
                old_size = currentNode->size;
                currentNode->state = 1;
                currentNode->size = objsize_8byte+8;


                MetaData* newnode = (MetaData*)((char*)currentNode+(currentNode->size));
                //printf("\n\nOldSize = %d\n",old_size);  // uncomment
                
                
                newnode->state = 0;  //originally 0 so this is is redundant
                newnode->size = old_size - (objsize_8byte+8); // check to see if this logic works

                //printf("\n\nNewSize = %d\n\n\n\n",newnode->size);     // uncomment

                node = 1;
                ptr = (void*)(currentNode+1);
                
            }else{
                currentNode = (MetaData*)((char*)currentNode + currentNode->size);
            }
            position += currentNode->size;
        }
    }
    //printf("5\n\n");
    //debugging loop checks the size of all of the nodes
    MetaData* thisNode = (MetaData*)heap_start;
    int count = 0;
    while ((thisNode->size != 0 && thisNode->size !=0) && count < 4096){
        
        //printf("%d %d \n", thisNode->state, thisNode->size);               // uncomment
        count += thisNode->size;
        //printf("count: %d\n\n", count);                                    // uncomment
        if (count < 4096)    
            thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));

        
        
    }


    if(ptr == NULL) {
        printf("Error: Not enough memory to allocate %ld.", objsize);
    }
    return ptr;
}


void myfree(void *ptr){

    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;

    
    
    if (ptr == NULL){
        printf("Error: Cannot Deallocate NULL Pointer");
        return;
    }

    char* heap_start = (char*)memory;

    MetaData* ptr_toFree = (MetaData*) ptr;

    //printf("ptr: %d %d", ((MetaData*)((char*)ptr-8))->state, ((MetaData*)((char*)ptr-8))->size); //uncomment


    MetaData* thisNode = (MetaData*)heap_start;
    MetaData* prev_node;
    int count = 0;

    /*printf("\n\nFree: %d %d\n",thisNode->state, thisNode->size);
    if (ptr_toFree == (thisNode+1)){
        printf("TRUE");
    }else{
        printf("FALSE\n%p\n%p\n%p\n%p\n",ptr_toFree, ptr, heap_start, (((char*)thisNode)+8));
    }*/

    

    while (thisNode->size != 0  && count < 4096){
        if ((thisNode+1) == ptr_toFree){
            thisNode->state = 0;
            //printf("\nPTR Freed  thisNode: %d %d\n\n", thisNode->state, thisNode->size);  // uncomment
            
            MetaData* tempNext = (MetaData*)(((char*)thisNode) + (thisNode->size));

            if (thisNode == (MetaData*)heap_start && tempNext->state ==0){
                thisNode->size = thisNode->size + tempNext->size;
            }

            if (thisNode != (MetaData*)heap_start && prev_node->state == 0){
                //coalesce free nodes
                prev_node->size = prev_node->size + thisNode->size;
                
                if (tempNext->state == 0 && (count+tempNext->size)< 4096){
                    prev_node->size = prev_node->size + tempNext->size;
                    count += tempNext->size;
                }
            }
            
            if (thisNode != (MetaData*)heap_start && prev_node->state == 1 && tempNext->state ==0){
                thisNode->size = thisNode->size +tempNext->size;
            }
        }
        if (count < 4096)
            count += thisNode->size;
        if (count < 4096){
            prev_node = thisNode;
            thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));
        }

    
    }

    MetaData* newHead = (MetaData*)heap_start;

    while ((newHead->size != 0 && newHead->size !=0) && count < 4096){
        //printf("Free: %d %d \n", newHead->state, newHead->size);  // uncomment
        count += newHead->size;
        //printf("Free count: %d\n\n", count);  // uncomment
        if (count < 4096)    
            newHead = (MetaData*)(((char*)newHead) + (newHead->size));
        
    }


}


int memCleared() {
    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;
    
    int chunkSize = (((MetaData*) memory)->size);
    int free = ((MetaData*) memory)->state;

    // Case: Memory pool is uninitialized or fully cleared
    if ((chunkSize == 0 && free == 0) || (chunkSize == 4096 && free == 0 )) {
        return 1;
    }

    return 0;
}




/*int main(int argc, char** argv){

    char* ptr = (char*)mymalloc(25);

    printf("ptr = %p\n\n\n",ptr);
    //printf("\n\n\n\n\n");
    
    
    //myfree(ptr);

    
    int* ptr2 = (int*)mymalloc(120);

    printf("ptr = %p\n\n\n",ptr2);

    
    

    //printf("a2=%d  ptr2 = %p\n\n",*ptr2, ptr2);
    printf("\n\n\n\n\n");
    int* ptr3 = (int*)mymalloc(16);


    printf("ptr = %p\n\n\n",ptr3);

    //myfree();
    myfree(ptr2);
    myfree(ptr);

    //printf("\n\n\n\n\n");
    int* ptr4 = (int*)mymalloc(500);


    printf("ptr = %p\n\n\n",ptr4);

    //printf("a3=%d  ptr3 = %p\n\n",*ptr3, ptr3);

    return 0;
}*/