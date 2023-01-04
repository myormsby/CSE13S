CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LDFLAGS = -g -lgmp -ggdb3
KEYGEN_OBJS = numtheory.o randstate.o rsa.o keygen.o
ENCRYPT_OBJS = numtheory.o randstate.o rsa.o encrypt.o
DECRYPT_OBJS = numtheory.o randstate.o rsa.o decrypt.o

all: encrypt decrypt keygen

keygen: $(KEYGEN_OBJS) 
	$(CC) $(LDFLAGS) -o keygen $(KEYGEN_OBJS) 

encrypt: $(ENCRYPT_OBJS) 
	$(CC) $(LDFLAGS) -o encrypt $(ENCRYPT_OBJS) 

decrypt: $(DECRYPT_OBJS) 
	$(CC) $(LDFLAGS) -o decrypt $(DECRYPT_OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f keygen encrypt decrypt $(KEYGEN_OBJS) $(ENCRYPT_OBJS) $(DECRYPT_OBJS)

format:
	clang-format -i -style=file *.[c,h]
