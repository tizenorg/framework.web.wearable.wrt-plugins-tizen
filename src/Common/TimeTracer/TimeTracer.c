//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#undef LOG_TAG
#define LOG_TAG "TIME_TRACER"

#include <stdio.h>
#include <dlog.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include "TimeTracer.h"

/*
* Global Variables
*/
#define TIME_TRACER_ITEM_MAX    500

#ifdef USE_REFACTORING_VERSION
#define OUTPUT_DIR "/opt/usr/media/wrt-plugins-tizen-time-trace-result"
const int REPORT_FORMAT_CSV = 1;
#else
#define OUTPUT_DIR "/tmp/wrt-plugins-tizen-time-trace-result"
const int REPORT_FORMAT_CSV = 0;
#endif

time_tracer_item ** g_items = NULL; //Time tracer item list
static int g_index = 0;
static int g_max_item_name_length = 0;
static long long g_first_time = 0;

#ifdef TIME_TRACER_UNIT_MSEC
static long long CURRENT_TIME()
{
    struct timeval startTime;
    gettimeofday(&startTime, NULL);
    long long usec = (long long)(((long long)(unsigned long)startTime.tv_sec) * 1000000LL + (long long)(startTime.tv_usec));
    return usec/1000;
}
#else
static long long CURRENT_TIME()
{
    struct timeval startTime;
    gettimeofday(&startTime, NULL);
    return (long long)(((long long)(unsigned long)startTime.tv_sec) * 1000000LL + (long long)(startTime.tv_usec));
}
#endif


/*
* Internal Implementation
*/
static void __time_tracer_item_free(void)
{
    int i = 0;
    if (!g_items)
        return;

    for(i=0;i<g_index;i++)
    {
        if(g_items[i])
        {
            if (g_items[i]->name)
                free(g_items[i]->name);
            free(g_items[i]);
            g_items[i] = NULL;
        }
    }

    g_index = 0;
    g_max_item_name_length = 0;
    g_first_time = 0;
    free(g_items);
    g_items = NULL;
}

static int __get_time_tracer_item_index(char* name)
{
    int i;
    assert(name);
    if(!g_items)
        return -1;

    for ( i = 0; i < g_index; i++ )
    {
        if(!g_items[i])
            return -1;
        if (strcmp(name, g_items[i]->name)==0)
            return i;
    }
    return -1;
}


/*
* Implementation
*/
int time_tracer_init(void)
{
    if (g_items) {
        LOGW("[%s] Ignored. Already Initialized.", __FUNCTION__);
        return 0;
    }
    g_items = (time_tracer_item **) malloc(TIME_TRACER_ITEM_MAX * sizeof(time_tracer_item *));
    if(!g_items)
    {
        LOGE("[%s] Failed to create global tracer item",__FUNCTION__);
        return -1;
    }
    g_first_time = CURRENT_TIME();
    LOGE("TIME TRACER INITIALIZED");
    return 0;
}

int time_tracer_release(void)
{
    if (!g_items)
        return 0;
    LOGE("TIME TRACER DESTROYED");
    __time_tracer_item_free();
    return 0;
}

