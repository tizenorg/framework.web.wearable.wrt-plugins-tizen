/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#include "RefImplManager.h"
#include "mock.h"
#include <Ecore.h>
#include <CommonsJavaScript/Converter.h>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>


using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;
using namespace std;

namespace DeviceAPI {
namespace Reference{

static void* workerThread(void *data);
//static void workerThread(void *data, Ecore_Thread *thread);
static Eina_Bool jobCompleteCB(void *data);
//static void jobCompleteCB(void *data, Ecore_Thread *thread);
static void platformCB( int result , void *user_data);

//static void workerThread(void *data, Ecore_Thread *thread){
static void* workerThread(void *data){
    RefCallbackUserData * callback = static_cast<RefCallbackUserData*>(data);
    callback->result = mock_delayed_sync_function(callback->a, callback->b);
    ecore_idler_add(jobCompleteCB, data);
    return NULL;
}

//static void jobCompleteCB(void *data, Ecore_Thread *thread){
static Eina_Bool jobCompleteCB(void *data){
    RefCallbackUserData * callback = static_cast<RefCallbackUserData*>(data);
    Converter convert(callback->getContext());
    if( callback->result == 44 ){
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::IO_ERROR, "44 is I/O Error");
        callback->callErrorCallback(errobj);
    }else{
        JSValueRef result = convert.toJSValueRef(callback->result);
        callback->callSuccessCallback(result);
    }
    delete callback;
    return false;
}

int RefImplManager::syncToSync(int a, int b){
    if( a == 5 )
        throw InvalidValuesException("5 is not allowed");

    int ret = mock_sync_function(a,b);
    if( ret == 44 )
        throw IOException("44 is I/O error");
    return ret;
}

void RefImplManager::syncToAsync( RefCallbackUserData * callback ){
    if( callback->a == 5 )
        throw InvalidValuesException("5 is not allowed");
    //Ecore_Thread *tid = ecore_thread_run( workerThread, jobCompleteCB, NULL, callback);
    pthread_t thread;
    pthread_create(&thread, NULL, workerThread, callback);
    pthread_detach(thread);
}


static void platformCB( int result , void *user_data){
    RefCallbackUserData * callback = static_cast<RefCallbackUserData*>(user_data);
    Converter convert(callback->getContext());
    if( result == 44 ){
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::IO_ERROR, "44 is I/O Error");
        callback->callErrorCallback(errobj);
    }else{
        JSValueRef jsresult = convert.toJSValueRef(result);
        callback->callSuccessCallback(jsresult);
    }
    delete callback;
}

void RefImplManager::asyncToAsync( RefCallbackUserData * callback ){
    if( callback->a == 5 )
        throw InvalidValuesException("5 is not allowed");

    mock_async_function(callback->a, callback->b, platformCB, callback);
}


RefImplManager* RefImplManager::getInstance(){
    static RefImplManager instance;
    return &instance;
}

int RefImplManager::onFire(void* user_data){
    RefImplManager* obj = static_cast<RefImplManager*>(user_data);
    obj->broadCast();
    return false;
}

void RefImplManager::fireListener(){
    g_idle_add(onFire, this);
}

int RefImplManager::addListener(boost::shared_ptr<CallbackUserData> callback){
    int id = mCounter++;
    mListenerList.insert(listenerT(id, callback));
    return id;
}

void RefImplManager::removeListener(int id){
    map<int, boost::shared_ptr<Common::CallbackUserData>>::iterator itr;
    itr = mListenerList.find(id);
    if( itr != mListenerList.end() ){
        mListenerList.erase(itr);
    }
}

void RefImplManager::broadCast(){
    map<int, boost::shared_ptr<Common::CallbackUserData>> tmplist(mListenerList);
    map<int, boost::shared_ptr<Common::CallbackUserData>>::iterator itr;
    itr = tmplist.begin();
    while( itr != tmplist.end() ){
        boost::shared_ptr<Common::CallbackUserData> callback = (*itr).second;
        callback->callSuccessCallback();
        ++itr;
    }
}


void RefImplManager::setCallbackObject( JSContextRef context, boost::shared_ptr<Common::MultiCallbackUserData> callback){
    mCallbackList[context] = callback;
}
void RefImplManager::unsetCallbackObject( JSContextRef context ){
    mCallbackList.erase(context);
}

struct NamedCallbackData{
    JSContextRef context;
    std::string name;
    RefImplManager* obj;
};

void RefImplManager::namedCallbackFire(JSContextRef context, const char *callbackName){
    struct NamedCallbackData * data = new NamedCallbackData();
    data->context = context;
    data->name = callbackName;
    data->obj = this;
    g_idle_add(onNamedFire, data);
}

int RefImplManager::onNamedFire(void* user_data){
    struct NamedCallbackData * data = static_cast<NamedCallbackData*>(user_data);
    data->obj->namedBroadCast(data->context, data->name.c_str());
    delete data;
    return false;
}

void RefImplManager::namedBroadCast(JSContextRef context, const char *callbackName){
    map<JSContextRef, boost::shared_ptr<Common::MultiCallbackUserData>>::iterator itr;
    itr = mCallbackList.find(context);
    if( itr != mCallbackList.end() ){
        mCallbackList[context]->invokeCallback(callbackName);
    }
}


RefImplManager::RefImplManager():mCounter(0){
}
RefImplManager::~RefImplManager(){
}



}
}

