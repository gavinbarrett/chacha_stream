#include <chacha.h>
#include <poly1305.h>
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
	x[0] = 0x80818283;
	x[1] = 0x84858687;
	x[2] = 0x88898a8b;
	x[3] = 0x8c8d8e8f;
	x[4] = 0x90919293;
	x[5] = 0x94959697;
	x[6] = 0x98999a9b; 
	x[7] = 0x9c9d9e9f;

	//int fn = open("/dev/urandom", O_RDONLY);
	//read(fn, x, 32);

	uint32_t y[2];
	y[0] = 0x03020100;
	y[1] = 0x07060504;
	//int fn2 = open("/dev/urandom", O_RDONLY);
	//read(fn2, y, 32);

	// generate the keystream (ptr)
	chacha_stream(x, y, ptr, amt);
	
	//unsigned char st[sz];
	// encrypt the input stream with the keystream
	/*
	for (int j = 0; j < 32; j++) {
		printf("%02x", ptr[j]);
		//st[j] = string[j] ^ ptr[j];
	}
	*/
	printf("\n");
	// deallocate memory
	//char* s = "Cryptographic Forum Research Group";
	//poly1305_mac(s);
	free(ptr);
	return 0;
}
