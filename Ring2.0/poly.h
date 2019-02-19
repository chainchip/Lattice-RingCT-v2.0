#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

/* 
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1] 
 */
typedef struct {
	uint16_t coeffs[NEWHOPE_N];
} poly

#if !defined(_WIN32)
    __attribute__((aligned(32)));
#else
    ;
#endif

uint16_t coeff_freeze(uint16_t x);
uint16_t coeff_freeze2Q(uint16_t x);
void poly_init(poly *r);
void poly_setValue(poly *r, uint16_t v);
void poly_uniform(poly *a, const unsigned char *seed);
void poly_sample(poly *r, const unsigned char *seed, unsigned char nonce);
void poly_add(poly *r, const poly *a, const poly *b);

void poly_ntt(poly *r);
void poly_invntt(poly *r);
void poly_mul_pointwise(poly *r, const poly *a, const poly *b);

void poly_frombytes(poly *r, const unsigned char *a);
void poly_tobytes(unsigned char *r, const poly *p);
void poly_compress(unsigned char *r, const poly *p);
void poly_decompress(poly *r, const unsigned char *a);

void poly_frommsg(poly *r, const unsigned char *msg);
void poly_tomsg(unsigned char *msg, const poly *x);
void poly_sub(poly *r, const poly *a, const poly *b);

void poly_print(const poly *r);
int poly_equal(const poly *a, const poly *b);
//
void poly_constmul(poly *r, const poly *a, uint16_t cof);
void poly_serial(poly *r);
void poly_cofcopy(poly *des, poly *sour);
void poly_copy(poly *des, poly *sour, size_t mLen);
void poly_shift(poly *des, const poly *r, int iNumber);
#endif
