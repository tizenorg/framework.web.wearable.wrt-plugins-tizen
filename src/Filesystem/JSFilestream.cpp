//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

/**
 * @file        JSFilestream.cpp
 */

#include "JSFilestream.h"

#include <Commons/Base64.h>
#include <CommonsJavaScript/JSUtils.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <JSUtil.h>
#include <TimeTracer.h>
#include <Logger.h>
#include <ArgumentValidator.h>
#include <Export.h>

#include "Converter.h"
#include "Encodings.h"
#include "plugin_config.h"
#include "FilesystemUtils.h"
#include "JSErrors.h"
#include <Commons/Exception.h>


using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace {
const char* PLUGIN_NAME = "FileStream";
const char* PROPERTY_EOF = "eof";
const char* PROPERTY_POSITION = "position";
const char* PROPERTY_BYTES_AVAILABLE = "bytesAvailable";
}

namespace DeviceAPI {
namespace Filesystem {

using namespace DeviceAPI::Common;

JSClassRef JSFilestream::m_classRef = 0;

JSClassDefinition JSFilestream::m_classInfo = {
    0,
    kJSClassAttributeNone,
    PLUGIN_NAME,
    0,
    m_properties,
    m_functions,
    initialize,
    finalize,
    NULL,
    NULL,
    NULL,
    NULL,
    getPropertyNames,
    NULL,
    NULL,
    hasInstance,
    NULL
};

JSStaticValue JSFilestream::m_properties[] = {
    { PROPERTY_EOF, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_POSITION, getProperty, setProperty, kJSPropertyAttributeNone },
    { PROPERTY_BYTES_AVAILABLE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSFilestream::m_functions[] = {
    { "close", close, kJSPropertyAttributeNone },
    { "read", read, kJSPropertyAttributeNone },
    { "readBytes", readBytes, kJSPropertyAttributeNone },
    { "readBase64", readBase64, kJSPropertyAttributeNone },
    { "write", write, kJSPropertyAttributeNone },
    { "writeBytes", writeBytes, kJSPropertyAttributeNone },
    { "writeBase64", writeBase64, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

void JSFilestream::initialize(JSContextRef context,
    JSObjectRef object)
{
}

void JSFilestream::finalize(JSObjectRef object)
{
    StreamHolder* priv = static_cast<StreamHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

const JSClassRef DLL_EXPORT JSFilestream::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

const JSClassDefinition* JSFilestream::getClassInfo()
{
    return &m_classInfo;
}

StreamPtr JSFilestream::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Object type do not match");
        throw TypeMismatchException("Object type is not JSFilestream");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    StreamHolder* priv = static_cast<StreamHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("NULL private data");
        throw UnknownException("Private data holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("NULL shared pointer in private data");
        throw UnknownException("Private data is null");
    }

    return priv->ptr;
}

void JSFilestream::setPrivateObject(JSObjectRef object, StreamPtr data)
{
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }

    StreamHolder* priv = static_cast<StreamHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new StreamHolder();
        priv->ptr = data;

        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set JSFilestream private datae");
            throw UnknownException("Failed to set JSFilestream private data");
        }
    }
}

JSObjectRef JSFilestream::makeJSObject(JSContextRef context, StreamPtr ptr,
        Common::SecurityAccessor* srcSecurityAccessor)
{
    if (!ptr) {
        LOGE("NULL pointer to File given");
        throw UnknownException("NULL pointer to File given");
    }

    StreamHolder* priv = new StreamHolder();
    priv->ptr = ptr;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    if(!JSObjectSetPrivate(obj, static_cast<void*>(priv))) {
        delete priv;
        LOGE("Failed to set private in JSFilestream");
        throw UnknownException("Private data not set");
    }

    if(srcSecurityAccessor) {
        ptr->copyAceCheckAccessFunction(srcSecurityAccessor);
    }
    else {
        LOGW("Source SecurityAccessor is not available!");
    }

    return obj;
}

JSValueRef JSFilestream::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, object);
        if (!priv) {
            LOGE("Private object is not set.");
            return JSValueMakeUndefined(context);
        }

        Converter converter(context);

        if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_EOF)) {
            return converter.toJSValueRef(priv->isEof());
        } else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_POSITION)) {
            long pos = priv->getPosition();
            if (pos < 0) {
                return JSValueMakeUndefined(context);
            }
            return converter.toJSValueRef(static_cast<unsigned long>(pos));
        } else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_BYTES_AVAILABLE)) {
            long position = priv->getPosition();
            long bytes = priv->getSize() - position;

            return converter.toJSValueRefLong(static_cast<long>(bytes));
        }
    } catch (const WrtDeviceApis::Commons::ConversionException& ex) {
        LOGW("trying to get incorrect value");
    } catch (const DeviceAPI::Common::BasePlatformException& ex) {
        LOGW("trying to get incorrect value");
    } catch(...) {
        LOGW("Unknown exception cought");
    }

    return JSValueMakeUndefined(context);
}

