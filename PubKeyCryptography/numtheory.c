#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t temp, at, bt;
    mpz_inits(temp, at, bt, NULL);
    mpz_set(at, a);
    mpz_set(bt, b);
    while (mpz_sgn(b) != 0) {
        mpz_set(temp, b);
        mpz_mod(b, a, b);
        mpz_set(a, temp);
    }
    mpz_set(d, a);
    mpz_set(a, at);
    mpz_set(b, bt);
    mpz_clears(temp, at, bt, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t a, test, exp, basei, modi;
    mpz_inits(a, test, exp, basei, modi, NULL);
    mpz_set(basei, base);
    mpz_set_ui(out, 1);
    mpz_set(modi, modulus);
    mpz_set(exp, exponent);
    mpz_set(a, basei);
    while (mpz_cmp_ui(exp, 0) > 0) {
        mpz_mod_ui(test, exp, 2);
        if ((mpz_cmp_ui(test, 1)) == 0) {
            mpz_mul(out, out, a);
            mpz_mod(out, out, modi);
        }
        mpz_mul(a, a, a);
        mpz_mod(a, a, modi);
        mpz_fdiv_q_ui(exp, exp, 2);
    }
    mpz_clears(a, test, exp, basei, modi, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t f, two, s, r, a, y, j, ni, si;
    mpz_inits(f, two, s, r, a, y, j, ni, si, NULL);
    mpz_set_ui(s, 0);
    mpz_set_ui(two, 2);
    mpz_sub_ui(r, n, 1);
    mpz_sub_ui(ni, n, 2);
    while (mpz_divisible_ui_p(r, 2) != 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
        ;
    }
    mpz_sub_ui(si, s, 1);
    uint64_t i;
    for (i = 1; i < iters; i++) {
        mpz_urandomm(a, state, ni);
        if (mpz_cmp_ui(a, 2) < 0) {
            mpz_add_ui(a, a, 2);
        }
        pow_mod(y, a, r, n);
        mpz_sub_ui(f, n, 1);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, f) != 0)) {
            mpz_set_ui(j, 1);
            while ((mpz_cmp(j, si) <= 0) && (mpz_cmp(y, f) != 0)) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(f, two, s, si, r, y, a, j);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, f) != 0) {
                mpz_clears(f, two, s, r, a, y, j, ni, si, NULL);
                return false;
            }
        }
    }
    mpz_clears(f, two, s, r, a, y, j, ni, si, NULL);
    return true;
}

void make_prime(mpz_t out, uint64_t bits, uint64_t iters) {
    mpz_t min, one, new_prime;
    mpz_inits(min, one, new_prime, NULL);
    mpz_set_ui(one, 1);
    mpz_mul_2exp(min, one, bits);
    do {
        mpz_urandomb(out, state, bits);
        mpz_add(new_prime, out, min);
        mpz_set(out, new_prime);

    } while (is_prime(out, iters) == false); // >:(
    mpz_clears(min, one, new_prime, NULL);
}

void mod_inverse(mpz_t out, mpz_t a, mpz_t n) {
    mpz_t r, rp, t, tp, q, tmp, tmpt;
    mpz_inits(r, rp, t, tp, q, tmp, tmpt, NULL);
    mpz_set(r, n);
    mpz_set(rp, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(tp, 1);
    while (mpz_cmp_ui(rp, 0) != 0) {
        mpz_fdiv_q(q, r, rp);
        mpz_set(tmp, rp);
        mpz_set(tmpt, tp);
        mpz_mul(rp, rp, q);
        mpz_sub(rp, r, rp);
        mpz_mul(tp, tp, q);
        mpz_sub(tp, t, tp);
        mpz_set(r, tmp);
        mpz_set(t, tmpt);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set(out, 0);
        mpz_clears(r, rp, t, tp, q, tmp, tmpt);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(out, t);
    mpz_clears(r, rp, t, tp, q, tmp, tmpt);
}
