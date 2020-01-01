#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void rev_str(char str[], char s[], size_t start, size_t end) {
	char tmp;
	char tmps[16];
	//char strr[32];

	size_t start_s = start;
	size_t end_s = end;
	for (int i = 0; i < (end-start); i++) {
		printf("%02x", str[i]);
	}
	printf("\n");
	while (start < end) {
		// FIXME: fix end of hex string value
		tmp = str[start];
		//sprintf(tmps,"%02x",str[start]);
		//strcat(strr, tmps);
		//printf("tmps: %s\n", tmps);
		str[start] = str[end];
		//sprintf(tmps, "%02x", str[end]);
		//strcat(strr, tmps);
		//printf("tmps: %s\n", tmps);
		str[end] = tmp;
		start++;
		end--;
	}
	for (; start_s < end_s; start_s++) {
		sprintf(tmps, "%02x", str[start_s+1]);
		strcat(s, tmps);
	}
	printf("%s\n",s);
	//memcpy(s, strr, end*2);
}

void print_mpz(mpz_t m) {
	mpz_out_str(stdout, 16, m);
}

void clamp(mpz_t r, mpz_t mask) {
	// and r with the mask
	mpz_and(r, r, mask);
}

int main(){
	
	// declare and initialize mpz object
	mpz_t mask;
	mpz_init(mask);
	// set object to mask value
	mpz_set_str(mask, "0fffffffc0ffffffc0ffffffc0ffffff", 16);

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
	clamp(n, mask);

	// use mpz_mul_2exp(n, n, 1); to 

	// declare and initialize accumulator
	mpz_t acc;
	mpz_init(acc);
	// set value to zero
	mpz_set_str(acc,"0", 16);
	mpz_t p;
	mpz_init(p);
	mpz_set_str(p,"1",16);

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
	char ar[34];
	size_t x = ceil((int)(((float)sz/16)));
	for(size_t i = 0; i < x; i++) {
		strncpy(a, &buff[i*16], 16);
		//a[17] = 0x01;
		rev_str(a, ar, 0, 16);
		//a[17] = '\0';
		ar[0] = '1';
		printf("a:\n%s\n", ar);
		
		mpz_t a_var;
		mpz_init(a_var);
		mpz_set_str(a_var, ar, 16);	
		//FIXME: add/mul/mod functions do not seem to be computing
		// add block value to accumulator
		printf("\na_var:\n");
		print_mpz(a_var);
		
		mpz_add(acc, acc, a_var);
		
		printf("\n%s:\n", "acc");
		print_mpz(acc);	
		
		printf("\nn:\n");
		print_mpz(n);
		printf("\n");
		// multiply accumulated value by n
		mpz_mul(acc, acc, n);
		printf("%s:\n", "mul");
		
		mpz_mod(acc, acc, p);
		printf("\n%s:\n", "mod");
		print_mpz(acc);	
		// mod accumulated product by p
		//mpz_mod(acc, acc, p);
		//print_mpz(acc);
		// clear block value
		mpz_clear(a_var);
		printf("\nar:\n");
		for (int j = 0; j < 18; j++) {
			printf("%c", ar[j]);
		}
	}
	//mpz_clear(a_var);
	printf("\nacc:\n");
	print_mpz(acc);
	printf("\n");
	size_t end_sz = sz-(x*17);
	char b[end_sz];
	memcpy(b, &buff[x*16], end_sz);
	b[end_sz] = 0x01;
	char br[end_sz*2];
	rev_str(b, br, 0, end_sz);
	//b[end_sz+1] = '\0';
	mpz_t b_var;
	mpz_init(b_var);
	mpz_set_str(b_var, b, 16);
	printf("b_var:\n");
	print_mpz(b_var);
	for (int j = 0; j < end_sz+2; j++)
		printf("%02x", b[j]);
	printf("\n");
	
	mpz_t it;
	mpz_init_set_str(it, ar, 16);
	print_mpz(it);

	// deallocate all numbers
	mpz_clear(acc);
	mpz_clear(p);
	mpz_clear(n);
	mpz_clear(mask);
	return 0;
}
