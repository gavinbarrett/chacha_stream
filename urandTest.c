#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
int main() {
	//unsigned char buff[256] = "";
	uint32_t x[8];
	int fn = open("/dev/urandom", O_RDONLY);
	read(fn, x, 32);
	close(fn);
	//printf("%s\n", buff);
	for (int i = 0; i < 8; i++) {
		printf("%08x\n", x[i]);
	}
	printf("\n");
	return 0;
}
