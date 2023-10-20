// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#include<stdio.h>
#include<stddef.h>
#include<sys/time.h>
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

/*******************************************
 * TEST 1
 * malloc() and immediately free() a 1-byte object, 120 times.
 *******************************************/

void test1() {
    for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);  // Allocate 1 byte of memory
        free(ptr);  // Release the memory
    }
    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

/*******************************************
 * TEST 2
 * Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to
 * deallocate the chunks.
 *******************************************/

void test2() {
    char *ptrArray[120];  // Array to store 120 pointers

    for(int i = 0; i < 120; i++) {
        ptrArray[i] = malloc(1);  // Allocate 1 byte of memory and store the address
    }

    for(int i = 0; i < 120; i++) {
        free(ptrArray[i]);  // Release the memory
    }

   // printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

/*******************************************
 * TEST 3
 * Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
 * object and adding the pointer to the array and deallocating a previously allocated object (if
 * any), until you have allocated 120 times. Deallocate any remaining objects.
 *******************************************/

void test3() {
    char *ptrArray[120];  // Array to store 120 pointers
    int allocated[120] = {0};  // Initialize the memory allocation status array
    int loc = 0;  // Current location

    for(int i = 0; i < 120; i++) {
        if(loc == 0 || (rand() % 2 == 0 && loc < 120)) {
            // Allocate 1 byte of memory and store the address
            // printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
        } else {
            // Release the memory
            loc--;
            // printf("free loc=%d\n", loc);
            free(ptrArray[loc]);
            allocated[loc] = 0;
        }
    }

    //printf("Process is done.\n");

    // Clean up any unreleased memory
    for(int i = 0; i < 120; i++) {
        if(allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }

    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared
}

/*******************************************
 * TEST 4
 * Completely fill memory with 1-byte chunks, store a pointer to a struct that looks just like our metadata
 * to make sure we only read actual metadata and not user data. Completely empty memory, and then do the 
 * same thing again to show we can rewrite into freed data.
 *******************************************/

void test4() {

    typedef struct MetaData{
        int state;   
        int size;   
    }MetaData;

    void *ptrArray[256];  // Array to store 256 pointers
    
    for(int i = 0; i < 256; i++) { // fill memory with fake metadata
        ptrArray[i] = malloc(1);
        MetaData** ptr = (MetaData**) ptrArray[i];

        MetaData* fakeHeader;
        *ptr = fakeHeader;
    }

    for(int i = 0; i < 256; i++) {
        free(ptrArray[i]);  // Release the memory
    }

    for(int i = 0; i < 256; i++) { // fill memory with fake metadata
        ptrArray[i] = malloc(1);
        MetaData** ptr = (MetaData**) ptrArray[i];

        MetaData* fakeHeader;
        *ptr = fakeHeader;
    }

    for(int i = 0; i < 256; i++) {
        free(ptrArray[i]);  // Release the memory
    }

    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared


}

/*******************************************
 * TEST 5
 * Create an array of 120 pointers. Repeatedly make a random choice between allocating a random amount of bytes
 * and adding the pointer to the array or deallocating a random previously allocated object (if
 * any), until you have allocated 120 times. Deallocate any remaining objects.
 *******************************************/

void test5() {
     char *ptrArray[120];  // Array to store 120 pointers
    int allocated[120] = {0};  // Initialize the memory allocation status array
    int ptrs = 0;  // Current last pointer

    for(int i = 0; i < 120; i++) {
        if(ptrs == 0 || (rand() % 2 == 0 && ptrs < 120)) {
            // Allocate a random amount of bytes of memory and store the address
            //printf("alloc loc=%d\n", ptrs);
            int randSize = (rand() % 800) + 1;
            ptrArray[ptrs] = malloc(randSize);
            if(ptrArray[ptrs] != NULL) { // only update the pointer if we haven't reached the end of memory
                allocated[ptrs] = 1;
                ptrs++;
            }
            
        } else {
            int randPtr = (rand() % ptrs);
            while(allocated[randPtr] != 1) { // make sure something is allocated to deallocate
                randPtr = (rand() % ptrs);
            }
            
            if(randPtr == ptrs - 1) { // reduce the number of pointers to choose from based on the last allocated pointer
                ptrs--;
                while(ptrs != 0 && allocated[ptrs - 1] == 0) {
                    ptrs--;
                }
            }

            //printf("free loc=%d\n", randPtr);
            free(ptrArray[randPtr]);
            allocated[randPtr] = 0;
        }
    }

    // printf("Process is done.\n");

    // Clean up any unreleased memory
    for(int i = 0; i < 120; i++) {
        if(allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }

    //printf("MemClear?: %d\n", memCleared());  // Check if memory is cleared

}


int main (int argc, void** argv){
    
    struct timeval start, end;
    double sum = 0;
    
    
    for(int i = 0; i < 50; i++) {
        gettimeofday(&start, NULL);
        test1();
        gettimeofday(&end, NULL);
        sum += (end.tv_usec - start.tv_usec);
    }

    printf("Task 1 took %lf ms. \n", sum/50000);

    sum = 0;

    for(int i = 0; i < 50; i++) {
        gettimeofday(&start, NULL);
        test2();
        gettimeofday(&end, NULL);
        sum += (end.tv_usec - start.tv_usec);
    }

    printf("Task 2 took %lf ms. \n", sum/50000);

    sum = 0;
    
    for(int i = 0; i < 50; i++) {
        gettimeofday(&start, NULL);
        test3();
        gettimeofday(&end, NULL);
        sum += (end.tv_usec - start.tv_usec);
    }

    printf("Task 3 took %lf ms. \n", sum/50000);

    sum = 0;

    for(int i = 0; i < 50; i++) {
        gettimeofday(&start, NULL);
        test4();
        gettimeofday(&end, NULL);
        sum += (end.tv_usec - start.tv_usec);
    }

    printf("Task 4 took %lf ms. \n", sum/50000);

    sum = 0;

    for(int i = 0; i < 50; i++) {
        gettimeofday(&start, NULL);
        test5();
        gettimeofday(&end, NULL);
        sum += (end.tv_usec - start.tv_usec);
    }

    printf("Task 5 took %lf ms. \n", sum/50000);
    

    return 0;
}
