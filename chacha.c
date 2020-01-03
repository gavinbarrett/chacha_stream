#include <chacha.h>

int main() {
	char* string = "This is my secret now";
	size_t sz = strlen(string);
	int amt = (sz / 64) + 1;
	unsigned char* ptr;
	//unsigned char st[sz];
	
	// dynamically allocate an array
	ptr = (unsigned char*)malloc((64*amt)*sizeof(unsigned char));
	
	// check to make sure memory was allocated
	if (!ptr) {
		printf("\nCannot allocate memory!\n");
		exit(0);
	}

	uint32_t x[8];
	int fn = open("/dev/urandom", O_RDONLY);
	read(fn, x, 32);

	uint32_t y[2];
	int fn2 = open("/dev/urandom", O_RDONLY);
	read(fn2, y, 32);

	// generate the keystream (ptr)
	chacha_stream(x, y, ptr, amt);
	
	unsigned char st[sz];
	// encrypt the input stream with the keystream
	for (int j = 0; j < sz; j++) {
		//printf("%02x", ptr[j]);
		st[j] = string[j] ^ ptr[j];
	}
	//printf("\nkeystream xored with plaintext:\n");
	for (int j = 0; j < sz; j++) {
		printf("%02x", st[j]);
	}
	printf("\n");
	// deallocate memory
	free(ptr);
	return 0;
}
