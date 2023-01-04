# Assignment 5 : Public Key Cryptography 

## Description
	Keygen will generate a private key and a public key to be used in encrypt and decrypt. Encrypt will then use the public key to encode a message and decrypt will decrypt the message.

## How to Build
	Using the Makefile in the same directory as the header files and c files, typing "make" or "make all" will compile encrypt, decrypt, and keygen.
	Typing "make encrypt" will only build encrypt, "make decrypt" will only build decrypt, and "make keygen" will only build keygen.

## How to Run
	
### Keygen
	./keygen
	Possible arguments include:
	-b bits: minimum number of bits needed for the public modulus
	-i iterations: number of iterations used for testing primes
	-n pbfile: specifies the public key file
	-d pvfile: specifies the private key file
	-s seed: specifies the seed for random state initialization
	-v: enables verbose output
	-h: displays the program's summary and usage

### Encrypt
	./encrypt
	Possible arguments include:
	-i input file: specifies the input file for encryption
	-o output file: specifies the output file for the encrypted message
	-n key file: specifies the file containing the public key
	-v: enables verbose output
	-h: displays the program's summary and usage

### Decrypt
	./decrypt
	Possible arguments include
	-i input file: specifies the input file for decryption
	-o output file: specifies the output file for the decrypted message
	-n key file: specifies the file containing the private key
	-v: enables verbose output
	-h: displays the program's summary and usage

## Cleaning
	Typing "make clean" on the command line will remove the files produced by the Makefile.

## Errors
	Typing ./keygen, ./encrypt, and ./decrypt without any file arguments will lead to nothing happening and the program running forever. 

