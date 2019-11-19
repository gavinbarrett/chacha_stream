#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

uint32_t rotl(uint32_t* byte, uint8_t amt) {
	/* circularly rotate the byte to the left by amt */
	uint32_t y = *byte << amt;
	return y | (*byte >> (32-amt));
}

uint32_t conv(uint32_t word) {
	/* convert endianness on a 32-bit buffer */
	return ((word << 24) | ((word << 8) & 0x00FF0000) | ((word >> 8) & 0x0000FF00) | (word >> 24));
}

void qr(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
	/* the ChaCha Quarter Round function */
	*a += *b; *d ^= *a; *d = rotl(d, 16);
	*c += *d; *b ^= *c; *b = rotl(b, 12);
	*a += *b; *d ^= *a; *d = rotl(d, 8);
	*c += *d; *b ^= *c; *b = rotl(b, 7);
}

void print_matrix(uint32_t* a) {
	/* print the internal state of the ChaCha matrix */
	printf("%08x  %08x  %08x  %08x\n", conv(a[0]), conv(a[1]), conv(a[2]), conv(a[3]));
	printf("%08x  %08x  %08x  %08x\n", conv(a[4]), conv(a[5]), conv(a[6]), conv(a[7]));
	printf("%08x  %08x  %08x  %08x\n", conv(a[8]), conv(a[9]), conv(a[10]), conv(a[11]));
	printf("%08x  %08x  %08x  %08x\n", conv(a[12]), conv(a[13]), conv(a[14]), conv(a[15]));
}

void print_bytestream(uint32_t* keystream) {
	/* print the ChaCha keystream */
	for(int i = 0; i < 16; i++)
		printf("%08x", conv(keystream[i]));
}

void print_charstream(unsigned char* st, size_t sz) {
	/* print the ChaCha key or cipher stream as an array of chars */
	for (int j = 0; j < (int)sz; j++) {
		printf("%c", st[j]);
	}
	printf("\n");
}

void load_constant(uint32_t* matrix) {
	/* load in constant value "expand 32-byte k" */
	//FIXME: we will have to account for endianness here
	matrix[0] = 0x61707865;		// "expa"
	matrix[1] = 0x3320646e;		// "nd 3"
	matrix[2] = 0x79622d32;		// "2-by"
	matrix[3] = 0x6b206574;		// "te k"

}

void load_key(uint32_t* matrix, uint32_t* key) {
	for (int i = 0; i < 8; i++) {
		matrix[i+4] = conv(key[i]);
		//printf("%08x ", key[i]);
		//printf("%08x", matrix[i+4]);
	}
	printf("\n");
}

void load_nonce(uint32_t* matrix, uint32_t* nonce) {
	for (int i = 0; i < 2; i++) {
		matrix[i+14] = nonce[i];
		printf("%08x", matrix[i+14]);
	}
}

//FIXME: create a super function that will take in a key
void chacha(uint32_t* key, uint32_t* nonce, unsigned char* str, uint32_t ctr) {
	// allocate an array of 16 words for the 4x4 chacha matrix
	uint32_t a[16];
	a[12] = ctr;
	a[13] = 0x00000000;

	// load the matrix
	load_constant(a);
	load_key(a, key);
	load_nonce(a, nonce);

/*
	printf("\nChaCha state:\n");
	for (int j = 0; j < 16; j++) {
		printf("%08x\n", a[j]);
	}
*/

	printf("\nkey:\n%08x%08x%08x%08x%08x%08x%08x%08x\n", key[0],key[1],key[2],key[3],key[4],key[5],key[6],key[7]);

	printf("nonce:\n%08x%08x\n",conv(a[14]),conv(a[15]));

	// copy input vector
	uint32_t b[16];
	for (int j = 0; j < 16; j++)
		b[j] = a[j];
	// perform the quarter round function 20 times total (twice per loop)
	for (int i = 0; i < 10; i++) {
		// column rounds
		qr(&a[0],&a[4],&a[8],&a[12]);
		qr(&a[1],&a[5],&a[9],&a[13]);
		qr(&a[2],&a[6],&a[10],&a[14]);
		qr(&a[3],&a[7],&a[11],&a[15]);
		// diagonal rounds
		qr(&a[0],&a[5],&a[10],&a[15]);
		qr(&a[1],&a[6],&a[11],&a[12]);
		qr(&a[2],&a[7],&a[8],&a[13]);
		qr(&a[3],&a[4],&a[9],&a[14]);
	}

	// add the mixed matrix to the original matrix mod 2^32 to generate the keystream
	// b now contains 64 bytes of pseudo random data to be used in our stream cipher
	for (int j = 0; j < 16; j++) {
		b[j] += a[j];
		//printf("%02x", b[j]);
	}
	//printf("\n");
	// ready the keystream buffer with correct index
	int k = 0 + (ctr*64);
	printf("\nkeystream:\n");
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			str[k] = b[i] >> ((8*j) & 0xff);
			printf("%02x", str[k]);
			k++;
		}
	}
	printf("\n");
}

void chacha_setup(unsigned char* key, unsigned char* nonce) {
	// FIXME: 
	printf("Printing key:\n");
	for (int i = 0; i < 4; i++)
		printf("%02x", key[i]);

	for (int i = 0; i < 3; i++)
		printf("%02x", key[i]);
}

int main(int argc, char** argv) {
	//char* str = "This is my secret message";
	size_t sz = strlen(argv[1]);
	int amt = (sz / 64) + 1;
	unsigned char* ptr;
	unsigned char st[sz];
	
	// dynamically allocate an array
	ptr = (unsigned char*)malloc((64*amt)*sizeof(unsigned char));
	
	// check to make sure memory was allocated
	if (!ptr) {
		printf("Cannot allocate memory!\n");
		exit(0);
	}

	uint32_t x[8];
	int fn = open("/dev/urandom", O_RDONLY);
	read(fn, x, 32);

	uint32_t y[2];
	int fn2 = open("/dev/urandom", O_RDONLY);
	read(fn2, y, 32);

	// generate the keystream (ptr)
	for (int i = 0; i < amt; i++) {
		chacha(x, y, ptr, i);
	}
	printf("\n");
	// encrypt the input stream with the keystream
	for (int j = 0; j < sz/*amt*64*/; j++) {
		printf("%02x", ptr[j]);
		st[j] = argv[1][j] ^ ptr[j];
	}
	printf("\nKeystream xored with plaintext:\n");
	for (int j = 0; j < sz; j++) {
		printf("%02x", st[j]);
	}
	printf("\n");
	// deallocate memory
	free(ptr);
	return 0;
}
