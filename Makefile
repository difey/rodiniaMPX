CC=icc
METHOD=PAPI_MPX_POST_NONE
PAPI_MPX_SAVING_LEVEL=PAPI_MPX_SAVING_RAW
CFLAGS=-DMETHOD=$(METHOD) -DPAPI_MPX_SAVING_LEVEL=$(PAPI_MPX_SAVING_LEVEL)
build: libmypapi.a

all: libmypapi.a papi_multiplex_test.o

libmypapi.a: papi_multiplex_test.o
	ar crv $@ papi_multiplex_test.o
	mv libmypapi.a lib/

papi_multiplex_test.o: papi_multiplex_test.c papi_multiplex_test.h
	$(CC) $(CFLAGS) -mcmodel=large -c papi_multiplex_test.c  -lpapi

clean:
	rm *.o lib/*
