#ifndef PAPI_MPX_TEST_H
#define PAPI_MPX_TEST_H

#include "papi.h"
#include "papi_accurate_multiplexing.h"

//lib functions api
#ifdef __cplusplus
extern "C"{
#endif
void mytest_papi_init(char *filename, char *kernelname, int mpx, int arch);

void mytest_papi_stop();

void mytest_papi_count_init(char *filename, char *kernelname, int mode, int arch);

void mytest_papi_count_stop(int mode);

#ifdef __cplusplus
}
#endif

#endif