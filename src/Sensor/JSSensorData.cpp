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
// distributed under the License is distributed on an "AS IS" BASIS,
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

#include "plugin_config.h"
#include "JSSensorData.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sensor {

JSClassDefinition JSLightData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "LightData",
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

JSStaticValue JSLightData::m_property[] = {
    { TIZEN_LIGHT_DATA_LIGHT_LEVEL, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSLightData::m_jsClassRef = JSClassCreate(JSLightData::getClassInfo());

const JSClassRef JSLightData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSLightData::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSLightData::createJSObject(JSContextRef context, LightData* lightData)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(lightData));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSLightData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");

}

void JSLightData::finalize(JSObjectRef object)
{
    LOGD("enter");
    LightData* priv = static_cast<LightData*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSLightData::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    LOGD("enter");
    double value = 0;
    LightData* priv = static_cast<LightData*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is NULL");
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_LIGHT_DATA_LIGHT_LEVEL)) {
        value = priv->getLightLevel();
        return JSUtil::toJSValueRef(context, value);
    }
    return JSValueMakeUndefined(context);

}


//Proximity
JSClassDefinition JSProximityData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "ProximityData",
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

JSStaticValue JSProximityData::m_property[] = {
    { TIZEN_PROXIMITY_DATA_PROXIMITY_LEVEL, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSProximityData::m_jsClassRef = JSClassCreate(JSProximityData::getClassInfo());

const JSClassRef JSProximityData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSProximityData::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSProximityData::createJSObject(JSContextRef context, ProximityData* proximityData)
{
    LOGD("enter : %d",proximityData->getProximityLevel());
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(proximityData));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSProximityData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSProximityData::finalize(JSObjectRef object)
{
    ProximityData* priv = static_cast<ProximityData*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSProximityData::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    int value = 0;
    std::string proximityLevel("");
    LOGD("enter");

    ProximityData* priv = static_cast<ProximityData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PROXIMITY_DATA_PROXIMITY_LEVEL)) {
        value = priv->getProximityLevel();
        if(value){
            proximityLevel.append("NEAR");
        }else{
            proximityLevel.append("FAR");
        }
        return JSUtil::toJSValueRef(context, proximityLevel);
    }
    return JSValueMakeUndefined(context);

}

// Magnetic
JSClassDefinition JSMagneticData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "MagneticData",
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

JSStaticValue JSMagneticData::m_property[] = {
    { TIZEN_PROXIMITY_DATA_MAGNETIC_X, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PROXIMITY_DATA_MAGNETIC_Y, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PROXIMITY_DATA_MAGNETIC_Z, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_PROXIMITY_DATA_MAGNETIC_ACCURACY, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSMagneticData::m_jsClassRef = JSClassCreate(JSMagneticData::getClassInfo());

const JSClassRef JSMagneticData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSMagneticData::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSMagneticData::createJSObject(JSContextRef context, MagneticData* magneticData)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(magneticData));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSMagneticData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSMagneticData::finalize(JSObjectRef object)
{
    MagneticData* priv = static_cast<MagneticData*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSMagneticData::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    double value = 0;
    std::string accuracy("");

    MagneticData* priv = static_cast<MagneticData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PROXIMITY_DATA_MAGNETIC_X)) {
        value = priv->getMagneticX();
        return JSUtil::toJSValueRef(context, value);
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PROXIMITY_DATA_MAGNETIC_Y)) {
        value = priv->getMagneticY();
        return JSUtil::toJSValueRef(context, value);
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PROXIMITY_DATA_MAGNETIC_Z)) {
        value = priv->getMagneticZ();
        return JSUtil::toJSValueRef(context, value);
    }else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PROXIMITY_DATA_MAGNETIC_ACCURACY)) {
        accuracy.append(priv->getMagneticAccuracy());
        return JSUtil::toJSValueRef(context, accuracy);
    }
    return JSValueMakeUndefined(context);

}


// Pressure

JSClassDefinition JSPressureData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PressureData",
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

JSStaticValue JSPressureData::m_property[] = {
    { TIZEN_PRESSURE_DATA_PRESSURE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSPressureData::m_jsClassRef = JSClassCreate(JSPressureData::getClassInfo());

const JSClassRef JSPressureData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPressureData::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSPressureData::createJSObject(JSContextRef context, PressureData* pressureData)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(pressureData));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSPressureData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSPressureData::finalize(JSObjectRef object)
{
    PressureData* priv = static_cast<PressureData*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSPressureData::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    double value = 0;
    LOGD("enter");

    PressureData* priv = static_cast<PressureData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_PRESSURE_DATA_PRESSURE)) {
        value = priv->getPressure();
        return JSUtil::toJSValueRef(context, value);
    }
    return JSValueMakeUndefined(context);

}

// Ultraviolet
JSClassDefinition JSUltravioletData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "UltravioletData",
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

JSStaticValue JSUltravioletData::m_property[] = {
    { TIZEN_ULTRAVIOLET_DATA_ULTRAVIOLET_LEVEL, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSUltravioletData::m_jsClassRef = JSClassCreate(JSUltravioletData::getClassInfo());

const JSClassRef JSUltravioletData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSUltravioletData::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSUltravioletData::createJSObject(JSContextRef context, UltravioletData* ultravioletData)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(ultravioletData));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }
    return jsValueRef;
}


void JSUltravioletData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");

}

void JSUltravioletData::finalize(JSObjectRef object)
{
    UltravioletData* priv = static_cast<UltravioletData*>(JSObjectGetPrivate(object));
    if (priv) {
        delete priv;
    }
}

JSValueRef JSUltravioletData::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    double value = 0;
    UltravioletData* priv = static_cast<UltravioletData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is NULL");
    }

    if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_ULTRAVIOLET_DATA_ULTRAVIOLET_LEVEL)) {
        value = priv->getUltraviolet();
        return JSUtil::toJSValueRef(context, value);
    }
    return JSValueMakeUndefined(context);

}

} // Sensor
} // DeviceAPI
