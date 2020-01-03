poly1305.out: poly1305.o poly_gmp_struct.o
	gcc -g -Wall -Werror -std=c11 -o poly1305.out poly1305.o poly_gmp_struct.o -lgmp

poly1305.o: poly1305.c poly1305.h
	gcc -g -Wall -Werror -std=c11 -c poly1305.c -lgmp -I .

poly_gmp_struct.o: poly_gmp_struct.c poly1305.h
	gcc -g -Wall -Werror -std=c11 -c poly_gmp_struct.c -lgmp -I .

clean:
	rm *.out *.o
