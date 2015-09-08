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

/**
 * @file    JSExifManager.cpp
 */

#include "JSExifManager.h"

#include <ArgumentValidator.h>
#include <Export.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <Logger.h>
#include <SecurityExceptions.h>
#include <TimeTracer.h>

#include "ExifManager.h"
#include "ExifUtil.h"
#include "JSExifInformation.h"
#include "plugin_config.h"

namespace DeviceAPI {
namespace Exif {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace {
const char* TIZEN_INTERFACE_EXIF_MANAGER = "ExifManager";

const char* EXIF_FUNCTION_API_GET_EXIF_INFO = "getExifInfo";
const char* EXIF_FUNCTION_API_SAVE_EXIF_INFO = "saveExifInfo";
const char* EXIF_FUNCTION_API_GET_THUMBNAIL = "getThumbnail";
}

JSClassDefinition JSExifManager::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_INTERFACE_EXIF_MANAGER,
        NULL, //ParentClass
        NULL, //StaticValues
        m_function,
        initialize,
        finalize,
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

JSStaticFunction JSExifManager::m_function[] = {
        { EXIF_FUNCTION_API_GET_EXIF_INFO, getExifInfo,
                kJSPropertyAttributeNone },
        { EXIF_FUNCTION_API_SAVE_EXIF_INFO, saveExifInfo,
                kJSPropertyAttributeNone },
        { EXIF_FUNCTION_API_GET_THUMBNAIL, getThumbnail,
                        kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSClassRef JSExifManager::m_jsClassRef = JSClassCreate(JSExifManager::getClassInfo());

const JSClassRef DLL_EXPORT JSExifManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSExifManager::getClassInfo()
{
    return &m_classInfo;
}

void JSExifManager::initialize(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
    JSObjectSetPrivate(object, static_cast<void*>(&ExifManager::getInstance()));
}

void JSExifManager::finalize(JSObjectRef object)
{
}

JSValueRef JSExifManager::getExifInfo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(1);
    LOGD("Entered");

    ExifInfoCallbackData* callback = NULL;
    try {
        //void getExifInfo (DOMString uri,
        //                  ExifInformationSuccessCallback successCallback,
        //                  optional ErrorCallback? errorCallback)
        ArgumentValidator validator(context, argumentCount, arguments);
        const std::string uri = validator.toString(0);

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new (std::nothrow) ExifInfoCallbackData(g_ctx, uri);
        if (!callback) {
            LOGE("Couldn't allocate callback - ExifInfoCallbackData");
            throw Common::UnknownException("Memory allocation failed");
        }
        callback->setSuccessCallback(validator.toFunction(1));
        callback->setErrorCallback(validator.toFunction(2, true));

        ExifManager::getInstance().getExifInfo(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error, cannot get exif information");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSExifManager::saveExifInfo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(1);
    LOGD("Entered");

    ExifInfoCallbackData* callback = NULL;
    Common::CallbackUserData* invalidUriCallback = NULL;
	
    try {
        //void saveExifInfo(ExifInformation exifInfo,
        //                  optional SuccessCallback? successCallback,
        //                  optional ErrorCallback? errorCallback);
        ArgumentValidator validator(context, argumentCount, arguments);
        ExifInformationPtr exif_info = JSExifInformation::getPrivateObject(context,
                validator.toObject(0));

        if (!exif_info->isSet(EXIF_INFORMATION_ATTRIBUTE_URI)) {
            LOGE("URI is not set!");
            throw InvalidValuesException("URI in exifInfo is not set");
        }

	auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);

        if(!ExifUtil::isValidAbsoluteURI(exif_info->getUri())) {
	        LOGE("URI is not valid absolute URI: %s", exif_info->getUri().c_str());

	        invalidUriCallback = new Common::CallbackUserData(g_ctx);
	        if (!invalidUriCallback) {
	            LOGE("Couldn't allocate callback - ExifInfoCallbackData");
	            throw Common::UnknownException("Memory allocation failed");
	        }

		invalidUriCallback->setSuccessCallback(validator.toFunction(1, true));
		invalidUriCallback->setErrorCallback(validator.toFunction(2, true));
		ExifManager::getInstance().saveExifInfoFailed(invalidUriCallback);

        }else{
	        LOGE("URI is valid absolute URI: %s", exif_info->getUri().c_str());

	        callback = new (std::nothrow) ExifInfoCallbackData(g_ctx, exif_info->getUri());
	        if (!callback) {
	            LOGE("Couldn't allocate callback - ExifInfoCallbackData");
	            throw Common::UnknownException("Memory allocation failed");
	        }
	        callback->setExifInfo(exif_info);
		callback->setSuccessCallback(validator.toFunction(1, true));
		callback->setErrorCallback(validator.toFunction(2, true));
		ExifManager::getInstance().saveExifInfo(callback);
        }

    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error, cannot save exif information");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSExifManager::getThumbnail(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(1);
    LOGD("Entered");

    GetThumbnailCallbackUserData* callback = NULL;
    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        //void getThumbnail(DOMString uri,
        //                  ExifThumbnailSuccessCallback successCallback,
        //                  optional ErrorCallback? errorCallback);

        // contentURI
        const std::string uri = validator.toString(0);

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new (std::nothrow) GetThumbnailCallbackUserData(g_ctx, uri);
        if (!callback) {
            LOGE("Couldn't allocate callback - GetThumbnailCallbackUserData");
            throw Common::UnknownException("Memory allocation failed");
        }

        callback->setSuccessCallback(validator.toFunction(1));
        callback->setErrorCallback(validator.toFunction(2, true));

        ExifManager::getInstance().getThumbnail(callback);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown Error, cannot get thumbnail");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

} // Exif
} // DeviceAPI
