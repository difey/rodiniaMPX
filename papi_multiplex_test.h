#ifndef PAPI_MPX_TEST_H
#define PAPI_MPX_TEST_H

#include "papi.h"

//lib functions api
#ifdef __cplusplus
extern "C"{
#endif
void mytest_papi_init(char *filename, char *kernelname, int mpx, int arch);

void mytest_papi_stop();

#ifdef __cplusplus
}
#endif

#endif