#include "papi_multiplex_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//global values for counters
long long *counter_values;
int *testEvent;
char **testEventname;
int eventSet;
int NUM_EVENTS;

PAPI_MPX_attr attr;
FILE *fout;
struct timespec start, end;

int overflow_cycle[]={1000000, 1000000, 130000}; //for cascade, set T_{mlpx} = T_{papisample}

//helperfunctions
void result_to_file(FILE *fp, long long *counter_values)
{
    int i;
    
    fprintf(fp, "%lld", counter_values[1]); 
    for(i = 2; i < NUM_EVENTS+1; ++i)
    {
        fprintf(fp, ",%lld", counter_values[i]);
    }
    fprintf(fp, "\n");
}

void sampling_handler(int eventSet, void *address, long_long overflow_vector, void *context)
{
    PAPI_read(eventSet, counter_values);
    result_to_file(fout, counter_values);
}

//lib functions
void mytest_papi_init(char *filename, char *kernelname, int mode, int arch)
{
    int i, id;
    
    NUM_EVENTS = 0;
    PAPI_library_init(PAPI_VER_CURRENT);
    FILE *fevent = fopen(filename, "r");

    char buf[1024];
    while(fgets(buf, 1024, fevent))
    {
        NUM_EVENTS++;
    }
    printf("num_events = %d\n", NUM_EVENTS);
    fclose(fevent);

    //allocate local array
    testEvent = (int*)malloc(NUM_EVENTS*sizeof(int));
    counter_values = (long long*)malloc((NUM_EVENTS+1)*sizeof(long long));
    testEventname = (char**)malloc(NUM_EVENTS*sizeof(char*));

    fevent = fopen(filename, "r");

    for(i = 0; i < NUM_EVENTS; ++i)
	{
        testEventname[i]=(char*)malloc(1024*sizeof(char));
        fscanf(fevent, "%s\n", testEventname[i]);
		PAPI_event_name_to_code(testEventname[i], &testEvent[i]);
	}
    fclose(fevent);

    eventSet = PAPI_NULL;
    PAPI_create_eventset(&eventSet);
    PAPI_assign_eventset_component(eventSet, 0);

    if(mode == 0) //multiplex
    {
        // PAPI_set_multiplex(eventSet);
        PAPI_option_t mpx, opt;
        memset( &mpx, 0x0, sizeof ( mpx ) );
        mpx.multiplex.eventset = eventSet;
        mpx.multiplex.flags = PAPI_MULTIPLEX_DEFAULT;
        mpx.multiplex.ns = 32767;
        PAPI_set_opt( PAPI_MULTIPLEX, &mpx );

        PAPI_get_opt(PAPI_MULTIPLEX, &opt);
        printf("multiplex resolution = %d ns\n", opt.multiplex.ns);
    }

    PAPI_add_event(eventSet, PAPI_TOT_CYC);
    for(id = 0; id < NUM_EVENTS; ++id)
    {
        int retval = PAPI_add_event(eventSet, testEvent[id]);
        if(retval != PAPI_OK)
        {
            printf("Failed to add %s\n", testEventname[id]);
            exit(-1);
        }
    }

    PAPI_overflow(eventSet, PAPI_TOT_CYC, overflow_cycle[arch], 0, sampling_handler);

    char outfilename[1024];
    if(mode == 0)
    {
        sprintf(outfilename, "result_%s_mpx.csv", kernelname);
    }
    else
    {
        sprintf(outfilename, "result_%s_ocoe_%d.csv", kernelname, mode);
    }

    fout = fopen(outfilename, "w");
    fprintf(fout, "%s", testEventname[0]);
    for(i = 1; i < NUM_EVENTS; ++i)
    {
        fprintf(fout, ",%s", testEventname[i]);
    }
    fprintf(fout, "\n");

    PAPI_start(eventSet);
}

void mytest_papi_stop()
{
    int i;

    PAPI_stop(eventSet, counter_values);
    result_to_file(fout, counter_values);
    fclose(fout);

    free(testEvent);
    free(counter_values);
    for(i = 0; i < NUM_EVENTS; ++i)
	{
        free(testEventname[i]);
	}
    free(testEventname);
}

//lib functions for counting
//0 for multiplex, -1 for accurate multiplex, others for ocoe
void mytest_papi_count_init(char *filename, char *kernelname, int mode, int arch)
{
    int i, id;
    
    NUM_EVENTS = 0;
    PAPI_library_init(PAPI_VER_CURRENT);
    FILE *fevent = fopen(filename, "r");

    char buf[1024];
    while(fgets(buf, 1024, fevent))
    {
        NUM_EVENTS++;
    }
    printf("num_events = %d\n", NUM_EVENTS);
    fclose(fevent);

    //allocate local array
    testEvent = (int*)malloc(NUM_EVENTS*sizeof(int));
    counter_values = (long long*)malloc((NUM_EVENTS)*sizeof(long long));
    testEventname = (char**)malloc(NUM_EVENTS*sizeof(char*));

    fevent = fopen(filename, "r");

    for(i = 0; i < NUM_EVENTS; ++i)
	{
        testEventname[i]=(char*)malloc(1024*sizeof(char));
        fscanf(fevent, "%s\n", testEventname[i]);
		PAPI_event_name_to_code(testEventname[i], &testEvent[i]);
	}
    fclose(fevent);

    eventSet = PAPI_NULL;

    if(mode != -2)
    {
	    memset(&attr, 0x0, sizeof(PAPI_MPX_attr));
        PAPI_MPX_sampling_init(&attr);
        PAPI_MPX_create_eventset(&attr, &eventSet);
        for(id = 0; id < NUM_EVENTS; ++id)
        {
            PAPI_MPX_add_event(&attr, &eventSet, testEvent[id]);
        }
        PAPI_MPX_start(&attr, &eventSet, 21*1e6);
    }
    else
    {
        PAPI_create_eventset(&eventSet);
        PAPI_assign_eventset_component(eventSet, 0);

        if(mode == 0) //multiplex
        {
            PAPI_set_multiplex(eventSet);
        }

        for(id = 0; id < NUM_EVENTS; ++id)
        {
            printf("event %d %s\n", id ,testEventname[id]);
        }

        for(id = 0; id < NUM_EVENTS; ++id)
        {
            int retval = PAPI_add_event(eventSet, testEvent[id]);
            if(retval != PAPI_OK)
            {
                printf("Failed to add %s\n", testEventname[id]);
                exit(-1);
            }
        }

        PAPI_start(eventSet);
    }

    // clock_gettime(CLOCK_MONOTONIC, &start);
}

void mytest_papi_count_stop(int mode)
{
    int i;

    if(mode != -1)
    {
        PAPI_MPX_stop(&attr, &eventSet, counter_values, METHOD, PAPI_MPX_SAVING_LEVEL);
        PAPI_MPX_finalize(&attr);
    }
    else
    {
        PAPI_stop(eventSet, counter_values);
    }

    // clock_gettime(CLOCK_MONOTONIC, &end);
    // double time_elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) * 1e-9;

    //display the result
    fprintf(stderr, "PAPI result in mode %d:\n", mode);
    for(i = 0; i < NUM_EVENTS; ++i)
    {
        // fprintf(stderr,"\t%s: %lld\n", testEventname[i], counter_values[i]);
        fprintf(stderr, "%lld\n", counter_values[i]);
    }
    // fprintf(stderr, "Total time elapsed with profile: %lf s\n", time_elapsed);

    free(testEvent);
    free(counter_values);
    for(i = 0; i < NUM_EVENTS; ++i)
	{
        free(testEventname[i]);
	}
    free(testEventname);
}
