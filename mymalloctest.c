#include<stdio.h>
#include<stddef.h>
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

//Function creates errors that malloc and free need to be able to detect
void detecting_errors(){

    int x;
    free(&x);

    int *p = malloc(sizeof(int)*2);
    free(p+1);

    int *t = malloc(sizeof(int)*100);
    int *q = t;
    free(t);
    free(q);



    //clean memory for other test cases
    free(p);
}


//Fills array then frees all pointers two times
void malloc_test1(){
    int count = 0;
    char *ptrArray[256];

    while (count < 256){
        ptrArray[count] = malloc(1);
        //printf("%d\n",count);
        count++;
    }
    count = 0;
    while (count < 256){
        //printf("%d\n",count+1);
        free(ptrArray[count]);
        
        count++;
    }

    char *ptrArray2[256];
    count = 0;
    while (count < 256){
        ptrArray2[count] = malloc(1);
        //printf("%d\n",count);
        count++;
    }
    count = 0;
    while (count < 256){
        //printf("%d\n",count+1);
        free(ptrArray2[count]);
        
        count++;
    }

    printf("\nMemClear?: %d\n\n", memCleared());

}


//Tests for when client calls malloc with to many bytes
//Tests for when client calls malloc with 0 bytes
void malloc_test2(){
    void *ptr = malloc(999999999999999999);
    void *ptr2 = malloc(0);
}


//Tests for when client calls malloc with in range byte amount, but there isn't enough memory to allocate 
void malloc_test3(){
    void *ptr =malloc(4000);
    void *ptr2 = malloc(100);


    //clean memory
    free(ptr);
    free(ptr2);
}





int main(int argc, char** argv){
    detecting_errors();
    malloc_test1();
    malloc_test2();
    malloc_test3();
    

    return 0;
}