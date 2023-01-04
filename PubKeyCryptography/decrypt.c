#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

void help(void) {
    printf("SUMMARY\n"
           "Decrypts a message from a file.\n"
           "\n"
           "USAGE\n"
           "[i:o:n:vh] [-i input file] [-o output file] [-n public key file]\n"
           "\n"
           "OPTIONS\n"
           "-i: specifies the input file for decryption (default: stdin)\n"
           "-o: specifies the output file for the decrypted message (default: stdout)\n"
           "-n: specifies the file containing the private key (default: rsa.priv)\n"
           "-v: enables verbose output\n"
           "-h: displays the program summary and usage\n");
}

int main(int argc, char **argv) {
    FILE *fpin = stdin;
    FILE *fpout = stdout;
    FILE *key = fopen("rsa.priv", "r");
    bool verbose = false;
    int opt = 0;
    mpz_t n, e;
    mpz_inits(n, e, NULL);
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
    rsa_read_priv(n, e, key);
    if (verbose == true) {
        fprintf(stderr, "n (%zu bits) = ", mpz_sizeinbase(n, 2));
        gmp_fprintf(stderr, "%Zd\n", n);
        fprintf(stderr, "d (%zu bits) = ", mpz_sizeinbase(e, 2));
        gmp_fprintf(stderr, "%Zd\n", e);
    }
    rsa_decrypt_file(fpin, fpout, n, e);
    fclose(fpin);
    fclose(fpout);
    fclose(key);
    mpz_clears(n, e, NULL);
    return 0;
}
