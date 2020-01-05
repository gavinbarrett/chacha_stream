chacha.out: chacha.o chachafunc.o poly_gmp_struct.o
	gcc -g -Wall -Werror -std=c11 -lgmp -o chacha.out chacha.o chachafunc.o poly_gmp_struct.o -I .

chacha.o: chacha.c chacha.h poly1305.h
	gcc -g -Wall -Werror -std=c11 -lgmp -c chacha.c -I .

chachafunc.o: chachafunc.c chacha.h
	gcc -g -Wall -Werror -std=c11 -c chachafunc.c -I .

poly_gmp_struct.o: poly_gmp_struct.c poly1305.h
	gcc -g -Wall -Werror -std=c11 -lgmp -c poly_gmp_struct.c -I .

clean:
	rm *.out *.o
