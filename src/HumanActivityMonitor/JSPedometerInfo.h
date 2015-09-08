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

#ifndef __TIZEN_JS_PEDOMETER_INFO_H__
#define __TIZEN_JS_PEDOMETER_INFO_H__

#include <JavaScriptCore/JavaScript.h>
#include <JSUtil.h>

#include "PedometerInfo.h"
#include "MotionManager.h"

namespace DeviceAPI {
namespace HumanActivityMonitor {

class JSPedometerInfo
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, PedometerInfo* pedometerInfo);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
    static JSValueRef StepDifferenceArraytoJSValueRef(JSContextRef context, int* stepDiff, int64_t* timeStamp, int timeStampLength);

private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};

class JSHRMInfo
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, HRMInfo* hrmInfo);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};


class JSAccumulativePedometerInfo
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, PedometerInfo* pedometerInfo);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
    static JSValueRef StepDifferenceArraytoJSValueRef(JSContextRef context, int* stepDiff, int64_t* timeStamp, int timeStampLength);
private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};


class GPSInfoArrayHolder {
public:
    GPSInfoArrayHolder();
    virtual ~GPSInfoArrayHolder();
    void setContext(JSContextRef ctx);
    JSContextRef getContext();
    void pushGPSInfo(double latitude, double longitude, double altitude, double speed,  double errorRange, int64_t timeStamp);

    std::list<GPInfoPtr> m_GPSInfoList;
    JSValueRef m_JSGPSInfoArray;
    JSContextRef m_JSContext;
private:
};

class GPSInfoHolder {
public:
    GPSInfoHolder(GPInfoPtr gpsInfo);
    virtual ~GPSInfoHolder();

    GPInfoPtr m_GPSInfo;
private:
};



class StepDifferenceInfoHolder {
public:
    StepDifferenceInfoHolder(StepDifferenceInfoPtr stepDifferenceInfo);
    virtual ~StepDifferenceInfoHolder();

    StepDifferenceInfoPtr m_StepDifferenceInfo;
private:
};


class JSGPSInfoArray
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, double* latitude, double* longitude,  double* altitude, double* speed,  double* errorRange, int64_t* timeStamp, int totalCnt);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
    static JSValueRef gpsInfoArraytoJSValueRef(JSContextRef context, GPSInfoArrayHolder* holder);

private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};



class JSGPSInfo
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, GPInfoPtr gpsInfo);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};




class JSStepDifferenceInfo
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, StepDifferenceInfoPtr stepDifferenceInfo);
    static JSValueRef getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception);
private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};


} // Context
} // DeviceAPI

#endif // __TIZEN_JS_DOWNLOAD_MANAGER_H__
