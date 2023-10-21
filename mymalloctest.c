#include<stdio.h>
#include<stddef.h>
#ifndef REALMALLOC
#include "mymalloc.h"
#endif



/*******************************************
 * TEST Mandatory MYMALLOC Error Reporting
 * Calls free on an address not obtained by malloc
 * Calls free on an address that is not the start of the chunk
 * Calls free on an address for the second time
 *******************************************/
void detecting_errors(){
    printf("Function tests mymalloc's error reporting\n\nTest 1: Calls free on an address not obtained by malloc\nExpected Output: \"Error: Unable to free memory address\"\n");

    int x;
    free(&x);

    printf("\nTest 2: Calls free on an address that is not the start of the chunk\nExpected Output: \"Error: Unable to free memory address\"\n");
    int *p = malloc(sizeof(int)*2);
    free(p+1);

    printf("\nTest 3: Calls free on an address for the second time\nExpected Output: \"Error: Unable to free memory address\"\n");
    int *t = malloc(sizeof(int)*100);
    int *q = t;
    free(t);
    free(q);



    
    free(p); //clean memory for other test cases
}


/*******************************************
 * TEST Memory capicity
 * Calls 256 1 byte malloc calls to fill memory
 * Calls on the 256 pointer to ensure proper coalescing 
 * Repeats malloc and free calls to test on an initialized memory
 *******************************************/
void malloc_test1(){
    printf("\nFunction fills memory with 256 1 byte malloc calls then frees 256 times, then repeats\nExpected Output: \"MemClear?: 1\"\n");

    int count = 0;
    char *ptrArray[256];

    while (count < 256){
        ptrArray[count] = malloc(1);
        count++;
    }
    count = 0;
    while (count < 256){
        free(ptrArray[count]);
        
        count++;
    }

    char *ptrArray2[256];
    count = 0;
    while (count < 256){
        ptrArray2[count] = malloc(1);
        count++;
    }
    count = 0;
    while (count < 256){
        free(ptrArray2[count]);
        count++;
    }

    printf("MemClear?: %d\n", memCleared());
}


/*******************************************
 * TEST Unexptable client malloc inputs
 * Calls malloc with 9999999 bytes to test for malloc client requests larger than memory
 * Calls malloc with 0 bytes to test for malloc client requests for no bytes 
 *******************************************/
void malloc_test2(){
    printf("\nFunction tests client request for 0 bytes and more bytes than memory\nExpected Output: \"Error: Cannot allocate 9999999 byte(s)\"\nExpected Output: \"Error: Cannot allocate 0 bytes\"\n");  
    void *ptr = malloc(9999999);
    void *ptr2 = malloc(0);
}


/*******************************************
 * TEST malloc call for bytes that memory is unable to allocate
 * Calls malloc 4000 bytes and then with 100 bytes to purposefully go over the memory size
 * Calls free on a NULL pointer recieved from malloc
 *******************************************/
void malloc_test3(){
    printf("\nFunction tests client request for more bytes than the memory can allocate for\nExpected Output: \"Error: Not enough memory to allocate 100 byte(s)\"\nExpected Output: \"Error: Cannot Deallocate NULL Pointer\"\n");  
    void *ptr =malloc(4000);
    void *ptr2 = malloc(100); //purposefully calls malloc with too many bytes

    free(ptr2); //purposefully free a NULL ptr returned by malloc
    



    free(ptr); //clean memory
}


int main(int argc, char** argv){
    detecting_errors();
    malloc_test1();
    malloc_test2();
    malloc_test3();
    return 0;
}