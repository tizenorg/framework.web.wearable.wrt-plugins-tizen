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

#include "JSConsole.h"
#include <CommonsJavaScript/Converter.h>
#include <stdio.h>
#include <dlog.h>
#include <sys/time.h>
#include <ctime>
#include <sys/time.h>


#undef LOG_TAG
#define LOG_TAG "TIZEN_DEVICEAPI"

using namespace std;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;


namespace DeviceAPI {
namespace Test {

JSClassRef JSConsole::m_jsClassRef = NULL;

JSClassDefinition JSConsole::m_jsClassInfo = {
    0,                                       // current (and only) version is 0
    kJSClassAttributeNone,        //attributes
    "console",                        //class name
    NULL,                                 // parent class
    NULL,                                 //static values
    JSConsole::m_function,     // static functions
    JSConsole::initialize,         // initialize
    JSConsole::finalize,          //finalize
    NULL,                                //hasProperty
    NULL,   //getProperty
    NULL,                                //setProperty
    NULL,                                //deleteProperty
    NULL,                                //getPropertyNames
    NULL,                               // callAsConstructor
    NULL,                               // constructor
    JSConsole::hasInstance,
    NULL // convertToType
};


JSStaticFunction JSConsole::m_function[] = {
    { "log", JSConsole::log, kJSPropertyAttributeNone },
    { "debug", JSConsole::log, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSConsole::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSConsole::getClassInfo()
{
    return &m_jsClassInfo;
}


JSObjectRef JSConsole::createJSObject( JSContextRef ctx ){
    return JSObjectMake(ctx, getClassRef(), NULL);
}


void JSConsole::initialize(JSContextRef ctx, JSObjectRef object)
{
}

void JSConsole::finalize(JSObjectRef object)
{
}

bool JSConsole::hasInstance(JSContextRef context, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception) {
    return JSValueIsObjectOfClass(context, possibleInstance, getClassRef());
}

#define timersub(a, b, result) \
do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((result)->tv_usec < 0) { \
        --(result)->tv_sec; \
        (result)->tv_usec += 1000000; \
    } \
} while (0)

JSValueRef JSConsole::log(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception){
    static struct timeval prev = {0};
    struct timeval current = {0};
    struct timeval diff = {0};
    gettimeofday(&current, NULL);
    if( prev.tv_sec == 0 && prev.tv_usec == 0)
        prev = current;

    timersub(&current, &prev, &diff);
    prev = current;

    Converter convert(ctx);
    if( argumentCount == 0 )
        return JSValueMakeUndefined(ctx);

    try{
        string result = convert.toString(arguments[0]);
        printf("<log[%d.%06d]>%s\n",(int)diff.tv_sec,(int)diff.tv_usec,result.c_str());
        LOGD("%s", result.c_str());
    }catch(const ConversionException& err){
    }
    return JSValueMakeUndefined(ctx);
}

} // Filesystem
} // TizenApis

