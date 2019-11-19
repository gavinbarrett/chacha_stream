#include <stdio.h>

int main() {

	unsigned char x[5] = {0x68, 0x65, 0x6c, 0x6c, 0x6f};

	unsigned char y[5] = {0x79, 0x6f, 0x64, 0x6f, 0x67};

	unsigned char c[5];

	for (int i = 0; i < 5; i++) {
		c[i] = x[i] ^ y[i];
		printf("%02x\n", c[i]);
	}

	return 0;
}
