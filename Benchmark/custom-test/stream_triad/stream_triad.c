#include "papi_multiplex_test.h"
#include <stdio.h>
#include <stdlib.h>

// data for stream kernel
#define N 10000000
double A[N], B[N], C[N];

void stream_triad()
{
    double s = 0.5;

    for(int i = 0; i < N; ++i) A[i] = (double)(i) / N;
    for(int i = 0; i < N; ++i) B[i] = (double)(i+1) / N;
    for(int i = 0; i < N; ++i) C[i] = (double)(i+2) / N;

    for(int i = 0; i < N; ++i)
    {
        A[i] = B[i] + s * C[i];
    }
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("usage: %s arch mode\n", argv[0]);
        printf("\t arch: 0 for kp920\n");
        printf("\t mode: 0 for mpx, 1 for ocoe1, ... \n");
        exit(-1);
    }

    int arch = atoi(argv[1]);
    char platform[1024];
    if(arch == 0)
    {
        sprintf(platform, "kp920");
    }

    int mode = atoi(argv[2]);
    char filename[1024];

    if(mode == 0)
    {
        sprintf(filename, "../events/%s/%s_events_mpx.txt", platform, platform);
    }
    else
    {
        sprintf(filename, "../events/%s/%s_events_ocoe_%d.txt", platform, platform, mode);
    }

    mytest_papi_init(filename, "stream_triad", mode);
    stream_triad();
    mytest_papi_stop();
    return 0;
}