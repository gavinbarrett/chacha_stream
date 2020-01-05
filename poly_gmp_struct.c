#include <poly1305.h>

typedef struct PolyAuth {
	mpz_t r;
	mpz_t s;
	mpz_t mask;
	mpz_t acc;
	mpz_t p;
	mpz_t block;
} PolyAuth;

void print_mpz(mpz_t m) {
	mpz_out_str(stdout, 16, m);
}

void clamp(mpz_t r, mpz_t mask) {
	/* and r with the mask */
	mpz_and(r, r, mask);
}

void initialize_p(mpz_t p) {
	/* initialie p variable */
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

PolyAuth setup(/* pass in key and message */) {
	/* return a PolyAuth struct */
	PolyAuth auth;
	// FIXME: split key into two fragments: r and s
	// set key fragments
	mpz_init_set_str(auth.r, "a806d542fe52447f336d555778bed685", 16);
	mpz_init_set_str(auth.s, "1bf54941aff6bf4afdb20dfb8a800301", 16);
	// set mask for clamping r
	mpz_init_set_str(auth.mask, "ffffffc0ffffffc0ffffffc0fffffff", 16);
	// set accumulator variable to 0
	mpz_init_set_ui(auth.acc, 0);
	// set block variable to 0
	mpz_init_set_ui(auth.block,0);
	// initialize p variable
	initialize_p(auth.p);
	// clamp r variable
	clamp(auth.r, auth.mask);
	return auth;
}

void destroy(PolyAuth* auth) {
	/* deallocate all gmp numbers */
	mpz_clear(auth->r);
	mpz_clear(auth->s);
	mpz_clear(auth->mask);
	mpz_clear(auth->acc);
	mpz_clear(auth->p);
	mpz_clear(auth->block);
}

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
	for (; start_s < end_s; start_s++) {
		sprintf(tmps, "%02x", str[start_s+1]);
		strcat(s, tmps);
	}
}

void num_to_le_bytes(mpz_t acc, char output[]) {
	//char output[34];
	char tmp1;
	char tmp2;
	
	mpz_get_str(output, 16, acc);
	// copy the hex string into the output buffer
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
	// terminate the tag buffer
	output[33] = '\0';
	// increment past the head of the array
	char* out = output; out++;
	// copy the buffer to the output array
	memcpy(output,out,33);
}

void compute_cycle(PolyAuth* auth) {
	/* run computation loop, summing accumulator */
	// acc = acc + block
	mpz_add(auth->acc, auth->acc, auth->block);
	// acc = acc * r
	mpz_mul(auth->acc, auth->acc, auth->r);
	// acc = acc % p
	mpz_mod(auth->acc, auth->acc, auth->p);
}

void poly1305_mac(char* msg) {
	// input:  key (char*), msg (char*)
	// output: tag (char*)
	// FIXME: pass in key and message
	PolyAuth auth = setup();
	size_t sz;
	sz = strlen(msg);
	
	size_t x = ceil((int)(((float)sz/16)));
	// sum each block

	for(size_t i = 0; i < x; i++) {
		// FIXME: refactor computation
		char a[17] = {0};
		char ar[34] = {0};

		// copy chars from the buffer
		strncpy(a, &msg[i*16], 16);
		// read the bytes in little endian order and
		// output it as a hex string 
		rev_str(a, ar, 0, 17);
		// set the tmp block to this integer value
		mpz_set_str(auth.block, ar, 16);
		
		// compute new accumulator value
		compute_cycle(&auth);
	}
	// FIXME: refactor final_block function
	size_t end_sz = sz-(x*16);
	char b[end_sz];
	memcpy(b, &msg[x*16], end_sz);
	char br[(end_sz)*2];
	rev_str(b, br, 0, end_sz+1);

	//mpz_t b_var;
	mpz_init_set_str(auth.block, br, 16);
	
	// compute final block sum 
	compute_cycle(&auth);
	
	// add acc to second half of the key (s)
	mpz_add(auth.acc, auth.acc, auth.s);

	char z[34];
	mpz_get_str(z, 16, auth.block);
	//printf("\n%s\n", z);
/*
	for(int i = 0; i < end_sz; i++) {
		printf("%02x ", br[i]);
	}
*/
	// print first 16 little endian bytes as the tag
	char tag[34];
	num_to_le_bytes(auth.acc, tag);
	printf("tag: %s\n", tag);
	
	// deallocate all gmp numbers
	destroy(&auth);
}
