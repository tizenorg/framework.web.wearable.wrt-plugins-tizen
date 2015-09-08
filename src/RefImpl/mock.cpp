#include "mock.h"
#include <Ecore.h>
#include <pthread.h>
#include <unistd.h>
#include <dlog.h>
#include <glib.h>

#undef LOG_TAG
#define LOG_TAG "TIZEN_DEVICEAPI"


struct worker_data{
    int a;
    int b;
    async_cb callback;
    void * user_data;
    int result;
};

static void* worker_function(void *arg);
static int threadsafe_function(void *arg);


int mock_sync_function(int a, int b){
    return a+b;
}
int mock_delayed_sync_function(int a, int b){
    sleep(2);
    return a+b;
}


int mock_async_function( int a, int b, async_cb callback, void *user_data){
    pthread_t thread;
    worker_data *data = new worker_data();
    data->a = a;
    data->b = b;
    data->callback = callback;
    data->user_data = user_data;
    pthread_create(&thread, NULL, worker_function,  data);
    pthread_detach(thread);
    return 0;
}


static void* worker_function(void *arg){
    static int count = 0;
    worker_data *data = (worker_data*)arg;
    data->result = mock_delayed_sync_function(data->a, data->b);
    g_idle_add(threadsafe_function, data);
    return NULL;
}

static int threadsafe_function(void *arg){
    worker_data *data = (worker_data*)arg;
    if( data->callback )
        data->callback(data->result, data->user_data);
    delete data;
    return false;
}

