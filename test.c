#include<stdio.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];

void *mallac(int objsize){

    typedef struct MetaData{
        int state;  
        struct MetaData *next;
    }MetaData;
    
    
    
    /*MetaData* current = (MetaData*)memory;

    //short len = 4*MEMLENGTH;

    //short isFree = -1;


    if ((*(current)).state == 0){
        printf("%d\n",current->state);
        //printf("%d\n",(*(current)).next->state);
        if ((*(current)).next == NULL){
            printf("NULL");
        }
    }
    else{
        printf("YAY!!!\n");
    }*/


    MetaData* head = (MetaData*)memory;

    //short len = 4*MEMLENGTH;

    //short isFree = -1;

    printf("%d\n", head->state);

    if ((*head).state != -1 && (*head).state != 1){
        MetaData* newnode = (MetaData*)(memory+ objsize);
        newnode->state = -1;
        newnode->next = NULL;
        head->state = 1;
        head->next = newnode;

    }


    printf("%d\n",head->state);
    printf("%d\n",head->next->state);


    void *r = (void*)memory;

    return r;
}

int main(int argc, char** argv){
   void *p = mallac(12);
    void *p2 = mallac(12);  
    printf("%p %p",p,p2);

    return 0;
}