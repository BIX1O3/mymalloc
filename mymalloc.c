#include<studio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH]




double *malloc(size_t objsize){

    objsize = (objsize+7) & ~7; //ensures that the object size is a multiple of 8 using bitwise &

    /*typedef struct header{ //use a linklist for the header which contains the size of the chunk and free or allocated
        int chunk_size;
        int state;  //1 is allocated and 0 will be free
    }header;

    typedef struct metadata{
        size_t payload_size;

        struct metadata* next;

    };*/


}


void free(double *ptr){

}

