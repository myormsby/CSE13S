CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g
LDFLAGS = -g
EXEC = sorting
OBJS = sorting.o stats.o batcher.o heap.o insert.o quick.o

all: sorting

$(EXEC): $(OBJS)  
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[c,h]
