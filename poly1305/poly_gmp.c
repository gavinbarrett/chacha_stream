#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void rev_str(char str[], char s[], size_t start, size_t end) {
	char tmp;
	char tmps[end];
	tmps[0] = '\0';
	size_t start_s = start;
	size_t end_s = end;
	str[end-1] = 0x01;
	while (start < end) {
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
		start++;
		end--;
	}
	// FIXME: refactor into two functions
	printf("\n");
	for (; start_s < end_s; start_s++) {
		printf("%02x ", str[start_s+1]);
		sprintf(tmps, "%02x", str[start_s+1]);
		printf("\ntmps:\n%s\n", tmps);
		strcat(s, tmps);
	}
	printf("\n");
}

void num_to_le_bytes(mpz_t acc) {
	char output[34];
	char tmp1;
	char tmp2;
	// copy the hex string into the output buffer
	mpz_get_str(output, 16, acc);

	printf("\ns:\n%s\n", output);
	printf("\ns:\n%s\n", output);
	int start = 1;
	int end = 32;
	while (start < end-1) {
		tmp1 = output[start];
		tmp2 = output[start+1];
		output[start] = output[end-1];
		output[start+1] = output[end];
		output[end-1] = tmp1;
		output[end] = tmp2;
		start+=2;
		end-=2;
	}
	char* out = output;
	
	out++;
	
	// print the message tag
	printf("\ns:\n%s\n", out);
}

void initialize_p(mpz_t p) {
	mpz_init_set_ui(p,1);
	// set p to (1 << 130)
	mpz_mul_2exp(p,p,130);
	// set f to constant 5 for computing p
	mpz_t f;
	mpz_init_set_ui(f,5);
	// p -= 5
	mpz_sub(p, p, f);
	// clear tmp variables
	mpz_clear(f);
}

void print_mpz(mpz_t m) {
	mpz_out_str(stdout, 16, m);
}

void compute_cycle(mpz_t acc, mpz_t n, mpz_t r, mpz_t p) {
	mpz_add(acc, acc, n);
	printf("\n(Acc+Block):\n");
	print_mpz(acc);
	printf("\n");
	mpz_mul(acc, acc, r);
	printf("\n(Acc+Block)*r:\n");
	print_mpz(acc);
	printf("\n");
	printf("\n");
	print_mpz(p);
	printf("\n");
	printf("\n((Acc+Block)*r)%%p:\n");
	mpz_mod(acc, acc, p);
	
	printf("\n");
}

void clamp(mpz_t r, mpz_t mask) {
	// and r with the mask
	printf("\n");
	print_mpz(r);
	printf("\n");
	print_mpz(mask);
	printf("\n");
	mpz_and(r, r, mask);
}

int main(){
	
	// declare and initialize mpz object
	mpz_t mask;
	mpz_init(mask);
	// set object to mask value
	//mpz_set_str(mask, "0fffffffc0ffffffc0ffffffc0ffffff", 16);
	mpz_set_str(mask, "ffffffc0ffffffc0ffffffc0fffffff", 16);
	mpz_t r;
	mpz_t s;
	size_t sz;

	char buff[34] = "Cryptographic Forum Research Group";
	sz = sizeof(buff);

	mpz_init(r);
	mpz_init(s);
	// set var r to first 128 bits of the key
	mpz_set_str(r, "a806d542fe52447f336d555778bed685", 16);
	mpz_set_str(s, "1bf54941aff6bf4afdb20dfb8a800301", 16);
	//mpz_set_str(s, "0103808afb0db2fd4abff6af4149f51b", 16);
	print_mpz(r);
	printf("\n");
	
	// clamp r
	clamp(r, mask);
	printf("\nClamped r:\n");
	print_mpz(r);
	printf("\n");
	// declare and initialize accumulator
	mpz_t acc;
	mpz_init_set_ui(acc, 0);
	// set value to zero
	//mpz_set_str(acc,"0", 16);
	
	mpz_t p;
	initialize_p(p);
	printf("\np:\n");
	print_mpz(p);
	printf("\n");
	size_t x = ceil((int)(((float)sz/16)));
	
	for(size_t i = 0; i < x; i++) {
		char a[17] = {0};
		char ar[34] = {0};
		strncpy(a, &buff[i*16], 16);
		rev_str(a, ar, 0, 17);
		//ar[1] = '1';
		//ar[0] = '0';
		printf("ar:\n%s\n", ar);
		
		mpz_t a_var;
		mpz_init_set_str(a_var, ar, 16);	
		

		printf("\na_var:\n");
		print_mpz(a_var);
		printf("\n");
		// compute new accumulator value
		compute_cycle(acc, a_var, r, p);

		// clear block value
		mpz_clear(a_var);
	}
	// FIXME: call final_block function
	size_t end_sz = sz-(x*16);
	char b[end_sz];
	memcpy(b, &buff[x*16], end_sz);
	//strncpy(b, &buff[x*16],end_sz);
	char br[(end_sz)*2];
	//b[end_sz-1] = '1';
	rev_str(b, br, 0, end_sz+1);
	//br[2] = '1';
	//br[1] = '0';

	mpz_t b_var;
	mpz_init(b_var);
	mpz_set_str(b_var, br, 16);
	printf("\n");
	print_mpz(b_var);	
	printf("\n");
	
	// compute final block sum 
	compute_cycle(acc, b_var, r, p);
	
	// add acc to second half of the key (s)
	printf("\nstuff:\n");
	print_mpz(s);
	printf("\n");
	print_mpz(acc);
	printf("\n");
	mpz_add(acc, acc, s);
	
	printf("\nacc+s:\n");
	print_mpz(acc);

	// print first 16 little endian bytes as the tag
	
	num_to_le_bytes(acc);

	// deallocate all numbers
	mpz_clear(acc);
	mpz_clear(p);
	mpz_clear(r);
	mpz_clear(mask);
	return 0;
}
