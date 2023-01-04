CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LDFLAGS = -g -lncurses
EXEC = io
OBJS = node.o code.o stack.o pq.o io.o

all: io

$(EXEC): $(OBJS)  
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[c,h]