int time_tracer_item_begin(const char* name, int show, const char* filename, int line)
{
    time_tracer_item *item = NULL;
    int index = 0;
    int name_len = 0;

    if (!g_items)
        return 0;

    if (g_index == TIME_TRACER_ITEM_MAX)
    {
        LOGE("[%s] Do not exceed tracer item max value (max : %d, index : %d)",__FUNCTION__,TIME_TRACER_ITEM_MAX, g_index);
        return -1;
    }

    if (!name)
    {
        LOGE("[%s] Item name is NULL.",__FUNCTION__);
        return -1;
    }

    name_len = strlen(name);
    if(name_len==0)
    {
        LOGE("[%s] Item name is Empty.",__FUNCTION__);
        return -1;
    }

    //1. Creates of gets the item
    //1-1. Creates new item if 'name' is not exists.
    if ((index = __get_time_tracer_item_index((char*)name)) == -1)
    {
        item = (time_tracer_item *)malloc(sizeof(time_tracer_item));
        if ( !item )
        {
            LOGE("[%s] Failed to create tracer item", __FUNCTION__);
            return -1;
        }

        //Clean-up
        memset( item, 0, sizeof (time_tracer_item) );
        item->min_elapsed_time = 0xFFFFFFFF;

        item->name = strdup(name);
        if (!item->name)
        {
            LOGE("[%s] Failed to strdup", __FUNCTION__);
            free(item);
            return -1;
        }

        //Add to the global item array
        g_items[g_index] = item;
        g_index++;

        if ( g_max_item_name_length < name_len )
            g_max_item_name_length = name_len;
    }
    else // 1-2. Returns existing item
    {
        item = g_items[index];
    }


    // 2. Gets the timestamp
    item->timestamp = CURRENT_TIME();


    if (item->first_timestamp == 0) // in case of first
    {
        //set first timestamp
        item->first_timestamp = item->timestamp;
        if (g_first_time > item->first_timestamp)
            g_first_time = item->first_timestamp;
    }

    // 3. Verify pairs of begin, end
    if (item->on_tracing)
    {
        LOGE("[%s] (Name : %s) is not 'end'ed!",__FUNCTION__, item->name);
        item->mismatch_count ++;
        return -1;
    }

    //set tracing on
    item->on_tracing = 1;

    if (show)
    {
        LOGE("[%s][BEGIN] %s (at %s:%d)", __FUNCTION__,name, filename, line );
        printf("[%s][BEGIN] %s (at %s:%d)\n", LOG_TAG,name, filename, line );
    }
    //Add invoke count for given item
    item->invoke_count++;
    return 0;
}

int time_tracer_item_end(const char* name, int show, const char* filename, int line)
{
    time_tracer_item * item = NULL;
    long long tval = 0;
    int index = 0;

    if (!g_items)
        return 0;

    // 1. Gets current timestamp first for more accuracy.
    tval = CURRENT_TIME();

    if (g_index == TIME_TRACER_ITEM_MAX)
    {
        LOGE("[%s] Do not exceed tracer item max value (max : %d, index : %d)",__FUNCTION__,TIME_TRACER_ITEM_MAX, g_index);
        return -1;
    }

    if (!name)
    {
        LOGE("[%s] Item name is NULL.",__FUNCTION__);
        return -1;
    }

    if(strlen(name)==0)
    {
        LOGE("[%s] Item name is Empty.",__FUNCTION__);
        return -1;
    }

    //2. Gets the item
    if ((index = __get_time_tracer_item_index((char*)name)) == -1)
    {
        LOGE("[%s] (Name : %s) is not exist.",__FUNCTION__, name);
        return -1;
    }
    item = g_items[index];

    // 3. Verify pairs of begin, end
    if (!item->on_tracing)
    {
        LOGE("[%s] (Name : %s) is not 'begin' yet",__FUNCTION__, item->name);
        item->mismatch_count ++;
        return -1;
    }

    //set last timestamp as current time
    item->last_timestamp = tval;

    //calculates the time gap(elapsed time) between current timestamp and item's timestamp.
    tval = tval - item->timestamp;

    //set max_elapsed_time if current elapsed time is larger than item's max elasped time.
    item->max_elapsed_time = tval > item->max_elapsed_time ? tval : item->max_elapsed_time;
    //set min_elapsed_time if current elapsed time is less than item's min elasped time.
    item->min_elapsed_time = tval < item->min_elapsed_time ? tval : item->min_elapsed_time;

    //Accumulates the item's total elapse time
    item->total_elapsed_time += tval;
    //set tracing off
    item->on_tracing = 0;

    if (show)
    {
        LOGE("[%s][END] %s - total elapsed time(acc): %lld, current elased time : %lld (at %s:%d)",__FUNCTION__, name, item->total_elapsed_time, tval, filename, line );
        printf("[%s][END] %s - total elapsed time(acc): %lld, current elased time : %lld (at %s:%d)\n",LOG_TAG,name, item->total_elapsed_time, tval, filename, line );
    }
    return 0;
}

