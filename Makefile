CC = gcc
CFLAGS = -g -fsanitize=address,undefined
MYMALLOC_SRC = mymalloc.c
MYMALLOC_OBJ = $(MYMALLOC_SRC:.c=.o)
SRCS = memgrind.c mymalloctest.c
TARGETS = memgrind mymalloctest

all: $(TARGETS)

memgrind: memgrind.o $(MYMALLOC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

mymalloctest: mymalloctest.o $(MYMALLOC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

memtest: memtest.o $(MYMALLOC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) $(MYMALLOC_OBJ) $(wildcard *.o)
