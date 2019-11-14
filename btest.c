#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {

	unsigned char x[] = "Hello world!";

	uint32_t y = 0x00000000;
	int lim = 3;
	for (int i = 0; i < lim; i++) {
		for (int j = 0; j < 4; j++) {
			
			printf("%02x\n", x[(i*4)+j]);	
		}
		//((lim-i)*8)
	}

	printf("%hhn\n%d\n", x, y);

	return 0;
}