void time_tracer_export_report(int direction, char* name)
{
    int i = 0;
    char format[256] = "";
    FILE* fp = stderr;

    if (!g_items)
        return;

    switch (direction)
    {
        case TIME_TRACER_EXPORT_STDOUT:
            fp = stdout;
            break;
        case TIME_TRACER_EXPORT_STDERR:
            fp = stderr;
            break;
        case TIME_TRACER_EXPORT_FILE:
            if(mkdir(OUTPUT_DIR,0755)!=0)
            {
                LOGE("[%s] Failed to mkdir()",__FUNCTION__);
            }
            char f_name[256];
            if(name==NULL || strlen(name)==0) {
#ifdef USE_REFACTORING_VERSION
                sprintf(f_name,"%s/default.%d.log",OUTPUT_DIR,getpid());
#else
                sprintf(f_name,"%s/%s",OUTPUT_DIR,"default.log");
#endif
            }
            else {
#ifdef USE_REFACTORING_VERSION
                sprintf(f_name,"%s/%s.%d.%s",OUTPUT_DIR,name,getpid(),"log");
#else
                sprintf(f_name,"%s/%s.%s",OUTPUT_DIR,name,"log");
#endif
            }

            fp = fopen(f_name, "wt");
            if (!fp)
            {
                LOGE("[%s] Failed to fopen().",__FUNCTION__);
                return;
            }
            break;
        default:
            LOGE("[%s] Invalid output direction.",__FUNCTION__);
            return;
    }

    if(REPORT_FORMAT_CSV) {
        for ( i = 0; i < g_index; i++ ) {
            if (g_items[i]->invoke_count == 0) {
                g_items[i]->invoke_count = 1;
            }

            long long average = 0;
            if (g_items[i]->total_elapsed_time == 0 || g_items[i]->invoke_count <3) {
                average = 0;
            }
            else {
                average = ((g_items[i]->total_elapsed_time) - (g_items[i]->min_elapsed_time) - (g_items[i]->max_elapsed_time)) / (g_items[i]->invoke_count-2);
            }

            fprintf(fp, "%s,%lld\n", g_items[i]->name, average);
            print_log(DLOG_ERROR, LOG_TAG, "%s,%lld\n", g_items[i]->name, average);
        }
    }
    else {

#ifdef USE_REFACTORING_VERSION
    snprintf(format, sizeof(format)-1,"[%%3d] %%-%ds |\tTotal:%%lld,\tCnt:%%lld,\tAvg:%%lld/%%lld,\tMin:%%lld,\tMax:%%lld,\tStart:%%lld,\tEnd:%%lld,\tMismatch:%%lld\n", g_max_item_name_length);
    fprintf(fp, "TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(usec)]\n", g_index);
    LOGE("TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(usec)]", g_index);
#else
#ifdef TIME_TRACER_UNIT_MSEC
    snprintf(format, sizeof(format)-1, "[%%3d] %%-%ds|\tTotal:%%4ld,\tCnt:%%3ld,\tAvg:%%4ld,\tMin:%%4ld,\tMax:%%4ld,\tStart:%%4lu,\tEnd:%%4lu,\tMismatch:%%3ld\n", g_max_item_name_length);
    fprintf(fp, "TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(msec)]\n", g_index);
    LOGE("TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(msec)]", g_index);
#else
    snprintf(format, sizeof(format)-1, "[%%3d] %%-%ds |\tTotal:%%ld,\tCnt:%%ld,\tAvg:%%ld,\tMin:%%ld,\tMax:%%ld,\tStart:%%lu,\tEnd:%%lu,\tMismatch:%%ld\n", g_max_item_name_length);
    fprintf(fp, "TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(usec)]\n", g_index);
    LOGE("TIME TRACER REPORT [BEGIN]=========================== [# of Item:%d, unit(usec)]", g_index);
#endif
#endif

        for ( i = 0; i < g_index; i++ ) {
            if (g_items[i]->invoke_count == 0) {
                g_items[i]->invoke_count = 1;
            }

#ifdef USE_REFACTORING_VERSION
            fprintf(fp, format, i,
                g_items[i]->name, // item name
                g_items[i]->total_elapsed_time, // total elasped time
                g_items[i]->invoke_count, // # of call
                (g_items[i]->total_elapsed_time == 0) ? 0 : (g_items[i]->total_elapsed_time / g_items[i]->invoke_count), // calculates the average elapsed time
                (g_items[i]->total_elapsed_time == 0 || g_items[i]->invoke_count <3) ? 0 : ((g_items[i]->total_elapsed_time-g_items[i]->min_elapsed_time-g_items[i]->max_elapsed_time) / (g_items[i]->invoke_count-2)), // calculates the average elapsed time (exclude min/max)
                g_items[i]->min_elapsed_time, // mininum elpased time
                g_items[i]->max_elapsed_time,// maximum elpased time
                g_items[i]->first_timestamp - g_first_time, //begin timestamp
                g_items[i]->last_timestamp - g_first_time, //end timestamp
                g_items[i]->mismatch_count ); // # of mismatch (begin - end mismatch)

            print_log(DLOG_ERROR, LOG_TAG, format,i,
                g_items[i]->name,
                g_items[i]->total_elapsed_time,
                g_items[i]->invoke_count,
                (g_items[i]->total_elapsed_time == 0) ? 0 : (g_items[i]->total_elapsed_time / (long long)g_items[i]->invoke_count),
                (g_items[i]->total_elapsed_time == 0 || g_items[i]->invoke_count <3) ? 0 : ((g_items[i]->total_elapsed_time-g_items[i]->min_elapsed_time-g_items[i]->max_elapsed_time) / (g_items[i]->invoke_count-2)),
                g_items[i]->min_elapsed_time,
                g_items[i]->max_elapsed_time,
                g_items[i]->first_timestamp - g_first_time,
                g_items[i]->last_timestamp - g_first_time,
                g_items[i]->mismatch_count );
#else
        fprintf(fp, format, i,
                g_items[i]->name, // item name
                g_items[i]->total_elapsed_time, // total elasped time
                g_items[i]->invoke_count, // # of call
                (g_items[i]->total_elapsed_time == 0)?0:(int)(g_items[i]->total_elapsed_time / g_items[i]->invoke_count), // calculates the average elapsed time
                g_items[i]->min_elapsed_time, // mininum elpased time
                g_items[i]->max_elapsed_time,// maximum elpased time
                g_items[i]->first_timestamp - g_first_time, //begin timestamp
                g_items[i]->last_timestamp - g_first_time, //end timestamp
                g_items[i]->mismatch_count ); // # of mismatch (begin - end mismatch)

        print_log(DLOG_ERROR, LOG_TAG, format, i,
                g_items[i]->name,
                g_items[i]->total_elapsed_time,
                g_items[i]->invoke_count,
                (g_items[i]->total_elapsed_time == 0) ? 0 : (int)(g_items[i]->total_elapsed_time / g_items[i]->invoke_count),
                g_items[i]->min_elapsed_time,
                g_items[i]->max_elapsed_time,
                g_items[i]->first_timestamp - g_first_time,
                g_items[i]->last_timestamp - g_first_time,
                g_items[i]->mismatch_count);

#endif
        }

        fprintf(fp, "TIME TRACER REPORT [END]  ============================\n");
        LOGE("TIME TRACER REPORT [END]  ============================");
    }

    if (direction == TIME_TRACER_EXPORT_FILE) {
        fclose(fp);
    }
}
