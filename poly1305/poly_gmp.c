#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

void print_mpz(mpz_t m) {
	mpz_out_str(stdout, 16, m);
}

void clamp(mpz_t r) {
	// declare and initialize mpz object
	mpz_t mask;
	mpz_init(mask);
	// set object to mask value
	mpz_set_str(mask, "0fffffffc0ffffffc0ffffffc0ffffff", 16);
	// and r with the mask
	mpz_and(r, r, mask);
	mpz_clear(mask);
}

int main(){

	mpz_t n;
	size_t sz;

	char buff[20] = "Hello there darling";
	sz = sizeof(buff);

	mpz_init(n);
	// set var r to first 128 bits of the key
	mpz_set_str(n, "a806d542fe52447f336d555778bed685", 16);
	
	print_mpz(n);
	printf("\n");
	
	// clamp n
	clamp(n);

	// use mpz_mul_2exp(n, n, 1); to 

	// declare and initialize accumulator
	mpz_t acc;
	mpz_init(acc);
	// set value to zero
	mpz_set_ui(acc,0);
	mpz_t p;
	mpz_init(p);
	mpz_set_ui(p,1);

	// set p to (1 << 130)
	mpz_mul_2exp(p, p, 130);
	// set f to constant 5 for computing p
	mpz_t f;
	mpz_init(f);
	mpz_set_ui(f,5);
	// p -= 5
	mpz_sub(p, p, f);
	print_mpz(p);
	printf("\n");
	char a[17];
	size_t x = ceil((int)(((float)sz/16)));
	for(size_t i = 0; i < x; i++) {
		strncpy(a, &buff[i*16], 16);
		a[16] = 0x01;
		for (int j = 0; j < 17; j++) {
			printf("%02x ", a[j]);
		}
	}
	//printf("%s\n", a);		
	//printf("%zu\n", x);
	//printf("sz: %zu\n", sz);
	
	size_t end_sz = sz-(x*17);
	char b[end_sz+1];
	memcpy(b, &buff[x*16], end_sz);
	b[end_sz] = 0x01;
	for (int j = 0; j < end_sz+1; j++) {
		printf("%02x ", b[j]);
	}
	printf("\n");


	// deallocate all numbers
	mpz_clear(acc);
	mpz_clear(p);
	mpz_clear(n);
	return 0;
}
