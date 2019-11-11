#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

uint32_t rotl(uint32_t* byte, uint8_t amt) {
	/* circularly rotate the byte to the left by amt */
	uint32_t y = *byte << amt;
	return y | (*byte >> (32-amt));
}

void QR(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
	/* The Chacha Quarter Round function */
	*a += *b; *d ^= *a; *d = rotl(d, 16);
	*c += *d; *b ^= *c; *b = rotl(b, 12);
	*a += *b; *d ^= *a; *d = rotl(d, 8);
	*c += *d; *b ^= *c; *b = rotl(b, 7);
}

void print_chacha(uint32_t* a) {
	/* Print the internal state of the Chacha matrix */
	printf("%08x  %08x  %08x  %08x\n",a[0],a[1],a[2],a[3]);
	printf("%08x  %08x  %08x  %08x\n",a[4],a[5],a[6],a[7]);
	printf("%08x  %08x  %08x  %08x\n",a[8],a[9],a[10],a[11]);
	printf("%08x  %08x  %08x  %08x\n",a[12],a[13],a[14],a[15]);
}

void chacha() {
	// allocate an array of 16 words for the 4x4 chacha matrix
	uint32_t a[16];

	// load in constant value "expand 32-byte k"
	// in little endian - 128 bits
	a[0] = 0x61707865;		// "expa"
	a[1] = 0x3320646e;		// "nd 3"
	a[2] = 0x79622d32;		// "2-by"
	a[3] = 0x6b206574;		// "te k"

	// load the key - 256 bits
	a[4] = 0x03020100;
	a[5] = 0x07060504;
	a[6] = 0x0b0a0908;
	a[7] = 0x0f0e0d0c;
	a[8] = 0x13121110;
	a[9] = 0x17161514;
	a[10] = 0x1b1a1918;
	a[11] = 0x1f1e1d1c;

	// load the counter - 32 bits
	a[12] = 0x00000001;

	// load the nonce - 96 bits
	a[13] = 0x09000000;
	a[14] = 0x4a000000;
	a[15] = 0x00000000;
	
	// copy input vector
	uint32_t b[16];
	for (int j = 0; j < 16; j++) {
		b[j] = a[j];
	}

	print_chacha(a);
	
	// perform the quarter round function 20 times total (twice per loop)
	for (int i = 0; i < 10; i++) {
		// column rounds
		QR(&a[0],&a[4],&a[8],&a[12]);
		QR(&a[1],&a[5],&a[9],&a[13]);
		QR(&a[2],&a[6],&a[10],&a[14]);
		QR(&a[3],&a[7],&a[11],&a[15]);
		// diagonal rounds
		QR(&a[0],&a[5],&a[10],&a[15]);
		QR(&a[1],&a[6],&a[11],&a[12]);
		QR(&a[2],&a[7],&a[8],&a[13]);
		QR(&a[3],&a[4],&a[9],&a[14]);
	}

	printf("\n");
	print_chacha(a);
	printf("\n");

	// add the mixed matrix to the original matrix mod 2^32 to generate the keystream
	// b now contains 64 bytes of pseudo random data to be used in our stream cipher
	for (int j = 0; j < 16; j++) {
		b[j] += a[j];
	}
	print_chacha(b);
}

int main() {
	chacha();
	return 0;
}
