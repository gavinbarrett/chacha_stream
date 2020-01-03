chacha.out: chacha.o chachafunc.o
	gcc -g -Wall -Werror -std=c11 -o chacha.out chacha.o chachafunc.o

chacha.o: chacha.c chacha.h
	gcc -g -Wall -Werror -std=c11 -c chacha.c -I .

chachafunc.o: chachafunc.c chacha.h
	gcc -g -Wall -Werror -std=c11 -c chachafunc.c -I .

clean:
	rm *.out *.o
