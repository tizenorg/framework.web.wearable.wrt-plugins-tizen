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
#include <Logger.h>

#include "plugin_config.h"
#include "JSSensorService.h"
#include "SensorService.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sensor {

JSClassDefinition JSSensorService::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "SensorService",
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

JSStaticFunction JSSensorService::m_function[] = {
    { SENSOR_SERVICE_API_GET_DEFAULT_SENSOR, getDefaultSensor, kJSPropertyAttributeNone },
    { SENSOR_SERVICE_API_GET_AVAILABLE_SENSOR, getAvailableSensor, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSSensorService::m_jsClassRef = JSClassCreate(JSSensorService::getClassInfo());

const JSClassRef JSSensorService::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSensorService::getClassInfo()
{
    return &m_classInfo;
}

void JSSensorService::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSSensorService::finalize(JSObjectRef object)
{
    LOGD("enter");
}

JSValueRef JSSensorService::getDefaultSensor(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        LOGD("enter");

        bool supported = false;
        ArgumentValidator validator(context, argumentCount, arguments);
        std::string sensorType = validator.toString(0);
        std::string proximityType = "PROXIMITY";
        std::string lightType = "LIGHT";
        std::string magneticType = "MAGNETIC";
        std::string pressureType = "PRESSURE";
        std::string ultravioletType = "ULTRAVIOLET";
        std::string hrmrawType = "HRM_RAW";

        // perform
        if(proximityType.compare(sensorType) == 0){
            ProximitySensor* proximitySensor = ProximitySensor::getInstance();

            supported = proximitySensor->isSupported();
            if(false == supported){
                LOGE("Not supported");
                throw NotSupportedException("Not supported");
            }

            JSValueRef proximityRef = JSProximitySensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), proximitySensor);
            return proximityRef;
        }else if(lightType.compare(sensorType) == 0){
            LightSensor* lightSensor = LightSensor::getInstance();

            supported = lightSensor->isSupported();
            if(false == supported){
                LOGE("Not supported");
                throw NotSupportedException("Not supported");
            }

            JSValueRef lightRef = JSLightSensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), lightSensor);
            return lightRef;
        }else if(magneticType.compare(sensorType) == 0){
            MagneticSensor* magneticSensor = MagneticSensor::getInstance();

            supported = magneticSensor->isSupported();
            if(false == supported){
                LOGE("Not supported");
                throw NotSupportedException("Not supported");
            }

            JSValueRef magneticRef = JSMagneticSensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), magneticSensor);
            return magneticRef;
        }else if(pressureType.compare(sensorType) == 0){
            PressureSensor* pressureSensor = PressureSensor::getInstance();

            supported = pressureSensor->isSupported();
            if(false == supported){
                LOGE("Not supported");
                throw NotSupportedException("Not supported");
            }

            JSValueRef pressureRef = JSPressureSensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), pressureSensor);
            return pressureRef;
        }else if(ultravioletType.compare(sensorType) == 0){
            UltravioletSensor* ultravioletSensor = UltravioletSensor::getInstance();

            supported = ultravioletSensor->isSupported();
            if(false == supported){
                LOGE("Not supported");
                throw NotSupportedException("Not supported");
            }

            JSValueRef uvsensorRef = JSUltravioletSensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), ultravioletSensor);
            return uvsensorRef;
        }else if (hrmrawType.compare(sensorType) == 0) {
            AceSecurityStatus status = SENSOR_CHECK_ACCESS(SENSOR_SERVICE_API_GET_DEFAULT_SENSOR);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
            HRMRawSensor* hrmrawSensor = HRMRawSensor::getInstance();

            supported = hrmrawSensor->isSupported();
            if (false == supported){
                LOGD("Not supported");
                throw NotSupportedException("Not supported");
            }
            JSValueRef hrmrawRef = JSHRMRawSensor::createJSObject(GlobalContextManager::getInstance()->getGlobalContext(context), hrmrawSensor);
            return hrmrawRef;
        }else{
            LOGE("argument type mismatch");
            throw TypeMismatchException("argument type mismatch");
        }

    } catch (const TypeMismatchException &err) {
        LOGE("Type Mismatch Exception");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSSensorService::getAvailableSensor(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        LOGD("enter");
        return SensorService::getInstance()->getAvailableSensor(GlobalContextManager::getInstance()->getGlobalContext(context));
    } catch (const TypeMismatchException &err) {
        LOGE("Type Mismatch Exception");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

// Proximity
JSClassDefinition JSProximitySensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "ProximitySensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticValue JSProximitySensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};


JSStaticFunction JSProximitySensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_PROXIMITY_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSProximitySensor::m_jsClassRef = JSClassCreate(JSProximitySensor::getClassInfo());

const JSClassRef JSProximitySensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSProximitySensor::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSProximitySensor::createJSObject(JSContextRef context, ProximitySensor* proximitySensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(proximitySensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSProximitySensor::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSProximitySensor::finalize(JSObjectRef object)
{
    LOGD("enter");
}

JSValueRef JSProximitySensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

    if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("PROXIMITY");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSProximitySensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        ProximitySuccessCallbackPtr proximityCB(new ProximitySuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            proximityCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            proximityCB->setErrorCallback(errorCB);
        }

        ProximitySensor::getInstance()->start(proximityCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSProximitySensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ProximitySensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSProximitySensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        ProximitySuccessCallbackPtr proximityCB(new ProximitySuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            proximityCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            proximityCB->setErrorCallback(errorCB);
        }

        ProximitySensor::getInstance()->setChangeListener(proximityCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }



}

JSValueRef JSProximitySensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ProximitySensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}

JSValueRef JSProximitySensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        ProximitySuccessCallbackPtr proximityCB(new ProximitySuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            proximityCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            proximityCB->setErrorCallback(errorCB);
        }

        ProximitySensor::getInstance()->getSensorData(proximityCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

// Light
JSClassDefinition JSLightSensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "LightSensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticFunction JSLightSensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_LIGHT_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSStaticValue JSLightSensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSLightSensor::m_jsClassRef = JSClassCreate(JSLightSensor::getClassInfo());

const JSClassRef JSLightSensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSLightSensor::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSLightSensor::createJSObject(JSContextRef context, LightSensor* sensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(sensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSLightSensor::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSLightSensor::finalize(JSObjectRef object)
{
    LOGD("enter");
}

JSValueRef JSLightSensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

    if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("LIGHT");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}


JSValueRef JSLightSensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        LightSuccessCallbackPtr lightCB(new LightSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            lightCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            lightCB->setErrorCallback(errorCB);
        }

        LightSensor::getInstance()->start(lightCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSLightSensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        LightSensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSLightSensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        LightSuccessCallbackPtr lightCB(new LightSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            lightCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            lightCB->setErrorCallback(errorCB);
        }

        LightSensor::getInstance()->setChangeListener(lightCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }



}

JSValueRef JSLightSensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        LightSensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSLightSensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        LightSuccessCallbackPtr lightCB(new LightSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            lightCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            lightCB->setErrorCallback(errorCB);
        }

        LightSensor::getInstance()->getSensorData(lightCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

// Magnetic
JSClassDefinition JSMagneticSensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "MagneticSensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticFunction JSMagneticSensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_MAGNETIC_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSStaticValue JSMagneticSensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};


JSClassRef JSMagneticSensor::m_jsClassRef = JSClassCreate(JSMagneticSensor::getClassInfo());

const JSClassRef JSMagneticSensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSMagneticSensor::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSMagneticSensor::createJSObject(JSContextRef context, MagneticSensor* sensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(sensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSMagneticSensor::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSMagneticSensor::finalize(JSObjectRef object)
{
    LOGD("enter");
}


JSValueRef JSMagneticSensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

    if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("MAGNETIC");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSMagneticSensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        MagneticSuccessCallbackPtr magneticCB(new MagneticSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            magneticCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            magneticCB->setErrorCallback(errorCB);
        }

        MagneticSensor::getInstance()->start(magneticCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSMagneticSensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        MagneticSensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSMagneticSensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        MagneticSuccessCallbackPtr magneticCB(new MagneticSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            magneticCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            magneticCB->setErrorCallback(errorCB);
        }

        MagneticSensor::getInstance()->setChangeListener(magneticCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSMagneticSensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        MagneticSensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}

JSValueRef JSMagneticSensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        MagneticSuccessCallbackPtr magneticCB(new MagneticSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            magneticCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            magneticCB->setErrorCallback(errorCB);
        }

        MagneticSensor::getInstance()->getSensorData(magneticCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}

// Pressure Sensor
JSClassDefinition JSPressureSensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PressureSensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticFunction JSPressureSensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_PRESSURE_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSStaticValue JSPressureSensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};


JSClassRef JSPressureSensor::m_jsClassRef = JSClassCreate(JSPressureSensor::getClassInfo());

const JSClassRef JSPressureSensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPressureSensor::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSPressureSensor::createJSObject(JSContextRef context, PressureSensor* sensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(sensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSPressureSensor::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSPressureSensor::finalize(JSObjectRef object)
{
    LOGD("enter");
}


JSValueRef JSPressureSensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

    if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("PRESSURE");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSPressureSensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        PressureSuccessCallbackPtr pressureCB(new PressureSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            pressureCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            pressureCB->setErrorCallback(errorCB);
        }

        PressureSensor::getInstance()->start(pressureCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSPressureSensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        PressureSensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSPressureSensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        PressureSuccessCallbackPtr pressureCB(new PressureSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            pressureCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            pressureCB->setErrorCallback(errorCB);
        }

        PressureSensor::getInstance()->setChangeListener(pressureCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSPressureSensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        PressureSensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSPressureSensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");
    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        PressureSuccessCallbackPtr pressureCB(new PressureSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            pressureCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            pressureCB->setErrorCallback(errorCB);
        }

        PressureSensor::getInstance()->getSensorData(pressureCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

//JSUltravioletSensor
JSClassDefinition JSUltravioletSensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "UltravioletSensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticValue JSUltravioletSensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSStaticFunction JSUltravioletSensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_ULTRAVIOLET_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSUltravioletSensor::m_jsClassRef = JSClassCreate(JSUltravioletSensor::getClassInfo());

const JSClassRef JSUltravioletSensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSUltravioletSensor::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSUltravioletSensor::createJSObject(JSContextRef context, UltravioletSensor* sensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(sensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}

void JSUltravioletSensor::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
}

void JSUltravioletSensor::finalize(JSObjectRef object)
{
    LOGD("enter");
}

JSValueRef JSUltravioletSensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

    if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("ULTRAVIOLET");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSUltravioletSensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        UltravioletSuccessCallbackPtr ultravioletCB(new UltravioletSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            ultravioletCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            ultravioletCB->setErrorCallback(errorCB);
        }

        UltravioletSensor::getInstance()->start(ultravioletCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSUltravioletSensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    try{
        UltravioletSensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSUltravioletSensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        UltravioletSuccessCallbackPtr ultravioletCB(new UltravioletSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            ultravioletCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            ultravioletCB->setErrorCallback(errorCB);
        }

        UltravioletSensor::getInstance()->setChangeListener(ultravioletCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSUltravioletSensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        UltravioletSensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}


JSValueRef JSUltravioletSensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        UltravioletSuccessCallbackPtr ultravioletCB(new UltravioletSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            ultravioletCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            ultravioletCB->setErrorCallback(errorCB);
        }

        UltravioletSensor::getInstance()->getSensorData(ultravioletCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}


JSClassDefinition JSHRMRawSensor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "HRMRawSensor",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticValue JSHRMRawSensor::m_property[] = {
    { SENSOR_ATTRIBUTE_SENSOR_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};



JSStaticFunction JSHRMRawSensor::m_function[] = {
    { SENSOR_API_START, start, kJSPropertyAttributeNone },
    { SENSOR_API_STOP, stop, kJSPropertyAttributeNone },
    { SENSOR_API_SET_CHANGE_LISTENER, setChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_UNSET_CHANGE_LISTENER, unsetChangeListener, kJSPropertyAttributeNone },
    { SENSOR_API_GET_HRMRAW_SENSOR_DATA, getSensorData, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSHRMRawSensor::m_jsClassRef = JSClassCreate(JSHRMRawSensor::getClassInfo());

const JSClassRef JSHRMRawSensor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSHRMRawSensor::getClassInfo()
{
    return &m_classInfo;
}


JSValueRef JSHRMRawSensor::createJSObject(JSContextRef context, HRMRawSensor* sensor)
{
    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(sensor));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}


void JSHRMRawSensor::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSHRMRawSensor::finalize(JSObjectRef object)
{

}

JSValueRef JSHRMRawSensor::getProperty(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
{
    std::string sensorType("");
    LOGD("enter");

     if (JSStringIsEqualToUTF8CString(propertyName, SENSOR_ATTRIBUTE_SENSOR_TYPE)) {
        sensorType.append("HRM_RAW");
        return JSUtil::toJSValueRef(context, sensorType);
    }
    return JSValueMakeUndefined(context);
}


JSValueRef JSHRMRawSensor::start(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        HRMRawSuccessCallbackPtr HRMRawCB(new HRMRawSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            HRMRawCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            HRMRawCB->setErrorCallback(errorCB);
        }

        HRMRawSensor::getInstance()->start(HRMRawCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGD("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSHRMRawSensor::stop(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        HRMRawSensor::getInstance()->stop(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGD("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSHRMRawSensor::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        HRMRawSuccessCallbackPtr HRMRawCB(new HRMRawSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            HRMRawCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            HRMRawCB->setErrorCallback(errorCB);
        }

        HRMRawSensor::getInstance()->setChangeListener(HRMRawCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGD("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}



JSValueRef JSHRMRawSensor::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try{
        HRMRawSensor::getInstance()->unsetChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context));
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGD("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


}


JSValueRef JSHRMRawSensor::getSensorData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{

    LOGD("enter");

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCB = validator.toFunction(0);

        HRMRawSuccessCallbackPtr HRMRawCB(new HRMRawSuccessCallback(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if (successCB) {
            HRMRawCB->setSuccessCallback(successCB);
        }

        JSObjectRef errorCB = validator.toFunction(1, true);
        if(errorCB) {
            HRMRawCB->setErrorCallback(errorCB);
        }

        HRMRawSensor::getInstance()->getSensorData(HRMRawCB);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGD("argument is wrong.");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}


} // Sensor
} // DeviceAPI
