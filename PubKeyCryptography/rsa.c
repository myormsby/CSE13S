#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include "numtheory.h"
#include "randstate.h"

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t lcm, pt, qt, gc, ni, pbit, large;
    mpz_inits(lcm, pt, qt, gc, ni, pbit, large, NULL);
    uint64_t random_large = nbits / 2 + 1;
    mpz_set_ui(large, random_large);
    mpz_urandomm(pbit, state, large);
    uint64_t pbits = mpz_get_ui(pbit) + (nbits / 4);
    uint64_t qbits = nbits - pbits;
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_sub_ui(pt, p, 1);
    mpz_sub_ui(qt, q, 1);
    gcd(gc, pt, qt);
    mpz_mul(n, p, q);
    mpz_mul(ni, pt, qt);
    mpz_fdiv_q(lcm, ni, gc);
    do {
        mpz_urandomb(e, state, nbits);
        gcd(gc, e, lcm);
    } while (mpz_cmp_ui(gc, 1) != 0);
    mpz_clears(lcm, pt, qt, gc, ni, pbit, large, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%ZX\n", n);
    gmp_fscanf(pbfile, "%ZX\n", e);
    gmp_fscanf(pbfile, "%ZX\n", s);
    gmp_fscanf(pbfile, "%s", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t pt, qt, n, lcm, gc;
    mpz_inits(pt, qt, n, lcm, gc, NULL);
    mpz_sub_ui(pt, p, 1);
    mpz_sub_ui(qt, q, 1);
    mpz_mul(n, pt, qt);
    gcd(gc, pt, qt);
    mpz_fdiv_q(lcm, n, gc);
    mod_inverse(d, e, lcm);
    mpz_clears(pt, qt, n, lcm, gc, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%ZX\n", n);
    gmp_fscanf(pvfile, "%ZX\n", d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t k, j;
    k = mpz_sizeinbase(n, 2);
    k -= 1;
    k /= 8;
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));
    while (!feof(infile)) {
        block[0] = 0xFF;
        j = fread(block + 1, 1, k - 1, infile);
        if (j != 0) {
            mpz_import(m, j + 1, 1, 1, 1, 0, block);
            rsa_encrypt(c, m, e, n);
            gmp_fprintf(outfile, "%Zx\n", c);
            mpz_set_ui(c, 0);
        }
    }
    free(block);
    mpz_clears(m, c, NULL); // encrypt
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t c, m, o;
    mpz_inits(c, m, o, NULL);
    uint64_t k, j;
    k = mpz_sizeinbase(n, 2);
    k -= 1;
    k /= 8;
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));
    block[0] = 0xFF;
    while (!feof(infile)) {
        gmp_fscanf(infile, "%Zx\n", c);
        rsa_decrypt(m, c, d, n);
        mpz_export(block, &j, 1, 1, 1, 0, m);
        fwrite(block + 1, 1, j - 1, outfile);
        mpz_set_ui(m, 0);
    }
    free(block);
    mpz_clears(c, m, o, NULL);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
