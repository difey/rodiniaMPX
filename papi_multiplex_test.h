#ifndef PAPI_MPX_TEST_H
#define PAPI_MPX_TEST_H

#include "papi.h"

//lib functions api
void mytest_papi_init(char *filename, char *kernelname, int mpx);

void mytest_papi_stop();

#endif