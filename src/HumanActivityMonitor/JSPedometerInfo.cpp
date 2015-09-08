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



#include <JSUtil.h>
#include <CommonsJavaScript/Converter.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include "plugin_config.h"
#include "JSPedometerInfo.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace HumanActivityMonitor {

JSClassDefinition JSPedometerInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PedometerInfo",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSPedometerInfo::m_property[] = {
    { TIZEN_PEDOMETER_INFO_STEP_STATUS, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_SPEED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_WALKING_FREQUENCY, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_CUMULATIVE_DISTANCE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_CUMULATIVE_CALORIE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_CUMULATIVE_TOTAL_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_CUMULATIVE_WALK_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_CUMULATIVE_RUN_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCES, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSClassRef JSPedometerInfo::m_jsClassRef = JSClassCreate(JSPedometerInfo::getClassInfo());

const JSClassRef JSPedometerInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPedometerInfo::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSPedometerInfo::createJSObject(JSContextRef context, PedometerInfo* pedometerInfo)
{
    pedometerInfo->m_jSStepDiffrence = JSPedometerInfo::StepDifferenceArraytoJSValueRef(context, pedometerInfo->getStepDifference(), pedometerInfo->getTimeStamp(), pedometerInfo->getTimeStampLength());

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(pedometerInfo));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    pedometerInfo->setContext(context);
    JSValueProtect(context, pedometerInfo->m_jSStepDiffrence);

    return jsValueRef;
}

JSValueRef JSPedometerInfo::StepDifferenceArraytoJSValueRef(JSContextRef context, int* stepDiff, int64_t* timeStamp, int timeStampLength)
{
    int index = 0;

    JSObjectRef jsResult = JSCreateArrayObject(context, 0, NULL);
    if (NULL == jsResult) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    for(index=0 ; index<timeStampLength ; index++){
        StepDifferenceInfoPtr stepDiffInfo(new StepDifferenceInfo(stepDiff[index], timeStamp[index]));

        JSValueRef jsObject = JSStepDifferenceInfo::createJSObject(context, stepDiffInfo);
        if (!jsObject) {
            throw UnknownException("object creation error");
        }

        if (!JSSetArrayElement(context, jsResult, index, jsObject)) {
            throw UnknownException("object creation error");
        }
    }
    return jsResult;

}


void JSPedometerInfo::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSPedometerInfo::finalize(JSObjectRef object)
{
    PedometerInfo* priv = static_cast<PedometerInfo*>(JSObjectGetPrivate(object));
    if (priv) {
        JSValueUnprotect(priv->getContext(), priv->m_jSStepDiffrence);
        LOGD("js value unprotected");
        delete priv;
    }
}

JSValueRef JSPedometerInfo::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    PedometerInfo* priv = static_cast<PedometerInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is NULL");
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_STEP_STATUS)) {
        std::string statusString("");
        statusString.append(priv->getStepStatusString());
        return JSUtil::toJSValueRef(context, statusString);
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_SPEED)) {
        return JSUtil::toJSValueRef(context, priv->getSpeed());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_WALKING_FREQUENCY)) {
        return JSUtil::toJSValueRef(context, priv->getWalkingFrequency());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_CUMULATIVE_DISTANCE)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeDistance());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_CUMULATIVE_CALORIE)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeCalorie());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_CUMULATIVE_TOTAL_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeTotalStepCount());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_CUMULATIVE_WALK_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeWalkStepCount());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_CUMULATIVE_RUN_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeRunStepCount());
    }else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_STEP_COUNT_DIFFERENCES)) {
        return priv->m_jSStepDiffrence;
    }
    return JSValueMakeUndefined(context);

}

