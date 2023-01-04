#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

void help(void) {
    printf("SUMMARY\n"
           "Creates a public key and a private key.\n"
           "\n"
           "USAGE\n"
           "[b:i:n:d:s:vh][-b bits] [-i iterations] [-n pbfile] [-d pvfile] [-s seed]\n"
           "\n"
           "OPTIONS\n"
           "-b: minimum number of bits needed for the public modulus\n"
           "-i: number of iterations to be used for testing primes (default 50)\n"
           "-n pbfile: specifies the public key file (default rsa.pub)\n"
           "-d pvfile: specifies the private key file (default rsa.priv)\n"
           "-s: specifies the random seed for random state initialization (default seconds since "
           "UNIX epoch)\n"
           "-v: enables verbose output\n"
           "-h: displays the program's summary and usage\n");
}

int main(int argc, char **argv) {
    uint64_t bits, iters, seed, opt, filen;
    iters = 50;
    FILE *pubfile;
    FILE *privfile;
    bool verbose = false;
    char *user;
    seed = time(NULL);
    mpz_t p, q, n, e, s, d, username;
    opt = 0;
    bits = 256;
    mpz_inits(p, q, n, e, s, d, username, NULL);
    pubfile = fopen("rsa.pub", "w");
    privfile = fopen("rsa.priv", "w");
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': bits = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'n':
            fclose(pubfile);
            pubfile = fopen(optarg, "w");
            if (pubfile == NULL) {
                printf("Error! Public key file does not exist!\n");
                return 0;
            }
            break;
        case 'd':
            fclose(privfile);
            privfile = fopen(optarg, "w");
            if (privfile == NULL) {
                printf("Error! Private key file does not exist!\n");
                return 0;
            }
            break;
        case 's': seed = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'v': verbose = true; break;
        default: help(); break;
        }
    }
    filen = fileno(privfile);
    fchmod(filen, 0600);
    randstate_init(seed);
    rsa_make_pub(p, q, n, e, bits, iters);
    rsa_make_priv(d, e, p, q);
    user = getenv("USER");
    mpz_set_str(username, user, 62);
    rsa_sign(s, username, d, n);
    rsa_write_pub(n, e, s, user, pubfile);
    rsa_write_priv(n, d, privfile);
    if (verbose == true) {
        gmp_fprintf(stderr, "user = %s\n", user);
        gmp_fprintf(stderr, "s (%zu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_fprintf(stderr, "p (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_fprintf(stderr, "q (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_fprintf(stderr, "n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_fprintf(stderr, "d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    fclose(pubfile);
    fclose(privfile);
    randstate_clear();
    mpz_clears(p, q, n, e, s, d, username, NULL);
    return 0;
}
