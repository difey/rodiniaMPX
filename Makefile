CC=gcc
METHOD=
CFLAGS=-DMETHOD=$(METHOD)
build: libmypapi.a

libmypapi.a: papi_multiplex_test.o
	ar crv $@ papi_multiplex_test.o
	mv libmypapi.a lib/

papi_multiplex_test.o: papi_multiplex_test.c papi_multiplex_test.h
	$(CC) $(CFLAGS) -mcmodel=large -c papi_multiplex_test.c

clean:
	rm *.o lib/*