JSClassDefinition JSHRMInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "HRMInfo",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSHRMInfo::m_property[] = {
    { TIZEN_HRM_INFO_HEART_RATE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_HRM_INFO_RR_INTERVAL, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSClassRef JSHRMInfo::m_jsClassRef = JSClassCreate(JSHRMInfo::getClassInfo());

const JSClassRef JSHRMInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSHRMInfo::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSHRMInfo::createJSObject(JSContextRef context, HRMInfo* hrmInfo)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(hrmInfo));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSHRMInfo::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSHRMInfo::finalize(JSObjectRef object)
{
    HRMInfo* priv = static_cast<HRMInfo*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSHRMInfo::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    long value = 0;
    HRMInfo* priv = static_cast<HRMInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_HRM_INFO_HEART_RATE)) {
        value = (long)priv->getHeartRate();
        return JSUtil::toJSValueRef(context, value);
    } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_HRM_INFO_RR_INTERVAL)) {
        value = (long)priv->getRRInterval();
        return JSUtil::toJSValueRef(context, value);
    }
    return JSValueMakeUndefined(context);

}


JSClassDefinition JSAccumulativePedometerInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PedometerInfo",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSAccumulativePedometerInfo::m_property[] = {
    { TIZEN_PEDOMETER_INFO_STEP_STATUS, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_SPEED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_WALKING_FREQUENCY, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PEDOMETER_INFO_ACCUMULATIVE_DISTANCE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PEDOMETER_INFO_ACCUMULATIVE_CALORIE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PEDOMETER_INFO_ACCUMULATIVE_TOTAL_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PEDOMETER_INFO_ACCUMULATIVE_WALK_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PEDOMETER_INFO_ACCUMULATIVE_RUN_STEP_COUNT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCES, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSAccumulativePedometerInfo::m_jsClassRef = JSClassCreate(JSAccumulativePedometerInfo::getClassInfo());

const JSClassRef JSAccumulativePedometerInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAccumulativePedometerInfo::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSAccumulativePedometerInfo::createJSObject(JSContextRef context, PedometerInfo* pedometerInfo)
{
    pedometerInfo->m_jSStepDiffrence = JSPedometerInfo::StepDifferenceArraytoJSValueRef(context, pedometerInfo->getStepDifference(), pedometerInfo->getTimeStamp(), pedometerInfo->getTimeStampLength());

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(pedometerInfo));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    pedometerInfo->setContext(context);
    JSValueProtect(context, pedometerInfo->m_jSStepDiffrence);

    return jsValueRef;
}


JSValueRef JSAccumulativePedometerInfo::StepDifferenceArraytoJSValueRef(JSContextRef context, int* stepDiff, int64_t* timeStamp, int timeStampLength)
{
    int index = 0;

    JSObjectRef jsResult = JSCreateArrayObject(context, 0, NULL);
    if (NULL == jsResult) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    for(index=0 ; index<timeStampLength ; index++){
        StepDifferenceInfoPtr stepDiffInfo(new StepDifferenceInfo(stepDiff[index], timeStamp[index]));

        JSValueRef jsObject = JSStepDifferenceInfo::createJSObject(context, stepDiffInfo);
        if (!jsObject) {
            throw UnknownException("object creation error");
        }

        if (!JSSetArrayElement(context, jsResult, index, jsObject)) {
            throw UnknownException("object creation error");
        }
    }
    return jsResult;

}

void JSAccumulativePedometerInfo::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSAccumulativePedometerInfo::finalize(JSObjectRef object)
{
    PedometerInfo* priv = static_cast<PedometerInfo*>(JSObjectGetPrivate(object));
    if (priv) {
        JSValueUnprotect(priv->getContext(), priv->m_jSStepDiffrence);
        LOGD("js value unprotected");
        delete priv;
    }
}

JSValueRef JSAccumulativePedometerInfo::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    PedometerInfo* priv = static_cast<PedometerInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_STEP_STATUS)) {
        std::string statusString("");
        statusString.append(priv->getStepStatusString());
        return JSUtil::toJSValueRef(context, statusString);
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_SPEED)) {
        return JSUtil::toJSValueRef(context, priv->getSpeed());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_WALKING_FREQUENCY)) {
        return JSUtil::toJSValueRef(context, priv->getWalkingFrequency());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_ACCUMULATIVE_DISTANCE)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeDistance());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_ACCUMULATIVE_CALORIE)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeCalorie());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_ACCUMULATIVE_TOTAL_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeTotalStepCount());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_ACCUMULATIVE_WALK_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeWalkStepCount());
    } else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_ACCUMULATIVE_RUN_STEP_COUNT)) {
        return JSUtil::toJSValueRef(context, priv->getCumulativeRunStepCount());
    }else if (JSStringIsEqualToUTF8CString(propertyName, PEDOMETER_INFO_STEP_COUNT_DIFFERENCES)) {
        return priv->m_jSStepDiffrence;
    }
    return JSValueMakeUndefined(context);

}

