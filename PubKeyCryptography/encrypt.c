#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

void help(void) {
    printf("SUMMARY\n"
           "Encrypts a message from a file.\n"
           "\n"
           "USAGE\n"
           "[i:o:n:vh] [-i input file] [-o output file] [-n public key file]\n"
           "\n"
           "OPTIONS\n"
           "-i: specifies the input file for encryption (default: stdin)\n"
           "-o: specifies the output file for the encrypted message (default: stdout)\n"
           "-n: specifies the file containing the public key (default: rsa.pub)\n"
           "-v: enables verbose output\n"
           "-h: displays the program summary and usage\n");
}

int main(int argc, char **argv) {
    FILE *fpin = stdin;
    FILE *fpout = stdout;
    FILE *key = fopen("rsa.pub", "r");
    bool verbose = false;
    char user[200];
    int opt = 0;
    mpz_t n, e, s, username;
    mpz_inits(n, e, s, username, NULL);
    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i': fpin = fopen(optarg, "r"); break;
        case 'o': fpout = fopen(optarg, "w"); break;
        case 'n':
            fclose(key);
            key = fopen(optarg, "r");
            if (key == NULL) {
                printf("Error! Key file does not exist:\n");
                return 0;
            }
            break;
        case 'v': verbose = true; break;
        default: help(); break;
        }
    }
    rsa_read_pub(n, e, s, user, key);
    if (verbose == true) {
        fprintf(stderr, "user = %s\n", user);
        gmp_fprintf(stderr, "s (%zu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_fprintf(stderr, "n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    mpz_set_str(username, user, 62);
    if (rsa_verify(username, s, e, n) == false) {
        printf("Error! Signature could not be verified\n");
        return 0;
    }
    rsa_encrypt_file(fpin, fpout, n, e);
    fclose(fpin);
    fclose(fpout);
    fclose(key);
    mpz_clears(n, e, s, username, NULL);
    return 0;
}
