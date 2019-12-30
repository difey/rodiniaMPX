#include "papi_multiplex_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global values for counters
long long *counter_values;
int *testEvent;
char **testEventname;
int eventSet;
int NUM_EVENTS;

FILE *fout;

int overflow_cycle[]={1000000, 1000000, 130000}; //for cascade, set T_{mlpx} = T_{papisample}

//helperfunctions
void result_to_file(FILE *fp, long long *counter_values)
{
    fprintf(fp, "%lld", counter_values[1]); 
    for(int i = 2; i < NUM_EVENTS+1; ++i)
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

    for(int i = 0; i < NUM_EVENTS; ++i)
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
    for(int id = 0; id < NUM_EVENTS; ++id)
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
    for(int i = 1; i < NUM_EVENTS; ++i)
    {
        fprintf(fout, ",%s", testEventname[i]);
    }
    fprintf(fout, "\n");

    PAPI_start(eventSet);
}

void mytest_papi_stop()
{
    PAPI_stop(eventSet, counter_values);
    result_to_file(fout, counter_values);
    fclose(fout);

    free(testEvent);
    free(counter_values);
    for(int i = 0; i < NUM_EVENTS; ++i)
	{
        free(testEventname[i]);
	}
    free(testEventname);
}