GPSInfoArrayHolder::GPSInfoArrayHolder()
{
    LOGD("enter");
}

GPSInfoArrayHolder::~GPSInfoArrayHolder()
{
    LOGD("enter");
    m_GPSInfoList.clear();
}

void GPSInfoArrayHolder::setContext(JSContextRef ctx)
{
    LOGD("enter");
    m_JSContext = ctx;
}

JSContextRef GPSInfoArrayHolder::getContext()
{
    LOGD("enter");
    return m_JSContext;
}


void GPSInfoArrayHolder::pushGPSInfo(double latitude, double longitude, double altitude, double speed,  double errorRange, int64_t timeStamp)
{
    LOGD("enter %f , %f , %f , %f , %f , %d", latitude, longitude, altitude, speed, errorRange, timeStamp);
    GPInfoPtr gpsInfoPtr(new GPSInfo(latitude, longitude, altitude, speed, errorRange, timeStamp));
    m_GPSInfoList.push_back(gpsInfoPtr);
}

JSClassDefinition JSGPSInfoArray::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "GPSInfoArray",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSGPSInfoArray::m_property[] = {
    { TIZEN_GPS_INFO_GPS_INFO, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSClassRef JSGPSInfoArray::m_jsClassRef = JSClassCreate(JSGPSInfoArray::getClassInfo());

const JSClassRef JSGPSInfoArray::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSGPSInfoArray::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSGPSInfoArray::createJSObject(JSContextRef context, double* latitude, double* longitude,  double* altitude, double* speed,  double* errorRange, int64_t* timeStamp, int totalCnt)
{
    GPSInfoArrayHolder* holder = new GPSInfoArrayHolder();
    int index = 0;

    for(index = 0 ; index<totalCnt ; index++){
        if(errorRange != NULL){
            holder->pushGPSInfo(latitude[index], longitude[index], altitude[index], speed[index], errorRange[index], timeStamp[index]);
        }else{
            LOGE("error range is null");
            holder->pushGPSInfo(latitude[index], longitude[index], altitude[index], speed[index], 0, timeStamp[index]);
        }
    }
    holder->m_JSGPSInfoArray = JSGPSInfoArray::gpsInfoArraytoJSValueRef(context, holder);
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    holder->setContext(context);
    JSValueProtect(context, holder->m_JSGPSInfoArray);

    LOGD("end");
    return jsValueRef;
}


void JSGPSInfoArray::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSGPSInfoArray::finalize(JSObjectRef object)
{
    GPSInfoArrayHolder* priv = static_cast<GPSInfoArrayHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSValueUnprotect(priv->getContext(), priv->m_JSGPSInfoArray);
        LOGD("js value unprotected");
        delete priv;
    }
}

JSValueRef JSGPSInfoArray::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    GPSInfoArrayHolder* priv = static_cast<GPSInfoArrayHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        return JSValueMakeUndefined(context);
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_GPS_INFO)) {
        return priv->m_JSGPSInfoArray;
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSGPSInfoArray::gpsInfoArraytoJSValueRef(JSContextRef context, GPSInfoArrayHolder* holder)
{

    std::list<GPInfoPtr>::iterator iterList = holder->m_GPSInfoList.begin();
    GPInfoPtr gpsInfo = NULL;
    int index = 0;

    JSObjectRef jsResult = JSCreateArrayObject(context, 0, NULL);
    if (NULL == jsResult) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    while(iterList != holder->m_GPSInfoList.end()){
        gpsInfo = *iterList;

        JSValueRef jsObject = JSGPSInfo::createJSObject(context, gpsInfo);
        if (!jsObject) {
            throw UnknownException("object creation error");
        }

        if (!JSSetArrayElement(context, jsResult, index, jsObject)) {
            throw UnknownException("object creation error");
        }

        index++;
        iterList++;
    }
    return jsResult;

}

// GPSInforHolder
GPSInfoHolder::GPSInfoHolder(GPInfoPtr gpsInfo)
{
    m_GPSInfo = gpsInfo;
}

GPSInfoHolder::~GPSInfoHolder()
{

}


// GPS Info
JSClassDefinition JSGPSInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "GPSInfo",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSGPSInfo::m_property[] = {
    { TIZEN_GPS_INFO_LATITUDE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_GPS_INFO_LONGITUDE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_GPS_INFO_ALTITUDE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_GPS_INFO_SPEED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_GPS_INFO_ERROR_RANGE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_GPS_INFO_TIME_STAMP, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSClassRef JSGPSInfo::m_jsClassRef = JSClassCreate(JSGPSInfo::getClassInfo());

const JSClassRef JSGPSInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSGPSInfo::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSGPSInfo::createJSObject(JSContextRef context, GPInfoPtr gpsInfo)
{
    GPSInfoHolder* gpsInforHolder = new GPSInfoHolder(gpsInfo);

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(gpsInforHolder));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSGPSInfo::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSGPSInfo::finalize(JSObjectRef object)
{
    GPSInfoHolder* priv = static_cast<GPSInfoHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSGPSInfo::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    GPSInfoHolder* priv = static_cast<GPSInfoHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        return JSValueMakeUndefined(context);
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_LATITUDE)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getLatitude());
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_LONGITUDE)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getLongitude());
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_ALTITUDE)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getAltitude());
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_SPEED)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getSpeed());
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_ERROR_RANGE)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getErrorRange());
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_GPS_INFO_TIME_STAMP)) {
        return JSUtil::toJSValueRef(context, priv->m_GPSInfo->getTimeStamp());
    }
    return JSValueMakeUndefined(context);

}

