#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

uint32_t rotl(uint32_t* byte, uint8_t amt) {
	uint32_t y = *byte << amt;
	return y | (*byte >> (32-amt));
}

uint32_t rotr(uint32_t byte, uint8_t amt) {
	uint32_t y = byte >> amt;
	return y | (byte << (32-amt));
}


void QR(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
	/* The Chacha Quarter Round function */
	*a += *b; *d ^= *a; *d = rotl(d, 16);
	*c += *d; *b ^= *c; *b = rotl(b, 12);
	*a += *b; *d ^= *a; *d = rotl(d, 8);
	*c += *d; *b ^= *c; *b = rotl(b, 7);
}

void print_chacha() {

}

int main() {

	uint32_t c0 = 0x61707865;
	uint32_t c1 = 0x3320646e;
	uint32_t c2 = 0x79622d32;
	uint32_t c3 = 0x6b206574;
	uint32_t arr[16];
	arr[0] = c0;
	arr[1] = c1;
	arr[2] = c2;
	arr[3] = c3;

	QR(&w,&x,&y,&z);
	
	return 0;
}
