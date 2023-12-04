# My Little Malloc
## CS 214 Fall 2023

## Contributors:
### Matthew Bixby - mjb593
### Serge Kuznetsov - sak339

## How to run tests:

#### run 'make' to create memgrind and mymalloctest
#### run './mymalloctest' to correctness test
#### run './memgrind' to stress test

## Design Choices:

Header Block: We used a MetaData struct that stores the size of the chunk and the state of the chunk. 
              We used a pointer of type MetaData in order to quickly iterate through memory without ever accessing the payload.


Error Reporting:  We chose to print a detailed error statement and return without exiting the program. 
                  We chose this as it will allow for efficient testing as mymalloc and myfree returns before the fatal error is commited. 

memCleared: We created the function memCleared in order to aid testing whether memory has been initialized or if the entire memory block is one free chunk.

## Test Plan and Programs:

We have two files for testing: mymalloctest.c and memgrind.c.

mymalloctest.c provides a set of correctness tests to evaluate functionality during typical and edge cases of mymalloc() and myfree(). 
The code contains four distinct functions:

 TEST Mandatory MYMALLOC Error Reporting:
    Purpose: This test checks the error reporting capabilities of myfree() with three tests.
    Test 1: Calls free on an address not obtained by malloc. 
        Test 1 ensures that the library can detect and report an error when trying to free a memory address that was never allocated.
    Test 2: Calls free on an address that is not the start of the chunk. 
        Test 2 checks if the library can detect when a user tries to free memory from the middle of an allocated chunk, which is an error.
    Test 3: Calls free on an address for the second time. 
        Test 3 tests if the library can detect and report an error when trying to free an already freed memory address.

 TEST Memory Capacity:
    Purpose: This test evaluates the system's ability to handle memory capacity and coalescing.
    The function fills the memory with 256 1-byte malloc calls, then frees the memory 256 times. This checks if the library can handle multiple allocations up to its capacity.
    It then repeats the allocation and deallocation process to ensure that the library can handle repeated operations on an initialized memory.

 TEST Unacceptable Client Malloc Inputs:
    Purpose: This test checks how the library handles invalid or edge-case requests from the client.
    The test calls malloc with a request for 9999999 bytes, which is larger than the available memory. This checks that the library can detect and report an error
    when the request exceeds available memory.
    The test also calls malloc with a request for 0 bytes. This checks if the library can detect and report an error when the client requests no memory.
    
 TEST Malloc Call for Bytes that Memory is Unable to Allocate:
    Purpose: This test checks how the library handles requests that it cannot fulfill due to memory constraints.
    The test calls malloc with 4000 bytes and then with 100 bytes, purposefully going over the memory size. This ensures that the library can detect and 
    report an error when there's not enough memory left to fulfill a request.
    The test also calls free on a NULL pointer received from malloc. This checks if the library can detect and report an error when trying to deallocate a NULL pointer.

Together, these tests provide a comprehensive correctness test for mymalloc, as the tests cover error reporting for various invalid free and malloc calls, such as calling 
malloc when memory is already full or freeing a null pointer.

memgrind.c provides a set of stress tests to evaluate the performance and functionality of mymalloc() and myfree(). The code contains five distinct tests:

Test 1: Allocates and immediately frees a 1-byte object, 120 times.
    Purpose: By repeatedly allocating and then immediately freeing memory, Test 1 checks that the memory management system
             can handle rapid, back-to-back operations without any hitches.

Test 2: Allocates 120 1-byte objects, storing the pointers in an array, and then deallocates the chunks.
    Purpose: Test 2 evaluates the mymalloc's ability to handle multiple concurrent allocations and then deallocate them all at once.
             It's a test of both the system's capacity and its ability to manage multiple memory chunks.

Test 3: Repeatedly makes a random choice between allocating a 1-byte object and deallocating a previously 
        allocated object (if any), until 120 allocations have been made. It then deallocates any remaining objects.
    Purpose: Test 3 introduces randomness, simulating a more real-world scenario where allocations and deallocations might
             not follow a predictable pattern. It checks the system's adaptability to varying memory operations.

Test 4: Completely fills memory with 1-byte chunks, storing a pointer to a struct that mimics metadata. It then completely empties the memory and 
        repeats the process.
    Purpose: Test 4 is a boundary test. By filling the memory entirely, it checks how the system behaves at its capacity limits. The use of
             fake metadata tests if the memory management system only reads actual metadata and not user data. Repeating the process tests
             the system's ability to reclaim and reuse memory that has been freed.

Test 5: Repeatedly makes a random choice between allocating a random amount of bytes (between 1 and 200) and deallocating a
        random previously allocated object (if any), until 120 allocations have been made. It then deallocates any remaining objects.
    Purpose: Test 5 introduces variability in the size of the memory chunks being allocated. It simulates a more complex real-world 
             scenario where different sizes of memory chunks are requested. By also introducing randomness in the deallocation process,
             it further stresses the system's ability to manage memory efficiently and coalesce.

Upon execution, the program runs each of the five tests 50 times, measuring the time taken for each test and printing the average time in milliseconds. 

Together, these tests provide a comprehensive stress test for mymalloc, ensuring that it can handle a wide range of scenarios and loads,
as the tests cover variability in allocation and memory sizing, consistency, and reusing freed memory.
