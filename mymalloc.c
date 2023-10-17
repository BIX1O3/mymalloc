#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];


void *mymalloc(size_t objsize){

    int objsize_8byte = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &
    //printf("obj: %d\n", objsize_8byte);
    typedef struct MetaData{ //If we want to have two ints we can use two short ints which are only 2 bytes each
        int state;  //free = 0 and allocated is set to 1 
        int size;   //points to next metadata block 
    }MetaData;

    
    char *heap_start = (char*)memory;
    MetaData* head = (MetaData*) memory;
    void *ptr;

    int node = 0;

    if ((*head).state == 0 && (*head).size == 0){
        head->state = 1;
        head->size = objsize_8byte+8;

        MetaData* newnode = (MetaData*)(heap_start+(head->size));
        newnode->state = 0;  //originally 0 so this is is redundant
        newnode->size = (MEMLENGTH * 8) - (head->size);
        //printf("1\n\n");
        ptr = (void*)(heap_start + 8);
    } else {
        MetaData* currentNode = (MetaData*)heap_start;
        int position = 0;
        //int node = 0;
        int old_size = 0;
        //printf("2.1\n");
        while(position < (MEMLENGTH*8) && node == 0) { //&& currentNode->state != 0 && currentNode->size != 0

            //printf("state: %d  size: %d", currentNode->state, currentNode->size);
            //printf("\tposition: %d\n", position);

            if (currentNode->state == 0 && currentNode->size >= (objsize_8byte+8)){
                //printf("O:%ld  O8:%d  C:%d", objsize, objsize_8byte, currentNode->size);

                if (currentNode->size == (objsize_8byte+8)){
                
                    currentNode->state = 1;

                    //printf("\n\nEqual:   %d %d\n", currentNode->state, currentNode->size);

                    //printf("mymalloc ptr: %p\n",ptr);
                    ptr = (void*)(currentNode+1);
                    break;
                }



                //printf("2.2\n\n");
                
                old_size = currentNode->size;
                currentNode->state = 1;
                currentNode->size = objsize_8byte+8;



                MetaData* newnode = (MetaData*)((char*)currentNode+(currentNode->size));
                printf("\n\nOldSize = %d\n",old_size);
                //printf("%d %d\n\n", currentNode->state, currentNode->size);
                
                newnode->state = 0;  //originally 0 so this is is redundant
                newnode->size = old_size - (objsize_8byte+8); // check to see if this logic works

                printf("\n\nNewSize = %d\n\n\n\n",newnode->size);

                node = 1;
                ptr = (void*)(currentNode+1);
                //printf("2\n\n");
            }else{
                //printf("3\n\n\n\n\n");
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
        //printf("runs\n");
        printf("%d %d \n", thisNode->state, thisNode->size);
        count += thisNode->size;
        printf("count: %d\n\n", count);
        if (count < 4096)    
            thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));

        //printf("\n%d %d\n", thisNode->state,  thisNode->size);
        
    }

    /*printf("first state: %d first size: %d\n", ((MetaData*)heap_start)->state, ((MetaData*)heap_start)->size);
    printf("Second state: %d Second size: %d\n", (((MetaData*)heap_start+(((MetaData*)heap_start)->size)))->state, ((MetaData*)(heap_start+((MetaData*)heap_start)->size))->size);

    if (node == 1){
        MetaData* thisNode = (MetaData*)(heap_start+(((MetaData*)heap_start)->size));

        MetaData* thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));
        

        printf("Third state: %d Third size: %d\n", here->state, here->size);
    }*/
    //printf("mymalloc ptr: %p\n",ptr);
    return ptr;
}


void myfree(void *ptr){

    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;
    


    char* heap_start = (char*)memory;

    MetaData* ptr_toFree = (MetaData*) ptr;

    MetaData* thisNode = (MetaData*)heap_start;
    MetaData* prev_node;
    int count = 0;

    /*printf("\n\nFree: %d %d\n",thisNode->state, thisNode->size);
    if (ptr_toFree == (thisNode+1)){
        printf("TRUE");
    }else{
        printf("FALSE\n%p\n%p\n%p\n%p\n",ptr_toFree, ptr, heap_start, (((char*)thisNode)+8));
    }*/

    while ((thisNode->size != 0 && thisNode->size !=0) && count < 4096){
        if ((thisNode+1) == ptr_toFree){
            thisNode->state = 0;
            printf("PTR Freed");
            MetaData* tempNext = (MetaData*)(((char*)thisNode) + (thisNode->size));
            if (prev_node->state == 0 && thisNode != (MetaData*)heap_start){
                //coalesce free nodes
                //printf("\nthis: %d %d\n", thisNode->state,thisNode->size);
                //printf("\nprev: %d %d\n", prev_node->state,prev_node->size);
                prev_node->size = prev_node->size + thisNode->size;
                //printf("\nprev: %d %d\n", prev_node->state,prev_node->size);
                //MetaData* tempNext = (MetaData*)(((char*)thisNode) + (thisNode->size));
                if (tempNext->state == 0 && (count+tempNext->size)< 4096){
                    prev_node->size = prev_node->size + tempNext->size;
                    count += tempNext->size;
                }
            }
            //MetaData* tempNext = (MetaData*)(((char*)thisNode) + (thisNode->size));
            if (prev_node->state == 1 && tempNext->state ==0){
                thisNode->size = thisNode->size +tempNext->size;
            }
        }
        if (count < 4096)
            count += thisNode->size;
        if (count < 4096){    
            //printf("\n\nPrevNode: %d\n",thisNode->state);
            prev_node = thisNode;
            //printf("\n\nthisNode: %p\n",thisNode);
            //printf("\n\nPrevNode: %p\n",prev_node);
            thisNode = (MetaData*)(((char*)thisNode) + (thisNode->size));
        }

    
    }

    MetaData* newHead = (MetaData*)heap_start;

    while ((newHead->size != 0 && newHead->size !=0) && count < 4096){
        printf("Free: %d %d \n", newHead->state, newHead->size);
        count += newHead->size;
        printf("Free count: %d\n\n", count);
        if (count < 4096)    
            newHead = (MetaData*)(((char*)newHead) + (newHead->size));
        
    }


}







int main(int argc, char** argv){

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
}