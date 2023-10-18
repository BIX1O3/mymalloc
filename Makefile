CC = gcc
CFLAGS = -g -fsanitize=address,undefined -Wall

TARGET = memgrind
SRCS = memgrind.c mymalloc.c
OBJS = $(SRCS:.c=.o)



all:$(TARGET)



$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

