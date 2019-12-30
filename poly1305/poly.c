#include <gmp.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint32_t conv(uint32_t word) {
	/* reverse a four byte word */
	return ((word << 24) | ((word << 8) & 0x00ff0000) | ((word >> 8) & 0x0000ff00) | (word >> 24));
}

void clamp(uint32_t* r) {
	/* clamp r within bounds */
	*r &= 0x0fffffff; r++;
	*r &= 0x0ffffffc; r++;
	*r &= 0x0ffffffc; r++;
	*r &= 0x0ffffffc; r-=3;
}

void poly(uint32_t* key) {
	clamp(key);
	printf("%02x",(key[3]));
	printf("%02x",(key[2]));
	printf("%02x",(key[1]));
	printf("%02x\n",(key[0]));
}

int main() {
	uint32_t arr[8];
	arr[0] = conv(0x85d6be78);
	arr[1] = conv(0x57556d33);
	arr[2] = conv(0x7f4452fe);
	arr[3] = conv(0x42d506a8);
	arr[4] = conv(0x0103808a);
	arr[5] = conv(0xfb0db2fd);
	arr[6] = conv(0x4abff6af);
	arr[7] = conv(0x4149f51b);

	poly(arr);

	return 0;
}
