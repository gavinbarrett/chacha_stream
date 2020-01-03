#include <gmp.h>
#include <stdio.h>

int main() {
a
	mpz_t n;
	mpz_init_set_str(n,"016f7247206863726165736552206d7572",16);
	
	mpz_add_ui(n,n,1);

	mpz_out_str(stdout, 16, n);
	mpz_clear(n);
	return 0;
}
