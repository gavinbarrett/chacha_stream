#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef POLY1305_H
#define POLY1305_H

typedef struct PolyAuth PolyAuth;
void print_mpz(mpz_t);
void clamp(mpz_t r, mpz_t mask);
void initialize_p(mpz_t p);
PolyAuth setup();
void destroy(PolyAuth* auth);
void rev_str(char str[], char s[], size_t start, size_t end);
void num_to_le_bytes(mpz_t acc, char output[]);
void compute_cycle(PolyAuth* auth);
void poly1305_mac();

#endif // POLY1305_H