StepDifferenceInfoHolder::StepDifferenceInfoHolder(StepDifferenceInfoPtr stepDifferenceInfo)
{
    LOGD("enter");

    m_StepDifferenceInfo = stepDifferenceInfo;
}

StepDifferenceInfoHolder::~StepDifferenceInfoHolder()
{

}

JSClassDefinition JSStepDifferenceInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "JSStepDifferenceInfo",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //getProperty, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticValue JSStepDifferenceInfo::m_property[] = {
    { TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PEDOMETER_INFO_TIMESTAMP, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSClassRef JSStepDifferenceInfo::m_jsClassRef = JSClassCreate(JSStepDifferenceInfo::getClassInfo());

const JSClassRef JSStepDifferenceInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSStepDifferenceInfo::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSStepDifferenceInfo::createJSObject(JSContextRef context, StepDifferenceInfoPtr stepDifferenceInfo)
{
    LOGD("enter");

    StepDifferenceInfoHolder* stepDifferenceHolder = new StepDifferenceInfoHolder(stepDifferenceInfo);

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(stepDifferenceHolder));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    LOGD("enter");
    return jsValueRef;
}


void JSStepDifferenceInfo::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSStepDifferenceInfo::finalize(JSObjectRef object)
{
    StepDifferenceInfoHolder* priv = static_cast<StepDifferenceInfoHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSStepDifferenceInfo::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    long int stepDiff = 0;
    StepDifferenceInfoHolder* priv = static_cast<StepDifferenceInfoHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        return JSValueMakeUndefined(context);
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCE)) {
        stepDiff = priv->m_StepDifferenceInfo->getStepDifference();
        return JSUtil::toJSValueRef(context, stepDiff);
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PEDOMETER_INFO_TIMESTAMP)) {
        return JSUtil::toJSValueRef(context, priv->m_StepDifferenceInfo->getTimeStamp());
    }
    return JSValueMakeUndefined(context);

}

} // Context
} // DeviceAPI
