//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#include <SecurityExceptions.h>
#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <cstring>

#include "plugin_config.h"
#include "JSMotionManager.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;
using namespace std;


namespace DeviceAPI {
namespace HumanActivityMonitor {

JSClassDefinition JSMotionManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "MotionManager",
    NULL, //ParentClass
    NULL, //StaticValues
    m_function, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticFunction JSMotionManager::m_function[] = {
    { MOTION_API_GET_CONTEXT_INFO, JSMotionManager::getContextInfo, kJSPropertyAttributeNone },
    { MOTION_API_SET_ACCUMULATIVE_PEDOMETER_LISTENER, JSMotionManager::setAccumulativePedometerListener, kJSPropertyAttributeNone },
    { MOTION_API_UNSET_ACCUMULATIVE_PEDOMETER_LISTENER, JSMotionManager::unsetAccumulativePedometerListener, kJSPropertyAttributeNone },
    { MOTION_API_START, JSMotionManager::setChangeListener, kJSPropertyAttributeNone },
    { MOTION_API_STOP, JSMotionManager::unsetChangeListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSMotionManager::m_jsClassRef = JSClassCreate(JSMotionManager::getClassInfo());

const JSClassRef JSMotionManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSMotionManager::getClassInfo()
{
    return &m_classInfo;
}

void JSMotionManager::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSMotionManager::finalize(JSObjectRef object)
{
    LOGD("enter");
}

JSValueRef JSMotionManager::getContextInfo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    string contextType;

    try {

        AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_GET_CONTEXT_INFO);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

        ArgumentValidator validator(context, argumentCount, arguments);
        contextType = validator.toString(0);

        if(!strcmp(MOTION_ENUM_TYPE_PEDOMETER,contextType.c_str())){
            // successCallback
            JSObjectRef successCB = validator.toFunction(1);
            if (successCB) {
                PedometerSuccessCallbackPtr pedometerGetCB(new PedometerSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
                pedometerGetCB->setSuccessCallback(successCB);

                // errorCallback
                JSObjectRef errCB = validator.toFunction(2, true);
                if(errCB){
                    pedometerGetCB->setErrorCallback(errCB);
                }

                // perform
                MotionManager::getInstance()->getContextInfo(pedometerGetCB);

            }else{
                LOGE("argument type mismatch");
                throw TypeMismatchException("argument type mismatch");
            }

        }else if(!strcmp(MOTION_ENUM_TYPE_WRIST_UP,contextType.c_str()) || !strcmp(MOTION_ENUM_TYPE_HRM,contextType.c_str())){
            LOGE("Not supported");
            throw NotSupportedException("Not supported");
        }else{
            LOGE("argument type mismatch");
            throw TypeMismatchException("Type mismatch");
        }

        return JSValueMakeUndefined(context);

    } catch (const TypeMismatchException &err) {
        LOGE("Type Mismatch Exception");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const DeviceAPI::Common::BasePlatformException &err) {
        LOGE("throw exception");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSMotionManager::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try {
        AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_START);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

        string contextType;
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(1, true);

        contextType = validator.toString(0);

        if(!strcmp(MOTION_ENUM_TYPE_PEDOMETER,contextType.c_str())){

            PedometerSuccessCallbackPtr pedometerCB(new PedometerSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
            if (successCB) {
                pedometerCB->setSuccessCallback(successCB);
            }
            MotionManager::getInstance()->setChangeListener(pedometerCB);

        }else if(!strcmp(MOTION_ENUM_TYPE_WRIST_UP,contextType.c_str())){

            WristUpSuccessCallbackPtr wristupCB(new WristUpSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
            if (successCB) {
                wristupCB->setSuccessCallback(successCB);
            }
            MotionManager::getInstance()->setChangeListener(wristupCB);

        }else if(!strcmp(MOTION_ENUM_TYPE_HRM,contextType.c_str())){

            HRMSuccessCallbackPtr hrmCB(new HRMSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
            if (successCB) {
                hrmCB->setSuccessCallback(successCB);
            }
            MotionManager::getInstance()->setChangeListener(hrmCB);

        }else if(!strcmp(MOTION_ENUM_TYPE_GPS,contextType.c_str())){
            AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_START);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

            GPSSuccessCallbackPtr gpsCB(new GPSSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
            if (successCB) {
                gpsCB->setSuccessCallback(successCB);
            }
            MotionManager::getInstance()->setChangeListener(gpsCB);

        }else{
               throw TypeMismatchException("Type Mismatch");
        }

        return JSValueMakeUndefined(context);

    } catch (const DeviceAPI::Common::BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSMotionManager::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try {
        AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_STOP);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

        string contextType;
        ArgumentValidator validator(context, argumentCount, arguments);
        contextType = validator.toString(0);


        if(!strcmp(MOTION_ENUM_TYPE_PEDOMETER,contextType.c_str()) || !strcmp(MOTION_ENUM_TYPE_WRIST_UP,contextType.c_str())
            || !strcmp(MOTION_ENUM_TYPE_HRM,contextType.c_str()) || !strcmp(MOTION_ENUM_TYPE_GPS,contextType.c_str())){
            AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_START);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
            MotionManager::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context), contextType.c_str());
        }else{
               throw TypeMismatchException("Type Mismatch");
        }

        return JSValueMakeUndefined(context);

    } catch (const DeviceAPI::Common::BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSMotionManager::setAccumulativePedometerListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");


    try {

        AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_START);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        PedometerSuccessCallbackPtr pedometerCB(new PedometerSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            pedometerCB->setSuccessCallback(successCB);
        }
        MotionManager::getInstance()->setAccumulativePedometerListener(pedometerCB);

        return JSValueMakeUndefined(context);

    } catch (const DeviceAPI::Common::BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}

JSValueRef JSMotionManager::unsetAccumulativePedometerListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try {
        AceSecurityStatus status = HUMANACTIVITYMONITOR_CHECK_ACCESS(MOTION_API_STOP);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

        ArgumentValidator validator(context, argumentCount, arguments);
        MotionManager::getInstance()->unsetAccumulativePedometerListener(GlobalContextManager::getInstance()->getGlobalContext(context));

        return JSValueMakeUndefined(context);

    } catch (const DeviceAPI::Common::BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}

} // Context
} // DeviceAPI