bool JSFilestream::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, object);
        if (!priv) {
            LOGE("Private object is not set.");
            return false;
        }

        Converter converter(context);

        if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_POSITION)) {
            priv->setPosition(converter.toLong(value));
            return true;
        }
    } catch (const WrtDeviceApis::Commons::Exception& ex) {
        LOGW("trying to set incorrect value");
    } catch (const DeviceAPI::Common::BasePlatformException& ex) {
        LOGW("trying to get incorrect value");
    }

    return false;
}

void JSFilestream::getPropertyNames(JSContextRef context,
        JSObjectRef object,
        JSPropertyNameAccumulatorRef propertyNames)
{
}

bool JSFilestream::hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef instance,
        JSValueRef* exception)
{
    return JSValueIsObjectOfClass(context, instance, JSFilestream::getClassRef());
}

JSValueRef JSFilestream::close(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_CLOSE);

        priv->close();
    } catch (WrtDeviceApis::Commons::PlatformException) {
        LOGW("PlatformException caught translated to UNKNOWN_ERROR sync");
        return JSWebAPIErrorFactory::postException(
                context,
                exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "Unknown error");
    } catch (const DeviceAPI::Common::BasePlatformException& ex) {
        LOGW("trying to get incorrect value");
        return JSWebAPIErrorFactory::postException(
                context,
                exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "Unknown error"
        );
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSFilestream::read(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    char* text = NULL;
    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_READ);

        ArgumentValidator validator(context, argc, argv);
        long count = validator.toLong(0, false, 0);
        if (count <= 0) {
            LOGE("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException("Invalid argument");
        }

        std::string currentCharSet = priv->getCharSet();

        const char* currentCharSet_c_str = currentCharSet.c_str();
        // utf8, iso8859-1, skip
        if (!strcmp(currentCharSet_c_str, Encodings::UTF8)
            || !strcmp(currentCharSet_c_str, Encodings::ISO88591)
            || !strcmp(currentCharSet_c_str, Encodings::SJIS))
        {
            text = priv->getChars(count);
            std::string str = std::string(text);
            delete[] text;
            return JSUtil::toJSValueRef(context,str);
        }
        else {
            std::string outStr = "";
            text = priv->getChars(count);
            Utils::toUTF8String(currentCharSet, text, count, outStr);
            delete[] text;
            return JSUtil::toJSValueRef(context, outStr);
        }
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        delete[] text;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        delete[] text;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFilestream::readBytes(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    unsigned char* data = NULL;
    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_READ_BYTES);

        long count = ArgumentValidator(context, argc, argv).toLong(0, false);
        if (count <= 0) {
            LOGE("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException("Invalid argument");
        }
        data = priv->getBytes(count);
        size_t objectCount = priv->getCount();
        auto to_ret = Converter(context).toJSValueRef(data, objectCount);
        delete [] data;
        return to_ret;
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        delete [] data;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        delete [] data;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFilestream::readBase64(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    unsigned char* data = NULL;

    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_READ_BASE64);
        long count = ArgumentValidator(context, argc, argv).toLong(0, false);
        if (count <= 0) {
            LOGE("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException("Invalid argument");
        }
        data = priv->getBytes(count);
        size_t objectCount = priv->getCount();
        std::string base64 =
            WrtDeviceApis::Commons::Base64::encode(data, objectCount);
        delete[] data;
        data = NULL;
        return JSUtil::toJSValueRef(context, base64);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        delete[] data;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        delete[] data;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFilestream::write(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argc,
    const JSValueRef argv[],
    JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_WRITE);
        priv->write(ArgumentValidator(context, argc, argv).toString(0, false));
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSFilestream::writeBytes(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_WRITE_BYTES);

        ArgumentValidator validator(context, argc, argv);

        JSObjectRef jsArrayRef = validator.toArrayObject(0);
        unsigned int arrSize = JSGetArrayLength(context, jsArrayRef);
        JSValueRef conversionException = NULL;
        unsigned char *arrData = new unsigned char[arrSize];
        try {
            for (int i=0; i<arrSize; i++) {
                JSValueRef element = JSGetArrayElement(context, jsArrayRef, i);
                arrData[i] = JSUtil::JSValueToOctet(context, element);
            }

            priv->write(arrData, arrSize);
            delete [] arrData;
        } catch (...) {
            delete [] arrData;
            throw;
        }

    } catch (const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }


    return JSValueMakeUndefined(context);
}

JSValueRef JSFilestream::writeBase64(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        StreamPtr priv = JSFilestream::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_WRITE_BASE64);

        ArgumentValidator validator(context, argc, argv);

        std::string base64 =
                WrtDeviceApis::Commons::Base64::decode(validator.toString(0, false));

        priv->write(base64);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGW("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::InvalidArgumentException& err) {
        LOGE("WrtDeviceApis::Commons::InvalidArgumentException caught");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::INVALID_VALUES_ERROR, err.GetMessage());
    } catch (...) {
        LOGW("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}
} // Tizen1_0
} // TizenApis

